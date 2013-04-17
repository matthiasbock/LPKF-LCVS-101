
//
// driver for high-frequency spindle output stage
// Alfred Jager 33-1 W02
//

#define U_plus 31
#define U_minus 33

#define V_plus 37
#define V_minus 39

#define W_plus 43
#define W_minus 45
//int forward[3][2] = { {U_plus, V_minus}, {V_plus, W_minus}, {W_plus, U_minus} };

void setup() {
        int pins[7] = {13, U_plus, U_minus, V_plus, V_minus, W_plus, W_minus};
        int i;
        for (i=0; i<=7; i++)  {
		pinMode(pins[i], OUTPUT);
		digitalWrite(pins[i], LOW);
        }
}

void OnOff(int pinPlus, int pinMinus, int waitHigh, int waitRecover) {
  digitalWrite(pinPlus, HIGH); 
  digitalWrite(pinMinus, HIGH);
  digitalWrite(13, HIGH);
  delay(waitHigh);
  digitalWrite(pinPlus, LOW);
  digitalWrite(pinMinus, LOW);
  digitalWrite(13, LOW);
  delay(waitRecover);
}

int led = 0;
int a = 2000;
int b = 2000;

void loop() {
  // U-V
  OnOff(U_plus, W_minus, a, b);
  
  // V-W
  //OnOff(V_plus, W_minus, a, b);
  
  // W-U
  //OnOff(W_plus, U_minus, a, b);

  // blink  
//  led = (led+1) % 1024;
//  digitalWrite(13, led & 16);
}
