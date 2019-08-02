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

#define FLASH_WORK_ADDR g_line.workAddr                       //�ڴ��й��˵ĵ�ַ
#if 0
#define RUN_IN_MEM 1                                          //�����ڴ����й���ģʽ

	
	
extern	CDV_INT08U *OfflineScript;
//extern u16 timeMS[20][200];/*��ʱʱ��*/
//extern u8 timeEn;/*���ӹ���*/
/*
 *���˽ű���ض���
 */
#if 1/*����flash�������нű�*/


#ifndef VIRTUAL_LOGIC
typedef struct {		
	CDV_INT16U howMany;                                 /*���ٸ�����Ҫ����*/
	CDV_INT32U startPos[SCRIP_MAX_RUN];                 /*ÿ�����˽ű���ʼλ��*/
	CDV_INT16U cmdNum[SCRIP_MAX_RUN];					          /*ÿ���ű���������*/
	CDV_INT16U *cmdPos[SCRIP_MAX_RUN];                  /*ÿ���ű���ÿ�������flash��ʼ��ַ��������ȡflash�еĸ�������*/
	CDV_INT16U doNo[SCRIP_MAX_RUN];                     /*�ű�������ţ����ۼӣ�����ű��д洢����Ž��жԱȼ��*/	
	CDV_INT08U *buf[SCRIP_MAX_RUN];                     /*���˽ű����棬һ��*/
	CDV_INT08U len[SCRIP_MAX_RUN];                      /*���˽ű����泤�ȣ�һ��*/
} RUN_SCRIPT;
#else
typedef struct {		
	CDV_INT16U howMany;                                 /*���ٸ�����Ҫ����*/
	CDV_INT32U startPos[SCRIP_MAX_RUN+1];               /*ÿ�����˽ű���ʼλ��*/		
	CDV_INT16U cmdNum[SCRIP_MAX_RUN+1];					        /*ÿ���ű���������*/
	CDV_INT32U cmdPos[SCRIP_MAX_RUN+1];                 /*ÿ���ű��ĵ�ǰ�����flash��ʼ��ַ��������ȡflash�еĸ�������*/
	CDV_INT16U doNo[SCRIP_MAX_RUN+1];                   /*�ű�������ţ����ۼӣ�����ű��д洢����Ž��жԱȼ��*/	
	CDV_INT16U totalLen[SCRIP_MAX_RUN+1];               /*�ű��ܳ���*/
	CDV_INT08U *buf[SCRIP_MAX_RUN+1];                   /*���˽ű����棬һ��*/
	CDV_INT08U len[SCRIP_MAX_RUN+1];                    /*���˽ű����泤�ȣ�һ��*/
//	CDV_INT08U which[SCRIP_MAX_RUN+1];                  /*�ĸ�����*/
//	WORKER_STATUS status[SCRIP_MAX_RUN+1];              /*�ĸ�״̬*/
} RUN_SCRIPT;
#endif

extern RUN_SCRIPT g_run;


typedef struct {
	CDV_INT08U flag;                                    /*�䷽����ָʾ��ǰ�������䷽*/
	ARRAY_U32  infPos;                                  /*ÿ��������Ϣ�Ĵ洢λ��*/
} WORKER_INFO;//���湤�˵���Ϣ

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
//ʹ����ǿת
extern CDV_INT08U *OfflineScript;
void Mem_Read_Ptr(void** ReadPtr, u32 ReadAddr);//readptr��ָ��
#define SCRIPT_GETADDR(addr) (void*)(OfflineScript + addr)
RET_STATUS LineInit(void);
#endif
