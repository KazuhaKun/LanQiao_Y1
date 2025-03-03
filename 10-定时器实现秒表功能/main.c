// #include <reg52.h>
#include <STC15F2K60S2.h>
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
    case 1:
        OutPutP0(6,0x01 << (Location - 1));
        OutPutP0(7,~NixieCode[num]);
        break;
    case 2:
        OutPutP0(6,Location);
        OutPutP0(7,~NixieCode[num]);
        break;
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

void Time()
{
    if(minSec == 20){minSec=0;Sec++;}
    if(Sec == 60){Sec=0;Min++;}
    if(Min == 100){Min = 0;}
}

void TimeDisplay()
{
    Nixie(1,1,Min/10);
    Nixie(1,2,Min%10);
    Nixie()
    Nixie(1,4,Sec/10);
    Nixie(1,5,Sec%10);

    Nixie(1,7,minSec/10);
    Nixie(1,8,minSec%10);
}
void main()
{
    Disable_Buzzer();
    InitTimer0();
    Delay(60000);
    OutPutP0(4,0xff);
    while(1)
    {

    }
}

void ServiceTimer0() interrupt 1
{
    static unsigned char Count = 0;
    // static unsigned char Count1 = 0;
    TH0 = (65535 - 50000) / 256;
    TL0 = (65535 - 50000) % 256;
    Count++;
    if(Count) minSec++;
}