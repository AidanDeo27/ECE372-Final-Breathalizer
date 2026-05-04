#include <avr/io.h>
#include <util/delay.h>
#include "switch.h"

/*
Switch on Arduino Mega pin 26 = PA4

Wiring:
PA4 / pin 26 -> one side of switch
GND          -> other side of switch

Uses internal pull-up.
Not pressed = 1
Pressed     = 0
*/

#define SWITCH_PIN PA4

void initSwitch(void) {
    // Set PA4 as input
    DDRA &= ~(1 << DDA4);

    // Enable internal pull-up resistor
    PORTA |= (1 << SWITCH_PIN);
}

unsigned char switchPressed(void) {
    // Active-low switch
    if (!(PINA & (1 << SWITCH_PIN))) {
        return 1;
    } else {
        return 0;
    }
}

void waitForSwitchPress(void) {
    while (!switchPressed()) {
        // wait
    }

    _delay_ms(50); // debounce
}

void waitForSwitchRelease(void) {
    while (switchPressed()) {
        // wait
    }

    _delay_ms(50); // debounce
}