/**
  ******************************************************************************
  * @file    /spi3.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-4-1
  * @brief   a package of spi3
  * 
@verbatim  
             ��NPC2.0�У�spi3���ڴӼ�����
						 ���ղ������жϻ���
						 ���Ͳ����˻������
						 �ʷ��ͽ��սӿڶ�û�в���spix�ĺ���
						 �����Ƭѡa15Ҳ�������ж����������رմӼ���spi
               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
//	#include <setjmp.h>
	#include "spi3.h"
	/*test*/
CDV_INT08U *TxBuffer = NULL;
CDV_INT08U Tx_Idx = 0, Tx_len = 0, Rx_Idx = 0;
CDV_INT08U RxBuffer[256] = {0};
/** @brief  ������ջ���
  * @param  
  * @retval 
  * @note   
  */
void ClearRxBuf(void) {
	Rx_Idx = 0;
}
/** @brief  ִ�н��ջ����е�����
  * @param  
  * @retval 
  * @note   ����������0xFF������Ϊ��������
  */
void DoRxCmd(void) {
	int i;
	for(i = 0; i < Rx_Idx; i++) {
		if(RxBuffer[i] != 0xFF) {
			//Cascade_Slave_Write("you can!" , 8);
#if USE_CASCADE== 1u
			CascadeSlaveParse2((char*)RxBuffer, Rx_Idx);
#endif
			break;
		}
	}
}

/** @brief  SPI3����
  * @param  
  * @retval 
  * @note   ����Ϊ��spi��������spi��������ж�a15�п���
  */
void SPI3_Configuration(uint16_t baudRatePrescaler){
	SPI_InitTypeDef SPI_InitStruct;	 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;  
	/*SPI2���ö�дFlash*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);//SCK
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);//MISO
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);//MOSI
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//p15��ӳ��
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM3);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;//�Ӷ˼���Ƭѡ
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_I2S_DeInit(SPI3);
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;//SPI_CPOL_Low;//
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;
	SPI_InitStruct.SPI_BaudRatePrescaler = baudRatePrescaler;//SPI_BaudRatePrescaler_128;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI3, &SPI_InitStruct);

	//DMA_Config(DMA1_Stream3,DMA_Channel_0,(CDV_INT32U)&SPI2->DR,(CDV_INT32U)0,0);
	 /* Configure the Priority Group to 1 bit */                  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
    
  /* Configure the SPI interrupt priority */  
  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
	
	SPI_I2S_ITConfig(SPI3,SPI_I2S_IT_RXNE/* | SPI_I2S_IT_TXE*/,ENABLE);
	//SPI_I2S_ClearITPendingBit(SPI3, SPI_I2S_IT_TXE);
	SPI_I2S_ClearFlag(SPI3,SPI_I2S_FLAG_TXE);
	//SPI_I2S_ClearITPendingBit(SPI3,SPI_I2S_IT_RXNE);
	//SPI_I2S_ITConfig(SPI3,SPI_I2S_IT_RXNE,ENABLE); 
	//SPI_I2S_ITConfig(SPI3,SPI_I2S_IT_TXE,ENABLE);
	//SPI_Cmd(SPI3, ENABLE);
	//while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)==RESET);
//	SPI_I2S_ClearFlag(SPI3,SPI_I2S_FLAG_TXE);
//	SPI_I2S_SendData(SPI3,0x00);
	

}
/** @brief  dma��������
  * @param  
  * @retval 
  * @note   δ��ɰ棬����
  */
void DMA_SPI3Send(CDV_INT32U mar,CDV_INT16U ndtr){
	
	
	DMA_MemoryTargetConfig(DMA1_Stream3,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream3,ndtr);    //��ʼһ��DMA���䣡	  
  
	while(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)==RESET) {};	
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET) {};
//	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
//	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	
}
/** @brief  ��������
  * @param  
  * @retval 
  * @note   д�뷢�ͻ��棬���巢����spi�ж���
  *         �����еĵ�һ���ֽ�Ϊ����������
  */
RET_STATUS SPI3_Send(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite){  
  //�ж���һ�������Ƿ������
//  if(Tx_Idx != Tx_len)
//		return OPT_FAILURE;
	ASSERT(pBuffer);
	ASSERT(NumByteToWrite);
	Tx_len = 0;
	Tx_Idx = 0;
	NEW08U(TxBuffer, NumByteToWrite + 2);
	MemCpy(TxBuffer+1, pBuffer, NumByteToWrite);
	TxBuffer[0] = NumByteToWrite;
	TxBuffer[NumByteToWrite + 1] = 0x00;
	//����˳��Ҫ�Ķ�
	
	Tx_len = NumByteToWrite + 2;
	return OPT_SUCCESS;
     
}

/** @brief  ���·�������
  * @param  
  * @retval 
  * @note   д�뷢�ͻ��棬���巢����spi�ж���
  *         �����еĵ�һ���ֽ�Ϊ����������
  */
void SPI3_ReSend(void){  
	Tx_Idx = 0;
}

/** @brief  SPI3�ж�
  * @param  
  * @retval 
  * @note   ����SPI3�ķ���
  */
void SPI3_IRQHandler(void) {
  OSIntEnter();
	if/*(SPI_I2S_GetITStatus(SPI3,SPI_I2S_IT_TXE) == SET)
	{
		if(Tx_Idx < 100)
		{
			SPI_I2S_SendData(SPI3,TxBuffer[Tx_Idx++]);
		}
		else
		{
			Tx_Idx = 0;
		}
	}
	else if*/ (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET)
  {
		//SPI_I2S_ClearITPendingBit(SPI3,SPI_I2S_IT_RXNE);
		if(Rx_Idx < 100)
		{
			
		}
		else
		{
			Rx_Idx = 0;
		}
		RxBuffer[Rx_Idx++] = SPI_I2S_ReceiveData(SPI3);
		
		if(Tx_Idx < Tx_len)
			SPI_I2S_SendData(SPI3, TxBuffer[Tx_Idx++]);
		else
			;//SPI_I2S_SendData(SPI3, 0xFF);
  }
	OSIntExit();
}

