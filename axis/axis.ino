//
// Arduino driver for LPKF 101 LC/VS milling machine X/Y axis
// using Keyes L298N motor modules/shields
//

/*
Y axis stepper motor:

yellow  = A_MotorA1;
white   = A_MotorA2;
blue    = A_MotorB1;
red     = A_MotorB2;
orange  = B_MotorA1;
green   = B_MotorA2;
gray    = B_MotorB1;
black   = B_MotorB2;
brown   = C_MotorA1;
violet  = C_MotorA2;


X axis stepper motor:

yellow  = C_MotorB1;
white   = C_MotorB2;
blue    = D_MotorA1;
red     = D_MotorA2;
orange  = D_MotorB1;
green   = D_MotorB2;
gray    = E_MotorA1;
black   = E_MotorA2;
brown   = E_MotorB1;
violet  = E_MotorB2;

In order for A to be high (B low), pin1 is low, pin2 high.
For C to be high (D low), pin3 is low, pin4 high.
*/


#define ENA 0
   #define IN1 1
      #define IN2 2
         #define IN3 3
            #define IN4 4
               #define ENB 5

// pins at which the five L298 modules are connected to the Arduino
int A[6] = {24,25,26,27,28,29};
int B[6] = {30,31,32,33,34,35};
int C[6] = {36,37,38,39,40,41};
int D[6] = {42,43,44,45,46,47};
int E[6] = {48,49,50,51,52,53};

// axis end detecting switches
#define Xmin 5
#define Xmax 4
#define Ymin 3
#define Ymax 2

void initPin(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void initModule(int m[6]) {
  for (int i=0; i<6; i++)
    initPin(m[i]);
}

void setup() {

  Serial.begin(9600);
 
  initPin(13);
  pinMode(Xmin, INPUT);
  pinMode(Xmax, INPUT);
  pinMode(Ymin, INPUT);
  pinMode(Ymax, INPUT);
  initModule(A);
  initModule(B);
  initModule(C);
  initModule(D);
  initModule(E);
}

// enable power for Y axis motor control pins
void switchYon() {
  digitalWrite(A[ENA], HIGH);
  digitalWrite(A[ENB], HIGH);
  digitalWrite(B[ENA], HIGH);
  digitalWrite(B[ENB], HIGH);
  digitalWrite(C[ENA], HIGH);
}

// disable power for Y axis motor control pins
void switchYoff() {
  // also switch all LEDs off to show shutdown
  initModule(A);
  initModule(B);
  digitalWrite(C[ENA], LOW);
  digitalWrite(C[IN1], LOW);
  digitalWrite(C[IN2], LOW);
}

// enable power for X axis motor control pins
void switchXon() {
  digitalWrite(C[ENB], HIGH);
  digitalWrite(D[ENA], HIGH);
  digitalWrite(D[ENB], HIGH);
  digitalWrite(E[ENA], HIGH);
  digitalWrite(E[ENB], HIGH);
}

// disable power for X axis motor control pins
void switchXoff() {
  // also switch all LEDs off to show shutdown
  digitalWrite(C[IN3], LOW);
  digitalWrite(C[IN4], LOW);
  digitalWrite(C[ENB], LOW);
  initModule(D);
  initModule(E);
}

/*
  A five phase stepper motor consists of five coils W1 to W5.
  The following functions implement the half-step timing procedure
  found in Berger-Lahr's 5-phase stepping motor datasheet.

  pin1 is the number of the Pin, that needs to be high, for the first wire of a pair to be + pole.
  pin2 accordingly is the Pin, that needs to be high, for the second wire to be + pole.
  lowDisable is the lower step number, where the level of a wire changes.
  highDisable is the higher step number, where wire levels change.

  If highDisable < lowDisable, then in the step range between lowDisable and highDisable
  the first wire is enabled instead of disabled.
*/

void setPhase(int step, int in1, int in2, int lowDisable, int highDisable, boolean inbetweenEnabled) {
  if ((step == lowDisable) || (step == highDisable)) { // disable
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  else if (not inbetweenEnabled) {
	   if ((step > lowDisable) && (step < highDisable)) { // A low, B high
	    // switch the one off before switching the other on
	    digitalWrite(in2, LOW);
	    digitalWrite(in1, HIGH);
	  }
	  else { // A high, B low
	    digitalWrite(in1, LOW);
	    digitalWrite(in2, HIGH);
	  }
  } else { // highDisable < lowDisable
	   if ((step > lowDisable) && (step < highDisable)) { // C high, D low
	    digitalWrite(in1, LOW);
	    digitalWrite(in2, HIGH);
	  }
	  else { // C low, D high
	    digitalWrite(in2, LOW);
	    digitalWrite(in1, HIGH);
	  }
       }
}

void updateY(int stepY) {
  setPhase( stepY, A[IN1], A[IN2], 2, 12, false );
  setPhase( stepY, A[IN3], A[IN4], 4, 14, true );
  setPhase( stepY, B[IN1], B[IN2], 6, 16, false );
  setPhase( stepY, B[IN3], B[IN4], 8, 18, true );
  setPhase( stepY, C[IN1], C[IN2], 10, 20, false );  
}

void updateX(int stepX) {
  setPhase( stepX, C[IN3], C[IN4], 2, 12, false );
  setPhase( stepX, D[IN1], D[IN2], 4, 14, true );
  setPhase( stepX, D[IN3], D[IN4], 6, 16, false );
  setPhase( stepX, E[IN1], E[IN2], 8, 18, true );
  setPhase( stepX, E[IN3], E[IN4], 10, 20, false );
}

int cyclePos = 1;

void moveRight(int steps, int wait) {
  switchXon();
  for (int i=0; i<steps; i++) {
    updateX(cyclePos);
    delayMicroseconds(wait);
    cyclePos = (cyclePos % 20) + 1;
  }
  switchXoff();
}

void moveUp(int steps, int wait) {
  switchYon();
  for (int i=0; i<steps; i++) {
    updateY(cyclePos);
    delayMicroseconds(wait);
    cyclePos = (cyclePos % 20) + 1;
  }
  switchYoff();
}

void moveLeft(int steps, int wait) {
  switchXon();
  for (int i=0; i<steps; i++) {
    updateX(cyclePos);
    delayMicroseconds(wait);
    cyclePos = (cyclePos + 19) % 20;
  }  
  switchXoff();
}

void moveDown(int steps, int wait) {
  switchYon();
  for (int i=0; i<steps; i++) {
    updateY(cyclePos);
    delayMicroseconds(wait);
    cyclePos = (cyclePos + 19) % 20;
  }
  switchYoff();
}

// drive a little back and forth
void testBackAndForth() {
  moveRight(6000, 300);
  delay(1000);
  moveUp(6000, 300);
  delay(1000);
  moveLeft(6000, 300);
  delay(1000);
  moveDown(6000, 300);
  delay(1000);
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

void loop() {
  testBackAndForth();
  delay(700);
}

