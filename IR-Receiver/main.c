#include <msp430.h> 
#include <stdio.h>
#include  <stdlib.h>


#define TXD BIT2
#define RXD BIT1
char message[100]={0};


void msp_transmit(char *str)
{
    while(*str)
    {
        while(!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }
}


int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT

    BCSCTL1 = CALBC1_8MHZ;                      // Set DCO to 8Mhz
    DCOCTL = CALDCO_8MHZ;
    P1DIR = BIT0;
    P1DIR &= ~BIT4;                             // IR SENSOR IS CONNECTED TO PORT2 OF BIT0
    P1IE = BIT4;

    P1SEL  = TXD|RXD;
    P1SEL2 = TXD|RXD;

    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0   = 104;
    UCA0BR1   = 0x00;
    UCA0MCTL  = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;

    int i=0;

    __enable_interrupt();
    __delay_cycles(500000);

    while(1)
    {
        if(P1IN == BIT4)                        // IF IR IS HIGH
        {
            P1OUT |= BIT0;
            __delay_cycles(1000000);            // GIVE SOME DELAY
        }
        else
        {
            P1OUT &= ~BIT0;
            __delay_cycles(1000000);            // GIVE SOME DELAY
        }

        sprintf(message, "val %d \r\n", i++);
        msp_transmit(message);
        __delay_cycles(1000000);
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

