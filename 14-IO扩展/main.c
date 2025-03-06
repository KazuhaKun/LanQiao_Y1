// #include <reg52.h>
#include <STC15F2K60S2.h>

//System Init
void Delay(unsigned int t) {
    while(t--);
    while(t--);
}
void InitHC138(unsigned char channel) {
    P2 = (P2 & 0x1f) | (channel << 5);
}
void OutPutP0(unsigned char channel,unsigned char dat){
    InitHC138(channel);
    P0 = dat;
}
void Disable_Buzzer(){
    OutPutP0(5,0x00);   // Disable Buzzer
}

void LEDRunning()
{
    InitHC138(4);
    P0 = 0xf0;
    Delay(60000);
    Delay(60000);
    P0 = 0x0f;
    Delay(60000);
    Delay(60000);
    P0 = 0xff;
    Delay(60000);
    Delay(60000);
}

void SMGRunning()
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
        InitHC138(6);
        P0 = 0x01 << i;
        InitHC138(7);
        P0 = 0x00;
        Delay(60000);
        Delay(60000);
    }
    P0 = 0xff;
    Delay(60000);
    Delay(60000);
}

void main()
{
    Disable_Buzzer();
    Delay(60000);
    OutPutP0(4,0xff);
    while(1)
    {
        LEDRunning();
        SMGRunning();
    }
}