#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "Configuration Bits.h"
#include "LCD_Driver.h"
#include "SPI.h"
#define _XTAL_FREQ 4000000
#define ss RD6

void SPI_Init (void);
void SPI_write (unsigned char data);
char SPI_read(char dummy_data);

char text [11];
uint16_t WriteToCntReg = 0x80; 
uint16_t MSBTempRead = 0x02;
char Temp_data;
void main(void)
{
    TRISD = 0x00;  PORTD = 0x00;
    SPI_Init ();
    LCDInit ();

    while (1)
    {
      ss = 1;
      SPI_write(0x80);
      SPI_write (0);
      ss = 0;
      
      __delay_ms (250);
      
      ss = 1;
      SPI_write(0x02);
      Temp_data = SPI_read(0);
      ss = 0;
      
      sprintf (text , "Temp is %d" , Temp_data);
      LCDSetCursor (1 , 1);
      LCDWritestr (text , 11);
    }
    
    return;
}
