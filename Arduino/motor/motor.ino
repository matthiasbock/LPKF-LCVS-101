// motor power control pins
// use PWM to regulate motor power
// possible values: 0-255

#define M1_EN 11
#define M2_EN 3
#define M3_EN 6
#define M4_EN 5

void setPower(int motor, int power) {
  analogWrite(motor, power);
}

// motor shield control pins

#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

// initialize motor shield
int latch_copy;

void setup() {
	// Set pins for shift register to output
	pinMode(MOTORLATCH, OUTPUT);
	pinMode(MOTORENABLE, OUTPUT);
	pinMode(MOTORDATA, OUTPUT);
	pinMode(MOTORCLK, OUTPUT);

        pinMode(13, OUTPUT);

	// Set pins for shift register to default value (low);
	digitalWrite(MOTORDATA, LOW);
	digitalWrite(MOTORLATCH, LOW);
	digitalWrite(MOTORCLK, LOW);

	// Enable the shift register, set Enable pin LOW.
	digitalWrite(MOTORENABLE, LOW);

        // power on all four half-H bridges
        int power = 127;
        analogWrite(M1_EN, power);
        analogWrite(M2_EN, power);
        analogWrite(M3_EN, power);
        analogWrite(M4_EN, power);

	// start with all shift register outputs low
	latch_copy = 0;
}

// upload latch_copy to motor shield

void uploadToMotorShield() {
	// Use the default Arduino 'shiftOut()' function to
	// shift the bits with the MOTORCLK as clock pulse.
	// The 74HC595 shiftregister wants the MSB first.
	shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
	delayMicroseconds(5);    // For safety, not really needed.

	// After that, generate a latch pulse with MOTORLATCH.
	digitalWrite(MOTORLATCH, HIGH);
	delayMicroseconds(5);    // For safety, not really needed.
	digitalWrite(MOTORLATCH, LOW);
}

// 8 bits control 8 half-H bridges
// 4 DC motors can be attached: M1-4

#define M1_A 2
#define M1_B 3
#define M2_A 1
#define M2_B 4
#define M3_A 5
#define M3_B 7
#define M4_A 0
#define M4_B 6
//#define HIGH 1
//#define LOW 0

void setPin(int output, int high_low) {
	// Change bit "output" in "latch_copy" to "high_low".
	// The defines HIGH and LOW are 1 and 0.
	bitWrite(latch_copy, output, high_low);
}

int coils[4][2] = { {M1_A,M1_B}, {M2_A,M2_B}, {M3_A,M3_B}, {M4_A,M4_B} };
#define OFF 0
#define ATTRACT 1
#define REPULSE 2

void setCoil(int coil, int level) {
  int P, Q;
  coil = coil % 4;
  P = coils[coil][0];
  Q = coils[coil][1];

  if (level == ATTRACT) {
    setPin(P, HIGH);
    setPin(Q, LOW);
    }
  else if (level == REPULSE) {
    setPin(P, LOW);
    setPin(Q, HIGH);
    }
  else {
    setPin(P, LOW);
    setPin(Q, LOW);
    }

  uploadToMotorShield();
  }

int m = 0;
int switch_to = ATTRACT;
int wait = 50;

void loop() {
	setCoil(m, OFF);
	delay(wait);
	setCoil(m, switch_to);
        delay(wait);
        digitalWrite(13, switch_to-1);
	m += 1;
	if (switch_to == ATTRACT)
		switch_to = REPULSE;
	else
		switch_to = ATTRACT;
}
