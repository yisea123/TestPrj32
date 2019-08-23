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
	#include "uart3.h"
	#include "uart4.h"
	#include "uart5.h"
	#include "uart6.h"

/*USART����*/
void USART_ConfigNull(u32 bound, u16 wordLength, u16 stopBits, u16 parity){
	
}
//TODO ������null���ÿպ���
void (*p_UartSet[])(u32 , u16 , u16 , u16) =
{
	
#if _NPC_VERSION_ == 1u
	USART1_Configuration,
	USART2_Configuration,
	USART3_Configuration,
	UART4_Configuration,
	UART5_Configuration,
	USART_ConfigNull,
#elif _NPC_VERSION_ == 2u
	USART1_Configuration,
	USART2_Configuration,
	USART3_Configuration,
	UART4_Configuration,
	UART5_Configuration,
	USART6_Configuration,
#elif _NPC_VERSION_ == 3u
	USART1_Configuration,
	USART_ConfigNull,
	USART_ConfigNull,
	UART4_Configuration,
	UART5_Configuration,
	USART6_Configuration,
#endif
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

//void USART_ConfigNull(u32 bound, u16 wordLength, u16 stopBits, u16 parity){
//	
//}
//void (*p_UartSet[])(u32 , u16 , u16 , u16) =
//{
//#if _NPC_VERSION_ == 1u
//	USART1_Configuration,
//	USART2_Configuration,
//	USART3_Configuration,
//	UART4_Configuration,
//	UART5_Configuration,
//	USART_ConfigNull,
//#elif _NPC_VERSION_ == 2u
//	USART1_Configuration,
//	USART2_Configuration,
//	USART3_Configuration,
//	UART4_Configuration,
//	UART5_Configuration,
//	USART6_Configuration,
//#elif _NPC_VERSION_ == 3u
//	USART1_Configuration,
//	USART_ConfigNull,
//	USART_ConfigNull,
//	UART4_Configuration,
//	UART5_Configuration,
//	USART6_Configuration,
//#endif
//};


/*USART����*/
void USART_Configuration(void){
	int i = 0;
	struct COM_PARAM comPar;
	
	for (i = 0; i < 6; i++)
	{
		Org_Flash_Read((CDV_INT08U *)&comPar, USART_SET_ADDR(i), sizeof(comPar));
		if(p_UartSet[i]) {
			if(OPT_SUCCESS == CorrectComParam(&comPar, i))
				p_UartSet[i](comPar.bound, comPar.wordLength, comPar.stopBits, comPar.parity);
			else
				p_UartSet[i](115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
		}
	}
//  USART1_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART2_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART3_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
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
	ASSERT(0 < no && 6 >= no);
  ASSERT(IS_USART_BAUDRATE(bound));  
  ASSERT(IS_USART_WORD_LENGTH(wordLength));
  ASSERT(IS_USART_STOPBITS(stopBits));
  ASSERT(IS_USART_PARITY(parity));
	
	if(!p_UartSet[no - 1])
		return;
		
	p_UartSet[no-1](bound, wordLength, stopBits, parity);
}

void USART_SendNull(u8 *buf,u16 len){}
void (*p_UartSend[])(u8* ,u16) =
{
#if _NPC_VERSION_ ==1u
	USART1_Send,
	USART2_Send,
	USART3_Send,
	UART4_Send,
	UART5_Send,
	USART_SendNull,
#elif _NPC_VERSION_ == 2u
	USART1_Send,
	USART2_Send,
	USART3_Send,
	UART4_Send,
	UART5_Send,
	USART6_Send,
#elif _NPC_VERSION_ == 3u
	USART1_Send,
	USART_SendNull,
	USART_SendNull,
	UART4_Send,
	UART5_Send,
	USART6_Send,
#endif
};

	

/**
  * @brief  ���ڷ���������
  * @param  buf
	*					len
	*					no :1 - 6
  * @retval void
  * @note   ���ڷ���ͳһ���
  */
void USARTSend(u8 *buf ,u16 len ,u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartSend[no - 1])
		return;
	p_UartSend[no-1](buf ,len);
	
}

void USART_TRNull(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen){}
void (*p_UartTR[])(u8 *,u16  ,u8*  ,u8 ,u8* ) =
{
#if _NPC_VERSION_ == 1u
	USART1_TR,
	USART2_TR,
	USART3_TR,
	UART4_TR,
	UART5_TR,
	USART_TRNull,
#elif _NPC_VERSION_ == 2u
	USART1_TR,
	USART2_TR,
	USART3_TR,
	UART4_TR,
	UART5_TR,
	USART6_TR,
#elif _NPC_VERSION_ == 3u
	USART1_TR,
	USART_TRNull,
	USART_TRNull,
	UART4_TR,
	UART5_TR,
	USART6_TR,
#endif
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
  */
void USARTTR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen ,u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartTR[no - 1])
		return;
	p_UartTR[no-1](txbuf, txlen , rxbuf , rxbufLen, rxlen);
}
/*ͳһ�жϴ���*/
void USARTx_IRQHandler(USART_TypeDef* USARTx) {
	CDV_INT08U Res;
  //tm1Re = 0;
	tm1Re = ReadClock1ms();
	if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)   /*�����ж�()*/
	{
		USART_ClearITPendingBit(USARTx,USART_IT_RXNE);
		Res =USART_ReceiveData(USARTx);                       /*(USARTx->DR);	//��ȡ���յ�������*/
		switch(g_cdvStat){
			case CDV_RECV:
				if (MAIN_COM == g_scriptRecv.port) {
					g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]++] = Res;//���浽����
					if (QUE_LEN <=  g_scriptRecv.len[g_scriptRecv.rxPos])
						MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
				}
				break;
			default:
				USART_RX_BUF_ADD_CHAR(Res);
				break;
		}
  } 
}