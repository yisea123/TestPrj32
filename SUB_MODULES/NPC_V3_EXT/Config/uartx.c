/**
  ******************************************************************************
  * @file    /uartx.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-4-1
  * @brief   a package of uartx
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "uartx.h"
	#include "uart1.h"
	#include "uart2.h"
//	#include "uart3.h"
//	#include "uart4.h"
//	#include "uart5.h"
	#include "uart6.h"

/*USART设置*/
void USART_ConfigNull(u32 bound, u16 wordLength, u16 stopBits, u16 parity){
	
}
//TODO 不能用null，用空函数
void (*p_UartSet[])(u32 , u16 , u16 , u16) =
{
	
	USART_ConfigNull,
	USART2_Configuration,
	USART_ConfigNull,
	USART_ConfigNull,
	USART_ConfigNull,
	USART_ConfigNull,

};

/** @brief  校正串口配置到stm32专用配置
  * @param  
  * @retval OPT_FAILURE 参数不适配
  * @note   此配置是stm32专用的，参考
位数	奇偶检验	停止位
 7  	奇或偶  	0.5、1、1.5、2
 8  	无检验  	0.5、1、1.5、2
 8  	奇或偶  	1
 9  	无检验  	1
UART4、UART5不支持0.5和1.5位的停止位
  */
RET_STATUS CorrectComParam(struct COM_PARAM* param, u8 uartNo)
{ 
	RET_STATUS ret = OPT_SUCCESS;
	
	ASSERT(param);
	
  if(!IS_USART_BAUDRATE(param->bound))
	{
		ret = OPT_FAILURE;
	}
	
	#define EVEN_AND_ODD(PARITY) (((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

	switch (param->wordLength)
	{
		case 7:
			param->wordLength = EVEN_AND_ODD(param->parity) ? USART_WordLength_8b : 0xFFFF;
			break;
		case 8:
			param->wordLength = EVEN_AND_ODD(param->parity) ? USART_WordLength_9b : USART_WordLength_8b;
			break;
		case 9:
			param->wordLength = EVEN_AND_ODD(param->parity) ? 0xFFFF : USART_WordLength_9b;
			break;
		default:
			//ret = OPT_FAILURE;
			break;
	}
	
	switch (param->wordLength)
	{
		case USART_WordLength_8b:
			
			break;
		case USART_WordLength_9b:
			if (param->stopBits != USART_StopBits_1)
			{
				ret = OPT_FAILURE;
			}
			break;
		default:
			ret = OPT_FAILURE;
			break;
	}
	
	if (4 == uartNo || 5 == uartNo)
	{
		if (param->stopBits == USART_StopBits_0_5 
			|| param->stopBits == USART_StopBits_1_5)
		{
			ret = OPT_FAILURE;
		}
	}
	
	return ret;
}

/** @brief  开发层串口查询
  * @param  
  * @retval OPT_FAILURE 参数不适配
  * @note   
  */
RET_STATUS ReturnComParam(struct COM_PARAM* param, u8 uartNo)
{ 
	RET_STATUS ret = OPT_SUCCESS;
	
	ASSERT(param);
	
  if(!IS_USART_BAUDRATE(param->bound))
	{
		ret = OPT_FAILURE;
	}
	
	#define EVEN_AND_ODD(PARITY) (((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

	switch (param->wordLength)
	{
		case USART_WordLength_8b:
			param->wordLength = EVEN_AND_ODD(param->parity) ? 7 : 8;
			break;
		case USART_WordLength_9b:
			if (param->stopBits != USART_StopBits_1)
			{
				ret = OPT_FAILURE;
			}
			param->wordLength = EVEN_AND_ODD(param->parity) ? 8 : 9;
			break;
		default:
			ret = OPT_FAILURE;
			break;
	}
	
	if (4 == uartNo || 5 == uartNo)
	{
		if (param->stopBits == USART_StopBits_0_5 
			|| param->stopBits == USART_StopBits_1_5)
		{
			ret = OPT_FAILURE;
		}
	}
	
	return ret;
}

