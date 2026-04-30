#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

void initLCD();

float readBatteryVoltage();

int getBatteryPercent(float voltage);

void displayBatteryStatus();

#endif
