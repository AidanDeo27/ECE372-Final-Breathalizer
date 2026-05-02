#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "rtc.h"

RTC_DS3231 rtc;

bool rtcWorking = false;

void initRTC() {
  Wire.begin();

  if (rtc.begin()) {    //makes sure arduino knows rtc is there
    rtcWorking = true;
  } 
  else {    //prints error message if not
    rtcWorking = false;
    Serial.println("RTC not found");
  }

  if (rtcWorking && rtc.lostPower()) {   //if lost power, updates time to computers time.
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void printTime() {    //function to print time like this 5/2/2026 09:04:07
  if (!rtcWorking) {
    Serial.println("RTC not working");
    return;
  }

  DateTime now = rtc.now();

  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.year());
  Serial.print(" ");

  if (now.hour() < 10) {
    Serial.print("0");
  }
  Serial.print(now.hour());
  Serial.print(":");

  if (now.minute() < 10) {
    Serial.print("0");
  }
  Serial.print(now.minute());
  Serial.print(":");

  if (now.second() < 10) {
    Serial.print("0");
  }
  Serial.println(now.second());
}
/////////////////////////////////////////////////////////////
void printMQ3TimeStamp(int mq3Value) {    //should be able to delete this. this is the code for displaying (14:32:15 | MQ3 Reading: 510) but maybe should do it in main
  if (!rtcWorking) {
    Serial.print("MQ3 Reading: ");
    Serial.println(mq3Value);
    return;
  }

  DateTime now = rtc.now();

  Serial.print("Time: ");

  if (now.hour() < 10) {
    Serial.print("0");
  }
  Serial.print(now.hour());
  Serial.print(":");

  if (now.minute() < 10) {
    Serial.print("0");
  }
  Serial.print(now.minute());
  Serial.print(":");

  if (now.second() < 10) {
    Serial.print("0");
  }
  Serial.print(now.second());

  Serial.print(" | MQ3 Reading: ");
  Serial.println(mq3Value);
}
//////////////////////////////////////////////////////////////////////////////
//Following grabs individual pieces of the time (getter functions)
int getHour() {
  if (!rtcWorking) {
    return -1;
  }

  DateTime now = rtc.now();   //sets hour of computer to rtc hour
  return now.hour();
}

int getMinute() {
  if (!rtcWorking) {
    return -1;
  }

  DateTime now = rtc.now();
  return now.minute();
}

int getSecond() {
  if (!rtcWorking) {
    return -1;
  }

  DateTime now = rtc.now();
  return now.second();
}
