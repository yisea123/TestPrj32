
/**
  ******************************************************************************
  * @file    /spi2.h 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-4-1
  * @brief   a package of spi2
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _SPI_2_
#define  _SPI_2_


#include "cdv_include.h"
#if _NPC_VERSION_ == 1u
#define	CASCADE_CS_EN 		
	#define	CASCADE_CS_DS 		
	#define	FPGA1_CS_EN 			
	#define	FPGA1_CS_DS 			
	#define	FPGA2_CS_EN 			
	#define	FPGA2_CS_DS 			
	#define	FLASH_CS_EN 			GPIO_ResetBits(GPIOI,GPIO_Pin_0)  //选中FLASH
	#define	FLASH_CS_DS 			GPIO_SetBits(GPIOI,GPIO_Pin_0)  //关闭FLASH					 
	#define SPEED_FLASH       SPIx_SetSpeed(SPI2, SPI_BaudRatePrescaler_2)
	#define SPEED_FPGA        SPIx_SetSpeed(SPI2, SPI_BaudRatePrescaler_2)
	#define SPEED_CASCADE     SPIx_SetSpeed(SPI2, SPI_BaudRatePrescaler_2)
#elif _NPC_VERSION_ == 2u
	#define	CASCADE_CS_EN 		GPIO_ResetBits(GPIOD,GPIO_Pin_15)  //选中级联
	#define	CASCADE_CS_DS 		GPIO_SetBits(GPIOD,GPIO_Pin_15)  //关闭级联				 
	//#define	FPGA1_CS_EN 			GPIO_ResetBits(GPIOF,GPIO_Pin_12)  //选中FPGA1
	//#define	FPGA1_CS_DS 			GPIO_SetBits(GPIOF,GPIO_Pin_12)  //关闭FPGA1			 
	//#define	FPGA2_CS_EN 			GPIO_ResetBits(GPIOH,GPIO_Pin_11)  //选中FPGA2
	//#define	FPGA2_CS_DS 			GPIO_SetBits(GPIOH,GPIO_Pin_11)  //关闭FPGA2
	#define	FPGA1_CS_EN 			GPIO_ResetBits(GPIOA,GPIO_Pin_8)  //选中FPGA1
	#define	FPGA1_CS_DS 			GPIO_SetBits(GPIOA,GPIO_Pin_8)  //关闭FPGA1			 
	#define	FPGA2_CS_EN 			GPIO_ResetBits(GPIOD,GPIO_Pin_3)  //选中FPGA2
	#define	FPGA2_CS_DS 			GPIO_SetBits(GPIOD,GPIO_Pin_3)  //关闭FPGA2

	#define	FLASH_CS_EN 			GPIO_ResetBits(GPIOI,GPIO_Pin_0)  //选中FLASH
	#define	FLASH_CS_DS 			GPIO_SetBits(GPIOI,GPIO_Pin_0)  //关闭FLASH					 

	#define SPEED_FLASH       SPIx_SetSpeed(SPI2, SPI_BaudRatePrescaler_2)
	#define SPEED_FPGA        SPIx_SetSpeed(SPI2, SPI_BaudRatePrescaler_8)
	#define SPEED_CASCADE     SPIx_SetSpeed(SPI2, SPI_BaudRatePrescaler_64)
#elif _NPC_VERSION_ == 3u
	#define	CASCADE_CS_EN 		
	#define	CASCADE_CS_DS 		
	#define	FPGA1_CS_EN 			
	#define	FPGA1_CS_DS 			
	#define	FPGA2_CS_EN 			
	#define	FPGA2_CS_DS 			
	#define	FLASH_CS_EN 			GPIO_ResetBits(GPIOI,GPIO_Pin_0)  //选中FLASH
	#define	FLASH_CS_DS 			GPIO_SetBits(GPIOI,GPIO_Pin_0)  //关闭FLASH					 
	#define SPEED_FLASH       SPIx_SetSpeed(SPI2, SPI_BaudRatePrescaler_2)
	#define SPEED_FPGA        SPIx_SetSpeed(SPI2, SPI_BaudRatePrescaler_2)
	#define SPEED_CASCADE     SPIx_SetSpeed(SPI2, SPI_BaudRatePrescaler_2)
#endif

void SPI2_Configuration(uint16_t baudRatePrescaler);
#endif

