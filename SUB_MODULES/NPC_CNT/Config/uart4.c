/**
  ******************************************************************************
  * @file    /uart4.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2019-8-2
  * @brief   a package of uart4 CONFIG
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
	
	#include "uart4.h"
	#include "dmax.h"

//485ģʽ����

  #define EN_UART4_485 0

#if EN_UART4_485
		#define UART4_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_5)	//485ģʽ����.0,����;1,����.
		#define UART4_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_5)	//485ģʽ����.0,����;1,����.
    #define UART4_TX_DISABLED     (0 == GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5))
#else
		#define UART4_TX_ENABLE
		#define UART4_TX_DISABLE
//    #define UART4_TX_DISABLED     1
#endif


//	#define DMA1_Stream4 DMA1_Stream4
//	#define DMA_Channel_4 DMA_Channel_4
//	#define DMA1_Stream2 DMA1_Stream2
//	#define DMA_Channel_4 DMA_Channel_4
	
//���ջ����� 	
QUEUE uart4_queue = {0};
/**
  *UART4dma����ʹ��
  */
void DMA_UART4_RecvEnable(u32 mar,u16 ndtr) {
	//DMA_MemoryTargetConfig(DMA2_Stream1,mar,DMA_Memory_0);
	DMA_Enable(DMA1_Stream2,mar,ndtr);    //��ʼһ��DMA���䣡	  
}

// ���ý���DMA
void UART4_RxReset(void) {
	USART_DMACmd(UART4,USART_DMAReq_Rx,DISABLE);

	DMA_UART4_RecvEnable((u32)(QUEUE_ING_BUF(uart4_queue)), QUEUE_BUF_LENGTH);
	
	USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
}

/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void) {
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)	{  
			UART4->SR;  
			UART4->DR; //��USART_IT_IDLE��־  
		
			//�رս���DMA  
			DMA_Cmd(DMA1_Stream2,DISABLE);  
			//�����־λ  
			DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF2);  

			//��ý���֡֡��  
		  QUEUE_ING_LEN(uart4_queue) = QUEUE_BUF_LENGTH - DMA_GetCurrDataCounter(DMA1_Stream2);  
		  
		  QUEUE_ING_NEXT(uart4_queue);
			//DMA_usart2RecvEnable((u32)(QUEUE_ING_BUF(uart4_queue)), QUEUE_BUF_LENGTH);
		  UART4_RxReset();
//		
//			//���ô������ݳ���  
//			DMA_SetCurrDataCounter(DMA2_Stream1,g_Uart2BufLen);  
//			//��DMA  
//			DMA_Cmd(DMA2_Stream1,ENABLE);  
	}
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET) { 
		USART_ITConfig(UART4,USART_IT_TC,DISABLE);  
		UART4_TX_DISABLE;//�л�������
		
	}
} 

void DMA1_Stream4_IRQHandler(void) {
	
	if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4) != RESET)  
	{
		/* �����־λ */
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);  
		/* �ر�DMA */
		DMA_Cmd(DMA1_Stream4,DISABLE);
		/* �򿪷�������ж�,ȷ�����һ���ֽڷ��ͳɹ� */
		USART_ITConfig(UART4,USART_IT_TC,ENABLE);  
			
  }  
	
}


/**
  *UART4����
  */
void UART4_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1����*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //����ΪTX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //����ΪRX
	
	//UART4�˿�����TX //RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
#if EN_UART4_485
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOH,&GPIO_InitStructure);
#endif
	
   //UART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = wordLength;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = stopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = parity;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
	//USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

  // dma tx
  UART4_TX_DISABLE;
  DMA_ConfigDir(DMA1_Stream4,DMA_Channel_4,(u32)&UART4->DR,(u32)0,0,DMA_DIR_MemoryToPeripheral);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);  // ��dam tc�жϣ������ж�dma�������

	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA���� 
	
	/* �����־λ */
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);  
		/* �ر�DMA */
		DMA_Cmd(DMA1_Stream4,DISABLE);
		
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
	DMA_ConfigDir(DMA1_Stream2,DMA_Channel_4,(u32)&UART4->DR,(u32)0,0,DMA_DIR_PeripheralToMemory);
	//USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
	

	UART4_RxReset();
	
}
/**
  *UART4����
  */
