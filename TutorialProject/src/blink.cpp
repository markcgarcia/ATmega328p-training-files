/* This code blinks the onboard LED.
   Onboard LED is PORTB5 (reference pinout for
   more details, which is digital pin 13. */
#include <stdint.h>

// Set macros
// Can change pin macro as needed
#define PORTB *((volatile uint8_t*)0x25)
#define DDRB  *((volatile uint8_t*)0x24)
#define PIN   5

int main() {

  // Set data direction (0 = read, 1 = write)
  DDRB |= 1 << PIN;

  while(1) {

    // Set pin to be on 
    PORTB |= 1 << PIN;
    for (long i = 0; i < 1000000; i++) {
      PORTB |= 1 << PIN;
    }

    // Set pin to be off
    PORTB &= ~(1 << PIN);
    for (long i = 0; i < 1000000; i++) {
      PORTB &= ~(1 << PIN);
    }
  }
}
