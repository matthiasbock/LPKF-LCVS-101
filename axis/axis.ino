//
// Arduino driver for LPKF 101 LC/VS milling machine X/Y axis
// using 5 Keyes L298N motor modules
//
// 5-phase stepper motor switching sequence implemented according to Berger Lahr datasheet page 6
//

/*
This is how the stepper motor cables are connected to the L298 modules (NO!):

stepper cable color
          module (A-E)
              label at module output pin

Y axis stepper motor:

yellow  = A_MotorA1;
white   = A_MotorA2;
red     = A_MotorB1;
blue    = A_MotorB2;
orange  = B_MotorA1;
green   = B_MotorA2;
black   = B_MotorB1;
gray    = B_MotorB2;
brown   = C_MotorA1;
violet  = C_MotorA2;


X axis stepper motor:

yellow  = C_MotorB1;
white   = C_MotorB2;
blue    = D_MotorA1;
red     = D_MotorA2;
green   = D_MotorB1;
orange  = D_MotorB2;
gray    = E_MotorA1;
black   = E_MotorA2;
violet  = E_MotorB1;
brown   = E_MotorB2;
*/

/*
 * Module pins
 */
#define ENA 0
#define IN1 1
#define IN2 2
#define IN3 3
#define IN4 4
#define ENB 5

/*
 * pins at which the five L298 modules are connected to the Arduino
 *
 * L298[module0-4][ENA, IN1, IN2, IN3, IN4, ENB]
 */
int L298[5][6] = {
                    {24,25,26,27,28,29},
                    {30,31,32,33,34,35},
                    {36,37,38,39,40,41},
                    {42,43,44,45,46,47},
                    {48,49,50,51,52,53}
                  };
// steppers
#define stepperY 0
#define stepperX 1

// stepper coils
#define W1 0
#define W2 1
#define W3 2
#define W4 3
#define W5 4

// Every L298 module has two output ports A and B
#define A 0
#define B 1

/*
 * Association of coils to module output ports
 *
 * CoilSetup[stepper][coil]
 *
 * e.g.
 * StepperX = CoilSetup[stepperX];
 * out = StepperX[W1];
 * out[0] equals module number
 * out[1] equals this module's output port (A or B)
 * out[2] equals this port's/coil's current magnetization
 */

#define N 1  // magnetic north pole
#define S -1 // magnetic south pole

int CoilSetup[2][5][3] = {
                          { {0,A,N}, {0,B,N}, {1,A,N}, {1,B,N}, {2,A,N} },
                          { {2,B,N}, {3,A,N}, {3,B,N}, {4,A,N}, {4,B,N} }
                         };

/*
 * Setter and getter for coil magnetization
 */

void setCoil(int stepper, int coil, int magnetization) {

  // 1 coil equals 1 output port of the 2 output ports of 1 L298 module
  
  /*
   * If A,B,C,D are the 4 output pins of an L298 module:
   * In order to set A to high and B to low, set IN1 to low and IN2 to high.
   * In order to set C to high and D to low, set IN3 to low and IN4 to high.
   */

  // which L298 module input pins do I need to control:
  int module = CoilSetup[stepper][coil][0], pin1, pin2;
  if (CoilSetup[stepper][coil][1] == A) {
    pin1 = L298[module][IN1];
    pin2 = L298[module][IN2];
  } else if (CoilSetup[stepper][coil][1] == B) {
    pin1 = L298[module][IN3];
    pin2 = L298[module][IN4];
  }

  /*
   * Note:
   * If the coil is W2 or W4 the plus and minus cables are switched.
   * That's just how they come out of the plug, so if the flatband cable
   * is connected to the L298 modules from left to right,
   * the software must do the polarity switch.
   */
  if (coil == W2 || coil == W4) {
    int cp = pin1;
    pin1 = pin2;
    pin2 = cp;
  }

  // set control pins of the selected module
  if (magnetization == 0) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  } else if (magnetization == N) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else if (magnetization == S) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  }
  // remember, what we set here
  CoilSetup[stepper][coil][2] = magnetization;
}

int getCoilMagnetization(int stepper, int coil) {

  // return the coil magnetization, we remembered from the invocation of setCoil
  return CoilSetup[stepper][coil][2];
}

/*
 * Switch the power supply for a stepper motor on or off
 */

#define ON HIGH
#define OFF LOW

void switchPower(int stepper, int level) {

  // switch power for all five coils of this stepper
  for (int coil=0; coil<5; coil++) {
    int module = CoilSetup[stepper][coil][0];
    if (CoilSetup[stepper][coil][1] == A) {
      digitalWrite( L298[module][ENA], level );
    } else if(CoilSetup[stepper][coil][1] == B) {
      digitalWrite( L298[module][ENB], level );
    }
  }
}

