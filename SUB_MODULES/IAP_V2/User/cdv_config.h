
/**
  ******************************************************************************
  * @file    /user_config.h 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   stm32 config
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CDV_CONFIG_
#define  _CDV_CONFIG_





#include "cdv_include.h"


extern u8 RomanNumerals[9][3];
/*
 *CDV外设相关定义
 */
/*初始化总函数*/ 
void PeriphDriverInit(void);

//void TIM3_Init(CDV_INT32U TIM_scale, CDV_INT32U TIM_Period);
void NewMemory(void **p , size_t size );
void DelMemory(void **p);
void NewError(void );
void WhileError(void );

void DelayUS(u32 cnt);
void CDVParamInit(void);
void ResetCdv(void);

#define _DEBUG_NPC_
#ifdef  _DEBUG_NPC_

/**
  * @brief  自定义ASSERT
  * @param  expr: 表达式
  * @retval None
  */
  #define ASSERT(expr) ((expr) ? (void)0 : assert((uint8_t *)__FILE__, (uint8_t *)__FUNCTION__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert(uint8_t* file, uint8_t* function, uint32_t line);
#else
  #define ASSERT(expr) ((void)0)
#endif /* USE_FULL_ASSERT */
	
#endif

