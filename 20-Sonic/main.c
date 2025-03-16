// // #include <reg52.h>
#include <STC15F2K60S2.h>
#include <INTRINS.H>

sbit Tx = P1^0;
sbit Rx = P1^1;


//Delay
void Delay(unsigned int t){
    while(t--);
}
void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
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

//UT
void Ut_Wave_Init(){
    unsigned char i;
    for(i=0;i<8;i++){
        Tx = 1;
        Delay12us();
        Tx = 0;
        Delay12us();
    }
}
unsigned char Ut_Wave_Data(){
    unsigned int time;
    TMOD &= 0X0f;
    TH1=TL1=0;
    Ut_Wave_Init();
    TR1 = 1;
    while((Rx == 1) && (TF1 == 0));
    TR1 = 0;
    if(TF1 == 0){
        time = TH1 << 8 | TL1;
        return (time*0.017); 
    }
    else {TF1=0;return 0;}
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
    unsigned char Wave_Data;
    InitSystem();
	Delay(10000);
    while(1)
    {
        Wave_Data = Ut_Wave_Data();
        SMG(0,Wave_Data/100%10);
        SMG(1,Wave_Data/10%10);
        SMG(2,Wave_Data%10);
    }
}
