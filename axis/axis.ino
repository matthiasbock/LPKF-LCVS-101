//
// Arduino driver for Keyes L298 motor module/shield
//

int A_ENA = 28;
int A_IN1 = 29;
int A_IN2 = 26;
int A_IN3 = 27;
int A_IN4 = 24;
int A_ENB = 25;

int B_ENA = 30;
int B_IN1 = 31;
int B_IN2 = 32;
int B_IN3 = 33;
int B_IN4 = 34;
int B_ENB = 35;

int C_ENA = 41;
int C_IN1 = 40;
int C_IN2 = 39;
int C_IN3 = 38;
int C_IN4 = 37;
int C_ENB = 36;

int D_ENA = 47;
int D_IN1 = 46;
int D_IN2 = 45;
int D_IN3 = 44;
int D_IN4 = 43;
int D_ENB = 42;

int E_ENA = 53;
int E_IN1 = 52;
int E_IN2 = 51;
int E_IN3 = 50;
int E_IN4 = 49;
int E_ENB = 48;

void init(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  }

void setup() {
  init(13); //LED
  init(E.ENA);
  init(E.IN1);
  init(E.IN2);
  init(E.IN3);
  init(E.IN4);
  init(E.ENB);
  }

int wait=1000;

void TestModule() {
  digitalWrite(E.IN1, LOW);
  digitalWrite(E.IN2, LOW);
  digitalWrite(E.IN3, LOW);
  digitalWrite(E.IN4, LOW);

  digitalWrite(E.IN1, HIGH);
  delay(wait);
  digitalWrite(E.IN1, LOW);
  
  digitalWrite(E.IN2, HIGH);
  delay(wait);
  digitalWrite(E.IN2, LOW);
  
  digitalWrite(E.IN3, HIGH);
  delay(wait);
  digitalWrite(E.IN3, LOW);
  
  digitalWrite(E.IN4, HIGH);
  delay(wait);
  digitalWrite(E.IN4, LOW);
  delay(wait);
  }

void loop() {

  TestModule();
  }
