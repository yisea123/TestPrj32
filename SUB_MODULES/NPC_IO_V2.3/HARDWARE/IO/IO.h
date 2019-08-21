
/**
  ******************************************************************************
  * @file    /IO.h 
  * @author  MMY
  * @version V0.0.1
  * @date    1-12-2015
  * @brief   a package of io and dip function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _I_O_
#define  _I_O_

#define PORT_A ((GPIO_TypeDef *) GPIOA_BASE)
#define PORT_B ((GPIO_TypeDef *) GPIOB_BASE)
#define PORT_C ((GPIO_TypeDef *) GPIOC_BASE)
#define PORT_D ((GPIO_TypeDef *) GPIOD_BASE)
#define PORT_E ((GPIO_TypeDef *) GPIOE_BASE)
#define PORT_F ((GPIO_TypeDef *) GPIOF_BASE)
#define PORT_G ((GPIO_TypeDef *) GPIOG_BASE)
#define PORT_H ((GPIO_TypeDef *) GPIOH_BASE)
#define PORT_I ((GPIO_TypeDef *) GPIOI_BASE)

#define PIN_0  ((uint16_t)0x0001)
#define PIN_1  ((uint16_t)0x0002)
#define PIN_2  ((uint16_t)0x0004)
#define PIN_3  ((uint16_t)0x0008)
#define PIN_4  ((uint16_t)0x0010)
#define PIN_5  ((uint16_t)0x0020)
#define PIN_6  ((uint16_t)0x0040)
#define PIN_7  ((uint16_t)0x0080)
#define PIN_8  ((uint16_t)0x0100)
#define PIN_9  ((uint16_t)0x0200)
#define PIN_10 ((uint16_t)0x0400)
#define PIN_11 ((uint16_t)0x0800)
#define PIN_12 ((uint16_t)0x1000)
#define PIN_13 ((uint16_t)0x2000)
#define PIN_14 ((uint16_t)0x4000)
#define PIN_15 ((uint16_t)0x8000)
#include "cdv_include.h"    
/*IO*/
typedef struct
{	
	GPIO_TypeDef * port;//����˿�
	CDV_INT16U pin;//����˿ں�
}CDV_IO;
/*CDV��Դȫ�ֱ���
��IOInit�г�ʼ��*/

#if _NPC_VERSION_ == 1u
 //#define CDV_IO_NUM 20
 #define CDV_I_NUM 20
 #define CDV_O_NUM 20
 #define CDV_DIP_NUM 6
#elif _NPC_VERSION_ == 2u
 //#define CDV_IO_NUM 22
 #define CDV_I_NUM 14
 #define CDV_O_NUM 15
 #define CDV_DIP_NUM 3
#endif

//#define CDV_I_NUM CDV_IO_NUM-2
//#define CDV_O_NUM CDV_IO_NUM
extern  CDV_IO g_cdvI[CDV_I_NUM];
extern  CDV_IO g_cdvO[CDV_O_NUM];

//i����

#define CH_CDV_I(A)          g_i[A]

#if 1
#define SET_I(A) do{\
	(CH_CDV_I((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);
#define RESET_I(A) do{\
	(CH_CDV_I((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);
#define READ_I(A) (((CH_CDV_I((A) >> 3)) & (0x01 <<((A) & 0x07)))? 1 :0)
#else
#define RESET_I(A) do{\
	(CH_CDV_I((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);
#define SET_I(A) do{\
	(CH_CDV_I((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);
#define READ_I(A) !(((CH_CDV_I((A) >> 3)) & (0x01 <<((A) & 0x07)))? 1 :0)
#endif

extern  CDV_INT08U* g_i;
//O����

#define CH_CDV_O(A)          g_o[A]

#if 1
#define SET_O(A) do{\
	(CH_CDV_O((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);
#define RESET_O(A) do{\
	(CH_CDV_O((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);
#define READ_O(A) (((CH_CDV_O((A) >> 3)) & (0x01 <<((A) & 0x07)))? 1 :0)
#else
#define RESET_O(A) do{\
	(CH_CDV_O((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);
#define SET_O(A) do{\
	(CH_CDV_O((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);
#define READ_O(A) !(((CH_CDV_O((A) >> 3)) & (0x01 <<((A) & 0x07)))? 1 :0)
#endif

extern  CDV_INT08U* g_o;
/*���뿪��*/
extern  CDV_IO g_dipSW[CDV_DIP_NUM];
#define READ_DIP_SW(A) GPIO_ReadInputDataBit(g_dipSW[(A)].port , g_dipSW[(A)].pin)
#define DIP_OFF   1
#define DIP_ON    0

#define SW1  GPIO_ReadInputDataBit(g_dipSW[0].port,g_dipSW[0].pin)
#define SW2  GPIO_ReadInputDataBit(g_dipSW[1].port,g_dipSW[1].pin)
#define SW3  GPIO_ReadInputDataBit(g_dipSW[2].port,g_dipSW[2].pin)
#define READ_ID   ((SW1<<2)|(SW2<<1)|SW3 )

#define CH_CDV_DIP          g_dip
extern  CDV_INT08U g_dip;
#define READ_CDV_DIP(A)  (((CH_CDV_DIP) & (0x01 << (A)))&&1)
 
//���仺��
//extern CDV_INT08U g_pluse[COIL_CHN];
//extern OS_SEM PLUSE_SEM;
#define CH_PLUSE_I(A)          g_pluse[A]


#define SET_PLUSE_I(A) do{\
	(CH_PLUSE_I((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);
#define RESET_PLUSE_I(A) do{\
	(CH_PLUSE_I((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);
#define READ_PLUSE_I(A) (((CH_PLUSE_I((A) >> 3)) & (0x01 <<((A) & 0x07)))? 1 :0)

//����npc�Ľӷ���ȷ��
typedef enum
{ 
  BIT_1  = 0, 
  BIT_0  = 1, 
}IO_VAL;

 /*�������ú���*/
void GPIO_Configuration(void);

void IOInit(void);
RET_STATUS ICmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
RET_STATUS OCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
IO_VAL IRead(CDV_INT32U no);
//IO_VAL ShakeIRead(CDV_INT32U no);
//IO_VAL ORead(CDV_INT32U no);
void OWrite(CDV_INT32U no , IO_VAL ioVal);
void ExIOInit(void);
//IO_VAL DIPRead(CDV_INT08U no);
//IO_VAL ExIRead(CDV_INT08U no);
//IO_VAL ExORead(CDV_INT08U no);
//void ExOWrite(CDV_INT08U no , IO_VAL val);
u16 Read_Input_All(void);
u16 Read_Output_ALL(void);
CDV_INT08U OBitRead(CDV_INT32U no);
CDV_INT08U IBitRead(CDV_INT32U no);
void NPC_GpioConfig(void);
void NPC_IOInit(void);
void OUTPUT(void);
RET_STATUS CmdParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);
//u16  READ_SW (void);
#define I_PAR_NUM 2 //���в������ֽڳ���
#endif
