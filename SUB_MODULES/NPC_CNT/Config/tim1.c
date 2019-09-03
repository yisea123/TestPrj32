/**
  ******************************************************************************
  * @file    /tim8.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2019-7-24
  * @brief   a package of tim8 function
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2019 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "tim1.h"
	#include "dmax.h"


uint16_t dma_cnt_buf[500] = {0};
//TIM 
//arr：自动重装值
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:42Mhz
//这里使用的是定时器3!
////ahb:168,apb1:42,apb2:84,pll:168
//tim1ch1 PE9
void TIM1_Init(u32 arr,u32 psc)
{
 //此部分需手动修改IO口设置
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 //AHB 域的最大频率为 168 MHz。高速 APB2 域的最大允许频率为 84 MHz。低速 APB1 域的最大允许频率为 42 MHz。
 //STM32F405xx/07xx 和 STM32F415xx/17xx 的定时器时钟频率由硬件自动设置。分为两种情况：
  //1. 如果 APB 预分频器为 1，定时器时钟频率等于 APB 域的频率。
  //2. 否则，等于 APB 域的频率的两倍 (×2)。
 //摘自《中文参考手册》
 //对照system_xx.c的SetSysClock中CFGR寄存器配置
 // /* HCLK = SYSCLK / 1*/
  // RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  //#if defined (STM32F40_41xxx) || defined (STM32F427_437xx) || defined (STM32F429_439xx)
  // /* PCLK2 = HCLK / 2*/
  // RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; //定时器1用的是APB2，此配置不是1分频
  // /* PCLK1 = HCLK / 4*/
  // RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
  //#endif /* STM32F40_41xxx || STM32F427_437x || STM32F429_439xx */
 //APB2 84M *2=168M
 //APB1 42M *2=84M
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能相关时钟 
/**
	*设置输入口
	*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
  //设置时基
  TIM_TimeBaseStructure.TIM_Prescaler=psc; //定时器分频(0-65535对应1-65536分频)
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式循环从0开始记到arr（上溢）
  TIM_TimeBaseStructure.TIM_Period=arr; //自动重装载值
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时钟1分频 1 * tck int
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//重复计数器，rcr+1次上溢后产生更新事件
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器14

  //TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 自动重载
	
	// 定时器输入配置
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //通道选择

	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿触发

	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //管脚与寄存器对应关系

	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //输入预分频。意思是控制在多少个输入周期做一次捕获，如果

	//输入的信号频率没有变，测得的周期也不会变。比如选择4分频，则每四个输入周期才做一次捕获，这样在输入信号变化不频繁的情况下，

	//可以减少软件被不断中断的次数。

	TIM_ICInitStructure.TIM_ICFilter = 0x0; //滤波设置，经历几个周期跳变认定波形稳定0x0～0xF
	
	//TIM_PWMIConfig(TIM1, &TIM_ICInitStructure); //pwm输入根据参数配置TIM外设信息
	TIM_ICInit(TIM1, &TIM_ICInitStructure); //输入捕获

  //设置从模式
	TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1); //选择IC1为始终触发源

//	TIM_SelectInputTrigger(TIM1, TIM_TS_ITR3); //选择tim4触发源

	TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);//TIM从模式：触发信号的上升沿重新初始化计数器和触发寄存器的更新事件

	//TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable); //同步从定时器

  //设置中断
	NVIC_InitStructure.NVIC_IRQChannel =/* TIM1_UP_TIM10_IRQn*/TIM1_CC_IRQn; //NVIC配置

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
//  //dma
//  TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);// 这个事件是从模式reset时生成的更新事件（只有在CR1->URS=0时才有效），不足是上溢下溢也会生成
//  DMA_ConfigDir16(DMA2_Stream5,DMA_Channel_6,(u32)&TIM1->CCR1,(u32)dma_cnt_buf,sizeof(dma_cnt_buf)/2,DMA_DIR_PeripheralToMemory,DMA_Mode_Circular);
//	DMA_Cmd(DMA2_Stream5, ENABLE);

  TIM_DMACmd(TIM1,TIM_DMA_CC1/*TIM_DMA_Update**/, ENABLE);// 这个事件是通道1输入捕获ti跳变时产生的ccx事件，上溢下溢不会生成
  DMA_ConfigDir16(DMA2_Stream3,DMA_Channel_6,(u32)&TIM1->CCR1,(u32)dma_cnt_buf,sizeof(dma_cnt_buf)/2,DMA_DIR_PeripheralToMemory,DMA_Mode_Circular);
	DMA_Cmd(DMA2_Stream3, ENABLE);
//
	TIM_Cmd(TIM1, ENABLE); //启动TIM2

	//TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE); //打开中断
		
}  

///**
//  * @brief  This function TIM1 interrupt request.
//  * @param  None
//  * @retval None
//  */ 
//void TIM1_UP_TIM13_IRQHandler(void)
//{
//	//if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //溢出中断
//		if(TIM_GetITStatus(TIM1,TIM_IT_CC3)==SET) //溢出中断
//	{
//		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
//		
//		
//	}
//	
//}
//void TIM1_CC_IRQHandler(void)
//{
//	//if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //溢出中断
//		if(TIM_GetITStatus(TIM1,TIM_IT_CC3)==SET) //溢出中断
//	{
//		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
//		
//		
//	}
//	
//}

