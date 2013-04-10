//
// Arduino driver for Keyes L298 motor module/shield
//

typedef struct {
  int ENA = 53;
  int IN1 = 52;
  int IN2 = 51;
  int IN3 = 50;
  int IN4 = 49;
  int ENB = 48;
  } L298;

L298 A,B,C,D,E;
L298 module[5] = {A,B,C,D,E};

void init(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  }

void setup() {
  init(13); //LED
  int i;
  for (i=0; i<5; i++) {
    init(module[i].ENA);
    init(module[i].IN1);
    init(module[i].IN2);
    init(module[i].IN3);
    init(module[i].IN4);
    init(module[i].ENB);
    }
  }

int wait=1000;

void TestModule(L298 module) {
  digitalWrite(module.IN1, LOW);
  digitalWrite(module.IN2, LOW);
  digitalWrite(module.IN3, LOW);
  digitalWrite(module.IN4, LOW);

  digitalWrite(module.IN1, HIGH);
  delay(wait);
  digitalWrite(module.IN1, LOW);
  
  digitalWrite(module.IN2, HIGH);
  delay(wait);
  digitalWrite(module.IN2, LOW);
  
  digitalWrite(module.IN3, HIGH);
  delay(wait);
  digitalWrite(module.IN3, LOW);
  
  digitalWrite(module.IN4, HIGH);
  delay(wait);
  digitalWrite(module.IN4, LOW);
  delay(wait);
  }

void loop() {
  TestModule(E);
  }
