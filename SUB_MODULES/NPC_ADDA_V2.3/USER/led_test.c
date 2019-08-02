#include "user.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
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
u16 u16Time;
u16 u16TimeTmp;
bit bCount;
u8 u8Time;

LEDPORT Led;
WSYJ wsyj;
GW gw[5];
SWPORT SWRun;
SWPORT SWStop;
SWPORT SWSet;
SWPORT SWAdd;
SWPORT SWReset;

//�˱�Ϊ LED ����ģ, ��������� 0-9  - 
//u8  Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 
u16 LedOut[8],D[16];
u16 u16Count;
u8 u8Cnt[4];
u8 u8CntTmp;
u8 display;

int main(void)
{	
	bit tmp;
	u8 i;
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 		//���ڳ�ʼ��Ϊ9600
 	TIM3_Int_Init(9,719);//10Khz�ļ���Ƶ�ʣ�������9Ϊ0.1ms  
	USER_Init();			     //�û���ʼ��
 	
	for(i=0;i<5;i++)
	{
		LedON(gw[i].LedPort[0],gw[i].LedIO[0]);
		LedON(gw[i].LedPort[1],gw[i].LedIO[1]);
	}
	delay_ms(500);
	for(i=0;i<5;i++)
	{
		LedOFF(gw[i].LedPort[0],gw[i].LedIO[0]);
		LedOFF(gw[i].LedPort[1],gw[i].LedIO[1]);
	}
	delay_ms(500);
	for(i=0;i<5;i++)
	{
		LedON(gw[i].LedPort[0],gw[i].LedIO[0]);
		LedON(gw[i].LedPort[1],gw[i].LedIO[1]);
	}
	delay_ms(500);
	for(i=0;i<5;i++)
	{
		LedOFF(gw[i].LedPort[0],gw[i].LedIO[0]);
		LedOFF(gw[i].LedPort[1],gw[i].LedIO[1]);
	}
	delay_ms(500);
	while(1)
	{
		//����ָʾ��
// 		for(i=0;i<5;i++)
// 		{
// 			LedON(gw[i].LedPort[0],gw[i].LedIO[0]);
// 			LedOFF(gw[i].LedPort[1],gw[i].LedIO[1]);
// 			delay_ms(100);
// 		}
// 		for(i=0;i<5;i++)
// 		{
// 			LedOFF(gw[i].LedPort[0],gw[i].LedIO[0]);
// 			LedON(gw[i].LedPort[1],gw[i].LedIO[1]);
// 			delay_ms(100);
// 		}
		//���Խ��չ�
		
		for(i=0;i<5;i++)
		{
			tmp=(bit)ReadIO(gw[i].GetPort[0],gw[i].GetIO[0]);
			if(tmp==F_0)
			{
				LedON(gw[i].LedPort[0],gw[i].LedIO[0]);
			}
			else
			{
				LedOFF(gw[i].LedPort[0],gw[i].LedIO[0]);
			}
			
		}
		delay_ms(500);
		for(i=0;i<5;i++)
		{
			
			tmp=(bit)ReadIO(gw[i].GetPort[1],gw[i].GetIO[1]);
			if(tmp==F_0)
			{
				LedON(gw[i].LedPort[1],gw[i].LedIO[1]);
			}
			else
			{
				LedOFF(gw[i].LedPort[1],gw[i].LedIO[1]);
			}
		}
		delay_ms(500);
	}
}

