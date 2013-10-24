#include <hidef.h>
#include "derivative.h"
#include "lcd.h"

void main(void) {
  // PM1 input
  // PT7 output
  // output compare ISR


  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}


