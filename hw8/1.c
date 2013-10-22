// Write C code to do the following. Use output compare to generate a pulse of width 10
// microseconds, at regular intervals of 100 milliseconds, on Port T, pin 2. Simultaneously, we
// have a series of high going pulses that are input to Port T, pin 3. Use input capture to
// measure the width of the input pulses. The maximum width of a pulse is 20 ms. Store the
// width of the latest input pulse into the variable PWIDTH (in units of timer counts). Use an
// interrupt-driven architecture.

// Get current value of TCNT, store into a variable startTime

// Keep testing the value of TCNT‐startTime until it is greater than 24000
#include <hidef.h>
#include "derivative.h"

#define HI_COUNT 30 // 10 us
#define LO_COUNT 60054 // 19.998 ms

char lo_interval_count = 0; // used to count to five for LoCount
int  rising_time; // used to capture time at rising edge of pulse
int  PWIDTH; // sets width of pulse (in units of timer counts)

void setup_timer(char prescaler);
void setup_input_capture(char channel, char edge_to_capture);
void setup_output_compare(char channel, char action);

void main(void) {
  char prescaler = 0x03;
  char IC_channel = 0x08; // enable input-capture 3
  char IC_edge_to_capture = 0x40; // capture rising edges of the PT3 Pin
  char OC_channel = 0x04; // OC2
  char OC_action = 0; // 0 corresponds to no action

  DDRT = PTT_PTT2_MASK;

  setup_timer(prescaler);
  setup_input_capture(IC_channel, IC_edge_to_capture);
  setup_output_compare(OC_Channel, OC_action);

  TC2 += LO_COUNT; // start an new OC2 operation

  // interrupts
  TIE = 0x08 & 0x04; // enable OC2 and OC3 interrupts locally
  asm("cli"); // enable interrupt globally

  while(1);
}

void interrupt VectorNumber_Vtimch3 oc3ISR (void) {
  if (TCTL4 &= 0x40) {
    rising_time = TC3; // captures time from input and sets it to risingTime
    TCTL4 = 0x80; // Setup to capture falling edge
  } else {
    PWIDTH = TC3 - rising_time; // pulse width (units of timer counts) = current_time - time at rising edge
    TCTL4 = 0x40; // setup to capture rising edge
  }
}

void interrupt VectorNumber_Vtimch2 oc2ISR (void) {
  if (lo_interval_count >= 5) {
    PTT2 = 1;
    TC2 += HI_COUNT;
    lo_interval_count = 0;
  } else {
    PTT2 = 0;
    TC2 += LO_COUNT;
    lo_interval_count++;
  }
}

void setup_timer(char prescaler) {
  TSCR1 = 0x90;      // enable timer counter, enable fast flag clear
  TSCR2 = prescaler; // disable TCNT overflow interrupt, set prescaler
}

void setup_input_capture(char channel, char edge_to_capture) {
  TFLG1 = channel;         // clear the CnF flag
  TIOS &= ~channel;        // enable input-capture 3
  TCTL4 = edge_to_capture; // capture rising edges of the PT3 pin
}

void setup_output_compare(char channel, char action) {
  TIOS |= channel; // enable OCn function
  TCTL2 = action;  // set OCn to be no action
  TFLG1 = channel; // clear the CnF flag
}
