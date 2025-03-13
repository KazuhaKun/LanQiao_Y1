// // #include <reg52.h>
#include <STC15F2K60S2.h>
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

unsigned int count_f = 0;
unsigned int dat_f = 0;
unsigned char count_s = 0;

void Init_Timer(){
	TMOD = 0X16;
	TH0 = 0xff;
	TL0 = 0xff;

	TH1 = (65535-50000+1)/256;
	TL1 = (65535-50000+1)%256;
	ET0 = 1;
	ET1 = 1;
	EA = 1;

	TR0 = 1;
	TR1 = 1;
}

void Service_T0() interrupt 1{
	count_f++;
}

void Service_T1() interrupt 3{
	TH1 = (65535-50000+1)/256;
	TL1 = (65535-50000+1)%256;
	count_s++;
	if(count_s == 20){
		count_s = 0;
		dat_f = count_f;
		count_f = 0;
	}
}

void Display_NE555(){
	SMG(0,15);
	// SMG(3,dat_f/10000);
	// SMG(4,(dat_f%10000)/1000);
	// SMG(5,(dat_f%1000)/100);
	// SMG(6,(dat_f%100)/10);
	// SMG(7,dat_f%10);
	if(dat_f > 9999) SMG(3,dat_f/10000);
	if(dat_f > 999) SMG(4,(dat_f%10000)/1000);
	if(dat_f > 99) SMG(5,(dat_f%1000)/100);
	if(dat_f > 9) SMG(6,(dat_f%100)/10);
	SMG(7,dat_f%10);
}

void InitSystem(){
	Set_HC573(0,0x00);
	Set_HC573(5,0x00);	//Disable Buzzer
	Set_HC573(4,0xff);	//Disable LED
	Set_HC573(6,0xff);
	Set_HC573(7,0xff);	//Disable SMG
	Init_Timer();
}

void main()
{
    InitSystem();
    Delay(100);
    while(1)
    {
		Display_NE555();
    }
}
