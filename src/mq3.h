#ifndef MQ3_H
#define MQ3_H

void initMQ3(void);

unsigned int MQ3_readRaw(void);
unsigned int MQ3_readAverage(void);

float MQ3_getVoltage(void);
float MQ3_getBAC(void);

unsigned char MQ3_isOverLimit(void);

#endif