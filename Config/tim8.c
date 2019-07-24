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

////通用定时器3中断初始化
////arr：自动重装值。
////psc：时钟预分频数
////定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
////Ft=定时器工作频率,单位:42Mhz
////这里使用的是定时器3!
////ahb:168,apb1:42,apb2:84,pll:168
//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
//	TIM_Cmd(TIM3,ENABLE); //使能定时器3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//}
//Tout=((TIM_scale)*(TIM_Period))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void TIM3_Init(u16 TIM_scale, u16 TIM_Period)//TIM_Period为16位的数
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef  NVIC_InitStructure; 
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //1
  	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);		
	
  	TIM_TimeBaseStructure.TIM_Period = TIM_Period-1;//每个周期计数值，从0开始计数所以其值应为计数次数减去1,计数器重装值,Period = (TIM counter clock / TIM output clock) - 1 = 20K
  	TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale-1;//预分频值，为0时分频系数为1.Timer clock = sysclock /(TIM_Prescaler+1) = 42M
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//死区控制用
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器方向
	  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;//F4新增的一个东西，只有高级定时器TIM1和TIM8有效，对应寄存器RCR。意思就是每TIM_RepetitionCounter+1个技术周期产生一次中断
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  	
//	TIM_PrescalerConfig(TIM3, (TIM_scale-1), TIM_PSCReloadMode_Immediate);
	  TIM_ClearFlag(TIM3,TIM_FLAG_Update);   //必须先清除配置时候产生的更新标志
	
  	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能中断，中断事件为定时器工薪事件
  	TIM_Cmd(TIM3, ENABLE); //使能定时器
}

/**
  * @brief  This function TIM3 interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM3_IRQHandler(void)
{
	OSIntEnter();    //进入中断
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
		
		
	}
	
	
	OSIntExit();    	//退出中断
}

//TIM14 PWM部分初始化
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
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
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE); //TIM14时钟使能
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //使能PORTF时钟
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOF9
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
 GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //推挽复用输出
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//GPIO_PuPd_UP; //上拉
 GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化PF9
 GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOF9复用为定时器14
 GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_TIM1); //GPIOF9复用为定时器14
 TIM_TimeBaseStructure.TIM_Prescaler=psc; //定时器分频(0-65535对应1-65536分频)
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式循环从0开始记到arr（上溢）
 TIM_TimeBaseStructure.TIM_Period=arr; //自动重装载值
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时钟1分频 1 * tck int
 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//重复计数器，rcr+1次上溢后产生更新事件
 TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器14
  //死区 互补
 TIM_BDTRStructInit(&TIM_BDTRInitStruct);
  TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct.TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//TIM_AutomaticOutput_Disable;
 TIM_BDTRConfig(TIM1, &TIM_BDTRInitStruct);
 //初始化TIM14 Channel1 PWM模式
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//互补输出
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
 TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_Low; //TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //输出空闲状态为1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //互补输出空闲状态为0
  TIM_OCInitStructure.TIM_Pulse = arr/2;;
  //OCX就是channelx
  TIM_OC2Init(TIM1, &TIM_OCInitStructure); //根据T指定的参数初始化外设TIM1 4OC1
 TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); //使能TIM14在CCR1上的预装载寄存器
  TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 自动重载
 TIM_Cmd(TIM1, ENABLE); //使能TIM14
 TIM_CtrlPWMOutputs(TIM1, ENABLE); //设置PMW主输出//原子例程没有

   TIM_SetCompare1(TIM14,arr/2);//设置占空比？
}  
