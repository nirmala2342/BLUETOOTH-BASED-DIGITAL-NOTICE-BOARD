/* main.c - Bluetooth + Dot Matrix Scroll integration
   Flow:
   1. Boot -> scroll "Waiting for message" in a loop
   2. When phone sends $$786<your message>$$  via Bluetooth,
      the UART ISR fills buff[] and sets r_flag=1
   3. main() detects r_flag, validates passkey "786", extracts message
   4. Updates display message immediately (no wait for scroll to finish)
   5. Loops scrolling the new message until another BT message arrives
*/
#include <LPC21xx.H>
#include <string.h>
#include "delays.h"
#include "defines.h"
#include "74LS164.h"
#include "row.h"
#include "dml.h"
#include "uart.h"
#include "i2c.h"          /* ADDED */
#include "i2c_eeprom.h"   /* ADDED */

/* ---- passkey: change this if your project uses a different code ---- */
#define PASSKEY "786"

/* ADDED: EEPROM slave address (default when A0-A2 are grounded on the module) */
#define EEPROM_ADDR 0x50

/* ---- active scroll message (what's currently on screen) ---- */
char display_msg[MAX_MSG_LEN + 8] = "   WAITING FOR MESSAGE   ";

/* ---- validate $$786<message>$$ and extract just the message part ---- */
int parse_message(char *raw, char *out)
{
    char *p;
    char *end;
    /* must start with $$ */
    if(raw[0] != '$' || raw[1] != '$')
        return 0;
    p = raw + 2;  /* move past $$ */
    /* next must be the passkey */
    if(strncmp(p, PASSKEY, strlen(PASSKEY)) != 0)
        return 0;
    p += strlen(PASSKEY);  /* move past passkey */
    /* must end with $$ */
    end = strstr(p, "$$");
    if(end == NULL)
        return 0;
    /* copy only the message portion between passkey and closing $$ */
    strncpy(out, p, end - p);
    out[end - p] = '\0';
    return 1;   /* success */
}

int main()
{
    int i;
    int msgLen;
    char extracted[MAX_MSG_LEN];

    Init_ANODES();
    Init_74LS164();
    InitUART0();   /* starts UART0 interrupt, HC-05 now active */
    init_i2c();    /* ADDED: bring up I2C0 for the EEPROM */

    /* ADDED: try to restore the last saved message from EEPROM on boot */
    {
        u8 savedLen = i2c_eeprom_read(EEPROM_ADDR, 0);   /* addr 0 stores length */
        if(savedLen > 0 && savedLen < MAX_MSG_LEN - 8)
        {
            char restored[MAX_MSG_LEN];
            i2c_eeprom_seq_read(EEPROM_ADDR, 1, (u8*)restored, savedLen);
            restored[savedLen] = '\0';
            strcpy(display_msg, "   ");
            strcat(display_msg, restored);
            strcat(display_msg, "   ");
        }
    }

    while(1)
    {
        /* --- check if a new Bluetooth message arrived --- */
        if(r_flag == 1)
        {
            r_flag = 0;   /* clear flag first so ISR can refill buff */
            if(parse_message(buff, extracted))
            {
                /* valid message received - update display */
                /* add leading/trailing spaces so text scrolls in cleanly */
                strcpy(display_msg, "   ");
                strcat(display_msg, extracted);
                strcat(display_msg, "   ");

                /* ADDED: persist this message to EEPROM so it survives power loss */
                {
                    u8 len = (u8)strlen(extracted);
                    i2c_eeprom_write(EEPROM_ADDR, 0, len);
                    i2c_eeprom_page_write(EEPROM_ADDR, 1, (u8*)extracted, len);
                }

                /* optional: echo back to phone to confirm receipt */
                UART0_Str("MSG OK: ");
                UART0_Str(extracted);
                UART0_Str("\r\n");
            }
            else
            {
                /* wrong passkey or format - tell sender */
                UART0_Str("INVALID: use $$786YourMessage$$\r\n");
            }
        }
        /* --- scroll one full pass of the current message --- */
        /* display_scroll shows 4 chars at a time; loop shifts window
           across the whole string one character per step             */
        msgLen = strlen(display_msg);
        for(i = 0; i < msgLen - 3; i++)
        {
            display_scroll((unsigned char *)(display_msg + i));
            /* check for new BT message after each step so we respond
               quickly without waiting for the whole scroll to finish */
            if(r_flag == 1)
                break;
        }
    }
}