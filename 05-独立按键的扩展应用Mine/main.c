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


unsigned char mode;

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

// void LED_Show(unsigned char mode,unsigned char dat)
// {
//     unsigned char Num;
//     switch (mode)
//     {
//         case 1:
//             OutPutP0(4,dat);
//             break;
        
//         case 2:
//             if (dat < 9)
//             {
//                 Num = ~(0x01 << (dat-1));
//                 OutPutP0(4,Num);
//             }
//             break;
//     }
// }

void LED_Ctrl()
{

    if(S7 == 0) 
    {
        Delay(100);
        if(S7 == 0)
        {
            if(mode == 0) {L1 = 0;mode = 1;}
            else if(mode == 1) {L1 = 1;mode = 0;}
        }
        
    }
    if(S6 == 0) 
    {
        Delay(100);
        if(S6 == 0)
        {
            if(mode == 0) {L2 = 0;mode = 2;}
            else if(mode == 2) {L2 = 1;mode = 0;}
        }
        
    }
    if(S5 == 0)
    {
        Delay(100);
        if(S5 == 0)
        {
            if(mode == 1) {L3 = 0;while(S5 == 0);L3 = 1;}
            else if(mode == 2) {L5 = 0;while(S5 == 0);L5 = 1;}
        }
    }
    if(S4 == 0)
    {
        Delay(100);
        if(S4 == 0)
        {
            if(mode == 1) {L4 = 0;while(S4 == 0);L4 = 1;}
            else if(mode == 2) {L6 = 0;while(S4 == 0);L6 = 1;}
        }
    }
}



void main()
{
    InitSystem();
    Delay(60000);
    OutPutP0(4,0xff);
    while(1)
    {        
        LED_Ctrl();
    }
}