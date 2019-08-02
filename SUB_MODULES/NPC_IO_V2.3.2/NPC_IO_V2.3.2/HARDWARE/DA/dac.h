#ifndef __DAC_H
#define __DAC_H	
#include "cdv_include.h"  	     			    
typedef struct 
{
	CDV_INT32U Slope;  //б��ֵ
	CDV_INT32U Zero;   //��λֵ
	CDV_INT32U Out_Min; //�����Сֵ
	CDV_INT32U Out_Max; //������ֵ
	CDV_INT08U Dac_num; //���ͨ��
	CDV_INT32U Da_Coeff1;  //ϵ��1
	CDV_INT32U Da_Coeff2;  //ϵ��2
	CDV_INT08U Pid_Flag;   //PID��־
	CDV_INT32U TRA_SPEED_LIMIT; //��Ƶ���ٶ�����
//	CDV_INT32U Slope[2];//б��ֵ
//	CDV_INT32U Zero[2];//��λֵ
//	CDV_INT32U Out_Min[2];//�����Сֵ
//	CDV_INT32U Out_Max[2];//������ֵ
//	CDV_INT32U PID_proportion[2];//PID ��������
//	CDV_INT32U PID_Derivative[2];//PID ΢�ֲ���
//	CDV_INT32U PID_integral[2];//PID ���ֲ���
//	CDV_INT32U PID_Integral_Limit[2];//PID ��������
//	CDV_INT32U PID_Ctrl_Time[2];//PID���Ƽ�� ��λms 
	float vol;//��ǰ�����ѹ
}DAC_STRUCT;

#define TRA_SPEED_LIMIT  5000
#define TRA1_SPEED_LIMIT 5000   //����Ƶ���ٶ����ޣ�������100��
#define TRA2_SPEED_LIMIT 5000   //����Ƶ���ٶ����ޣ�������100��
#define Dac_no   4

//extern DAC_STRUCT dac_par[Dac_no];

void Dac1_Init(void);		//DACͨ��1��ʼ��	 	 
void Dac2_Init(void);
void Dac1_Set_Vol(u16 vol);	//����ͨ��1�����ѹ

void Dac2_Set_Vol(u16 vol);//DACͨ��2��ʼ��
void Dac2_Set_Vol(u16 vol);	//����ͨ��2�����ѹ
RET_STATUS DacCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CMD_ARG *arg);
u8 Read_Dac_Par(u8 *buf,u8 no,u32 pflag);//DAC������ȡ
u16  Get_Adc_Voltge(u8 adc_num);
u16  Get_Da_value(u8 no);
//void  Dac_Set_Vol(DAC_STRUCT *dac_par,u8 no);
//void Dac_set_Voltge( CDV_INT08U dac_num, float vol);
void DAC_SETOUT(void);
#endif

















