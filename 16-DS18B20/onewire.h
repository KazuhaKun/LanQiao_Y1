#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

#include <STC15F2K60S2.h>

sbit DQ = P1^4;

void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
bit init_ds18b20(void);


#endif
