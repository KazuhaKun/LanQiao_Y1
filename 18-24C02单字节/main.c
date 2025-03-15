// // #include <reg52.h>
#include <STC15F2K60S2.h>
#include "iic.h"
//Delay
void Delay(unsigned int t){
    while(t--);
}
//System Init
void Set_HC573(unsigned char channel,unsigned char dat){
  P2 = (P2 & 0x1f) | (channel << 5);
  P0 = dat;
}
unsigned char LED_State = 0xff;
//SMG
unsigned char SMGCode[] =
{
0xc0, //0
0xf9, //1
0xa4, //2
0xb0, //3
0x99, //4
0x92, //5
0x82, //6
0xf8, //7
0x80, //8
0x90, //9
0x88, //A
0x83, //b
0xc6, //C
0xa1, //d
0x86, //E
0x8e, //F
0xBF, //-
0xff //None
};
void SMG(unsigned char Location,unsigned char num){
    Set_HC573(6,(0x01 << Location));
    Set_HC573(7, SMGCode[num]);
    Delay(500);
	Set_HC573(7,0xff);
}
void SMGDot(unsigned char Location,unsigned char num){
    Set_HC573(6,(0x01 << Location));
    Set_HC573(7, SMGCode[num]-0x80);
    Delay(500);
	Set_HC573(7,0xff);
}
unsigned char num=0;

void Write_24C02_Byte(unsigned char addr, unsigned char dat){
	I2CStart();		//起始信号
	I2CSendByte(0xa0);		//设备&写地址
	I2CWaitAck();		//等待从机应答
	I2CSendByte(addr);		//内存地址
	I2CWaitAck();		//等待从机应答
	I2CSendByte(dat);		//写入数据
	I2CWaitAck();		//等待从机应答
	I2CStop();		//停止信号
}

unsigned char Read_24C02_Byte(unsigned char addr){
	unsigned char tmp;
	//伪写操作
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	//读取
	I2CStart();
	I2CSendByte(0xa1);
	I2CWaitAck();
	tmp = I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
	return tmp;
}

void Read_Save_OpenData(){
	num = Read_24C02_Byte(0x00);
	num++;
	if(num > 99) num = 1;
	Write_24C02_Byte(0x00,num);
}

void DisplayNum(){
	SMG(6,num/10);
	SMG(7,num%10);
}
void InitSystem(){
	Set_HC573(0,0x00);
	Set_HC573(5,0x00);	//Disable Buzzer
	Set_HC573(4,0xff);	//Disable LED
	Set_HC573(6,0xff);
	Set_HC573(7,0xff);	//Disable SMG
}

void main()
{
    InitSystem();
	Read_Save_OpenData();
    Delay(100);
    while(1)
    {
		DisplayNum();
    }
}
