#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// SETUP UART A1
	UCA0CTL1 |= UCSWRST ; // PUT A1 INTO SW RESET



	return 0;
}
