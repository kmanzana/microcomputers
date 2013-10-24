#include <hidef.h>
#include "derivative.h"
#include "lcd.h"


void main(void) {
  int photocell_input;
  char photocell_string[80];

  initialize_LCD();

  initialize_AD_converter();

  EnableInterrupts;

  for(;;) {
      photocell_input = ATDDR0L;
    sprintf(photocell_string, "%d", photocell_input);
    print_LCD(photocell_string);
    delay_u_sec(50000);
    _FEED_COP();
  }
}
