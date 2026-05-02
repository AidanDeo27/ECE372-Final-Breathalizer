#include <avr/io.h>
#include <avr/interrupt.h>

// main.cpp
// MQ3 Breathalyzer Prototype
// Arduino Mega 2560

#include <Arduino.h>
#include "switch.h"
#include "rtc.h"
#include "pwm.h"
#include "lcd.h"

// MQ3 sensor pin
const int mq3Pin = A0;

// Timing values
const unsigned long calibrationTime = 10000; // 10 seconds
const unsigned long breathTime = 5000;       // 5 seconds
const unsigned long resultTime = 5000;       // 5 seconds

// Breathalyzer states
enum BreathState {
  WAIT_FOR_CALIBRATION,
  CALIBRATING,
  WAIT_FOR_BREATH,
  READING_BREATH,
  DISPLAY_RESULT
};

BreathState breathState = WAIT_FOR_CALIBRATION;

unsigned long stateStartTime = 0;

int mq3Value = 0;
int peakMQ3Value = 0;

void changeState(BreathState newState) {
  breathState = newState;
  stateStartTime = millis();
}


//FLOW FOR PROJECT
/*WAIT_FOR_CALIBRATION
press button
CALIBRATING for 10 sec
WAIT_FOR_BREATH
press button
READING_BREATH for 5 sec
DISPLAY_RESULT for 5 sec
back to start
  */
void setup() {
  Serial.begin(9600);

  initSwitchPD0();
  initRTC();
  initPWM();
  initLCD();
  initADC()
  init7Seg();
  sei();
  
  turnBuzzerOff();

  clearLCD();
  printLCD("Press button");
  setLCDCursor(0, 1);
  printLCD("to calibrate");

  Serial.println("MQ3 Breathalyzer Ready");
}

void loop() {
  switch (breathState) {

    case WAIT_FOR_CALIBRATION:
      turnBuzzerOff();

      if (buttonPressed()) {
        peakMQ3Value = 0;

        clearLCD();
        printLCD("Calibrating...");
        Serial.println("Calibration started");

        changeState(CALIBRATING);
      }
      break;

    case CALIBRATING:
      beepLow();

      if (millis() - stateStartTime >= calibrationTime) {
        turnBuzzerOff();

        clearLCD();
        printLCD("Ready");
        setLCDCursor(0, 1);
        printLCD("Press to blow");

        Serial.println("Calibration complete");

        changeState(WAIT_FOR_BREATH);
      }
      break;

    case WAIT_FOR_BREATH:
      turnBuzzerOff();

      if (buttonPressed()) {
        peakMQ3Value = 0;

        clearLCD();
        printLCD("Blow now!");

        Serial.println("Breath reading started");
        printTime();

        changeState(READING_BREATH);
      }
      break;

    case READING_BREATH:
      beepHigh();

      mq3Value = analogRead(mq3Pin);

      if (mq3Value > peakMQ3Value) {
        peakMQ3Value = mq3Value;
      }

      Serial.print("MQ3 Reading: ");
      Serial.println(mq3Value);

      printMQ3TimeStamp(mq3Value);

      if (millis() - stateStartTime >= breathTime) {
        turnBuzzerOff();

        clearLCD();
        printLCD("Peak MQ3:");
        setLCDCursor(0, 1);
        printLCDNumber(peakMQ3Value);

        Serial.print("Final Peak MQ3 Value: ");
        Serial.println(peakMQ3Value);

        changeState(DISPLAY_RESULT);
      }
      break;

    case DISPLAY_RESULT:
      turnBuzzerOff();

      if (millis() - stateStartTime >= resultTime) {
        clearLCD();
        printLCD("Press button");
        setLCDCursor(0, 1);
        printLCD("to calibrate");

        changeState(WAIT_FOR_CALIBRATION);
      }
      break;
  }
}
