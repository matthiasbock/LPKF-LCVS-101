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

void setWlow(int in1, int in2, int lowDisable, int highDisable) {
  if ((cyclePos == lowDisable) || (cyclePos == highDisable)) { // disable
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  else if ((cyclePos > lowDisable) && (cyclePos < highDisable)) { // A low, B high
    // switch the one off before switching the other on
    digitalWrite(in2, LOW);
    digitalWrite(in1, HIGH);
  }
  else { // A high, B low
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
}

void setWhigh(int in3, int in4, int lowDisable, int highDisable) {
  if ((cyclePos == lowDisable) || (cyclePos == highDisable)) { // disable
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  else if ((cyclePos > lowDisable) && (cyclePos < highDisable)) { // C high, D low
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  else { // C low, D high
    digitalWrite(in4, LOW);
    digitalWrite(in3, HIGH);
  }
}

void updateYsWs() {
  setWlow(  A[IN1], A[IN2], 2, 12 );
  setWhigh( A[IN3], A[IN4], 4, 14 );
  setWlow(  B[IN1], B[IN2], 6, 16 );
  setWhigh( B[IN3], B[IN4], 8, 18 );
  setWlow(  C[IN1], C[IN2], 10, 20 );  
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

// drive a little forth and back
void loop() {
  switchYon();

  for (int i=0; i<4000; i++) {
    updateYsWs();
    delayMicroseconds(600);
    cyclePos = (cyclePos % 20) + 1;
  }

  switchYoff();
  delay(1000);
  switchYon();

  for (int i=0; i<4000; i++) {
    updateYsWs();
    delayMicroseconds(600);
    cyclePos = (cyclePos + 19) % 20;
  }
  
  switchYoff();
  delay(1000);
}

