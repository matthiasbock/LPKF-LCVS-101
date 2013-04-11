//
// Arduino driver for Keyes L298 motor module/shield
//

int ENA = 0;
int IN1 = 1;
int IN2 = 2;
int IN3 = 3;
int IN4 = 4;
int ENB = 5;

int A[6] = {47,46,45,44,43,42};
int B[6] = {53,52,51,50,49,48};
int E[6] = {28,29,26,27,24,25};
int D[6] = {30,31,32,33,34,35};
int C[6] = {41,40,39,38,37,36};

/*
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
*/

void init(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void initM(int m[6]) {
  for (int i=0; i<6; i++) init(m[i]);

  digitalWrite(m[ENA], HIGH);
  digitalWrite(m[ENB], HIGH);
}

void setup() {
  initM(A);
  initM(B);
  initM(C);
  initM(D);
  initM(E);
}

int cyclePos = 1;

void setW1() {
  int enable = A[ENA];
  int in1 = A[IN1];
  int in2 = A[IN2];
  if ((cyclePos == 2) || (cyclePos == 12)) { // disable
//    digitalWrite(enable, LOW);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  else if ((cyclePos > 2) && (cyclePos < 12)) { // A low, B high
    // inactivate one first, before activating the other
    digitalWrite(in2, LOW);
    digitalWrite(in1, HIGH);
  }
  else { // A high, B low
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
}

void setW2() {
  int enable = A[ENB];
  int in3 = A[IN3];
  int in4 = A[IN4];
  if ((cyclePos == 4) || (cyclePos == 14)) { // disable
//    digitalWrite(enable, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  else if ((cyclePos > 4) && (cyclePos < 14)) { // C high, D low
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  else { // C low, D high
    digitalWrite(in4, LOW);
    digitalWrite(in3, HIGH);
  }
}

void setW3() {
  int enable = B[ENA];
  int in1 = B[IN1];
  int in2 = B[IN2];
  if ((cyclePos == 6) || (cyclePos == 16)) { // disable
//    digitalWrite(enable, LOW);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  else if ((cyclePos > 6) && (cyclePos < 16)) { // E low, F high
    digitalWrite(in2, LOW);
    digitalWrite(in1, HIGH);
  }
  else { // E high, F low
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
}

void setW4() {
  int enable = B[ENB];
  int in3 = B[IN3];
  int in4 = B[IN4];
  if ((cyclePos == 8) || (cyclePos == 18)) { // disable
//    digitalWrite(enable, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  else if ((cyclePos > 8) && (cyclePos < 18)) { // G high, H low
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  else { // G low, H high
    digitalWrite(in4, LOW);
    digitalWrite(in3, HIGH);
  }
}

void setW5() {
  int enable = C[ENA];
  int in1 = C[IN1];
  int in2 = C[IN2];
  if ((cyclePos == 10) || (cyclePos == 20)) { // disable
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  else if ((cyclePos > 10) && (cyclePos < 20)) { // J low, K high
    digitalWrite(in2, LOW);
    digitalWrite(in1, HIGH);
  }
  else { // high
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
}

void loop() {
  setW1();
  setW2();
  setW3();
  setW4();
  setW5();
  // 5000 steps/s
  //delayMicroseconds(200);
  delay(100);
  cyclePos = (cyclePos % 20) + 1;
}

