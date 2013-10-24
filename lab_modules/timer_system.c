#include "timer_system.h"

void setup_timer(char prescaler) {
  TSCR1 = 0x90;      // enable timer counter, enable fast flag clear
  TSCR2 = prescaler; // disable TCNT overflow interrupt, set prescaler
}

void setup_output_compare(char channel, char action) {
  TIOS |= channel; // enable OCn function
  // TCTL1 = action;  // set OCn to be no action
  TCTL1 = 0x30; // set OC6 pin action to go high
  // TCTL2 for other ports
  TFLG1 = channel; // clear the CnF flag
  TIE |= channel;
}

void setup_input_capture(char channel, char edge_to_capture) {
  TIOS &= ~channel;        // enable input-capture
  TCTL3 = edge_to_capture; // capture rising edges of the PT3 pin
  // TCTL4 = edge_to_capture; // for other PT ports
  TFLG1 = channel;         // clear the CnF flag
  TIE |= channel;
}
