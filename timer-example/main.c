#include <msp430.h>
#define BLINKY_DELAY_MS 500    //Change this as per your needs

void initTimer_A(void);

#define ROTL(X, N)  (((X) << (N)) | ((X) >> (8 * sizeof(X) - (N))))

unsigned int OFCount;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   //Stop watchdog timer
    P1DIR |= BIT0;              //Configure P1.0 as Output
    P2DIR = 0xFF;

    //Set MCLK = SMCLK = 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    initTimer_A();
    _enable_interrupt();

    OFCount  = 0;
    TACCR0 = 1000-1; //Start Timer, Compare value for Up Mode to get 1ms delay per loop

    for(;;);
}
// TIMER FORMULA DIV*( TACCR0 )/()
void initTimer_A(void)
{
    //Timer Configuration
    TACCR0 = 0; //Initially, Stop the Timer
    TACCTL0 |= CCIE; //Enable interrupt for CCR0.
    TACTL = TASSEL_2 + ID_0 + MC_1; //Select SMCLK, SMCLK/1 , Up Mode
}
unsigned char temp = 0x01;
//Timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void)
{

    if(OFCount >= BLINKY_DELAY_MS)
    {
        P2OUT = temp;
        OFCount = 0;
        temp=ROTL(temp,1);
    }

    OFCount++;
}