void initPin(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void initModulePins() {
  for (int i=0; i<5; i++)
    for (int j=0; j<6; j++)
      initPin( L298[i][j] );
}

/*
 * Initialize stepper motor
 *
 * set 4 coils to alternating magnetization, one neutral
 */
int Wnow[2] = {4,4};
void initStepper(int whichStepper) {
  switchPower( whichStepper, OFF );
  setCoil( whichStepper, W1, N );
  setCoil( whichStepper, W2, S );
  setCoil( whichStepper, W3, N );
  setCoil( whichStepper, W4, S );
  setCoil( whichStepper, W5, 0 );
  switchPower( whichStepper, ON );
  delay(1);
  switchPower( whichStepper, OFF );
  Wnow[whichStepper] = 4;
}

#define FORWARD 1
#define REVERSE -1

#define HALF 1
#define FULL 2

void setup() {
  // Computer interface
  Serial.begin(9600);
  // LED
  initPin(13);
  // end of slide detection
/*  pinMode(Xmin, INPUT);
  pinMode(Xmax, INPUT);
  pinMode(Ymin, INPUT);
  pinMode(Ymax, INPUT); */
  // stepper control
  initModulePins();
  initStepper(stepperX);
  initStepper(stepperY);
}

/*
 * Proceed by number of steps on specified axis in specified direction
 */

int stepWait = 150;
void step(int stepper, int steps, int rotate, int stepping_mode) {

  String cmd = "step("; // cmd needs initial value before concatenation: http://arduino.cc/en/Tutorial/StringAdditionOperator
  Serial.println(cmd+stepper+", "+steps+", "+rotate+", "+stepping_mode+");");

  int Wnext;
  for (int i=0; i<steps; i++) {

    if (rotate == FORWARD)
      Wnext = (Wnow[stepper]+1) % 5;
    else if (rotate == REVERSE)
      Wnext = (Wnow[stepper]-1+5) % 5;
    else {
      Serial.println("Error: Invalid rotation direction.");
      break;
    }
    
//    String arrow = " -> ";
//    Serial.println(Wnow[stepper]+arrow+Wnext);
    
    // Full-step mode
    if (stepping_mode == FULL) {
      // magnetize Wnow like Wnext
      setCoil(stepper, Wnow[stepper], getCoilMagnetization(stepper, Wnext));
      // demagnetize Wnext
      setCoil(stepper, Wnext, 0);
      // proceed with next coil
      Wnow[stepper] = Wnext;
    }
    
    // Half-step mode: full-step mode divided into two separate steps
    else if (stepping_mode == HALF) {
      // if Wnow is demagnetized
      if (getCoilMagnetization(stepper, Wnow[stepper]) == 0) {
        // magnetize Wnow like Wnext
        int mag = getCoilMagnetization(stepper, Wnext);
        if (mag == 0) mag = N; // cannot be 0, else we would get stuck here
        setCoil(stepper, Wnow[stepper], mag);
      } else {
        setCoil(stepper, Wnext, 0);
        // proceed with next coil
        Wnow[stepper] = Wnext;
      }
    } else {
      Serial.println("Error: Invalid stepping mode.");
      break;
    }
    
    // wait between each step
    switchPower(stepper, ON);
    delayMicroseconds(stepWait);
  }
switchPower(stepper, OFF);
}

/*
 * Just drive a little back and forth
 */
void testBackAndForth() {

  int wait = 200;

  step(stepperX, 6000, FORWARD, HALF);
  delay(wait);
  step(stepperX, 6000, REVERSE, HALF);
  delay(wait);
  step(stepperY, 6000, FORWARD, HALF);
  delay(wait);
  step(stepperY, 6000, REVERSE, HALF);
  delay(wait);
/*
  step(stepperX, 6000, FORWARD, FULL);
  delay(wait);
  step(stepperX, 6000, REVERSE, FULL);
  delay(wait);
  step(stepperY, 6000, FORWARD, FULL);
  delay(wait);
  step(stepperY, 6000, REVERSE, FULL);
  delay(wait);*/
}

void testModuleLEDs() {
  switchPower(stepperX, ON);
  switchPower(stepperY, ON);
  for (int stepper=0; stepper<2; stepper++)
    for (int coil=0; coil<5; coil++) {
      setCoil(stepper, coil, N);
      delay(800);
      setCoil(stepper, coil, 0);
    }
}

void loop() {
  testBackAndForth();
//  testModuleLEDs();
}


#define led 13
void blink(int n) {
  for (int i=0; i<n; i++) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }
}

/*
 * Four mechanical switches are present in the machine
 * to detect when the head has reached the end of an axis.
 * In their unpressed state the switches are conducting.
 * They become resistant (2K) when pressed.
 * All four switches have a common 3.3V voltage supply pin.
 * The four switch pins should be connected to a pull-down resistor.
 */

// axis end detecting switches
#define Xmin 5
#define Xmax 4
#define Ymin 3
#define Ymax 2

// test min/max switches
void testMinMaxSwitches() {
  int level = LOW;
  if (digitalRead(Xmin) == level) {
    Serial.println("X axis reached min");
    blink(1);
  }
  else if (digitalRead(Xmax) == level) {
    Serial.println("X axis reached max");
    blink(2);
  }
  else if (digitalRead(Ymin) == level) {
    Serial.println("Y axis reached min");
    blink(3);
  }
  else if (digitalRead(Ymax) == level) {
    Serial.println("Y axis reached max");
    blink(4);
  }
}

void testGCode() {
  /*
   * Read a G-code command from serial port
   */
  String cmd = "";
  char c = ' ';
  while (c != 10) {
    while (Serial.available() < 1)
      delay(100);
    char c = Serial.read();
    cmd += c;
  }

  if (cmd.substring(0,4) != "G01 "){
    Serial.println("Unrecognized command.");
    return;
  }
  // 4 is 'X'
  String x = cmd.substring(5, 7);
  if (cmd[5] == '-')
    x = cmd.substring(5, 8);
  
  Serial.println("'"+x+"'");
/*
    if (&c == "X") {
      moveRight(1000);
    }
    else if (c == 'x') {
      moveLeft(1000);
    }
    else if (c == 'y') {
      moveDown(1000);
    }
    else if (c == 'Y') {
      moveUp(1000);
    }
*/
}


