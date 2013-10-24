#include <hidef.h>
#include "derivative.h"
#include "lcd.h"
#include "rti.h"


void main(void) {
  char RTI_M = 7;
  char RTI_N = 16;

  initialize_LCD();

  initialize_AD_converter();

  set_rti_flag_freq(RTI_M, RTI_N);
  enable_rti_interrupts();

  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}
