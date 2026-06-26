#include <avr/io.h>
#include <avr/interrupt.h>

int main() {

    // Allow PORTB to be write instead of read
    DDRB |= (1 << PB5);

    // Setup Timer1 mode to CTC
    // TCCR1 ("Timer/Counter Control Register")
    TCCR1B |= (0 << WGM13) | (1 << WGM12);
    TCCR1A |= (0 << WGM11) | (0 << WGM10);

    // Set prescaler (256)
    // Acts as a clock divider
    TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);

    // Enable interrupts
    // TIMSK1 ("Timer/Counter Interrupt Mask Register")
    // Enables interrupts
    TIMSK1 |= (1 << OCIE1A);

    // Clear out counter, set up OCR0
    // TCNT1 has our "running count", OCR1A is value we count up to
    // use 62499 for 1 second on, 1 second off
    TCNT1 = 0;
    OCR1A = 62499/2;

    while(1) {
        // Check if compare match happens
        if (TIFR1 & (1 << OCF1A)) {
            
            // Clear flag for match compare
            // Clear out with logic 1: refer to datasheet
            TIFR1 |= (1 << OCF1A);

            // Reset TCNT1 (counter) back to zero
            TCNT1 = 0;

            // Toggle PB5 (controls LED)
            PORTB ^= (1 << PB5);
        }
    }
}