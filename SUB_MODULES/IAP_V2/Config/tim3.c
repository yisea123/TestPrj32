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
	
	#include "tim3.h"
CDV_INT32S Time_1ms = 0;
CDV_INT32S Time_10ms = 0;
CDV_INT32S Time_100ms = 0;
CDV_INT32S Time_1s = 0;


CDV_INT32S ReadClock1ms(void);
CDV_INT32S ReadClock10ms(void);
CDV_INT32S ReadClock100ms(void);
CDV_INT32S ReadClock1s(void);


/**
  * @brief  ��ʱ����ʱ
  *  
  * @param  no     Ҫ�����Ķ�ʱ����
  *   
  * @retval 
  *
  * @note   ʹ��ucos��ʱ��������ʹ�ö�ʱ��,ms��ʱ
  */
void TimeDelay(CDV_INT08U no, CDV_INT32U num) {
	
	OS_ERR err;

//	RW_TIMER(no) = (0xFFFF < num)? 0xFFFF : num;
	OSTimeDlyHMSM(0,0,0,num,OS_OPT_TIME_HMSM_NON_STRICT,&err); //��ʱ ms		
//	RW_TIMER(no) = 0;	
}

/**
  * @brief  ��ʱ����1ms,10ms,100ms,1s
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
CDV_INT32S ReadClock1ms(void) {
	return GetCdvTimeTick();	
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


//Tout=((TIM_scale)*(TIM_Period))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM3_Init(u16 TIM_scale, u16 TIM_Period)//TIM_PeriodΪ16λ����
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef  NVIC_InitStructure; 
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //1
  	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);		
	
  	TIM_TimeBaseStructure.TIM_Period = TIM_Period-1;//ÿ�����ڼ���ֵ����0��ʼ����������ֵӦΪ����������ȥ1,��������װֵ,Period = (TIM counter clock / TIM output clock) - 1 = 20K
  	TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale-1;//Ԥ��Ƶֵ��Ϊ0ʱ��Ƶϵ��Ϊ1.Timer clock = sysclock /(TIM_Prescaler+1) = 42M
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����������
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//����������
	  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;//F4������һ��������ֻ�и߼���ʱ��TIM1��TIM8��Ч����Ӧ�Ĵ���RCR����˼����ÿTIM_RepetitionCounter+1���������ڲ���һ���ж�
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  	
//	TIM_PrescalerConfig(TIM3, (TIM_scale-1), TIM_PSCReloadMode_Immediate);
	  TIM_ClearFlag(TIM3,TIM_FLAG_Update);   //�������������ʱ������ĸ��±�־
	
  	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//ʹ���жϣ��ж��¼�Ϊ��ʱ����н�¼�
  	TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��
}

/**
  * @brief  This function TIM3 interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM3_IRQHandler(void)
{
	OSIntEnter();    //�����ж�
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
		
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
		
	}
	
	
	OSIntExit();    	//�˳��ж�
}
