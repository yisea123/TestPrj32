
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
	GPIO_TypeDef * port;//输出端口
	CDV_INT16U pin;//输出端口号
}CDV_IO;
/*CDV资源全局变量
在IOInit中初始化*/

#if _NPC_VERSION_ == 1u
 //#define CDV_IO_NUM 20
 #define CDV_I_NUM 20
 #define CDV_O_NUM 20
 #define CDV_DIP_NUM 6
#elif _NPC_VERSION_ == 2u
 //#define CDV_IO_NUM 22
 #define CDV_I_NUM 20
 #define CDV_O_NUM 22
 #define CDV_DIP_NUM 3
#elif _NPC_VERSION_ == 3u
 #define CDV_I_NUM 20
 #define CDV_O_NUM 22
 #define CDV_DIP_NUM 3
#endif

//#define CDV_I_NUM CDV_IO_NUM-2
//#define CDV_O_NUM CDV_IO_NUM
extern  CDV_IO g_cdvI[CDV_I_NUM];
extern  CDV_IO g_cdvO[CDV_O_NUM];

//i缓存

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
//O缓存

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
/*拨码开关*/
extern  CDV_IO g_dipSW[CDV_DIP_NUM];
#define READ_DIP_SW(A) GPIO_ReadInputDataBit(g_dipSW[(A)].port , g_dipSW[(A)].pin)
#define DIP_OFF   1
#define DIP_ON    0

#define CH_CDV_DIP          g_dip
extern  CDV_INT08U g_dip;
#define READ_CDV_DIP(A)  (((CH_CDV_DIP) & (0x01 << (A)))&&1)

//根据npc的接法来确定
typedef enum
{ 
  BIT_1  = 0, 
  BIT_0  = 1, 
}IO_VAL;

 /*外设配置函数*/
void GPIO_Configuration(void);

void IOInit(void);
RET_STATUS ICmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
RET_STATUS OCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
IO_VAL IRead(CDV_INT32U no);
//IO_VAL ShakeIRead(CDV_INT32U no);
//IO_VAL ORead(CDV_INT32U no);
void OWrite(CDV_INT32U no , IO_VAL ioVal);
void ExIOInit(void);
void LED_Init(void);
u32 Read_Input_All(void);
u32 Read_Output_ALL(void);
#define I_PAR_NUM 2 //所有参数的字节长度

 #define LED_ON  0
 #define LED_OFF 1
#if _NPC_VERSION_ == 1u
	#if defined(CDV_V1)
		#define LED1 PDout(9)	  // LED1 运行指示
		#define LED2 PDout(11)	// LED2	告警指示
		#define LED3 PDout(13)	// LED3 故障指示
		#else
		#define LED1 PHout(12)	// LED1 运行指示
		#define LED2 PBout(12)	// LED2	告警指示
		#define LED3 PHout(11)	// LED3 故障指示
	#endif
#elif _NPC_VERSION_ == 2u
	#define LED1 PGout(4)	//上板中间
	#define LED2 PGout(4) //
	#define LED3 PBout(6)	//下板 LED3 RED指示对应板子的LED1
#elif _NPC_VERSION_ == 3u
	#define LED1 PGout(7)	//运行
	#define LED2 PGout(6) //故障
	#define LED3 PBout(6)	//调试
#endif
#endif

