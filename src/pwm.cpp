// pwm.cpp
// MQ3 Breathalyzer Project
// Uses Timer4 PWM on OC4C, Arduino Mega pin 8

#include <Arduino.h>
#include <avr/io.h>
#include "pwm.h"

void initPWM() {
  // OC4C is PH5, Arduino Mega digital pin 8
  DDRH |= (1 << DDH5);

  // Clear Timer4 control registers
  TCCR4A = 0x00;
  TCCR4B = 0x00;

  // Fast PWM Mode 15
  // WGM43:0 = 1111
  // TOP = OCR4A
  TCCR4A |= (1 << WGM40) | (1 << WGM41);
  TCCR4B |= (1 << WGM42) | (1 << WGM43);

  // Non-inverting PWM on OC4C
  TCCR4A |= (1 << COM4C1);
  TCCR4A &= ~(1 << COM4C0);

  // Prescaler = 1
  TCCR4B |= (1 << CS40);

  // Start with buzzer off
  OCR4A = 0;
  OCR4C = 0;
}

void setFrequency(unsigned int frequency) {
  if (frequency == 0) {
    // Turn off PWM output
    TCCR4A &= ~(1 << COM4C1);
    OCR4A = 0;
    OCR4C = 0;
    return;
  }

  // Reconnect PWM output
  TCCR4A |= (1 << COM4C1);

  // Formula:
  // frequency = 16 MHz / (prescaler * (1 + OCR4A))
  unsigned long top = (16000000UL / frequency) - 1;

  OCR4A = (unsigned int)top;

  // 50% duty cycle
  OCR4C = OCR4A / 2;
}

void turnBuzzerOff() {
  setFrequency(0);
}

void beepLow() {
  setFrequency(500);
}

void beepMedium() {
  setFrequency(1000);
}

void beepHigh() {
  setFrequency(2000);
}
