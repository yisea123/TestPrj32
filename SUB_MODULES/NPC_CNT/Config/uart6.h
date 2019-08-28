
/**
  ******************************************************************************
  * @file    /uart6.h 
  * @author  MMY
  * @version V0.0.1
  * @date    30-11-2015
  * @brief   a package of uart communicate with kfc and  yyc
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _UART_6_
#define  _UART_6_


#include "_include.h"

void USART6_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity);

/*½Ó¿Úº¯Êý*/
void DMA_usart6Send(u32 mar,u16 ndtr);
void USART6_Send(u8 *buf,u16 len);
void USART6_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen);

#endif

