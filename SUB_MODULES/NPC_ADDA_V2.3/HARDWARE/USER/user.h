#ifndef __USER_H
#define __USER_H	
//#include "stm32f10x_tim.h"
#include "cdv_include.h"
//////////////////////////////////////////////////////////////////////////////////	 
//海盛电子
//修改日期:2014/1/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 嘉兴海盛电子有限公司
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
/*************定义变量结构********/
typedef enum 
{
	T_1 = 1,
	F_0 = 0
}bit;

#define CyOut(a,b)	GPIO_ResetBits(a,b)	  	//水平气缸出
#define CyIn(a,b)	GPIO_SetBits(a,b)		//水平气缸收
#define DayON(a,b)	GPIO_SetBits(a,b)		//垂直气缸上
#define DayOFF(a,b)	GPIO_ResetBits(a,b)		//垂直气缸下
#define NightON(a,b)	GPIO_SetBits(a,b)		//垂直气缸上
#define NightOFF(a,b)	GPIO_ResetBits(a,b)		//垂直气缸下
#define LEDH(a,b)	GPIO_SetBits(a,b)		//数码管-高
#define LEDL(a,b)	GPIO_ResetBits(a,b)		//数码管-低
#define MoF(a,b)	GPIO_SetBits(a,b)		//电机正转
#define MoZ(a,b)	GPIO_ResetBits(a,b)		//电机反转
#define LedON(a,b)	GPIO_ResetBits(a,b)		//报警灯亮
#define LedOFF(a,b)	GPIO_SetBits(a,b)		//报警灯灭

#define bDay	T_1
#define bNight	F_0
#define IR_no	T_1
#define IR_yes	F_0
#define OK_0	F_0
#define NO_1	T_1
#define MotorPulse	3000
#define MotorDelay	50000

#define IO_Turn(a,b)				GPIO_WriteBit(a, b,(BitAction)(1-(GPIO_ReadOutputDataBit(a, b)))) //引脚翻转

#define ReadIO(a,b)					GPIO_ReadInputDataBit(a,b)//读取引脚电平

/************结构体类型定义***************/
typedef struct
{		
	GPIO_TypeDef * LedPort[8];   //led段选端口
	u16 LedIO[8];                //led段选端口号
	GPIO_TypeDef * LedDuanPort[8];   //led段选使能端口
	u16 LedDuanIO[8];                //led段选使能端口号
	GPIO_TypeDef * LedBitPort[8];   //led位选使能端口
	u16 LedBitIO[8];                //led位选端口号
	
	
}LEDPORT;//数码管工位

typedef struct
{		
	GPIO_TypeDef * SWPort;  //led端口
	u16 SWIO;     //按键端口号
	bit btntmp;
	u16 btn;
}SWPORT;//按键工位


typedef struct
{		
	GPIO_TypeDef  * PulsePort;//脉冲端口
	u16 PulseIO;//脉冲端口号
	GPIO_TypeDef * GetPort[2];//接受管端口
	u16 GetIO[2];//接受管端口号
	GPIO_TypeDef  * LedPort[2];//报警灯端口  0红灯 1绿灯
	u16 LedIO[2];//报警灯端口号
	u8 u16Check[20];
	u8 u8Num;
	u16 u16ChkNum;
	u16 u16LedCnt;
}GW;//工位

typedef struct
{		
	GPIO_TypeDef * RLedPort;//cpu指示灯端口
	u16 RLedIO;//cpu指示灯端口号
	GPIO_TypeDef * GLedPort;//cpu指示灯端口
	u16 GLedIO;//cpu指示灯端口号
	GPIO_TypeDef * QGPort[3];//气缸端口
	u16 QGIO[3];//气缸端口号
	GPIO_TypeDef * MotorPort[2];//电机端口
	u16 MotorIO[2];//电机端口号
	GPIO_TypeDef * CGQPort[3];//传感器端口
	u16 CGQIO[3];//传感器端口号
	
	u16 u16CheckEn;//检测使能计数值
	bit bCheckD;
	u8 u8CheckD;
	bit bCheckN;
	u8 u8CheckN;
	
	bit bMoEn;
	bit bMoTmp;
	bit bMoZ;//电机正转使能
	bit bMoZS;//电机正转停顿
	bit bMoFR;//电机反转返回使能
	bit bMoSS1;//电机反转返回水平位置
	bit bMoF;//电机反转使能
	bit bMoFS;//电机反转停顿	
	bit bMoZR;//电机正转返回使能
	bit bMoSS2;//电机反转返回水平位置
	u16 u16MoT;//电机脉冲间隔
	u16 u16CntMZ;//电机正转脉冲计数
	u16 u16CntMS;//电机停顿延时计数
	u16 u16CntMF;//电机反转脉冲计数
	
	u16 bc;
}WSYJ;//外设

/********变量定义**********/
extern bit bRun;//检测使能标记
extern bit bRunMotor;//电机运行使能标记
extern bit btntmp;
extern u16 u16TimeT;
extern u16 u16TimeTmp;
extern u8 u8Time;
extern u16 LedOut[8];
extern u8 u8Cnt[4];//用于暂停
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
extern LEDPORT Led[4];//数码管

extern SWPORT SWReset[4];//计数重置按钮
extern WSYJ wsyj;//外设硬件
extern GW gw[4];//各工位
 
void USER_Init(void);//初始化
void read_button(void);//读取按钮动作
void LED_Display( LEDPORT pled, u8 LedOut);//数码管段选
void LED_show0(u32 Count);//数码管显示函数
void LED_show1(u32 Count);//数码管显示函数
void LED_show2(u32 Count);//数码管显示函数
void LED_show3(u32 Count);//数码管显示函数
void UPRun(void);//向上检测函数
void DownRun(void);//向下检测函数
void CheckIR(void);//红外检测函数
u8 Run(bit DayOrNight,u8 num);//日夜检测函数
void MotorRun(void);//电机运转函数
void EndShow(void);//结束显示函数
void UartSend(void);//串口发送数据
// u8 LLEDOUT[8];
#endif
