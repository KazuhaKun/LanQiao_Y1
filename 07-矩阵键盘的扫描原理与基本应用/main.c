// #include <reg52.h>
#include <STC15F2K60S2.h>

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;

sbit X1 = P3^0;
sbit X2 = P3^1;
sbit X3 = P3^2;
sbit X4 = P3^3;
sbit Y1 = P4^4;
sbit Y2 = P4^2;
sbit Y3 = P3^5;
sbit Y4 = P3^4;

unsigned char CurrentKeyNum = 0;

unsigned char NixieCode[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 
    0x77, 0x7C, 0x39, 0x5e, 0x79, 0x71,
    0x00, 0x40};

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

void DisplayKeyNum(unsigned char value)
{
    Nixie(1,value-1,1);
    Delay(500);
}



unsigned char MatrixKey()
{
    unsigned char KeyNum = 0;

    P3 |= 0x3F;
    P42 = 1;
    P44 = 1;

    X1 = 0;
    if(Y1 == 0) {while(Y1 == 0);KeyNum = 1;}
    if(Y2 == 0) {while(Y2 == 0);KeyNum = 2;}
    if(Y3 == 0) {while(Y3 == 0);KeyNum = 3;}
    if(Y4 == 0) {while(Y4 == 0);KeyNum = 4;}

    P3 |= 0x3F;
    P42 = 1;
    P44 = 1;

    X2 = 0;
    if(Y1 == 0) {while(Y1 == 0);KeyNum = 5;}
    if(Y2 == 0) {while(Y2 == 0);KeyNum = 6;}
    if(Y3 == 0) {while(Y3 == 0);KeyNum = 7;}
    if(Y4 == 0) {while(Y4 == 0);KeyNum = 8;}

    P3 |= 0x3F;
    P42 = 1;
    P44 = 1;

    X3 = 0;
    if(Y1 == 0) {while(Y1 == 0);KeyNum = 9;}
    if(Y2 == 0) {while(Y2 == 0);KeyNum = 10;}
    if(Y3 == 0) {while(Y3 == 0);KeyNum = 11;}
    if(Y4 == 0) {while(Y4 == 0);KeyNum = 12;}

    P3 |= 0x3F;
    P42 = 1;
    P44 = 1;

    X4 = 0;
    if(Y1 == 0) {while(Y1 == 0);KeyNum = 13;}
    if(Y2 == 0) {while(Y2 == 0);KeyNum = 14;}
    if(Y3 == 0) {while(Y3 == 0);KeyNum = 15;}
    if(Y4 == 0) {while(Y4 == 0);KeyNum = 16;}

    return KeyNum;
}

void main()
{
    unsigned char keyValue;

    InitSystem();
    Delay(60000);
    OutPutP0(4,0xff);
    while(1)
    {        
        keyValue = MatrixKey();
        if(keyValue != 0)
        {
            CurrentKeyNum = keyValue;
        }
        DisplayKeyNum(CurrentKeyNum);
    }
}


//注意位运算的意义，如|才可以将某一位设为1，&才可以将某一位设为0