
/**
  ******************************************************************************
  * @file    /uartx.h 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-4-1
  * @brief   a package of uartx
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _UART_X_
#define  _UART_X_


#include "cdv_include.h" 
struct COM_PARAM {
	u32 bound;
	u32 wordLength;
	u32 stopBits;
	u32 parity;
};
#define USART_SET_ADDR(n) (USART_ADDR + (n) * 16)

void USART_Configuration(void);
RET_STATUS CorrectComParam(struct COM_PARAM* param, u8 uartNo);
/*外设基础函数*/

void USARTSend(u8 *buf ,u16 len ,u8 no);
void USARTTR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen ,u8 no);
void USARTx_IRQHandler(USART_TypeDef* USARTx);
void USARTSet(u32 bound, u16 wordLength, u16 stopBits, u16 parity, u8 no);
RET_STATUS ReturnComParam(struct COM_PARAM* param, u8 uartNo);
#endif

