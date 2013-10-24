#include <hidef.h>
#include "derivative.h"
#include "lcd.h"


void main(void) {
  char my_str[] = "Waddup Yo!";

  initialize_LCD();

  print_LCD(my_str);
  
  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}
