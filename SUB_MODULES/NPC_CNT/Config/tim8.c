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
	#include "dmax.h"
	//用于更新arr
uint16_t dma_buf[13] = {5600,2800,1400,1400,700,700/*,350,350*/,700,700,1400,1400,2800,2800,0};/*最后一个设成<ccr3都会停在pwm但是只有0会不产生update事件*/
//#if USE_PULSE_DRIVE == 1u

//TIM PWM部分初始化
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:42Mhz
//这里使用的是定时器3!
////ahb:168,apb1:42,apb2:84,pll:168
//tim8ch3 Pi7
//tim8ch3n PH15
void TIM8_PWM_Init(u16 arr,u16 psc)
{
  //此部分需手动修改IO口设置
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //推挽复用输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//GPIO_PuPd_UP; //上拉
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOI,&GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOI,GPIO_PinSource7,GPIO_AF_TIM8);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOH,&GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOH,GPIO_PinSource15,GPIO_AF_TIM8);
 
 
  TIM_TimeBaseStructure.TIM_Prescaler=psc; //定时器分频(0-65535对应1-65536分频)
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式循环从0开始记到arr（上溢）
  TIM_TimeBaseStructure.TIM_Period=arr; //自动重装载值
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时钟1分频 1 * tck int
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x01; // tim1 8 需要
  //重复计数器，rcr+1次上溢后产生更新事件，只影响update事件，不影响cc事件；
  /*update是cnt 与 arr的上溢 or 下溢使rep-到0产生；cc是cnt = min(ccrx, arr)的时候产生*/
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
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1 cnt < ccrx 输出ref电平 1 ； 模式2 cnt < ccrx 输出ref电平 0
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//互补输出
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低，low = ref ，high = ！ref
  TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_Low; //TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //输出空闲状态为1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //互补输出空闲状态为0
  TIM_OCInitStructure.TIM_Pulse = arr/2;;
  //OCX就是channelx
  TIM_OC3Init(TIM8, &TIM_OCInitStructure); //根据T指定的参数初始化外设TIM1 4OC1
  TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //使能TIM14在CCR1上的预装载寄存器，这样操作ccr1比较值不会立即生效，只有在下一次uev时才会更新ccr1影子寄存器
  TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 自动重载
	
	//不设优先级不能出现中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
 
  TIM_ITConfig(TIM8, TIM_IT_CC3 | TIM_IT_Update, ENABLE);//使能中断，中断事件为定时器工薪事件
 
  // 主模式 输出设置
  TIM_SelectMasterSlaveMode(TIM8, TIM_MasterSlaveMode_Enable);
  TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);
  //dma
  TIM_DMACmd(TIM8, TIM_DMA_Update, ENABLE);
  DMA_ConfigDir16(DMA2_Stream1,DMA_Channel_7,(u32)&TIM8->ARR,(u32)dma_buf,sizeof(dma_buf)/2,DMA_DIR_MemoryToPeripheral,DMA_Mode_Normal);
	//
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);  // 打开dam tc中断，用于判断dma传输结束
	/* 清除标志位 */
	DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  

	DMA_Cmd(DMA2_Stream1, DISABLE);//enable 一开始会被触发出一个dma事件
  //
  TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  //清除中断标志位

  TIM_SetCompare3(TIM8,200/*arr/2*/);//设置占空比
  TIM_Cmd(TIM8, DISABLE); //使能TIM14
  TIM_CtrlPWMOutputs(TIM8, ENABLE); //设置PMW主输出//tim1 8 需要
  //TIM_CCxCmd  TIM_CCxNCmd 
	//配置完后会产生一个update事件，应该是某些函数置位了ug
	//这里直接disable
}  

/**
  * @brief  This function TIM3 interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET) //溢出中断
	{
		LED2 =~LED2;
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  //清除中断标志位
		
	}
	
}
void TIM8_CC_IRQHandler(void)
{
	//if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET) //溢出中断
		if(TIM_GetITStatus(TIM8,TIM_IT_CC3)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM8,TIM_IT_CC3);  //清除中断标志位
		
		StartPWM();
	}
	
	
}


void DMA2_Stream1_IRQHandler(void) {
	
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) != RESET)  
	{
		/* 清除标志位 */
		DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  
		/* 关闭DMA */
		DMA_Cmd(DMA2_Stream1,DISABLE);
			
  }  
	
}

void StartPWM(void) {
	DMA_Enable(DMA2_Stream1,(u32)dma_buf,sizeof(dma_buf)/2);    //开始一次DMA传输！	到dma中计数器是ndtr - 1；
  TIM8->ARR = 2800;//由于存在影子寄存器，所以需要提前赋值，应该是数组第一个，经试验，数组第一个会被跳过（当arr设置的比较小时，比如说1，可能有例外）
  TIM8->EGR = 1;//产生更新事件 ，dma中计数器-- ，把arr的值传输到影子寄存器，才能起作用，或者如果arr是0，更新到影子寄存器还是0，不起作用。同时启动一次dma传输到arr，同时重载重复寄存器，另外psc也会更新到影子寄存器，还有计数器从0开始，这些都是更新事件的功劳
	
	
}
//#endif
