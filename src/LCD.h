#ifndef LCD_H
#define LCD_H

void initLCD(void);

void lcdClear(void);
void moveCursor(unsigned char row, unsigned char col);
void writeCharacter(unsigned char character);
void writeString(const char *string);

void displayAlcoholStatus(unsigned int rawValue, float bac, unsigned char overLimit);

void displayBACWithTime(float bac, unsigned char overLimit,
                        unsigned char hour, unsigned char minute, unsigned char second);

#endif