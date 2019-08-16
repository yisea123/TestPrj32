
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
//	CDV_INT08U *buf;                                /*���˽ű����棬������ʱ��������ȡһ��*/
//	CDV_INT08U len;
//	CDV_INT16U *pos;                                /*�����������������������ű��е����λ�ã����������п��ٶ�λ*/
//	CDV_INT16U num;                                 /*��������λ����������*/
//	CDV_INT32U flashAddr;														/*flashPos��ʼ��ַ*/
//} VAL_SCRIPT;

typedef struct {
	//CDV_INT32U flashAddr;														/*flashPos��ʼ��ַ*/
	ARRAY_U32  no;                                  /*����ÿ�������ĺ�*/
	ARRAY_U32  pos;                                 /*����ÿ�������������еĵ�ַ*/
} VAL_SCRIPT;

extern VAL_SCRIPT g_val, g_formulaVal;

RET_STATUS AppValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen);
RET_STATUS Formula_Cmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo);
RET_STATUS InitValCmdPos(void);
//RET_STATUS InitFormulaValCmdPos(VAL_SCRIPT *val , CDV_INT16U cmdNum , CDV_INT32U flashAddr , CDV_INT16U len);


#endif

