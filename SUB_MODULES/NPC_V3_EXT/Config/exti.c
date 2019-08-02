/**
  ******************************************************************************
  * @file    /exti.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-2-24
  * @brief   a package of exti
  * 
@verbatim  


@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "exti.h"
	
/** @brief  中断设置
  * @param  
  * @retval 
  * @note   
  */
void EXTI_Configuration(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource15);//PA0 连接到中断线0
	
  /* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line15;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
}


/** @brief  外部中断15-10
  * @param  
  * @retval 
  * @note   
  */
void EXTI15_10_IRQHandler(void) {
	int i;
  OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)  
  {
    EXTI_ClearITPendingBit(EXTI_Line15);  
		if(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)){//表示下降沿
			ClearRxBuf();//Rx_Idx = 0;
			//SPI3_Configuration(SPI_BaudRatePrescaler_2);
			SPI_Cmd(SPI3, ENABLE);
		}else{//上升沿
			//while(SET != SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE));
			while(SET != SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE));
			while(SET == SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY));
			SPI_Cmd(SPI3, DISABLE);
			DoRxCmd();
			ClearRxBuf();
//			for(i = 0; i < Rx_Idx; i++) {
//	      if(RxBuffer[i] != 0xFF) {
//					//Cascade_Slave_Write("you can!" , 8);
//					CascadeSlaveParse(RxBuffer, Rx_Idx);
//					break;
//				}
//			}
		}
  }
	OSIntExit();
}

