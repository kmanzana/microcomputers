#include <SoftwareSerial.h>
#include <serLCD.h>
#include "EmonLib.h"

void setupCurrentSensors();

serLCD        lcd(1); // pin to transmit to serLCD backpack (1 = tx)
EnergyMonitor emonCT1;
EnergyMonitor emonCT2;

void setup() {
  lcd.setType(5); // for 4x20 screen
  lcd.clear();
  lcd.setBrightness(30); // full brightness = 30, no brightness = 1
  // setContrast(0x20);

  setupCurrentSensors();
}

void loop() {
  printMeasuredPower();
  delay(1000);
}

void setupCurrentSensors() {
  emonCT1.current(A1, 57.9); // Current: input pin, calibration; orig = 60.6
  emonCT2.current(A2, 57.9);
}

void printMeasuredPower() {
  double irms = emonCT1.calcIrms(1480) + emonCT2.calcIrms(1480);
  double voltage = emonCT1.readVcc();

  lcd.selectLine(1);
  lcd.print("        EMon");

  lcd.selectLine(2);
  lcd.print("Voltage: ")
  lcd.print(voltage);
  lcd.print(" V   ");

  lcd.selectLine(3);
  lcd.print("Current: ")
  lcd.print(irms);
  lcd.print(" A (rms)   ");

  lcd.selectLine(4);
  lcd.print("Power: ")
  lcd.print(irms * 120.0);
  lcd.print(" W (apparent)   ");

  delay(50);
}

void setContrast(int contrast) {
   lcd.write(254);
   lcd.write(80);
   lcd.write(contrast);
}

void lcdPrintLine(char string[]) {

}

