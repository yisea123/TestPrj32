
/**
  ******************************************************************************
  * @file    /uart2.h 
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




#ifndef  _UART_2_
#define  _UART_2_


#include "_include.h"

/*
 *串口queue相关定义
 */
#define MAX_VAL_ADD(a,max) (((a)<(max)-1)?1+(a):0)
#define MAX_SELF_ADD(a,max) (a) = ((a)<(max)-1)?1+(a):0
#define MAX_SELF_SUB(a,max) (a) = ((a)==0)?(max)-1:(a)-1

#define QUEUE_LENGTH 3     /*串口接收队列长度*/
#define QUEUE_BUF_LENGTH 300    /*串口接收长度*/

#define QUEUE_ING_NEXT(Q) do{\
	if(0 == Q.len[Q.ingPos])break;\
	(Q.doPos!=MAX_VAL_ADD(Q.ingPos,QUEUE_LENGTH))?MAX_SELF_ADD(Q.ingPos,QUEUE_LENGTH):NULL;\
	Q.len[Q.ingPos]=0;\
}while(0);

#define QUEUE_ING_ADD_CHAR(Q,ch) do{\
	Q.buf[Q.ingPos][Q.len[Q.ingPos]]=(u8)(ch);\
	Q.len[Q.ingPos] = Q.len[Q.ingPos]<QUEUE_BUF_LENGTH-1?1+Q.len[Q.ingPos]:QUEUE_BUF_LENGTH-1;\
}while(0);

#define QUEUE_HAD(Q)  (Q.ingPos!=Q.doPos) /*&& 0 != Q.len[Q.doPos]*/

#define QUEUE_CLEAR(Q)  (Q.doPos=Q.ingPos)

#define QUEUE_DO_NEXT(Q) do{\
	(Q.ingPos!=Q.doPos) ? MAX_SELF_ADD(Q.doPos,QUEUE_LENGTH) : NULL;\
}while(0);

#define QUEUE_DO_BUF(Q) Q.buf[Q.doPos]

#define QUEUE_DO_LEN(Q) Q.len[Q.doPos]

#define QUEUE_ING_BUF(Q) Q.buf[Q.ingPos]

#define QUEUE_ING_LEN(Q) Q.len[Q.ingPos]

typedef struct {
	u8 buf[QUEUE_LENGTH][QUEUE_BUF_LENGTH];         /*接收队列*/
	u16 len[QUEUE_LENGTH];              /*接收长度*/	
  u8 ingPos;              /*数组正在写入位置，ingpos+1==dopos时说明满了*/	
	u8 doPos;              /*数组待处理位置，dopos ！= ingpos时表示有数据*/	
} QUEUE;

extern QUEUE uart2_queue;
////////////////////////////////////////////

void USART2_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity);

/*外设基础函数*/
int  DMA_usart2Send(u32  mar,u16  ndtr);
int USART2_Send(u8 *buf,u16 len);
int USART2_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen);

#endif

