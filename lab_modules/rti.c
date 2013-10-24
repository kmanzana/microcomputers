#include "rti.h"

void interrupt VectorNumber_Vrti rti_isr() {
  static char RTI_PERIOD = 1;
  static char rti_count = 0;
  int photocell_input;
  char photocell_string[80];

  clear_rti_flag();

  rti_count++;

  if (rti_count == RTI_PERIOD) {
    rti_count = 0;

    sprintf(photocell_string, "%d", ATDDR0L);
    print_LCD(photocell_string);
  }
}

// m: 0-7
// n: 1-16
void set_rti_flag_freq(char m, char n) {
  // m -> / 2^(9 + m)
  // n -> / n
  RTICTL = (m << 4) + (n - 1);
  // RTICTL = (m << 4) | (n - 1);
}

void enable_rti_interrupts() {
  CRGINT = 0x80;
}

void clear_rti_flag() {
  CRGFLG = 0x80; // acknowledge, clear RTIF flag
}
