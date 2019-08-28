/**
  ******************************************************************************
  * @file    /uart1.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2019-8-2
  * @brief   a package of uart1 CONFIG
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2019 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _UART_1_
#define  _UART_1_


#include "_include.h"



//extern QUEUE uart1_queue;
////////////////////////////////////////////

void USART1_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity);

/*外设基础函数*/
//int  DMA_usart2Send(u32  mar,u16  ndtr);
int USART1_Send(u8 *buf,u16 len);
int USART1_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen);
int USART1_RT(int (*p_cmd)(u8 *,u16  ,u8** ,u16* ));
#endif

