/**
  ******************************************************************************
  * @file    /tim4.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2019-8-28
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
	
	#include "tim4.h"

//#if USE_PULSE_DRIVE == 1u

//TIM 部分初始化
//arr：自动重装值
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft s.
//Ft=外部工作频率,hz
//用来记主tim的更新次数
void TIM4_Count_Init(u32 arr,u32 psc)
{
  //此部分需手动修改IO口设置
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	
	NVIC_InitTypeDef NVIC_InitStructure;
 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
 
 
  TIM_TimeBaseStructure.TIM_Prescaler=psc; //定时器分频(0-65535对应1-65536分频)
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式循环从0开始记到arr（上溢）
  TIM_TimeBaseStructure.TIM_Period=arr; //自动重装载值
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时钟1分频 1 * tck int
  //TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//重复计数器，rcr+1次上溢后产生更新事件
  TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器14
	
  TIM_SelectInputTrigger(TIM4, TIM_TS_ITR3);//选择tim8为主
  TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_External1);
	
  // 主模式 输出设置
  TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);
//  //死区 互补
// TIM_BDTRStructInit(&TIM_BDTRInitStruct);
//  TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
//  TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
//  TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
//  TIM_BDTRInitStruct.TIM_DeadTime = 0x00;
//  TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
//  TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
//  TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//TIM_AutomaticOutput_Disable;
// TIM_BDTRConfig(TIM4, &TIM_BDTRInitStruct);
//  //初始化 Channel3 PWM模式
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing; //选择定时器模式:TIM脉冲宽度调制模式2
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出使能
//  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//互补输出
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
//  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //TIM_OCNPolarity_Low;
//  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //输出空闲状态为1
//  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //互补输出空闲状态为0
//  TIM_OCInitStructure.TIM_Pulse = arr/2;;
//  //OCX就是channelx
//  TIM_OC1Init(TIM4, &TIM_OCInitStructure); //根据T指定的参数初始化外设TIM1 4OC1
//  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能TIM14在CCR1上的预装载寄存器
  TIM_ARRPreloadConfig(TIM4, ENABLE);//ARPE使能 自动重载
		//不设优先级不能出现中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_Trigger, ENABLE);//使能中断，中断事件为定时器工薪事件
  TIM_Cmd(TIM4, ENABLE); //使能TIM14
// TIM_CtrlPWMOutputs(TIM4, ENABLE); //设置PMW主输出//原子例程没有
////TIM_CCxCmd  TIM_CCxNCmd 
//   TIM_SetCompare3(TIM4,arr/2);//设置占空比？
}

/**
  * @brief  This function TIM3 interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除中断标志位
		
		
	}
	if(TIM_GetITStatus(TIM4, TIM_IT_Trigger)==SET) //触发中断
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Trigger);  //清除中断标志位
		
		
	}
	
}

//#endif
