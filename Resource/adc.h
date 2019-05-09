#ifndef __ADC_H
#define __ADC_H	
#include "cdv_include.h"  
//#include "sys.h" 
#define BUFFX_LEN 200
#define BUFFV_LEN 200

#if _NPC_VERSION_ == 1u
#define adc1_value_lens   100
#define adc2_value_lens   100
#define adc3_value_lens   100
#define adc4_value_lens   100
#define  N  1
#define  M  1
#define  Adc_no  3
#elif _NPC_VERSION_ == 2u
#define adc1_value_lens   100
#define adc2_value_lens   100
#define adc3_value_lens   100
#define adc4_value_lens   100
#define  N  100
#define  M  2
#define  Adc_no  4
#elif _NPC_VERSION_ == 3u
#define adc1_value_lens   1
#define adc2_value_lens   1
#define adc3_value_lens   1
#define adc4_value_lens   1
#define  N  100
#define  M  4
#define  Adc_no  4
#endif

typedef struct 
{	
	CDV_INT32U Slope;   //斜率值 K
	CDV_INT32U Zero;	  //零位值 B
	CDV_INT32U Out_Min; //输出最小值
	CDV_INT32U Out_Max; //输出最大值
	CDV_INT32U Slope2;  //临时斜率值 K
	CDV_INT32U Zero2;   //临时零位值 B
	CDV_INT32U Ad_Coeff1;  //系数1
	CDV_INT32U Ad_Coeff2;  //系数2
#if _NPC_VERSION_ == 1u ||_NPC_VERSION_ == 2u
	u16 buf[adc1_value_lens]; //	
#elif _NPC_VERSION_ == 3u
	u16 *buf; //	
#endif
	u16 fen_ya_value ;       //分压系数
} ADC_STRUCT;
extern ADC_STRUCT adc_par[Adc_no];

extern CDV_INT32S* g_adcval;
//extern u16 adc1_value[adc1_value_lens];
//extern u16 adc2_value[adc2_value_lens];
//extern u16 adc3_value[adc3_value_lens];
 							   
void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  

void Adc2_Init(void); 				//ADC通道初始化
void Adc2_Dma_Init(void);
u16  Get_Adc2(u8 ch); 				//获得某个通道值 
u16 Get_Adc2_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值 

void Adc3_Dma_Init(void);
void Adc3_Init(void); 				//ADC3通道初始化	


u16 Adc_val_filter(u16 *buf,u16 lens);
u16 Grubbs(u16 x[],u16 n);
float AVRG(u16 x[],u16 n);
u32 SUM(u16 x[],u16 n);
void ADC1_DMA_Init(void);
void Adc2_Dma_Init2(void);
void Adc2_Dma_Init4(void);
u16 Get_VFD_Speed(u16 *buf,u16 lens);
u8 Read_Adc_Par(u8 *buf,u8 no,u32 pflag);//ADC参数读取
RET_STATUS AdcCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg);

u16 Get_Adc_Voltge(u8 adc_num);
void Adc_Voltge ( void);
u16 Get_Ad_value(u8 no);
s32 Get_Read_AD(u8 no);
void UpdateAdcVal(void);
CDV_INT32S AdcRead(CDV_INT08U no);
#endif















