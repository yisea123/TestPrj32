
/**
  ******************************************************************************
  * @file    /Prompt.h 
  * @author  MMY
  * @version V0.0.1
  * @date    2-18-2016
  * @brief   a package of prompt function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _PROMPT_
#define  _PROMPT_


#include "cdv_include.h" 

typedef struct{

	CDV_INT08U over;         /*结束*/
	CDV_INT08U varNo;        /*保存到变量*/
	CDV_INT08U no;           /*窗口号*/
	CDV_INT08U uartNo;
} PROMPT_INFO;

RET_STATUS PromptCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo);
RET_STATUS PromptParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen);
void Ini_Cmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo);
#endif

