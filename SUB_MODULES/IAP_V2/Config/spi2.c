/**
  ******************************************************************************
  * @file    /spi2.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-4-1
  * @brief   a package of spi2
  * 
@verbatim  
             在NPC2.0中，spi2是一个多片选spi
						 可以有如下用途
						 flash
						 FPGA1
						 FPGA2
						 级联主端口
						 
						 由于spi2的没有使用中断等特殊机制，故发送接收使用的是spix中的通用函数
               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	#include "spi2.h"

/*主级联spi*/
void SPI2_Configuration(uint16_t baudRatePrescaler)
{
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	/*SPI2配置读写Flash*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);//APB2时钟一般为84Mhz.1是42M？
	
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_SPI2);//SCK
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_SPI2);//MISO
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource3, GPIO_AF_SPI2);//MOSI
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
#if  USE_CASCADE == 1u
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;//主端级联片选
	GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;//FPGA1控制片选
//	GPIO_Init(GPIOF, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;//FPGA2控制片选
//	GPIO_Init(GPIOH, &GPIO_InitStructure);
#if _NPC_VERSION_ == 2u
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;//FPGA1控制片选
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;//FPGA2控制片选
	GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;//Flash片选
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	
	CASCADE_CS_DS;
	FPGA1_CS_DS;
	FPGA2_CS_DS;
	FLASH_CS_DS;
	
	SPI_I2S_DeInit(SPI2);
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;//SPI_CPOL_Low;//
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;
	SPI_InitStruct.SPI_BaudRatePrescaler = baudRatePrescaler;//SPI_BaudRatePrescaler_128;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStruct);
	
	SPI_Cmd(SPI2, ENABLE);
	
	SPIx_ReadWriteByte(SPI2, 0xff);//启动传输	
}


