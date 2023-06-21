#ifndef _DS1820_INCLUDED_
#define _DS1820_INCLUDED_

#include <iom162.h>
#include <inavr.h>
#include "kvarz.h"
#include "bithandler.h"

#define DS_PORT PORTC
#define DS_DIR DDRC
#define DS_PIN PINC
#define DS_bit 0

#define timeA 6 // Настройки времени для обращения к 1wire устройствам (см документацию на термометр)
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
