
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


#include "cdv_include.h"


void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,CDV_INT32U chx,CDV_INT32U par,CDV_INT32U mar,CDV_INT16U ndtr);
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,CDV_INT16U ndtr);
#endif

