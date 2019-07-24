
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

extern CDV_INT08S g_dipCtrlWorker;//掉电恢复用
extern CDV_INT08S g_noWrite;//禁止spiflash写
/*
 *CDV外设相关定义
 */
/*初始化总函数*/ 
void PeriphDriverInit(void);
CDV_INT32U GetOnlyID(void);
//void TIM3_Init(CDV_INT32U TIM_scale, CDV_INT32U TIM_Period);
void NewMemory(void **p , size_t size );
void ReNewMemory(void **p , size_t size );
void DelMemory(void **p);
void* MemCpy(void* dst, const void* src, size_t n);
void* MemCpyUnsafe(void* dst, const void* src, size_t n);
void NewError(void );
void WhileError(void );

void DelayUS(u32 cnt);
void CDVParamInit(void);
void ShutDown(void);

#ifdef  DEBUG_TIME
void time_log_clear(void);
void time_log(CDV_INT32S info);
void time_log_send( CMD_ARG *arg);
#endif

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
	
	#define NPC_PRINT(str) com_print(str)
	void com_print(const char* str);
#else
  #define ASSERT(expr) ((void)0)
	#define NPC_PRINT(str) ((void)0)
#endif /* USE_FULL_ASSERT */
	
		
#endif

