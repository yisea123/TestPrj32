/**
  ******************************************************************************
  * @file    /spix.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-2-24
  * @brief   a package of spix CONFIG
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

	#include "spix.h"

/*SPI����*/
void SPI_Configuration(void)
{
	SPI2_Configuration(SPI_BaudRatePrescaler_64);
	#if _NPC_VERSION_ == 2u
	SPI3_Configuration(SPI_BaudRatePrescaler_2);
	#endif
}
//SPI1�ٶ����ú���
//SpeedSet:0~7
//SPI�ٶ�=fAPB2/2^(SpeedSet+1)
//fAPB2ʱ��һ��Ϊ84Mhz
void SPIx_SetSpeed(SPI_TypeDef* SPIx, u8 SPI_BaudRatePrescaler)
{
  ASSERT(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPIx->CR1&=0XFFC7;
	SPIx->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPIx,ENABLE);  
} 

/*SPI��д
1.�ȴ�SPI TX��
2.��������
3.�ȴ�SPI TX��
4.�ȴ�SPI RX�ǿ�
5.��ʱ
6.��������*/
CDV_INT08U SPIx_ReadWriteByte(SPI_TypeDef* SPIx, CDV_INT08U TxData)
{
	uint16_t ret;
//	OS_ERR err;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPIx,TxData);
  while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==RESET);
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	
	ret = SPI_I2S_ReceiveData(SPIx);
//	OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);	
	return ret;
}


void SPIx_WriteByte(SPI_TypeDef* SPIx, u8 TxData)
{  
 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  

	SPI_I2S_SendData(SPIx, TxData); //ͨ������SPIx����һ��byte  ����

  //while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
//return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
     
}



