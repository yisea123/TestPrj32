
/**
  ******************************************************************************
  * @file    /CentralizedControl.h 
  * @author  MMY
  * @version V0.0.1
  * @date    05-17-2018
  * @brief   
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CENTRALIZED_CONTROL_
#define  _CENTRALIZED_CONTROL_

#include "cdv_include.h"

#if USE_CENTRALIZEDCONTROL == 1u

RET_STATUS CentralizedControl_QuerySysInfo(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);
RET_STATUS CentralizedControl_QueryDevData(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);
RET_STATUS CentralizedControl_SetDevData(CDV_INT08U* buf, CDV_INT16U len, CMD_ARG *arg);

RET_STATUS CentralizedControl_SysOperation(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);
void CentralizedControl_OTA(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);

#endif // #if USE_CENTRALIZEDCONTROL == 1u

#endif // #ifndef  _CENTRALIZED_CONTROL_

