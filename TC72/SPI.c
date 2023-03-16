#include <xc.h>
#include "SPI.h"

void SPI_Init (void)
{
    // Configurate GPIO Pins
    TRISC5 = 0; // SDO -> Output
    TRISC4 = 1; // SDI -> Input
    TRISC3 = 0; // SCK -> Output
    
    //SMP = 1: Input data sampled at end of data output time. 
    //CKE = 0: SPI Clock Edge Select bit CLEAR.
    SSPSTAT = 0x80; 
    
    //SSPEN = 1: Enable Serial Port. 
    //CKP = 1: Clock Polarity Select bit: Idle state for clock is a high level
    // No Overflow , and Clear Write collision
    SSPCON = 0x30; 
    
    // SPI Reception Interrupt if Needed  
    //GIE = 1;    PEIE = 1;   SSPIE = 1;
}


void SPI_write (unsigned char data)
{
   SSPBUF = data;
   while(!BF);
}

char SPI_read(char dummy_data)
{
   SSPBUF = dummy_data;   //Assign to clear
   while(!BF);

   return SSPBUF;
}
