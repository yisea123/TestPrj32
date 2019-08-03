/**
  ******************************************************************************
  * @file    /uart2.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2019-8-2
  * @brief   a package of uart2 CONFIG
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2019 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _UART_2_
#define  _UART_2_


#include "_include.h"



//extern QUEUE uart2_queue;
////////////////////////////////////////////

void USART2_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity);

/*外设基础函数*/
//int  DMA_usart2Send(u32  mar,u16  ndtr);
int USART2_Send(u8 *buf,u16 len);
int USART2_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen);

#endif

