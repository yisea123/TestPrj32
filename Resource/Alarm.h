/**
  ******************************************************************************
  * @file    /PH.h 
  * @author  MMY
  * @version V0.0.1
  * @date    12-9-2016
  * @brief   a package of PH function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	//#include "stm32f4xx.h"

#ifndef  __ALARM_H
#define  __ALARM_H

#define ALARM_NUM 20
#include "cdv_include.h"

/*±¨¾¯²éÑ¯*/
void AlarmQuery11(u8 ID);
//void AlarmCancel(u8 num);
void AlarmCancel(u8 no);

int FlashToAlarmStruct(void);
//void AlarmQuery(u8* rxBuf,u8 rxLen,CMD_ARG *arg);
void AlarmQuery(u8* rxBuf,u8 rxLen,CMD_ARG *uartNo);
typedef struct 
{
	CDV_INT32U cAlarmID;
	CDV_INT32U cAlarmNo;
	CDV_INT32U iAlarmGrade;
	CDV_INT32U iValMin;
	CDV_INT32U iValMax;
	
} AlarmInfo;

extern int nAlarmCount;
extern u8 Alarmflag;
extern AlarmInfo strAlarmInfo [ALARM_NUM];
extern int nAlarmCount;
extern AlarmInfo strAlarmInfo [ALARM_NUM];
RET_STATUS ApplyParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);
RET_STATUS App1Parse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
u32 Alarm_flag(u8 flag);

#endif





