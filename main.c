#include <msp430.h>
#include "sounddata.h"
#include "sound2data.h"

#define LED0 BIT3
#define LED1 BIT4
#define IRLED1 BIT4
#define IRrec BIT1
#define IRrec1 BIT2
#define debug BIT0
#define relay BIT5
#define CS BIT0  //2.0 is SPI CS
#define MOSI BIT7  //1.7 is SPI MOSI
#define SCLK BIT5  //1.5 is SPI clock

void tx_send(long irdata);
void play(void);
void play2(void);

int loopcountMAX =0;
int loopcount=0;
int count=0;
int sendcount =4900;
int send2count =4900;
int resetcount =900;
int one =0;
int two =0;
int three =0;
int four =0;
int five =0;
int six =0;
int seven =0;
int eight =0;
int odd =0;
int bit =0;
int bitt =0;
int i=0;
int ii =0;
int iii=0;
int sample =0;
int sample2 =0;

unsigned long rxdata =0;
unsigned long realdata =0;
unsigned long irdata =0;


void main(void){

  WDTCTL = WDTPW + WDTHOLD;                            // Stop watchdog timer

  P1DIR |= (debug + IRLED1);                            // Set all pins but RXD to output
  P2DIR |= (LED0 + LED1 + relay + CS);
  P1OUT =0;
  P2OUT=0;
  P2IE |= IRrec; // P1.3 interrupt enabled
  P2IES &= ~IRrec; // P1.3 interrupt enabled
  P2IFG &= ~IRrec; // P1.3 IFG cleared
  P2IE |= IRrec1; // P1.3 interrupt enabled
  P2IES |= IRrec1; // P1.3 interrupt enabled
  P2IFG &= ~IRrec1; // P1.3 IFG cleared

  TA1CCTL0 |= CCIE;                             // CCR0 interrupt enabled
  TA1CTL = TASSEL_2 + MC_1 + ID_2;           // SMCLK/8, upmode
  TA1CCR0 =  20;


  P1SEL |= MOSI + SCLK;
  P1SEL2 |= MOSI + SCLK;

  UCB0CTL1 = UCSWRST;
  UCB0CTL0 |= UCMSB + UCMST + UCSYNC + UCCKPH; // 4-pin, 8-bit SPI master
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 1;                          // /2
  UCB0BR1 = 0;                              //
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**


  __enable_interrupt(); // enable all interrupts                                   // Enable CPU interrupts



  while(1)
  {

    _delay_cycles(100);

    if(resetcount == 200)
    {
      loopcountMAX=loopcount;
      loopcount =0;
      realdata = rxdata >> 1;

    }

    if(realdata == 0xFF17943) //gun 1 code
    {
      sendcount =0;
      realdata =0;
      P2OUT |=LED1;
      P2OUT &=~LED0;
      realdata =0;
    }
    else if(realdata == 0xFF46731) //gun 2 code
    {
      send2count =0;
      realdata =0;
      P2OUT |=LED1;
      P2OUT &=~LED0;
      realdata =0;
    }
if(send2count ==1)
{
	tx_send(0xFF9992); //tv code
    _delay_cycles(100);
	play2();
    P2OUT |=LED0;
    P2OUT &=~LED1;
}
if(sendcount ==1)
{
	tx_send(0xFF9912); //tv code
    _delay_cycles(100);
	play();
    P2OUT |=LED0;
    P2OUT &=~LED1;
}
if(sendcount == 4000 || send2count == 4000)
{
    P2OUT &=~LED0;
    P2OUT &=~LED1;
}

sendcount++;
if(sendcount >5000)
{
  sendcount =4999;
}
send2count++;
if(send2count >5000)
{
  send2count =4999;
}

    resetcount++;
    if(resetcount >1000)
    {
      resetcount =999;
    }
    if(count >1000)
    {
      count =999;
    }

  }
}




#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
  count++;
}



