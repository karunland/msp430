#include <msp430g2553.h>
//
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR = BIT0+BIT6;
    P2DIR = 0XFF;

//    P1REN = BIT3;

    P1OUT = BIT6+BIT0;
    P2OUT = 0XFF;
    P1IE |= BIT3+BIT4; // P1.3 interrupt enabled
    __enable_interrupt(); // enable all interrupts
    while(1)
    {
        // buton okuma kosulu

    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    unsigned char flags = P1IFG & P1IE;
    if (flags & BIT3)
    {

        P1OUT ^= 0XFF;
    }
    if (flags & BIT4)
    {
        P2OUT ^= 0XFF;
    }


    P1IFG = 0; // clear interrupt
}
