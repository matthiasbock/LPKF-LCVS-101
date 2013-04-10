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
}

int safewait=1;

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
  int x = A[IN1];
  int y = A[IN2];
  if (cyclePos == 2 || cyclePos == 12) { // disable
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, LOW);
  }
  else if (cyclePos > 2 && cyclePos < 12) { // low
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, HIGH);
    digitalWrite(enable, HIGH);
  }
  else { // high
    digitalWrite(enable, LOW);
    digitalWrite(x, HIGH);
    digitalWrite(y, LOW);
    digitalWrite(enable, HIGH);
  }
}

void setW2() {
  int enable = A[ENB];
  int x = A[IN3];
  int y = A[IN4];
  if (cyclePos == 4 || cyclePos == 14) { // disable
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, LOW);
  }
  else if (cyclePos > 4 && cyclePos < 14) { // high
    digitalWrite(enable, LOW);
    digitalWrite(x, HIGH);
    digitalWrite(y, LOW);
    digitalWrite(enable, HIGH);
  }
  else { // low
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, HIGH);
    digitalWrite(enable, HIGH);
  }
}

void setW3() {
  int enable = B[ENA];
  int x = B[IN1];
  int y = B[IN2];
  if (cyclePos == 6 || cyclePos == 16) { // disable
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, LOW);
  }
  else if (cyclePos > 6 && cyclePos < 16) { // low
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, HIGH);
    digitalWrite(enable, HIGH);
  }
  else { // high
    digitalWrite(enable, LOW);
    digitalWrite(x, HIGH);
    digitalWrite(y, LOW);
    digitalWrite(enable, HIGH);
  }
}

void setW4() {
  int enable = B[ENB];
  int x = B[IN3];
  int y = B[IN4];
  if (cyclePos == 8 || cyclePos == 18) { // disable
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, LOW);
  }
  else if (cyclePos > 8 && cyclePos < 18) { // high
    digitalWrite(enable, LOW);
    digitalWrite(x, HIGH);
    digitalWrite(y, LOW);
    digitalWrite(enable, HIGH);
  }
  else { // low
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, HIGH);
    digitalWrite(enable, HIGH);
  }
}

void setW5() {
  int enable = C[ENA];
  int x = C[IN1];
  int y = C[IN2];
  if (cyclePos == 10 || cyclePos == 20) { // disable
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, LOW);
  }
  else if (cyclePos > 10 && cyclePos < 20) { // low
    digitalWrite(enable, LOW);
    digitalWrite(x, LOW);
    digitalWrite(y, HIGH);
    digitalWrite(enable, HIGH);
  }
  else { // high
    digitalWrite(enable, LOW);
    digitalWrite(x, HIGH);
    digitalWrite(y, LOW);
    digitalWrite(enable, HIGH);
  }
}

void loop() {
  setW1();
  setW2();
  setW3();
  setW4();
  setW5();
  delay(50);
  cyclePos = (cyclePos % 20) + 1;
}

