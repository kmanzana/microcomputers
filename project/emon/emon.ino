#include <SoftwareSerial.h>
#include <serLCD.h>
#include "EmonLib.h"

void setupCurrentSensors();

serLCD        lcd(1); // pin to transmit to serLCD backpack (1 = tx)
EnergyMonitor emonCT1;
EnergyMonitor emonCT2;

void setup() {
  lcd.setType(5);
  lcd.clear();
  lcd.setBrightness(30);
  // setContrast(0x20);

  setupCurrentSensors();
}

void loop() {
  lcd.setCursor(1, 1);

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

  lcd.print("EMon: ");
  lcd.print(voltage);
  lcd.print("       ");
  lcd.print(irms);          // Irms
  lcd.print("       ");
  lcd.print(irms * 120.0);    // Apparent power
  lcd.print("       ");

  delay(50);
}

void setContrast(int contrast) {
   lcd.write(254);
   lcd.write(80);
   lcd.write(contrast);
}

void lcdPrintLine(char string[]) {

}

