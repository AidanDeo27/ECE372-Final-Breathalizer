#include <Arduino.h>
#include "switch.h"

const int buttonPin = 2;

volatile bool buttonInterruptFlag = false;

enum BreathState {
  WAIT_FOR_CALIBRATION,
  CALIBRATING,
  WAIT_FOR_BREATH,
  READING_BREATH
};

BreathState breathState = WAIT_FOR_CALIBRATION;

unsigned long switchTime = 0;
const unsigned long debounceDelay = 25;

ISR(INT0_vect) {
  buttonInterruptFlag = true;
}

void initSwitch() {
  pinMode(buttonPin, INPUT_PULLUP);

  // INT0 is digital pin 2 on Arduino Mega
  EICRA |= (1 << ISC01);   // falling edge
  EICRA &= ~(1 << ISC00);

  EIMSK |= (1 << INT0);    // enable INT0
}

bool buttonPressed() {
  if (buttonInterruptFlag) {
    buttonInterruptFlag = false;

    if (millis() - switchTime >= debounceDelay) {
      switchTime = millis();
      return true;
    }
  }

  return false;
}

void updateBreathState() {
  if (buttonPressed()) {
    switch (breathState) {

      case WAIT_FOR_CALIBRATION:
        breathState = CALIBRATING;
        break;

      case CALIBRATING:
        breathState = WAIT_FOR_BREATH;
        break;

      case WAIT_FOR_BREATH:
        breathState = READING_BREATH;
        break;

      case READING_BREATH:
        breathState = WAIT_FOR_CALIBRATION;
        break;
    }
  }
}

BreathState getBreathState() {
  return breathState;
}
