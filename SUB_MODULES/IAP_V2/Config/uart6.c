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

#define EN_USART6_RX 1
#define EN_USART6_485 1

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
void USART6_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);//��������ж�
	//NEW08U(g_pUart6RxBuf,len);
	g_pUart6RxBuf = buf;
	g_Uart6BufLen = len;
	g_Uart6RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�
	
}

void USART6_RxDeInit(void)
{
	g_pUart6RxBuf = NULL;
	g_Uart6BufLen = 0;
	g_Uart6RxLen = 0;
}

/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USART6_IRQHandler(void)
{
#if 6 == MAIN_COM
	OS_ERR err;
	OSIntEnter();                                           /*�����ж�*/
  USARTx_IRQHandler(USART6);
	
	OSIntExit();    	    
#else
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)//���յ�����
	{
	  res =USART_ReceiveData(USART6);//;��ȡ���յ�������USART3->DR
		
		(g_pUart6RxBuf)[g_Uart6RxLen]=res;
		g_Uart6RxLen++;
		
		if(g_Uart6RxLen >= g_Uart6BufLen)
			g_Uart6RxLen = 0;
		
	}
  OSIntExit();
#endif
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
	
	
#endif

}
/**
  *USART6����
  */
void DMA_usart6Send(CDV_INT32U mar,CDV_INT16U ndtr){
	
	USART6_TX_ENABLE;
	DMA_MemoryTargetConfig(DMA2_Stream6,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
	USART_ClearFlag(USART6, USART_FLAG_TC);
	
	DMA_Enable(DMA2_Stream6,ndtr);    //��ʼһ��DMA���䣡	  
  
	while(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)==RESET) {};	
	while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
//	USART_ClearFlag(USART6, USART_FLAG_TC);
	
	USART6_TX_DISABLE;
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
	u8 preCnt = 0;
	u32 startTime ,endTime , time;
	
	if(NULL == len) {
		
		return 0;
	}
	
	startTime = GetCdvTimeTick();
	
	do {
		endTime = GetCdvTimeTick();
		time = CalcTimeMS(endTime , startTime);
		
		if (time > 500) {
			*len = 0;
			return 0;
			
		}
		
	}while(g_Uart6RxLen == 0 );
	
	do {
		endTime = GetCdvTimeTick();
		time = CalcTimeMS(endTime , startTime);
		if(preCnt == g_Uart6RxLen) {
			
		} else {
			startTime = endTime;
			time = 0;
			preCnt = g_Uart6RxLen;
		}
	}while(time < 3);
	
	//g_Uart6BufLen = 0;//��ֹ���ܵ�����
	
	//*buf = g_pUart6RxBuf;
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
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	USART6_RxInit(rxbuf ,rxbufLen);
	USART6_Send(txbuf ,txlen);
	USART6_Receive(rxlen);
	USART6_RxDeInit();
	OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
}

