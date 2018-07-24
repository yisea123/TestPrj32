/**
  ******************************************************************************
  * @file    /worker.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-15
  * @brief   
  * 
@verbatim  

@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "worker.h"
	
const char *WorkerStatusName[] = {"WORKER_STOP","WORKER_ONCE","WORKER_LOOP","WORKER_PAUSE","WORKER_EXIT","WORKER_MAX_STATUS"};
THREAD_INFO g_threadInfo[WORKER_MAX_NUM] = {0};

/** @brief  工人控制
  * @param  no 几号工人
  *         status 目标状态，无此状态，返回OPT_FAILURE
  * @retval OPT_FAILURE 切换失败| OPT_SUCCESS 切换成功
  * @note   
  */
BOOL IsTaskExist(const OS_TCB tcb) {
	if(OS_TASK_STATE_DEL == tcb.TaskState
			|| NULL == tcb.StkPtr)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
	
}
	


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////调用工人及工人流程的控制/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////依赖于MEM中的脚本///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @brief  读指令到buf
  * @param  
  * @retval 返回值说明
  * @note   
  */
RET_STATUS ReadWorkerBuf(DEBUG_SCRIPT *debugWorker) {

	RET_STATUS ret;
//#ifdef  _DEBUG_NPC_
//	Mem_Read(&debugWorker->len, debugWorker->cmdPos + 2, 1);
//#else
	debugWorker->len = *(CDV_INT08U*)(SCRIPT_GETADDR(debugWorker->cmdPos + 2));
//#endif	
	debugWorker->len += 3;
	//NEW08U(debugWorker->buf , debugWorker->len);
  //Mem_Read(debugWorker->buf, debugWorker->cmdPos + 3, (CDV_INT16U)(debugWorker->len));	
  debugWorker->buf = Mem_Ptr(debugWorker->cmdPos/* + 3*/);
	//ASSERT(NULL != debugWorker->buf);
	return ret;
}


/** @brief  跳转到工人的某条命令
  * @param  void
  * @retval 返回值说明
  * @note   必须在初始化之后执行
  */
RET_STATUS ToWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT16U cmdNo) {
  CDV_INT32U staddr = debugWorker->startPos - FLASH_WORK_ADDR;
	
	ASSERT(cmdNo < (debugWorker)->cmdNum);
	
	if(cmdNo >= (debugWorker)->cmdNum)
		return OPT_FAILURE;
	
	debugWorker->doNo = cmdNo;

	debugWorker->cmdPos = Mem_GetCmdPos(cmdNo , debugWorker->cmdNum , staddr);
	
	ReadWorkerBuf(debugWorker);
  return OPT_SUCCESS;
}



/** @brief  初始化调试工人
  * @param  no     待获取工人号
  * @retval 返回值说明
  * @note   
  */
RET_STATUS InitWorker(const CDV_INT08U no, DEBUG_SCRIPT *debugWorker) {
	
//	CDV_INT08U i = no/* ,*buf = NULL*/;
	CDV_INT32U staddr;
	CDV_INT16U len = 0;
	//DebugClear();
  if(!g_line.init) {
		return OPT_FAILURE;
	}
	
	if(no >= g_line.workerAddr.len) {
		return OPT_FAILURE;
	}
	
	///////////////////////////
	
	len = 0;
	/*读取脚本信息*/
	
	debugWorker->startPos = SCRIPADDR(no);//起始地址
//#ifdef  _DEBUG_NPC_
//	Mem_Read((CDV_INT08U*)&len, SCRIP_LEN(no) - FLASH_WORK_ADDR, 2);
//#else
	len = *(CDV_INT16U*)(SCRIPT_GETADDR(SCRIP_LEN(no) - FLASH_WORK_ADDR));
//#endif	


  debugWorker->totalLen = len;
	
	if(2 < len) {
		debugWorker->cmdNum =  Mem_TestCmdNum(debugWorker->startPos - FLASH_WORK_ADDR , len);
	} else {
		debugWorker->cmdNum = 0;
	}
	
	if(0 == debugWorker->cmdNum)
		return OPT_FAILURE;
	
	ToWorkerCmd(debugWorker, 0);//
	debugWorker->which = no;
	return OPT_SUCCESS;
}
	

