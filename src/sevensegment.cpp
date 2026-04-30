// LAB4 ECE372
// NICK ROMAN, AIDAN DEOGRACIAS, NOAH MONROE, ANDREW GHARTEY
// COMPLETED 4/6/26
#include <avr/io.h>
#include "sevensegment.h"

/*
 * a -> PC0 -> Arduino pin 37
 * b -> PC1 -> Arduino pin 36
 * c -> PC2 -> Arduino pin 35
 * d -> PC3 -> Arduino pin 34
 * e -> PC4 -> Arduino pin 33
 * f -> PC5 -> Arduino pin 32
 * g -> PC6 -> Arduino pin 31
 */

void init7Seg(void)
{
    DDRC |= 0x7F;      // Set PC0-PC6 as outputs
    PORTC &= ~0x7F;    // Turn all segments off
}

void displayDigit(unsigned char num)
{
    unsigned char digits[10] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };

    if (num < 10) {
        PORTC = (PORTC & 0x80) | digits[num];
    }
}

void clearDisplay(void)
{
    PORTC &= ~0x7F;
}