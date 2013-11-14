#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

void initialize_spi(void);
void put_char_spi(char cx);
char get_char_spi(void);
void write_byte(char byte);
void read_byte(void);
void delay_u_sec(unsigned int u_seconds);

char test_byte;

void main(void) {
  DDRT |= PTT_PTT0_MASK; // PT0 for output
  DDRM |= PTM_PTM4_MASK | PTM_PTM5_MASK; // PM4 and PM5 for output
  DDRM &= ~PTM_PTM2_MASK; // PM2 for input

  initialize_spi();

  write_byte('D');
  read_byte();

  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}

void write_byte(char byte) {
  PTT_PTT0 = 0; // Select slave, pull /CS low to select chip
  put_char_spi(0x06); // Select â€œwrite enableâ€ instruction 0x06 on SI
  PTT_PTT0 = 1; // Deselect slave, pull /CS high to deselect chip

  PTT_PTT0 = 0; // Select slave, pull /CS low to select chip
  put_char_spi(0x02); // Send write instruction 0x02 on SI
  put_char_spi(0x00); // Send MSB of address
  put_char_spi(0x00); // Send LSB of address
  put_char_spi(byte); // Send data byte
  PTT_PTT0 = 1; // Deselect slave, pull /CS high to deselect chip

  delay_u_sec(5000); // Delay 5 ms
}

void read_byte(void) {
  PTT_PTT0 = 0;       // Select slave, pull /CS low to select // chip
  put_char_spi(0x03);       // Send read instruction 0x03
  put_char_spi(0x00); // Send MSB of address
  put_char_spi(0x00); // Send LSB of address
  test_byte = get_char_spi(); //Get data byte
  PTT_PTT0 = 1;       // Deselect slave, pull /CS high to deselect chip
}

// Send a character out through SPI
void put_char_spi(char cx) {
  char temp;
  while(!(SPISR & 0x20)); // wait until SPTEF=1 (transmit reg empty)
  SPIDR = cx;             // output the byte to the SPI
  while(!(SPISR & 0x80)); // wait until SPIF=1 (receive reg full)
  temp = SPIDR;           // clear the SPIF flag
}

// Get a character from SPI
char get_char_spi(void) {
  while(!(SPISR & 0x20)); // wait until SPTEF=1 (transmit reg empty)
  SPIDR = 0x00;           // trigger 8 SCK pulses to shift in data

  while(!(SPISR & 0x80)); // wait until SPIF=1(receive reg full)
  return SPIDR;           // return the character
}

// Initialize SPI system
void initialize_spi(void) {
  // Set baud rate. Baud rate divisor = (SPPR+1) * 2^(SPR+1).
  // for baud = 1 MHz   divisor = 24   SPPR=5, SPR=1 -> 0x51
  SPIBR = 0x51; // Bits: 0 SP PR2 SPPR1 SPPR0 0 SPR2 SPR1 SPR0

  // SPICR1 bits: SPIE SPTIE SPE MSTR CP OL CPHA SSOE LSBFE
  // SPIE = 0 no interrupts
  // SPE = 1 enable SPI system
  // SPTIE = 0 no interrupts
  // MSTR = 1 we are master
  // CPOL = 0 data latched on rising edge of clock
  // CPHA = 0 data starts with first edge
  // SSOE = 0 set to 1 to enable SS output
  // LSBF = 0 transfer most signif bit first
  SPICR1 = 0x50;

  // SPICR2 bits: 0 0 0 MODF EN BIDIROE 0 SPSWAI SPC0
  // MODFEN = 0 set to 1 to allow SS to go low on transmission
  // BIDIROE = 0 disable bidirectional mode
  // SPSWAI = 0 SPI clock operates normally in wait mode
  // SPC0 = 0 only used for bidirectional mode
  SPICR2 = 0x00;

  PTT_PTT0 = 1; // deselect slave to start
}

void delay_u_sec(unsigned int u_seconds) {
  unsigned int i;

  for (i = 0; i < u_seconds; i++) {
    __asm {
        ; Main loop is 24 cycles, or 1 usec
        ldx #1 ; 3 additional cycles here
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