/** @brief  下一条命令
  * @param  void
  *         loop 循环，1循环，0不循环
  * @retval OPT_SUCCESS 成功指向下一条命令
  *         OPT_FAILURE 失败
  * @note   pos会跳转到下一条命令开始处
20151228 将原来的判断序号来决定是否脚本结束改成判断flash地址偏移大小
  */
RET_STATUS NextWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT08U loop) {
		
  CDV_INT32U staddr = debugWorker->startPos - FLASH_WORK_ADDR;
	
	
  debugWorker->cmdPos = Mem_GetNextCmdPos(debugWorker->cmdPos);

	if(debugWorker->cmdPos - staddr >= debugWorker->totalLen) {
		if(loop) {

			debugWorker->cmdPos = staddr;

			debugWorker->doNo = 0;	
		} else {
			return OPT_FAILURE;
		}
	}	else {
		
		debugWorker->doNo++;
	}
	
	ReadWorkerBuf(debugWorker);
	
	//debugWorker->doNo = Mem_GetNoFromPos(staddr,debugWorker->cmdPos);
	return OPT_SUCCESS;
}



/** @brief  分析当前命令并执行
  * @param  scriptNo     待分析工人号
  * @retval 返回值说明
  * @note   必须保证同时只有一个线程在分析相同scriptNo的脚本
            第几条（高位）	第几条（低位）	命令剩余字节	    资源类型	功能码1	        功能码2	    功能码3
            XX	            XX	            剩余长度（00-FF）	00	      资源号（00-FF）	00绝对运动	FF
  */
RET_STATUS WorkerCmdParse(DEBUG_SCRIPT *debugWorker) {
	RET_STATUS ret = OPT_SUCCESS;
	CMD_ARG *arg = debugWorker->arg;
	//INIT_CLEAR(arg);
	//CmdArgInit(&arg);
	if(arg->hostid == debugWorker->buf[3] || 
		(KFC_IC == debugWorker->buf[0] && 0x10 > debugWorker->buf[1]))
	{
//		arg->uart = uartNo;
//		arg->ptrWorker = debugWorker;
		ret = CmdParse (debugWorker->buf , debugWorker->len , arg);
		//ret =  ResParse (debugWorker->buf , debugWorker->len ,(void* )&debugWorker->which, 0, &arg);
	}
#if _NPC_VERSION_ > 1u
	else
	{
		ret = TFUsartCmdParse (debugWorker->buf , debugWorker->len , arg);
//		CDV_INT08U* recvBuf = NULL;
//		CDV_INT08U  recvLen = 0;
//		
//		
////		if(OPT_SUCCESS == (ret = CascadeTranferCmd(&recvBuf, &recvLen, debugWorker->buf , debugWorker->len)))
////		  ret = TFCmdParse(recvBuf, recvLen);
//		if(OPT_SUCCESS == (ret = CascadeUsartTranferCmd(&recvBuf, &recvLen, debugWorker->buf , debugWorker->len)))
//		{
//			if(recvLen - debugWorker->len == 7)//如果有变量操作，则进行隐射
//			{
//				CDV_INT08U index = g_olCache.len;
//				if(0x05 == recvBuf[index])
//				{
//					ValSet(*(CDV_INT16U*)(recvBuf + index + 1) , *(CDV_INT32S*)(recvBuf + index + 3));
//				}
//			}
//		}
//		

//		DELETE(recvBuf);
	}
#endif
	//CmdArgDelete(&arg);
	return ret;
}


/** @brief  mem脚本运行
  * @param  void
  *         ret   1 ，从0运行
  *               0， 继续运行
						loop 循环，1循环，0不循环
  * @retval 返回值说明OPT_SUCCESS继续执行，OPT_FAILURE停止执行，脚本执行到最后一条
  * @note   2-9号脚本
  */
RET_STATUS RunWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT08U loop) {

	if(OPT_SUCCESS == WorkerCmdParse(debugWorker/*, loop*/)) {		
	    return NextWorkerCmd(debugWorker, loop);
	} else {
		return OPT_SUCCESS;
	}
}
/** @brief  调用工人
  * @param  
  * @retval 返回值说明
  * @note   必须在获取完工人流程后进行
  */
CDV_INT32U time[100] = {0};
CDV_INT08U time_cnt = 0;
CDV_INT08U time_which = 0;
CDV_INT32U time_tmp = 0;
CDV_INT32U time_max = 100;

