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
  DEBUG_DISABLE  = 0, //����ģʽ����������ģʽ
	DEBUG_ENABLE,
	DEBUG_CONTINUE,
	DEBUG_PAUSE,
	DEBUG_STEP,
	DEBUG_NULL,//�������
}DEBUG_STATE;

typedef struct {
	ARRAY_VOID*  point;                                 /*����ÿ���ϵ磬�ݲ�֧������*/
	DEBUG_STATE   state;//0 �ϵ㲻�����ã�1 �����ϵ� 2 �������Զ���0��3 ��ͣ��4 ����
} DEBUG_CTRL;

//#include "cdv_include.h"
typedef struct {
	CDV_INT16U pos;
	CDV_INT08U* buf;         /*����*/
	CDV_INT08U len;          /*��������*/	
} DEBUG_POINT;

#include "cdv_include.h"

#if USE_WORKER_DEBUG == 1u
CDV_INT08S DebugOpen(DEBUG_CTRL* debug);
CDV_INT08S DebugClose(DEBUG_CTRL* debug);
  BOOL IsDebugPoint(DEBUG_CTRL* debug, CDV_INT16U pos);

void AllDebugStateSet(DEBUG_STATE state);
RET_STATUS WorkerDebugParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);

	extern OS_MUTEX DEBUG_SEM;
#endif
#endif

