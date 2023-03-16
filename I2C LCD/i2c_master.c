#include <xc.h>
#include <stdint.h>
#include "i2c_master.h"
#define I2C_BaudRate 100000
#define _XTAL_FREQ 4000000

void I2C_Idle (void)
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  
}

void I2C_Master_Init (void)
{
  TRISC3 = 1;  // SCL as Input
  TRISC4 = 1;  // SDA as Input
  SSPSTAT = 0x00;  //slew rate enables for high speed control
  SSPCON = 0x28;
  SSPCON2 = 0x00; 
  SSPADD = ( (_XTAL_FREQ / 4) / I2C_BaudRate ) - 1;
}

void I2C_Start (void)
{
   I2C_Idle ();
   SEN = 1;
}

void I2C_Stop (void)
{
   I2C_Idle ();
   PEN = 1;
}

void I2C_Repeatead_Start (void)
{
   I2C_Idle ();
   RSEN = 1;
}

void I2C_ACK (void)
{
  I2C_Idle ();
  ACKEN = 1;  // Initiate Acknowledge Sequence
  ACKDT = 0;  // Acknowledge      
}

void I2C_NACK (void)
{
  I2C_Idle ();
  ACKEN = 1;  //Initiate Acknowledge Sequence
  ACKDT = 1;  // Not Acknowledge     
}

uint8_t I2C_Write (uint8_t Data)
{
  I2C_Idle ();
  SSPBUF = Data;
  while(!SSPIF); // Wait Until Completion
  SSPIF = 0;
  return ACKSTAT;
}

uint8_t I2C_Read (unsigned short ACK_Det)
{
    I2C_Idle ();
    RCEN = 1;
    while(!SSPIF); // Wait Until Completion
    SSPIF = 0;
    I2C_Idle ();
    ACKDT = (ACK_Det)? 0 : 1;
    ACKEN = 1;
    return SSPBUF;
}