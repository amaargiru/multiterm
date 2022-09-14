
// �������� ��������� ���������


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

void reset( char const *text ); // ����� ��� ������ ������
void BeginInit(void); // ��������� ���������
void LEDBlink(void);  // ������ ������� �����������
void LEDFlash(void);
void setLEDLevel(signed char level); // ������� ���������� � %
char check_point(void); // ���������� 1 ���� check_point



