#ifndef _bit_handling_  // Макросы для битовых операций
#define _bit_handling_
#define bit(n)		(1 << (n))
#define setbit(p,n)	(p|=bit(n))
#define clrbit(p,n)	(p&=~bit(n))
#define invbit(p,n)	(p=p^bit(n))
#define tstbit(p,n)	(p&bit(n))
#endif
