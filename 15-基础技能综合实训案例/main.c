// #include <reg52.h>
#include <STC15F2K60S2.h>

sbit S5 = P3^2;
sbit S4 = P3^3;
sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L7 = P0^6;
sbit L8 = P0^7;
//Delay
void Delay(unsigned int t){
    while(t--);
}

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

//System Init
void InitHC138(unsigned char channel){
    P2 = (P2 & 0x1f) | (channel << 5);
}
void OutPutP0(unsigned char channel,unsigned char dat){
    InitHC138(channel);
    P0 = dat;
}
void Disable_Buzzer(){
    OutPutP0(5,0x00);
}
unsigned char LED_State = 0xff;
void Disable_LED(){
    OutPutP0(4,0xff);
}
void InitSystem(){
    Disable_Buzzer();
    Disable_LED();
}

//LED
void LED_Scan(){
    unsigned char i=0;
    InitHC138(4);
    for(i;i<8;i++){
        P0 = ~(0x01 << i);
        Delay(50000);
    }
    P0 = 0xff;
}
//SMG
void SMG_Scan(){
    unsigned char i=0;
    for(i;i<8;i++){
        OutPutP0(6,(0x01 << i));
        Delay(50000);
        OutPutP0(7,0x00);
        Delay(50000);
    }
    P0 = 0xff;
}
//LED&SMG Check
void LS_Check(){
    LED_Scan();
    SMG_Scan();
}

unsigned char Sec=0;
unsigned char Min=0;
unsigned char Hour=0;
//Timer0
void InitTimer0(){
    TMOD &= 0xf0;
    TMOD |= 0x01;

    //50ms
    TH0 = (65535-50000)/256;
    TL0 = (65535-50000)%256;

    ET0 = 1;
    EA = 1;
    TR0 = 1;
}
void TimeSet();
void ServiceTimer0() interrupt 1
{
    static unsigned char Count0;
    TH0 = (65535-50000)/256;
    TL0 = (65535-50000)%256;
    Count0++;
    if(Count0 >= 40) {Count0 = 0;Sec += 2;}
    TimeSet();

}
//Nixie CountDown
void TimeSet(){
    if(Sec >= 60){Sec = 0;Min++;}
    if(Min >= 60){Min = 0;Hour++;}
    if(Hour >= 99){Hour = 0;}
}
void SMG(unsigned char Location,unsigned char num){
    OutPutP0(6,(0x01 << Location));
    // Delay(1000);
    OutPutP0(7, SMGCode[num]);
    Delay(1000);
}
void SMGDisplay(){
    SMG(0,Hour/10);
    SMG(1,Hour%10);
    SMG(2,16);
    SMG(3,Min/10);
    SMG(4,Min%10);
    SMG(5,16);
    SMG(6,Sec/10);
    SMG(7,Sec%10);
}

//Local LED Control
void LocalLEDSet(){
    if(S5 == 0){
        Delay(100);
        while(S5 == 0) SMGDisplay();
        LED_State ^= 0x40;
        OutPutP0(4, LED_State);
    }
    if(S4 == 0){
        Delay(100);
        while(S4 == 0) SMGDisplay();
        LED_State ^= 0x80;
        OutPutP0(4, LED_State);
    }
}

//UART Control
void InitUART(){
    TMOD &= 0x0f;
    TMOD |= 0X20; //8位自动重装
    TH1 = 0xfd;
    TL1 = 0Xfd;
    TR1 = 1;

    SCON = 0x50;
    AUXR = 0x00;

    ES = 1;
    EA = 1;
}
unsigned char command = 0x00;
void ServiceUART() interrupt 4
{
    if(RI == 1){
        RI = 0;
        command = SBUF;
    }
}
void SendByte(unsigned char dat){
    SBUF = dat;
    while(TI == 0);
    TI = 0;
}
void UARTWorking(){
    unsigned char bcd_s,bcd_m,bcd_h;
    if(command != 0){
        switch(command & 0xf0){
            case 0xa0:
            LED_State = (LED_State & 0Xf0) | (~command & 0x0f);
            OutPutP0(4,LED_State);
            command = 0x00;
            break;
            case 0xb0:
            bcd_s = ((Sec/10)<<4) + (Sec%10);
            bcd_m = ((Min/10)<<4) + (Min%10);
            bcd_h = ((Hour/10)<<4) + (Hour%10);

            SendByte(bcd_h);
            SendByte(bcd_m);
            SendByte(bcd_s);
            command = 0x00;
            break;
        }
    }
}

void main()
{
    InitSystem();
    Delay(100);
    LS_Check();
    InitTimer0();
    InitUART();
    while(1)
    {
        SMGDisplay();
        LocalLEDSet();
        UARTWorking();
    }
}