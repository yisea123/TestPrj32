
/**
  ******************************************************************************
  * @file    /Msg.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-2
  * @brief   a package of Msg function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _MSG_
#define  _MSG_

#include "cdv_include.h"    

#define CDV_MSG_NUM 32


RET_STATUS MsgCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg);
void ClearMsg(void);


#endif

