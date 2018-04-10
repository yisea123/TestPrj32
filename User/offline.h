/**
  ******************************************************************************
  * @file    /cdv_offline.h 
  * @author  MMY
  * @version V2.0.0
  * @date    8-July-2015
  * @brief   online function and var and 
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CDV_OFFLINE_
#define  _CDV_OFFLINE_


#include "cdv_include.h"

#define FLASH_WORK_ADDR g_line.workAddr                       //内存中工人的地址
#if 0
#define RUN_IN_MEM 1                                          //启动内存运行工人模式

	
	
extern	CDV_INT08U *OfflineScript;
//extern u16 timeMS[20][200];/*临时时间*/
//extern u8 timeEn;/*监视工人*/
/*
 *工人脚本相关定义
 */
#if 1/*采用flash机制运行脚本*/


#ifndef VIRTUAL_LOGIC
typedef struct {		
	CDV_INT16U howMany;                                 /*多少个工人要运行*/
	CDV_INT32U startPos[SCRIP_MAX_RUN];                 /*每个工人脚本起始位置*/
	CDV_INT16U cmdNum[SCRIP_MAX_RUN];					          /*每个脚本命令总数*/
	CDV_INT16U *cmdPos[SCRIP_MAX_RUN];                  /*每个脚本的每条命令的flash起始地址，用于提取flash中的各条命令*/
	CDV_INT16U doNo[SCRIP_MAX_RUN];                     /*脚本处理序号，自累加，可与脚本中存储的序号进行对比检错*/	
	CDV_INT08U *buf[SCRIP_MAX_RUN];                     /*工人脚本缓存，一条*/
	CDV_INT08U len[SCRIP_MAX_RUN];                      /*工人脚本缓存长度，一条*/
} RUN_SCRIPT;
#else
typedef struct {		
	CDV_INT16U howMany;                                 /*多少个工人要运行*/
	CDV_INT32U startPos[SCRIP_MAX_RUN+1];               /*每个工人脚本起始位置*/		
	CDV_INT16U cmdNum[SCRIP_MAX_RUN+1];					        /*每个脚本命令总数*/
	CDV_INT32U cmdPos[SCRIP_MAX_RUN+1];                 /*每个脚本的当前命令的flash起始地址，用于提取flash中的各条命令*/
	CDV_INT16U doNo[SCRIP_MAX_RUN+1];                   /*脚本处理序号，自累加，可与脚本中存储的序号进行对比检错*/	
	CDV_INT16U totalLen[SCRIP_MAX_RUN+1];               /*脚本总长度*/
	CDV_INT08U *buf[SCRIP_MAX_RUN+1];                   /*工人脚本缓存，一条*/
	CDV_INT08U len[SCRIP_MAX_RUN+1];                    /*工人脚本缓存长度，一条*/
//	CDV_INT08U which[SCRIP_MAX_RUN+1];                  /*哪个工人*/
//	WORKER_STATUS status[SCRIP_MAX_RUN+1];              /*哪个状态*/
} RUN_SCRIPT;
#endif

extern RUN_SCRIPT g_run;


typedef struct {
	CDV_INT08U flag;                                    /*配方工人指示当前的运行配方*/
	ARRAY_U32  infPos;                                  /*每个工人信息的存储位置*/
} WORKER_INFO;//保存工人的信息

extern WORKER_INFO g_exitInf, g_startInf, g_runInf, g_debugInf, g_formulaInf;

RET_STATUS AppParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen);

//void OfflineRequest(CDV_INT08U no,CDV_INT08U errNo);

void RunClear(void);
RET_STATUS RunInit(void);
RET_STATUS StartScript(CDV_INT08U uartNo);
RET_STATUS EndScript(CDV_INT08U uartNo);
RET_STATUS ToScriptCmd(CDV_INT08U scriptNo , CDV_INT16U cmdNo);
CDV_INT32U GetCmdPos(CDV_INT16U no , CDV_INT16U cmdNum , CDV_INT32U flashAddr);
CDV_INT16U GetNoFromPos(CDV_INT32U startPos , CDV_INT32U currentPos);
CDV_INT16U TestCmdNum(CDV_INT32U flashAddr , CDV_INT16U len);
void CopyCmdLen(CDV_INT32U flashAddr , CDV_INT16U len);
RET_STATUS NextScriptCmd(CDV_INT08U scriptNo , CDV_INT08U loop);

CDV_INT32U GetNextCmdPos(CDV_INT32U flashAddr);
RET_STATUS RunScript(CDV_INT08U scriptNo , CDV_INT08U loop, CDV_INT08U uartNo);
RET_STATUS InitCmdPos(CDV_INT16U *cmdPos, CDV_INT16U cmdNum , CDV_INT32U flashAddr , CDV_INT16U len);

RET_STATUS ScriptCmdParse(CDV_INT08U scriptNo, CDV_INT08U debug, CDV_INT08U uartNo);
RET_STATUS ReadScriptBuf(CDV_INT08U scriptNo) ;

#endif
RET_STATUS Mem_ToScriptCmd(CDV_INT08U scriptNo , CDV_INT16U cmdNo);
#endif


CDV_INT16U Mem_TestCmdNum(CDV_INT32U flashAddr , CDV_INT16U len) ;
CDV_INT32U Mem_GetNextCmdPos(CDV_INT32U flashAddr);
CDV_INT16U Mem_GetNoFromPos(CDV_INT32U startPos , CDV_INT32U currentPos);
CDV_INT32U Mem_GetCmdPos(CDV_INT16U no , CDV_INT16U cmdNum , CDV_INT32U flashAddr) ;
void Mem_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
u8* Mem_Ptr(u32 offsetAddr);
//使用请强转
extern CDV_INT08U *OfflineScript;
void Mem_Read_Ptr(void** ReadPtr, u32 ReadAddr);//readptr是指针
#define SCRIPT_GETADDR(addr) (void*)(OfflineScript + addr)
RET_STATUS LineInit(void);
#endif
