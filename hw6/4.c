// (a)
// Write assembly or C statements to program the PWM system to
// generate a 2 kHz digital waveform with 40% duty cycle, on
// the PT2 pin.

PWMPRCLK = 1<<(0+4);// prescalar M=0 (shift 4 for B)
PWMCLK   = 1<<2; // choose clock channel=2
                 // channels: A=4,1,0; B=3,2
                 // choose source: 0=A,B; 1=SA,SB
PWMSCLB  = 25;   // divider N SA (PWMSCLB for SB)
PWMPOL   = 1<<2; // pos pulse channel=2
PWMDTY2  = 96;   // duty (pulse width)
PWMPER2  = 240;  // period
PWME     = 1<<2; // enable channel=2
MODRR    = 1<<2; // route channel to PTT (only 0-4)

// (b)
// Write assembly or C statements to program the PWM system to
// generate a 15 kHz digital waveform with 60% duty cycle, on
// the PT0 pin.

PWMPRCLK = 1<<0; // prescalar M=0
PWMCLK   = 1<<0; // choose clock
                 // channels: A=5,4,1,0; B=7,6,3,2
                 // choose source: 0=A,B; 1=SA,SB
PWMSCLA  = 4;    // divider N SA (PWMSCLB for SB)
PWMPOL   = 1<<0; // pos pulse
PWMDTY0  = 120;  // duty (pulse width)
PWMPER0  = 200;  // period
PWME     = 1<<0; // enable channel
MODRR    = 1<<0; // route channel to PTT (only 0-4)

// assembly example from slides:
// movb #0,PWMPRCLK ; set clock prescaler (M=1)
// movb #1,PWMCLK ; select clock SA
// movb #12,PWMSCLA ; divider for clock SA (N=12)
// movb #1,PWMPOL ; positive going pulse
// movb #1,PWMDTY0 ; pulse width 1*1usec
// movb #10,PWMPER0 ; period is 10*1usec
// movb #1,PWME ; enable channel 0
// movb #1,MODRR ; route PWM channel 0 to PT0
