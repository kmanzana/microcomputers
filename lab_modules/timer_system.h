#include "derivative.h"

void setup_timer(char prescaler);

// ACTIONS:
  // 0: no action
  // 01: toggle
  // 10: clear
  // 11: set

// channel for OCn and PTn

// Example:
// setup_output_compare(0x04, 0);
// channel OC2 and PT2 with no action
void setup_output_compare(char channel, char action);

void setup_input_capture(char channel, char edge_to_capture);
