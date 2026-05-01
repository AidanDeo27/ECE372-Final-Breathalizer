#include <avr/io.h>
#include <util/delay.h>
#include "mq3.h"
#include "adc.h"

#define BAC_LIMIT 0.08

/*
    Calibration values:

    MQ3_BASELINE_ADC:
    ADC reading in clean air.

    MQ3_ADC_AT_008_BAC:
    ADC reading that your project treats as the 0.08 BAC threshold.

    These are placeholder values.
    You MUST test your sensor and replace them.
*/

// found online calibration values for MQ3 sensor

// code found online for Arduino UNO, had ai help with calibration values, but we will need to test and adjust them for our specific sensor and setup
#define MQ3_BASELINE_ADC     150
#define MQ3_ADC_AT_008_BAC   600

void initMQ3(void) {
    initADC();
}

unsigned int MQ3_readRaw(void) {
    return readADC();
}

unsigned int MQ3_readAverage(void) {
    unsigned long sum = 0;

    for (unsigned char i = 0; i < 100; i++) {
        sum += MQ3_readRaw();
        _delay_ms(10);
    }

    return sum / 100;
}

float MQ3_getVoltage(void) {
    unsigned int adcValue = MQ3_readAverage();

    return adcValue * (5.0 / 1023.0);
}

float MQ3_getBAC(void) {
    unsigned int adcValue = MQ3_readAverage();

    if (adcValue <= MQ3_BASELINE_ADC) {
        return 0.00;
    }

    /*
        Linear estimate:

        baseline ADC          -> 0.00 BAC
        MQ3_ADC_AT_008_BAC    -> 0.08 BAC

        Anything above that will calculate above 0.08.
    */
    float bac = ((float)(adcValue - MQ3_BASELINE_ADC) /
                (float)(MQ3_ADC_AT_008_BAC - MQ3_BASELINE_ADC)) * BAC_LIMIT;

    return bac;
}

unsigned char MQ3_isOverLimit(void) {
    float bac = MQ3_getBAC();

    if (bac >= BAC_LIMIT) {
        return 1;
    } else {
        return 0;
    }
}