// Write the C function: writeByteToLCD(char b,char rs,int t). This
// function sends a byte to the LCD. It must call writeNibbleToLCD twice, first for
// the high nibble, then for low nibble, then wait the desired delay time t (where t is in
// microseconds).

void writeByteToLCD(char b, char rs, int t) {
  writeNibbleToLCD(b, rs, 1);
  writeNibbleToLCD(b, rs, 1);
  delayUSec(t);
}

