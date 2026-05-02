#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "switch.h"
#include "rtc.h"
#include "pwm.h"
#include "lcd.h"
#include "adc.h"
#include "sevensegment.h"
#include "timer.h"

// ─── Constants ───────────────────────────────────────────────────────────────
const int     MQ3_PIN          = A0;
const uint8_t REQUIRED_BLOW_S  = 8;
const int    MQ3_THRESHOLD    = 100;  // Adjust this threshold based on testing

// ─── State Machine ───────────────────────────────────────────────────────────
enum BreathState {
    WAIT_FOR_CALIBRATION,
    CALIBRATING,
    WAIT_FOR_BREATH,
    READING_BREATH,
    DISPLAY_RESULT,
};

BreathState breathState = WAIT_FOR_CALIBRATION;

// ─── Globals ─────────────────────────────────────────────────────────────────
int mq3Value     = 0;
int peakMQ3Value = 0;

// ─── Main ────────────────────────────────────────────────────────────────────
int main() {
    // Initializations
    Serial.begin(9600);
    initSwitchPD0();
    initRTC();
    initPWM();
    initLCD();
    initADC();
    init7Seg();
    timer1_init();
    sei();

    turnBuzzerOff();
    clearLCD();
    printLCD("Press button");
    setLCDCursor(0, 1);
    printLCD("to calibrate");

    Serial.println("MQ3 Breathalyzer Ready");

    // Main loop
    while (1) {
        switch (breathState) {

            case WAIT_FOR_CALIBRATION:
                turnBuzzerOff();

                if (buttonPressed()) {
                    peakMQ3Value = 0;
                    clearLCD();
                    printLCD("Calibrating...");
                    Serial.println("Calibration started");
                    timer1_start();
                    breathState = CALIBRATING;
                }
                break;

            case CALIBRATING:
                beepLow();
                display7Seg(REQUIRED_BLOW_S - seconds_blown);

                if (seconds_blown >= 10) {   // 10 second calibration
                    timer1_stop();
                    timer1_reset();
                    turnBuzzerOff();
                    clearLCD();
                    printLCD("Ready");
                    setLCDCursor(0, 1);
                    printLCD("Press to blow");
                    Serial.println("Calibration complete");
                    breathState = WAIT_FOR_BREATH;
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
                    timer1_start();
                    breathState = READING_BREATH;
                }
                break;

            case READING_BREATH:
                beepHigh();
                display7Seg(REQUIRED_BLOW_S - seconds_blown);  // countdown

                mq3Value = analogRead(MQ3_PIN);
                if (mq3Value > peakMQ3Value) {
                    peakMQ3Value = mq3Value;
                }

                Serial.print("MQ3 Reading: ");
                Serial.println(mq3Value);
                printMQ3TimeStamp(mq3Value);

                //FINISH , CHCECK DISCORD FOR PROBLEM EXPLAINED, maybe do this if with an & condition to check for the sensor state. 
                if (seconds_blown < REQUIRED_BLOW_S && mq3Value < MQ3_THRESHOLD){ 
                  timer1_stop();
                  timer1_reset();
                  turnBuzzerOff();//seconds blown is less than required and the user stop blowing.
                  display7Seg(0);  // show 0 if they haven't blown long enough
                  clearLCD();
                  printLCD("Invalid blow");
                  setLCDCursor(0, 1);
                  printLCD("Please Retry");
                  peakMQ3Value = 0;
                  breathState = WAIT_FOR_BREATH; //goes back to waiting for breath, they can try again if they didn't blow long enough or if they stopped blowing before the required time.
                  break;
                }

                if (seconds_blown >= REQUIRED_BLOW_S) {
                    timer1_stop();
                    timer1_reset();
                    turnBuzzerOff();
                    clearLCD();
                    printLCD("Peak MQ3:");
                    setLCDCursor(0, 1);
                    printLCDNumber(peakMQ3Value);
                    Serial.print("Final Peak MQ3 Value: ");
                    Serial.println(peakMQ3Value);
                    breathState = DISPLAY_RESULT;
                }
                break;

            case DISPLAY_RESULT:
                turnBuzzerOff();

                if (seconds_blown >= 5) {    // show result for 5 seconds
                    timer1_stop();
                    timer1_reset();
                    clearLCD();
                    printLCD("Press button");
                    setLCDCursor(0, 1);
                    printLCD("to calibrate");
                    breathState = WAIT_FOR_CALIBRATION;
                }
                break;
        }
    }

    return 0;
}