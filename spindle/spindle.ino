
//
// driver for high-frequency spindle output stage
// Alfred Jager 33-1 W02
//

// pins
#define U_plus  2
#define U_minus 3
#define V_plus  4
#define V_minus 5
#define W_plus  6
#define W_minus 7

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

// waitStep: microseconds
// how long to apply power during a step
int waitStep = 6000;

// waitFETLock: microseconds
// required to avoid short-circuit, since
// FET needs about 50 microseconds to lock
int waitFETLock = 60;

int led = 0;

void setChannel(int ch, int level) {
  int p = channel[ch][plus];
  int m = channel[ch][minus];
  if (level == HIGH) { // connect channel to positive voltage
    digitalWrite(m, LOW);
    delayMicroseconds(waitFETLock);
    digitalWrite(p, HIGH);
  }
  else { // connect channel to ground
    digitalWrite(p, LOW);
    delayMicroseconds(waitFETLock);
    digitalWrite(m, HIGH);
  }
}

void spin() {
  setChannel(U, LOW);
  delayMicroseconds(waitStep);
  setChannel(V, HIGH);
  delayMicroseconds(waitStep);
  setChannel(W, LOW);
  delayMicroseconds(waitStep);
  setChannel(U, HIGH);
  delayMicroseconds(waitStep);
  setChannel(V, LOW);
  delayMicroseconds(waitStep);
  setChannel(W, HIGH);
  delayMicroseconds(waitStep);
}

void loop() {
  if (waitStep > 1800)
    waitStep -= 50;
  else
    waitStep = 1800;
 
  spin();
  
  // blink  
  led = (led+1) % 1024;
  digitalWrite(13, led & 2);
}
