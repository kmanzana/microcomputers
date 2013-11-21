#include <SoftwareSerial.h>
#include <serLCD.h>
#include "EmonLib.h"

serLCD lcd(1);
EnergyMonitor emonCT1;                   // Create an instance
EnergyMonitor emonCT2;

void setup() {
  lcd.setType(6);
  lcd.clear();
  // Serial.begin(9600);
}

void loop() {
  lcd.clear();

  measureAndLCDPrintVoltage();

  delay(1000);
}

void setupCurrentSensors() {
  emonCT1.current(8, 111.1);             // Current: input pin, calibration.
  emonCT2.current(8, 111.1);
}

void measureAndSerialPrintPower() {
  double Irms = emonCT1.calcIrms(1480);  // Calculate Irms only; # of samples
  double voltage = emonCT1.readVcc();

  Serial.print(voltage);
  Serial.print(" ");
  Serial.print(Irms * 230.0);    // Apparent power
  Serial.print(" ");
  Serial.println(Irms);          // Irms

  delay(50);
}

void measureAndLCDPrintVoltage() {
  int voltage = emonCT1.readVcc();

  lcd.print("Voltage: ");
  lcd.print(voltage);
  lcd.print(" mV");
}
