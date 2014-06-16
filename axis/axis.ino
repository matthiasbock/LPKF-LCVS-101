//
// Arduino driver for LPKF 101 LC/VS milling machine X/Y axis
// using 5 Keyes L298N motor modules
//
// 5-phase stepper motor switching sequence implemented according to Berger Lahr datasheet page 6
//

/*
 * The wires of the LPKF cable are connected to the L298 module outputs
 * from left to right without twists
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

// axis end detecting switch pins
#define SWITCH_X_NEAR_MOTOR      17
#define SWITCH_X_FAR_FROM_MOTOR  16
#define SWITCH_Y_NEAR_MOTOR      14
#define SWITCH_Y_FAR_FROM_MOTOR  15

boolean isMin(int stepper) {
  switch (stepper) {
    case stepperX:
      return digitalRead(SWITCH_X_NEAR_MOTOR)==LOW;
    case stepperY:
      return digitalRead(SWITCH_Y_NEAR_MOTOR)==LOW;
  }
  return true;
}

boolean isMax(int stepper) {
  switch (stepper) {
    case stepperX:
      return digitalRead(SWITCH_X_FAR_FROM_MOTOR)==LOW;
    case stepperY:
      return digitalRead(SWITCH_Y_FAR_FROM_MOTOR)==LOW;
  }
  return true;
}

/*
 * true: everything seems in order
 * false: there is a problem
 */
boolean checkSwitches() {
  // the two switches of one axis cannot be switched at the same time
  if (isMin(stepperX) && isMax(stepperX))
    return false;
  else if (isMin(stepperY) && isMax(stepperY))
    return false;
  else
    return true;
}

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

/*
 * return the coil magnetization,
 * we remembered from the invocation of setCoil
 */
