#ifndef BMP180_H
#define BMP180_H

void Read_Calibration_data (void);
int Get_UT_Value (void);
float Get_Temp_Value (void);
int Get_UP_Value (short oss);
float Get_Press_Value (short oss);
float Get_Altitude_Value (short oss);

#endif 

