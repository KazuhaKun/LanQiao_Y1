// // #include <reg52.h>
#include <STC15F2K60S2.h>
#include "ds1302.h"

code unsigned char WRITE_RTC_ADDR[7] = {0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
code unsigned char READ_RTC_ADDR[7] = {0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};
//20-4-18 sat. 23:59:24
unsigned char Timer[] = {0x24,0x59,0x23,0x18,0x04,0x06,0x20};

void DS1302_Config(){
	char i;
	Write_Ds1302_Byte(0x8e,0x00);	//Disable WP
	for(i=0;i<7;i++){
		Write_Ds1302_Byte(WRITE_RTC_ADDR[i],Timer[i]);
	}
	Write_Ds1302_Byte(0x8e,0x80);	//Enable WP
}
void Read_DS1302_Timer(){
	char i;
	for(i=0;i<7;i++){
		Timer[i]=Read_Ds1302_Byte(READ_RTC_ADDR[i]);
	}
}

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

void KeyDetect(){

}

void Display_DS1302(){
	SMG(0,Timer[2]/16);
	SMG(1,Timer[2]%16);
	SMG(2,16);
	SMG(3,Timer[1]/16);
	SMG(4,Timer[1]%16);
	SMG(5,16);
	SMG(6,Timer[0]/16);
	SMG(7,Timer[0]%16);
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
    Delay(100);
	DS1302_Config();
    while(1)
    {
		Read_DS1302_Timer();
		Display_DS1302();
    }
}
