/**
  ******************************************************************************
  * @file    /workerDebug.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2018-12-12
  * @brief   
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _WORKER_DEBUG_
#define  _WORKER_DEBUG_

#include "cdv_array.h"

typedef enum
{ 
  DEBUG_DISABLE  = 0, //联机模式，开机启动模式
	DEBUG_ENABLE,
	DEBUG_CONTINUE,
	DEBUG_PAUSE,
	DEBUG_STEP,
	DEBUG_NULL,//其他情况
}DEBUG_STATE;

typedef struct {
	ARRAY_VOID*  point;                                 /*保存每个断电，暂不支持条件*/
	DEBUG_STATE   state;//0 断点不起作用；1 启动断点 2 继续，自动清0；3 暂停；4 单步
} DEBUG_CTRL;

//#include "cdv_include.h"
typedef struct {
	CDV_INT16U pos;
	CDV_INT08U* buf;         /*条件*/
	CDV_INT08U len;          /*条件长度*/	
} DEBUG_POINT;


CDV_INT08S DebugOpen(DEBUG_CTRL* debug);
CDV_INT08S DebugClose(DEBUG_CTRL* debug);
  BOOL IsDebugPoint(DEBUG_CTRL* debug, CDV_INT16U pos);
	#include "cdv_include.h"

void AllDebugStateSet(DEBUG_STATE state);
RET_STATUS WorkerDebugParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);

	extern OS_SEM DEBUG_SEM;

#endif

