/*
  CodeVisionAVR C Compiler
  (C) 1998-2000 Pavel Haiduc, HP InfoTech S.R.L.

  Prototypes for Dallas Semiconductor
  DS1820 1 Wire bus temperature sensor
  functions

  BEFORE #include -ING THIS FILE YOU
  MUST DECLARE THE I/O ADDRESS OF THE
  DATA REGISTER OF THE PORT AT WHICH
  THE 1 WIRE BUS IS CONNECTED AND
  THE DATA BIT USED

  EXAMPLE FOR PORTB:

    #asm
        .equ __w1_port=0x18
        .equ __w1_bit=3
    #endasm
    #include <ds1820.h>
*/



#ifndef _DS1820_INCLUDED_
#define _DS1820_INCLUDED_

#include <iom162.h>
#include <inavr.h>
#include "kvarz.h"
#include "bithandler.h"

#define DS_PORT PORTC  // настрой куда подпаян градусник !!!
#define DS_DIR DDRC
#define DS_PIN PINC
#define DS_bit 0

#define timeA 6                    // НАСТРОЙКИ времени для обращения к 1wire устройствам (см pdf)
#define timeB 64
#define timeC 60
#define timeD 10
#define timeE 9
#define timeF 55
#define timeG 0
#define timeH 480
#define timeI 70
#define timeJ 410
#define time_gradusnik_delay_ms 550


class ds1820 {

public:
        ds1820(void); // конструктор
	signed int tC(void); // выдает температуру в градусах
//	void get_uid(unsigned char *addr); 
	unsigned char uidDS[8]; // выдает uid 
	void WriteResetCnt(unsigned int Cnt);
	unsigned int ReadResetCnt(void);

	struct __ds1820_scratch_pad_struct  // внутреннее представление данных в термометре
	       {
	       unsigned char temp_lsb,
        	             temp_msb,
                	     user_high,
	                     user_low,
        	             res1,
                	     res2,
	                     cnt_rem,
        	             cnt_c,
                	     crc;
	       } __ds1820_scratch_pad;


private:


//	unsigned char DS_PORT;  // настрой куда подпаян градусник !!!
//	unsigned char DS_DIR;
//	unsigned char DS_bit;   // и на какую ножку
	
	unsigned char w1_read(void);
	void w1_write(unsigned char data);
	unsigned char w1_init(void);
	unsigned char w1_readBit(void);
	void w1_writeBit(unsigned char bit);
	unsigned char ds1820_init(void);
	unsigned char ds1820_read_spd(void);
	unsigned char testDQ(void);
};





#endif

