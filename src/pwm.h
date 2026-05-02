#ifndef PWM_H
#define PWM_H

void initPWM();

void setFrequency(unsigned int frequency);
void turnBuzzerOff();

void beepLow();
void beepMedium();
void beepHigh();

#endif
