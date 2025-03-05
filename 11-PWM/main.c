// #include <reg52.h>
#include <STC15F2K60S2.h>

sbit L1 = P0^0;
sbit S7 = P3^0;

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


unsigned char State;
unsigned char Rate;
unsigned char RateNum[]={0,25,50,100};
//Timer0
void InitTimer0()
{
    TMOD &= 0xf0;
    TMOD |= 0x01;

    //定时50ms
    TH0 = (65535 - 100) / 256;
    TL0 = (65535 - 100) % 256;

    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

void ServiceTimer0() interrupt 1
{
    static unsigned char Count = 0;
    // static unsigned char Count1 = 0;
    TH0 = (65535 - 100) / 256;
    TL0 = (65535 - 100) % 256;
    Count++;
    // if(Count == Rate) State=!State;
    // else if(Count == 100) {Count=0;State=!State;}
    // if(Count == Rate) L1 = 1;
    // else if(Count == 100) {L1 = 0;Count = 0;}
    if(Count <= Rate) L1 = 0;
    else L1 = 1;
    if(Count >= 100) Count = 0;
}

//Key
void KeyDetect()
{
    static unsigned char i;
    unsigned char stat;
    if(S7 == 0)
    {
        Delay(100);
        if(S7 == 0) 
        {
            i = (i + 1) % 4;
            Rate = RateNum[i];
            while(S7 == 0) Delay(10);
        }
        Delay(100);
    }
}

void main()
{
    Disable_Buzzer();
    InitTimer0();
    Delay(60000);
    OutPutP0(4,0xff);
    while(1)
    {
        KeyDetect();
    }
}