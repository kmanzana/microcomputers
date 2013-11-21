#include <SoftwareSerial.h>
#include <serLCD.h>
#include "EmonLib.h"

void setupLCD();
void clearLCD();

serLCD lcd(1);
EnergyMonitor emonCT1;                   // Create an instance
EnergyMonitor emonCT2;

// Attach the serial display's RX line to digital pin 2
SoftwareSerial mySerial(0, 1); // pin 3 = RX (unused), pin 2 = TX

void setup() {
  lcd.setType(6);
  lcd.clear();
  // Serial.begin(9600);

  // setupLCD();
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
  char voltageString[20];
  int voltage = emonCT1.readVcc();

  // mySerial.print("Voltage: ");
  // mySerial.print(voltage);
  // mySerial.print(" mV");
  lcd.print("Voltage: ");
  lcd.print(voltage);
  lcd.print(" mV");
}

void setupLCD() {
  mySerial.begin(9600);
  delay(500); // wait for display to boot up

  // mySerial.write(124);
  // mySerial.write(0x03);
}

void clearLCD() {
  // mySerial.write(254); // move cursor to beginning of first line
  // mySerial.write(128);

  // mySerial.write("                    "); // clear display
  // mySerial.write("                    ");
  // mySerial.write("                    ");
  // mySerial.write("                    ");

  mySerial.write(0xFE);
  mySerial.write(0x01);

  mySerial.write(254); // move cursor to beginning of first line
  mySerial.write(128);
}
