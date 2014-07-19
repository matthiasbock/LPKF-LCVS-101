
//
// driver for high-frequency spindle output stage
// Alfred Jager 33-1 W02
//

/*
 * Minus pin high -> output connected to GND
 * Minus pin low  -> output disconnected from GND
 * Plus pin high  -> output connected to VCC
 * Plus pin low   -> output disconnected from VCC
 */ 

#include <SoftwareSerial.h>

// pins
#define U_plus  12
#define U_minus 13
#define V_plus  10
#define V_minus 11
#define W_plus  6
#define W_minus 7

#define U 0
#define V 1
#define W 2
#define plus 0
#define minus 1

int channel[3][2] = { {U_plus, U_minus}, {V_plus, V_minus}, {W_plus, W_minus} };

void setup() {
  Serial.begin(9600);
        int pins[7] = {13, U_plus, U_minus, V_plus, V_minus, W_plus, W_minus};
        for (int i=0; i<=7; i++)  {
		pinMode(pins[i], OUTPUT);
		digitalWrite(pins[i], LOW);
        }
}


// program runtime in microseconds
long runtime = 0;
#define sec 1000000

// delayStep: microseconds
// function: increase spindle speed step-by-step
void delayStep() { 
  // from 2000 to 100 in 5s
  int from = 10000;
  int to = 40;
  float time = 10*sec;
  int wait = round(from+((runtime/time)*(to-from)));
  if (runtime > time)
    wait = to;
  delayMicroseconds(wait);
  if (runtime < 60*sec)
    runtime += wait;
};


// waitFETLock: microseconds
int waitFETLock = 1000;

void setChannel(int ch, int level) {
  int p = channel[ch][plus];
  int m = channel[ch][minus];
  if (level == HIGH) { // connect channel to positive voltage
    // disconnect GND
    digitalWrite(m, LOW);
    // wait until MOSFET is locked
    delayMicroseconds(waitFETLock);
//    runtime += waitFETLock;
    // connect to VCC
    digitalWrite(p, HIGH);
  }
  else { // connect channel to ground
    // switch off VCC
    digitalWrite(p, LOW);
    // wait until MOSFET is locked
    delayMicroseconds(waitFETLock);
//    runtime += waitFETLock;
    // connect to GND
    digitalWrite(m, HIGH);
  }
}


void spin() {
  setChannel(U, LOW);
  delayStep();
  setChannel(V, HIGH);
  delayStep();
  setChannel(W, LOW);
  delayStep();
  setChannel(U, HIGH);
  delayStep();
  setChannel(V, LOW);
  delayStep();
  setChannel(W, HIGH);
  delayStep();
}

void halt() {
  setChannel(U, LOW);
  setChannel(V, LOW);
  setChannel(W, LOW);
  delay(5000);
}

boolean spinning = true;

void loop() {
  if (spinning) {
    spin();
  }
  if (Serial.available()) {
    int ch = Serial.read();
    if (ch == '0') {
      spinning = false;
    }
    else if (ch == '1') {
      spinning = true;
    }
  }
}
