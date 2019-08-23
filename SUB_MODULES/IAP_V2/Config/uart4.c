/**
  ******************************************************************************
  * @file    /uart4.c 
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
	
	#include "uart4.h"

#define EN_UART4_RX 1
#if _NPC_VERSION_ == 1u
  #define EN_UART4_485 1
#endif

//485ģʽ����
#if EN_UART4_485
#define UART4_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_10)	//485ģʽ����.0,����;1,����.
#define UART4_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_10)	//485ģʽ����.0,����;1,����.
#else
#define UART4_TX_ENABLE
#define UART4_TX_DISABLE
#endif

//���ջ����� 	
u8 UART4_RX_BUF[16];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 UART4_RX_CNT=0; 
u8 UART4_REC_OVER=0;
//u32 rxlen=0;
u8 *g_pUart4RxBuf = NULL;
u8 g_Uart4BufLen = 0;
u8 g_Uart4RxLen = 0;
void UART4_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);//��������ж�
	//NEW08U(g_pUart4RxBuf,len);
	g_pUart4RxBuf = buf;
	g_Uart4BufLen = len;
	g_Uart4RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�
	
}

void UART4_RxDeInit(void)
{
	g_pUart4RxBuf = NULL;
	g_Uart4BufLen = 0;
	g_Uart4RxLen = 0;
}
/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
//���ջ����� 	
u8 RS485_RX_BUF[16];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u32 RS485_RX_CNT=0;
void UART4_IRQHandler(void)
{
#if 4 == MAIN_COM
	OS_ERR err;
	OSIntEnter();                                           /*�����ж�*/
  USARTx_IRQHandler(UART4);
	OSIntExit();    	    
#else
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//���յ�����
	{
	  res =USART_ReceiveData(UART4);
		
		(g_pUart4RxBuf)[g_Uart4RxLen]=res;
		g_Uart4RxLen++;
		
		if(g_Uart4RxLen >= g_Uart4BufLen)
			g_Uart4RxLen = 0;
		
	}
  OSIntExit();
#endif
//	u8 res;
//	OSIntEnter();
//	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//���յ�����
//	{	 	
//		//Rec_Over=0;
//	  res =USART_ReceiveData(UART4);//;��ȡ���յ�������UART4->DR
//		RS485_RX_BUF[RS485_RX_CNT]=res;
//    RS485_RX_CNT=RS485_RX_CNT+1;
//    if(RS485_RX_CNT > 15)
//		  RS485_RX_CNT = 15;			
//	}
//  OSIntExit();
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
//	
//	USART_InitStructure.USART_BaudRate = bound;//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
#if EN_UART4_RX	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
  UART4_TX_DISABLE;
  DMA_Config(DMA1_Stream4,DMA_Channel_4,(u32)&UART4->DR,(u32)0,0);
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����  

}
/**
  *UART4����
  */
void DMA_usart4Send(CDV_INT32U mar,CDV_INT16U ndtr){
	
	UART4_TX_ENABLE;
	DMA_MemoryTargetConfig(DMA1_Stream4,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream4,ndtr);    //��ʼһ��DMA���䣡	  
  
	while(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)==RESET) {};	
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
//	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	UART4_TX_DISABLE;
}

void UART4_Send(u8 *buf,u16 len)
{
	/*u16 t;
  for(t=0;t<len;t++)		//ѭ����������
	{
    while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���
    USART_SendData(UART4,buf[t]); //��������
	}
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���*/
	
	//RS485_RX_CNT=0;		//����
	DMA_usart4Send((u32)buf, len);	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	
	u32 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	//delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
			RS485_RX_BUF[i]=0;
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
		//Rec_Over=1;
	}
}

/**
  * @brief  ���� ����
  * @param  buf     Ҫ���͵��ֽڴ�
  *         len     Ҫ���͵��ֽڴ����ȣ���λB
  * @retval void
  * @note   
  */
u8 UART4_Receive(u8 *len)
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
		
	}while(g_Uart4RxLen == 0 );
	
	do {
		endTime = GetCdvTimeTick();
		time = CalcTimeMS(endTime , startTime);
		if(preCnt == g_Uart4RxLen) {
			
		} else {
			startTime = endTime;
			time = 0;
			preCnt = g_Uart4RxLen;
		}
	}while(time < 3);
	
	//g_Uart4BufLen = 0;//��ֹ���ܵ�����
	
	//*buf = g_pUart4RxBuf;
	*len = g_Uart4RxLen;
	
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
void UART4_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	UART4_RxInit(rxbuf ,rxbufLen);
	UART4_Send(txbuf ,txlen);
	UART4_Receive(rxlen);
	UART4_RxDeInit();
	OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
}