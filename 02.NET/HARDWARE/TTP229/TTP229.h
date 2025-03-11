#ifndef __TTP229_H__
#define	__TTP229_H__

#include "sys.h"

//#define	SDA_IN()	{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<8;}
//#define	SDA_OUT()	{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<8;}

#define	SDA_IN()	{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X80000000;}
#define	SDA_OUT()	{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X30000000;}

#define	TTP_SCL		PBout(6)
#define	TTP_SDO		PBout(7)
#define	TTP_SDI		PBin(7)

extern uint16_t Touch_Read(void);
extern uint16_t Touch_Read2(void);
extern void Touch_Configuration(void);
extern uint8_t Touch_KeyScan(void);

#endif

