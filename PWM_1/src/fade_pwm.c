#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

int main() {

    // Allow PORTD to be write instead of read
    DDRD |= (1 << DDD6);

    // Setup Timer0 mode to PWM phase correct
    // You actually want fast PWM
    TCCR0A |= (1 << COM0A1) | (0 << COM0A0);

    TCCR0A |= (0 << WGM01) | (1 << WGM00);
    TCCR0B |= (0 << WGM02);

    // Set prescaler (256)
    // Acts as a clock divider
    TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);

    // Enable interrupts
    // TIMSK0 ("Timer/Counter Interrupt Mask Register")
    // Enables interrupts
    TIMSK0 |= (1 << OCIE0A);

    // Clear out counter, set up OCR0
    // OCR0 acts as our duty cycle
    TCNT0 = 0;
    OCR0A = 128;

    uint8_t brightness = 128;
    int8_t fadeAmount = 5;

    while(1) {
        // Going up or down?
        if (brightness >= 255 || brightness <= 0) {
          fadeAmount = -fadeAmount;
        }

        // Change the duty cycle
        brightness += fadeAmount;
        OCR0A = brightness;
        
        _delay_ms(50);
    }
}