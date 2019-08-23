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

/*SPI设置*/
void SPI_Configuration(void)
{
	SPI2_Configuration(SPI_BaudRatePrescaler_64);
	#if _NPC_VERSION_ == 2u
	SPI3_Configuration(SPI_BaudRatePrescaler_2);
	#endif
}
//SPI1速度设置函数
//SpeedSet:0~7
//SPI速度=fAPB2/2^(SpeedSet+1)
//fAPB2时钟一般为84Mhz
void SPIx_SetSpeed(SPI_TypeDef* SPIx, u8 SPI_BaudRatePrescaler)
{
  ASSERT(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPIx->CR1&=0XFFC7;
	SPIx->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPIx,ENABLE);  
} 

/*SPI读写
1.等待SPI TX空
2.发送数据
3.等待SPI TX空
4.等待SPI RX非空
5.延时
6.接收数据*/
CDV_INT08U SPIx_ReadWriteByte(SPI_TypeDef* SPIx, CDV_INT08U TxData)
{
	uint16_t ret;
//	OS_ERR err;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
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
 
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  

	SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个byte  数据

  //while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
//return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据
     
}



