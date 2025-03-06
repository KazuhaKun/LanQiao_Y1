// #include <reg52.h>
#include <STC15F2K60S2.h>
#include<ABSACC.H>

void Delay(unsigned int t) {
    while(t--);
    while(t--);
}

void LEDRunning()
{
    XBYTE[0x8000] = 0xf0;
    Delay(60000);
    Delay(60000);
    XBYTE[0x8000] = 0x0f;
    Delay(60000);
    Delay(60000);
    XBYTE[0x8000] = 0xff;
    Delay(60000);
    Delay(60000);
}

void SMGRunning()
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        XBYTE[0xc000] = 0x01 << i;
        XBYTE[0xe000] = 0x00;
        Delay(60000);
        Delay(60000);
    }
    XBYTE[0xe000] = 0xff;
    Delay(60000);
    Delay(60000);
}

void main()
{
    while(1)
    {
        LEDRunning();
        SMGRunning();
    }
}