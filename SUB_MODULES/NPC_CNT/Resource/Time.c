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





RET_STATUS TimeCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg) { 
//	OS_ERR err;
	//CPU_SR_ALLOC();//OS_CRITICAL_ENTER()
	CDV_INT08U opt , type /*, strLen*/;
	CDV_INT32U no ,valNo = 0; 
	CDV_INT32S num;
	//strLen = rxLen - 3;//���ĳ���
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
		case 0x00:/*��ʱ*/
			num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
			TimeDelay(no,num);
			break;
		case 0x01:/*��������*/
			num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
		  if(!num)
				return OPT_FAILURE;
			break;
		case 0x02:/*��ʼ��ʱ*/
		  StartCdvTime(no);
			break;
		case 0x03:/*��ȡ��ʱ*/
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

RET_STATUS ClockCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg) { 
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
		case 0x00:/*��1ms*/
		  switch(type) {
				case 0xFE:
				  ValSet(valNo, ReadClock1ms());
					break;
			}
			break;
		case 0x01:/*��10ms*/
		  switch(type) {
				case 0xFE:
				  ValSet(valNo, ReadClock10ms());
					break;
			}
			break;
		case 0x02:/*��100ms*/
		  switch(type) {
				case 0xFE:
				  ValSet(valNo, ReadClock100ms());
					break;
			}
			break;
		case 0x03:/*��1s*/
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
		default:
			break;
		
	}
	return OPT_SUCCESS;
}

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
  which ѡ���ȡ�ļ�ʱ�����͡�0 - 1ms��1 - 10ms��2 - 100ms��3 - 1s
return:
  ��ѡ��ļ�ʱ���ĵ�ǰ����ֵ��һ���ǿ�������ȡʱ��һ������ֵ��������Ŀ���
*/
CDV_INT32S ReadClock(CDV_INT08U which) {
	return p_ReadClock[which]();
}
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
	
	
	OSIntExit();    	//�˳��ж�
}
