
/**
  ******************************************************************************
  * @file    /extiPowerOff.h
  * @author  MMY
  * @version V0.0.1
  * @date    2018-6-7
  * @brief   a package of exti power off
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _EXTI_POWER_OFF_
#define  _EXTI_POWER_OFF_
#include "cdv_include.h"
#if USE_EXTI_POWER_OFF == 1u

void EXTIPowerOff_Configuration(void);
s32 EXTIPowerOff_GetFlag(void);
void EXTIPowerOff_ClearFlag(void)  ;
#endif
#endif

