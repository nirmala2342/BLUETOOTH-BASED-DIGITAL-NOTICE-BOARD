#include <LPC21xx.h>
#include "types.h"
#include "delays.h"
#include "i2c_defines.h"

void init_i2c(void)
{
    /* Configure I/O pin for SCL & SDA functions using PINSEL0 */
    PINSEL0 |= (SCL_EN | SDA_EN);   /* use |= so we don't wipe other PINSEL0 bits
                                        (UART0, shift register pins, etc.) */
    I2SCLL = LOADVAL;
    I2SCLH = LOADVAL;
    I2CONSET = 1 << I2EN_BIT;       /* I2C peripheral enable */
}

void i2c_start(void)
{
    I2CONSET = 1 << STA_BIT;
    while(((I2CONSET >> SI_BIT) & 1) == 0);
    I2CONCLR = 1 << STA_BIT;
}

void i2c_restart(void)
{
    I2CONSET = 1 << STA_BIT;
    I2CONCLR = 1 << SI_BIT;
    while(((I2CONSET >> SI_BIT) & 1) == 0);
    I2CONCLR = 1 << STA_BIT;
}

void i2c_write(u8 dat)
{
    I2DAT = dat;
    I2CONCLR = 1 << SI_BIT;
    while(((I2CONSET >> SI_BIT) & 1) == 0);
}

void i2c_stop(void)
{
    I2CONSET = 1 << STO_BIT;
    I2CONCLR = 1 << SI_BIT;
}

u8 i2c_nack(void)
{
    I2CONSET = 0x00;
    I2CONCLR = 1 << SI_BIT;
    while(((I2CONSET >> SI_BIT) & 1) == 0);
    return I2DAT;
}

u8 i2c_masterack(void)
{
    I2CONSET = 0x04;
    I2CONCLR = 1 << SI_BIT;
    while(((I2CONSET >> SI_BIT) & 1) == 0);
    I2CONCLR = 0x04;
    return I2DAT;
}