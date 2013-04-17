
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

// PWM
//#define LOW 0
//#define HIGH 1

int channel[3][2] = { {U_plus, U_minus}, {V_plus, V_minus}, {W_plus, W_minus} };

void setup() {
        int pins[7] = {13, U_plus, U_minus, V_plus, V_minus, W_plus, W_minus};
        int i;
        for (i=0; i<=7; i++)  {
		pinMode(pins[i], OUTPUT);
		digitalWrite(pins[i], LOW);
        }
}

// waitHigh: microseconds
// how long to apply power during one step
int waitHigh = 2000;

// waitRecover: microseconds
// required to avoid short-circuit, since
// FET needs about 50 microseconds to lock
int waitRecover = 50;

int led = 0;

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

void spin() {
  setChannel(U, LOW);
  delayMicroseconds(waitHigh);
  setChannel(V, HIGH);
  delayMicroseconds(waitHigh);
  setChannel(W, LOW);
  delayMicroseconds(waitHigh);
  setChannel(U, HIGH);
  delayMicroseconds(waitHigh);
  setChannel(V, LOW);
  delayMicroseconds(waitHigh);
  setChannel(W, HIGH);
  delayMicroseconds(waitHigh);
}

// start with one rotation per second
int rpm = 60;
int final_rpm = 1000;
int rpm_inc = 60;

void loop() {
  float rps = rpm/60;
  waitHigh = 1000000*((1/rps)/6.0);
 
  spin();

  if (rpm < final_rpm) {
    rpm = rpm+rpm_inc;
  }
  
  // blink  
  led = (led+1) % 1024;
  digitalWrite(13, led & 2);
}
