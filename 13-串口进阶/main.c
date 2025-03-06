// #include <reg52.h>
#include <STC15F2K60S2.h>

sbit L1 = P0^0;
sbit S7 = P3^0;

//System Init
void Delay(unsigned int t) {
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

unsigned char urdat;

void InitUART()
{
    TMOD = 0x20;
    TH1 = 0xfd;
    TL1 = 0xfd;
    TR1 = 1;

    SCON = 0x50;
    AUXR = 0x00;

    ES = 1;
    EA = 1;

}

void SendByte(unsigned char dat)
{
    SBUF = dat;
    while(TI == 0);
    TI = 1;
}

void ServiceUART() interrupt 4
{
    if(RI == 1)
    {
        RI = 0;     //人工清零
        urdat = SBUF;
        SendByte(urdat + 1);
    }
    // if(TI == 1)
    // {

    // }
}

void main()
{
    Disable_Buzzer();
    Delay(60000);
    OutPutP0(4,0xff);
    InitUART();
    SendByte(0x5a);
    SendByte(0xa5);
    while(1)
    {

    }
}