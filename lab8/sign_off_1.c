#include <hidef.h>
#include "derivative.h"
#include "lcd.h"

void main(void) {
  // PM1 input
  // PT7 output
  // output compare ISR to generate trigger pulse every 80 ms
  // PT1 output compare




  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}


