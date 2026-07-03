/* uart.h */
#ifndef __UART_H__
#define __UART_H__

#define MAX_MSG_LEN 80   /* max characters we can receive in one message */

extern char  buff[MAX_MSG_LEN]; /* raw received buffer, filled by ISR */
extern unsigned char r_flag;    /* set to 1 by ISR when full message received */

void InitUART0(void);
void UART0_Tx(char ch);
void UART0_Str(char *s);

#endif
