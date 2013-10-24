#include "lcd.h"

void initialize_LCD(void) {
  int i;

  for (i = 0; i < 100; i++) // delay 100ms to allow LCD powerup
    delay_u_sec(1000);

  // The first parameter in each call is the nibble to be sent,
  // the second parameter is the rs value (rs=0 indicates an instruction),
  // and the third parameter is the time to delay after sending (in units of us).
  write_nibble_to_LCD(0x03, 0, 5000); // delay at least 4 ms = 4000 us
  write_nibble_to_LCD(0x03, 0, 5000);
  write_nibble_to_LCD(0x03, 0, 5000);
  write_nibble_to_LCD(0x02, 0, 5000);

  // The first parameter in each call is the byte to be sent (as two nibbles),
  // the second parameter is the rs value (rs=0 indicates an instruction),
  // and the 3rdparameter is the time to delay after sending both nibbles (usec).
  // These commands are all fast (~40 us) except for "clear display" (2 ms)
  write_byte_to_LCD(0x28,0,50); // 2 lines, 5x8 dots
  write_byte_to_LCD(0x0c,0,50); // display on, no cursor, no blinking
  write_byte_to_LCD(0x14,0,50); // shift cursor right
  write_byte_to_LCD(0x01,0,2000); // clear display and cursor home
}

void write_nibble_to_LCD(char n, char rs, int t) {
  rs <<= 5; // get rs bit into the bit 5 position
  PTT = rs | 0x10 | (0x0f & n); // output the nibble with E=1
  delay_u_sec(1); // keep E pulse high a little while
  PTT &= ~0x10; // make E go to 0
  delay_u_sec(t);
}

void write_byte_to_LCD(char b, char rs, int t) {
  // delay 1 us between nibbles to meet minimum cycle time limit
  write_nibble_to_LCD((0x0f & (b >> 4)), rs, 1);
  write_nibble_to_LCD((0x0f & b), rs, t);
}

void print_LCD(char mystr[]) {
  int i;
  char ch;

  write_byte_to_LCD(0x01,0,2000); // clear display and cursor home

  for (i = 0; i < 80; i++) {
    ch = mystr[i];
    if (ch == 0) break;
    write_byte_to_LCD(ch, 1, 50);
  }
}
