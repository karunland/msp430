#include <msp430.h> 


int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT

    BCSCTL1 = CALBC1_8MHZ;                      // Set DCO to 8Mhz
    DCOCTL = CALDCO_8MHZ;
    P1DIR = BIT0;
    P1DIR &= ~BIT4;                             // IR SENSOR IS CONNECTED TO PORT2 OF BIT0
    P1IE = BIT4;
    __enable_interrupt();

    __delay_cycles(500000);

    while(1)
    {
//        if(P1IN == BIT4)                        // IF IR IS HIGH
//        {
//            P1OUT |= BIT0;
//            __delay_cycles(1000000);            // GIVE SOME DELAY
//        }
//        else
//        {
//            P1OUT &= ~BIT0;
//            __delay_cycles(1000000);            // GIVE SOME DELAY
//        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    unsigned char flags = P1IFG & P1IE;
    if (flags & BIT4)
    {
        P1OUT ^= BIT0;
    }

    P1IFG = 0; // clear interrupt
}

