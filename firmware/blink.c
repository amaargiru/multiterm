#include <iom16.h>
#include <inavr.h>
#include <stdio.h>

#define bit(n)			(1 << (n))
#define setbit(p,n)	(p|=bit(n))
#define clrbit(p,n)	(p&=~bit(n))
#define invbit(p,n)	(p=p^bit(n))
#define tstbit(p,n)	(p&bit(n))

#define RXB8	1
#define TXB8	0
#define UPE		2
#define OVR		3
#define FE		4
#define UDRE	5
#define RXC		7

#define FRAMING_ERROR			(1<<FE)
#define PARITY_ERROR				(1<<UPE)
#define DATA_OVERRUN				(1<<OVR)
#define DATA_REGISTER_EMPTY	(1<<UDRE)
#define RX_COMPLETE				(1<<RXC)

#define REDLED	7

#define LEDELAY					400
#define TXDELAY 					500
#define RXBUFLENGTH				128

#define MasterClock 7500000
#define delay_us(c) __delay_cycles(MasterClock/1000000*c);
#define delay_ms(c) __delay_cycles(MasterClock/1000*c);

#define DS_PORT PORTA 
#define DS_DIR DDRA
#define DS_PIN PINA
unsigned char DS_bit  = 0;

#define timeA 6
#define timeB 64
#define timeC 60
#define timeD 10
#define timeE 5			////
#define timeF 55
#define timeG 0
#define timeH 480
#define timeI 70
#define timeJ 410
#define time_gradusnik_delay_ms 1000

unsigned char uidDS[8];

struct __ds1820_scratch_pad_struct
{
	unsigned char	temp_lsb,
						temp_msb,
						user_high,
						user_low,
						res1,
						res2,
						cnt_rem,
						cnt_c,
						crc;
} __ds1820_scratch_pad;

unsigned char RxBuffer[RXBUFLENGTH];
unsigned char RxBufWritePoint = 0;
unsigned char RxBufReadPoint = 0;
unsigned int temp = 0; 

void InitPorts(void);
void LEDBlink(void);
void InitUSART(void);
int putchar(int data);
unsigned char USARTReceive(void);
void ClearRxBuffer(void);

unsigned char tC(void); 			
unsigned char w1_read(void);
void w1_write(unsigned char data);
unsigned char w1_init(void);
unsigned char w1_readBit(void);
void w1_writeBit(unsigned char bit);
unsigned char ds1820_init(void);
unsigned char ds1820_read_spd(void);
unsigned char testDQ(void)
{
	if ((DS_PIN&(1 << DS_bit))) return 1;
		else
		{
			return 0;
		}
}
void setDQ()
{
DS_DIR&=~(1 << (DS_bit)); __no_operation();
}
void clrDQ()
{
DS_DIR|=(1 << (DS_bit));
DS_PORT&=~(1 << (DS_bit));
}

void main(void)
{
	InitPorts();
	InitUSART();
	ClearRxBuffer();
	LEDBlink();
	__enable_interrupt();
	printf("\n\r###8 x DS18S20 Data Acquisition System###\n\r");
	
	while (1)
	{		
		if (RxBuffer[0] != 0)
		{
			RxBuffer[0] = 0;
			RxBufWritePoint = 0;

			DS_bit = 0;
			ds1820_init();
			printf("%c", tC());

			DS_bit = 1;
			ds1820_init();
			printf("%c", tC());

			DS_bit = 2;
			ds1820_init();
			printf("%c", tC());

			DS_bit = 3;
			ds1820_init();
			printf("%c", tC());

			DS_bit = 4;
			ds1820_init();
			printf("%c", tC());

			DS_bit = 5;
			ds1820_init();
			printf("%c", tC());

			DS_bit = 6;
			ds1820_init();
			printf("%c", tC());

			DS_bit = 7;
			ds1820_init();
			printf("%c\r\n", tC());

			invbit(PORTD,REDLED);
		}
	}
}

unsigned char w1_read(void)
{
	int loop, result=0;

	__disable_interrupt();
	for (loop = 0; loop < 8; loop++)
	{
		result >>= 1;
		if (w1_readBit())
			result |= 0x80;
	}
	__enable_interrupt();

	return result;
}

void w1_write(unsigned char data)
{
	int loop;

	__disable_interrupt();
	for (loop = 0; loop < 8; loop++)
	{
		w1_writeBit(data & 0x01);
		data >>= 1;
	}
	__enable_interrupt();

}

