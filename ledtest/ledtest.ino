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

void init(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void initM(int m[6]) {
  for (int i=0; i<6; i++) init(m[i]);
}

void setup() {
  init(13); //LED
  initM(A);
  initM(B);
  initM(C);
  initM(D);
  initM(E);
}

void Test(int X[6]) {
  int wait=100;

  digitalWrite(X[IN1], LOW);
  digitalWrite(X[IN2], LOW);
  digitalWrite(X[IN3], LOW);
  digitalWrite(X[IN4], LOW);

  digitalWrite(X[IN1], HIGH);
  delay(wait);
  digitalWrite(X[IN1], LOW);
  
  digitalWrite(X[IN2], HIGH);
  delay(wait);
  digitalWrite(X[IN2], LOW);
  
  digitalWrite(X[IN3], HIGH);
  delay(wait);
  digitalWrite(X[IN3], LOW);
  
  digitalWrite(X[IN4], HIGH);
  delay(wait);
  digitalWrite(X[IN4], LOW);

  delay(wait);
}

void loop() {
  Test(A);
  Test(B);
  Test(C);
  Test(D);
  Test(E);
}
