// // #include <reg52.h>
#include <STC15F2K60S2.h>
#include "onewire.h"

//Delay
void DelaySMG(unsigned int t){
    while(t--);
}

void SMGDisplay();
// void Delay(unsigned int t){
//     while(t--) SMGDisplay();
// }

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
    DelaySMG(500);
	Set_HC573(7,0xff);
}
void SMGDot(unsigned char Location,unsigned char num){
    Set_HC573(6,(0x01 << Location));
    Set_HC573(7, SMGCode[num]-0x80);
    DelaySMG(500);
	Set_HC573(7,0xff);
}

//18B20
int temp;
void Init_18B20(){
	char i;
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	for(i=0;i<8;i++){
		DelaySMG(60000);
	}
}
void Read_DS18B20_Temp(){
    unsigned char LSB,MSB;
    int dat;

    init_ds18b20();
	SMGDisplay();
    Write_DS18B20(0xcc);
    Write_DS18B20(0x44);

    SMGDisplay();

    init_ds18b20();
	SMGDisplay();
    Write_DS18B20(0xcc);
    Write_DS18B20(0xbe);
    LSB = Read_DS18B20();
    MSB = Read_DS18B20();

	SMGDisplay();

    dat = MSB;
    dat = (dat << 8) | LSB;

    if((dat & 0xf800) == 0x0000){
		temp = dat >> 4;
		temp = temp * 100;
		temp += ((dat & 0x0f)*100)/16;
	}
    else if((dat & 0xf800) == 0xf800){
		dat = ~dat + 1;
		temp = dat >> 4;
		temp = temp * 100;
		temp += ((dat & 0x0f)*100)/16;
		temp = - temp;
	}
	SMGDisplay();
}

void SMGDisplay(){
	int ads_temp;
	if(temp>=0){
		ads_temp = temp;
		if(ads_temp > 999) SMG(4,ads_temp/1000);
		SMGDot(5,ads_temp/100%10);
		SMG(6,ads_temp/10%10);
		SMG(7,ads_temp%10);
	}
	if(temp<0){
		ads_temp = -temp;
		SMG(3,16);
		if(ads_temp > 999) SMG(4,ads_temp/1000);
		SMGDot(5,ads_temp/100%10);
		SMG(6,ads_temp/10%10);
		SMG(7,ads_temp%10);
	}
}

void InitSystem(){
	Set_HC573(0,0x00);
	Set_HC573(5,0x00);	//Disable Buzzer
	Set_HC573(4,0xff);	//Disable LED
	Set_HC573(6,0xff);
	Set_HC573(7,0xff);	//Disable SMG
	Init_18B20();
}

void main()
{
    InitSystem();
    DelaySMG(100);
    while(1)
    {
        Read_DS18B20_Temp();
        SMGDisplay();
    }
}
