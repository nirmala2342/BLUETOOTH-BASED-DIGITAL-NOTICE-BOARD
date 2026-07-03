/* i2c_defines.h */
#ifndef __I2C_DEFINES_H__
#define __I2C_DEFINES_H__

/* defines for pin function selection (P0.2 = SCL0, P0.3 = SDA0) */
#define SCL_EN  0x00000010
#define SDA_EN  0x00000040

/* defines for I2C speed configuration
   IMPORTANT: set CCLK to match YOUR actual crystal/PLL setup.
   If unsure, check your delays.h / existing UART baud divider comment
   for the PCLK value already used in your project and match it here. */
#define CCLK      60000000  /* Hz - change if your board runs a different clock */
#define PCLK      CCLK/4    /* Hz */
#define I2C_SPEED 100000    /* Hz - standard mode */
#define LOADVAL   ((PCLK/I2C_SPEED)/2)

/* bit defines for I2CONSET sfr */
#define   AA_BIT 2
#define   SI_BIT 3
#define  STO_BIT 4
#define  STA_BIT 5
#define I2EN_BIT 6

#endif