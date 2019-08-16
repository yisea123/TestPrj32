
/**
  ******************************************************************************
  * @file    /lowPower.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2018-5-31
  * @brief   a package of lowPower
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */



#ifndef  _LOW_POWER_

#define  _LOW_POWER_

#include "cdv_include.h"

#if USE_LOWPOWER == 1u
void Sys_Enter_Standby(void);
void WKUP_Init(void);
#endif

#endif
