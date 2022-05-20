#include <msp430g2553.h>

#define LED0 BIT0
//#define LED1 BIT6
#define BUTTON BIT3

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

  P1DIR |= (LED0 ); // Set P1.0 to output direction

  // P1.3 must stay at input
  P1OUT &= ~(LED0 ); // set P1.0 to 0 (LED OFF)
  P1IE |= BUTTON; // P1.3 interrupt enabled

  P1IFG &= ~BUTTON; // P1.3 IFG cleared

  __enable_interrupt(); // enable all interrupts

  while(1)
  {
  }

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  P1OUT ^= LED0;
  P1IFG = 0; // clear interrupt
}
