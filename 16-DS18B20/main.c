// // #include <reg52.h>
#include <STC15F2K60S2.h>
// #include "onewire.h"

// //Delay
// void DelaySMG(unsigned int t){
//     while(t--);
// }

// void SMGDisplay();
// void Delay(unsigned int t){
//     while(t--) SMGDisplay();
// }

// unsigned char SMGCode[] =
// {
// 0xc0, //0
// 0xf9, //1
// 0xa4, //2
// 0xb0, //3
// 0x99, //4
// 0x92, //5
// 0x82, //6
// 0xf8, //7
// 0x80, //8
// 0x90, //9
// 0x88, //A
// 0x83, //b
// 0xc6, //C
// 0xa1, //d
// 0x86, //E
// 0x8e, //F
// 0xBF, //-
// 0xff //None
// };

// //System Init
// void InitHC138(unsigned char channel){
//     P2 = (P2 & 0x1f) | (channel << 5);
// }
// void OutPutP0(unsigned char channel,unsigned char dat){
//     InitHC138(channel);
//     P0 = dat;
// }
// void Disable_Buzzer(){
//     OutPutP0(5,0x00);
// }
// unsigned char LED_State = 0xff;
// void Disable_LED(){
//     OutPutP0(4,0xff);
// }
// void InitSystem(){
//     Disable_Buzzer();
//     Disable_LED();
// }

// void SMG(unsigned char Location,unsigned char num){
//     OutPutP0(6,(0x01 << Location));
//     // Delay(1000);
//     OutPutP0(7, SMGCode[num]);
//     DelaySMG(1000);
// }
// void SMGDot(unsigned char Location,unsigned char num){
//     OutPutP0(6,(0x01 << Location));
//     // Delay(1000);
//     OutPutP0(7, SMGCode[num]-0x80);
//     DelaySMG(1000);
// }
// int tem;

// void SMGDisplay(){
//     if(tem>=0){
//         SMG(4,17);
//         SMG(5,tem/100);
//         SMGDot(6,tem%100/10);
//         SMG(7,tem%10);
//     }
//     if(tem<0){
//         SMG(4,16);
//         SMG(5,-tem/100);
//         SMGDot(6,-tem%100/10);
//         SMG(7,-tem%10);
//     }
// }

// void Read_DS18B20_Temp(){
//     unsigned char LSB,MSB;
    
//     init_ds18b20();
//     Write_DS18B20(0xcc);
//     Write_DS18B20(0x44);
    
//     Delay(1000);

//     init_ds18b20();
//     Write_DS18B20(0xcc);
//     Write_DS18B20(0xbe);
//     LSB = Read_DS18B20();
//     MSB = Read_DS18B20();

//     tem = MSB;
//     tem = (tem << 8) | LSB;

//     if((tem & 0xf800) == 0x0000)
//     {
//         tem >>= 4;
//         tem = tem * 10;
//         tem = tem + (LSB & 0x0f) * 0.0625;
//     }
// }

// void main()
// {
//     InitSystem();
//     DelaySMG(100);
//     while(1)
//     {
//         Read_DS18B20_Temp();
//         SMGDisplay();
//     }
// }

/*==================蓝桥杯单片机特训==================
【进阶06】DS18B20的温度采样与显示
**硬件平台：CT107D单片机综合实训平台
**开发环境：Keil 4
**模    式：IO模式
**设    计：欧浩源（小蜜蜂老师，ohy3686@qq.com）
**时    间：2024-03-17
**更多详见：www.yuque.com/ouhaoyuan
====================================================*/
// #include "reg52.h"
#include "onewire.h"

