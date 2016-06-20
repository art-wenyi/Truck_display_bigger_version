#ifndef I2C_H_
#define I2C_H_

#include "msp430f5659.h"


// I2C Settings for master initialization
//****************************************************************************//
#define 	I2C_SOURCE_CLOCK		0x00	// 0x00 SMCLK or 0x01 ACLK
#define		I2C_SOURCE_CLOCK_FREQ  SMCLK_FREQ
#define     SMCLK_FREQ			   8000000   // 1Mhz?
#define 	I2C_SOURCE_CLOCK_SMCLK  0x00
#define 	I2C_SOURCE_CLOCK_ACLK  0x01

// Display I2C clock
#define		I2C_CLOCK_DISPLAY_FREQ  400000


// Display Slave Address - 7-bit address (does not include R/nW bit)
#define I2C_Display_Write_ADDR		0x3C		// 0x78
#define I2C_Display_Read_ADDR		0x3C		// 0x79
//****************************************************************************//



// Display Global Functions
extern void I2C_Master_Display_Init(unsigned char selectClockSource,
						    unsigned long clockSourceFrequency,
						    unsigned long desiredI2CClock);

extern void I2C_Display_Set_Slave_Address(unsigned char slaveAddress);
extern void I2C_Display_Enable(void);
extern void I2C_Display_Disable(void);
extern void I2C_Display_Set_Mode(unsigned short receive);
extern unsigned short I2C_Display_Bus_Busy(void);
extern unsigned short I2C_Display_Busy(void);
extern void I2C_Display_Interrupt_Enable(unsigned char interruptFlags);
extern void I2C_Display_Interrupt_Disable(unsigned char interruptFlags);
extern void I2C_Display_Interrupt_Clear(unsigned char interruptFlags);
extern unsigned char I2C_Display_Interrupt_Status(unsigned char mask);
extern void I2C_Write_Packet_To_Display(unsigned char displayRegName,
   								   unsigned char dataLength,
								   unsigned char writeData);



#endif /*I2C_H_*/

