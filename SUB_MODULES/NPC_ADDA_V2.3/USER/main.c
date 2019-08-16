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
u32 u32TimeT100us;
u32 StartTime,StartTime2;
SWPORT SWReset[4];
WSYJ wsyj;//外设硬件
GW gw[4];//各工位
#include <stm32f10x.h>
u16 TestTimer[100]={0};
u16 TestTimer2[100]={0};
//此表为 LED 的字模, 共阴数码管 0-9  - 
//u8  Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 
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
	
 	TIM3_Int_Init(9,719);   //10Khz的计数频率，计数到9为0.1ms
	LED_Init();
 	Adc1_DMA_Init();
	Adc3_DMA_Init();
	Dac1_Init();
	Dac2_Init();
	DAC7512_Pin_Config();
	uart1_init(115200);	 		  //232串口初始化为115200，对应主机uart5
	//uart2_init(921600);	 		  //串口初始化为750000 921600
	uart2_init(921600);	     //485
	
	while(1)
	{  
		if(u16TimeT>=400)
		{
			 u16TimeT = 0;
			 LED1 = ~LED1;  
			 LED2 = ~LED2;
			 LED3 =1;       //故障指示灯;
		}

    AD_ReadInRegister();
		IN_DisPlay((is1<<15)|(is2<<14)|(is3<<13)|(is4<<12)|(is5<<11)|(is6<<10)|0x03ff);
		OUT_DisPlay((os1<<15)|(os2<<14)|(os3<<13)|(os4<<12)|(os5<<11)|(os6<<10)|0x03ff);
//		{
//			u8 txbuf[10]={0xaa};
//			USART2_Send(txbuf,10);
//		}
	  modbus();   //通讯解析

	}	 
		
}
 
