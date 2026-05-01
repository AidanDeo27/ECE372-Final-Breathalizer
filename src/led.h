#ifndef LED_H
#define LED_H

void initLED(void);

void turnOnLEDWithChar(unsigned char num);

void greenLEDsOn(void);
void redLEDsOn(void);
void allLEDsOff(void);

void flashRedLEDs(void);
void flashGreenLEDs(void);

#endif