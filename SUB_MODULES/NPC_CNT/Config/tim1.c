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
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:42Mhz
//����ʹ�õ��Ƕ�ʱ��3!
////ahb:168,apb1:42,apb2:84,pll:168
//tim1ch1 PE9
void TIM1_Init(u32 arr,u32 psc)
{
 //�˲������ֶ��޸�IO������
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ�����ʱ�� 
/**
	*���������
	*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
  //����ʱ��
  TIM_TimeBaseStructure.TIM_Prescaler=psc; //��ʱ����Ƶ(0-65535��Ӧ1-65536��Ƶ)
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽѭ����0��ʼ�ǵ�arr�����磩
  TIM_TimeBaseStructure.TIM_Period=arr; //�Զ���װ��ֵ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ��1��Ƶ 1 * tck int
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;//�ظ���������rcr+1���������������¼�
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

  //TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� �Զ�����
	
	// ��ʱ����������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //ͨ��ѡ��

	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //�����ش���

	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //�ܽ���Ĵ�����Ӧ��ϵ

	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //����Ԥ��Ƶ����˼�ǿ����ڶ��ٸ�����������һ�β������

	//������ź�Ƶ��û�б䣬��õ�����Ҳ����䡣����ѡ��4��Ƶ����ÿ�ĸ��������ڲ���һ�β��������������źű仯��Ƶ��������£�

	//���Լ�������������жϵĴ�����

	TIM_ICInitStructure.TIM_ICFilter = 0x0; //�˲����ã������������������϶������ȶ�0x0��0xF
	
	//TIM_PWMIConfig(TIM1, &TIM_ICInitStructure); //pwm������ݲ�������TIM������Ϣ
	TIM_ICInit(TIM1, &TIM_ICInitStructure); //���벶��

  //���ô�ģʽ
	TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1); //ѡ��IC1Ϊʼ�մ���Դ

//	TIM_SelectInputTrigger(TIM1, TIM_TS_ITR3); //ѡ��tim4����Դ

	TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);//TIM��ģʽ�������źŵ����������³�ʼ���������ʹ����Ĵ����ĸ����¼�

	//TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable); //ͬ���Ӷ�ʱ��

  //�����ж�
	NVIC_InitStructure.NVIC_IRQChannel =/* TIM1_UP_TIM10_IRQn*/TIM1_CC_IRQn; //NVIC����

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
//  //dma
//  TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);// ����¼��Ǵ�ģʽresetʱ���ɵĸ����¼���ֻ����CR1->URS=0ʱ����Ч������������������Ҳ������
//  DMA_ConfigDir16(DMA2_Stream5,DMA_Channel_6,(u32)&TIM1->CCR1,(u32)dma_cnt_buf,sizeof(dma_cnt_buf)/2,DMA_DIR_PeripheralToMemory,DMA_Mode_Circular);
//	DMA_Cmd(DMA2_Stream5, ENABLE);

  TIM_DMACmd(TIM1,TIM_DMA_CC1/*TIM_DMA_Update**/, ENABLE);// ����¼���ͨ��1���벶��ti����ʱ������ccx�¼����������粻������
  DMA_ConfigDir16(DMA2_Stream3,DMA_Channel_6,(u32)&TIM1->CCR1,(u32)dma_cnt_buf,sizeof(dma_cnt_buf)/2,DMA_DIR_PeripheralToMemory,DMA_Mode_Circular);
	DMA_Cmd(DMA2_Stream3, ENABLE);
//
	TIM_Cmd(TIM1, ENABLE); //����TIM2

	//TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE); //���ж�
		
}  

///**
//  * @brief  This function TIM1 interrupt request.
//  * @param  None
//  * @retval None
//  */ 
//void TIM1_UP_TIM13_IRQHandler(void)
//{
//	//if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //����ж�
//		if(TIM_GetITStatus(TIM1,TIM_IT_CC3)==SET) //����ж�
//	{
//		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ
//		
//		
//	}
//	
//}
//void TIM1_CC_IRQHandler(void)
//{
//	//if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //����ж�
//		if(TIM_GetITStatus(TIM1,TIM_IT_CC3)==SET) //����ж�
//	{
//		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ
//		
//		
//	}
//	
//}

