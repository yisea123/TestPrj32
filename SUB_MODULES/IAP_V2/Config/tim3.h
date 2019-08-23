
/**
  ******************************************************************************
  * @file    /Time.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-2
  * @brief   a package of Time function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _TIME_
#define  _TIME_

#include "cdv_include.h"

CDV_INT32S ReadClock1ms(void);
CDV_INT32S ReadClock100ms(void);
/*��ʱms*/
void TimeDelay(CDV_INT08U no, CDV_INT32U num) ;
RET_STATUS TimeCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo);
RET_STATUS ClockCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo);
void TIM3_Init(u16 TIM_scale, u16 TIM_Period);


#endif

