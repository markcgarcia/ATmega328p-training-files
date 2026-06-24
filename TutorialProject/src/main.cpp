#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  PORTB = 0b00100000;   // 0d 32
  delay(200);
  PORTB = 0b00000000;   // 0d 0
  delay(200);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}