void RunWorker(THREAD_INFO *pCtrl) {
  CDV_INT08U loop = 0;
  DEBUG_SCRIPT worker = {0, 0, 0, 0, 0, 0, NULL, 0, NULL};
	OS_ERR err;
	CMD_ARG arg;
	CmdArgInit(&arg);
	
	arg.uart = 0xFF;//不使用
	arg.ptrWorker = &worker;
	
	if (OPT_SUCCESS != InitWorker(pCtrl->which, &worker)) {
		pCtrl->status = WORKER_STOP;
    goto runworker_exit;//return;
	}
	
	worker.arg = &arg;
	
	if(1/*0 != pCtrl->which*/) {/*非管理工人*/
		do {
			if(time_which==pCtrl->which && !worker.doNo)
			{
				time[time_cnt] = CalcCount(GET_TICK, time_tmp);
				if(time[time_cnt] > time_max) {//1是0.1ms
					time_tmp = GetCdvTimeTick();
				}
				MAX_SELF_ADD(time_cnt, 100);
				time_tmp = GetCdvTimeTick();
			}
			
			switch (pCtrl->status) {
				case WORKER_LOOP:
					if (worker.doNo == 0) {
						TaskSched();
					}
					loop = 1;
					break;
				case WORKER_ONCE:
					loop = 0;
					break;
				case WORKER_STOP:
					goto runworker_exit;//return;
				case WORKER_PAUSE:
					while (WORKER_PAUSE == pCtrl->status)
						OS_TaskSuspend((OS_TCB*)0,&err);
					break;
				case WORKER_EXIT:
					if (worker.doNo == pCtrl->exitStep) {
						pCtrl->status = WORKER_STOP;
						goto runworker_exit;//return;
					}
					break;
				default:
					break;
			}
			
			switch(pCtrl->opt)
			{
				case 0:
					break;
				case 1:
					ToWorkerCmd(&worker, pCtrl->gotoStep);
				default:
			    pCtrl->opt = 0;
					break;
			}
			pCtrl->step = worker.doNo;
			//if(8 == pCtrl->which && 29 == pCtrl->step )
			//	pCtrl->step = worker.doNo;
		} while (OPT_SUCCESS == RunWorkerCmd (&worker , loop));
	}	else {/*管理工人*/
		do {
			int i = 0;
			int flag = 0;
			switch (pCtrl->status) {
				case WORKER_LOOP:
					
					break;
				case WORKER_ONCE:
					loop = 0;
					break;
				case WORKER_STOP:
					do {
						flag = 0;
						for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*判断工人是否已经运行*/
						{
							if (IsTaskExist(WorkerTaskTCB[i]))
							{
								WorkerControl(g_threadInfo[i].which, WORKER_STOP);
								flag = 1;
							}
						}
					} while(flag);/*等待直到所有工人退出*/
					goto runworker_exit;//return;
				case WORKER_PAUSE:
					
					break;
				case WORKER_EXIT:
					
					break;
				default:
					loop = 1;
					break;
			}
			pCtrl->step = worker.doNo;
		} while (OPT_SUCCESS == RunWorkerCmd (&worker , loop));
		
	}

runworker_exit:
	CmdArgDelete(&arg);
	pCtrl->status = WORKER_STOP;
//	for ( worker.doNo = 0; worker.doNo < worker.cmdNum ;) {
//		if (OPT_FAILURE == RunWorkerCmd(worker.doNo, uartNo)) {

//		} else {
//		   worker.doNo++;
//		}
//	}	
//	return OPT_SUCCESS;
}
void WorkerTask(void *p_arg) {
	
	RunWorker((THREAD_INFO *)p_arg);
}

/** @brief  工人线程分配
  * @param  
  * @retval 返回值说明
  * @note   外部需考虑线程是否在运行，否则会导致该线程运行数据被清除
  */
