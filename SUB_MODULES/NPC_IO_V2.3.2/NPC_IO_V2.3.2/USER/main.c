#include "cdv_include.h"
//////////////////////////////////////////////////////////////////////////////////	 
//��ʢ����
//�޸�����:2014/1/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ���˺�ʢ�������޹�˾
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
WSYJ wsyj;//����Ӳ��
GW gw[4];//����λ

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
	delay_init();	    	     //��ʱ������ʼ��
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	TIM3_Int_Init(9,719);    //10Khz�ļ���Ƶ�ʣ�������9Ϊ0.1ms  
	NPC_GpioConfig();
	NPC_IOInit();
	LED_Init();
	uart1_init(750000);	 		  //���ڳ�ʼ��Ϊ 115200
	uart2_init(921600);//(460800);	 		  //���ڳ�ʼ��Ϊ 115200 750000 921600

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
		modbus();               //ͨѶ����
	
	}	 
		
}

