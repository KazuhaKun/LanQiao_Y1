// #include <reg52.h>
#include <STC15F2K60S2.h>

sbit L1 = P0^0;
sbit L8 = P0^7;

void Delay(unsigned int t)
{
    while(t--);
    // while(t--);
}

void InitHC138(unsigned char channel)
{
    P2 = (P2 & 0x1f) | (channel << 5);
}

void OutPutP0(unsigned char channel,unsigned char dat)
{
    InitHC138(channel);
    P0 = dat;
}

void InitSystem()
{
    OutPutP0(5,0x00);   // Disable Buzzer
}

void Working()
{
    L1 = 0;
    Delay(60000);
    L1 = 1;
    Delay(60000);
}

void Init_INT0()
{
    IT0 = 1;    //下降沿触发
    EX0 = 1;    //INT0中断使能
    EA = 1;     //总使能
}

unsigned char State_INT0;

void ServiceINT0() interrupt 0
{
    // L8 = 0;
    // Delay(60000);
    // Delay(60000);
    // Delay(60000);
    // Delay(60000);
    // Delay(60000);
    // L8 = 1;
    State_INT0 = 1;
}

void LED_INT()
{
    if(State_INT0)
    {
        L8 = 0;
        Delay(60000);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        L8 = 1;
    }
    State_INT0 = 0;
}

void main()
{
    InitSystem();
    Init_INT0();
    Delay(60000);
    OutPutP0(4,0xff);
    while(1)
    {
        Working();
        LED_INT();
    }
}
