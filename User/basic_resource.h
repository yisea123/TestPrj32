
/**
  ******************************************************************************
  * @file    /cdv_resource.h 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   user function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CDV_BASIC_RESOURCE_
#define  _CDV_BASIC_RESOURCE_








#include "cdv_include.h"    
/*
 *CDV ��Դ����
 */
 /*ָʾ��*/
 #define LED_ON  0
 #define LED_OFF 1

#if _NPC_VERSION_ == 1u
	#if defined(CDV_V1)
		#define LED1 PDout(9)	  // LED1 ����ָʾ
		#define LED2 PDout(11)	// LED2	�澯ָʾ
		#define LED3 PDout(13)	// LED3 ����ָʾ
	#else
		#define LED1 PHout(12)	// LED1 ����ָʾ
		#define LED2 PBout(12)	// LED2	�澯ָʾ
		#define LED3 PHout(11)	// LED3 ����ָʾ
	#endif
#elif _NPC_VERSION_ == 2u
	#define LED1 PGout(4)	//�ϰ��м�
	#define LED2 PGout(4) //
	#define LED3 PBout(6)	//�°� LED3 REDָʾ��Ӧ���ӵ�LED1
#elif _NPC_VERSION_ == 3u
	#define LED1 PGout(7)	//����
	#define LED2 PGout(6) //����
	#define LED3 PBout(6)	//����
#endif

//extern CDV_INT08U resReqed;
typedef enum  {
	RC_NONE= 0,
	RC_CRC,
	
}REQUEST_CTRL;

//extern CDV_INT08U g_resUart;
/*��������״̬�ṹ*/
//typedef enum
//{ 
//	OPT_SUCCESS = 0,
//	OPT_FAILURE,
//}RET_STATUS;

//RET_STATUS MotorRun(CDV_INT08U no /*, CDV_INT08U opt */, CDV_INT32S num);
//RET_STATUS MotorChk(CDV_INT08U no , CDV_INT08U opt);



//void IRefresh(void);
//void ORefresh(void);
//void MotorRefresh(void);
//void MotorStepRefresh(void);
void CdvInit(void);
RET_STATUS ResParse(CDV_INT08U* rxBuf, CDV_INT16U rxLen,  CMD_ARG *arg);
//RET_STATUS DisplayCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U debug, CDV_INT08U uartNo);
RET_STATUS VirtualMotorRun(CDV_INT08U no /*, CDV_INT08U opt*/ , CDV_INT32S num);
RET_STATUS ActualMotorRun(CDV_INT08U no /*, CDV_INT08U opt*/ , CDV_INT32S num);
//RET_STATUS MotorStop(CDV_INT08U no );
void ResParaRequest(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CDV_INT08U* para, CDV_INT08U paraLen, CDV_INT08U uartNo);
//RET_STATUS Motor_Forward_Backward(CDV_INT08U no /*, CDV_INT08U opt*/ , CDV_INT32S num,u8 type);
void ResRequest(CDV_INT08U* rxBuf, CDV_INT16U rxLen , CDV_INT08U* para, CDV_INT16U paraLen, CMD_ARG *arg, REQUEST_CTRL ctrl);
CDV_INT32S CalculateForAll(CDV_INT08U* buf,CDV_INT32S addr,CDV_INT32S lenth);
RET_STATUS DoResRequest(CMD_ARG *arg);
RET_STATUS DoSpecRequest(CMD_ARG *arg);
void ResVarSet(CDV_INT32U no, CDV_INT32S num, CMD_ARG *arg);
#endif
