#ifndef __IIC_H__
#define __IIC_H__

#include <STC15F2K60S2.H>
#include "intrins.h"

sbit scl=P2^0;
sbit sda=P2^1;

void I2CStart(void);
void I2CStop(void);
void I2CSendByte(unsigned char byt);
unsigned char I2CReceiveByte(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(unsigned char ackbit);

#endif