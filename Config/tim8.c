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

////ͨ�ö�ʱ��3�жϳ�ʼ��
////arr���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
////Ft=��ʱ������Ƶ��,��λ:42Mhz
////����ʹ�õ��Ƕ�ʱ��3!
////ahb:168,apb1:42,apb2:84,pll:168
//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
//	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//}
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
		
		
	}
	
	
	OSIntExit();    	//�˳��ж�
}

//TIM14 PWM���ֳ�ʼ��
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM8_PWM_Init(u32 arr,u32 psc)
{
 //�˲������ֶ��޸�IO������
 GPIO_InitTypeDef GPIO_InitStructure;
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_OCInitTypeDef TIM_OCInitStructure;
 TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
 //AHB ������Ƶ��Ϊ 168 MHz������ APB2 ����������Ƶ��Ϊ 84 MHz������ APB1 ����������Ƶ��Ϊ 42 MHz��
 //STM32F405xx/07xx �� STM32F415xx/17xx �Ķ�ʱ��ʱ��Ƶ����Ӳ���Զ����á���Ϊ���������
  //1. ��� APB Ԥ��Ƶ��Ϊ 1����ʱ��ʱ��Ƶ�ʵ��� APB ���Ƶ�ʡ�
  //2. ���򣬵��� APB ���Ƶ�ʵ����� (��2)��
 //ժ�ԡ����Ĳο��ֲᡷ
 //����system_xx.c��SetSysClock��CFGR�Ĵ�������
 // /* HCLK = SYSCLK / 1*/
  // RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  //#if defined (STM32F40_41xxx) || defined (STM32F427_437xx) || defined (STM32F429_439xx)
  // /* PCLK2 = HCLK / 2*/
  // RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; //��ʱ��1�õ���APB2�������ò���1��Ƶ
  // /* PCLK1 = HCLK / 4*/
  // RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
  //#endif /* STM32F40_41xxx || STM32F427_437x || STM32F429_439xx */
 //APB2 84M *2=168M
 //APB1 42M *2=84M
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE); //TIM14ʱ��ʹ��
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //ʹ��PORTFʱ��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOF9
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
 GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴�����
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//GPIO_PuPd_UP; //����
 GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��PF9
 GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOF9����Ϊ��ʱ��14
 GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_TIM1); //GPIOF9����Ϊ��ʱ��14
 TIM_TimeBaseStructure.TIM_Prescaler=psc; //��ʱ����Ƶ(0-65535��Ӧ1-65536��Ƶ)
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽѭ����0��ʼ�ǵ�arr�����磩
 TIM_TimeBaseStructure.TIM_Period=arr; //�Զ���װ��ֵ
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ��1��Ƶ 1 * tck int
 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//�ظ���������rcr+1���������������¼�
 TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
  //���� ����
 TIM_BDTRStructInit(&TIM_BDTRInitStruct);
  TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct.TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//TIM_AutomaticOutput_Disable;
 TIM_BDTRConfig(TIM1, &TIM_BDTRInitStruct);
 //��ʼ��TIM14 Channel1 PWMģʽ
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//�������
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
 TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_Low; //TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //�������״̬Ϊ1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //�����������״̬Ϊ0
  TIM_OCInitStructure.TIM_Pulse = arr/2;;
  //OCX����channelx
  TIM_OC2Init(TIM1, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM1 4OC1
 TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� �Զ�����
 TIM_Cmd(TIM1, ENABLE); //ʹ��TIM14
 TIM_CtrlPWMOutputs(TIM1, ENABLE); //����PMW�����//ԭ������û��

   TIM_SetCompare1(TIM14,arr/2);//����ռ�ձȣ�
}  
