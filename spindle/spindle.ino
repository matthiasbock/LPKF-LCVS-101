
//
// driver for high-frequency spindle output stage
// Alfred Jager 33-1 W02
//

#define V_A 31
#define V_B 33
#define W_A 37
#define W_B 39
#define U_A 43
#define U_B 45

void setup() {
        int pins[7] = {13, U_A, U_B, V_A, V_B, W_A, W_B};
        int i;
        for (i=0; i<=7; i++)  {
		pinMode(pins[i], OUTPUT);
		digitalWrite(pins[i], LOW);
        }
}

//int forward[3][2] = { {U_A, V_B}, {V_A, W_B}, {W_A, U_B} };
int led = LOW;

int a = 1;
int b = 1;

void loop() {
  // A B B
  digitalWrite(U_A, HIGH); 
  digitalWrite(W_B, HIGH);
  delay(a);
  digitalWrite(U_A, LOW);
  digitalWrite(V_B, LOW);
  delay(b);
  
  // B A B
  digitalWrite(U_B, HIGH);
  digitalWrite(V_A, HIGH);
  delay(a);
  digitalWrite(V_A, LOW);
  digitalWrite(W_B, LOW);
  delay(b);
  
  // B B A
  digitalWrite(V_B, HIGH);
  digitalWrite(W_A, HIGH);
  delay(a);
  digitalWrite(W_A, LOW);
  digitalWrite(U_B, LOW);
  delay(b);
  
  led = (led+1) % 1024;
  digitalWrite(13, led & 8);
}
