#include <hidef.h> /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include <stdio.h>

unsigned int T1;
char my_string[80];
unsigned long PWIDTH, distance;

void delay_u_sec(int micro_seconds) {
  __asm {
          ldx micro_seconds ; 3 additional cycles here
    loop: psha  ; 2 E cycles
          pula  ; 3 E cycles
          psha  ; 2 E cycles
          pula  ; 3 E cycles
          psha  ; 2 E cycles
          pula  ; 3 E cycles
          psha  ; 2 E cycles
          pula  ; 3 E cycles
          nop   ; 1 E cycle
          dbne x, loop ; 3 E cycles
  }
}

void write_nibble_to_lcd(char n, char rs, int t) {
  rs <<= 5; // get rs bit into the bit 5 position
  PTT = rs | 0x10 | (0x0f & n); // output the nibble with E=1
  delay_u_sec(1); // keep E pulse high a little while
  PTT &= ~0x10; // make E go to 0
  delay_u_sec(t);
}

void write_byte_to_lcd(char b, char rs, int t) {
  // delay 1 us between nibbles to meet minimum cycle time limit
  write_nibble_to_lcd( (0x0f & (b >> 4)), rs, 1);
  write_nibble_to_lcd( (0x0f & b), rs, t);
}

void init_lcd(void) {
  int i;

  for (i = 0; i < 100; i++) // delay 100ms to allow LCD powerup
    delay_u_sec(1000); // The first parameter in each call is the nibble to be sent,

  // the second parameter is the rs value (rs=0 indicates an instruction),
  // and the third parameter is the time to delay after sending (in units of us).
  write_nibble_to_lcd(0x03, 0, 5000); // delay at least 4 ms = 4000 us
  write_nibble_to_lcd(0x03, 0, 5000);
  write_nibble_to_lcd(0x03, 0, 5000);
  write_nibble_to_lcd(0x02, 0, 5000);

  // The first parameter in each call is the byte to be sent (as two nibbles),
  // the second parameter is the rs value (rs=0 indicates an instruction),
  // and the 3rdparameter is the time to delay after sending both nibbles (usec).
  // These commands are all fast (~40 us) except for "clear display" (2 ms)
  write_byte_to_lcd(0x28, 0, 50); // 2 lines, 5x8 dots
  write_byte_to_lcd(0x0c, 0, 50); // display on, nocursor, no blinking
  write_byte_to_lcd(0x14, 0, 50);

  // shift cursor right
  write_byte_to_lcd(0x01, 0, 2000); // clear display and cursor home
}

void print_lcd(char my_string[]) {
  int i;
  char ch;

  write_byte_to_lcd(0x01,0,2000); // clear display and cursor home

  for (i = 0; i < 80; i++) {
    ch = my_string[i];

    if (ch == 0) break;

    write_byte_to_lcd(ch,1,50);
  }
}

// IC7 ISR
void interrupt VectorNumber_Vtimch7 ic7_isr(void) {
  // Test bit EDG3A to see what IC3 was configured to do
  if (TCTL3 & 0x40) {
    // EDG3A was a 1, meaning IC3 was configured to look for rising
    T1 = TC7;
    TCTL3 = 0x80; // EDG3B:EDG3A = 1:0 to look for falling edge next
  } else {
    // EDG3A was a 0, meaning IC3 was configured to look for falling
    PWIDTH = TC7 - T1;
    TCTL3 = 0x40; // EDG3B:EDG3A = 0:1 to look for rising edge next
  }

  distance = PWIDTH * 1330 / 1000 / 58;

  if (distance < 300) {
    sprintf(my_string, "Value is %ld cm", distance);
    print_lcd(my_string);
  } else {
    print_lcd("Out of Range");
  }
}

// OC6 ISR
void interrupt VectorNumber_Vtimch6 oc6_isr(void) {
  // Test bit OL6 to see what OC6 was configured to do
  if (TCTL1 & 0x10) {
    // OL6 was a 1, meaning OC6 went high upon compare
    TC6 = TC6 + 8; // This is about 10 us
    TCTL1 = 0x20; // OM2:OL2 = 1:0 to go low next
  } else {
    // OL6 was a 0, meaning OC6 went low upon compare
    TC6 = TC6 + 60150; // This is about 80 ms
    TCTL1 = 0x30; // OM2:OL2 = 1:1 to go high next
  }
}

void main(void) {
  DDRT = 0x7f;
  DDRM &= ~0x02;

  init_lcd();

  TSCR1 = 0x90; // enable timer counter, enable fast flag clear
  TSCR2 = 0x05; // disable TCNT overflow interrupt, set prescaler=32

  // Set up input capture on channel 7.
  TIOS &= ~0x80; // enable input-capture 7
  TCTL3 = 0x40; // capture the rising edge of the PT7 pin
  TFLG1 = 0x80; // clear the C7F flag
  TIE |= 0x80; // enable interrupts on channel 7

  // Set up output compare on channel 6.
  TIOS |= 0x40; // enable output compare channel 6
  TCTL1 = 0x30; // set OC6 pin action to go high
  TFLG1 = 0x40; // clear the C6F flag
  TIE |= 0x40; // enable interrupts on channel 6

  EnableInterrupts;

  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
}
