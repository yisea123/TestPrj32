/**
  ******************************************************************************
  * @file    /uart1.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2019-8-2
  * @brief   a package of uart1 CONFIG
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
	
	#include "uart1.h"
	#include "dmax.h"

//485ģʽ����

  #define EN_USART1_485 0

#if EN_USART1_485
		#define USART1_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_5)	//485ģʽ����.0,����;1,����.
		#define USART1_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_5)	//485ģʽ����.0,����;1,����.
    #define USART1_TX_DISABLED     (0 == GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5))
#else
		#define USART1_TX_ENABLE
		#define USART1_TX_DISABLE
//    #define USART1_TX_DISABLED     1
#endif

//���ջ����� 	
QUEUE uart1_queue = {0};
/**
  *USART1dma����ʹ��
  */
void DMA_USART1_RecvEnable(u32 mar,u16 ndtr) {
	//DMA_MemoryTargetConfig(DMA2_Stream1,mar,DMA_Memory_0);
	DMA_Enable(DMA1_Channel5,mar,ndtr);    //��ʼһ��DMA���䣡	  
}

// ���ý���DMA
void USART1_RxReset(void) {
	USART_DMACmd(USART1,USART_DMAReq_Rx,DISABLE);

	DMA_USART1_RecvEnable((u32)(QUEUE_ING_BUF(uart1_queue)), QUEUE_BUF_LENGTH);
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
}

/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void) {
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)	{  
			USART1->SR;  
			USART1->DR; //��USART_IT_IDLE��־  
		
			//�رս���DMA  
			DMA_Cmd(DMA1_Channel5,DISABLE);  
			//�����־λ  
			//DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  
		  DMA_ClearFlag(DMA1_FLAG_TC5);

			//��ý���֡֡��  
		  QUEUE_ING_LEN(uart1_queue) = QUEUE_BUF_LENGTH - DMA_GetCurrDataCounter(DMA1_Channel5);  
		  
		  QUEUE_ING_NEXT(uart1_queue);
			//DMA_usart2RecvEnable((u32)(QUEUE_ING_BUF(uart1_queue)), QUEUE_BUF_LENGTH);
		  USART1_RxReset();
//		
//			//���ô������ݳ���  
//			DMA_SetCurrDataCounter(DMA2_Stream1,g_Uart2BufLen);  
//			//��DMA  
//			DMA_Cmd(DMA2_Stream1,ENABLE);  
	}
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET) { 
		USART_ITConfig(USART1,USART_IT_TC,DISABLE);  
		USART1_TX_DISABLE;//�л�������
		
	}
} 

void DMA1_Channel4_IRQHandler(void) {
	 //if(DMA_GetITStatus(DMA2_Stream6,DMA_IT_TCIF6) != RESET)  

// dma tc �ж�
	 if(DMA_GetITStatus(DMA1_IT_TC4) != RESET) {  
        /* �����־λ */
        //DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);  
				DMA_ClearFlag(DMA1_FLAG_TC4);
        /* �ر�DMA */
        DMA_Cmd(DMA1_Channel4,DISABLE);
        /* �򿪷�������ж�,ȷ�����һ���ֽڷ��ͳɹ� */
        USART_ITConfig(USART1,USART_IT_TC,ENABLE);  
			
    }  
	
}


/**
  *USART1����
  */
void USART1_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*usart1����*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);   //GPIOAʱ��
	//RCC_APB1PeriphClockCmd(/*|RCC_APB2Periph_AFIO��Ҫ�õ��������ӳ�书��ʱ����Ҫʹ��AFIO��ʱ�ӣ�����û��remap��p180*/,ENABLE);  	//ʹ��USART1ʱ��
	USART_DeInit(USART1);  //��λ����2

#if EN_USART1_485
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PC5�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
	//����������������   ����2 PA2	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2

	//����2  PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��PA3

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);//��λ����2
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);//ֹͣ��λ
   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = wordLength;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = stopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = parity;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	//USART_OverSampling8Cmd
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ��� 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����


  USART1_TX_DISABLE; // ����ģʽ
	// ����DMA����
	DMA_ConfigDir(DMA1_Channel4,(u32)&USART1->DR,(u32)0,0, DMA_DIR_PeripheralDST);//����DMA����

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);  // ��dam tc�жϣ������ж�dma�������

	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA���� 
	// ����DMA����
//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	DMA_ConfigDir(DMA1_Channel5,(u32)&USART1->DR,(u32)0,0,DMA_DIR_PeripheralSRC);//����DMA
	
	USART1_RxReset();
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
}
/**
  *USART1����
  */
