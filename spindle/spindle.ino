
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
#define W_plus  8
#define W_minus 9

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
  int wait;
/*  if (runtime < 3*sec) {
    wait = 6000;
  }
  else if (runtime < 4*sec) {
    wait = 5000;
  }
  else if (runtime < 6*sec) {
    wait = 4000;
  }
  else if (runtime < 8*sec) {
    wait = 3000;
  }
  else if (runtime < 10*sec) {
    wait = 1800;
  }
  else
    wait = 1600;
  delayMicroseconds(wait); */
  delay(666);
  if (runtime < 10*sec)
    runtime += wait;
};


// waitFETLock: microseconds
int waitFETLock = 100;

void setChannel(int ch, int level) {
  int p = channel[ch][plus];
  int m = channel[ch][minus];
  if (level == HIGH) { // connect channel to positive voltage
    digitalWrite(m, LOW);
    delayMicroseconds(waitFETLock);
    runtime += waitFETLock;
    digitalWrite(p, HIGH);
  }
  else { // connect channel to ground
    digitalWrite(p, LOW);
    delayMicroseconds(waitFETLock);
    runtime += waitFETLock;
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


int led = 0;

void loop() {
  spin();

}
