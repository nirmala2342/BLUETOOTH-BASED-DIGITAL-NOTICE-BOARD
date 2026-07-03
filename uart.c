/* uart.c
   UART0 interrupt-driven receive driver for LPC2129.
   HC-05 connects: HC05-TXD -> P0.1 (RXD0), HC05-RXD -> P0.0 (TXD0)
   Baud: 9600, 8N1
*/
#include <LPC21xx.H>
#include <string.h>
#include "uart.h"

char          buff[MAX_MSG_LEN];   /* message accumulator */
unsigned char r_flag = 0;          /* 1 = complete message waiting in buff */
static unsigned char dummy;
static unsigned char idx = 0;      /* current write position in buff */

/* ---------- ISR: called on every received byte ---------- */
void UART0_isr(void) __irq
{
    if(U0IIR & 0x04)               /* RDA: Receive Data Available */
    {
        char ch = U0RBR;           /* reading RBR clears the interrupt */

        if(ch == '@')             /* carriage return = end of message */
        {
            buff[idx] = '\0';      /* null-terminate */
            idx    = 0;
            r_flag = 1;            /* signal main loop */
        }
        else if(idx < MAX_MSG_LEN - 1)
        {
            buff[idx++] = ch;
        }
        /* silently drop if buffer full - shouldn't happen at 9600 baud */
    }
    else
    {
        dummy = U0IIR;             /* read to clear TX interrupt */
    }
    VICVectAddr = 0;               /* must always end ISR with this on LPC21xx */
}

/* ---------- Init ---------- */
void InitUART0(void)
{
    /* IMPORTANT: use |= not = so we don't wipe other PINSEL0 bits
       (other peripherals like I2C also live in PINSEL0)
       Bits 1:0 = 01 -> TxD0 on P0.0
       Bits 3:2 = 01 -> RxD0 on P0.1                                 */
    PINSEL0 |= 0x00000005;

    U0LCR = 0x83;   /* 8 data bits, no parity, 1 stop, DLAB=1 */
    U0DLL = 97;     /* 9600 baud @ 14.7456 MHz crystal / 4 = PCLK
                       If your board has a 12 MHz crystal use 78 instead */
    U0LCR = 0x03;   /* DLAB=0, lock baud registers */

    /* Wire UART0 interrupt into the VIC */
    VICIntSelect = 0x00000000;          /* all IRQ (not FIQ) */
    VICVectAddr0 = (unsigned)UART0_isr; /* slot 0 = highest priority */
    VICVectCntl0 = 0x20 | 6;           /* enable slot, source = UART0 (6) */
    VICIntEnable = 1 << 6;             /* unmask UART0 in VIC */

    U0IER = 0x03;   /* enable RDA + THRE interrupts */
}

/* ---------- Transmit helpers ---------- */
void UART0_Tx(char ch)
{
    while(!(U0LSR & 0x20));   /* wait for TX holding register empty */
    U0THR = ch;
}

void UART0_Str(char *s)
{
    while(*s)
        UART0_Tx(*s++);
}