int DMA_USART1_Send(u32 mar,u16 ndtr) {

	if(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
		return -1; // �ϴ�δ�������
#if EN_USART1_485
	if(!USART1_TX_DISABLED)
		return - 1;// δ�л������գ����ܸշ�����
#endif
	USART1_TX_ENABLE;
	
	DMA_ClearFlag(DMA1_FLAG_TC4);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Channel4,mar,ndtr);    //��ʼһ��DMA���䣡	  
  
//#if EN_USART1_TCIF
//	//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {TaskSched();};
//#else
//	while(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)==RESET) {};	
//		
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {};
////	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
////	USART_ClearFlag(USART1, USART_FLAG_TC);
//	
//  gstartTime = GetCdvTimeTick();
//	////delay_ms(10);
//	USART1_TX_DISABLE;
//#endif

return 0;
}



int USART1_Send(u8 *buf,u16 len) {
	return DMA_USART1_Send((u32)buf, len);
}

/////////////////////////////////////////////////////
/**
  * @brief  ����
  * @param  
  * @retval int �ж��Ƿ���յ���0 �ɹ��� -1 ʧ��
  * @note   
  */
int USART1_Receive(void) {
	if (QUEUE_HAD(uart1_queue)) {
//		USART1->SR;  
//		USART1->DR; //��USART_IT_IDLE��־  
//		//�ر�DMA  
//		DMA_Cmd(DMA1_Channel5,DISABLE);  
//		//�����־λ  
//		DMA_ClearFlag(DMA1_FLAG_TC5);  
		
		return 0;
	}
	
	return -1;
}

///**
//  * @brief  ���� ���Ͳ����� �ķ�װ����
//  * @param  txbuf  Ҫ���͵��ַ���
//  *         txlen  Ҫ���͵��ַ�������
//  *         rxbuf  ���յ��ַ�������ָ��
//  *         rxlen  ���յ����ַ�������
//  * @retval int 1 ���յ����0 ���Է��ͣ�-1 ���ڽ���
//  * @note   test 02 02 00 00 00 0E F9 FD
//  *         ���͵ȴ�ʾ��
//while��1�� {
//	if(1 == USART1_TR()) {
//		 // do something
//	}
//}
//  */
//int USART1_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen) {
//	static int stat = 0;  // ��¼״̬
//	static u32 ticks = 0; // ���㳬ʱ
//	static u16 crc;
//	
//	if(stat == 0 && txbuf && txlen) { // ����
//		QUEUE_CLEAR(uart1_queue);
//		USART1_RxReset();
//	  USART1_Send(txbuf ,txlen);
//		ticks = sys_ticks;
//		
//	}
//	
//	if(0 == (stat = USART1_Receive()) && rxlen) { //����
//		rxbuf = QUEUE_DO_BUF(uart1_queue);
//		*rxlen = QUEUE_DO_LEN(uart1_queue);
//		
//		if(*rxlen > 2)
//		  crc = MODBUS_CRC16(rxbuf, -2+*rxlen,0xFFFF);
//		else
//			crc = 0;
//		
//		if(crc == *(u16*)(rxbuf+*rxlen-2))
//			return 1;// ���������յ�����
//		
//	}else if( CalcCount(sys_ticks, ticks) > 2) { // ���ճ�ʱ
//		stat = 0;
//	}
//	//DelayTick(5);
//	return stat;// ��
//}


/**
  * @brief  ���� ���ս������� �ķ�װ����
  * @param  
  * @retval int 1 ����������� 0 ���յ����� -1 δ���յ�����  -2 �����쳣
  * @note   
  *         ���ս�������ʾ��
while (1) {
  while ( 1 != USART1_RT) {
    // do something
  }
}
  */
	

int USART1_RT(int (*p_cmd)(u8 *,u16  ,u8** ,u16* )) {
	static int stat = 0;  // ��¼״̬
	//static u32 ticks = 0; // ���㳬ʱ
	static u16 crc;
	
	u8* rxbuf = NULL ;
	u16 rxlen = 0;
	
	static u8* rtbuf = NULL ;
	static u16 rtlen = 0;

	
	if(rtbuf && rtlen) {
		if (0 == USART1_Send(rtbuf ,rtlen)) {//�������
		  rtbuf = NULL ;
	    rtlen = 0;
	    return 1;// ���������յ�����
		} else {
			return -2; // �����쳣
		}
	}
	
	if(0 == (stat = USART1_Receive())) { //����
		
		rxbuf = QUEUE_DO_BUF(uart1_queue);
		rxlen = QUEUE_DO_LEN(uart1_queue);
		
		if(rxlen > 2)
		  crc = MODBUS_CRC16(rxbuf, -2+rxlen,0xFFFF);
		else
			crc = 0;
		
		if(crc == *(u16*)(rxbuf+rxlen-2)) {
			p_cmd(rxbuf,rxlen,&rtbuf,&rtlen);//���������rtbuf�û����д���
			
//			if(rtbuf && rtlen) {
//				USART1_Send(rtbuf ,rtlen);
//			}
//			return 1;// ���������յ�����
		}
		QUEUE_DO_NEXT(uart1_queue);
	}
//	else if( CalcCount(sys_ticks, ticks) > 2) { // ���ճ�ʱ
//		stat = 0;
//	}
	//DelayTick(5);
	return stat;// ��
}


