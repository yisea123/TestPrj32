
/**
  ******************************************************************************
  * @file    /uart3.h 
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




#ifndef  _UART_3_
#define  _UART_3_


#include "cdv_include.h"
extern u8 *g_pUsart3RxBuf;
extern u8 g_Usart3RxLen;


void USART3_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity);

/*½Ó¿Úº¯Êý*/
void DMA_usart3Send(CDV_INT32U mar,CDV_INT16U ndtr);
void USART3_Send(u8 *buf,u16 len);
void USART3_Receive_Data(u8 *buf,u32 *len);
void USART3_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen);

u8 GetUsartRxLen(void);
void SetUsartRxLen(u8 len);
#endif

