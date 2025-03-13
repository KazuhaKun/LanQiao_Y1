#ifndef __DS1302_H__
#define __DS1302_H__

#include <STC15F2K60S2.h>
#include <INTRINS.H>

sbit SDA = P2^3;		
sbit RST = P1^3; 
sbit SCK = P1^7;		

void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte ( unsigned char address );


#endif
