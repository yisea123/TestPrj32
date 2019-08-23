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

//CDV_INT08U g_whichUart; //指示串口线程中当前命令是从哪个串口发过来的
/**/
OS_SEM TX_SEM, MOTO_SEM,MEM_SEM;
OS_SEM GENERAL_SERIAL_SEM;
OS_SEM TCP_TX_SEM;

	//TEST
double RPressureData[100] = {0};
int RPressureCnt = 0;

void CmdArgDelete(CMD_ARG *arg)
{
	DELETE(arg->reqbuf);
	DELETE(arg->specbuf);
	INIT_CLEAR(*arg);
}
