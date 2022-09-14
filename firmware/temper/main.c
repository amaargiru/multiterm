


/* Написано сие в году 1905 от рожъдества Христова, в мае 
 * мьсяце числа 25-го, 
 * ученикомъ церковно-приходской школы села Возницы Арханъгельской 
 * губернiи Афанасиемъ Светловымъ */ 


#include "main.h"

ds1820 ds;
GR_47 GR;
//char StrToGm[90];

char Power_on_Reset=0;//,lives=6;

int main(void)
{



	// Crystal Oscillator division factor: 1
	CLKPR=0x80; CLKPR=0x00;


	BeginInit();

	// 1 Wire Bus initialization - подключаем ds1820
//	 ds.tC(); // сразу читаем температуру чтобы убрать начальное значение +85оС

	// Watchdog Timer initialization
	// Watchdog Timer Prescaler: OSC/16=62500гц
	WDTCR=0x18;
	WDTCR=0x08;

	

	// Reset Source checking - пока информация не используется
	if (MCUCSR & 1)   { MCUCSR&=0xE0; Power_on_Reset=1;  /* Power-on Reset*/ } 
	  else if (MCUCSR & 2)  {  MCUCSR&=0xE0; /* External Reset*/ } 
	    else if (MCUCSR & 4) {  MCUCSR&=0xE0; /* Brown-Out Reset*/ } 
	      else if (MCUCSR & 8) { MCUCSR&=0xE0; /* Watchdog Reset */ } 
	        else if (MCUCSR & 0x10) {  /* JTAG Reset*/  };


	_SEI();           /* Enable interrupts => enable UART interrupts */

  if (Power_on_Reset)
  {
          wait_response(0);
          // только для совместимости со старыми платами
//	  clrbit(PORTD,4);
 //	  delay_ms(1500);
//	  setbit(PORTD,4);
 //	  delay_ms(5000);
  }


  if (!tstbit(PIND,6))
  {
// 	  delay_ms(5000);
	  setLEDLevel(2);
	  clrbit(PORTD,4);
 	  delay_ms(1500);
	  setbit(PORTD,4);
 	  delay_ms(5000);
	  setLEDLevel(4);
  }


// 	  delay_ms(5000);

//        OutText("\n\n\n\n\n ---- Starting debug monitor ----\n\n");
//        OutText("Resets ");OutDat(ds.ReadResetCnt(),5,10);
//        ds.WriteResetCnt(ds.ReadResetCnt() + 1); // тут же сохраненное значение увеличиваем на 1
////        ds.WriteResetCnt(0); // а можно обнулить :)
//        OutText("\nLocal UID "); for (int i=0; i<8; i++) OutDat(ds.uidDS[i],2,16);
//	signed int t = ds.tC(); 
//	OutText("\nLocal Temp "); if (t > 0) OutText("+"); else if (t<0) OutText("-"); OutDat(t,2,10); //OutText("\n");
//while(1)	OutText(" oC\n\n");



	if (check_point()) // возвращает 1 если check_point
	{
		if (GR.InitGR_47()) reset ("\n\nGR Trooble!!!\n\n");
		while(1) { init(); scan(); }
	}
	
	else while(1)
	{
		init();	wait_response(0); LEDBlink();
        }

        
}


void BeginInit(void)
{

	// Input/Output Ports initialization
	PORTA=0x00; DDRA=0xFF; // Port A initialization
	PORTB=0x00; DDRB=0x09; // Port B initialization
	PORTC=0xff; DDRC=0x0a; // Port C initialization
	PORTD=0x10; DDRD=0x3a; // Port D initialization uart0
	PORTE=0x00; DDRE=0x07; // Port E initialization



	// External Interrupt(s) initialization - отключены
	// INT0: Off
	// Interrupt on any change on pins PCINT0-7: Off
	// Interrupt on any change on pins PCINT8-15: Off
	GICR|=0x00;

	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK=0x00; ETIMSK=0x00;

	// средства работы с уарт в uarts.c
//	USART0_Init( 9 );   /* Set the baudrate to 115,200 bps using a 16.000MHz crystal */
	USART0_Init( 9 );   /* Set the baudrate to 115,200 bps using a 16.000MHz crystal */
	USART1_Init( 119 );   /* Set the baudrate to 115,200 bps using a 16.000MHz crystal */

	// средства работы с таймерами в timers.c
	init_timers();

	// Analog Comparator initialization
	// Analog Comparator: Off
	// Analog Comparator Input Capture by Timer/Counter 1: Off
	// Analog Comparator Output: Off
	ACSR=0x80;

	LEDBlink(); //блинк светодиодом - инит закончен

}


void setLEDLevel(signed char level) // яркость светодиода в %
{
        if (level>8 || level<0) return;
        if (level==0)
        {
        	TCCR0=0x00;
        	setbit(PORTC,REDLED);
        }

        else 
        {
        	TCCR0=0x04;
	        if(level==1) OCR0 = 0;
			else if(level==2) OCR0 = 2;
				else if(level==3) OCR0 = 8;
					else if(level==4) OCR0 = 0xf;
						else if(level==5) OCR0 = 0x1f;
							else if(level==6) OCR0 = 0x3f;
								else if(level==7) OCR0 = 0x7f;
									else if(level==8) OCR0 = 0xff;
	}

//	OCR0 = 0xff*level/100;
}



void LEDBlink(void)
{

	int b=0; int d=0;
        
        for (b=0;b++<LEDELAY;)
	{
		for (d=0;d++<b;) clrbit(PORTC,REDLED);
		for (d=0;d++<(LEDELAY-b);) setbit(PORTC,REDLED);
	}
	
	for (b=0;b++<LEDELAY;)
	{
		for (d=0;d++<(LEDELAY-b);) clrbit(PORTC,REDLED);
		for (d=0;d++<b;) setbit(PORTC,REDLED);
	}
}

void LEDFlash(void)
{
	clrbit(PORTC,REDLED);
	delay_ms(1000);
	setbit(PORTC,REDLED);
	delay_ms(1000);
	clrbit(PORTC,REDLED);
	delay_ms(1000);
	setbit(PORTC,REDLED);
	delay_ms(1000);
	clrbit(PORTC,REDLED);
	delay_ms(1000);
	setbit(PORTC,REDLED);
	delay_ms(1000);
}


void reset( char const *text ) // сброс при грубой ошибке
{
	OutText (text);
  	delay_ms(1000);
  		
	typedef int(*TFunc)();
	TFunc StartAdr = (TFunc)(0);
	StartAdr();
}


char check_point(void) // возвращает 1 если check_point
{
  clrbit(PORTC,3);
  __no_operation();
  if (tstbit(PINC,2)) return 0;
  setbit(PORTC,3);
   __no_operation();
  if (!tstbit(PINC,2)) return 0;
  return 1;
}
