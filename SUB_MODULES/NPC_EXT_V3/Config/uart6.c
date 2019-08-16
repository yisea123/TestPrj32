/**
  ******************************************************************************
  * @file    /uart6.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-2-9
  * @brief   a package of uart1 CONFIG
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
	
	#include "uart6.h"
	
	#define RECV_MSG_NUM 1
	OS_Q Recv_Msg;
	#define _NPC_VERSION_ 3u
	
#define EN_USART6_RX 1
#define EN_USART6_485 1

#if _NPC_VERSION_ == 3u
  #define EN_USART6_DMA 1u
	#define EN_USART6_TCIF 1u
#endif

//485ģʽ����
#if _NPC_VERSION_ == 1u
	#undef EN_USART6_485
#endif

#if EN_USART6_485
	#if _NPC_VERSION_ == 2u
		#define USART6_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_8)	//485ģʽ����.0,����;1,����.
		#define USART6_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_8)	//485ģʽ����.0,����;1,����.

	#elif _NPC_VERSION_ == 3u
		#define USART6_TX_ENABLE		GPIO_SetBits(GPIOB,GPIO_Pin_7)	//485ģʽ����.0,����;1,����.
		#define USART6_TX_DISABLE		GPIO_ResetBits(GPIOB,GPIO_Pin_7)	//485ģʽ����.0,����;1,����.
    #define USART6_TX_DISABLED     (0 == GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7))

	#endif
#else
	#define USART6_TX_ENABLE
	#define USART6_TX_DISABLE
#endif

//���ջ����� 	
u8 USART6_RX_BUF[16];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 USART6_RX_CNT=0; 
u8 USART6_REC_OVER=0;
////////
u8 *g_pUart6RxBuf = NULL;
u8 g_Uart6BufLen = 0;
u8 g_Uart6RxLen = 0;
//u32 rxlen=0;

/**
  *USART6dma����ʹ��
  */
void DMA_usart6RecvEnable(u32 mar,u16 ndtr){
	DMA_MemoryTargetConfig(DMA2_Stream1,mar,DMA_Memory_0);
	DMA_Enable(DMA2_Stream1,ndtr);    //��ʼһ��DMA���䣡	  
}


void USART6_RxInit(u8* buf ,u8 len)
{
#if EN_USART6_DMA
	OS_ERR err;
	USART_DMACmd(USART6,USART_DMAReq_Rx,DISABLE);
#else
	USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
#endif
	//NEW08U(g_pUart6RxBuf,len);
	g_pUart6RxBuf = buf;
	g_Uart6BufLen = len;
	g_Uart6RxLen = 0;
	
	if(NULL !=buf && 0 < len) {
#if EN_USART6_DMA
		DMA_usart6RecvEnable((CDV_INT32U)buf, len);
		USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);
		OSQFlush(&Recv_Msg, &err);
#else
		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�
#endif
	}
	
}

void USART6_RxDeInit(void)
{
	g_pUart6RxBuf = NULL;
	g_Uart6BufLen = 0;
	g_Uart6RxLen = 0;
}

u32 gstartTime ,gendTime , gtime;


/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USART6_IRQHandler(void)
{
	OS_ERR err;
#if 6 == MAIN_COM
	OSIntEnter();                                           /*�����ж�*/
  USARTx_IRQHandler(USART6, 6);
	
	OSIntExit();    	    
#else
	u8 res;
	OSIntEnter();
#if EN_USART6_DMA
	if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)  
	{  
			USART6->SR;  
			USART6->DR; //��USART_IT_IDLE��־  
			//�ر�DMA  
			DMA_Cmd(DMA2_Stream1,DISABLE);  
			//�����־λ  
			DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  

			//��ý���֡֡��  
			g_Uart6RxLen = g_Uart6BufLen - DMA_GetCurrDataCounter(DMA2_Stream1);  
		  OSQPost(&Recv_Msg, &g_Uart6RxLen, 1, OS_OPT_POST_FIFO, &err);
			if(g_Uart6RxLen == 0){
//				while (1) {
//					gendTime = GetCdvTimeTick();
//					gtime = CalcCount(gendTime , gstartTime);
//				}
			} else {
				gendTime = GetCdvTimeTick();
				gtime = CalcCount(gendTime , gstartTime);
			}
//		
//			//���ô������ݳ���  
//			DMA_SetCurrDataCounter(DMA2_Stream1,g_Uart6BufLen);  
//			//��DMA  
//			DMA_Cmd(DMA2_Stream1,ENABLE);  
	}
