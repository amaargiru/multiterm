


#ifndef _delay_handling_                                       // ������ ��� ����থ�
#define _delay_handling_
#define MasterClock 8000000                                  // ���� ����� - ����� !!!
#define delay_us(c) __delay_cycles(MasterClock/1000000*c);   // ����ᥪ㭤��� ����প�
#define delay_ms(c) __delay_cycles(MasterClock/1000*c);      // �����ᥪ㭤��� ����প�
#endif
