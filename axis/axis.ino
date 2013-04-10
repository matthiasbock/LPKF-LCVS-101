//
// Arduino driver for Keyes L298 motor module/shield
//

#include </home/code/Arduino-CNC/axis/axis.h>

L298 E;

int E.ENA=53;
/*E.IN1 = 52;
E.IN2 = 51;
E.IN3 = 50;
E.IN4 = 49;
E.ENB = 48;*/

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