int DMA_UART4_Send(u32 mar,u16 ndtr) {

	if(/*DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)==RESET 
		|| */USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET
	  || DMA_GetCmdStatus(DMA1_Stream4) != DISABLE)
		return -1; // �ϴ�δ�������
#if EN_UART4_485
	if(!UART4_TX_DISABLED)
		return - 1;// δ�л������գ����ܸշ�����
#endif
	UART4_TX_ENABLE;
	
	DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream4, mar, ndtr);    //��ʼһ��DMA���䣡	  
  
//#if EN_UART4_TCIF
//	//while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {TaskSched();};
//#else
//	while(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF4)==RESET) {};	
//		
//	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {};
////	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF4);
////	USART_ClearFlag(UART4, USART_FLAG_TC);
//	
//  gstartTime = GetCdvTimeTick();
//	////delay_ms(10);
//	UART4_TX_DISABLE;
//#endif

return 0;
}



int UART4_Send(u8 *buf,u16 len) {
	return DMA_UART4_Send((u32)buf, len);
}

/////////////////////////////////////////////////////
/**
  * @brief  ����
  * @param  
  * @retval int �ж��Ƿ���յ���0 �ɹ��� -1 ʧ��
  * @note   
  */
int UART4_Receive(void) {
	if (QUEUE_HAD(uart4_queue)) {
//		UART4->SR;  
//		UART4->DR; //��USART_IT_IDLE��־  
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
//	if(1 == UART4_TR()) {
//		 // do something
//	}
//}
//  */
//int UART4_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen) {
//	static int stat = 0;  // ��¼״̬
//	static u32 ticks = 0; // ���㳬ʱ
//	static u16 crc;
//	
//	if(stat == 0 && txbuf && txlen) { // ����
//		QUEUE_CLEAR(uart4_queue);
//		UART4_RxReset();
//	  UART4_Send(txbuf ,txlen);
//		ticks = sys_ticks;
//		
//	}
//	
//	if(0 == (stat = UART4_Receive()) && rxlen) { //����
//		rxbuf = QUEUE_DO_BUF(uart4_queue);
//		*rxlen = QUEUE_DO_LEN(uart4_queue);
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
  while ( 1 != UART4_RT) {
    // do something
  }
}
  */
	

int UART4_RT(int (*p_cmd)(u8 *,u16  ,u8** ,u16* )) {
	static int stat = 0;  // ��¼״̬
	//static u32 ticks = 0; // ���㳬ʱ
	static u16 crc;
	
	u8* rxbuf = NULL ;
	u16 rxlen = 0;
	
	static u8* rtbuf = NULL ;
	static u16 rtlen = 0;

	
	if(rtbuf && rtlen) {
		if (0 == UART4_Send(rtbuf ,rtlen)) {//�������
		  rtbuf = NULL ;
	    rtlen = 0;
	    return 1;// ���������յ�����
		} else {
			return -2; // �����쳣
		}
	}
	
	if(0 == (stat = UART4_Receive())) { //����
		
		rxbuf = QUEUE_DO_BUF(uart4_queue);
		rxlen = QUEUE_DO_LEN(uart4_queue);
		
		if(rxlen > 2)
		  crc = MODBUS_CRC16(rxbuf, -2+rxlen,0xFFFF);
		else
			crc = 0;
		
		//if(crc == *(u16*)(rxbuf+rxlen-2)) {
			p_cmd(rxbuf,rxlen,&rtbuf,&rtlen);//���������rtbuf�û����д���
			
//			if(rtbuf && rtlen) {
//				UART4_Send(rtbuf ,rtlen);
//			}
//			return 1;// ���������յ�����
		//}
		QUEUE_DO_NEXT(uart4_queue);
	}
//	else if( CalcCount(sys_ticks, ticks) > 2) { // ���ճ�ʱ
//		stat = 0;
//	}
	//DelayTick(5);
	return stat;// ��
}


