// // #include <reg52.h>
#include <STC15F2K60S2.h>
#include "iic.h"

//Delay
void Delay(unsigned int t){
    while(t--);
}
//System Init
void Set_HC573(unsigned char channel,unsigned char dat){
  P2 = (P2 & 0x1f) | (channel << 5);
  P0 = dat;
}
unsigned char LED_State = 0xff;
//SMG
unsigned char SMGCode[] =
{
0xc0, //0
0xf9, //1
0xa4, //2
0xb0, //3
0x99, //4
0x92, //5
0x82, //6
0xf8, //7
0x80, //8
0x90, //9
0x88, //A
0x83, //b
0xc6, //C
0xa1, //d
0x86, //E
0x8e, //F
0xBF, //-
0xff //None
};
void SMG(unsigned char Location,unsigned char num){
    Set_HC573(6,(0x01 << Location));
    Set_HC573(7, SMGCode[num]);
    Delay(500);
	Set_HC573(7,0xff);
}
void SMGDot(unsigned char Location,unsigned char num){
    Set_HC573(6,(0x01 << Location));
    Set_HC573(7, SMGCode[num]-0x80);
    Delay(500);
	Set_HC573(7,0xff);
}

unsigned char dat[8];

void PCF8591_DAC(unsigned char *dat,unsigned char len){
	unsigned char i;
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x44);		//0100 0100
	I2CWaitAck();
	for(i=0;i<len;i++){
		I2CSendByte(dat[i]);
		I2CWaitAck();
	}
	I2CStop();
}

void PCF8591_ADC_Init(unsigned char channel) {
    I2CStart();
    I2CSendByte(0x90);  // PCF8591地址 + 写位(0)
    I2CWaitAck();
    I2CSendByte(0x40 | channel);  // 控制字节：选择通道(0-3)，模拟输入模式
    I2CWaitAck();
    I2CStop();
    
    // 等待转换完成
    Delay(10);
}

unsigned char PCF8591_ReadADC(unsigned char channel) {
    unsigned char val;
    
    // 设置ADC通道
    PCF8591_ADC_Init(channel);
    
    // 读取ADC值
    I2CStart();
    I2CSendByte(0x91);  // PCF8591地址 + 读位(1)
    I2CWaitAck();
    
    // 丢弃第一个字节(上次转换结果)
    val = I2CReceiveByte();
    I2CSendAck(0);  // 继续读取
    
    // 读取有效值
    val = I2CReceiveByte();
    I2CSendAck(1);  // 发送NACK结束读取
    
    I2CStop();
    return val;
}

void PCF8591_ReadMultiChannel(unsigned char *dat, unsigned char channels) {
    unsigned char i;
    
    // 读取多个通道
    for(i = 0; i < channels && i < 4; i++) {
        dat[i] = PCF8591_ReadADC(i);
        Delay(5);  // 短暂延时确保转换完成
    }
}
void PCF8591_ADC(unsigned char *dat,unsigned char len){
	unsigned char i;
	I2CStart();
	I2CSendByte(0x91);
	I2CWaitAck();
	for(i=0;i<len-1;i++){
		dat[i]=I2CReceiveByte();
		I2CSendAck(0);
	}
	dat[len-1]=I2CReceiveByte();
	I2CSendAck(1);
	I2CStop();
}

void DisplayNum(){
	SMG(0,dat[0]/10);
	SMG(1,dat[0]%10);
	SMG(2,16);
	SMG(3,dat[1]/10);
	SMG(4,dat[1]%10);
	SMG(5,16);
	SMG(6,dat[2]/10);
	SMG(7,dat[2]%10);
}
void InitSystem(){
	Set_HC573(0,0x00);
	Set_HC573(5,0x00);	//Disable Buzzer
	Set_HC573(4,0xff);	//Disable LED
	Set_HC573(6,0xff);
	Set_HC573(7,0xff);	//Disable SMG
}

// void main()
// {
//     InitSystem();
// 	Delay(100);
//     while(1)
//     {
// 		PCF8591_ADC(dat,8);
// 		DisplayNum();
//     }
// }
void main() {
    unsigned char adc_values[4];
    
    InitSystem();
    Delay(100);
    
    while(1) {
        // 读取所有通道
		adc_values[0]=PCF8591_ReadADC(2);
        // PCF8591_ReadMultiChannel(adc_values, 0);
        
        // 显示光敏电阻值(AIN0)
        SMG(0, 10);  // 'A'
        SMG(1, 0);   // '0'
        SMG(2, 17);  // 空白
        SMG(3, adc_values[0]/100);      // 百位
        SMG(4, (adc_values[0]%100)/10); // 十位
        SMG(5, adc_values[0]%10);       // 个位
        // Delay(50000);
        
        // // 显示热敏电阻值(AIN1)
        // SMG(0, 10);  // 'A'
        // SMG(1, 1);   // '1'
        // SMG(2, 17);  // 空白
        // SMG(3, adc_values[1]/100);      // 百位
        // SMG(4, (adc_values[1]%100)/10); // 十位
        // SMG(5, adc_values[1]%10);       // 个位
        // Delay(50000);
        
        // // 显示电位器值(AIN2)
        // SMG(0, 10);  // 'A'
        // SMG(1, 2);   // '2'
        // SMG(2, 17);  // 空白
        // SMG(3, adc_values[2]/100);      // 百位
        // SMG(4, (adc_values[2]%100)/10); // 十位
        // SMG(5, adc_values[2]%10);       // 个位
        // Delay(50000);
    }
}