RET_STATUS WorkerThreadRun(const CDV_INT08U tNo, const CDV_INT08U no, const WORKER_STATUS status) {
	OS_ERR err;
	RET_STATUS ret = OPT_FAILURE;
	ASSERT(tNo < WORKER_MAX_NUM);
  g_threadInfo[tNo].which = no;
	g_threadInfo[tNo].status = status;
	{
	  OSTaskCreate((OS_TCB* )&WorkerTaskTCB[tNo],
				(CPU_CHAR* )workerName[tNo],
				(OS_TASK_PTR )WorkerTask,
				(void* )&g_threadInfo[tNo],
				(OS_PRIO )WORKER_TASK_PRIO,
				(CPU_STK* )&WORKER_TASK_STK[tNo][0],
				(CPU_STK_SIZE )WORKER_STK_SIZE/10,
				(CPU_STK_SIZE )WORKER_STK_SIZE,
				(OS_MSG_QTY )0,
				(OS_TICK )0,
				(void* )0,
				(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
				(OS_ERR* )&err);	
				
		if (err == OS_ERR_NONE)
			ret = OPT_SUCCESS;
	}
	
	return ret;
}



/** @brief  工人控制
  * @param  no 几号工人
  *         status 目标状态，无此状态，返回OPT_FAILURE
  * @retval OPT_FAILURE 切换失败| OPT_SUCCESS 切换成功
  * @note   
  */
RET_STATUS WorkerControl(const CDV_INT08U no, const WORKER_STATUS status) {
	int i = 0;
  OS_ERR err;
	RET_STATUS ret = OPT_FAILURE;
	
	if (status >= WORKER_MAX_STATUS)
		return OPT_FAILURE;
	
	OSSemPend(&WORKER_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
	
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*判断工人是否已经运行*/
	{
		if (no == g_threadInfo[i].which && IsTaskExist(WorkerTaskTCB[i]))
		{
			//g_threadInfo[i].status = status;
			break;
		}
	}
	
	switch (status) {
		case WORKER_STOP:
		case WORKER_PAUSE:
		case WORKER_EXIT:
			if (WORKER_MAX_NUM != i) {                                          /*工人已经在i线程运行*/
				g_threadInfo[i].status = status;
				OSTaskResume((OS_TCB*)&WorkerTaskTCB[i],&err);
			}
			ret = OPT_SUCCESS;
			break;
		case WORKER_ONCE:
		case WORKER_LOOP:
			if (WORKER_MAX_NUM != i) {
				g_threadInfo[i].status = status;
				OSTaskResume((OS_TCB*)&WorkerTaskTCB[i],&err);	
				ret = OPT_SUCCESS;
			} else {
				for( i = 0; i < WORKER_MAX_NUM; i++)                              /*查找空闲线程*/
				{
					if(!IsTaskExist(WorkerTaskTCB[i]))
					{
						ret = WorkerThreadRun(i, no, status);                        /*尝试启动工人*/
						break;
					}
				}
			}
			break;
		default:
			break;
	}
  
  OSSemPost (&WORKER_SEM,OS_OPT_POST_1,&err);
	return ret;
}
/** @brief  重启所有工人
  * @param  
  * @retval 
  * @note   根据g_threadInfo的值恢复工人运行，只适合重启时用
  */
void RestartWorkers(void) {
	CDV_INT08U i;
	RET_STATUS ret = OPT_FAILURE;
	for( i = 0; i < WORKER_MAX_NUM; i++)
	{
		if (WORKER_STOP != g_threadInfo[i].status)
		{
			g_threadInfo[i].gotoStep = g_threadInfo[i].step;
			g_threadInfo[i].opt = 1;
			ret = WorkerThreadRun(i, g_threadInfo[i].which, g_threadInfo[i].status);
			//break;
		}
	}
}

/** @brief  工人退出设置
  * @param  no 几号工人
  *         step 第几步
  * @retval OPT_FAILURE 设置失败| OPT_SUCCESS 设置成功
  * @note   
  */
RET_STATUS WorkerExitSet(CDV_INT08U no, CDV_INT32U step) {
	int i = 0;
	RET_STATUS ret = OPT_FAILURE;
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*判断工人是否已经运行*/
	{
		if (no == g_threadInfo[i].which && IsTaskExist(WorkerTaskTCB[i]))
		{
			g_threadInfo[i].exitStep = step;
			ret = OPT_SUCCESS;
			break;
		}
	}
	return ret;
}
/** @brief  工人跳转设置
  * @param  no 几号工人
  *         step 第几步
  * @retval OPT_FAILURE 设置失败| OPT_SUCCESS 设置成功
  * @note   
  */
RET_STATUS WorkerGotoSet(CDV_INT08U no, CDV_INT32U step) {
	int i = 0;
	RET_STATUS ret = OPT_FAILURE;
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*判断工人是否已经运行*/
	{
		if (no == g_threadInfo[i].which && IsTaskExist(WorkerTaskTCB[i]))
		{
			g_threadInfo[i].gotoStep = step;
			g_threadInfo[i].opt = 1;
			ret = OPT_SUCCESS;
			break;
		}
	}
	return ret;
}
/** @brief  工人状态查询
  * @param  no 几号工人
  *         status 目标状态
  * @retval OPT_FAILURE 查询失败| OPT_SUCCESS 查询成功
  * @note   
  */
RET_STATUS WorkerQuery(const CDV_INT08U no, WORKER_STATUS* pStatus, CDV_INT32U* pStep) {
	int i = 0;
	RET_STATUS ret = OPT_FAILURE;
	ASSERT(pStatus != NULL);
	ASSERT(pStep != NULL);
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*判断工人是否已经运行*/
	{
		if (no == g_threadInfo[i].which)
		{
			*pStatus = g_threadInfo[i].status;
			*pStep = g_threadInfo[i].step;
			ret = OPT_SUCCESS;
			break;
		}
	}
	return ret;
}

/** @brief  工人状态查询
  * @param  no 几号工人
  *         status 目标状态
  * @retval OPT_FAILURE 查询失败| OPT_SUCCESS 查询成功
  * @note   
  */
RET_STATUS WorkerQueryStatus(const CDV_INT08U no, WORKER_STATUS* pStatus) {
	int i = 0;
	RET_STATUS ret = OPT_FAILURE;
	ASSERT(pStatus != NULL);
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*判断工人是否已经运行*/
	{
		if (no == g_threadInfo[i].which)
		{
			*pStatus = g_threadInfo[i].status;
			ret = OPT_SUCCESS;
			break;
		}
	}
	return ret;
}

/** @brief  工人状态查询
  * @param  no 几号工人
  *         status 目标状态
  * @retval OPT_FAILURE 查询失败| OPT_SUCCESS 查询成功
  * @note   
  */
RET_STATUS WorkerQueryStep(const CDV_INT08U no, CDV_INT16U* pStep) {
	int i = 0;
	RET_STATUS ret = OPT_FAILURE;
	ASSERT(pStep != NULL);
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*判断工人是否已经运行*/
	{
		if (no == g_threadInfo[i].which)
		{
			*pStep = g_threadInfo[i].step;
			ret = OPT_SUCCESS;
			break;
		}
	}
	return ret;
}


/** @brief  管理工人控制
  * @param  status 目标状态
  * @retval OPT_FAILURE 切换失败| OPT_SUCCESS 切换成功
  * @note   
  */
RET_STATUS ManagerControl(const WORKER_STATUS status) {
	//return WorkerControl(0, status);
	if(WORKER_STOP != status) //启动
	{	
		WorkerControl(0, WORKER_LOOP);//隐藏
		WorkerControl(1, WORKER_ONCE);//监工
	}
	else
	{
		AllWorkerCtrl(WORKER_STOP);
	}
}

/** @brief  工人状态查询
  * @param  no 几号工人
  *         status 目标状态
  * @retval OPT_FAILURE 查询失败| OPT_SUCCESS 查询成功
  * @note   
  */
RET_STATUS ManagerQueryStatus(WORKER_STATUS* pStatus) {
//	return WorkerQueryStatus(0, pStatus);
	u8 i;
	*pStatus = 0;
  for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*判断工人是否已经运行*/
	{
		if (0 != g_threadInfo[i].status)
		{
			*pStatus = 1;
			
			break;
		}
	}
	return OPT_SUCCESS;
}

void AllWorkerCtrl(const WORKER_STATUS status)
{
	CDV_INT08U i;
	
	for(i = 0; i < g_line.workerAddr.len; i++)
	{
		WorkerControl(i, status);
	}
	
}
/******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
******************************************************************/

/** @brief  工人管理命令
  * @param    
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  * @note   							
  */
RET_STATUS WorkerCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CMD_ARG *arg) {
	OS_ERR err;
	CDV_INT08U opt, type;
	CDV_INT32U no, par;
	CDV_INT32S num;
	RET_STATUS ret =OPT_SUCCESS;
	
	no   = *(CDV_INT32U*)(rxBuf);/*工人代号*/
	opt  = rxBuf[4];/**/
	type = rxBuf[5];
	par  = *(CDV_INT32U*)(rxBuf + 6);
  
	switch(opt) {
		case 0x01:/*运行*/
			ret = WorkerControl(no, WORKER_ONCE);
			break;
		case 0x04:/*退出*/
			ret = WorkerControl(no, WORKER_EXIT);
			break;
		case 0x03:/*暂停*/
			ret = WorkerControl(no, WORKER_PAUSE);
			break;
		case 0x00:/*急停*/
			ret = WorkerControl(no, WORKER_STOP);
			break;
		case 0x02:/*循环运行*/
			ret = WorkerControl(no, WORKER_LOOP);
			break;
		case 0x05://状态查询
			switch(type) {
				case 0xED:
					ret = WorkerQueryStatus(no, (WORKER_STATUS*)&num);
				  ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&num), 4, arg, RC_CRC);
					break;
				case 0xFE:
					ret = WorkerQueryStatus(no, (WORKER_STATUS*)&num);
					ValSet(par, num);
					break;
				default:
					break;
			}
			break;
		case 0x06:
			switch(type) {
				case 0xED:
					ret = WorkerGotoSet(no, 0);
					break;
				default:
					break;
			}
			break;
		case 0xFF://状态查询
			ret = WorkerQueryStatus(no, (WORKER_STATUS*)&num);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&num), 4, arg,RC_CRC);
			break;
		default:
			break;
	}
	
	return ret;
}

