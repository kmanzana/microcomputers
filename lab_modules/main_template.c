#include <hidef.h>
#include "derivative.h"
#include "lcd.h"
#include "rti.h"

void main(void) {
  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}
