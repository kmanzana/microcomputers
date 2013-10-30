#include <hidef.h>
#include "derivative.h"
#include "lcd.h"
#include "timer_system.h"

#define HI_COUNT 30 // 10 us
#define LO_COUNT 60053 // 19.9975 ms (19.9975 ms * 4 = 79.99 ms)
#define LO_INTERVAL 4

char lo_interval_count = 0; // used to count to five for LoCount
int  rising_time; // used to capture time at rising edge of pulse

// input capture
void interrupt VectorNumber_Vtimch7 ic7ISR(void) {
  static int distance;
  int  pulse_width_ticks;
  int  pulse_width_micro_seconds;

  if (TCTL3 & 0x40) {
    rising_time = TC7; // captures time from input and sets it to risingTime
    TCTL3 = 0x80; // Setup to capture falling edge
  } else {
    pulse_width_ticks = TC7 - rising_time; // pulse width (units of timer counts) = current_time - time at rising edge
    TCTL3 = 0x40; // setup to capture rising edge
  }

  // 1 tick is 333 ns = 333 / 1000 us
  pulse_width_micro_seconds = pulse_width_ticks * 333 / 1000;
  // distance (cm) = us / 58
  distance = pulse_width_micro_seconds / 58;

  if (distance < 300) {
    sprintf(my_string, "Value is %ld cm", distance);
    print_lcd(my_string);
  } else {
    print_lcd("Out of Range");
  }
}

// output compare
void interrupt VectorNumber_Vtimch6 oc6ISR(void) {
  // Test bit OL6 to see what OC6 was configured to do
  if (TCTL1 & 0x10) {
    // OL6 was a 1, meaning OC6 went high upon compare
    TC6 += HI_COUNT; // This is about 10 us
    TCTL1 = 0x20; // OM2:OL2 = 1:0 to go low next
  } else {
    // OL6 was a 0, meaning OC6 went low upon compare
    TC6 += LO_COUNT; // This is about 80 ms
    TCTL1 = 0x30; // OM2:OL2 = 1:1 to go high next
  }

  // can you write to PT6 while it is configured to do something based on the output compare?
  // if (lo_interval_count >= LO_INTERVAL) {
  //   PTT_PTT6 = 1;
  //   TC6 += HI_COUNT;
  //   lo_interval_count = 0;
  // } else {
  //   PTT_PTT6 = 0;
  //   TC6 += LO_COUNT;
  //   lo_interval_count++;
  // }
}

void main(void) {
  char prescaler = 0x03; // clock / 8
  char IC_channel = 0x80; // enable input-capture PT7
  char IC_edge_to_capture = 0x40; // capture rising edges of the PT6 Pin
  char OC_channel = 0x40; // OC6 and PT6
  char OC_action = 0; // 0 corresponds to no action
  char string[] = "HI";

  initialize_LCD();
  print_LCD(string);

  setup_timer(prescaler);
  setup_input_capture(IC_channel, IC_edge_to_capture);
  setup_output_compare(OC_channel, OC_action);

  DDRT |= PTT_PTT6_MASK; // setup PT6 for output
  DDRT &= ~PTT_PTT7_MASK; // setup PT7 for input
  DDRM &= ~0x02; // PM1 for input

  EnableInterrupts;

  for(;;) {
    _FEED_COP();
  }
}