void USART_Configuration(void){
//	int i = 0;
//	struct COM_PARAM comPar;
//	
//	for (i = 0; i < 6; i++)
//	{
//		Org_Flash_Read((u8 *)&comPar, USART_SET_ADDR(i), sizeof(comPar));
//		if(p_UartSet[i]) {
//			if(OPT_SUCCESS == CorrectComParam(&comPar, i))
//				p_UartSet[i](comPar.bound, comPar.wordLength, comPar.stopBits, comPar.parity);
//			else
//				p_UartSet[i](115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//		}
//	}
  USART1_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
	USART2_Configuration(460800, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART3_Configuration(460800, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	UART4_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	UART5_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART6_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);

}
/**
  * @brief  串口设置主函数
  * @param  buf
	*					len
	*					no :1 - 6
  * @retval void
  * @note   串口设置统一入口
  */
void USARTSet(u32 bound, u16 wordLength, u16 stopBits, u16 parity, u8 no) {
	struct COM_PARAM comPar;
	ASSERT(0 < no && 6 >= no);
  ASSERT(IS_USART_BAUDRATE(bound));  
  ASSERT(IS_USART_WORD_LENGTH(wordLength));
  ASSERT(IS_USART_STOPBITS(stopBits));
  ASSERT(IS_USART_PARITY(parity));
	
	if(!p_UartSet[no - 1])
		return;
		
	p_UartSet[no-1](bound, wordLength, stopBits, parity);
	
	comPar.bound = bound;
	comPar.wordLength = wordLength;
	comPar.stopBits = stopBits;
	comPar.parity = parity;
	//SPI_Flash_Write((u8 *)&comPar, USART_SET_ADDR(no - 1), sizeof(comPar));
}


int USART_SendNull(u8 *buf,u16 len){return -1;}

int (*p_UartSend[])(u8* ,u16) =
{

	USART_SendNull,
	USART2_Send,
	USART_SendNull,
	USART_SendNull,
	USART_SendNull,
	USART_SendNull,

};

	

/**
  * @brief  串口发送主函数
  * @param  buf
	*					len
	*					no :1 - 6
  * @retval void
  * @note   串口发送统一入口
  */
int USARTSend(u8 *buf ,u16 len ,u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartSend[no - 1])
		return -1;
	return p_UartSend[no-1](buf ,len);
	
}
int USART_TRNull(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen){}
int (*p_UartTR[])(u8 *,u16  ,u8* ,u16* ) =
{
	USART_TRNull,
	USART2_TR,
	USART_TRNull,
	USART_TRNull,
	USART_TRNull,
	USART_TRNull,

};

	

/**
  * @brief  串口发送接收主函数
  * @param  txbuf  要发送的字符串
  *         txlen  要发送的字符串长度
  *         rxbuf  接收的字符串缓存
  *         rxbufLen 接收字符串缓存的长度
  *         rxlen  接收到的字符串长度
	*					no :1 - 6操作的串口号
  * @retval void
  * @note   串口发送接收统一入口
  */
int USARTTR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen ,u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartTR[no - 1])
		return -1;
	return p_UartTR[no-1](txbuf, txlen , rxbuf , rxlen);
}

/////////////////RT
int USART_RTNull(int (*p_cmd)(u8 *,u16  ,u8* ,u16* )){return 0;}
int (*p_UartRT[])(int (*p_cmd)(u8 *,u16  ,u8* ,u16* )) =
{
	USART1_RT,
	USART_RTNull,
	USART_RTNull,
	USART_RTNull,
	USART_RTNull,
	USART_RTNull,

};

//typedef int (*FUN_CMD)(u8 *,u16  ,u8* ,u16* );
/**
  * @brief  串口接收发送主函数
  * @param  
  * @retval void
  * @note   串口接收发送统一入口
  */
int USARTRT(int (*p_cmd)(u8 *,u16  ,u8* ,u16* ),u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartRT[no - 1])
		return -1;
	return p_UartRT[no-1](p_cmd);
}


/*统一中断处理*/
void USARTx_IRQHandler(USART_TypeDef* USARTx, u8 uartNo)
{
	u8 Res;
  
	if (USART_GetFlagStatus(USARTx, USART_FLAG_ORE) != RESET)
	{
	  USART_ReceiveData(USARTx);
	}
	if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)   /*接收中断()*/
	{
		USART_ClearITPendingBit(USARTx,USART_IT_RXNE);
		Res =USART_ReceiveData(USARTx);                       /*(USARTx->DR);	//读取接收到的数据*/
		
  }else if(USART_GetITStatus(USARTx, USART_IT_TC) != RESET){
		USART_ClearITPendingBit(USARTx,USART_IT_TC);
	}
}

