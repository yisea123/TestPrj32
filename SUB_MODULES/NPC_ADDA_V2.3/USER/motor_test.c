#include "user.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
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

//此表为 LED 的字模, 共阴数码管 0-9  - 
//u8  Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 
u16 LedOut[8],D[16];
u16 u16Count;
u8 u8Cnt[4];
u8 u8CntTmp;
u8 display;

int main(void)
{	
	bit tmp1,tmp2;
	delay_init();	    	 //延时函数初始化
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 		//串口初始化为9600
 	TIM3_Int_Init(9,719);//10Khz的计数频率，计数到9为0.1ms  
	USER_Init();			     //用户初始化
 	
	while(1)
	{
// 		if(wsyj.u16CntMZ<100)
// 		{
// 			MoZ(wsyj.MotorPort[1],wsyj.MotorIO[1]);//电机正转
// 		}
// 		else if(wsyj.u16CntMZ<200)
// 		{
// 			MoF(wsyj.MotorPort[1],wsyj.MotorIO[1]);//电机反转
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
				MoF(wsyj.MotorPort[1],wsyj.MotorIO[1]);//电机反转
				if(wsyj.u16MoT>=2)
				{
					wsyj.u16MoT=0;
					IO_Turn(wsyj.MotorPort[0],wsyj.MotorIO[0]);//电机脉冲
					wsyj.u16CntMZ++;
				}
			}
			
		}
		tmp2 = (bit)ReadIO(SWStop.SWPort,SWStop.SWIO);
		if(tmp2==F_0)
		{
			MoZ(wsyj.MotorPort[1],wsyj.MotorIO[1]);//电机正转
			if(wsyj.u16MoT>=2)
			{
				wsyj.u16MoT=0;
				IO_Turn(wsyj.MotorPort[0],wsyj.MotorIO[0]);//电机脉冲
				wsyj.u16CntMZ++;
			}
		}
		
	}	 
}

