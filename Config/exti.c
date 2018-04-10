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
	
/** @brief  �ж�����
  * @param  
  * @retval 
  * @note   
  */
void EXTI_Configuration(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource15);//PA0 ���ӵ��ж���0
	
  /* ����EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line15;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
}


/** @brief  �ⲿ�ж�15-10
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
		if(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)){//��ʾ�½���
			ClearRxBuf();//Rx_Idx = 0;
			//SPI3_Configuration(SPI_BaudRatePrescaler_2);
			SPI_Cmd(SPI3, ENABLE);
		}else{//������
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

