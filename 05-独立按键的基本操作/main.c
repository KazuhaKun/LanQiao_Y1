#include <reg52.h>

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;

unsigned char mode = 0;

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

void LED_Show(unsigned char mode,unsigned char dat)
{
    unsigned char Num;
    switch (mode)
    {
        case 1:
            OutPutP0(4,dat);
            break;
        
        case 2:
            if (dat < 9)
            {
                Num = ~(0x01 << (dat-1));
                OutPutP0(4,Num);
            }
            break;
    }
}

void InitSystem()
{
    OutPutP0(5,0x00);
}



void ScanKeys_Alone()
{
    if (S7 == 0)
    {
        Delay(100);
        if (S7 == 0)
        {
            L1 = 0;
            while (S7 == 0);
            L1 = 1;
        }
    }
    if (S6 == 0)
    {
        Delay(100);
        if (S6 == 0)
        {
            L2 = 0;
            while (S6 == 0);
            L2 = 1;
        }
    }
    if (S5 == 0)
    {
        Delay(100);
        if (S5 == 0)
        {
            L3 = 0;
            while (S5 == 0);
            L3 = 1;
        }
    }
    if (S4 == 0)
    {
        Delay(100);
        if (S4 == 0)
        {
            L4 = 0;
            while (S4 == 0);
            L4 = 1;
        }
    }
    
}




void main()
{
    InitSystem();
    Delay(60000);
    LED_Show(1,0xff);
    InitHC138(4);
    while(1)
    {        
        ScanKeys_Alone();
    }
}