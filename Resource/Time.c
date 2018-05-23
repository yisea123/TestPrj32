/**
  ******************************************************************************
  * @file    /Time.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-2
  * @brief   a package of Time function
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
	
	#include "Time.h"
CDV_INT32S Time_1ms = 0;
CDV_INT32S Time_10ms = 0;
CDV_INT32S Time_100ms = 0;
CDV_INT32S Time_1s = 0;


CDV_INT32S ReadClock1ms(void);
CDV_INT32S ReadClock10ms(void);
CDV_INT32S ReadClock100ms(void);
CDV_INT32S ReadClock1s(void);


RET_STATUS TimeCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) { 
//	OS_ERR err;
	//CPU_SR_ALLOC();//OS_CRITICAL_ENTER()
	CDV_INT08U opt , type /*, strLen*/;
	CDV_INT32U no ,valNo = 0; 
	CDV_INT32S num;
	//strLen = rxLen - 3;//中文长度
//	if(debug) {
//		return OPT_SUCCESS;
//	}
	no   = *(CDV_INT32U*)(rxBuf);
	opt  = rxBuf[4];
	type = rxBuf[5];
	
	switch(type) {
		case 0xFF:
			//num = (CDV_INT32S)((rxBuf[3]<<24) | (rxBuf[4]<<16) | (rxBuf[5]<<8) | rxBuf[6]);
			num = *(CDV_INT32S*)(rxBuf + 6);
			break;
		case 0xFE:
			//num = (CDV_INT32S)((rxBuf[3]));
		  valNo = *(CDV_INT32U*)(rxBuf + 6);
			num = VarGet(valNo);
			break;
	}

	switch(opt) {
		case 0x00:/*延时*/
			num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
			TimeDelay(no,num);
			break;
		case 0x01:/*满足条件*/
			num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
		  if(!num)
				return OPT_FAILURE;
			break;
		case 0x02:/*开始计时*/
		  StartCdvTime(no);
			break;
		case 0x03:/*读取计时*/
		  switch(type) {
				case 0xFE:
				  ValSet(valNo, EndCdvTime(no));
					break;
			}
			break;
		default:
			break;
		
	}
	return OPT_SUCCESS;
}

RET_STATUS ClockCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) { 
	CDV_INT08U /*no , */opt , type /*, strLen*/;
	CDV_INT32U no ,valNo = 0; 
	CDV_INT32S num;

	no   = rxBuf[0];
	opt  = rxBuf[4];
	type = rxBuf[5];
	
	switch(type) {
		case 0xFF:
			num = *(CDV_INT32S*)(rxBuf + 6);
			break;
		case 0xFE:
			valNo = *(CDV_INT32U*)(rxBuf + 6);
			num = VarGet(valNo);
			break;
	}

	switch(opt) {
		case 0x00:/*读1ms*/
		  switch(type) {
				case 0xFE:
				  ValSet(valNo, ReadClock1ms());
					break;
			}
			break;
		case 0x01:/*读10ms*/
		  switch(type) {
				case 0xFE:
				  ValSet(valNo, ReadClock10ms());
					break;
			}
			break;
		case 0x02:/*读100ms*/
		  switch(type) {
				case 0xFE:
				  ValSet(valNo, ReadClock100ms());
					break;
			}
			break;
		case 0x03:/*读1s*/
		  switch(type) {
				case 0xFE:
				  ValSet(valNo, ReadClock1s());
					break;
			}
			break;
		case 0xFF:
			valNo = ReadClock(no);
		  ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&valNo), 4, arg, RC_CRC);
			break;
			break;
		default:
			break;
		
	}
	return OPT_SUCCESS;
}

/**
  * @brief  定时器延时
  *  
  * @param  no     要操作的定时器号
  *   
  * @retval 
  *
  * @note   使用ucos延时函数，不使用定时器,ms延时
  */
void TimeDelay(CDV_INT08U no, CDV_INT32U num) {
	
	OS_ERR err;

//	RW_TIMER(no) = (0xFFFF < num)? 0xFFFF : num;
	OSTimeDlyHMSM(0,0,0,num,OS_OPT_TIME_HMSM_NON_STRICT,&err); //延时 ms		
//	RW_TIMER(no) = 0;	
}

/**
  * @brief  计时器读1ms,10ms,100ms,1s
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
CDV_INT32S ReadClock1ms(void) {
	return GetCdvTimeTick()/(OSCfg_TickRate_Hz/1000);	
}
CDV_INT32S ReadClock10ms(void) {
	return Time_10ms;	
}
CDV_INT32S ReadClock100ms(void) {
	return Time_100ms;	
}
CDV_INT32S ReadClock1s(void) {
	return Time_1s;	
}
CDV_INT32S (*p_ReadClock[])(void) =
{
	ReadClock1ms,
	ReadClock10ms,
	ReadClock100ms,
	ReadClock1s,
};

/*
param:
  which 选择读取的计时器类型。0 - 1ms；1 - 10ms；2 - 100ms；3 - 1s
return:
  所选择的计时器的当前计数值，一般是开机到读取时的一个计数值，有溢出的可能
*/
CDV_INT32S ReadClock(CDV_INT08U which) {
	return p_ReadClock[which]();
}
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
		
		Time_10ms++;
		if(Time_10ms >= 0x7270E00)//120000000
			Time_10ms = 0;
		
		if(Time_10ms%10 == 0) {
		  Time_100ms++;
			if(Time_100ms >= 0x7270E00)
			  Time_100ms = 0;
			
			if(Time_100ms%10 == 0) {
		    Time_1s++;
			  if(Time_1s >= 0x7270E00)
			    Time_1s = 0;
		  }
		}

#ifdef ENABLE_PID
		if(Time_10ms%5==0)
			ValveDAadjust_Flag = 1;
#endif
		
		
	}
	
	
	OSIntExit();    	//退出中断
}
