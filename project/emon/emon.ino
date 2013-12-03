#include <SoftwareSerial.h>
#include <serLCD.h>
#include "EmonLib.h"

void setupCurrentSensors();

serLCD lcd(1);
EnergyMonitor emonCT1;                   // Create an instance
EnergyMonitor emonCT2;

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  lcd.setType(5);
  lcd.clear();
  lcd.setBrightness(30);

  // setContrast(0x20);
  // Serial.begin(9600);
  setupCurrentSensors();
}

void loop() {
  lcd.clear();
  lcd.setCursor(1, 1);

  // measureAndLCDPrintVoltage();
  measureAndLCDPrintPower();
  // lcd.print(5.0 * analogRead(A2) / 1024.0);

  delay(1000);
}

void setupCurrentSensors() {
  emonCT1.current(A1, 57.9);             // Current: input pin, calibration. 60.6
  emonCT2.current(A2, 57.9);
}

void measureAndLCDPrintPower() {
  double Irms = emonCT1.calcIrms(1480) + emonCT2.calcIrms(1480);  // Calculate Irms only; # of samples
  double voltage = (emonCT1.readVcc() + emonCT1.readVcc()) / 2;

  lcd.print("EMon: ");
  lcd.print(voltage);
  lcd.print("       ");
  lcd.print(Irms * 120.0);    // Apparent power
  lcd.print("       ");
  lcd.print(Irms);          // Irms
  lcd.print("       ");
  lcd.print(analogRead(A1));
  lcd.print("       ");
  lcd.print(analogRead(A2));

  delay(50);
}

void measureAndLCDPrintVoltage() {
  float voltage = emonCT1.readVcc();

  lcd.print("Voltage: ");
  lcd.print(voltage);
  lcd.print(" mV");
}

void setContrast(int contrast) {
   lcd.write(254);
   lcd.write(80);
   lcd.write(contrast);
}

void lcdPrintLine(char string[]) {

}

