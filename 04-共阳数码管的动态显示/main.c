#include <reg52.h>

unsigned char Month;

void Delay(unsigned int t)
{
    while(t--);
    // while(t--);
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

unsigned char NixieCode[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 
    0x77, 0x7C, 0x39, 0x5e, 0x79, 0x71,
    0x00, 0x40};

void Nixie(unsigned char Mode,unsigned char Num,unsigned char Location)
{
    switch (Mode)
    {
        case 1:
        {
            OutPutP0(6,0x01 << ((Location-1)%8));
            OutPutP0(7,~NixieCode[Num]);
            // OutPutP0(7,0xff);
            break;
        }
        case 2:
        {
            OutPutP0(6,Location);
            OutPutP0(7,~NixieCode[Num]);
            // OutPutP0(7,0xff);
            break;
        }
    }
}

void Nixie_Dynamic()
{
    Nixie(1,2,1);
    Delay(500);
    Nixie(1,0,2);
    Delay(500);
    Nixie(1,1,3);
    Delay(500);
    Nixie(1,8,4);
    Delay(500);

    Nixie(1,17,5);
    Delay(500);
    Nixie(1,17,6);
    Delay(500);

    Nixie(1,Month/10,7);
    Delay(500);
    Nixie(1,Month%10,8);
    Delay(500); 
}



void main()
{
    InitSystem();
    Month = 12;
    Delay(60000);
    while(1)
    {
        unsigned char i;
        for (i = 0; i < 100; i++)
        {
            Nixie_Dynamic();
        }
        
        Month++;
        if (Month > 12) Month = 1;
    }
}