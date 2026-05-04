#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "mq3.h"
#include "sevensegment.h"
#include "switch.h"
#include "led.h"
#include "rtc.h"

typedef enum {
    STATE_WAIT_START,
    STATE_COUNTDOWN,
    STATE_READING,
    STATE_SHOW_RESULT
} BreathalyzerState;

int main(void) {
    init();   // Needed for Arduino libraries like Wire/RTClib

    BreathalyzerState state = STATE_WAIT_START;

    unsigned int rawValue = 0;
    float bac = 0.0;
    unsigned char overLimit = 0;

    RTCTime sampleTime;

    initLCD();
    initMQ3();
    init7Seg();
    initSwitch();
    initLED();
    initRTC();

    while (1) {
        switch (state) {

            case STATE_WAIT_START:
                clearDisplay();
                allLEDsOff();

                lcdClear();
                moveCursor(0, 0);
                writeString("Press button");
                moveCursor(1, 0);
                writeString("to start test");

                waitForSwitchPress();
                waitForSwitchRelease();

                state = STATE_COUNTDOWN;
                break;

            case STATE_COUNTDOWN:
                for (signed char count = 5; count > 0; count--) {
                    displayDigit(count);

                    lcdClear();
                    moveCursor(0, 0);
                    writeString("Keep blowing");

                    moveCursor(1, 0);
                    writeString("Time left: ");
                    writeCharacter(count + '0');

                    _delay_ms(1000);
                }

                clearDisplay();
                state = STATE_READING;
                break;

            case STATE_READING:
                lcdClear();
                moveCursor(0, 0);
                writeString("Reading...");
                moveCursor(1, 0);
                writeString("Please wait");

                rawValue = MQ3_readAverage();
                bac = MQ3_calculateBAC(rawValue);
                overLimit = MQ3_isOverLimitFromRaw(rawValue);

                RTC_getTime(&sampleTime);

                state = STATE_SHOW_RESULT;
                break;

            case STATE_SHOW_RESULT:
                displayBACWithTime(bac, overLimit,
                                   sampleTime.hour,
                                   sampleTime.minute,
                                   sampleTime.second);

                if (overLimit) {
                    flashRedLEDs();
                } else {
                    flashGreenLEDs();
                }

                // Keep result on screen until button is pressed again
                waitForSwitchPress();
                waitForSwitchRelease();

                state = STATE_WAIT_START;
                break;
        }
    }

    return 0;
}