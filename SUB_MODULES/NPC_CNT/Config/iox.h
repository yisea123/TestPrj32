
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

#include "_include.h"    



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

//����npc�Ľӷ���ȷ��
typedef enum
{ 
  BIT_1  = 0, 
  BIT_0  = 1, 
}IO_VAL;

 /*�������ú���*/
void ConfigGpio(void);
void InitGpio(void);

 #define LED_ON  0
 #define LED_OFF 1

#if defined (STM32F10X_HD)
  #define LED1 PBout(7)		// ����ָʾ��;û�������
  #define LED2 PBout(1)	  // ����ָʾ��;	
  #define LED3 PBout(0)	// ����ָʾ��;

	#define SW1 PBin(10)		// ����ָʾ��;
	#define SW2 PBin(11)	  // ����ָʾ��;	
	#define SW3 PCin(7)	// ����ָʾ��;		
#elif defined (STM32F40_41xxx)
	#define LED1 PGout(4)	//
	#define LED2 PGout(4) //
	#define LED3 PBout(6)	//

	#define SW1 PHin(10)		// ����ָʾ��;
#endif


#endif

