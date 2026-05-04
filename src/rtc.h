#ifndef RTC_H
#define RTC_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} RTCTime;

void initRTC(void);
unsigned char RTC_isWorking(void);
void RTC_getTime(RTCTime *time);

int getHour(void);
int getMinute(void);
int getSecond(void);

#ifdef __cplusplus
}
#endif

#endif