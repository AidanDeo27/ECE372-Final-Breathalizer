#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

/* Initialize PA0, PA1, PA2, and PA3 to outputs
pins 22-25 on the board
*/
void initLED(){
    DDRA |= (1<<PA0);
    DDRA |= (1<<PA1);
    DDRA |= (1<<PA2);
    DDRA |= (1<<PA3); 

    //start with all the leds off
    allLEDsOff();
}

/* This must be one line of code.
* In this function you will be giving a number "num" which will be represented
* in binary by four LEDs. You must effectively assign the lowest four bits of
* "num" to the appropriate bits of PORTA.
*/
void turnOnLEDWithChar(unsigned char num){
  // combined the lines from the requirements
    // top 4 bits are reserved and the bottom 4 are discarded 
    //0xF0 = 240 = 11110000
    PORTA = (PORTA & 0xF0) | (num & 0x0F);
}

void greenLEDsOn(void) {
    // Turn the green leds on pins 22 and 23
    PORTA &= ~((1 << PA2) | (1 << PA3));
    PORTA |=  (1 << PA0) | (1 << PA1);
}

void redLEDsOn(void) {
    // Turn the red leds on pins 24 and 25
    PORTA &= ~((1 << PA0) | (1 << PA1));
    PORTA |=  (1 << PA2) | (1 << PA3);
}

void allLEDsOff(void) {
    PORTA &= ~((1 << PA0) | (1 << PA1) | (1 << PA2)   | (1 << PA3));
}

// NICK we flashing red leds above limit and green below limit
void flashRedLEDs(void) {
    // for loop will flash the leds 3 times
    for (unsigned char i = 0; i < 3; i++) {

        // lights stay on for half a second and off for 300 ms
        redLEDsOn();
        _delay_ms(500);

        allLEDsOff();
        _delay_ms(300);
    }
}

// repeat for green leds
void flashGreenLEDs(void) {
    for (unsigned char i = 0; i < 3; i++) {
        greenLEDsOn();
        _delay_ms(500);

        allLEDsOff();
        _delay_ms(300);
    }
}
