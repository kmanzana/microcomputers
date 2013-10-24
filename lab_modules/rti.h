#include "derivative.h"
#include "lcd.h"
#include <stdio.h>

void interrupt VectorNumber_Vrti rti_isr(void);

// m: 0-7
// n: 0-16
void set_rti_flag_freq(char m, char n);

void enable_rti_interrupts(void);

void clear_rti_flag(void);

