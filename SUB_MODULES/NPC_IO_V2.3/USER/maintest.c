#include "user.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//用户驱动代码	   
//海盛电子
//修改日期:2014/1/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 嘉兴海盛电子有限公司
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

SYSWS WSYJ;//外设硬件
JXSDZ SMPH;
JXSDZ SUPH;
JXSDZ SDWH;
JXSDZ SCMH;
GWSTATUS SMPP;
GWSTATUS SCKP;
GWSTATUS SOPP;
GWSTATUS SCMP;
bit button;
bit btntmp;
u8 u8DGPos;
u8 u8DGPosSMPP;
u8 u8DGPosSCMP;
u8 time;
u8 SpeakerTime;

int main(void)
{	
//	u16 tt=0;
//	u8 stp;
//	stp=0;
	bit busy;
	delay_init();	    	 //延时函数初始化
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 		//串口初始化为9600
 	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
	USER_Init();			     //用户初始化
	u8DGPosSMPP=1;
	u8DGPosSCMP=51;
	DGStepOne(u8DGPosSCMP,SCMP);
	DGStepOne(u8DGPosSMPP,SMPP);
	while(1)
	{
		//测试speaker，led，转盘气缸
//		GPIO_ResetBits(WSYJ.SpeakerPort,WSYJ.SpeakerIO);
//		GPIO_SetBits(WSYJ.TurnTablePort,WSYJ.TurnTableIO);
//	 	GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		delay_ms(500);
//		GPIO_ResetBits(WSYJ.TurnTablePort,WSYJ.TurnTableIO);
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		delay_ms(500);
		//测试气阀引脚
//		GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		GPIO_SetBits(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);//
//		GPIO_SetBits(SUPH.HorizontalCylinderPort,SUPH.HorizontalCylinderIO);//
//		GPIO_SetBits(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);//
//		GPIO_SetBits(SDWH.HorizontalCylinderPort,SDWH.HorizontalCylinderIO);//
//		GPIO_SetBits(SDWH.VerticalCylinderPort,SDWH.VerticalCylinderIO);//
//		GPIO_SetBits(SCMH.HorizontalCylinderPort,SCMH.HorizontalCylinderIO);//
//		delay_ms(10);
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		GPIO_ResetBits(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);// 1
//		delay_ms(20);
//		GPIO_ResetBits(SUPH.HorizontalCylinderPort,SUPH.HorizontalCylinderIO);//  3
//		delay_ms(30);
//		GPIO_ResetBits(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);//	   6
//		delay_ms(40);
//		GPIO_ResetBits(SDWH.HorizontalCylinderPort,SDWH.HorizontalCylinderIO);//	10
//		delay_ms(50);
//		GPIO_ResetBits(SDWH.VerticalCylinderPort,SDWH.VerticalCylinderIO);//15
//		delay_ms(60);
//		GPIO_ResetBits(SCMH.HorizontalCylinderPort,SCMH.HorizontalCylinderIO);//21
//		delay_ms(70);
		
//		//吸盘引脚测试
//		GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		GPIO_SetBits(SMPH.SuckerPort,SMPH.SuckerIO);//
//		GPIO_SetBits(SUPH.SuckerPort,SUPH.SuckerIO);//
//		GPIO_SetBits(SDWH.SuckerPort,SDWH.SuckerIO);//
//		GPIO_SetBits(SCMH.SuckerPort,SCMH.SuckerIO);//
//		delay_ms(10);
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		GPIO_ResetBits(SMPH.SuckerPort,SMPH.SuckerIO);// 1
//		delay_ms(20);
//		GPIO_ResetBits(SUPH.SuckerPort,SUPH.SuckerIO);//  3
//		delay_ms(30);
//		GPIO_ResetBits(SDWH.SuckerPort,SDWH.SuckerIO);//	60
//		delay_ms(60);
//		GPIO_ResetBits(SCMH.SuckerPort,SCMH.SuckerIO);//120
//		delay_ms(70);
		//定位气缸测试
//		GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		GPIO_SetBits(SMPP.QGPort[0],SMPP.QGIO[0]);//
//		GPIO_SetBits(SCMP.QGPort[0],SCMP.QGIO[0]);//
//		GPIO_SetBits(SOPP.QGPort[0],SOPP.QGIO[0]);//
//		GPIO_SetBits(SOPP.QGPort[1],SOPP.QGIO[1]);//
//		delay_ms(10);
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		GPIO_ResetBits(SMPP.QGPort[0],SMPP.QGIO[0]);// 1
//		delay_ms(20);
//		GPIO_ResetBits(SCMP.QGPort[0],SCMP.QGIO[0]);//  3
//		delay_ms(30);
//		GPIO_ResetBits(SOPP.QGPort[0],SOPP.QGIO[0]);//	60
//		delay_ms(60);
//		GPIO_ResetBits(SOPP.QGPort[1],SOPP.QGIO[1]);//120
//		delay_ms(70);				
		//测试电缸输出引脚
//		GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		GPIO_SetBits(SCMP.DG_SVON_Port,SCMP.DG_SVON_IO);//
//		GPIO_SetBits(SCMP.DG_RESET_Port,SCMP.DG_RESET_IO);//
//		GPIO_SetBits(SCMP.DG_DRIVE_Port,SCMP.DG_DRIVE_IO);//
//		GPIO_SetBits(SCMP.DG_HOLD_Port,SCMP.DG_HOLD_IO);//
//		GPIO_SetBits(SCMP.DG_SETUP_Port,SCMP.DG_SETUP_IO);//
//		GPIO_SetBits(SCMP.DG_IN_Port[5],SCMP.DG_IN_IO[5]);//
//		GPIO_SetBits(SCMP.DG_IN_Port[4],SCMP.DG_IN_IO[4]);//
//		GPIO_SetBits(SCMP.DG_IN_Port[3],SCMP.DG_IN_IO[3]);//
//		GPIO_SetBits(SCMP.DG_IN_Port[2],SCMP.DG_IN_IO[2]);//
//		GPIO_SetBits(SCMP.DG_IN_Port[1],SCMP.DG_IN_IO[1]);//
//		GPIO_SetBits(SCMP.DG_IN_Port[0],SCMP.DG_IN_IO[0]);//
//
//		delay_ms(10);
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		GPIO_ResetBits(SCMP.DG_SVON_Port,SCMP.DG_SVON_IO);// 1
//		delay_ms(20);
//		GPIO_ResetBits(SCMP.DG_RESET_Port,SCMP.DG_RESET_IO);//  3
//		delay_ms(30);
//		GPIO_ResetBits(SCMP.DG_DRIVE_Port,SCMP.DG_DRIVE_IO);//	   6
//		delay_ms(40);
//		GPIO_ResetBits(SCMP.DG_HOLD_Port,SCMP.DG_HOLD_IO);//	10
//		delay_ms(50);
//		GPIO_ResetBits(SCMP.DG_SETUP_Port,SCMP.DG_SETUP_IO);//15
//		delay_ms(60);
//		GPIO_ResetBits(SCMP.DG_IN_Port[5],SCMP.DG_IN_IO[5]);//21
//		delay_ms(70);	
//		GPIO_ResetBits(SCMP.DG_IN_Port[4],SCMP.DG_IN_IO[4]);//28
//		delay_ms(80);
//		GPIO_ResetBits(SCMP.DG_IN_Port[3],SCMP.DG_IN_IO[3]);//360
//		delay_ms(90);
//		GPIO_ResetBits(SCMP.DG_IN_Port[2],SCMP.DG_IN_IO[2]);//450
//		delay_ms(100);
//		GPIO_ResetBits(SCMP.DG_IN_Port[1],SCMP.DG_IN_IO[1]);//550
//		delay_ms(110);
//		GPIO_ResetBits(SCMP.DG_IN_Port[0],SCMP.DG_IN_IO[0]);//670
//		delay_ms(120);
		//传感器测试
//		cgq=(bit)ReadIO(WSYJ.CGQPort[0],WSYJ.CGQIO[0]);
//		if(cgq==T_1)
//		{
//			GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		}
//		else
//		{
//			GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);	
//		}
		//按钮测试
		//read_button();
//		button = (bit)ReadIO(WSYJ.ButtonPort,WSYJ.ButtonIO);
//		while(T_1 == button)
//		{
//			GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//			//read_button();
//			button = (bit)ReadIO(WSYJ.ButtonPort,WSYJ.ButtonIO);
//		}
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
		//电缸初始化测试
		//stp=10;
//		read_button();
//		while(T_1 == button)
//		{
//			DG_IO_ON(SMPP.DG_DRIVE_Port,SMPP.DG_DRIVE_IO);
//			busy = (bit)ReadIO(SMPP.DG_BUSY_Port,SMPP.DG_BUSY_IO);			
//			if(T_1 == busy)//busy信号为低电平，运行结束
//			{
//				GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//				//DGRun(stp);
//				DG_IO_OFF(SMPP.DG_DRIVE_Port,SMPP.DG_DRIVE_IO);
//		
//			}
//			
//			read_button();
//		}
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
		//电缸循环运行测试	  启动按钮按下，从0位置开始运动，逐个运动到49号位置，再返回0位置
//		u8DGPos = 0;
//		DGRunSMPP(u8DGPos);
//		read_button();
//		HorizontalCylinderOut(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);
//		while(T_1 == button)
//		{
//			//DG_IO_ON(SMPP.DG_DRIVE_Port,SMPP.DG_DRIVE_IO);
//			GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//			HorizontalCylinderIn(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);
//			busy = (bit)ReadIO(SMPP.DG_BUSY_Port,SMPP.DG_BUSY_IO);			
//			if(T_1 == busy)//busy信号为高电平，运行结束
//			{
//				
//				VerticalCylinderDown(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);
//				if(u8DGPos>=50)
//				{
//					u8DGPos=0;
//				}
//				GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);					
//				//delay_ms(200);
//				DG_IO_ON(SMPP.DG_DRIVE_Port,SMPP.DG_DRIVE_IO);
//				delay_ms(100);
//				
//				GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//				DG_IO_OFF(SMPP.DG_DRIVE_Port,SMPP.DG_DRIVE_IO);
//				u8DGPos++;
//				DGRunSMPP(u8DGPos);
//				delay_ms(100);
//			}
//			VerticalCylinderUp(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);
//			read_button();
//		}
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		DG_IO_OFF(SMPP.DG_DRIVE_Port,SMPP.DG_DRIVE_IO);
		//成品电缸循环运行测试	  启动按钮按下，从0位置开始运动，逐个运动到49号位置，再返回0位置
//		u8DGPos = 0;
//		DGRunSCMP(u8DGPos);
//		read_button();
//		HorizontalCylinderOut(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);
//		while(T_1 == button)
//		{
//			//DG_IO_ON(SMPP.DG_DRIVE_Port,SMPP.DG_DRIVE_IO);
//			GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//			HorizontalCylinderIn(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);//提示
//			busy = (bit)ReadIO(SCMP.DG_BUSY_Port,SCMP.DG_BUSY_IO);			
//			if(T_1 == busy)//busy信号为高电平，运行结束
//			{
//				//提示
//				VerticalCylinderDown(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);
//				if(u8DGPos>=50)
//				{
//					u8DGPos=0;
//				}
//				GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);					
//				//delay_ms(200);
//				DG_IO_ON(SCMP.DG_DRIVE_Port,SCMP.DG_DRIVE_IO);
//				delay_ms(100);
//				
//				GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//				DG_IO_OFF(SCMP.DG_DRIVE_Port,SCMP.DG_DRIVE_IO);
//				u8DGPos++;
//				DGRunSCMP(u8DGPos);
//				delay_ms(100);
//			}
//			VerticalCylinderUp(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);//提示
//			read_button();
//		}
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		DG_IO_OFF(SCMP.DG_DRIVE_Port,SCMP.DG_DRIVE_IO);
		//成品电缸单步运动测试
//		u8DGPos = 1;
//		DGRunSCMP(u8DGPos);
//		read_button();
//		while(T_1 == button)
//		{
//			GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//			busy = (bit)ReadIO(SCMP.DG_BUSY_Port,SCMP.DG_BUSY_IO);			
//			if(T_1 == busy)//busy信号为高电平，运行结束
//			{
//				if(u8DGPos>=50)
//				{
//					u8DGPos=0;
//				}
//				GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);					
//				//delay_ms(200);
//				DG_IO_ON(SCMP.DG_DRIVE_Port,SCMP.DG_DRIVE_IO);
//				delay_ms(100);
//				u8DGPos++;
//				GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//				DG_IO_OFF(SCMP.DG_DRIVE_Port,SCMP.DG_DRIVE_IO);
//				DGRunSCMP(u8DGPos);
//				delay_ms(100);
//			}
//			read_button();
//		}
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		DG_IO_OFF(SCMP.DG_DRIVE_Port,SCMP.DG_DRIVE_IO);
		//读取电缸输出位置测试
//		u8DGPos = ReadDGPos(SMPP);
		//引脚翻转测试
//		IO_Turn(SMPP.QGPort[0],SMPP.QGIO[0]);//气缸切换
		
		//在系统 测试水平气缸
//		GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		HorizontalCylinderOut(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);
//		delay_ms(500);
//		HorizontalCylinderIn(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);
//		HorizontalCylinderOut(SUPH.HorizontalCylinderPort,SUPH.HorizontalCylinderIO);
//		delay_ms(500);
//		HorizontalCylinderIn(SUPH.HorizontalCylinderPort,SUPH.HorizontalCylinderIO);
//		HorizontalCylinderOut(SDWH.HorizontalCylinderPort,SDWH.HorizontalCylinderIO);
//		delay_ms(500);
//		HorizontalCylinderIn(SDWH.HorizontalCylinderPort,SDWH.HorizontalCylinderIO);
//		HorizontalCylinderOut(SCMH.HorizontalCylinderPort,SCMH.HorizontalCylinderIO);
//		delay_ms(500);
//		HorizontalCylinderIn(SCMH.HorizontalCylinderPort,SCMH.HorizontalCylinderIO);
		//在系统 测试垂直气缸
//		VerticalCylinderDown(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);
//		delay_ms(500);
//		VerticalCylinderUp(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);
//		VerticalCylinderDown(SDWH.VerticalCylinderPort,SDWH.VerticalCylinderIO);
//		delay_ms(500);
//		VerticalCylinderUp(SDWH.VerticalCylinderPort,SDWH.VerticalCylinderIO);
//		delay_ms(500);
//		//在系统 测试水平气缸
//		HorizontalCylinderOut(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);
//		delay_ms(500);
//		HorizontalCylinderIn(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);
//		HorizontalCylinderOut(SUPH.HorizontalCylinderPort,SUPH.HorizontalCylinderIO);
//		delay_ms(500);
//		HorizontalCylinderIn(SUPH.HorizontalCylinderPort,SUPH.HorizontalCylinderIO);
//		HorizontalCylinderOut(SDWH.HorizontalCylinderPort,SDWH.HorizontalCylinderIO);
//		delay_ms(500);
//		HorizontalCylinderIn(SDWH.HorizontalCylinderPort,SDWH.HorizontalCylinderIO);
//		HorizontalCylinderOut(SCMH.HorizontalCylinderPort,SCMH.HorizontalCylinderIO);
//		delay_ms(500);
		//原料电缸动作
		read_button();
		
		while(T_1 == button)
		{
//			u8DGPos = ReadDGPos(SMPP);//读取原料电缸位置
//			u8DGPos = u8DGPos + 1;//电缸位置减1
			DGStepOne(u8DGPos,SMPP);
			DGStepOne(u8DGPos,SCMP);
			delay_ms(1000);
			u8DGPos++;
			if((time%20) == 0)//响10s后关闭
			{
				GPIO_ResetBits(WSYJ.SpeakerPort,WSYJ.SpeakerIO);//关蜂鸣器报警
			}
			read_button();
		}
		if((time%20) == 0)//响10s后关闭
		{
			GPIO_ResetBits(WSYJ.SpeakerPort,WSYJ.SpeakerIO);//关蜂鸣器报警
		}
		//成品电缸动作
//		read_button();
//		
//		while(T_1 == button)
//		{
//			u8DGPos = ReadDGPos(SCMP);//读取原料电缸位置
//			u8DGPos = u8DGPos + 1;//电缸位置减1
//			DGStepOne(u8DGPos,SCMP);
//			delay_ms(1000);
//			if(time%20 == 0)//响10s后关闭
//			{
//				GPIO_ResetBits(WSYJ.SpeakerPort,WSYJ.SpeakerIO);//关蜂鸣器报警
//			}
//			read_button();
//		}

	}	 
}

