#include <avr/io.h>
#include <avr/interrupt.h>

int main() {

    // Allow PORTB to be write instead of read
    DDRB |= (1 << PB5);

    // Setup Timer0 mode to PWM phase correct
    TCCR0A |= (1 << COM0A1) | (0 << COM0A0);

    TCCR0A |= (0 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << WGM02);

    // Set prescaler (256)
    // Acts as a clock divider
    TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00);

    // Enable interrupts
    // TIMSK0 ("Timer/Counter Interrupt Mask Register")
    // Enables interrupts
    TIMSK0 |= (1 << OCIE0A);

    // Clear out counter, set up OCR0
    // OCR0 acts as our duty cycle
    TCNT0 = 0;
    OCR0A = 128;

    uint8_t multiplier = 1;

    while(1) {
        // Going up or down?
        if (OCR0A + 5 > 255) {
          multiplier = -1;
        }
        if (OCR0A - 5 < 0) {
          multiplier = 1;
        }

        // Change the duty cycle
        OCR0A += 5 * multiplier;
    }
}