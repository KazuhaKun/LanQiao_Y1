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
void Disable_LED(){
    OutPutP0(4,0xff);
}
void InitSystem(){
    Disable_Buzzer();
    Disable_LED();
}


unsigned char urdat;

void SendByte(unsigned char dat);

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
unsigned char command = 0x00;

void ServiceUART() interrupt 4
{
    if(RI == 1)
    {
        RI = 0;     //人工清零
        command = SBUF;
    }
    // if(TI == 1)
    // {

    // }
}

void SendByte(unsigned char dat)
{
    SBUF = dat;
    while(TI == 0);
    TI = 0;
}

void SendString(unsigned char *str)
{
    while(*str != '\0'){
        SendByte(*str++);
    }
}

void Working()
{
    if(command != 0x00)
    {
        switch(command & 0xf0)
        {
            case 0xa0:
            P0 = (P0 & 0xf0) | ~(command & 0x0f);
            command = 0x00;
            break;

            case 0xb0:
            P0 = (P0 & 0x0F) | ~((command & 0x0f) << 4);
            command = 0x00;
            break;

            case 0xc0:
            SendString("The System is Running...\r\n");
            command = 0x00;
            break;
        }
    }
}

void main()
{
    InitSystem();
    Delay(60000);
    InitUART();
    SendString("Welcome to XMF System!\r\n");
    while(1)
    {
        Working();
    }
}