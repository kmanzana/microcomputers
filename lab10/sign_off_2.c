#include <hidef.h>
#include "derivative.h"
#include <stdio.h>

void initialize_spi(void);
void put_char_spi(char cx);
char get_char_spi(void);
void write_byte(char byte, unsigned int address);
char read_byte(unsigned int address);

void put_string_sci(char string[]);
void put_char_sci(char c);
char get_char_sci(void);
void initialize_sci(void);
void initialize_pll(void);

void delay_u_sec(unsigned int u_seconds);

char test_byte;

void main(void) {
  char char_from_pc;
  char phrase[80];
  char memory_pointer = 0x0000;
  unsigned int i;

  initialize_pll();

  DDRT |= PTT_PTT0_MASK; // PT0 for output
  DDRM |= PTM_PTM4_MASK | PTM_PTM5_MASK; // PM4 and PM5 for output
  DDRM &= ~PTM_PTM2_MASK; // PM2 for input

  initialize_spi();
  initialize_sci();

  write_byte('D', 0x0000);
  read_byte(0x0000);

  EnableInterrupts;

  for(;;) {
    char_from_pc = get_char_sci();
    put_char_sci(char_from_pc); // echo it

    if (char_from_pc == 0xD) { // \r ?
      put_string_sci("\r\n"); // give some space to print out new string

      for (i = 0x0000; i < memory_pointer; i++) {
        phrase[i] = read_byte(i);
      }

      phrase[memory_pointer] = 0;
      put_string_sci(phrase);
      put_string_sci("\r\n");

      memory_pointer = 0;
    } else {
      write_byte(char_from_pc, memory_pointer);
      ++memory_pointer;
    }

    _FEED_COP();
  }
}

void write_byte(char byte, unsigned int address) {
  PTT_PTT0 = 0; // Select slave, pull /CS low to select chip
  put_char_spi(0x06); // Select â€œwrite enableâ€ instruction 0x06 on SI
  PTT_PTT0 = 1; // Deselect slave, pull /CS high to deselect chip

  PTT_PTT0 = 0; // Select slave, pull /CS low to select chip
  put_char_spi(0x02); // Send write instruction 0x02 on SI
  put_char_spi(address >> 8); // Send MSB of address
  put_char_spi(address & 0xFF); // Send LSB of address
  put_char_spi(byte); // Send data byte
  PTT_PTT0 = 1; // Deselect slave, pull /CS high to deselect chip

  delay_u_sec(5000); // Delay 5 ms
}

char read_byte(unsigned int address) {
  PTT_PTT0 = 0;       // Select slave, pull /CS low to select // chip
  put_char_spi(0x03);       // Send read instruction 0x03
  put_char_spi(address >> 8); // Send MSB of address
  put_char_spi(address & 0xFF); // Send LSB of address
  test_byte = get_char_spi(); //Get data byte
  PTT_PTT0 = 1;       // Deselect slave, pull /CS high to deselect chip

  return test_byte;
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

void put_string_sci(char string[]) {
  int i;

  for (i = 0; string[i] != 0; i++) {
    put_char_sci(string[i]);
  }
}

void put_char_sci(char c) {
 // wait till transmit data register is empty
 while (!(SCISR1 & SCISR1_TDRE_MASK)) ;
 SCIDRL = c; // send character
}

char get_char_sci(void) {
  // wait until receive data register is full
  while (!(SCISR1 & SCISR1_RDRF_MASK)) ;
  return SCIDRL;
}

void initialize_sci(void) {
  // SBR = fE / 16 / baud rate
  SCIBDH = 0x00;
  SCIBDL = 156; // = 24000000 / 16 / 9600 assuming 24 MHz

  // word length, parity, other configuration bits
  // often the defaults are ok
  SCICR1 = 0x00;

  // enable transmitter and receiver
  // interrupts (if desired)
  SCICR2 = SCICR2_TE_MASK | SCICR2_RE_MASK;
}

void initialize_pll(void) {
  CLKSEL_PLLSEL = 0;       // turn off PLL for source clock
  PLLCTL_PLLON  = 1;       // turn on PLL

  // Assuming OscFreq = $8000, we mult by synr+1=3 (to get 24MHz)
  SYNR_SYN      = 2;       // set PLL multiplier
  REFDV_REFDV   = 0;       // set PLL divider assuming 24 MHz (divide by REFDV+1)

  CRGFLG_LOCKIF = 1;       // clear the lock interrupt flag

  while (!CRGFLG_LOCK) {}; // wait for the PLL to lock before continuing

  CLKSEL_PLLSEL = 1;       // select the PLL for source clock
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
