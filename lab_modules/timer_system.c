#include "timer_system.h"

void setup_timer(char prescaler) {
  TSCR1 = 0x90;      // enable timer counter, enable fast flag clear
  TSCR2 = prescaler; // disable TCNT overflow interrupt, set prescaler
}

void setup_output_compare(char channel, char action) {
  TIOS |= channel; // enable OCn function
  TCTL2 = action;  // set OCn to be no action
  TFLG1 = channel; // clear the CnF flag
}

void setup_input_capture(char channel, char edge_to_capture) {
  TFLG1 = channel;         // clear the CnF flag
  TIOS &= ~channel;        // enable input-capture 3
  TCTL4 = edge_to_capture; // capture rising edges of the PT3 pin
}
