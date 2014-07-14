
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
  int wait = 100;
//  delayMicroseconds(wait);
  if (runtime < 10*sec)
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
    runtime += waitFETLock;
    // connect to VCC
    digitalWrite(p, HIGH);
//    delayMicroseconds(waitFETLock);
  }
  else { // connect channel to ground
    // switch off VCC
    digitalWrite(p, LOW);
    // wait until MOSFET is locked
    delayMicroseconds(waitFETLock);
    runtime += waitFETLock;
    // connect to GND
    digitalWrite(m, HIGH);
//    delayMicroseconds(waitFETLock);
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


int led = 0;

void loop() {
  spin();

}
