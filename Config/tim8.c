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

//TIM PWM���ֳ�ʼ��
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:42Mhz
//����ʹ�õ��Ƕ�ʱ��3!
////ahb:168,apb1:42,apb2:84,pll:168
//tim8ch3 PC8
//tim8ch3n PH15
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
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
 GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴�����
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//GPIO_PuPd_UP; //����
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
 GPIO_Init(GPIOH,&GPIO_InitStructure);
 GPIO_PinAFConfig(GPIOH,GPIO_PinSource15,GPIO_AF_TIM8);
 
 
 TIM_TimeBaseStructure.TIM_Prescaler=psc; //��ʱ����Ƶ(0-65535��Ӧ1-65536��Ƶ)
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽѭ����0��ʼ�ǵ�arr�����磩
 TIM_TimeBaseStructure.TIM_Period=arr; //�Զ���װ��ֵ
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ��1��Ƶ 1 * tck int
 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//�ظ���������rcr+1���������������¼�
 TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
  //���� ����
 TIM_BDTRStructInit(&TIM_BDTRInitStruct);
  TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct.TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//TIM_AutomaticOutput_Disable;
 TIM_BDTRConfig(TIM8, &TIM_BDTRInitStruct);
 //��ʼ�� Channel3 PWMģʽ
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//�������
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
 TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_Low; //TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //�������״̬Ϊ1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //�����������״̬Ϊ0
  TIM_OCInitStructure.TIM_Pulse = arr/2;;
  //OCX����channelx
  TIM_OC3Init(TIM8, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM1 4OC1
 TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPEʹ�� �Զ�����
 TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);//ʹ���жϣ��ж��¼�Ϊ��ʱ����н�¼�
  	TIM_Cmd(TIM8, ENABLE); //ʹ��TIM14
 TIM_CtrlPWMOutputs(TIM8, ENABLE); //����PMW�����//ԭ������û��
//TIM_CCxCmd  TIM_CCxNCmd 
   TIM_SetCompare3(TIM8,arr/2);//����ռ�ձȣ�
}  

/**
  * @brief  This function TIM3 interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM8_UP_TIM13_IRQHandler(void)
{
	OSIntEnter();    //�����ж�
	//if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET) //����ж�
		if(TIM_GetITStatus(TIM8,TIM_IT_CC3)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  //����жϱ�־λ
		
		
	}
	
	
	OSIntExit();    	//�˳��ж�
}
void TIM8_CC_IRQHandler(void)
{
	OSIntEnter();    //�����ж�
	//if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET) //����ж�
		if(TIM_GetITStatus(TIM8,TIM_IT_CC3)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  //����жϱ�־λ
		
		
	}
	
	
	OSIntExit();    	//�˳��ж�
}
#endif
