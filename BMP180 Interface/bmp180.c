#include <xc.h>
#include <stdint.h>
#include <math.h>
#include "i2c_master.h"
#include "bmp180.h"

short AC1 = 0;
short AC2 = 0;
short AC3 = 0;
unsigned short AC4 = 0;
unsigned short AC5 = 0;
unsigned short AC6 = 0;
short B1 = 0;
short B2 = 0;
short MB = 0;
short MC = 0;
short MD = 0;

unsigned int UT = 0;
short oss = 0;
unsigned int UP = 0;
unsigned int X1 = 0;
unsigned int X2 = 0;
unsigned int X3 = 0;
unsigned int B3 = 0;
unsigned int B5 = 0;
unsigned int B4 = 0;
unsigned int B6 = 0;
unsigned int B7 = 0;

unsigned int Press = 0;
unsigned int AltPress = 0;
unsigned int Temp = 0;

void Read_Calibration_data (void)
{
    short calibrations [22] = {0};
    
    I2C_Start ();
    I2C_Write (0xAA);
    I2C_Stop ();
    
    AC1 = ( ( calibrations [0] << 8 )  | calibrations [1] );
    AC2 = ( ( calibrations [2] << 8 )  | calibrations [3] );
    AC3 = ( ( calibrations [4] << 8 )  | calibrations [5] );
    AC4 = ( ( calibrations [6] << 8 )  | calibrations [7] );
    AC5 = ( ( calibrations [8] << 8 )  | calibrations [9] );
    AC6 = ( ( calibrations [10] << 8 ) | calibrations [11] );
    B1  = ( ( calibrations [12] << 8 ) | calibrations [13] );
    B2  = ( ( calibrations [14] << 8 ) | calibrations [15] );
    MB  = ( ( calibrations [16] << 8 ) | calibrations [17] );
    MC  = ( ( calibrations [18] << 8 ) | calibrations [19] );
    MD  = ( ( calibrations [20] << 8 ) | calibrations [21] );
}

int Get_UT_Value (void)
{
    I2C_Start ();
    I2C_Write (0xAA);
    I2C_Write (0x2E);
    I2C_Write (0xF4);
    I2C_Stop ();
    __delay_ms(5);
    
    short Read_UT [2];
   return ( (Read_UT[0] << 8) | Read_UT[1]); 
}

float Get_Temp_Value (void)
{
	UT = Get_UT_Value();
	X1 = ((UT - AC6) * ( AC5 / (pow(2,15))));
	X2 = ((MC * (pow(2,11))) / (X1 + MD));
	B5 = X1 + X2;
	Temp = (B5 + 8)/(pow(2,4));
	return Temp / 10.0;
}

int Get_UP_Value (short oss)
{
    I2C_Start ();
    I2C_Write (0xAA);
    I2C_Write (0x34 + (oss << 6));
    I2C_Stop ();
    
    switch (oss)
    {
	   case (1):
			__delay_ms(8);
			break;
	   case (2):
			__delay_ms(14);
			break;
	   case (3):
			__delay_ms(26);
			break;
        // case 0       
        default:
               __delay_ms(5);
    }
    
    short Read_UP [3];
    return (( (Read_UP[0] << 16) + (Read_UP[1] << 8) + (Read_UP[2]) ) >> (8 - oss));   
}

float Get_Press_Value (short oss)
{
	UP = Get_UP_Value (oss);
	X1 = ((UT-AC6) * (AC5 / (pow(2,15))));
	X2 = ((MC * (pow(2,11))) / (X1+MD));
	B5 = X1 + X2;
	B6 = B5 - 4000;
	X1 = (B2 * (B6 * B6/(pow(2,12))))/(pow(2,11));
	X2 = AC2 * B6/(pow(2,11));
	X3 = X1 + X2;
	B3 = (((AC1* 4 + X3) << oss) + 2) / 4;
	X1 = AC3 * B6 / pow(2,13);
	X2 = (B1 * (B6 * B6 / (pow(2,12)))) / (pow(2,16));
	X3 = ((X1+X2)+2)/pow(2,2);
	B4 = AC4 * (unsigned long)(X3+32768) / (pow(2,15));
	B7 = ((unsigned long)UP - B3) * (50000 >> oss);
	if (B7 < 0x80000000) Press = (B7 * 2) / B4;
	else Press = (B7 / B4) * 2;
	X1 = (Press / (pow(2,8))) * (Press / (pow(2,8)));
	X1 = (X1 * 3038)/(pow(2,16));
	X2 = (-7357 * Press) / (pow(2,16));
	Press = Press + (X1 + X2 + 3791) / (pow(2,4));

	return Press;
}

#define  ATM_PRESS      101325   // in Pascal
// Calculating pressure at sea level
float Get_Altitude_Value (short oss)
{
	AltPress = Get_Press_Value (oss);
	return 44330 * (1 - (pow(((float)AltPress / (float)ATM_PRESS), 0.19029495718)));
}

