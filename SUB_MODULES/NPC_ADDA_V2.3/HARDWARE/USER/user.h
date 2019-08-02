#ifndef __USER_H
#define __USER_H	
//#include "stm32f10x_tim.h"
#include "cdv_include.h"
//////////////////////////////////////////////////////////////////////////////////	 
//��ʢ����
//�޸�����:2014/1/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ���˺�ʢ�������޹�˾
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
/*************��������ṹ********/
typedef enum 
{
	T_1 = 1,
	F_0 = 0
}bit;

#define CyOut(a,b)	GPIO_ResetBits(a,b)	  	//ˮƽ���׳�
#define CyIn(a,b)	GPIO_SetBits(a,b)		//ˮƽ������
#define DayON(a,b)	GPIO_SetBits(a,b)		//��ֱ������
#define DayOFF(a,b)	GPIO_ResetBits(a,b)		//��ֱ������
#define NightON(a,b)	GPIO_SetBits(a,b)		//��ֱ������
#define NightOFF(a,b)	GPIO_ResetBits(a,b)		//��ֱ������
#define LEDH(a,b)	GPIO_SetBits(a,b)		//�����-��
#define LEDL(a,b)	GPIO_ResetBits(a,b)		//�����-��
#define MoF(a,b)	GPIO_SetBits(a,b)		//�����ת
#define MoZ(a,b)	GPIO_ResetBits(a,b)		//�����ת
#define LedON(a,b)	GPIO_ResetBits(a,b)		//��������
#define LedOFF(a,b)	GPIO_SetBits(a,b)		//��������

#define bDay	T_1
#define bNight	F_0
#define IR_no	T_1
#define IR_yes	F_0
#define OK_0	F_0
#define NO_1	T_1
#define MotorPulse	3000
#define MotorDelay	50000

#define IO_Turn(a,b)				GPIO_WriteBit(a, b,(BitAction)(1-(GPIO_ReadOutputDataBit(a, b)))) //���ŷ�ת

#define ReadIO(a,b)					GPIO_ReadInputDataBit(a,b)//��ȡ���ŵ�ƽ

/************�ṹ�����Ͷ���***************/
typedef struct
{		
	GPIO_TypeDef * LedPort[8];   //led��ѡ�˿�
	u16 LedIO[8];                //led��ѡ�˿ں�
	GPIO_TypeDef * LedDuanPort[8];   //led��ѡʹ�ܶ˿�
	u16 LedDuanIO[8];                //led��ѡʹ�ܶ˿ں�
	GPIO_TypeDef * LedBitPort[8];   //ledλѡʹ�ܶ˿�
	u16 LedBitIO[8];                //ledλѡ�˿ں�
	
	
}LEDPORT;//����ܹ�λ

typedef struct
{		
	GPIO_TypeDef * SWPort;  //led�˿�
	u16 SWIO;     //�����˿ں�
	bit btntmp;
	u16 btn;
}SWPORT;//������λ


typedef struct
{		
	GPIO_TypeDef  * PulsePort;//����˿�
	u16 PulseIO;//����˿ں�
	GPIO_TypeDef * GetPort[2];//���ܹܶ˿�
	u16 GetIO[2];//���ܹܶ˿ں�
	GPIO_TypeDef  * LedPort[2];//�����ƶ˿�  0��� 1�̵�
	u16 LedIO[2];//�����ƶ˿ں�
	u8 u16Check[20];
	u8 u8Num;
	u16 u16ChkNum;
	u16 u16LedCnt;
}GW;//��λ

typedef struct
{		
	GPIO_TypeDef * RLedPort;//cpuָʾ�ƶ˿�
	u16 RLedIO;//cpuָʾ�ƶ˿ں�
	GPIO_TypeDef * GLedPort;//cpuָʾ�ƶ˿�
	u16 GLedIO;//cpuָʾ�ƶ˿ں�
	GPIO_TypeDef * QGPort[3];//���׶˿�
	u16 QGIO[3];//���׶˿ں�
	GPIO_TypeDef * MotorPort[2];//����˿�
	u16 MotorIO[2];//����˿ں�
	GPIO_TypeDef * CGQPort[3];//�������˿�
	u16 CGQIO[3];//�������˿ں�
	
	u16 u16CheckEn;//���ʹ�ܼ���ֵ
	bit bCheckD;
	u8 u8CheckD;
	bit bCheckN;
	u8 u8CheckN;
	
	bit bMoEn;
	bit bMoTmp;
	bit bMoZ;//�����תʹ��
	bit bMoZS;//�����תͣ��
	bit bMoFR;//�����ת����ʹ��
	bit bMoSS1;//�����ת����ˮƽλ��
	bit bMoF;//�����תʹ��
	bit bMoFS;//�����תͣ��	
	bit bMoZR;//�����ת����ʹ��
	bit bMoSS2;//�����ת����ˮƽλ��
	u16 u16MoT;//���������
	u16 u16CntMZ;//�����ת�������
	u16 u16CntMS;//���ͣ����ʱ����
	u16 u16CntMF;//�����ת�������
	
	u16 bc;
}WSYJ;//����

/********��������**********/
extern bit bRun;//���ʹ�ܱ��
extern bit bRunMotor;//�������ʹ�ܱ��
extern bit btntmp;
extern u16 u16TimeT;
extern u16 u16TimeTmp;
extern u8 u8Time;
extern u16 LedOut[8];
extern u8 u8Cnt[4];//������ͣ
extern u8 u8CntTmp;
extern u32 Count0;
extern u32 Count1;
extern u32 Count2;
extern u32 Count3;
extern u32 u16Count;
extern u32 u16Count0;
extern u32 u16Count1;
extern u32 u16Count2;
extern u32 u16Count3;
extern bit bCount;
extern u8 display;
extern u16 u16UartTime;
extern bit bUart;
extern u32 u32TimeT100us;
extern u16 TestTimer[100],TestTimer2[100];
extern u32 StartTime,StartTime2;
extern LEDPORT Led[4];//�����

extern SWPORT SWReset[4];//�������ð�ť
extern WSYJ wsyj;//����Ӳ��
extern GW gw[4];//����λ
 
void USER_Init(void);//��ʼ��
void read_button(void);//��ȡ��ť����
void LED_Display( LEDPORT pled, u8 LedOut);//����ܶ�ѡ
void LED_show0(u32 Count);//�������ʾ����
void LED_show1(u32 Count);//�������ʾ����
void LED_show2(u32 Count);//�������ʾ����
void LED_show3(u32 Count);//�������ʾ����
void UPRun(void);//���ϼ�⺯��
void DownRun(void);//���¼�⺯��
void CheckIR(void);//�����⺯��
u8 Run(bit DayOrNight,u8 num);//��ҹ��⺯��
void MotorRun(void);//�����ת����
void EndShow(void);//������ʾ����
void UartSend(void);//���ڷ�������
// u8 LLEDOUT[8];
#endif
