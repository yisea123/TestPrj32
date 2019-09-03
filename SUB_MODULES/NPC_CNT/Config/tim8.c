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
	//���ڸ���arr
//uint16_t dma_buf[13] = {5600,2800,1400,1400,700,700/*,350,350*/,700,700,1400,1400,2800,2800,0};/*���һ�����<ccr3����ͣ��pwm����ֻ��0�᲻����update�¼�*/
uint16_t dma_buf[200] = {0};
//#if USE_PULSE_DRIVE == 1u

//TIM PWM���ֳ�ʼ��
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:42Mhz
//����ʹ�õ��Ƕ�ʱ��3!
////ahb:168,apb1:42,apb2:84,pll:168
//tim8ch3 Pi7
//tim8ch3n PH15
void TIM8_PWM_Init(u16 arr,u16 psc)
{
  //�˲������ֶ��޸�IO������
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴�����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//GPIO_PuPd_UP; //����
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOI,&GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOI,GPIO_PinSource7,GPIO_AF_TIM8);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOH,&GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOH,GPIO_PinSource15,GPIO_AF_TIM8);
 
 
  TIM_TimeBaseStructure.TIM_Prescaler=psc; //��ʱ����Ƶ(0-65535��Ӧ1-65536��Ƶ)
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽѭ����0��ʼ�ǵ�arr�����磩
  TIM_TimeBaseStructure.TIM_Period=arr; //�Զ���װ��ֵ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ��1��Ƶ 1 * tck int
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x01; // tim1 8 ��Ҫ
  //�ظ���������rcr+1���������������¼���ֻӰ��update�¼�����Ӱ��cc�¼���
  /*update��cnt �� arr������ or ����ʹrep-��0������cc��cnt = min(ccrx, arr)��ʱ�����*/
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
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1 cnt < ccrx ���ref��ƽ 1 �� ģʽ2 cnt < ccrx ���ref��ƽ 0
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//�������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Եͣ�low = ref ��high = ��ref
  TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_Low; //TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //�������״̬Ϊ1
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //�����������״̬Ϊ0
  TIM_OCInitStructure.TIM_Pulse = arr/2;;
  //OCX����channelx
  TIM_OC3Init(TIM8, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM1 4OC1
  TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ�������������ccr1�Ƚ�ֵ����������Ч��ֻ������һ��uevʱ�Ż����ccr1Ӱ�ӼĴ���
  TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPEʹ�� �Զ�����
	
	//�������ȼ����ܳ����ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;//�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
 
  TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  //����жϱ�־λ

  TIM_ITConfig(TIM8, TIM_IT_CC3 | TIM_IT_Update, DISABLE);//ʹ���жϣ��ж��¼�Ϊ��ʱ����н�¼�//�����������һ��update�¼�,��ǰ����Ҫclear
 
  // ��ģʽ �������
  TIM_SelectMasterSlaveMode(TIM8, TIM_MasterSlaveMode_Enable);
  TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);
  //dma
  TIM_DMACmd(TIM8, TIM_DMA_Update, ENABLE);
  DMA_ConfigDir16(DMA2_Stream1,DMA_Channel_7,(u32)&TIM8->ARR,(u32)dma_buf,sizeof(dma_buf)/2,DMA_DIR_MemoryToPeripheral,DMA_Mode_Normal);
	//
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
	/* �����־λ */
	DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  
	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);  // ��dam tc�жϣ������ж�dma�������

	DMA_Cmd(DMA2_Stream1, DISABLE);//enable һ��ʼ�ᱻ������һ��dma�¼������֮ǰ��TIM_IT_Updateû�б�clear��������DMA_Cmd(DMA_Streamx, DISABLE)ʱ�����dma tc�жϣ����disable 
  //
  
  TIM_SetCompare3(TIM8, 100/*arr/2*/);//����ccr3
  TIM_Cmd(TIM8, DISABLE/*DISABLE*/); //ʹ��TIM
  TIM_CtrlPWMOutputs(TIM8, ENABLE); //����PMW�����//tim1 8 ��Ҫ
  //TIM_CCxCmd  TIM_CCxNCmd 
	
	//����ֱ��disable
	//TIM8->EGR = 1;//
}  

