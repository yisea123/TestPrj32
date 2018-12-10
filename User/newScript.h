
/**
  ******************************************************************************
  * @file    /newScript.h 
  * @author  MMY
  * @version V0.0.1
  * @date    04-06-2016
  * @brief   internal implement and interface of new uart control of newScript
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _NEW_SCRIPT_
#define  _NEW_SCRIPT_

#include "cdv_include.h"

#define __LINE_LEN					 SCRIP_LINE_ADDR - 16
#define __LINE_ADDR          SCRIP_LINE_ADDR
#define LINE_MAX_LEN         SCRIP_LINE_LEN              //生产线大小
#define SCRIP_NUM_RUN        g_line.workerAddr.len       //脚本运行数量
#define SCRIP_NAME_LEN(i)    GetConfigAddr(i) + 2        //脚本名称长度地址
#define SCRIP_LINE_NAME      g_line.addr+4               //生产线名称
#define SCRIP_YYC            g_line.endAddr+4            //应用层脚本包起始地址
#define SCRIP_NAME(i)        GetConfigAddr(i) + 3        //脚本名称存储地址
#define SCRIP_LEN(i)         GetWorkerAddr(i)						 //脚本长度
#define SCRIPADDR(i)         GetWorkerAddr(i) + 2        //脚本地址
#define SCRIP_TYPE(i)        GetConfigAddr(i) - 1        //脚本类型
#define SCRIP_RES(i)         GetResAddr(i)               //获得资源脚本
#define SCRIP_ALARM					 g_line.AlarmAddr            //报警信息脚本
#define SCRIP_MAP					   g_line.mapAddr            	 //映射信息脚本
#define INI_LEN_ADDR(i)      GetResAddr0(0x0A, 0x00, 0xFF, 3)       //INI flash长度存储地址
#define INI_ADDR(i)          GetResAddr0(0x0A, 0x00, 0xFF, 3) + 2   //INI flash存储地址



typedef struct {
	CDV_INT08U init;                                       /*是否已经初始化*/
	CDV_INT08U no;                                         /*几号生产线*/
	CDV_INT32U addr;                                       /*当前生产线起始地址*/
	CDV_INT32U endAddr;                                    /*当前生产线结束地址*/
	CDV_INT32U resAddr;                                    /*当前生产线资源地址*/
	CDV_INT32U mapAddr;                                    /*当前生产线映射地址*/
	CDV_INT32U AlarmAddr;                                  /*当前生产线报警地址*/
	CDV_INT32U workAddr;
	ARRAY_U32  configAddr;                                 /*保存每个工人配置的地址*/
	ARRAY_U32  workerAddr;                                 /*保存每个工人的地址*/
} LINE_ADDR_INFO;//保存生产线的信息flash存储地址

extern LINE_ADDR_INFO g_line;

CDV_INT32U GetWorkerAddr(const CDV_INT08U workerNo);
CDV_INT32U GetConfigAddr(const CDV_INT08U workerNo);
CDV_INT32U GetResAddr(const CDV_INT08U resType);
CDV_INT32U GetResAddr0(const CDV_INT08U resType, const CDV_INT08U resNo, const CDV_INT08U resPar, const CDV_INT08U ctrl);
//CDV_INT32U GetResAddr(const CDV_INT08U resType, const CDV_INT08U resNo, const CDV_INT08U resPar, const CDV_INT08U ctrl);
RET_STATUS GetLineInfo(const CDV_INT08U no);


void Flash_Send(const CDV_INT32U addr , const CDV_INT32U len, CDV_INT08U uartNo);
RET_STATUS GetLineVersion(CDV_INT08U** buf, CDV_INT32U *len);
RET_STATUS GetYYCVersion(CDV_INT08U** buf, CDV_INT32U *len);
#endif


