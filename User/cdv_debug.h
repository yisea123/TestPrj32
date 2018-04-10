/**
  ******************************************************************************
  * @file    /cdv_debug.h 
  * @author  MMY
  * @version V2.0.0
  * @date    30-July-2015
  * @brief   debug function and var and 
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  CDV_CDV_DEBUG
#define  CDV_CDV_DEBUG



#include "cdv_include.h"

//typedef enum
//{ 
//	OPT_SUCCESS = 0,
//	OPT_FAILURE,
//}RET_STATUS;
void ChangeCdvStat(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo);

RET_STATUS DebugParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo);
RET_STATUS DebugSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen);
RET_STATUS DebugNext(void);
CDV_INT08U CurrentCmd(void);
CDV_INT08U NextCmd(void);
RET_STATUS DebugLast(void);
CDV_INT08U LastCmd(void);
RET_STATUS DebugJump(CDV_INT08U no, CDV_INT08U uartNo);
CDV_INT08U JumpCmd(CDV_INT08U no);
void DebugRun(CDV_INT08U uartNo);
RET_STATUS ScriptDebugParse(CDV_INT08U no, CDV_INT08U uartNo);

RET_STATUS GetWorkerScript(CDV_INT08U no, CDV_INT08U uartNo);
RET_STATUS GetWorkerTypeScript(CDV_INT08U type, CDV_INT08U no, CDV_INT08U uartNo);
RET_STATUS ChangeTypeDebugWorker(CDV_INT08U type, CDV_INT08U no, CDV_INT08U uartNo);

RET_STATUS GetAllWorker(CDV_INT08U uartNo);
RET_STATUS ScriptCmdSend(CDV_INT08U scriptNo);
RET_STATUS AppScriptSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen);

RET_STATUS DebugCmdSend(CDV_INT08U uartNo);

RET_STATUS GetWorkerInfo(void);
RET_STATUS SetRecFormula(CDV_INT08U no, CDV_INT08U uartNo);
CDV_INT08U GetRecFormula(CDV_INT08U uartNo) ;
#endif

