#include <hidef.h>
#include "derivative.h"
#include "lcd.h"
#include "timer_system.h"

#define HI_COUNT 30 // 10 us
#define LO_COUNT 60053 // 19.9975 ms (19.9975 ms * 4 = 79.99 ms)
#define LO_INTERVAL 4

char lo_interval_count = 0; // used to count to five for LoCount
int  rising_time; // used to capture time at rising edge of pulse
int  pulse_width;

// output compare
void interrupt VectorNumber_Vtimch6 oc6ISR(void) {
  if (lo_interval_count >= LO_INTERVAL) {
    PTT_PTT6 = 1;
    TC6 += HI_COUNT;
    lo_interval_count = 0;
  } else {
    PTT_PTT6 = 0;
    TC6 += LO_COUNT;
    lo_interval_count++;
  }
}

// input capture
void interrupt VectorNumber_Vtimch7 ic7ISR(void) {
  static int time_of_pulse;

  if (TCTL3 &= 0x40) {
    rising_time = TC7; // captures time from input and sets it to risingTime
    TCTL3 = 0x80; // Setup to capture falling edge
  } else {
    pulse_width = TC7 - rising_time; // pulse width (units of timer counts) = current_time - time at rising edge
    time_of_pulse = pulse_width * 3;
    TCTL3 = 0x40; // setup to capture rising edge
  }
}

void main(void) {
  char prescaler = 0x03; // /8
  char IC_channel = 0x80; // enable input-capture PT7
  char IC_edge_to_capture = 0x40; // capture rising edges of the PT6 Pin
  char OC_channel = 0x40; // OC6 and PT6
  char OC_action = 0; // 0 corresponds to no action
  char string[] = "HI";

  initialize_LCD();
  print_LCD(string);

  setup_timer(prescaler);
  setup_output_compare(OC_channel, OC_action);
  setup_input_capture(IC_channel, IC_edge_to_capture);

  DDRT |= PTT_PTT6_MASK; // setup PT6 for output
  DDRT &= ~PTT_PTT7_MASK; // setup PT7 for input
  DDRM &= ~0x02; // PM1 for input

  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}


