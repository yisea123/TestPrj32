
/**
  ******************************************************************************
  * @file    /Var.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-2
  * @brief   a package of var function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _VAR_
#define  _VAR_


#include "cdv_include.h"    

#if _NPC_VERSION_ == 1u || _NPC_VERSION_ == 2u
#define CDV_VAR_NUM 390//290
#elif _NPC_VERSION_ == 3u
#define CDV_VAR_NUM 490//390//290
#endif


#define VAR_ADDR(A) (VAL_STADDR + 4 * (A))



//extern CDV_INT32S* g_var;

RET_STATUS ValCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg);
void ValSet(CDV_INT32U no, CDV_INT32S num);
CDV_INT32S VarGet(CDV_INT32U no);
void ValFlashSet(CDV_INT32U no, CDV_INT32S num);
void ValToFlash(CDV_INT32U no, CDV_INT16U num);
void FlashToVal(CDV_INT32U no, CDV_INT16U num);

CDV_INT32S ValFlashGet(CDV_INT32U no);


#define VAL_PAR_NUM 4 //���в������ֽڳ���
#endif

