/**
  ******************************************************************************
  * @file    /extiPowerOff.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2018-6-7
  * @brief   a package of exti power off
  * 
@verbatim  


@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "extiPowerOff.h"
	#include "FlashBak.h"
#if USE_EXTI_POWER_OFF == 1u

volatile s32 g_exti_power_off_flag = 0;

/**
  * @brief  
  * @param  
  * @retval
  */
s32 EXTIPowerOff_GetFlag(void)   
{
	return g_exti_power_off_flag;
}

void EXTIPowerOff_ClearFlag(void)   
{
	g_exti_power_off_flag = 0;
}

/** @brief  中断设置
  * @param  
  * @retval 
  * @note   
  */
void EXTIPowerOff_Configuration(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);//
	
  /* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
}


/** @brief  外部中断15-10
  * @param  
  * @retval 
  * @note   
  */
void EXTI9_5_IRQHandler(void) {
	int i;
	static u32 start = 0;
	
  OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)  
  {
    EXTI_ClearITPendingBit(EXTI_Line5);  
		if(0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)){//表示下降沿
			FlashBak_BackUp();
			start = GetCdvTimeTick();
			g_exti_power_off_flag = -1;
		}else if(g_exti_power_off_flag == -1){//上升沿
			if(0 < CalcTimeMS(GetCdvTimeTick(), start)) {
			  ResetCdv();
			}else{
				FlashBak_Restore();
			}
			g_exti_power_off_flag = 1;
		}
  }
	OSIntExit();
}

#endif