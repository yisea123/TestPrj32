
/**
  ******************************************************************************
  * @file    /dmax.h 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-4-1
  * @brief   a package of dmax
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _DMA_X_
#define  _DMA_X_


#include "_include.h"


#if defined (STM32F10X_HD)
//void DMA_Config(DMA_Channel_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);
void DMA_Enable(DMA_Channel_TypeDef *DMA_Channelx,u32 mar/*内存地址*/,u16 ndtr/*内存长度*/);
void DMA_ConfigDir(DMA_Channel_TypeDef *DMA_Channelx,u32 par/*外设地址*/,u32 mar/*内存地址*/,u16 ndtr/*内存字节长度，配置成字节*/,u32 dir);
#elif defined (STM32F40_41xxx)
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u32 mar/*内存地址*/,u16 ndtr/*内存长度*/);
void DMA_ConfigDir(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par/*外设地址*/,u32 mar/*内存地址*/,u16 ndtr/*内存字节长度，配置成字节*/,u32 dir);
void DMA_ConfigDir16(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par/*外设地址*/,u32 mar/*内存地址*/,u16 ndtr/*内存字节长度，配置成字节*/,u32 dir,u32 mode);

#endif

#endif

