
//
// driver for high-frequency spindle output stage
// Alfred Jager 33-1 W02
//

#define U_A 31
#define U_B 33
#define V_A 37
#define V_B 39
#define W_A 43
#define W_B 45

void setup() {
        int pins[7] = {13, U_A, U_B, V_A, V_B, W_A, W_B};
        int i;
        for (i=0; i<=7; i++)  {
		pinMode(pins[i], OUTPUT);
		digitalWrite(pins[i], LOW);
        }
}

int forward[3][2] = { {U_A, V_B}, {V_A, W_B}, {W_A, U_B} };
int step = 0;
int led = LOW;
int a, b;

void loop() {
	a = forward[step][0];
	b = forward[step][1];
	digitalWrite(a, HIGH);
	digitalWrite(b, HIGH);
	delay(1);
	digitalWrite(a, LOW);
	digitalWrite(b, LOW);
	delay(1);
	step = (step+1) % 3;
	led = (led+1) % 1024;
	digitalWrite(13, led & 64);
}
