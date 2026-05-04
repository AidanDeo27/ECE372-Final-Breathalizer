#ifndef MQ3_H
#define MQ3_H

void initMQ3(void);

unsigned int MQ3_readRaw(void);
unsigned int MQ3_readAverage(void);

float MQ3_getVoltage(void);
float MQ3_getBAC(void);

unsigned char MQ3_isOverLimit(void);

float MQ3_calculateBAC(unsigned int adcValue);
unsigned char MQ3_isOverLimitFromRaw(unsigned int adcValue);

unsigned char MQ3_isBlowing(unsigned int adcValue);
unsigned char MQ3_isBlowingAboveBaseline(unsigned int adcValue, unsigned int baseline);

#endif