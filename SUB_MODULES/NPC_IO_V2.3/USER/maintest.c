#include "user.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�û���������	   
//��ʢ����
//�޸�����:2014/1/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ���˺�ʢ�������޹�˾
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

SYSWS WSYJ;//����Ӳ��
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
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 		//���ڳ�ʼ��Ϊ9600
 	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	USER_Init();			     //�û���ʼ��
	u8DGPosSMPP=1;
	u8DGPosSCMP=51;
	DGStepOne(u8DGPosSCMP,SCMP);
	DGStepOne(u8DGPosSMPP,SMPP);
	while(1)
	{
		//����speaker��led��ת������
//		GPIO_ResetBits(WSYJ.SpeakerPort,WSYJ.SpeakerIO);
//		GPIO_SetBits(WSYJ.TurnTablePort,WSYJ.TurnTableIO);
//	 	GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		delay_ms(500);
//		GPIO_ResetBits(WSYJ.TurnTablePort,WSYJ.TurnTableIO);
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		delay_ms(500);
		//������������
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
		
//		//�������Ų���
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
		//��λ���ײ���
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
		//���Ե���������
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
		//����������
//		cgq=(bit)ReadIO(WSYJ.CGQPort[0],WSYJ.CGQIO[0]);
//		if(cgq==T_1)
//		{
//			GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		}
//		else
//		{
//			GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);	
//		}
		//��ť����
		//read_button();
//		button = (bit)ReadIO(WSYJ.ButtonPort,WSYJ.ButtonIO);
//		while(T_1 == button)
//		{
//			GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//			//read_button();
//			button = (bit)ReadIO(WSYJ.ButtonPort,WSYJ.ButtonIO);
//		}
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
		//��׳�ʼ������
		//stp=10;
//		read_button();
//		while(T_1 == button)
//		{
//			DG_IO_ON(SMPP.DG_DRIVE_Port,SMPP.DG_DRIVE_IO);
//			busy = (bit)ReadIO(SMPP.DG_BUSY_Port,SMPP.DG_BUSY_IO);			
//			if(T_1 == busy)//busy�ź�Ϊ�͵�ƽ�����н���
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
		//���ѭ�����в���	  ������ť���£���0λ�ÿ�ʼ�˶�������˶���49��λ�ã��ٷ���0λ��
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
//			if(T_1 == busy)//busy�ź�Ϊ�ߵ�ƽ�����н���
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
		//��Ʒ���ѭ�����в���	  ������ť���£���0λ�ÿ�ʼ�˶�������˶���49��λ�ã��ٷ���0λ��
//		u8DGPos = 0;
//		DGRunSCMP(u8DGPos);
//		read_button();
//		HorizontalCylinderOut(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);
//		while(T_1 == button)
//		{
//			//DG_IO_ON(SMPP.DG_DRIVE_Port,SMPP.DG_DRIVE_IO);
//			GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//			HorizontalCylinderIn(SMPH.HorizontalCylinderPort,SMPH.HorizontalCylinderIO);//��ʾ
//			busy = (bit)ReadIO(SCMP.DG_BUSY_Port,SCMP.DG_BUSY_IO);			
//			if(T_1 == busy)//busy�ź�Ϊ�ߵ�ƽ�����н���
//			{
//				//��ʾ
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
//			VerticalCylinderUp(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);//��ʾ
//			read_button();
//		}
//		GPIO_ResetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//		DG_IO_OFF(SCMP.DG_DRIVE_Port,SCMP.DG_DRIVE_IO);
		//��Ʒ��׵����˶�����
//		u8DGPos = 1;
//		DGRunSCMP(u8DGPos);
//		read_button();
//		while(T_1 == button)
//		{
//			GPIO_SetBits(WSYJ.LEDPort,WSYJ.LEDIO);
//			busy = (bit)ReadIO(SCMP.DG_BUSY_Port,SCMP.DG_BUSY_IO);			
//			if(T_1 == busy)//busy�ź�Ϊ�ߵ�ƽ�����н���
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
		//��ȡ������λ�ò���
//		u8DGPos = ReadDGPos(SMPP);
		//���ŷ�ת����
//		IO_Turn(SMPP.QGPort[0],SMPP.QGIO[0]);//�����л�
		
		//��ϵͳ ����ˮƽ����
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
		//��ϵͳ ���Դ�ֱ����
//		VerticalCylinderDown(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);
//		delay_ms(500);
//		VerticalCylinderUp(SUPH.VerticalCylinderPort,SUPH.VerticalCylinderIO);
//		VerticalCylinderDown(SDWH.VerticalCylinderPort,SDWH.VerticalCylinderIO);
//		delay_ms(500);
//		VerticalCylinderUp(SDWH.VerticalCylinderPort,SDWH.VerticalCylinderIO);
//		delay_ms(500);
//		//��ϵͳ ����ˮƽ����
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
		//ԭ�ϵ�׶���
		read_button();
		
		while(T_1 == button)
		{
//			u8DGPos = ReadDGPos(SMPP);//��ȡԭ�ϵ��λ��
//			u8DGPos = u8DGPos + 1;//���λ�ü�1
			DGStepOne(u8DGPos,SMPP);
			DGStepOne(u8DGPos,SCMP);
			delay_ms(1000);
			u8DGPos++;
			if((time%20) == 0)//��10s��ر�
			{
				GPIO_ResetBits(WSYJ.SpeakerPort,WSYJ.SpeakerIO);//�ط���������
			}
			read_button();
		}
		if((time%20) == 0)//��10s��ر�
		{
			GPIO_ResetBits(WSYJ.SpeakerPort,WSYJ.SpeakerIO);//�ط���������
		}
		//��Ʒ��׶���
//		read_button();
//		
//		while(T_1 == button)
//		{
//			u8DGPos = ReadDGPos(SCMP);//��ȡԭ�ϵ��λ��
//			u8DGPos = u8DGPos + 1;//���λ�ü�1
//			DGStepOne(u8DGPos,SCMP);
//			delay_ms(1000);
//			if(time%20 == 0)//��10s��ر�
//			{
//				GPIO_ResetBits(WSYJ.SpeakerPort,WSYJ.SpeakerIO);//�ط���������
//			}
//			read_button();
//		}

	}	 
}

