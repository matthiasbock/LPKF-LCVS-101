
//
// driver for high-frequency spindle output stage
// Alfred Jager 33-1 W02
//

// pins
#define U_plus  22
#define U_minus 24
#define V_plus  26
#define V_minus 28
#define W_plus  30
#define W_minus 32

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


// program runtime in microseconds
long runtime = 0;
#define sec 1000000

// delayStep: microseconds
// function: how long to apply power during a step
void delayStep() { 
  int wait = 1600;
  if (runtime < 1*sec) {
    wait = 6000;
  }
  else if (runtime < 2*sec) {
    wait = 3000;
  }
  else if (runtime < 4*sec) {
    wait = 1800;
  }
  delayMicroseconds(wait);
  if (runtime < 10*sec)
    runtime += wait;
};


// waitFETLock: microseconds
int waitFETLock = 1;

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
  // blink  
  led = (led+1) % 1024;
  digitalWrite(13, led & 2);
}
