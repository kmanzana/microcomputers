#include "misc.h"

/* A collection of functions for use with the LCD */

/* This function writes a nibble to the LCD. Input parameters:n contains the nibble to be written (in the lower 4 bits)rs indicates instruction or data (rs=0 for inst, rs=1 for data)t is the time to delay after sending (units of 1 us)Assumes these connections:PT0:PT3 - connect to LCD pins 8:14 (DB4:DB7)PT4 - connect to LCD pin 6 (E)PT5 - connect to LCD pin 4 (RS) */
void write_nibble_to_LCD(char n, char rs, int t);

/* This function writes a byte to the LCD. b is the byte to be written rs indicates instruction or data (rs=0 for inst, rs=1 for data) t is the time to delay after sending the byte (units of 1 us) */
void write_byte_to_LCD(char b, char rs, int t);

/* Print a string to the LCD. A maximum of 80 characters are printed. */
void print_LCD(char mystr[]);

void initialize_LCD(void);
