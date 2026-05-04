#ifndef TIMER_H
#define TIMER_H

extern volatile unsigned int seconds_blown;

void initTimer1(void);
void startTimer1(void);
void stopTimer1(void);
void delaySeconds(unsigned int seconds);

#endif