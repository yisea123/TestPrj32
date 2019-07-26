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
	
	#include "tim8.h"

#if USE_PULSE_DRIVE == 1u

//TIM PWM部分初始化
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:42Mhz
//这里使用的是定时器3!
////ahb:168,apb1:42,apb2:84,pll:168
//tim8ch3 PC8
//tim8ch3n PH15
void TIM8_PWM_Init(u32 arr,u32 psc)
{
 //此部分需手动修改IO口设置
 GPIO_InitTypeDef GPIO_InitStructure;
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_OCInitTypeDef TIM_OCInitStructure;
 TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
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
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
 GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //推挽复用输出
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//GPIO_PuPd_UP; //上拉
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
 GPIO_Init(GPIOH,&GPIO_InitStructure);
 GPIO_PinAFConfig(GPIOH,GPIO_PinSource15,GPIO_AF_TIM8);
 
 
 TIM_TimeBaseStructure.TIM_Prescaler=psc; //定时器分频(0-65535对应1-65536分频)
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式循环从0开始记到arr（上溢）
 TIM_TimeBaseStructure.TIM_Period=arr; //自动重装载值
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时钟1分频 1 * tck int
 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//重复计数器，rcr+1次上溢后产生更新事件
 TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//初始化定时器14
  //死区 互补
 TIM_BDTRStructInit(&TIM_BDTRInitStruct);
  TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct.TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//TIM_AutomaticOutput_Disable;
 TIM_BDTRConfig(TIM8, &TIM_BDTRInitStruct);
 //初始化 Channel3 PWM模式
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//互补输出
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
 TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_Low; //TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //输出空闲状态为1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //互补输出空闲状态为0
  TIM_OCInitStructure.TIM_Pulse = arr/2;;
  //OCX就是channelx
  TIM_OC3Init(TIM8, &TIM_OCInitStructure); //根据T指定的参数初始化外设TIM1 4OC1
 TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //使能TIM14在CCR1上的预装载寄存器
  TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 自动重载
 TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);//使能中断，中断事件为定时器工薪事件
  	TIM_Cmd(TIM8, ENABLE); //使能TIM14
 TIM_CtrlPWMOutputs(TIM8, ENABLE); //设置PMW主输出//原子例程没有
//TIM_CCxCmd  TIM_CCxNCmd 
   TIM_SetCompare3(TIM8,arr/2);//设置占空比？
}  

/**
  * @brief  This function TIM3 interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM8_IRQHandler(void)
{
	OSIntEnter();    //进入中断
	//if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET) //溢出中断
		if(TIM_GetITStatus(TIM8,TIM_IT_CC3)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  //清除中断标志位
		
		
	}
	
	
	OSIntExit();    	//退出中断
}
#endif
