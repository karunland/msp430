#include <msp430g2553.h>

#define LED0 BIT0
#define LED1 BIT6
#define BUTTON0 BIT3


int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    P1DIR |= (LED0 | LED1 | ~BUTTON0); // Set pins as output direction
    P1REN |= (LED0 | LED1 ); // Set pins as output direction
    P1OUT |= (LED0 | ~LED1); // set P1.0 to 0 (LED OFF)
    P1IE  |= (BUTTON0); // P1.3 interrupt enabled


//    P2DIR = 0X0F;
//    P2REN = 0XFF;
//    P2OUT = 0X00;


    P1IFG &= ~(BUTTON0); // P1.  3 IFG cleared

    __enable_interrupt(); // enable all interrupts

    while(1);
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{

//    P2OUT ^= (0X0F);
    P1OUT ^= (0b01000001);
    P1IFG = 0; // clear interrupt
}


//#include <msp430g2553.h>
////#include <msp430g2553.h>
////
//void main(void)
//{
//    WDTCTL = WDTPW + WDTHOLD;
//    P1DIR = BIT0;
//    P2DIR = BIT3;
//    P1REN = BIT3;
//    P1OUT = BIT3;
//    P2OUT = BIT0;
//    P1IFG &= ~(BIT0); // P1.  3 IFG cleared
//    __enable_interrupt(); // enable all interrupts
//    while(1)
//    {
//        ;;
//
//    }
//}
//
//
//#pragma vector=PORT1_VECTOR
//__interrupt void PORT1_ISR(void)
//{
//    // buton okuma kosulu
//    if((P1IN & BIT3)!=BIT3)
//    {
//
////        __delay_cycles(220000);
//
//        P1OUT ^= BIT0;
//        P2OUT ^= BIT3;
//
//    }
//    P1IFG = 0; // clear interrupt
//}
