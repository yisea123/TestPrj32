
/**
  ******************************************************************************
  * @file    /syncUart.h 
  * @author  MMY
  * @version V0.0.1
  * @date    04-06-2016
  * @brief   internal implement and interface of new uart control of sync
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _SYNC_UART_
#define  _SYNC_UART_

//#define SYNC_UART_DRIVE 1u

#include "cdv_include.h"

//保存缓存的结构
typedef struct tagCache{
	u8 flag;
	u8 len;
	u8 *buf;
}Cache;

//保存事件串口级级传输的的信息
typedef struct tagUart{
	u8 no;
}Uart;



//加头尾信息发送，返回表示，发送完成
u8 CmdSend(Cache* send, Uart uart);
//加头尾信息发送，返回表示，回复完成
u8 CmdSendReq(Cache* send, Cache* recv, Uart uart);
//不带头尾crc的发送
u8 BufSend(Cache* send, Uart uart);

//接收线程专用
void ThreadCmdRecv(void);


#endif