#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P1OUT ^= debug;
  TA1CTL |= TACLR;

  if(loopcount == 0)
  {
    rxdata =0;
  }
  if (loopcount == 50)
  {
    one = count;
  }
  else if (loopcount ==51)
  {
    two = count;
  }
  else if(loopcount ==52)
  {
    three = count;
  }
  else if(loopcount ==53)
  {
    four = count;
  }
  else if(loopcount == 54)
  {
    five = count;

  }
  else if(loopcount == 55)
  {
    six = count;

  }
  else if(loopcount == 56)
  {
    seven = count;

  }
  else if(loopcount == 57)
  {
    eight = count;

  }

  odd = (loopcount & 0x1);
  if(odd == 0)
  {
    	if(count >13 && count < 24)
    		{
    			bit = 1;
    		}
    	else
    	{
			bit = 0;
    	}

    		    rxdata += bit;
    		    rxdata<<=1;
  }

loopcount++;
  count=0;
  resetcount=0;
  P2IFG &= ~IRrec; // P1.3 IFG cleared
  P2IFG &= ~IRrec1; // P1.3 IFG cleared
}


void tx_send(long irrealdata)
{


  __disable_interrupt(); // enable all interrupts
//  irdata = irrealdata;

for(iii=32;iii>0;iii--)
{
	bitt = irrealdata & 1;
	irdata |= bitt;
	irrealdata>>=1;
	irdata<<=1;
}

  for (i = 322;i>0;i--)
  {
    P1OUT |= IRLED1;
    __delay_cycles(4);
    P1OUT &= ~IRLED1;
    __delay_cycles(4);

  }

    __delay_cycles(3830);


  for(ii = 32; ii>0;ii--)
  {
    bit = irdata & 1;
    if(bit == 0)
    {
        __delay_cycles(500);
      for (i = 21;i>0;i--)
      {
        P1OUT |= IRLED1;
        __delay_cycles(4);
        P1OUT &= ~IRLED1;
        __delay_cycles(4);

      }



    }
    else
    {

        __delay_cycles(1590);

      for (i = 21;i>0;i--)
      {
        P1OUT |= IRLED1;
        __delay_cycles(4);
        P1OUT &= ~IRLED1;
        __delay_cycles(4);


      }



    }
    irdata >>= 1;
  }

  for (i = 5;i>0;i--)
  {
    P1OUT |= IRLED1;
    __delay_cycles(4);
    P1OUT &= ~IRLED1;
    __delay_cycles(4);


  }


    __delay_cycles(1590);

    for (i = 20;i>0;i--)
    {
      P1OUT |= IRLED1;
      __delay_cycles(4);
      P1OUT &= ~IRLED1;
      __delay_cycles(4);

    }

  __enable_interrupt(); // enable all interrupts
}


void play(void)
{
	__disable_interrupt(); // enable all interrupts
	int MSB=0;
	int LSB=0;
	sample =0;
	TA1CCTL0 &= ~CCIE;
	P2OUT |= relay;
	_delay_cycles(100000);
	for(i = NUM_ELEMENTS;i>0;i--)
	{
		_delay_cycles(5);
	MSB = 0x70 | ((data[sample] & 0XF0) >> 4);
	LSB = ((data[sample] & 0X0F) << 4);
	P2OUT &= ~CS;
	UCB0TXBUF = MSB ;
	while (UCB0STAT & UCBUSY);
	UCB0TXBUF = LSB ;
	while (UCB0STAT & UCBUSY);
	P2OUT |= CS;
	sample++;
	}
	//P2OUT ^= debug;
	P2OUT &= ~relay;
	TA1CCTL0 |= CCIE;
	__enable_interrupt(); // enable all interrupts
}

void play2(void)
{
	__disable_interrupt(); // enable all interrupts
	int MSB=0;
	int LSB=0;
	sample =0;
	TA1CCTL0 &= ~CCIE;
	P2OUT |= relay;
	_delay_cycles(100000);
	for(i = NUM_ELEMENTS2;i>0;i--)
	{
		_delay_cycles(5);
	MSB = 0x70 | ((data2[sample] & 0XF0) >> 4);
	LSB = ((data2[sample] & 0X0F) << 4);
	P2OUT &= ~CS;
	UCB0TXBUF = MSB ;
	while (UCB0STAT & UCBUSY);
	UCB0TXBUF = LSB ;
	while (UCB0STAT & UCBUSY);
	P2OUT |= CS;
	sample++;
	}
	//P2OUT ^= debug;
	P2OUT &= ~relay;
	TA1CCTL0 |= CCIE;
	__enable_interrupt(); // enable all interrupts
}
