#include "EmonLib.h"                     // Include Emon Library
EnergyMonitor emonCT1;                   // Create an instance
// EnergyMonitor emonCT2;                   // Create an instance

int led = 13;

void setup() {
  Serial.begin(9600);

  // emonCT1.current(8, 111.1);             // Current: input pin, calibration.
  // emonCT2.current(8, 111.1);             // Current: input pin, calibration.
}

void loop() {
  // double Irms = emonCT1.calcIrms(1480);  // Calculate Irms only; # of samples
  double voltage = emonCT1.readVcc();

  Serial.print(voltage);
  Serial.print(" ");
  // Serial.print(Irms * 230.0);        // Apparent power
  Serial.print(" ");
  // Serial.println(Irms);          // Irms

  delay(50);
}
