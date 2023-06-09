#include "LCD.h"
#include "i2c_master.h"

unsigned char RS, i2c_add, BackLight_State = LCD_BACKLIGHT;

void LCD_Init_Add (unsigned char I2C_Add)
{
  i2c_add = I2C_Add;
  IO_Expander_Write(0x00);
  
  __delay_ms(30);          // Wait for more than 15 ms after VCC rises to 4.5 V
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  
  LCD_CMD(LCD_RETURN_HOME);
  __delay_ms(5);
  
  LCD_CMD(LCD_TYPE);
  __delay_ms(50);
  
  LCD_CMD(LCD_TURN_ON);
  __delay_ms(50);
  
  LCD_CMD(LCD_CLEAR);     
  __delay_ms(50);
  
  LCD_CMD(LCD_ENTRY_MODE_SET + LCD_RETURN_HOME); 
  __delay_ms(50);

}
 
void IO_Expander_Write (unsigned char Data)
{
  I2C_Start();
  I2C_Write(i2c_add);
  I2C_Idle ();
  I2C_Write (Data);
  I2C_Stop();
}
 
void LCD_Write_4Bit (unsigned char Nibble)
{
  Nibble |= RS;  // Overwrite RS Value To LSB OF Data -> LSB (RS)? 1 (Data) : 0 (Command)
  IO_Expander_Write (Nibble | 0x04);
  IO_Expander_Write (Nibble & 0xFB);
  __delay_us(50);
}
 
void LCD_CMD (unsigned char CMD)
{
  RS = 0;    // Command Register Select
  LCD_Write_4Bit (CMD & 0xF0);  
  LCD_Write_4Bit ((CMD << 4) & 0xF0);
}
 
void LCD_Write_Char (char Data)
{
  RS = 1;    // Data Register Select
  LCD_Write_4Bit (Data & 0xF0);
  LCD_Write_4Bit ((Data << 4) & 0xF0);
}
 
void LCD_Write_String(char* Str)
{
  for(int i=0; Str[i] != '\0'; i++)
    LCD_Write_Char(Str[i]);
}
 
void LCD_Set_Cursor(unsigned char ROW, unsigned char COL)
{
  switch(ROW)
  {
    case 2:
      LCD_CMD(0xC0 + COL-1);
      break;
    case 3:
      LCD_CMD(0x94 + COL-1);
      break;
    case 4:
      LCD_CMD(0xD4 + COL-1);
      break;
    // Case 1
    default:
      LCD_CMD(0x80 + COL-1);
  }
}
 
void LCD_SL()
{
  LCD_CMD(0x18);
  __delay_us(40);
}
 
void LCD_SR()
{
  LCD_CMD(0x1C);
  __delay_us(40);
}
 
void LCD_Clear()
{
  LCD_CMD(0x01);
  __delay_us(40);
}

