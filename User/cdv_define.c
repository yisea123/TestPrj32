/**
  ******************************************************************************
  * @file    /user_define.c 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   global variable 
	*
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
#include "cdv_define.h"


CDV_STATUS g_cdvStat = CDV_FPGA_INIT/*,g_tmpCdvStat = CDV_NULL*/;                      /*CDV状态标记*/
CDV_INT08U g_nowDoRes = 0;   /*标记现在进入的资源号*/
SCRIPT_INFO g_scriptInfo;                               /*CDV脚本状态结构体*/
//DEBUG_DATA g_debugData;
/*软件定时器*/
CDV_INT32U tm1Re = 0,tm2Re = 0,tm3Re = 0;
OS_TMR 	tmr1;		//定时器1//用于串口1收
OS_TMR	tmr2;		//定时器2//用于wifi
OS_TMR	tmr3;		//定时器3//用于串口缓存
/*任务控制块*/
OS_TCB StartTaskTCB;
OS_TCB UsartRecvTaskTCB;
OS_TCB UsartSendTaskTCB;
OS_TCB CdvRefreshTaskTCB;
OS_TCB CdvRefreshTaskTCB;
OS_TCB CdvValveTaskTCB;
OS_TCB CdvMotoTaskTCB;
OS_TCB WorkerManageTaskTCB;
OS_TCB WorkerTaskTCB[WORKER_NUM];
OS_TCB TaskParseTCB;
const char *workerName[] = {"work1","work2","work3","work4","work5","work6","work7","work8","work9","work10","work11","work12","work13","work14","work15","work16","work17","work18","work19","work20"};
//CDV_INT08U workerId[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
CPU_STK WORKER_TASK_STK[WORKER_NUM][WORKER_STK_SIZE];//任务堆栈
//CDV_INT08U g_whichUart; //指示串口线程中当前命令是从哪个串口发过来的
/**/
OS_SEM TX_SEM, WORKER_SEM, MOTO_SEM,MEM_SEM , MSG_SEM;//, PROMPT_SEM  , CACHE_SEM , WIFI_SEM, VAL_SEM, SPI_SEM 
OS_SEM GENERAL_SERIAL_SEM;
OS_SEM TCP_TX_SEM;

//CDV_WORD g_workerRunNum = 0;
//CDV_WORD g_appSetNum = 0;

//CDV_WORD g_numRun = 0;
//CDV_WORD g_numDebug = 0;
//CDV_WORD g_getNo = 0;
	

	//TEST
double RPressureData[100] = {0};
int RPressureCnt = 0;

void CmdArgInit(CMD_ARG *arg)
{
	INIT_CLEAR(*arg);
#if _NPC_VERSION_ > 1u
	arg->hostid = CascadeGetNativeNo();
#endif
	arg->uart = 0;//占用串口
	arg->len = 0;//命令长度
	arg->buf = NULL;//命令
	arg->ptrWorker = NULL; //指向工人结构体，用于逻辑等工人控制
	/*资源回复相关*/
	arg->reqlen = 0;
	arg->reqbuf = NULL;
	/*特殊操作记录，例如设置变量*/
	arg->speclen = 0;
	arg->specbuf = NULL;
	//memset(&arg->stack, 0, sizeof(arg->stack));
	//stack初始化
	Init(&arg->stack.istack);
	InitC(&arg->stack.cstack);
}

void CmdArgDelete(CMD_ARG *arg)
{
	DELETE(arg->reqbuf);
	DELETE(arg->specbuf);
	DeInit(&arg->stack.istack);
	DeInitC(&arg->stack.cstack);
	INIT_CLEAR(*arg);
}
