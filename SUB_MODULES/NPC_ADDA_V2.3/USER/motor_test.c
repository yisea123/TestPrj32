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
	bit tmp1,tmp2;
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 		//���ڳ�ʼ��Ϊ9600
 	TIM3_Int_Init(9,719);//10Khz�ļ���Ƶ�ʣ�������9Ϊ0.1ms  
	USER_Init();			     //�û���ʼ��
 	
	while(1)
	{
// 		if(wsyj.u16CntMZ<100)
// 		{
// 			MoZ(wsyj.MotorPort[1],wsyj.MotorIO[1]);//�����ת
// 		}
// 		else if(wsyj.u16CntMZ<200)
// 		{
// 			MoF(wsyj.MotorPort[1],wsyj.MotorIO[1]);//�����ת
// 		}
// 		else
// 		{
// 			wsyj.u16CntMZ=0;
// 		}
		tmp1 = (bit)ReadIO(SWRun.SWPort,SWRun.SWIO);
		if(tmp1==F_0)
		{
			tmp1 = (bit)ReadIO(wsyj.CGQPort[1],wsyj.CGQIO[1]);
			if(tmp1 == T_1)
			{
				MoF(wsyj.MotorPort[1],wsyj.MotorIO[1]);//�����ת
				if(wsyj.u16MoT>=2)
				{
					wsyj.u16MoT=0;
					IO_Turn(wsyj.MotorPort[0],wsyj.MotorIO[0]);//�������
					wsyj.u16CntMZ++;
				}
			}
			
		}
		tmp2 = (bit)ReadIO(SWStop.SWPort,SWStop.SWIO);
		if(tmp2==F_0)
		{
			MoZ(wsyj.MotorPort[1],wsyj.MotorIO[1]);//�����ת
			if(wsyj.u16MoT>=2)
			{
				wsyj.u16MoT=0;
				IO_Turn(wsyj.MotorPort[0],wsyj.MotorIO[0]);//�������
				wsyj.u16CntMZ++;
			}
		}
		
	}	 
}

