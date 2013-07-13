//
// Arduino driver for LPKF 101 LC/VS milling machine X/Y axis
// using Keyes L298N motor modules/shields
//

#define Xmin 3
#define Xmax 2
#define Ymin 22
#define Ymax 23

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
int A[6] = {47,46,45,44,43,42};
int B[6] = {53,52,51,50,49,48};
int C[6] = {41,40,39,38,37,36};
int D[6] = {30,31,32,33,34,35};
int E[6] = {28,29,26,27,24,25};

void initPin(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void initModule(int m[6]) {
  for (int i=0; i<6; i++)
    initPin(m[i]);
}

void setup() {
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

// drive a little back and forth
void testBackAndForth() {

  switchXon();
  for (int i=0; i<6000; i++) {
    updateX(cyclePos);
    delayMicroseconds(400);
    cyclePos = (cyclePos % 20) + 1;
  }
  switchXoff();
  delay(1000);

  switchYon();
  for (int i=0; i<6000; i++) {
    updateY(cyclePos);
    delayMicroseconds(400);
    cyclePos = (cyclePos % 20) + 1;
  }
  switchYoff();
  delay(1000);

  switchXon();
  for (int i=0; i<6000; i++) {
    updateX(cyclePos);
    delayMicroseconds(400);
    cyclePos = (cyclePos + 19) % 20;
  }  
  switchXoff();
  delay(1000);

  switchYon();
  for (int i=0; i<6000; i++) {
    updateY(cyclePos);
    delayMicroseconds(400);
    cyclePos = (cyclePos + 19) % 20;
  }
  switchYoff();
  delay(1000);
}

#define led 13

void blink(n) {
  for (int i=0; i<n; i++) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }
}

// test min/max switches
void testMinMaxSwitches() {
  if (digitalRead(Xmin) == HIGH) {
    blink(1);
  }
  else if (digitalRead(Xmax) == HIGH) {
    blink(2);
  }
  else if (digitalRead(Ymin) == HIGH) {
    blink(3);
  }
  else if (digitalRead(Ymax) == HIGH) {
    blink(4);
  }
}

void loop() {
  testBackAndForth();
}

