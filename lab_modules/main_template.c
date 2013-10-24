#include <hidef.h>
#include "derivative.h"

void main(void) {
  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}
