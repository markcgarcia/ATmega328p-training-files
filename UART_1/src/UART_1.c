#include <avr/io.h>

void UART_init();
void UART_putchar(unsigned char data);
void UART_putstr(char* str);
void UART_putU8(uint8_t val);
void UART_putU16(uint16_t val);
unsigned char UART_getchar();
void ADC_init();
uint16_t ADC_get();

/* This code reads from a potentiometer with the A0
   pin, and then uses UART to put that sensor reading
   to the serial monitor. */
int main() {

  // Set pin 14 (port c 0) as input
  DDRC &= ~(1 << DDC0);

  // Initialize UART and ADC reading
  UART_init();
  ADC_init();
  UART_putstr("Hello");

  while(1) {
    uint16_t reading = ADC_get();
    UART_putU16(reading); 
    UART_putchar('\r');
    UART_putchar('\n');      
  }

}

void ADC_init() {
  // Initialize 
  ADMUX  |= (0 << REFS1) | (1 << REFS0);  // ADC to channel 0 if we dont change mux bits; ref to 5V
  ADCSRA |= (1 << ADEN);                  // Enable ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // prescaler: 64
}

uint16_t ADC_get() {
  ADCSRA |= (1 << ADSC);
  while(!(ADCSRA & (1 << ADSC)));
  return ADC;
}

void UART_init() {
  // write to lower byte
  UBRR0L = (uint8_t)(103 & 0xFF);

  // write to higher byte
  UBRR0H = (uint8_t)(103 >> 8);

  // enable Tx and Rx
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}

void UART_putchar(unsigned char data) {
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;
}

void UART_putstr(char* str) {
  while (*str != 0) {
    UART_putchar(*str);
    str++;
  }
}

void UART_putU8(uint8_t val) {
    uint8_t dig1 = '0', dig2 = '0';

    // basically a poor man's modulo
    while(val >= 100) {  
        val -= 100;
        dig1++;
    }

    while(val >= 10) {
        val -= 10;
        dig2++;
    }

    if(dig1 != '0') { 
      UART_putchar(dig1);
    }

    // print second digit (or ignore leading zeros)
    if((dig1 != '0') || (dig2 != '0')) {
      UART_putchar(dig2);
    }

    // print final digit
    UART_putchar(val + '0');
}

// UART puts function from http://www.rjhcoding.com/avrc-uart.php.
// I'll rewrite it later but it's pretty annoying.
// Converts 16-bit integer to bits.
void UART_putU16(uint16_t val) {
    uint8_t dig1 = '0', dig2 = '0', dig3 = '0', dig4 = '0';

    // count value in 10000s place
    while(val >= 10000) {
        val -= 10000;
        dig1++;
    }

    // count value in 1000s place
    while(val >= 1000) {
        val -= 1000;
        dig2++;
    }

    // count value in 100s place
    while(val >= 100) {
        val -= 100;
        dig3++;
    }

    // count value in 10s place
    while(val >= 10) {
        val -= 10;
        dig4++;
    }

    // was previous value printed?
    uint8_t prevPrinted = 0;

    // print first digit (or ignore leading zeros)
    if(dig1 != '0') {
      UART_putchar(dig1); 
      prevPrinted = 1;
    }

    // print second digit (or ignore leading zeros)
    if(prevPrinted || (dig2 != '0')) {
      UART_putchar(dig2); 
      prevPrinted = 1;
    }

    // print third digit (or ignore leading zeros)
    if(prevPrinted || (dig3 != '0')) {
      UART_putchar(dig3); 
      prevPrinted = 1;
    }

    // print third digit (or ignore leading zeros)
    if(prevPrinted || (dig4 != '0')) {
      UART_putchar(dig4); 
      prevPrinted = 1;
    }

    // print final digit
    UART_putchar(val + '0');
}

unsigned char UART_getchar() {
  while (!(UCSR0A & (1 << RXC0)));
  return UDR0;
}