#if EN_USART6_TCIF
	if(USART_GetITStatus(USART6, USART_IT_TC) != RESET)  
	{ 
		USART_ITConfig(USART6,USART_IT_TC,DISABLE);  
		USART6_TX_DISABLE;
	}
#endif
//	if(USART_GetITStatus(USART1, USART_IT_TXE) == RESET)  
//    {  
//        /* �رշ�������ж�  */ 
//        USART_ITConfig(USART1,USART_IT_TC,DISABLE);  
//        /* �������  */
//       // UART1_Use_DMA_Tx_Flag = 0;  
//    }    
#else		
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)//���յ�����
	{
	  res =USART_ReceiveData(USART6);//;��ȡ���յ�������USART3->DR
		
		if(g_Uart6RxLen < g_Uart6BufLen) {
			(g_pUart6RxBuf)[g_Uart6RxLen]=res;
			g_Uart6RxLen++;
			
			if(g_Uart6RxLen == 1){
				
			gendTime = GetCdvTimeTick();
			gtime = CalcCount(gendTime , gstartTime);
			}
			
//			if(g_Uart6RxLen >= g_Uart6BufLen)
//				g_Uart6RxLen = 0;
		}
		
	}
#endif	
  OSIntExit();
#endif
} 

void DMA2_Stream6_IRQHandler(void) {
	 if(DMA_GetITStatus(DMA2_Stream6,DMA_IT_TCIF6) != RESET)   
    {  
        /* �����־λ */
        DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);  
        /* �ر�DMA */
        DMA_Cmd(DMA2_Stream6,DISABLE);
        /* �򿪷�������ж�,ȷ�����һ���ֽڷ��ͳɹ� */
        USART_ITConfig(USART6,USART_IT_TC,ENABLE);  
			
    }  
	
}


/**
  *USART6����
  */
void USART6_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {

#if _NPC_VERSION_ == 2u
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1����*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //����ΪTX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //����ΪRX
	//USART6�˿�����TX //RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
#if EN_USART6_485
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
#endif
	
   //USART6 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = wordLength;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = stopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = parity;//����żУ��λ
//	USART_InitStructure.USART_BaudRate = bound;//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART6, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
#if EN_USART6_RX	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
  USART6_TX_DISABLE;
  DMA_Config(DMA2_Stream6,DMA_Channel_5,(u32)&USART6->DR,(u32)0,0);
	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����  
	
#elif _NPC_VERSION_ == 3u
	 GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
#if EN_USART6_DMA
  OS_ERR err;
	OSQCreate(&Recv_Msg, "Recv_Msg", RECV_MSG_NUM, &err);
#endif

	/*usart1����*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //����ΪTX
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //����ΪRX
	//USART6�˿�����TX //RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
#if EN_USART6_485
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
#endif
	
   //USART6 ��ʼ������
	USART_InitStructure.USART_BaudRate = 460800;//921600;//bound;//����������
	USART_InitStructure.USART_WordLength = wordLength;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = stopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = parity;//����żУ��λ
//	USART_InitStructure.USART_BaudRate = bound;//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	//USART_OverSampling8Cmd
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART6, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
#if EN_USART6_RX	
	//USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
#endif

  USART6_TX_DISABLE;
  DMA_Config(DMA2_Stream6,DMA_Channel_5,(u32)&USART6->DR,(u32)0,0);
	
#if EN_USART6_TCIF
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	DMA_ITConfig(DMA2_Stream6, DMA_IT_TC, ENABLE);  // ��dam tc�жϣ������ж�dma�������
#endif

	USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA���� 
	
#if EN_USART6_DMA
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);
	DMA_ConfigDir(DMA2_Stream1,DMA_Channel_5,(u32)&USART6->DR,(u32)0,0,DMA_DIR_PeripheralToMemory);
	USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);
#endif 
	
	
#endif

}
/**
  *USART6����
  */
void DMA_usart6Send(CDV_INT32U mar,CDV_INT16U ndtr){
#if EN_USART6_485
	OS_ERR  err;
	CPU_SR_ALLOC();
#endif

#if EN_USART6_TCIF
	if(!USART6_TX_DISABLED)
		return;
#endif
	
	USART6_TX_ENABLE;
	
#if EN_USART6_485 && !EN_USART6_TCIF
	OSSchedLock(&err);
#endif
	
	
	DMA_MemoryTargetConfig(DMA2_Stream6,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
	USART_ClearFlag(USART6, USART_FLAG_TC);
	
	DMA_Enable(DMA2_Stream6,ndtr);    //��ʼһ��DMA���䣡	  
  
#if EN_USART6_TCIF
	while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET) {TaskSched();};
#else
	while(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)==RESET) {};	
		
	while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
//	USART_ClearFlag(USART6, USART_FLAG_TC);
	
  gstartTime = GetCdvTimeTick();
	//delay_ms(10);
	USART6_TX_DISABLE;
#endif
		
#if EN_USART6_485 && !EN_USART6_TCIF
	OSSchedUnlock(&err);
#endif
}



