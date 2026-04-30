// LAB4 ECE372
// NICK ROMAN, AIDAN DEOGRACIAS, NOAH MONROE, ANDREW GHARTEY
// COMPLETED 4/6/26
#include <avr/io.h>
#include "adc.h"

void initADC(void) {

    // clear any bits that are previously stored
    ADMUX = 0x00;

    // AVCC = 5V output LECTURE 15 CODE
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    // determine left or right justified (ADLAR = 0 RIGHT JUSTIFIED)
    // use right adjustment so that 8 bits are in adc low and we read them first
    ADMUX &= ~(1 << ADLAR);

    //Set ADC0 as single-ended input with MUX[5:0] = 0b00000
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4));

    // Enable ADC and disable auto triggering
    ADCSRA|= (1 << ADEN);
    ADCSRA &= ~(1 << ADATE);

    // ADC clock frequency is 16 Mhz divided by pre-scaler = 125KHz
    // Sampling rate is fs=13 f_ADC = (13 clock cycles)*125K Hz = 9.615 KHz
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // enable analog ADC pins and disable ADC0 pin digital input 
    DIDR0 |= (1 << ADC0D);
}

unsigned int readADC(void) {

    // LECTURE 15 CODE
    unsigned char low;
    unsigned char high;
    unsigned int result;

    // start the first ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait until conversion finishes
    while (ADCSRA & (1 << ADSC)) {
    }

    // Read low byte first, then high byte
    // ADCL will be read first followed by ADCH
    low = ADCL;
    high = ADCH;

    // both the low and high into one 10 bit unsigned integer
    result = ((unsigned int)high << 8) | low;

    return result;
}