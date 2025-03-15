// // #include <reg52.h>
#include <STC15F2K60S2.h>
#include "iic.h"

sbit S4 = P3^3;

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
unsigned char dat[8];

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

void Write_24C02_Page(unsigned char addr, unsigned char *dat, unsigned char len){
	unsigned char i;
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	for(i=0;i<len;i++){
		I2CSendByte(dat[i]);
		I2CWaitAck();
	}
	I2CStop();
}

void Read_24C02_Page(unsigned char addr, unsigned char *dat, unsigned char len){
	unsigned char i;
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();

	I2CStart();
	I2CSendByte(0xa1);
	I2CWaitAck();
	for(i=0;i<len-1;i++){
		dat[i] = I2CReceiveByte();
		I2CSendAck(0);
	}
	dat[len-1] = I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
}

void DisplayNum();

void S4_Function(){
	dat[0]+=1;
	dat[1]+=2;
	dat[2]+=5;
	if(dat[0]>=100) dat[0]=0;
	if(dat[1]>=100) dat[1]=0;
	if(dat[2]>=100) dat[2]=0;
	Write_24C02_Page(0x01,dat,3);
	DisplayNum();
}

void S4Scan(){
	if(S4 == 0){
		Delay(500);
		if(S4 == 0){
			S4_Function();
			while(S4 == 0) DisplayNum();
		}
	}
}

void DisplayNum(){
	SMG(0,dat[0]/10);
	SMG(1,dat[0]%10);
	SMG(2,16);
	SMG(3,dat[1]/10);
	SMG(4,dat[1]%10);
	SMG(5,16);
	SMG(6,dat[2]/10);
	SMG(7,dat[2]%10);
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
	Read_24C02_Page(0x01,dat,3);
	Delay(100);
    while(1)
    {
		S4Scan();
		DisplayNum();
    }
}
