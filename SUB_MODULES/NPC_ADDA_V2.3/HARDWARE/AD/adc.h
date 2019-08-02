#ifndef __ADC_H
#define __ADC_H	
#include "cdv_include.h"  
//#include "sys.h" 
#define BUFFX_LEN 500
#define BUFFV_LEN 500

#define adc1_value_lens   10
#define adc2_value_lens   10
#define adc3_value_lens   10
#define adc4_value_lens   10
#define adc5_value_lens   10
#define adc6_value_lens   10
#define DMA_Buf_lens      20
#define  N  10
#define  M  2
#define  M1  4
#define  Adc_no  4

typedef struct 
{	
	CDV_INT32U Slope;   //б��ֵ K
	CDV_INT32U Zero;	  //��λֵ B
	CDV_INT32U Out_Min; //�����Сֵ
	CDV_INT32U Out_Max; //������ֵ
	CDV_INT32U Slope2;  //��ʱб��ֵ K
	CDV_INT32U Zero2;   //��ʱ��λֵ B
	CDV_INT32U Ad_Coeff1;  //ϵ��1
	CDV_INT32U Ad_Coeff2;  //ϵ��2
	u16 buf[adc1_value_lens]; //
	u16 fen_ya_value ;       //��ѹϵ��
} ADC_STRUCT;
extern ADC_STRUCT adc_par[Adc_no];


extern u16 adc1_value[adc1_value_lens];
extern u16 adc2_value[adc2_value_lens];
extern u16 adc3_value[adc3_value_lens];
 							   
void Adc_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc1(u8 ch); 				//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  

void Adc2_Init(void); 				//ADCͨ����ʼ��
void Adc2_Dma_Init(void);
u16  Get_Adc2(u8 ch); 				//���ĳ��ͨ��ֵ 
u16 Get_Adc2_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ 

void Adc3_Dma_Init(void);
void Adc3_Init(void); 				//ADC3ͨ����ʼ��	
void Adc1_DMA_Init(void);
void Adc2_DMA_Init(void);
void Adc3_DMA_Init(void);
u16 Adc_val_filter(u16 *buf,u16 lens);
u16 Grubbs(u16 x[],u16 n);
float AVRG(u16 x[],u16 n);
u32 SUM(u16 x[],u16 n);
void ADC1_DMA_Init(void);
void Adc2_Dma_Init2(void);
u16 Get_VFD_Speed(u16 *buf,u16 lens);
u8 Read_Adc_Par(u8 *buf,u8 no,u32 pflag);//ADC������ȡ
RET_STATUS AdcCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);

u16 Get_Adc_Voltge(u8 adc_num);
void Adc_Voltge ( void);
void AD_ReadInRegister(void);
u16 Get_Ad_value(u8 no);
s32 Get_Read_AD(u8 no);
#endif 















