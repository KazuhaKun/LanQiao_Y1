#include <reg52.h>

// sbit HC138_A = P2^5;
// sbit HC138_B = P2^6;
// sbit HC138_C = P2^7;
// sbit N_BUZZ = P0^6;

int Number;

void Delay(unsigned int t)
{
    while(t--);
    while(t--);
}

void InitHC138(unsigned char Num)
{
    P2 = (P2 & 0x1f) | (Num << 5);
    // P2 = Num << 5;
    // HC138_A = 0x01 & Num;
    // HC138_B = 0x01 & (Num >> 1);
    // HC138_C = 0x01 & (Num >> 2);
    //原方案 switch case
}

void OutPutP0(unsigned char channel,unsigned char dat)
{
    InitHC138(channel);
    P0 = dat;
}

void LEDRunning()
{
    unsigned char i;

    for (i = 0; i < 3; i++)
    {
        OutPutP0(4,0x00);
        Delay(60000);
        Delay(60000);
        OutPutP0(4,0xff);
        Delay(60000);
        Delay(60000);
    }

    for (i = 1; i <= 8; i++)
    {
        OutPutP0(4,(0xff << i));
        Delay(60000);
    }

    OutPutP0(5,0x10);
    Delay(60000);
    Delay(60000);
    OutPutP0(5,0x00);

    InitHC138(4);

    for (i = 1; i <= 8; i++)
    {
        OutPutP0(4,~(0xff << i));
        Delay(60000);
    }

    // OutPutP0(5,0x40);
    OutPutP0(5,0x40);
    Delay(60000);
    Delay(60000);
    OutPutP0(5,0x00);
}

void InitSystem()
{
    OutPutP0(5,0x00);
}


void main()
{
    InitSystem();
    Delay(60000);
    while(1)
    {
        LEDRunning();
    }
}