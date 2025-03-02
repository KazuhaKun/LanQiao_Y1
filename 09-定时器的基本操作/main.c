// #include <reg52.h>
#include <STC15F2K60S2.h>

sbit L1 = P0^0;
sbit L8 = P0^7;

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

void InitSystem(){
    OutPutP0(5,0x00);   // Disable Buzzer
}
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

// unsigned char Count = 0;
// unsigned char Count1 = 0;

void ServiceTimer0() interrupt 1
{
    static unsigned char Count = 0;
    // static unsigned char Count1 = 0;
    TH0 = (65535 - 50000) / 256;
    TL0 = (65535 - 50000) % 256;
    Count++;
    // Count1++;
    // if(Count == 10) {L1 = ~L1;Count = 0;}
    // if(Count1 == 100) {L8 = ~L8;Count1 = 0;}
    if(!(Count%10)) {L1 = ~L1;}
    if(Count == 100) {L8 = ~L8;Count = 0;}
}

void main()
{
    InitSystem();
    InitTimer0();
    Delay(60000);
    OutPutP0(4,0xff);
    while(1)
    {

    }
}
