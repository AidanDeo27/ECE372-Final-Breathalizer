#include "timer.h"
#include <Arduino.h>




//one second counder using timer1
void initTimer1() {
 TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode


 TCCR1B |= (1 << CS12) | (1 << CS10); // Set up timer with prescaler = 1024

 TCNT1 = 0; // Initialize counter value to 0
 
 OCR1A = 15624; // Set CTC compare value for 1Hz increments

 TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt

 sei(); // Enable global interrupts

}

void delaySeconds(unsigned int seconds) {
    TCCR1B = (TCCR1B & ~((1 << CS12) | (1 << CS10))) | (1 << CS12) | (1 << CS10); // Start timer with prescaler = 1024

    while (seconds > 0) {
        TCNT1 = 0; // Reset timer counter
        TIFR1 |= (1 << OCF1A); // Clear compare match flag

        while ((TIFR1 & (1 << OCF1A)) == 0) {
            // Wait for compare match
        }

        seconds--; // Decrement seconds
    }

    TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Stop timer

    }

void startTimer1() {
    TCNT1 = 0;
    TIFR1 |= (1 << OCF1A); // Clear compare match flag

    TCCR1B |= (1 << CS12) | (1 << CS10); // Start timer with prescaler = 1024
}

void stopTimer1() {
    TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Stop timer
}


extern volatile unsigned int seconds_blown; // Declare the variable as extern to be defined in main.cpp
ISR(TIMER1_COMPA_vect) {
  // This will be called every second
  // You can use this to update the display, read sensors, etc.
  seconds_blown++;
}

    


