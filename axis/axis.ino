//
// Arduino driver for Keyes L298 motor module/shield
//

int ENA = 53;
int IN1 = 52;
int IN2 = 51;
int IN3 = 50;
int IN4 = 49;
int ENB = 48;

int all[7] = {13, ENA, IN1, IN2, IN3, IN4, ENB};

void setup() {
  int i;
  for (i=0; i<7; i++) {
    pinMode(all[i], OUTPUT);
    digitalWrite(all[i], LOW);
    }
}

void loop() {

  // test 1: test enable pins
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);

  digitalWrite(ENA, HIGH);
  delay(1000);
  digitalWrite(ENA, LOW);
  delay(1000);
  
  digitalWrite(ENB, HIGH);
  delay(1000);
  digitalWrite(ENB, LOW);
  delay(1000);

  // test 2: test input pins
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  digitalWrite(IN1, HIGH);
  delay(1000);
  digitalWrite(IN1, LOW);
  
  digitalWrite(IN2, HIGH);
  delay(1000);
  digitalWrite(IN2, LOW);
  
  digitalWrite(IN3, HIGH);
  delay(1000);
  digitalWrite(IN3, LOW);
  
  digitalWrite(IN4, HIGH);
  delay(1000);
  digitalWrite(IN4, LOW);

  delay(1000);
}
