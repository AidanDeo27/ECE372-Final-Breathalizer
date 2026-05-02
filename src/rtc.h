#ifndef RTC_H
#define RTC_H

void initRTC();

void printTime();
void printMQ3TimeStamp(int mq3Value);

int getHour();
int getMinute();
int getSecond();

#endif
