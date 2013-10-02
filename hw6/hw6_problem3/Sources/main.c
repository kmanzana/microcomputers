// Write a C function that does a table lookup. The input parameter is a value between 0 and
// 255. The return value is the period in microseconds corresponding to the tones of a musical
// scale. Use the table you used in Lab 4. Note - since that table has only 32 values, your
// function will have to scale the input parameter from the range 0..255 to the range 0..31. So
// for example, if your function has an input value in the range 0..7, it should return the first
// value of the table. If the input value is between 8..15, it should return the second value in the
// table, and so on. Use the following test program and implement it using CodeWarrior. Show
// the values in memory, corresponding to the output table of answers.

#include <hidef.h>                /* common defines and macros */
#include "derivative.h"           /* derivative-specific definitions */

/* The output answers are put here */
unsigned int Answers[5];

unsigned int tonesTable[] = {
  3822,3608,3405,3214,3034,2863,2703,2551,
  2408,2273,2145,2025,1911,1804,1703,1607,
  1517,1432,1351,1276,1204,1136,1073,1012,
  956, 902, 851, 804, 758, 716, 676, 638
};

unsigned int GetValue(unsigned char input)
{
  return tonesTable[input >> 3];    // scale by 8
}

void main(void) {
  /* Define the table of input test numbers */
  unsigned char v[] = {
    0,15,16,128,255
  };

  int i; // a local variable

  EnableInterrupts;

  for(i=0; i<5; i++) {
    Answers[i] = GetValue(v[i]);
  }

  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
}

