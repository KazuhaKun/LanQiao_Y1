#include <reg52.h>

sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit N_BUZZ = P0^6;

int Number;

void Delay(unsigned int t)
{
    while(t--);
    while(t--);
}

void LEDRunning()
{
    unsigned char i;
    HC138_C = 1;
    HC138_B = 0;
    HC138_A = 0;

    for (i = 0; i < 3; i++)
    {
        P0 = 0x00;
        Delay(60000);
        Delay(60000);
        P0 = 0xff;
        Delay(60000);
        Delay(60000);
    }

    for (i = 1; i <= 8; i++)
    {
        P0 = 0xff << i;
        Delay(60000);
        Delay(60000);
    }
    for (i = 1; i <= 8; i++)
    {
        P0 = ~(0xff << i);
        Delay(60000);
        Delay(60000);
    }
    
}


void main()
{
    P2 = 0xA0;
    N_BUZZ = 0;
    Delay(60000);
    while(1)
    {
        LEDRunning();
    }
}