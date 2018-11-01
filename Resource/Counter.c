/**
  ******************************************************************************
  * @file    /Counter.h 
  * @author  MMY
  * @version V0.0.1
  * @date    10-16-2018
  * @brief   a package of counter
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
	
	#include "Counter.h"

COUNTER counter[COUNTER_NUM];


/** @brief  中断设置
  * @param  
  * @retval 
  * @note   
  */
void EXTI0_4_Configuration(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);//
	
  /* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = (uint32_t)(0x00001<<EXTI_PinSourcex);//EXTI_Line5;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI_PinSourcex + 6;//EXTI9_5_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
}


void Counter_init(void) {;
	
	counter[4].port = PORT_H;
	counter[4].pin = PIN_4;
	counter[4].cnt_var = ((CDV_INT32S*)g_modbusReg.reg)+CDV_VAR_NUM-1;
	counter[4].dir_var = ((CDV_INT32S*)g_modbusReg.reg)+CDV_VAR_NUM-2;
	*counter[4].cnt_var = 0;
	*counter[4].dir_var = 0;
	counter[0].port = PORT_A;
	counter[0].pin = PIN_0; 
	counter[0].cnt_var = ((CDV_INT32S*)g_modbusReg.reg)+CDV_VAR_NUM-3;
	counter[0].dir_var = ((CDV_INT32S*)g_modbusReg.reg)+CDV_VAR_NUM-4;
	*counter[0].cnt_var = 0;
	*counter[0].dir_var = 0;
	counter[3].port = PORT_C;
	counter[3].pin = PIN_3;  
	counter[3].cnt_var = ((CDV_INT32S*)g_modbusReg.reg)+CDV_VAR_NUM-5;
	counter[3].dir_var = ((CDV_INT32S*)g_modbusReg.reg)+CDV_VAR_NUM-6;
	*counter[3].cnt_var = 0;
	*counter[3].dir_var = 0;
	counter[2].port = PORT_C;
	counter[2].pin = PIN_2;  
	counter[2].cnt_var = ((CDV_INT32S*)g_modbusReg.reg)+CDV_VAR_NUM-7;
	counter[2].dir_var = ((CDV_INT32S*)g_modbusReg.reg)+CDV_VAR_NUM-8;
	*counter[2].cnt_var = 0;
	*counter[2].dir_var = 0;
	
	EXTI0_4_Configuration(EXTI_PortSourceGPIOC, EXTI_PinSource2);
	EXTI0_4_Configuration(EXTI_PortSourceGPIOC, EXTI_PinSource3);
	EXTI0_4_Configuration(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	EXTI0_4_Configuration(EXTI_PortSourceGPIOH, EXTI_PinSource4);
}



void EXTI0_IRQHandler(void) {
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)  
  {
    EXTI_ClearITPendingBit(EXTI_Line0);  
		if(0 == GPIO_ReadInputDataBit(counter[0].port,counter[0].pin)){//表示下降沿
	    (*counter[0].cnt_var)+=(*counter[0].dir_var);
		}
  }
	OSIntExit();
}
void EXTI1_IRQHandler(void) {
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)  
  {
    EXTI_ClearITPendingBit(EXTI_Line1);  
		if(0 == GPIO_ReadInputDataBit(counter[1].port,counter[1].pin)){//表示下降沿
			(*counter[1].cnt_var)+=(*counter[1].dir_var);
		}
  }
	OSIntExit();
}
void EXTI2_IRQHandler(void) {
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)  
  {
    EXTI_ClearITPendingBit(EXTI_Line2);  
		if(0 == GPIO_ReadInputDataBit(counter[2].port,counter[2].pin)){//表示下降沿
			(*counter[2].cnt_var)+=(*counter[2].dir_var);
		}
  }
	OSIntExit();
	
}
void EXTI3_IRQHandler(void) {
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)  
  {
    EXTI_ClearITPendingBit(EXTI_Line3);  
		if(0 == GPIO_ReadInputDataBit(counter[3].port,counter[3].pin)){//表示下降沿
			(*counter[3].cnt_var)+=(*counter[3].dir_var);
		}
  }
	OSIntExit();
	
}
void EXTI4_IRQHandler(void) {
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)  
  {
    EXTI_ClearITPendingBit(EXTI_Line4);  
		if(0 == GPIO_ReadInputDataBit(counter[4].port,counter[4].pin)){//表示下降沿
			(*counter[4].cnt_var)+=(*counter[4].dir_var);
		}
  }
	OSIntExit();
	
}
//void EXTI4_IRQHandler(void) {
//	OSIntEnter();
//	if(EXTI_GetITStatus(EXTI_Line4) != RESET)  
//  {
//    EXTI_ClearITPendingBit(EXTI_Line4);  
//		if(0 == GPIO_ReadInputDataBit(counter[4].port,counter[4].pin)){//表示下降沿
//			
//		}
//  }
//	OSIntExit();
//}

