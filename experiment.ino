//
// Arduino driver for LPKF 101 LC/VS milling machine X/Y axis
// using Keyes L298N motor modules/shields
//

      int ENA = 0;
        int IN1 = 1;
          int IN2 = 2;
            int IN3 = 3;
              int IN4 = 4;
                int ENB = 5;

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
  for (int i=0; i<6; i++) initPin(m[i]);
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

void setup() {
  initModule(A);
  initModule(B);
  initModule(C);
  initModule(D);
  initModule(E);
}

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
*/

int cyclePos = 1;

/*
  A five phase stepper motor consists of five coils W1 to W5.
  The following three functions implement the half-step timing procedure
  found in Berger-Lahr's 5-phase stepping motor datasheet.
*/

//
// This function sets the voltage levels according to cycle position.
// A five phase stepper motor has 5 pairs of wires.
// For every pair and step this function is called.
//
// pin1 is the number of the Pin, that needs to be high, for the first wire of a pair to be + pole.
// pin2 accordingly is the Pin, that needs to be high, for the second wire to be + pole.
// lowDisable is the lower step number, where the level of a wire changes.
// highDisable is the higher step number, where wire levels change.
//
// If highDisable < lowDisable, then in the step range between lowDisable and highDisable
// the first wire is enabled instead of disabled.
//
void setPhase(int stepPosition, int pin1, int pin2, int lowDisable, int highDisable) {
  if ((stepPosition == lowDisable) || (stepPosition == highDisable)) { // disable
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
  else if (lowDisable < highDisable) {
	if ((stepPosition > lowDisable) && (stepPosition < highDisable)) { // first low, second high
		// switch the one off before switching on the other
		digitalWrite(pin2, LOW);
		digitalWrite(pin1, HIGH);
	} else { // first high, second low
	    digitalWrite(pin1, LOW);
	    digitalWrite(pin2, HIGH);
	  }
  } else { // highDisable < lowDisable
	if ((stepPosition > lowDisable) && (stepPosition < highDisable)) { // first high, second low
	    digitalWrite(pin1, LOW);
	    digitalWrite(pin2, HIGH);
	} else { // first low, second high
		digitalWrite(pin2, LOW);
		digitalWrite(pin1, HIGH);
	  }	
    }
}

void updateYPosition(int stepY) {
  setPhase( stepY, A[IN1], A[IN2], 2, 12 );
  setPhase( stepY, A[IN3], A[IN4], 14, 4 );
  setPhase( stepY, B[IN1], B[IN2], 6, 16 );
  setPhase( stepY, B[IN3], B[IN4], 18, 8 );
  setPhase( stepY, C[IN1], C[IN2], 10, 20 );  
}

void updateXPosition(int stepX) {
  setPhase( stepX, C[IN3], C[IN4], 2, 12 );
  setPhase( stepX, D[IN1], D[IN2], 14, 4 );
  setPhase( stepX, D[IN3], D[IN4], 6, 16 );
  setPhase( stepX, E[IN1], E[IN2], 18, 8 );
  setPhase( stepX, E[IN3], E[IN4], 10, 20 );
}

// drive a little forth and back
void loop() {
  switchXon();
  for (int i=0; i<6000; i++) {
    updateXPosition(cyclePos);
    delayMicroseconds(400);
    cyclePos = (cyclePos % 20) + 1;
  }
  switchXoff();

  delay(1000);

  switchYon();
  for (int i=0; i<6000; i++) {
    updateYPosition(cyclePos);
    delayMicroseconds(400);
    cyclePos = (cyclePos % 20) + 1;
  }
  switchYoff();
  
  delay(1000);

  switchXon();
  for (int i=0; i<6000; i++) {
    updateXPosition(cyclePos);
    delayMicroseconds(400);
    cyclePos = (cyclePos + 19) % 20;
  }  
  switchXoff();

  delay(1000);

  switchYon();
  for (int i=0; i<6000; i++) {
    updateYPosition(cyclePos);
    delayMicroseconds(400);
    cyclePos = (cyclePos + 19) % 20;
  }
  switchYoff();

  delay(1000);
}

