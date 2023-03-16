#include <xc.h>
#include <stdint.h>
#include "Configuration Bits.h"
#include "i2c_master.h"
#include "lcd.h"
#define I2C_BaudRate 100000
#define _XTAL_FREQ 4000000


void main(void) 
{  
   I2C_Master_Init ();
   /* Initialize LCD module with I2C address = 0x4E (Write)
   According to PCF8574 Remote 8-Bit I/O Expander for I2C Bus */
   LCD_Init_Add (0x4E);     
 
  LCD_Set_Cursor(1, 3);
  LCD_Write_String("Islam Negm");
  __delay_ms(1000);
 
  while(1)
  {
      for (int i = 0; i < 4; i++)
      {
        LCD_SR();
        __delay_ms(85);  
      }
      for (int i = 0; i < 4; i++)
      {
        LCD_SL();
        __delay_ms(85);  
      }
      
      
      for (int i = 0; i < 2; i++)
      {
        LCD_SL();
        __delay_ms(85);  
      }
      for (int i = 0; i < 2; i++)
      {
        LCD_SR();
        __delay_ms(85);  
      }
  }
    
    return;
}



