#ifndef __DAC_H
#define __DAC_H	
#include "cdv_include.h"  	     			    
typedef struct 
{
	CDV_INT32U Slope;  //斜率值
	CDV_INT32U Zero;   //零位值
	CDV_INT32U Out_Min; //输出最小值
	CDV_INT32U Out_Max; //输出最大值
	CDV_INT08U Dac_num; //输出通道
	CDV_INT32U Da_Coeff1;  //系数1
	CDV_INT32U Da_Coeff2;  //系数2
	CDV_INT08U Pid_Flag;   //PID标志
	CDV_INT32U TRA_SPEED_LIMIT; //变频器速度上限
//	CDV_INT32U Slope[2];//斜率值
//	CDV_INT32U Zero[2];//零位值
//	CDV_INT32U Out_Min[2];//输出最小值
//	CDV_INT32U Out_Max[2];//输出最大值
//	CDV_INT32U PID_proportion[2];//PID 比例参数
//	CDV_INT32U PID_Derivative[2];//PID 微分参数
//	CDV_INT32U PID_integral[2];//PID 积分参数
//	CDV_INT32U PID_Integral_Limit[2];//PID 积分上限
//	CDV_INT32U PID_Ctrl_Time[2];//PID控制间隔 单位ms 
	float vol;//当前输出电压
}DAC_STRUCT;

#define TRA_SPEED_LIMIT  5000
#define TRA1_SPEED_LIMIT 5000   //主变频器速度上限，扩大了100倍
#define TRA2_SPEED_LIMIT 5000   //辅变频器速度上限，扩大了100倍
#define Dac_no   4

//extern DAC_STRUCT dac_par[Dac_no];

void Dac1_Init(void);		//DAC通道1初始化	 	 
void Dac2_Init(void);
void Dac1_Set_Vol(u16 vol);	//设置通道1输出电压

void Dac2_Set_Vol(u16 vol);//DAC通道2初始化
void Dac2_Set_Vol(u16 vol);	//设置通道2输出电压
RET_STATUS DacCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CMD_ARG *arg);
u8 Read_Dac_Par(u8 *buf,u8 no,u32 pflag);//DAC参数读取
u16  Get_Adc_Voltge(u8 adc_num);
u16  Get_Da_value(u8 no);
//void  Dac_Set_Vol(DAC_STRUCT *dac_par,u8 no);
//void Dac_set_Voltge( CDV_INT08U dac_num, float vol);
void DAC_SETOUT(void);
#endif

















