    
#define magnet 8

void setup() {
	pinMode(13, OUTPUT);
	pinMode(magnet, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  digitalWrite(magnet, HIGH);
  delay(2000);
  digitalWrite(13, LOW);
  digitalWrite(magnet, LOW);
  delay(2000);
}
