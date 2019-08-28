
/**
  ******************************************************************************
  * @file    /uart5.h 
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




#ifndef  _UART_5_
#define  _UART_5_


#include "cdv_include.h"

void UART5_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity);

/*½Ó¿Úº¯Êý*/
void DMA_usart5Send(CDV_INT32U mar,CDV_INT16U ndtr);
void UART5_Send(u8 *buf,u16 len);
void UART5_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen);
#endif

