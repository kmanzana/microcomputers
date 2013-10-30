#include "timer_system.h"

void setup_timer(char prescaler) {
  TSCR1 = 0x90;      // enable timer counter, enable fast flag clear

  // 0x03
  TSCR2 = prescaler; // disable TCNT overflow interrupt, set prescaler
}

void setup_input_capture(char channel, char edge_to_capture) {
  // 0x80
  TIOS &= ~channel;        // enable input-capture

  // 0x40
  TCTL3 = edge_to_capture; // capture rising edges of the PT3 pin
  // TCTL4 = edge_to_capture; // for other PT ports

  // 0x80
  TFLG1 = channel;         // clear the CnF flag

  // 0x80
  TIE |= channel;
}

void setup_output_compare(char channel, char action) {
  // 0x40
  TIOS |= channel; // enable OCn function

  // 0x00
  // TCTL1 = action;  // set OCn to be no action
  // 0x30
  TCTL1 = 0x30; // set OC6 pin action to go high

  // TCTL2 for other ports
  // 0x40
  TFLG1 = channel; // clear the CnF flag

  // 0x40
  TIE |= channel;
}
