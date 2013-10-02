// (a)
// Write C statement(s) that sets bit 1 of Port T, without changing any of the other bits, if bit
// 2 is clear

if (~PTT & 0x04)
	PTT |= 0x02;

// (b)
// Write C statement(s) that toggles bit 7 of Port T, without changing any of the other bits.

PTT ^= 0x80;

// (c)
// Write C statement(s) that clear bits 5 and 6 of Port T, without changing any of the other
// bits, if bits 0 and 1 are both set.

if (PTT & 0x03)
  PTT &= ~0x60;