unsigned char w1_init(void)
{
	int result;

	__disable_interrupt();
	delay_us(timeG);
	clrDQ();
	delay_us(timeH);
	setDQ();
	delay_us(timeI);
	result = testDQ();
	delay_us(timeJ);
//	if (result == 1) printf ("testDQ Error. DS_PIN = %d, DS_bit = %d.\r\n", DS_PIN, DS_bit);
	__enable_interrupt();
	return result;
}

unsigned char w1_readBit(void)
{
	int result;
	clrDQ();
	delay_us(timeA);
	setDQ();
	delay_us(timeE);
	result = testDQ();
	delay_us(timeF);
	return result;
}

void w1_writeBit(unsigned char bit)
{
	if (bit)
	{
		clrDQ();
		delay_us(timeA);
		setDQ();
		delay_us(timeB);
	}
	else
	{
		clrDQ();
		delay_us(timeC);
		setDQ();
		delay_us(timeD);
	}
}

unsigned char ds1820_init(void)
{
	unsigned char i;
	if (w1_init())
	{
		for (i=0; i<8; i++) 
			uidDS[i] = 0;
		return 0;
	}
	else
	{
		w1_write(0x33);
		for (i=0; i<8; i++) 
		uidDS[i] = w1_read();
		return 1;
	}		
}

unsigned char ds1820_read_spd(void)
{
	unsigned char i;
	unsigned char *p;

	i=0;
	p=(unsigned char *) &__ds1820_scratch_pad;
	do  *(p++)=0xFF; while (++i<9);

	if (!ds1820_init()) return 0;
	w1_write(0xBE);  //READ SCRATCHPAD

	i=0;
	p=(unsigned char *) &__ds1820_scratch_pad;
	do  *(p++)=w1_read(); while (++i<9);

	return 1;
}

unsigned char tC(void)
{
	if (!ds1820_init()) return 0xFF;
	w1_write(0x44);
	delay_ms(time_gradusnik_delay_ms);
	if (ds1820_read_spd()==0) return 0xFF;
	w1_init();
	switch (DS_bit)
	{
		case 0 :
		case 6 :
		case 7 :
			return (unsigned char)((signed int)(((__ds1820_scratch_pad.temp_msb<<8) + __ds1820_scratch_pad.temp_lsb)/16) + 55);
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
			return (unsigned char)((signed int)(((__ds1820_scratch_pad.temp_msb<<8) + __ds1820_scratch_pad.temp_lsb)/2) + 55);
	}
	return 0xFF;
}

void InitPorts(void)
{
	DDRB = 0x00;

	DDRD = 0x00;
	setbit(DDRD,PIND1);		// TxD
	setbit(DDRD,PIND7);
	
	DDRC = 0x00;
	DDRA = 0xFF;
}

void LEDBlink(void)
{
	int a=0; int b=0; int d=0;

	for (;a++<3;)
	{
		for (b=0;b++<LEDELAY;)
		{
			for (d=0;d++<b;) clrbit(PORTD,REDLED);
			for (d=0;d++<(LEDELAY-b);) setbit(PORTD,REDLED);
		}
		for (b=0;b++<LEDELAY;)
		{
			for (d=0;d++<(LEDELAY-b);) clrbit(PORTD,REDLED);
			for (d=0;d++<b;) setbit(PORTD,REDLED);
		}
	}
	clrbit(PORTD,REDLED);
}

void InitUSART(void)
{
// USART Baud rate: 9600
	UCSRA=0x00;
	UCSRB=0x98;
	UCSRC=0x86;
	UBRRH=0x00;
	UBRRL=0x33;
}

int putchar( int data )
{
	while ( !( UCSRA & (1<<UDRE)) );	// Wait for empty transmit buffer
	UDR = data;								// Put data into buffer, sends the data
	__delay_cycles(TXDELAY);
	return data;
}

unsigned char USARTReceive( void )
{
	while ( !(UCSRA & (1<<RXC)) );	// Wait for data to be received
	return UDR;								// Get and return received data from buffer
}

#pragma vector=USART_RXC_vect
__interrupt void USART_RXC_interrupt(void)
{
	RxBuffer[RxBufWritePoint]=UDR;
	if(RxBufWritePoint == RXBUFLENGTH-1) RxBufWritePoint=0;
		else RxBufWritePoint++;
}

void ClearRxBuffer(void)
{
	unsigned int a = 0;
	for (;a++<RXBUFLENGTH;RxBuffer[a] = 0);
}
