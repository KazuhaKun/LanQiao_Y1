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
    OutPutP0(5,0x00);
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

unsigned char KeyNum()
{
    unsigned char KeyNum = 0;
    if (S7 == 0){while(S7 == 0);KeyNum = 7;}
    if (S6 == 0){while(S6 == 0);KeyNum = 6;}
    if (S5 == 0){while(S5 == 0);KeyNum = 5;}
    if (S4 == 0){while(S4 == 0);KeyNum = 4;}

    return KeyNum;
}


void LED_Ctrl()
{
    unsigned char mode = 0;
    if(KeyNum() == 7 && mode == 0) mode = 1;
    if(KeyNum() == 6 && mode == 0) mode = 2;

    switch (mode)
    {
        case 0:
            LED_Show(1,0xff);
            break;
        case 1:
            LED_Show(2,1);
            if(KeyNum() == 5) L3 = !L3;
            
            if(KeyNum() == 4) L4 = !L4;
            // if(KeyNum() == 4) 
            // {
            //     L4 = 0;
            //     if(KeyNum() == 4)
            //     {
            //         L4 = 1;
            //     }
            // }
            if(KeyNum() == 7) mode = 0;
            break;
        case 2:
            LED_Show(2,2);
            if(KeyNum() == 5) L5 = !L5;
            if(KeyNum() == 4) L6 = !L6;
            if(KeyNum() == 6) mode = 0;
            break;
    }
}



void main()
{
    InitSystem();
    Delay(60000);
    LED_Show(1,0xff);
    while(1)
    {        
        LED_Ctrl();
    }
}