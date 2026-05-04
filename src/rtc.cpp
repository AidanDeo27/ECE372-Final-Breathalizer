#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "rtc.h"

// Use DS1307 for your module
RTC_DS1307 rtc;

static bool rtcWorking = false;

void initRTC(void) {
    Wire.begin();

    if (rtc.begin()) {
        rtcWorking = true;
    } else {
        rtcWorking = false;
        return;
    }

    // For DS1307: if clock is not running, set it to compile time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

unsigned char RTC_isWorking(void) {
    if (rtcWorking) {
        return 1;
    } else {
        return 0;
    }
}

void RTC_getTime(RTCTime *time) {
    if (!rtcWorking) {
        time->hour = 0;
        time->minute = 0;
        time->second = 0;
        return;
    }

    DateTime now = rtc.now();

    time->hour = now.hour();
    time->minute = now.minute();
    time->second = now.second();
}

int getHour(void) {
    if (!rtcWorking) {
        return -1;
    }

    DateTime now = rtc.now();
    return now.hour();
}

int getMinute(void) {
    if (!rtcWorking) {
        return -1;
    }

    DateTime now = rtc.now();
    return now.minute();
}

int getSecond(void) {
    if (!rtcWorking) {
        return -1;
    }

    DateTime now = rtc.now();
    return now.second();
}