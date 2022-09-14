

/*
  Dallas Semiconductor DS1820 1 Wire bus temperature sensor functions
*/

// DS1820 Temperature Sensor functions
#include "ds1820.h"


//
// просто для удобочитаемости
//
#define setDQ clrbit(DS_DIR,DS_bit);  __no_operation(); 
#define clrDQ setbit(DS_DIR,DS_bit); clrbit(DS_PORT,DS_bit);
unsigned char ds1820::testDQ(void)
{
	if (tstbit(DS_PIN,DS_bit)) return 1;
	  else return 0;
}
#define tstDQ testDQ();


//-----------------------------------------------------------------------------
//             конструктор класса
// при создании берем uid термометра 
//
ds1820::ds1820(void)
{
	ds1820_init();
}



//-----------------------------------------------------------------------------
// Generate a 1-Wire reset, return 1 if no presence detect was found,
// return 0 otherwise.
//
//
unsigned char ds1820::w1_init(void)
{
	int result;
	delay_us(timeG);
	clrDQ;		// Drives DQ low
	delay_us(timeH);
	setDQ; 		// Releases the bus
	delay_us(timeI);
	result = tstDQ; // Sample for presence pulse from slave
	delay_us(timeJ); // Complete the reset sequence recovery
	return result; 	// Return sample presence pulse result
}



//-----------------------------------------------------------------------------
// Send a 1-Wire write bit. Provide 10us recovery time.
//
void ds1820::w1_writeBit(unsigned char bit)
{
	if (bit)
	{
		// Write '1' bit
		clrDQ; 	// Drives DQ low
		delay_us(timeA);
		setDQ; // Releases the bus
		delay_us(timeB); // Complete the time slot and 10us recovery
	}

	else
	{
		// Write '0' bit
		clrDQ; // Drives DQ low
		delay_us(timeC);
		setDQ; // Releases the bus
		delay_us(timeD);
	}
}



//-----------------------------------------------------------------------------
// Read a bit from the 1-Wire bus and return it. Provide 10us recovery time.
//
unsigned char ds1820::w1_readBit(void)
{
	int result;
	clrDQ; 		// Drives DQ low
	delay_us(timeA);
	setDQ; 		// Releases the bus
	delay_us(timeE);
	result = tstDQ; // Sample the bit value from the slave
	delay_us(timeF); // Complete the time slot and 10us recovery
	return result;
}



//-----------------------------------------------------------------------------
// Write 1-Wire data byte
//
void ds1820::w1_write(unsigned char data)
{
	int loop;
	// Loop to write each bit in the byte, LS-bit first
	for (loop = 0; loop < 8; loop++)
	{
		w1_writeBit(data & 0x01);
		// shift the data byte for the next bit
		data >>= 1;
	}
}



//-----------------------------------------------------------------------------
// Read 1-Wire data byte and return it
//
unsigned char ds1820::w1_read(void)
{
	int loop, result=0;
	for (loop = 0; loop < 8; loop++)
	{
		// shift the result to get it ready for the next bit
		result >>= 1;
		// if result is one, then set MS bit
		if (w1_readBit())
			result |= 0x80;
	}
	return result;
}


//-------------------------------------------------
//
//  дальше больше :)
//
unsigned char ds1820::ds1820_init(void)
{

	if (w1_init())
	{
		for (unsigned char i=0; i<8; i++) 
			uidDS[i] = 0;  // не отвечает (или не запаян)
		return 0;
	}

	    else
	    {
	        w1_write(0x33);
		for (unsigned char i=0; i<8; i++) 
		uidDS[i] = w1_read();  // берем uid термометра
		return 1;
	    }		
	
}


unsigned char ds1820::ds1820_read_spd(void)
{
	unsigned char i;
	unsigned char *p;
	if (!ds1820_init()) return 0;
	w1_write(0xbe);  //READ SCRATCHPAD
	i=0;
	p=(unsigned char *) &__ds1820_scratch_pad;

	do  *(p++)=w1_read(); while (++i<9);

	if (__ds1820_scratch_pad.crc) return 1; else return 0;
}

void ds1820::WriteResetCnt(unsigned int Cnt)
{

	if (!ds1820_init()) return;
	w1_write(0x4e);  //write SCRATCHPAD
	w1_write((unsigned char) (Cnt>>8));  
	w1_write((unsigned char) Cnt);  
	if (!ds1820_init()) return;
	w1_write(0x48);  // собственно запоминаем значение в еепроме
}

unsigned int ds1820::ReadResetCnt(void)
{
	if (ds1820_read_spd()==0) return 65535;
	w1_init();
	return (((unsigned int)__ds1820_scratch_pad.user_high<<8)| __ds1820_scratch_pad.user_low);
}


signed int ds1820::tC(void)
{
	if (!ds1820_init()) return -9999;
	w1_write(0x44); // CONVERT T
	delay_ms(time_gradusnik_delay_ms);
	if (ds1820_read_spd()==0) return -9999;
	w1_init();
	return (((signed int)__ds1820_scratch_pad.temp_msb<<8)| __ds1820_scratch_pad.temp_lsb)/2;
}

