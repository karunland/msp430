#include <msp430.h> 

unsigned int sicaklik=0;
volatile char tick;

unsigned int sicaklik_ham,sicaklik;

void sicaklik_init(void);
void sicaklik_oku(void);

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;

  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);

  TA0CTL = TASSEL_2 + TACLR;
  TA0CCR0 = 50000-1;
  TA0CCTL0 = CCIE;

  sicaklik_init();



  TA0CTL |= MC_1; //Timer Baslatiliyor...

  __bis_SR_register(LPM0_bits + GIE);
}

// TimerA0 Kesme Vekt�r�
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{
  tick++;
  if(tick==10)
  {
    sicaklik_oku();
    tick = 0;
  }
}

// ADC10 Kesme Vekt�r�
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}



void sicaklik_init()
{
  ADC10CTL0 &= ~ENC;
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + MSC + ADC10IE;
  ADC10CTL1 = INCH_10 + ADC10SSEL_0 + ADC10DIV_0 + CONSEQ_2;
  ADC10DTC1 = 32;
  __delay_cycles(256);
}

void sicaklik_oku()
{
  char i;
  float yeni=0;
  unsigned int toplam=0;
  unsigned int dma_buf[32];

  ADC10SA = (unsigned int)dma_buf;
  ADC10CTL0 |= ENC + ADC10SC;
  __bis_SR_register(CPUOFF + GIE);

  for(i=0;i<32;i++)
    toplam+=dma_buf[i];

  toplam >>= 5;

  sicaklik_ham = toplam;
  yeni=(((sicaklik_ham - 673) * 423) / 1024.0f);
  sicaklik=(unsigned int)yeni;

}

