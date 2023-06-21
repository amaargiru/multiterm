#ifndef _delay_handling_  // Макросы для задержек
#define _delay_handling_

#define MasterClock 18432000  // Частота кварца

#define delay_us(c) __delay_cycles(MasterClock/1000000*c);  // Микросекундная задержка
#define delay_ms(c) __delay_cycles(MasterClock/1000*c);  // Миллисекундная задержка
#endif
