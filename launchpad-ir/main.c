#include <msp430.h> 

#define T05 300
#define T65 T05*13
#define T2 T05*4
#define T3 T05*6

#define TXD BIT2
#define RXD BIT1

unsigned int rxData = 0; // received data: A4-A0 and C6-C0 0000 AAAA ACCC CCCC
unsigned int bitCounter = 0;


void msp_transmit(char *str)
{
    while(*str)
    {
        while(!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }
}


void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // stop WDT
    BCSCTL1 = CALBC1_1MHZ; // load calibrated data
    DCOCTL = CALDCO_1MHZ;

    P1DIR &= ~BIT1; // P1.1 input
    P1SEL = BIT1; // P1.1 Timer_A CCI0A

    P1OUT &= ~(BIT3 + BIT4 + BIT6); // P1.3-1.5 out
    P1DIR |= (BIT3 + BIT4 + BIT6);

    TACTL = TASSEL_2 | MC_2; // SMCLK, continuous mode
    CCTL0 = CM_2 | CCIS_0 | CAP | CCIE; // falling edge capture mode, CCI0A, enable IE


    P1SEL  = TXD;
    P1SEL2 = TXD;

    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0   = 104;
    UCA0BR1   = 0x00;
    UCA0MCTL  = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;


    __bis_SR_register(LPM0_bits + GIE); // switch to LPM0 with interrupts
    while(1)
    {
        P1OUT ^= BIT6;
        msp_transmit(rxData);
        __delay_cycles(1000000);
    }
}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
    if(CCTL0 & CAP)
    { // start bit
        bitCounter++; // start counting bits
        CCR0 += T65; // add 6.5 bits to counter
        CCTL0 &= ~ CAP; // compare mode
    }
    else
    {
        switch (bitCounter)
        {
            case 0x1000: // received all bits
                bitCounter = 0; // reset counter
                // process received data, for example toggle LEDs
            switch (rxData & 0x003F)
            { // mask device number
                case 19: // Volume - 0010011 = 19
                P1OUT ^= BIT6;
                break;

                case 18: // Volume + 0010010 = 18
                P1OUT ^= BIT6;
                break;

                case 21: // Power 0010101 = 21
                P1OUT ^= BIT6;
                break;
            }
            rxData = 0;

            // end process received data
            CCTL0 |= CAP; // capture mode
            break;
            default: // data bit

                if (CCTL0 & SCCI)
                { // bit = 1
                    CCR0 += T2; // add 2 bits to counter
                }
                else
                { // bit = 0
                    rxData |= bitCounter; // set proper bit of rxData
                    CCR0 += T3; // add 3 bits to counter
                }

                bitCounter <<= 1; // increase (shift) bit counter
                break;
            }
    }
}
