#include "misc.h"

void delay_u_sec(int u_seconds) {
  int i;
  for (i = 0; i < u_seconds; i++) {
    __asm {
        ; Main loop is 24 cycles, or 1 usec
        ldx #10 ; 3 additional cycles here
  loop: psha ; 2 E cycles
        pula ; 3 E cycles
        psha ; 2 E cycles
        pula ; 3 E cycles
        psha ; 2 E cycles
        pula ; 3 E cycles
        psha ; 2 E cycles
        pula ; 3 E cycles
        nop ; 1 E cycle
        dbne x,loop ; 3 E cycles
    }
  }
}

void initialize_AD_converter() {
  ATDCTL2 = 0xc0; // enable ATD and fast flag clear
  ATDCTL3 = 0x08; // set ATD for 1 channel conversion
  ATDCTL4 = 0x85; // set ATD for 2 MHz,2 sample clks,8 bits
  ATDCTL5 = 0xa2; // right justif, continuous conv, use AN02
  // A/D results appear in ATDDR0L
}
