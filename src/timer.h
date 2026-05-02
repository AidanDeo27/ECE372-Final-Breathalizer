#ifndef timer.h
#define timer.h
#include <avr/io.h>
#include <avr/interrupt.h>

void initTimer1();
void delaySeconds(unsigned int seconds);    
void startTimer1();
void stopTimer1();

#endif