/**
  * @brief  This function TIM3 interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  //����жϱ�־λ
		
  TIM_ITConfig(TIM8, TIM_IT_Update, DISABLE);//ʹ���жϣ��ж��¼�Ϊ��ʱ����н�¼�//�����������һ��update�¼�,��ǰ����Ҫclear
 
	if(SW1 == LED_ON){
//		 TIM8->ARR = 1400;
		StartPWM(); //������ȽϺ���ȷ��dma����������
	}
	}
	
}
void TIM8_CC_IRQHandler(void)
{
	//if(TIM_GetITStatus(TIM8,TIM_IT_Update)==SET) //����ж�
		if(TIM_GetITStatus(TIM8,TIM_IT_CC3)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM8,TIM_IT_CC3);  //����жϱ�־λ
		
	}
	
	
}


void DMA2_Stream1_IRQHandler(void) {
	//dma tc�� tim tc֮ǰ����Ϊdma�ȴ�����ɣ�tim��Ҫ��һ�μ����Ž�����
	//�����������dma �ڴ棬���������������0ʱ����Ϊ�����������¼��������ֶ����»��arr��ֵ���ᵼ�����������⣩���������0�������ﲻ��Ҳ��Ҫ�ֶ�����
	//��Ȥ���ǣ�����������pwm֮��
	//ǰһ�����0������arr����0��0��ֹͣ�����������¼��������൱�ڰ�dma���һ�δ����0ֵ�����ǵ���������һ�θ���Ӱ�ӾͲ�����յ�0ֵ���Ӷ�����ֹͣ������ǰarr���������Ż��������[0]�����壻�����ֶ����£����һ����������[0]��ֵ�������ǲ��������θ��£���һ�ΰɵ�ǰarr��ֵ����Ӱ�ӣ�����0����arr�����Եڶ��β�������0��ֵ����Ӱ�ӣ�����ƺ�����������ɵ�һ������ʱ������������������
	//��һ�������0����ʱpwm��һֱ���ڣ�һֱ�и��²�������ע�ⲻ�ֶ����£���һ����������0��ֵ��������arr�ᵼ���������һ�����屻���Ƕ��ı䣬ͬǰһ��������ơ�
	//���ϣ��������������Ҫ����arr��// ����������ж����и�0ʱ���������
	//  ����dma����tim tc������startpwm�е�dma enableʱdma����û��������arr������³�����[0]�������Ļ���TIM8->EGR = 1�ֶ����£���ѵ�ǰֵˢ��0����ֹͣpwm��Ҳ�����ִ���߼��ж�ʱdma�������ˣ�tim��ʱ�̣�,����Ҫ����tim tc �ȴ�����ɣ����ƴ��ڵĲ�����
	//  �����Ҫ������ж����棬��ô������TIM8->EGR = 1�ֶ�����ǰ����arr��Ϊ�������һ����0ֵ����ֹ���һ�������������
	//���������һ������ڸ������������£���main����������Ҫ�����ֶ����¡�����ԭ������
	//  dma enable ������ʹ�������ϴε�δ���������軹δ������һ��dma����dma��tc����tim��up������dma���ᴫ�䵽arr�������ⲿʹ��ʱ�ϴε�dma�Ѿ������������Ѿ�������һ��dma����tim��up�Ѿ����������ʻ���������һ��dma���������0���µ�arr���Ӷ�ֻ��һ���ֶ�����
	//  �������úõ�ʱ��û���������󣬹���Ҫ�����ֶ����£���ô����ͨ������itʱ�����ĸ����¼��������һ���ֶ����������ԣ����У���Ϊdma����ʱ����δ��ɣ�dma����ᱻ�������dma enable�����ú�ʵ���Ͼ�ֻ��һ����
	//����һ��Ҫע�⣬����ʱ����ʱ��PWM�ǲ�ͣ�Ļᴥ���¼���ֻ�ǲ����жϣ���������ʵ���ʱ��׼ȷ
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) != RESET)  
	{
		/* �����־λ */
		DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  
		/* �ر�DMA */
		DMA_Cmd(DMA2_Stream1,DISABLE);
		/* �򿪷�������ж�,ȷ�����һ���ֽڷ��ͳɹ� */
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);  //����жϱ�־λ

		 TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);//ʹ���жϣ�
  }  
	
}
/*
����һ��pwm
��һ��ʹ��
  StartPWM();
	TIM8->EGR = 1;
	TIM_Cmd(TIM8, ENABLE);
ƽʱʹ��
  StartPWM();
ע�⣺dma���������0������ҪTIM8->EGR = 1;���߲���Ҫ
*/
void StartPWM(void) {
	DMA_Enable(DMA2_Stream1,(u32)dma_buf,sizeof(dma_buf)/2);    //��ʼһ��DMA���䣡	��dma�м�������ndtr - 1��
 // TIM8->ARR = 1400;//���ڴ���Ӱ�ӼĴ�����������Ҫ��ǰ��ֵ��Ӧ���������һ���������飬�����һ���ᱻ��������arr���õıȽ�Сʱ������˵1�����������⣩
  TIM8->EGR = 1;//���������¼� ��dma�м�����-- ����arr��ֵ���䵽Ӱ�ӼĴ��������������ã��������arr��0�����µ�Ӱ�ӼĴ�������0���������á�ͬʱ����һ��dma���䵽arr��ͬʱ�����ظ��Ĵ���������pscҲ����µ�Ӱ�ӼĴ��������м�������0��ʼ����Щ���Ǹ����¼��Ĺ���
	
	
}
//#endif
