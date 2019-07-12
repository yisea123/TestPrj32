/**
  ******************************************************************************
  * @file    /worker.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-15
  * @brief   
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _WORKER_
#define  _WORKER_

#include "cdv_include.h"

#define WORKER_MAX_NUM WORKER_NUM
/*data*/
typedef enum {                       /*静态      动态*/
	WORKER_STOP,                       /*停止      立即停止*/
  WORKER_ONCE,                       /*运行一次  继续*/
	WORKER_LOOP,                       /*循环运行  继续*/
	WORKER_PAUSE,                      /*暂停      暂停*/
	WORKER_EXIT,                       /*退出      退出*/
	WORKER_MAX_STATUS,                 /*最大状态标识*/
} WORKER_STATUS;

extern const char *WorkerStatusName[]; 

typedef struct {
	CDV_INT16U which;                  /*哪个工人要运行*/
	CDV_INT32U startPos;               /*工人脚本起始位置*/		
	CDV_INT16U cmdNum;					       /*脚本命令总数*/
	CDV_INT32U cmdPos;                 /*脚本的当前命令的flash起始地址，用于提取flash中的各条命令*/
	CDV_INT16U doNo;                   /*脚本处理序号，自累加，可与脚本中存储的序号进行对比检错*/	
	CDV_INT16U totalLen;               /*脚本总长度*/
	CDV_INT08U *buf;                   /*工人脚本缓存指针，一条*/
	CDV_INT16U len;                    /*工人脚本缓存长度，一条*/
	CMD_ARG *arg;//指向执行时的命令参数，做传递用
} DEBUG_SCRIPT;

typedef struct {
	CDV_INT08U which;                  /*哪个工人占用该线程*/
	CDV_INT08U uartNo;                 /*占用串口*/
	CDV_INT08U opt;                    /*特殊操作，0：无，1：goto*/
	CDV_INT16U step;                   /*指令所在位置*/
	CDV_INT16U exitStep;               /*退出指令所在位置*/
	CDV_INT16U gotoStep;
	
#if USE_WORKER_DEBUG == 1u
	DEBUG_CTRL debug;
#endif
	
	WORKER_STATUS status;              /*哪个状态*/
} THREAD_INFO;

extern THREAD_INFO g_threadInfo[WORKER_MAX_NUM];
/*function*/
BOOL IsTaskExist(const OS_TCB tcb) ;
void WorkerTask(void *p_arg);
RET_STATUS ManagerControl(const WORKER_STATUS status);
void RestartWorkers(void);
BOOL IsWorkerExist(const CDV_INT08U no);
RET_STATUS ManagerQueryStatus(WORKER_STATUS* pStatus);
RET_STATUS WorkerControl(const CDV_INT08U no, const WORKER_STATUS status);
RET_STATUS WorkerExitSet(CDV_INT08U no, CDV_INT32U step);
RET_STATUS WorkerQuery(const CDV_INT08U no, WORKER_STATUS *pStatus, CDV_INT32U* pStep);
RET_STATUS WorkerQueryStatus(const CDV_INT08U no, WORKER_STATUS* pStatus);
RET_STATUS WorkerQueryStep(const CDV_INT08U no, CDV_INT16U* pStep);
RET_STATUS WorkerCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen , CMD_ARG *arg);
RET_STATUS LineCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg);

RET_STATUS ToWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT16U cmdNo) ;

void AllWorkerCtrl(const WORKER_STATUS status);
CDV_INT32S WorkerRead(CDV_INT08U no);

//time 

#ifdef  DEBUG_TIME
extern CDV_INT32U time_worker[100];
extern CDV_INT08U time_which_worker;//几号工人
#endif

#endif

