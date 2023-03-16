#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "Configuration Bits.h"
#include "i2c_master.h"
#include "lcd.h"
#include "bmp180.h"
#define I2C_BaudRate 100000
#define _XTAL_FREQ 4000000

float Temperature , Pressure , Altitude;
char text1 [7];
char text2 [7];
char text3 [7];

void main(void) 
{  
   I2C_Master_Init ();
   /* Initialize LCD module with I2C address = 0x4E (Write)
   According to PCF8574 Remote 8-Bit I/O Expander for I2C Bus */
   LCD_Init_Add (0x4E);     
   LCD_Set_Cursor (1,1);
   LCD_Write_String ("Temperature =\0");
   LCD_Set_Cursor (2,1);
   LCD_Write_String ("Pressure =\0");
   LCD_Set_Cursor (3,1);
   LCD_Write_String ("Altitude =\0");
   
   
  while(1)
  {
      Read_Calibration_data ();
      __delay_ms(10);
      
      Temperature = Get_Temp_Value ();
      __delay_ms(10);
      LCD_Set_Cursor (1,15);
      text1 [0] = '\0';
      sprintf (text1 , "%f" ,Temperature);
      LCD_Write_String (text1);
      
      Pressure = Get_Press_Value (0);
      __delay_ms(10);
      LCD_Set_Cursor (2,12);
      text2 [0] = '\0';
      sprintf (text2 , "%f" ,Pressure);
      LCD_Write_String (text2);
      
      Altitude = Get_Altitude_Value (0);
      __delay_ms(10);
      LCD_Set_Cursor (3,12);
      text3 [0] = '\0';
      sprintf (text3 , "%f" ,Altitude);
      LCD_Write_String (text3);
     
  }
    
    return;
}



