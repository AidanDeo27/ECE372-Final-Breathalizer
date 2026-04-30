#ifndef SWITCH_H
#define SWITCH_H

enum BreathState {
  WAIT_FOR_CALIBRATION,
  CALIBRATING,
  WAIT_FOR_BREATH,
  READING_BREATH
};

void initSwitch();
bool buttonPressed();
void updateBreathState();
BreathState getBreathState();

#endif
