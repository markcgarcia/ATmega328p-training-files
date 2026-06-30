#include <avr/io.h>

void delay(int ms);
/* This is a basic LED fade program.
   We use Timer0 to act as a delay and Timer1 to
   act as our PWM controller. */
   
int main() {

    /* Setup Timer1 for our delay function.
       We're going to use Timer0 for our PWM control. */

    // Setup Timer1 mode to CTC
    TCCR1B |= (0 << WGM13) | (1 << WGM12);
    TCCR1A |= (0 << WGM11) | (0 << WGM10);

    // Set prescaler (256)
    TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);

    /* Setup Timer0 for our Fast PWM control.
       Also configure digital pin 6 to output to 
       the LED on the breadboard. */

    // Allow PORTD to be write instead of read, as
    // digital pin 6 is wired to OCR0A.
    DDRD |= (1 << DDD6);

    // Setup Timer0 mode to fast PWM
    TCCR0A |= (1 << COM0A1) | (0 << COM0A0);
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (0 << WGM02);

    // Set prescaler (64)
    // Acts as a clock divider
    TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);

    // Clear out counter, set up OCR0
    // OCR0 acts as our duty cycle
    TCNT0 = 0;
    OCR0A = 128;

    uint8_t brightness = 130;
    int8_t fadeAmount = 5;

    while(1) {
        // Change the duty cycle
        brightness += fadeAmount;
        OCR0A = brightness;
        delay(30);

        // Going up or down?
        if (brightness > 250) {
          fadeAmount = -fadeAmount;
        } else if (brightness < 5) {
          fadeAmount = -fadeAmount;
          // Delay at bottom: makes IRL fading better
          delay(1000);
        }
    }
}

void delay(int ms) {
    // Clear out counter, set up OCR0
    // Use formula to convert
    TCNT1 = 0;
    OCR1A = 62500 * (ms * 0.001) - 1;

    // Poll TIFR
    while (1) {
        if (TIFR1 & (1 << OCF1A)) {
            // Clear flag for match compare
            TIFR1 |= (1 << OCF1A);
            break;
        }
    }
}