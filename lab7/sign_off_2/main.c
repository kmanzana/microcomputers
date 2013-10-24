#include <hidef.h>
#include "derivative.h"
#include "lcd.h"

void main(void) {
  int photocell_input;
  char photocell_string[80];

  initialize_AD_converter();

  photocell_input = ATDDR0L;
  sprintf(photocell_string, "%d", photocell_input);
  print_LCD(photocell_string);

  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}


