
//
// driver for high-frequency spindle output stage
// Alfred Jager 33-1 W02
//

// pins
#define U_plus 31
#define U_minus 33
#define V_plus 37
#define V_minus 39
#define W_plus 43
#define W_minus 45

#define U 0
#define V 1
#define W 2
#define plus 0
#define minus 1

int channel[3][2] = { {U_plus, U_minus}, {V_plus, V_minus}, {W_plus, W_minus} };

void setup() {
        int pins[7] = {13, U_plus, U_minus, V_plus, V_minus, W_plus, W_minus};
        int i;
        for (i=0; i<=7; i++)  {
		pinMode(pins[i], OUTPUT);
		digitalWrite(pins[i], LOW);
        }
}

int waitHigh = 5;
int waitRecover = 1;

void setChannel(int ch, int level) {
  int p = channel[ch][plus];
  int m = channel[ch][minus];
  if (level == HIGH) { // connect channel to positive voltage
    digitalWrite(m, LOW);
    delayMicroseconds(waitRecover);
    digitalWrite(p, HIGH);
  }
  else { // connect channel to ground
    digitalWrite(p, LOW);
    delayMicroseconds(waitRecover);
    digitalWrite(m, HIGH);
  }
}

int led = 0;

void loop() {
  setChannel(U, LOW);
  delay(waitHigh);
  setChannel(V, HIGH);
  delay(waitHigh);
  setChannel(W, LOW);
  delay(waitHigh);
  setChannel(U, HIGH);
  delay(waitHigh);
  setChannel(V, LOW);
  delay(waitHigh);
  setChannel(W, HIGH);
  delay(waitHigh);

  // blink  
  led = (led+1) % 1024;
  digitalWrite(13, led & 16);
}
