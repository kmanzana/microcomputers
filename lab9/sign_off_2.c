#include <hidef.h>
#include "derivative.h"

void put_char_sci(char c);
void initialize_pll(void);
char get_char_sci(void);
void put_string_sci(char string[]);

void main(void) {
  initialize_pll();

  // SBR = fE / 16 / baud rate
  SCIBDH = 0x00;
  SCIBDL = 156; // = 24000000 / 16 / 9600 assuming 24 MHz

  // word length, parity, other configuration bits
  // often the defaults are ok
  SCICR1 = 0x00;

  // enable transmitter and receiver
  // interrupts (if desired)
  SCICR2 = 0x0C;

	EnableInterrupts;

  for (;;) {
    put_char_sci('D');
    _FEED_COP();
  }
}

void put_char_sci(char c) {
 // wait till transmit data register is empty
 while (!(SCISR1 & 0x80)) ;
 SCIDRL = c; // send character
}

char get_char_sci(void) {
  // wait until receive data register is full
  while (SCISR1 & 0x20) ;
  return SCIDRL;
}

void put_string_sci(char string[]) {
  for (i = 0; string[i] != 0; i++) {
    put_char_sci(string[i]);
  }
}

void initialize_pll(void) {
  CLKSEL_PLLSEL = 0;       // turn off PLL for source clock
  PLLCTL_PLLON  = 1;       // turn on PLL
  initialize_pll();

  // Assuming OscFreq = $8000, we mult by synr+1=3 (to get 24MHz)
  SYNR_SYN      = 2;       // set PLL multiplier
  REFDV_REFDV   = 0;       // set PLL divider assuming 24 MHz (divide by REFDV+1)

  CRGFLG_LOCKIF = 1;       // clear the lock interrupt flag

  while (!CRGFLG_LOCK) {}; // wait for the PLL to lock before continuing

  CLKSEL_PLLSEL = 1;       // select the PLL for source clock
}
