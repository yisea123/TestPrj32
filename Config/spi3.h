
/**
  ******************************************************************************
  * @file    /spi3.h 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-4-1
  * @brief   a package of spi3
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _SPI_3_
#define  _SPI_3_


#include "cdv_include.h"			 

//extern CDV_INT08U TxBuffer[100];
//extern CDV_INT08U Tx_Idx, Tx_len, Rx_Idx;
//extern CDV_INT08U RxBuffer[100];

void ClearRxBuf(void);
void DoRxCmd(void);

RET_STATUS SPI3_Send(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite);
void SPI3_Configuration(uint16_t baudRatePrescaler);
#endif

