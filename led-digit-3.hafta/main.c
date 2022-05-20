#include "msp430.h"

#define   SEGMENT0   BIT0
#define   SEGMENT1   BIT1
#define   SEGMENT2   BIT2
#define   SEGMENT3   BIT3

unsigned char segment[]={ 0x3F, 0x06, 0x5B, 0x4F, 0x66,
                          0x6D, 0x7D, 0x07, 0x7F, 0x6F };

unsigned int OFCount=0, OFCount2=0;
unsigned int num=0,i=0;
unsigned int temp=10000;
int j=0;

void segment_yaz(unsigned char, char);
void initTimer_A(void);

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;

  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;

  P1DIR = (0xFF);
  P2DIR = (BIT0 | BIT1 | BIT2 | BIT3);

  P1OUT = (0x00);
  P2OUT = (0x00);

  initTimer_A();

  _enable_interrupt();

  for(;;);

}

void initTimer_A(void)
{

    TACCR0 = 1000-1;
    TACCTL0 |= CCIE;
    TACTL = TASSEL_2 + ID_0 + MC_1;

    TA1CTL   = MC_1 + TASSEL_2 + ID_0;
    TA1CCR0  = 5000-1;
    TA1CCTL0 = CCIE;
}

void segment_yaz(unsigned char sayi_f, char seg)
{
    P2OUT = 0X00;
    P1OUT = sayi_f;
    P2OUT = seg;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TA1_A0_ISR(void)
{

    if(++num > 10000)
      num=0;

    if(--temp <=0)
        temp = 10000;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_A0_ISR(void)
{
  i++;

  if (j / 4000 == 0)
      switch(i)
      {

          case 1 : segment_yaz( segment[0],    SEGMENT0);             break;

          case 2 : segment_yaz((segment[num/100 %10]),SEGMENT1);      break;

          case 3 : segment_yaz( segment[num/10  %10], SEGMENT2);      break;

          case 4 : segment_yaz( segment[num%10],      SEGMENT3); i=0; break;
      }
  else
      switch(i)
      {

          case 1 : segment_yaz( segment[0],    SEGMENT0);              break;

          case 2 : segment_yaz((segment[temp/100 %10]),SEGMENT1);      break;

          case 3 : segment_yaz( segment[temp/10  %10], SEGMENT2);      break;

          case 4 : segment_yaz( segment[temp%10],      SEGMENT3); i=0; break;
      }

  if(j==8000)
      j=0;

  j++;
}
