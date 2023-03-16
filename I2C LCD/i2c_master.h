#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 4000000

#ifndef IIC_MASTER_H
#define IIC_MASTER_H

void I2C_Idle (void);
void I2C_Master_Init (void);
void I2C_Start (void);
void I2C_Stop (void);
void I2C_Repeatead_Start (void);
void I2C_ACK (void);
void I2C_NACK (void);
uint8_t I2C_Write (uint8_t Data);
uint8_t I2C_Read (unsigned short ACK_Det);

#endif	

