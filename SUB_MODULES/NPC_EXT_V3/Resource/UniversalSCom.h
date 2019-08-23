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

#include "_include.h"

typedef enum{
	CHECK_CRC, 
	
}CHECK_MODE;

void ClearUniversalCnt(void);
RET_STATUS UniSerialModbusParse(u8* buf, u8 len, u8 uart, CMD_ARG *arg);
RET_STATUS UniSerialComSetCmd(u8* rxBuf, u8 rxLen, CMD_ARG *arg);
RET_STATUS UniSerialSendCRC(u8* txBuf, const u8 txLen,u8** rxBuf ,u16* rxLen , const u8 uart,BUF_OPT opt);
int UniSerialSendCRC2(u8* txBuf, const u8 txLen,u8** rxBuf ,u16* rxLen , const u8 uart,BUF_OPT opt);
void ResRequest(u8* rxBuf, u16 rxLen , u8* para, u16 paraLen, CMD_ARG *arg, REQUEST_CTRL ctrl);
void AddTxPlus(u8* txBuf, u16 txLen, CMD_ARG *arg) ;
#endif

