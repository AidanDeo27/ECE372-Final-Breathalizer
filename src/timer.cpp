#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

volatile unsigned int seconds_blown = 0;

// one second counter using timer1
void initTimer1(void) {
    TCCR1A = 0x00;
    TCCR1B = 0x00;

    TCCR1B |= (1 << WGM12); // CTC mode

    OCR1A = 15624; // 1 second at 16 MHz / 1024

    TCNT1 = 0;

    TIMSK1 |= (1 << OCIE1A); // Enable compare match interrupt

    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler = 1024

    sei();
}

void startTimer1(void) {
    TCNT1 = 0;
    TIFR1 |= (1 << OCF1A);
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

void stopTimer1(void) {
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

void delaySeconds(unsigned int seconds) {
    while (seconds > 0) {
        TCNT1 = 0;
        TIFR1 |= (1 << OCF1A);

        while ((TIFR1 & (1 << OCF1A)) == 0) {
        }

        seconds--;
    }
}

ISR(TIMER1_COMPA_vect) {
    seconds_blown++;
}