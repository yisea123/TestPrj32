/**
  ******************************************************************************
  * @file    /UniversalSCom.h 
  * @author  MMY
  * @version V1.0.1
  * @date    2017-4-5
  * @brief   
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _UNIVERSAL_SERIAL_COM_
#define  _UNIVERSAL_SERIAL_COM_

#include "cdv_include.h"

typedef enum{
	CHECK_CRC, 
	
}CHECK_MODE;

void ClearUniversalCnt(void);
RET_STATUS UniSerialModbusParse(CDV_INT08U* buf, CDV_INT08U len, CDV_INT08U uart, CMD_ARG *arg);
RET_STATUS UniSerialComSetCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
RET_STATUS UniSerialSendCRC(u8* txBuf, const u8 txLen,u8* rxBuf ,u8 rxbufLen,u8* rxLen , const CDV_INT08U uart,BUF_OPT opt);
#endif

