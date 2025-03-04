// #include <reg52.h>
#include <STC15F2K60S2.h>

sbit S4 = P3^3;
sbit S5 = P3^2;

//System Init
void Delay(unsigned int t) {
    while(t--);
}
void InitHC138(unsigned char channel) {
    P2 = (P2 & 0x1f) | (channel << 5);
}
void OutPutP0(unsigned char channel,unsigned char dat){
    InitHC138(channel);
    P0 = dat;
}
void Disable_Buzzer(){
    OutPutP0(5,0x00);   // Disable Buzzer
}
//Nixie Display
unsigned char NixieCode[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 
    0x77, 0x7C, 0x39, 0x5e, 0x79, 0x71,
    0x00, 0x40};
void Nixie(unsigned char mode,unsigned char Location,unsigned char num)
{
    switch (mode)
    {
        case 1:
        {
            OutPutP0(6,0x01 << (Location - 1));
            OutPutP0(7,~NixieCode[num]);
            Delay(500);
            break;
        }
        case 2:
        {
            OutPutP0(6,Location);
            OutPutP0(7,~NixieCode[num]);
            break;
        }
    }
}
//Timer0
void InitTimer0()
{
    TMOD &= 0xf0;
    TMOD |= 0x01;

    //定时50ms
    TH0 = (65535 - 50000) / 256;
    TL0 = (65535 - 50000) % 256;

    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

unsigned char minSec,Sec,Min;
unsigned char State;

void Clear_Time()
{
    minSec = 0;
    Sec = 0;
    Min = 0;
}

void TimeDisplay()
{
    Nixie(1,1,Min/10);
    Nixie(1,2,Min%10);
    Nixie(1,3,17);
    Nixie(1,4,Sec/10);
    Nixie(1,5,Sec%10);
    Nixie(1,6,17);
    Nixie(1,7,minSec/10);
    Nixie(1,8,minSec%10);
}

void KeyDetect()
{
    if(S4 == 0)
    {
        Delay(100);
        if(S4 == 0) 
        {
            State = !State;
            while(S4 ==0) TimeDisplay();
        }
    }
    if(S5 == 0)
    {
        Delay(100);
        if(S5 == 0)
        {
            Clear_Time();
            while(S5 ==0) TimeDisplay();
        }
    }
}

void Time()
{
    if(State == 1)
    {
        if(minSec == 20){minSec=0;Sec++;}
        if(Sec == 60){Sec=0;Min++;}
        if(Min == 99){Min = 0;}
    }
}

void main()
{
    Disable_Buzzer();
    InitTimer0();
    Delay(60000);
    OutPutP0(4,0xff);
    State = 0;
    while(1)
    {
        TimeDisplay();
        KeyDetect();
    }
}

void ServiceTimer0() interrupt 1
{
    static unsigned char Count = 0;
    // static unsigned char Count1 = 0;
    TH0 = (65535 - 50000) / 256;
    TL0 = (65535 - 50000) % 256;
    Count++;
    if(State == 1 && Count) minSec++;
    // if(Count) KeyDetect();
    Time();
}