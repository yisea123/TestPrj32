
/**
  ******************************************************************************
  * @file    /formula.h 
  * @author  MMY
  * @version V0.0.1
  * @date    04-06-2016
  * @brief   internal implement and interface of new uart control of formula
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _FORMULA_
#define  _FORMULA_

#include "cdv_include.h"
//typedef struct {
//	CDV_INT08U *buf;                                /*工人脚本缓存，分析的时候用于提取一条*/
//	CDV_INT08U len;
//	CDV_INT16U *pos;                                /*保存启动设置命令在启动脚本中的相对位置，方便运行中快速定位*/
//	CDV_INT16U num;                                 /*保存上面位置数组总数*/
//	CDV_INT32U flashAddr;														/*flashPos起始地址*/
//} VAL_SCRIPT;

typedef struct {
	//CDV_INT32U flashAddr;														/*flashPos起始地址*/
	ARRAY_U32  no;                                  /*保存每个变量的号*/
	ARRAY_U32  pos;                                 /*保存每个变量的配置中的地址*/
} VAL_SCRIPT;

extern VAL_SCRIPT g_val, g_formulaVal;

RET_STATUS AppValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen);
RET_STATUS Formula_Cmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo);
RET_STATUS InitValCmdPos(void);
//RET_STATUS InitFormulaValCmdPos(VAL_SCRIPT *val , CDV_INT16U cmdNum , CDV_INT32U flashAddr , CDV_INT16U len);


#endif

