#ifndef SWITCH_H
#define SWITCH_H

void initSwitch(void);
unsigned char switchPressed(void);
void waitForSwitchPress(void);
void waitForSwitchRelease(void);

#endif