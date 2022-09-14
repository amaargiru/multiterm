


#ifndef _delay_handling_                                       // макросы для задержек
#define _delay_handling_
#define MasterClock 8000000                                  // частота кварца - важно !!!
#define delay_us(c) __delay_cycles(MasterClock/1000000*c);   // микросекундная задержка
#define delay_ms(c) __delay_cycles(MasterClock/1000*c);      // миллисекундная зажержка
#endif
