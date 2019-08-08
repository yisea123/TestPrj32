#include "cdv_include.h"
//////////////////////////////////////////////////////////////////////////////////	 
//海盛电子
//修改日期:2014/1/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 嘉兴海盛电子有限公司
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

bit bRun;
bit btntmp;
u16 u16TimeT;
u16 u16TimeTmp;
bit bCount;
u8 u8Time;
u32 u16Count0;
u32 u16Count1;
u32 u16Count2;
u32 u16Count3;
LEDPORT Led[4];

SWPORT SWReset[4];
WSYJ wsyj;//外设硬件
GW gw[4];//各工位

u16 LedOut[8],D[16];
u32 u16Count;
u8 u8Cnt[4];
u8 u8CntTmp;
u8 display;
u32 Count0;
u32 Count1;
u32 Count2;
u32 Count3;
u32 data1;
u32 data2;
int main(void)
{	
	CMD_ARG arg;
	CDV_INT08U RX_buf[100];
//	CDV_INT08U Modbus_RX_buf[100];
	CDV_INT08U Rxlen;
	delay_init();	    	     //延时函数初始化
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	TIM3_Int_Init(9,719);    //10Khz的计数频率，计数到9为0.1ms  
	NPC_GpioConfig();
	NPC_IOInit();
	LED_Init();
	uart1_init(750000);	 		  //串口初始化为 115200
	uart2_init(921600);//(460800);921600	 		  //串口初始化为 115200 750000 921600

	while(1)
	{  
		if(u16TimeT>=2000)
		{
			u16TimeT = 0;
			LED3 = ~LED3;
      LED1 = ~LED1;
      LED2 = 0;			
		}
		
		IN_DisPlay(Read_Input_All());
		OUT_DisPlay(Read_Output_ALL());
		modbus();               //通讯解析
	
	}	 
		
}

