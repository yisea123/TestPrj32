
/**
  ******************************************************************************
  * @file    /SysMng.h 
  * @author  MMY
  * @version V0.0.1
  * @date    6-14-2016
  * @brief   a package of SysMng function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _SYS_MNG_
#define  _SYS_MNG_

#include "cdv_include.h"

RET_STATUS SysCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
void ResetCdv(void);
void ChangeToCdvStat(CDV_INT08U stat);
#endif

