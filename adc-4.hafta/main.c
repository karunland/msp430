#include "msp430.h"

volatile char tick;
void ADC_Init(void);
unsigned int raw=0, volt=0;


void main( void )
{
    WDTCTL = WDTPW + WDTHOLD;

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;
    __delay_cycles(100000);

    P1DIR |= BIT6;
    P1DIR &= ~BIT0;
    P1OUT |= BIT6;


    TA0CTL = TASSEL_2 + TACLR;
    TA0CCR0 = 50000-1;
    TA0CCTL0 = CCIE;

    ADC_Init();

    TA0CTL |= MC_1;

    __bis_SR_register(LPM0_bits + GIE);
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{

    tick++;

    if(tick == 10)
    {

        ADC10CTL0 |= ENC + ADC10SC;

        __bis_SR_register(CPUOFF + GIE);

        raw = ADC10MEM;

        volt = (raw*2930) / 1000000 ;

        if(volt <= 500 )
        {
            P1OUT &= ~BIT6;
        }
        else
        {
            P1OUT |= BIT6;
        }

        tick = 0;
    }
}


void ADC_Init(void)
{

    ADC10AE0 |= BIT0;

    ADC10CTL0 &= ~ENC;

    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + MSC + ADC10IE;
    ADC10CTL1 = INCH_0 + ADC10SSEL_0 + ADC10DIV_0 + CONSEQ_2;
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    __no_operation();
}

