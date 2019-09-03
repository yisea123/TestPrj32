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

//TIM ���ֳ�ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft s.
//Ft=�ⲿ����Ƶ��,hz
//��������tim�ĸ��´���
void TIM4_Count_Init(u32 arr,u32 psc)
{
  //�˲������ֶ��޸�IO������
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	
	NVIC_InitTypeDef NVIC_InitStructure;
 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
 
 
  TIM_TimeBaseStructure.TIM_Prescaler=psc; //��ʱ����Ƶ(0-65535��Ӧ1-65536��Ƶ)
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽѭ����0��ʼ�ǵ�arr�����磩
  TIM_TimeBaseStructure.TIM_Period=arr; //�Զ���װ��ֵ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ��1��Ƶ 1 * tck int
  //TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//�ظ���������rcr+1���������������¼�
  TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
	
  TIM_SelectInputTrigger(TIM4, TIM_TS_ITR3);//ѡ��tim8Ϊ��
  TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_External1);
	
  // ��ģʽ �������
  TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);
//  //���� ����
// TIM_BDTRStructInit(&TIM_BDTRInitStruct);
//  TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
//  TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
//  TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
//  TIM_BDTRInitStruct.TIM_DeadTime = 0x00;
//  TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
//  TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
//  TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//TIM_AutomaticOutput_Disable;
// TIM_BDTRConfig(TIM4, &TIM_BDTRInitStruct);
//  //��ʼ�� Channel3 PWMģʽ
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ����ʹ��
//  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//�������
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
//  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //TIM_OCNPolarity_Low;
//  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //�������״̬Ϊ1
//  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //�����������״̬Ϊ0
//  TIM_OCInitStructure.TIM_Pulse = arr/2;;
//  //OCX����channelx
//  TIM_OC1Init(TIM4, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM1 4OC1
//  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  TIM_ARRPreloadConfig(TIM4, ENABLE);//ARPEʹ�� �Զ�����
		//�������ȼ����ܳ����ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_Trigger, ENABLE);//ʹ���жϣ��ж��¼�Ϊ��ʱ����н�¼�
  TIM_Cmd(TIM4, ENABLE); //ʹ��TIM14
// TIM_CtrlPWMOutputs(TIM4, ENABLE); //����PMW�����//ԭ������û��
////TIM_CCxCmd  TIM_CCxNCmd 
//   TIM_SetCompare3(TIM4,arr/2);//����ռ�ձȣ�
}

/**
  * @brief  This function TIM3 interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //����жϱ�־λ
		
		
	}
	if(TIM_GetITStatus(TIM4, TIM_IT_Trigger)==SET) //�����ж�
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Trigger);  //����жϱ�־λ
		
		LED2 =~LED2;
		
	}
	
}

//#endif