/** @brief  工人管理命令
  * @param    
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  * @note   							
  */
RET_STATUS LineCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) {
//	OS_ERR err;
//	CDV_INT08U opt/* , type , strLen*/;
//	CDV_INT32S num;
//	RET_STATUS ret =OPT_SUCCESS;
//	
//	//no   = 0;/*工人代号*/
//	opt  = rxBuf[4];/**/
//	//type = rxBuf[2];
//  
//	switch(opt) {
//		case 0x00:/*运行*/
//			ret = ManagerControl(WORKER_ONCE);
//			break;
//		case 0x01:/*退出*/
//			ret = ManagerControl(WORKER_EXIT);
//			break;
//		case 0x02:/*暂停*/
//			ret = ManagerControl(WORKER_PAUSE);
//			break;
//		case 0x03:/*急停*/
//			ret = ManagerControl(WORKER_STOP);
//			break;
//		case 0x04:/*循环运行*/
//			ret = ManagerControl(WORKER_LOOP);
//			break;
//		case 0x05:/*工人状态读到变量*/
//			ret = ManagerQueryStatus((WORKER_STATUS*)&num);
//		  ValSet(rxBuf[3], num);
//			break;
//		default:
//			break;
//		
//	}
//	
//	return ret;
	///////////////////////////////////////////////////
	OS_ERR err;
	CDV_INT08U opt, type;
	CDV_INT32U par;
	CDV_INT32S num;
	RET_STATUS ret =OPT_SUCCESS;
	
	opt  = rxBuf[4];/**/
	type = rxBuf[5];
	par  = *(CDV_INT32U*)(rxBuf + 6);
  
	switch(opt) {
		case 0x00://状态切换
			switch(par) {
				case 0x01:/*运行*/
					ret = ManagerControl(WORKER_ONCE);
					break;
				case 0x04:/*退出*/
					ret = ManagerControl(WORKER_EXIT);
					break;
				case 0x03:/*暂停*/
					ret = ManagerControl(WORKER_PAUSE);
					break;
				case 0x00:/*急停*/
					ret = ManagerControl(WORKER_STOP);
					break;
				case 0x02:/*循环运行*/
					ret = ManagerControl(WORKER_LOOP);
					break;
				default:
					break;
			}
			break;
		case 0x01://状态查询
			switch(type) {
				case 0xED:
					ret = ManagerQueryStatus((WORKER_STATUS*)&num);
				  ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&num), 4, arg, RC_CRC);
					break;
				case 0xFE:
					ret = ManagerQueryStatus((WORKER_STATUS*)&num);
					ValSet(par, num);
					break;
				default:
					break;
			}
			break;
		case 0x02://重启
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)NULL, 0, arg, RC_CRC);
			delay_ms(500);
		  ResetCdv();
			break;
		default:
			break;
	}
	
	return ret;
}


CDV_INT32S WorkerRead(CDV_INT08U no) {
	CDV_INT32S num = WORKER_STOP;
	RET_STATUS ret = WorkerQueryStatus(no, (WORKER_STATUS*)&num);
}
