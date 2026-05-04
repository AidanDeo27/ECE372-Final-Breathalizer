#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

/*
LCD wiring for Arduino Mega:

RS  -> pin 7  -> PH4
E   -> pin 8  -> PH5
D4  -> pin 9  -> PH6
D5  -> pin 10 -> PB4
D6  -> pin 11 -> PB5
D7  -> pin 12 -> PB6

RW  -> GND
VO  -> GND or potentiometer
*/

// LCD pin definitions
#define LCD_RS PH4
#define LCD_E  PH5
#define LCD_D4 PH6
#define LCD_D5 PB4
#define LCD_D6 PB5
#define LCD_D7 PB6

// Private helper prototypes
static void pulseEnable(void);
static void sendNibble(unsigned char nibble);
static void sendCommand(unsigned char command);
static void lcdPrintUnsignedInt(unsigned int number);
static void lcdPrintBAC(float bac);
static void lcdPrintTwoDigits(unsigned char value);
static void clearLine(unsigned char row);

// Pulse the LCD enable pin
static void pulseEnable(void) {
    PORTH |= (1 << LCD_E);
    _delay_us(1);
    PORTH &= ~(1 << LCD_E);
    _delay_us(100);
}

// Send 4 bits to LCD data pins D4-D7
static void sendNibble(unsigned char nibble) {
    // LCD D4 -> PH6
    if (nibble & 0x01) {
        PORTH |= (1 << LCD_D4);
    } else {
        PORTH &= ~(1 << LCD_D4);
    }

    // LCD D5 -> PB4
    if (nibble & 0x02) {
        PORTB |= (1 << LCD_D5);
    } else {
        PORTB &= ~(1 << LCD_D5);
    }

    // LCD D6 -> PB5
    if (nibble & 0x04) {
        PORTB |= (1 << LCD_D6);
    } else {
        PORTB &= ~(1 << LCD_D6);
    }

    // LCD D7 -> PB6
    if (nibble & 0x08) {
        PORTB |= (1 << LCD_D7);
    } else {
        PORTB &= ~(1 << LCD_D7);
    }

    pulseEnable();
}

// Send command byte to LCD
static void sendCommand(unsigned char command) {
    PORTH &= ~(1 << LCD_RS);      // RS = 0 for command

    sendNibble(command >> 4);     // upper nibble
    sendNibble(command & 0x0F);   // lower nibble

    _delay_ms(2);
}

// Clear entire LCD
void lcdClear(void) {
    sendCommand(0x01);
    _delay_ms(3);
}

// Move cursor to row and column
void moveCursor(unsigned char row, unsigned char col) {
    unsigned char address;

    if (row == 0) {
        address = 0x80 + col;
    } else {
        address = 0xC0 + col;
    }

    sendCommand(address);
}

// Write one character to LCD
void writeCharacter(unsigned char character) {
    PORTH |= (1 << LCD_RS);       // RS = 1 for data

    sendNibble(character >> 4);
    sendNibble(character & 0x0F);

    _delay_us(100);
}

// Write null-terminated string to LCD
void writeString(const char *string) {
    while (*string != '\0') {
        writeCharacter(*string);
        string++;
    }
}

// Initialize LCD
void initLCD(void) {
    // Set LCD pins as outputs
    DDRH |= (1 << LCD_RS) | (1 << LCD_E) | (1 << LCD_D4);
    DDRB |= (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);

    // Make sure control lines start low
    PORTH &= ~((1 << LCD_RS) | (1 << LCD_E));

    _delay_ms(50);

    // LCD 4-bit initialization sequence
    sendNibble(0x03);
    _delay_ms(5);

    sendNibble(0x03);
    _delay_us(150);

    sendNibble(0x03);
    _delay_us(150);

    sendNibble(0x02); // Set 4-bit mode
    _delay_us(150);

    // Function set: 4-bit, 2-line, 5x8 font
    sendCommand(0x28);

    // Display off
    sendCommand(0x08);

    // Clear display
    lcdClear();

    // Entry mode: cursor moves right
    sendCommand(0x06);

    // Display on, cursor off, blink off
    sendCommand(0x0C);

    lcdClear();

    moveCursor(0, 0);
    writeString("Breathalyzer");

    moveCursor(1, 0);
    writeString("Starting...");

    _delay_ms(1500);
    lcdClear();
}

// Print unsigned integer
static void lcdPrintUnsignedInt(unsigned int number) {
    char buffer[6];
    unsigned char i = 0;

    if (number == 0) {
        writeCharacter('0');
        return;
    }

    while (number > 0 && i < 5) {
        buffer[i] = (number % 10) + '0';
        number /= 10;
        i++;
    }

    while (i > 0) {
        i--;
        writeCharacter(buffer[i]);
    }
}

// Print BAC as 0.00 through 0.99
static void lcdPrintBAC(float bac) {
    unsigned int bacTimes100;

    if (bac < 0) {
        bac = 0;
    }

    if (bac > 0.99) {
        bac = 0.99;
    }

    bacTimes100 = (unsigned int)((bac * 100.0) + 0.5);

    writeCharacter('0');
    writeCharacter('.');
    writeCharacter((bacTimes100 / 10) + '0');
    writeCharacter((bacTimes100 % 10) + '0');
}

// Print value as two digits, e.g. 4 -> 04
static void lcdPrintTwoDigits(unsigned char value) {
    writeCharacter((value / 10) + '0');
    writeCharacter((value % 10) + '0');
}

// Clear one LCD line with spaces
static void clearLine(unsigned char row) {
    moveCursor(row, 0);
    writeString("                ");  // 16 spaces
    moveCursor(row, 0);
}

// Display raw ADC, BAC, and status
void displayAlcoholStatus(unsigned int rawValue, float bac, unsigned char overLimit) {
    lcdClear();
    _delay_ms(3);

    clearLine(0);
    moveCursor(0, 0);
    writeString("Raw:");
    lcdPrintUnsignedInt(rawValue);

    clearLine(1);
    moveCursor(1, 0);
    writeString("BAC:");
    lcdPrintBAC(bac);

    moveCursor(1, 9);

    if (overLimit) {
        writeString("OVER");
    } else {
        writeString("SAFE");
    }
}

// Display BAC with RTC sample time
void displayBACWithTime(float bac, unsigned char overLimit,
                        unsigned char hour, unsigned char minute, unsigned char second) {
    lcdClear();
    _delay_ms(3);

    clearLine(0);
    moveCursor(0, 0);
    writeString("BAC:");
    lcdPrintBAC(bac);

    moveCursor(0, 9);
    if (overLimit) {
        writeString("OVER");
    } else {
        writeString("SAFE");
    }

    clearLine(1);
    moveCursor(1, 0);
    writeString("T:");
    lcdPrintTwoDigits(hour);
    writeCharacter(':');
    lcdPrintTwoDigits(minute);
    writeCharacter(':');
    lcdPrintTwoDigits(second);
}