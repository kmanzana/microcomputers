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
