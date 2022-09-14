
// основные настройки программы


#include <iom162.h>
//#include <mega162.h>
//#include <inavr.h>
#include <ina90.h>


// DS1820 Temperature Sensor functions
#include "ds1820.h"
#include "kvarz.h"
#include "usart.h"
#include "timers.h"
#include "bithandler.h"
#include "bluetooth.h"
#include "GR_47.h"
 
#define LEDELAY		700
#define REDLED	        1

void reset( char const *text ); // сброс при грубой ошибке
void BeginInit(void); // начальные установки
void LEDBlink(void);  // плавно моргает светодиодом
void LEDFlash(void);
void setLEDLevel(signed char level); // €ркость светодиода в %
char check_point(void); // возвращает 1 если check_point