int getCoilMagnetization(int stepper, int coil) {
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

/*
 * Disable a stepper for a while
 * without setting the L298s to brake
 */
void stepperIdle(int stepper, int wait) {
  int cache[5] = {0,0,0,0,0};
  for (int coil=W1; coil<=W5; coil++) {
    cache[coil] = getCoilMagnetization(stepper,coil);
    setCoil(stepper,coil,0);
  }
  delayMicroseconds(wait);
  for (int coil=W1; coil<=W5; coil++) {
    setCoil(stepper,coil,cache[coil]);
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
  delayMicroseconds(200);
  switchPower( whichStepper, OFF );
  Wnow[whichStepper] = 4;
}

#define TOWARDS_MOTOR 1
#define AWAY_FROM_MOTOR -1
// OFF equals 0

// stepping modes
#define HALF 1
#define FULL 2

/*
 * Geschwindigkeitsrampe fahren
 */

int stepWait(long current_step, long total_steps, int ramp_min, int ramp_max) {
  int default_delay = ramp_min;
  int additional_delay = ramp_max-ramp_min;
  int inclination_steps = 500;
  
  if (total_steps > inclination_steps) {
    if (current_step < inclination_steps) {
      return default_delay + int((1-(float(current_step)/inclination_steps)) * additional_delay);
    }
    else if (current_step > total_steps-inclination_steps) {
      return default_delay + int((1-(float(total_steps-current_step)/inclination_steps)) * additional_delay);
    }
    else
      return default_delay;
  }
  else
    return default_delay;
}

/*
 * Proceed by number of steps on specified axis in specified direction
 */
void step(int stepper, long steps, int rotate, int stepping_mode, boolean ramp) {

  // debug
  String cmd = "step("; // cmd needs initial value before concatenation: http://arduino.cc/en/Tutorial/StringAdditionOperator
  Serial.println(cmd+stepper+", "+steps+", "+rotate+", "+stepping_mode+");");

  if (!checkSwitches()) {
    Serial.println("Aborting: Min/max switch defect.");
    return;
  }

  int power = OFF;
  int Wnext;
  for (long i=0; i<steps; i++) {

    // make sure, we haven't reached the end of the axis
    if ( ((rotate==TOWARDS_MOTOR) && isMin(stepper)) || ((rotate==AWAY_FROM_MOTOR) && isMax(stepper)) ) {
      Serial.println("Aborting: Reached end of axis. Steps made: "+i);
      break;
    } else {

      // only switch no power, if not reached end of axis
      if (power == OFF) {
        switchPower(stepper, ON);
        power = ON;
        delayMicroseconds(100);
      }
      
      // select next coil according to rotation direction
      if (rotate == TOWARDS_MOTOR)
        Wnext = (Wnow[stepper]+1) % 5;
      else if (rotate == AWAY_FROM_MOTOR)
        Wnext = (Wnow[stepper]-1+5) % 5;
      else {
        Serial.println("Error: Invalid rotation direction.");
        break;
      }

      // debug coil selection  
//    String arrow = " -> ";
//    Serial.println(Wnow[stepper]+arrow+Wnext);
      
      // Full-step mode
      if (stepping_mode == FULL) {
        /*
         * In full step mode, in every step exactly one coil is demagnetized.
         * Wnow equals the coil, which currently is demagnetized.
         */
        
        // are we in a half step position?
        if (getCoilMagnetization(stepper, Wnow[stepper]) != 0) {
          /*
           * We cannot operate full step rotation from a half step position,
           * so we need to perform a half step first.
           */
          setCoil(stepper, Wnow[stepper], 0);
        } else {
          // we are in proper full step position

          // magnetize Wnow like Wnext
          setCoil(stepper, Wnow[stepper], getCoilMagnetization(stepper, Wnext));
          // demagnetize Wnext
          setCoil(stepper, Wnext, 0);
          // proceed with next coil
          Wnow[stepper] = Wnext;
        }
      }
      
      // Half-step mode: full-step mode divided into two separate steps
      else if (stepping_mode == HALF) {
        // if Wnow is demagnetized
        if (getCoilMagnetization(stepper, Wnow[stepper]) == 0) {
          // magnetize Wnow like Wnext
          int mag = getCoilMagnetization(stepper, Wnext);
          if (mag == 0) mag = N; // must not be 0, else we would get stuck here
          setCoil(stepper, Wnow[stepper], mag);
        } else {
          setCoil(stepper, Wnext, 0);
          // proceed with next coil
          Wnow[stepper] = Wnext;
        }
      }
      
      // everything else is an error
      else {
        Serial.println("Error: Invalid stepping mode.");
        break;
      }
      
      // wait between each step
      if (ramp)
        delayMicroseconds(stepWait(i,steps,200,500));
      else
        delayMicroseconds(200);
    }
  }
  // if we switched on the power, switch it back off
  if (power == ON)
    switchPower(stepper, OFF);
}

/*
 * Just drive a little back and forth
 */
void testBackAndForth() {

  int wait = 1000;
  boolean ramp = true;

  step(stepperX, 6000, TOWARDS_MOTOR,   HALF, ramp);
  delay(wait);
  step(stepperX, 6000, AWAY_FROM_MOTOR, HALF, ramp);
  delay(wait);
  step(stepperY, 6000, TOWARDS_MOTOR,   HALF, ramp);
  delay(wait);
  step(stepperY, 6000, AWAY_FROM_MOTOR, HALF, ramp);
  delay(wait);

  step(stepperX, 6000, TOWARDS_MOTOR,   FULL, ramp);
  delay(wait);
  step(stepperX, 6000, AWAY_FROM_MOTOR, FULL, ramp);
  delay(wait);
  step(stepperY, 6000, TOWARDS_MOTOR,   FULL, ramp);
  delay(wait);
  step(stepperY, 6000, AWAY_FROM_MOTOR, FULL, ramp);
  delay(wait);
  
}

/*
 * Wrap stepping into a more convenient function
 */

#define MOVE_LEFT  10 // arbitrary; avoid collision with other constants
#define MOVE_RIGHT 11
#define MOVE_UP    12
#define MOVE_DOWN  13

void move(int where, long steps, int mode, boolean ramp) {
  switch (where) {
    case MOVE_LEFT:
      step(stepperX, steps, TOWARDS_MOTOR,   mode, ramp);
      break;
    case MOVE_RIGHT:
      step(stepperX, steps, AWAY_FROM_MOTOR, mode, ramp);
      break;
    case MOVE_UP:
      step(stepperY, steps, TOWARDS_MOTOR,   mode, ramp);
      break;
    case MOVE_DOWN:
      step(stepperY, steps, AWAY_FROM_MOTOR, mode, ramp);
      break;
  }
}

/*
 * Are the L298 module LEDs lighting up
 * in the desired fashion ?
 */
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

/*
 * Four mechano-electrical switches are present in the machine
 * to detect when a head has reached the end of an axis.
 *
 * In their unpressed state the switches are conducting.
 * They become resistant (2K) when pressed.
 * This way is more clever, than the other way around,
 * because this way the controller has a means of detecting problems
 * with the switches and can protect the machine from damage
 * (No more than one switch per axis can
 * be low at a time, else they are not functioning properly).
 *
 * All four switches have a common 3.3V voltage supply pin.
 * The four switch pins should be connected to a pull-down resistor.
 */

// test min/max switches
void testMinMaxSwitches() {
  if (isMin(stepperX)) {
    Serial.println("X axis reached min");
  } else if (isMax(stepperX)) {
    Serial.println("X axis reached max");
  } else if (isMin(stepperY)) {
    Serial.println("Y axis reached min");
  } else if (isMax(stepperY)) {
    Serial.println("Y axis reached max");
  }
}

void setup() {
  // Computer interface
  Serial.begin(9600);
  // LED
  initPin(13);
  // end of slide detection
  pinMode(SWITCH_X_NEAR_MOTOR, INPUT);
  pinMode(SWITCH_X_FAR_FROM_MOTOR, INPUT);
  pinMode(SWITCH_Y_NEAR_MOTOR, INPUT);
  pinMode(SWITCH_Y_FAR_FROM_MOTOR, INPUT);
  // stepper control
  initModulePins();
  initStepper(stepperX);
  initStepper(stepperY);
}

String readln() {
  String result = "";
  int c;
  while (true) {
    while (Serial.available() == 0)
      delay(1);
    c = Serial.read();
    Serial.print(char(c));
    if (c == 10 || c == 13)
      return result;
    else
      result += char(c);
  }
}

int gcode_mode, gcode_direction;
long gcode_steps;
String param;

void parseInstruction(String cmd) {
  if (cmd=="G00") {
    gcode_mode = FULL;
    Serial.println("FULL step mode");
  }
  else if (cmd=="G01") {
    gcode_mode = HALF;
    Serial.println("HALF step mode");
  }
  else if (cmd.charAt(0)=='X') {
    param = cmd.substring(1); // except X
    if (param.charAt(0) == '-') {
      gcode_direction = MOVE_LEFT;
      param = param.substring(1); // except minus
      Serial.println(param+" steps to the left");
    } else {
      gcode_direction = MOVE_RIGHT;
      Serial.println(param+" steps to the right");
    }
    gcode_steps = param.toInt();
  }
  else if (cmd.charAt(0)=='Y') {
    param = cmd.substring(1); // except Y
    if (param.charAt(0) == '-') {
      gcode_direction = MOVE_UP;
      param = param.substring(1); // except minus
      Serial.println(param+" steps up");
    } else {
      gcode_direction = MOVE_DOWN;
      Serial.println(param+" steps down");
    }
    gcode_steps = param.toInt();
  }
}

void GCodeConsole() {
  /*
   * Read a G-code command from serial port
   */
  Serial.print("g-code@cnc:/>");
  String line = readln();
  Serial.println(line);

  /*
   * G00: rapid motion (full step mode)
   * G01: precise motion (half step mode)
   */

  // G00 X1000 Y0

  String cmd[4] = {"","","",""};
  int pos, i=0;
  while (line.indexOf(" ") > -1) {
    pos = line.indexOf(" ");
    cmd[i] = line.substring(0,pos);
    i++;
    line = line.substring(pos+1);
  }
  cmd[i] = line;

  if (cmd[0]=="G00" || cmd[0]=="G01") {
    for (int i=0; i<4; i++)
      parseInstruction(cmd[i]);
    move(gcode_direction, gcode_steps, gcode_mode, true);
  } else 
    Serial.println("Unknown command.");
}

void loop() {
  GCodeConsole();
}

