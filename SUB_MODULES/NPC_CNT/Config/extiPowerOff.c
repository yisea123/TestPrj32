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

/** @brief  �ж�����
  * @param  
  * @retval 
  * @note   
  */
void EXTIPowerOff_Configuration(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);//
	
  /* ����EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
}


/** @brief  �ⲿ�ж�15-10
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
		if(0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)){//��ʾ�½���
			FlashBak_BackUp();
			start = GetCdvTimeTick();
			g_exti_power_off_flag = -1;
		}else if(g_exti_power_off_flag == -1){//������
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