void USART6_Send(u8 *buf,u16 len)
{
//	u16 t;
//  for(t=0;t<len;t++)		//ѭ����������
//	{
//    while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���
//    USART_SendData(USART6,buf[t]); //��������
//	}
//	while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���
	
	DMA_usart6Send((CDV_INT32U)buf, len);
}

/////////////////////////////////////////////////////
/**
  * @brief  ���� ����
  * @param  buf     Ҫ���͵��ֽڴ�
  *         len     Ҫ���͵��ֽڴ����ȣ���λB
  * @retval void
  * @note   
  */
u8 USART6_Receive(u8 *len)
{
	u32 startTime ,endTime , time = 100;
	u8 preCnt = 0;
#if EN_USART6_DMA
  OS_ERR err;
	OS_MSG_SIZE size;
	u8* ptr;
#endif	
	if(NULL == len) {
		
		return 0;
	}
#if EN_USART6_DMA
	ptr = OSQPend(&Recv_Msg, 500, OS_OPT_PEND_BLOCKING, &size, 0, &err);

	if (err != OS_ERR_NONE || *ptr == 0) {
		USART6->SR;  
		USART6->DR; //��USART_IT_IDLE��־  
		//�ر�DMA  
		DMA_Cmd(DMA2_Stream1,DISABLE);  
		//�����־λ  
		DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  
		*len = 0;
		return 0;
	}
//	
//	startTime = GetCdvTimeTick();
//	
//	do {
//		endTime = GetCdvTimeTick();
//		time = CalcCount(endTime , startTime);
//		
//		if (time > 500) {
//			*len = 0;
//			return 0;
//			
//		}
//		//delay_ms(1);
//	} while(g_Uart6RxLen == 0 );
	
#else
	
	startTime = GetCdvTimeTick();
	
	do {
		endTime = GetCdvTimeTick();
		time = CalcCount(endTime , startTime);
		
		if (time > 500) {
			*len = 0;
			return 0;
			
		}
		//delay_ms(1);
	} while(g_Uart6RxLen == 0 );
	startTime = GetCdvTimeTick();
	do {
		endTime = GetCdvTimeTick();
		//time = CalcTimeMS(endTime , startTime);
		time = CalcCount(endTime , startTime);
		if(preCnt == g_Uart6RxLen) {
			
		} else {
			startTime = endTime;
			time = 0;
			preCnt = g_Uart6RxLen;
		}
		
		//delay_ms(1);
	} while(time < 20);
	
	//g_Uart6BufLen = 0;//��ֹ���ܵ�����
	
	//*buf = g_pUart6RxBuf;

#endif	
	*len = g_Uart6RxLen;
	return 1;
}

/**
  * @brief  ���� ���ͼ����� �ķ�װ����
  * @param  txbuf  Ҫ���͵��ַ���
  *         txlen  Ҫ���͵��ַ�������
  *         rxbuf  ���յ��ַ�������
  *         rxbufLen �����ַ�������ĳ���
  *         rxlen  ���յ����ַ�������
  * @retval void
  * @note   
  */
void USART6_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSMutexPend(&COM_SEM[5],0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	USART6_RxInit(rxbuf ,rxbufLen);
	USART6_Send(txbuf ,txlen);
	USART6_Receive(rxlen);
	USART6_RxDeInit();
	//DelayTick(5);
	OSMutexPost (&COM_SEM[5],OS_OPT_POST_NO_SCHED,&err); 
}

