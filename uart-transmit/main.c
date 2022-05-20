#include <msp430.h> 
#include <stdio.h>
#include  <stdlib.h>

char message[100]={0};

#define TXD BIT2
#define RXD BIT1

void msp_transmit(char *);


void main(void)
{
    int i=0;
    WDTCTL = WDTPW | WDTHOLD;


    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;
    P1DIR &= ~BIT4;                             // IR SENSOR IS CONNECTED TO PORT2 OF BIT0
    P1IE = BIT4;

    P1DIR  = BIT0 + BIT6;
    P1SEL  = TXD|RXD;
    P1SEL2 = TXD|RXD;

    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0   = 104;
    UCA0BR1   = 0x00;
    UCA0MCTL  = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;
    __enable_interrupt();

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
        P1OUT ^= BIT0 + BIT6;
        msp_transmit(message);
        __delay_cycles(1000000);
    }
}

void msp_transmit(char *str)
{
    while(*str)
    {
        while(!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
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

