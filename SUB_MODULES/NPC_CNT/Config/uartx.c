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
	#include "uart4.h"
//	#include "uart5.h"
//	#include "uart6.h"

/*USART����*/
void USART_ConfigNull(u32 bound, u16 wordLength, u16 stopBits, u16 parity){
	
}
//TODO ������null���ÿպ���
void (*p_UartSet[])(u32 , u16 , u16 , u16) =
{
	
	USART_ConfigNull,
	USART_ConfigNull,
	USART_ConfigNull,
	UART4_Configuration,
	USART_ConfigNull,
	USART_ConfigNull,

};

/** @brief  У���������õ�stm32ר������
  * @param  
  * @retval OPT_FAILURE ����������
  * @note   ��������stm32ר�õģ��ο�
λ��	��ż����	ֹͣλ
 7  	���ż  	0.5��1��1.5��2
 8  	�޼���  	0.5��1��1.5��2
 8  	���ż  	1
 9  	�޼���  	1
UART4��UART5��֧��0.5��1.5λ��ֹͣλ
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

/** @brief  �����㴮�ڲ�ѯ
  * @param  
  * @retval OPT_FAILURE ����������
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
//  USART1_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART2_Configuration(921600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART3_Configuration(460800, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
	UART4_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	UART5_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART6_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);

}
/**
  * @brief  ��������������
  * @param  buf
	*					len
	*					no :1 - 6
  * @retval void
  * @note   ��������ͳһ���
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
	USART_SendNull,
	USART_SendNull,
	UART4_Send,
	USART_SendNull,
	USART_SendNull,

};

	

/**
  * @brief  ���ڷ���������
  * @param  buf
	*					len
	*					no :1 - 6
  * @retval void
  * @note   ���ڷ���ͳһ���
  */
int USARTSend(u8 *buf ,u16 len ,u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartSend[no - 1])
		return -1;
	return p_UartSend[no-1](buf ,len);
	
}
int USART_TRNull(u8 *txbuf,u16 txlen ,u8** rxbuf ,u16* rxlen){}
int (*p_UartTR[])(u8 *,u16  ,u8** ,u16* ) =
{
	USART_TRNull,
	USART_TRNull,
	USART_TRNull,
	USART_TRNull,
	USART_TRNull,
	USART_TRNull,

};

	

/**
  * @brief  ���ڷ��ͽ���������
  * @param  txbuf  Ҫ���͵��ַ���
  *         txlen  Ҫ���͵��ַ�������
  *         rxbuf  ���յ��ַ�������
  *         rxbufLen �����ַ�������ĳ���
  *         rxlen  ���յ����ַ�������
	*					no :1 - 6�����Ĵ��ں�
  * @retval void
  * @note   ���ڷ��ͽ���ͳһ���
while��1�� {
	if(1 == USARTTR()) {
		 // do something
	}
}
  */
int USARTTR(u8 *txbuf,u16 txlen ,u8** rxbuf ,u16* rxlen ,u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartTR[no - 1])
		return -1;
	return p_UartTR[no-1](txbuf, txlen , rxbuf , rxlen);
}

/////////////////RT
int USART_RTNull(int (*p_cmd)(u8 *,u16  ,u8** ,u16* )){return 0;}
int (*p_UartRT[])(int (*p_cmd)(u8 *,u16  ,u8** ,u16* )) =
{
	USART_RTNull,
	USART_RTNull,
	USART_RTNull,
	UART4_RT,
	USART_RTNull,
	USART_RTNull,

};

//typedef int (*FUN_CMD)(u8 *,u16  ,u8* ,u16* );
/**
  * @brief  ���ڽ��շ���������
  * @param  
  * @retval void
  * @note   ���ڽ��շ���ͳһ���
  */
int USARTRT(int (*p_cmd)(u8 *,u16  ,u8** ,u16* ),u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartRT[no - 1])
		return -1;
	return p_UartRT[no-1](p_cmd);
}


/*ͳһ�жϴ���*/
void USARTx_IRQHandler(USART_TypeDef* USARTx, u8 uartNo)
{
	u8 Res;
  
	if (USART_GetFlagStatus(USARTx, USART_FLAG_ORE) != RESET)
	{
	  USART_ReceiveData(USARTx);
	}
	if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)   /*�����ж�()*/
	{
		USART_ClearITPendingBit(USARTx,USART_IT_RXNE);
		Res =USART_ReceiveData(USARTx);                       /*(USARTx->DR);	//��ȡ���յ�������*/
		
  }else if(USART_GetITStatus(USARTx, USART_IT_TC) != RESET){
		USART_ClearITPendingBit(USARTx,USART_IT_TC);
	}
}

