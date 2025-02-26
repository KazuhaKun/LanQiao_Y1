#include <reg52.h>

void Delay(unsigned int t)
{
    while(t--);
    while(t--);
}

void InitHC138(unsigned char Num)
{
    P2 = (P2 & 0x1f) | (Num << 5);
}

void OutPutP0(unsigned char channel,unsigned char dat)
{
    InitHC138(channel);
    P0 = dat;
}

void InitSystem()
{
    OutPutP0(5,0x00);
}

unsigned char NixieCode[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,0x77,0x7C,0x39,0x5e,0x79,0x71,0x00,0x40};

void Nixie(unsigned char Num,unsigned char Location)
{
    if (Location < 7)
    {
        OutPutP0(6,0x01 << ((Location)%8));
        OutPutP0(7,~NixieCode[Num]);
    }
    else
    {
        OutPutP0(6,Location);
        OutPutP0(7,~NixieCode[Num]);
    }
}

void Nixie_Advance(unsigned char Mode,unsigned char Num,unsigned char Location)
{
    switch (Mode)
    {
        case 1:
        {
            OutPutP0(6,0x01 << ((Location-1)%8));
            OutPutP0(7,~NixieCode[Num]);
            break;
        }
        case 2:
        {
            OutPutP0(6,Location);
            OutPutP0(7,~NixieCode[Num]);
            break;
        }
    }
}

void Nixie_Static()
{
    unsigned char i,j;    
    for (j = 1; j < 9; j++)
    {
        for (i = 0; i < 10; i++)
        {
            Nixie_Advance(1,i,j);
            Delay(120000);
        }
    }
    for (i = 0; i < 16; i++)
    {
        Nixie_Advance(2,i,0xff);
        Delay(480000);
    }
}


void main()
{
    InitSystem();
    Delay(60000);
    while(1)
    {
        Nixie_Static();
    }
}