//定义动态显示中单个数码管点亮时长
#define TSMG  500
//-------共阳数码管的段码编码表（无小数点）--------
unsigned char code SMG_NoDot[18]={0xc0,0xf9,
    0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
    0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
//-------共阳数码管的段码编码表（带小数点）--------
//0.～9.
unsigned char code SMG_Dot[10]={0x40,0x79,
    0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
//----------------------------------------------

float temp = 0;					//温度数据变量
int num = 0;					//数码管显示变量
void Display_Num();             //声明数码管显示函数              

//==============数码管动态显示专用延时函数=============
void DelaySMG(unsigned int t)
{
  while(t--);
}
//==================设置选通的锁存器===================
void Set_HC573(unsigned char channel, unsigned char dat)
{
  P0 = dat;                      //待设置数据
  switch(channel)                //选通锁存器
  {
    case 4:
      P2 = (P2 & 0x1f) | 0x80;  //Y4输出0，LED控制
    break;
    case 5:
      P2 = (P2 & 0x1f) | 0xa0;  //Y5输出0，蜂鸣器和继电器控制
    break;
    case 6:
      P2 = (P2 & 0x1f) | 0xc0;  //Y6输出0，数码管位选
    break;
    case 7:
      P2 = (P2 & 0x1f) | 0xe0;  //Y7输出0，数码管段码
    break;
    case 0:
      P2 = (P2 & 0x1f) | 0x00;  //所有锁存器不选择
    break;
  }
  P2 = (P2 & 0x1f) | 0x00;      //设置完成，关闭全部锁存器
}
//=================单个数码管动态显示=================
void DisplaySMG_Bit(unsigned char pos, unsigned char value)
{
  Set_HC573(6, 0x01 << pos);    //数码管的段位
  Set_HC573(7, value);          //数码管显示内容
  DelaySMG(TSMG);             	//数码管点亮时长
  Set_HC573(6, 0x01 << pos);        
  Set_HC573(7, 0xff);          	//消隐
}
//=================全部数码管静态显示=================
void DisplaySMG_All(unsigned char value)
{
  Set_HC573(6, 0xff);    		//使能所有数码管
  Set_HC573(7, value);  		//数码管显示内容  
}
//==================计数值显示函数====================
void Display_Num()
{
  //数码管右起第3位
  if(num > 999)
  {
	DisplaySMG_Bit(4,SMG_NoDot[num  / 1000]);
  }
  //数码管右起第2位
  DisplaySMG_Bit(5,SMG_Dot[(num / 100) % 10]);   
  //数码管右起第1位
  DisplaySMG_Bit(6,SMG_NoDot[(num / 10) % 10]);
  //数码管右起第0位
  DisplaySMG_Bit(7,SMG_NoDot[num % 10]); 	
}

//==============DS18B20温度读出与显示==============
void Begin_18B20()
{
  char i;
  init_ds18b20();            	//初始化DS18B20
  Write_DS18B20(0xcc);        	//忽略ROM操作
  Write_DS18B20(0x44);        	//启动温度转换
  //等待第一次温度数据采集完成，不会显示默认值85.00
  for(i = 0; i < 8; i++)			
  {
	DelaySMG(60000);
  }
}

void Read_Dsiplay_DS18B20()
{
  unsigned char LSB,MSB;      
  unsigned int dat = 0;      
  
  init_ds18b20();            	//初始化DS18B20
  Display_Num();
  Write_DS18B20(0xcc);        	//忽略ROM操作
  Write_DS18B20(0x44);        	//启动温度转换
  Display_Num();
  init_ds18b20();            	//初始化DS18B20
  Display_Num();
  Write_DS18B20(0xcc);        	//忽略ROM操作
  Write_DS18B20(0xbe);        	//读出内部存储器
	
  LSB = Read_DS18B20();      	//第0字节：温度低8位
  MSB = Read_DS18B20();      	//第1字节：温度高8位
  Display_Num();
  //上述程序中插入多处数码管刷新，可使显示亮度充足
  dat = MSB;                  
  dat = (dat << 8) | LSB;		//合并为16位温度原始数据
	//判断符号，处理正温度
  if((dat & 0xf800) == 0x0000)    	
  {
    temp = dat * 0.0625;  		//计算实际温度值
  }
	num = temp * 100;			//将2位小数部分变为整数
  Display_Num();          		//动态刷新数码管
}

//=====================系统初始化=====================
void Init_sys()
{
  Set_HC573(0, 0x00);        	//关闭所有锁存器
  Set_HC573(5, 0x00);        	//关闭蜂鸣器和继电器
  Set_HC573(4, 0xff);     		//点亮L4灯其余关闭
  DisplaySMG_All(0xff);      	//关闭全部数码管
  Begin_18B20();
}
//=======================主函数=======================
void main()
{
  Init_sys();                	//系统初始化
  while(1)
  {
    Read_Dsiplay_DS18B20();   //采样温度数据
    Display_Num();          	//数码管刷新
  }
}
//========================END========================