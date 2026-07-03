/* row.c
   NEW FILE - replaces the single-byte ROW write that the original
   reference code did directly on P0.0-P0.7.
   ANODES connector is now wired to these 8 non-contiguous pins:
   P0.21, P0.22, P0.23, P0.24, P0.25, P0.27, P0.28, P0.29
   (P0.0-P0.7 are reserved: P0.0/P0.1 = Bluetooth UART0,
    P0.2/P0.3 = EEPROM I2C0)
*/
#include <LPC21xx.H>
#include "defines.h"
#include "row.h"

void Init_ANODES(void)
{
    /* set all 8 ANODES pins as GPIO outputs */
    IODIR1 |= (255UL<<23);
}

void write_row_data(unsigned char rowByte)
{
	WRITEBYTE(IOPIN1,23,rowByte);
   /* WRITEBIT(IOPIN0, 21, (rowByte>>0)&1);
    WRITEBIT(IOPIN0, 22, (rowByte>>1)&1);
    WRITEBIT(IOPIN0, 23, (rowByte>>2)&1);
    WRITEBIT(IOPIN0, 24, (rowByte>>3)&1);
    WRITEBIT(IOPIN0, 25, (rowByte>>4)&1);
    WRITEBIT(IOPIN0, 27, (rowByte>>5)&1);
    WRITEBIT(IOPIN0, 28, (rowByte>>6)&1);
    WRITEBIT(IOPIN0, 29, (rowByte>>7)&1);*/
}
