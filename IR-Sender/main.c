#include <msp430.h>

#define BLINKY_DELAY_MS 1               //Change this as per your needs
#define BUTTON BIT3

void delay(int);

unsigned int OFCount=0;
unsigned int i=0, j=0;
unsigned char temp, vals[4]={0x0f,3,7,15};


int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           //Stop watchdog timer
    P1DIR |= BIT0+BIT6;                 //Configure P1.0 as Output
    P1OUT = 0x00;

    //Set MCLK = SMCLK = 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1IE |= BUTTON;                     // P1.3 interrupt enabled

    P1IFG &= ~BUTTON;                   // P1.3 IFG cleared

    TACCR0 = 0;                         //Initially, Stop the Timer
    TACCTL0 |= CCIE;                    //Enable interrupt for CCR0.
    TACTL = TASSEL_2 + ID_0 + MC_1;     //Select SMCLK, SMCLK/1 , Up Mode

    _enable_interrupt();

    TACCR0 = 90-1;                      //Start Timer, Compare value for Up Mode to get 1ms delay per loop

    for(;;)
    {
        __delay_cycles(1500000);
        P1OUT^=BIT0;
        P1OUT |= BIT6;
        __delay_cycles(9000);
        P1OUT &= ~BIT6;
        __delay_cycles(4500*2);
        P1OUT |= BIT6;

        for(i=0;i<1;i++)
        {
            temp = vals[i];
            for(j=0;j<8;j++)
            {
                temp = vals[i] >> 1*j;
                if(temp&0x01)
                {
                    P1OUT |= BIT6;
                    delay(5);
                    P1OUT &= ~BIT6;
                    delay(17);
                }
                else
                {
                    P1OUT |= BIT6;
                    delay(5);
                    P1OUT &= ~BIT6;
                    delay(5);
                }
            }
        }
    }

    }



#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void)
{
    OFCount++;
}


void delay(int count)
{
    OFCount = 0;
    TACCR0 = 100-1;

    while(OFCount<=count);

    TACCR0 = 0;
}


#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    unsigned char flags = P1IFG & P1IE;
    if (flags & BIT3)
    {
//        P1OUT ^= 0x01;
//
//        P1OUT |= BIT6;
//        __delay_cycles(9000);
//        P1OUT &= ~BIT6;
//        __delay_cycles(4500*2);
//        P1OUT |= BIT6;
//        __delay_cycles(9000);
//        P1OUT &= ~BIT6;
//        __delay_cycles(4500*2);

//        for(i=0;i<1;i++)
//        {
//            temp = vals[i];
//            for(j=0;j<8;j++)
//            {
//                temp = vals[i] >> 1*j;
//                if(temp&0x01)
//                {
//                    P1OUT |= BIT6;
//                    delay(5);
//                    P1OUT &= ~BIT6;
//                    delay(17);
//                }
//                else
//                {
//                    P1OUT |= BIT6;
//                    delay(5);
//                    P1OUT &= ~BIT6;
//                    delay(5);
//                }
//            }
//        }
    }
    P1IFG = 0; // clear interrupt
}
