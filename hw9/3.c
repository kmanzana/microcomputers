// Write C or assembly language instructions to set the SCI system on the HCS12 for the
// following configuration:
// a. Enable transmitter and receiver
// b. Baud rate = 115.2 K
// c. 8 bit characters, no parity bit
// d. No interrupts


// SBR = fE / 16 / baud rate
SCI0BDH = 13;
SCI0BDL = 0x00;

// word length, parity, other configuration bits
// often the defaults are ok
SCI0CR1 = 0x00;

// enable transmitter and receiver
// interrupts (if desired)
SCI0CR2 = 0x0C;
