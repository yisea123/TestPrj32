/**
  ******************************************************************************
  * @file    /IO.c 
  * @author  MMY
  * @version V0.0.1
  * @date    1-12-2015
  * @brief   a package of io and dip function
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "iox.h"
/** @brief  gpio初始化
  * @param 
  * @retval 
  * @note   
  */
void GpioConfig(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE );	  //使能GPIOA GPIOB GPIOC GPIOD通道时钟
	
	// 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}		

/** @brief  gpio初始化
  * @param 
  * @retval 
  * @note   
  */
void GpioInit(void) {
	GpioConfig();                                              /* Configure the LEDs */
	
	LED1 = LED_OFF;
	
	LED2 = LED_OFF;
	
	LED3 = LED_OFF;
	
	
}
