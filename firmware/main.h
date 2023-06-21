#include <iom162.h>
#include <ina90.h>

// DS1820 Temperature Sensor functions
#include "ds1820.h"
#include "kvarz.h"
#include "usart.h"
#include "timers.h"
#include "bithandler.h"
#include "bluetooth.h"
#include "GR_47.h"

#define LEDELAY	700
#define REDLED 1

void reset(char const* text); // Сброс при грубой ошибке
void BeginInit(void); // Начальные установки
void LEDBlink(void);  // Плавно моргает светодиодом
void LEDFlash(void);
void setLEDLevel(signed char level); // Яркость светодиода в %
char check_point(void); // Возвращает 1 если check_point
