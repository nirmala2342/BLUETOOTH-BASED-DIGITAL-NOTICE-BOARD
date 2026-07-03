/* 74LS164.c - UPDATED for 4 displays (DM1-DM4) */
#include "defines.h"
#include "delays.h"
#include <LPC21xx.h>

#define SIN1  8     //p0.8  - DM1 DSA
#define CP1   9     //p0.9  - DM1 CP
#define SIN2  10    //p0.10 - DM2 DSA
#define CP2   11    //p0.11 - DM2 CP
#define SIN3  12    //p0.12 - DM3 DSA
#define CP3   13    //p0.13 - DM3 CP
#define SIN4  14    //p0.14 - DM4 DSA
#define CP4   15    //p0.15 - DM4 CP

void Init_74LS164(void)
{
	IODIR0 |= (1<<CP1|1<<SIN1|1<<CP2|1<<SIN2|1<<CP3|1<<SIN3|1<<CP4|1<<SIN4);
}

/* shifts one byte into ONE specific display's shift register
   dispNum: 1,2,3, or 4 */
void SIPO_74LS164_disp(unsigned char sDat, unsigned int dispNum)
{
   unsigned char i, sinPin, cpPin;

   switch(dispNum)
   {
     case 1: sinPin=SIN1; cpPin=CP1; break;
     case 2: sinPin=SIN2; cpPin=CP2; break;
     case 3: sinPin=SIN3; cpPin=CP3; break;
     case 4: sinPin=SIN4; cpPin=CP4; break;
     default: return;
   }

   for(i=0;i<8;i++)
   {
		   WRITEBIT(IOPIN0,sinPin,((sDat>>(7-i))&1));
		   SSETBIT(IOSET0,cpPin);
		   delay_us(1);
		   SCLRBIT(IOCLR0,cpPin);
		   delay_us(1);
   }
}

/* shifts a byte into ALL 4 displays at once (used for clearing, etc) */
void SIPO_74LS164_all(unsigned char sDat)
{
	SIPO_74LS164_disp(sDat,1);
	SIPO_74LS164_disp(sDat,2);
	SIPO_74LS164_disp(sDat,3);
	SIPO_74LS164_disp(sDat,4);
}
