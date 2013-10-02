// (a)
// Write assembly or C statements to program the PWM system to
// generate a 2 kHz digital waveform with 40% duty cycle, on
// the PT2 pin.

void setPWM(int period,
            int duty,
            int clockSA,
            int prescaler,
            int divider,
            int portTPin)
{
  PWMPRCLK = prescaler;
  PWMCLK   = clockSA;
  PWMSCLA  = divider;
  PWMPOL   = 0x01;
  PWMDTY0  = duty;
  PWMPER0  = period;
  PWME     = 0x01;
  MODRR    = 0x01;
}

setPWM()

// (b)
// Write assembly or C statements to program the PWM system to
// generate a 15 kHz digital waveform with 60% duty cycle, on
// the PT0 pin.


// assembly example from slides:
// movb #0,PWMPRCLK ; set clock prescaler (M=1)
// movb #1,PWMCLK ; select clock SA
// movb #12,PWMSCLA ; divider for clock SA (N=12)
// movb #1,PWMPOL ; positive going pulse
// movb #1,PWMDTY0 ; pulse width 1*1usec
// movb #10,PWMPER0 ; period is 10*1usec
// movb #1,PWME ; enable channel 0
// movb #1,MODRR ; route PWM channel 0 to PT0
