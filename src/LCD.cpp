#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD pin setup: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Battery voltage divider pin
const int batteryPin = A1;

// Voltage divider resistors
// Example: R1 = 10k from battery + to A1
//          R2 = 10k from A1 to GND
const float R1 = 10000.0;
const float R2 = 10000.0;

// Arduino ADC reference voltage
const float ADC_REF = 5.0;

// 9V battery estimate range
const float BATTERY_FULL = 9.0;
const float BATTERY_EMPTY = 6.5;

void initLCD() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Breathalyzer");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1500);
  lcd.clear();
}

float readBatteryVoltage() {
  int adcValue = analogRead(batteryPin);

  float pinVoltage = (adcValue / 1023.0) * ADC_REF;

  // Undo voltage divider
  float batteryVoltage = pinVoltage * ((R1 + R2) / R2);

  return batteryVoltage;
}

int getBatteryPercent(float voltage) {
  int percent = (int)(((voltage - BATTERY_EMPTY) / 
              (BATTERY_FULL - BATTERY_EMPTY)) * 100.0);

  if (percent > 100) {
    percent = 100;
  }

  if (percent < 0) {
    percent = 0;
  }

  return percent;
}

void displayBatteryStatus() {
  float batteryVoltage = readBatteryVoltage();
  int batteryPercent = getBatteryPercent(batteryVoltage);

  lcd.setCursor(0, 0);
  lcd.print("Batt: ");
  lcd.print(batteryPercent);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("Volt: ");
  lcd.print(batteryVoltage, 2);
  lcd.print(" V   ");
}
