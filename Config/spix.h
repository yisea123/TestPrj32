
/**
  ******************************************************************************
  * @file    /spix.h 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-2-24
  * @brief   a package of spix CONFIG
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _SPI_X_
#define  _SPI_X_


#include "cdv_include.h"

void SPI_Configuration(void);
CDV_INT08U SPIx_ReadWriteByte(SPI_TypeDef* SPIx, CDV_INT08U TxData);
void SPIx_SetSpeed(SPI_TypeDef* SPIx, u8 SPI_BaudRatePrescaler);
#endif

