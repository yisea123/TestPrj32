/**
  ******************************************************************************
  * @file    /uart3.c 
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
	
	#include "uart3.h"

#define EN_USART3_RX 1
#define EN_USART3_485 1

//485ģʽ����
#if EN_USART3_485
#if _NPC_VERSION_ == 1u
#define USART3_TX_ENABLE		GPIO_SetBits(GPIOA,GPIO_Pin_3)	//485ģʽ����.0,����;1,����.
#define USART3_TX_DISABLE		GPIO_ResetBits(GPIOA,GPIO_Pin_3)	//485ģʽ����.0,����;1,����.

#else

#define USART3_TX_ENABLE		GPIO_SetBits(GPIOF,GPIO_Pin_0)	//485ģʽ����.0,����;1,����.
#define USART3_TX_DISABLE		GPIO_ResetBits(GPIOF,GPIO_Pin_0)	//485ģʽ����.0,����;1,����.

#endif
#endif

#if 3 == CASCADE_USART
#define BUF_LEN 255
#else
#define BUF_LEN 16
#endif
//���ջ����� 
u8 USART3_RX_BUF[BUF_LEN];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 USART3_RX_CNT=0; 
u8 USART3_REC_OVER=0;

u8 *g_pUsart3RxBuf = USART3_RX_BUF;
u8 g_Usart3BufLen = BUF_LEN;
u8 g_Usart3RxLen = 0;

u8 GetUsartRxLen(void)
{
	if(g_pUsart3RxBuf == USART3_RX_BUF)
	  return g_Usart3RxLen;
	else
		return 0;
}

void SetUsartRxLen(u8 len)
{
	if(g_pUsart3RxBuf == USART3_RX_BUF)
	  g_Usart3RxLen = len;
}


void USART3_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//��������ж�
	//NEW08U(g_pUsart3RxBuf,len);
	g_pUsart3RxBuf = buf;
	g_Usart3BufLen = len;
	g_Usart3RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�
	
}

void USART3_RxDeInit(void)
{
	g_pUsart3RxBuf = NULL;
	g_Usart3BufLen = 0;
	g_Usart3RxLen = 0;
}

//void USART3_RxInit(u8 len)
//{
//	NEW08U(g_pUsart3RxBuf,len);
//	g_Usart3BufLen = len;
//	g_Usart3RxLen = 0;
//}

//void USART3_RxDeInit(void)
//{
//	DELETE(g_pUsart3RxBuf);
//	g_Usart3BufLen = 0;
//	g_Usart3RxLen = 0;
//}

//u32 rxlen=0;
/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
#if 3 == MAIN_COM
	OS_ERR err;
	OSIntEnter();                                           /*�����ж�*/
  USARTx_IRQHandler(USART3,3);
	OSIntExit();    	    
#else
	
	u8 res;
	OSIntEnter();
	tm2Re = ReadClock1ms();
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
	{
	  res =USART_ReceiveData(USART3);//;��ȡ���յ�������USART3->DR
		
		(g_pUsart3RxBuf)[g_Usart3RxLen]=res;
		g_Usart3RxLen++;
		
		if(g_Usart3RxLen >= g_Usart3BufLen)
			g_Usart3RxLen = 0;
		
	}
  OSIntExit();
#endif
//	
//	u8 res;
//	OSIntEnter();
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
//	{
//	  res =USART_ReceiveData(USART3);//;��ȡ���յ�������USART3->DR
//		
//		if(g_Usart3BufLen) {
//			g_pUsart3RxBuf[g_Usart3RxLen]=res;
//			g_Usart3RxLen++;
//			
//			if(g_Usart3RxLen >= g_Usart3BufLen)
//				g_Usart3RxLen = 0;
//		}
//	}
//  OSIntExit();
}
/**
  *USART3����
  */
void USART3_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {
#if _NPC_VERSION_ == 1u 
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1����*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	
  //����3���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	
	//USART3    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
#if EN_USART3_485
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PG8
#endif
	
   //USART3 ��ʼ������
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
	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
  USART3_TX_DISABLE;
  DMA_Config(DMA1_Stream3,DMA_Channel_4,(u32)&USART3->DR,(u32)0,0);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA���� 
#else
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1����*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //����ΪTX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //����ΪRX
	
	//USART3�˿�����TX //RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
#if EN_USART3_485
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
#endif
	
   //USART3 ��ʼ������
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
	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
  USART3_TX_DISABLE;
  DMA_Config(DMA1_Stream3,DMA_Channel_4,(u32)&USART3->DR,(u32)0,0);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����  
#endif
}
/**
  *USART3����
  */
void DMA_usart3Send(CDV_INT32U mar,CDV_INT16U ndtr){
	OS_ERR  err;
#if EN_USART3_485
	CPU_SR_ALLOC();
#endif
	USART3_TX_ENABLE;
#if EN_USART3_485
	OSSchedLock(&err);
#endif
	DMA_MemoryTargetConfig(DMA1_Stream3,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream3,ndtr);    //��ʼһ��DMA���䣡	  
  
	while(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)==RESET) {};	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
//	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART3_TX_DISABLE;
#if EN_USART3_485
	OSSchedUnlock(&err);
#endif
}

void USART3_Send(u8 *buf,u16 len)
{
//	u16 t;
//  for(t=0;t<len;t++)		//ѭ����������
//	{
//    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���
//    USART_SendData(USART3,buf[t]); //��������
//	}
//	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���
	DMA_usart3Send((CDV_INT32U)buf, len);
}

//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void USART3_Receive_Data(u8 *buf,u32 *len)
{
	u32 rxlen=USART3_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(20);		//�ȴ�20ms,��������20msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==USART3_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART3_RX_BUF[i];	
			USART3_RX_BUF[i]=0;
		}		
		*len=USART3_RX_CNT;	//��¼�������ݳ���
		USART3_RX_CNT=0;		//����
		USART3_REC_OVER=1;
	}
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
//u8 USART3_Receive(u8 **buf,u32 *len)
//{
//	u8 preCnt = 0;
//	u32 startTime ,endTime , time;
//	
//	if(NULL != *buf) {
//		
//		return 0;
//	}
//	
//	startTime = GetCdvTimeTick();
//	
//	do {
//		endTime = GetCdvTimeTick();
//		time = CalcTimeMS(endTime , startTime);
//		
//		if (time > 20) {
//			return 0;
//		}
//		
//	}while(g_Usart3RxLen == 0 );
//	
//	do {
//		endTime = GetCdvTimeTick();
//		time = CalcTimeMS(endTime , startTime);
//		if(preCnt == g_Usart3RxLen) {
//			
//		} else {
//			startTime = endTime;
//			preCnt = g_Usart3RxLen;
//		}
//	}while(time < 3);
//	
//	g_Usart3BufLen = 0;//��ֹ���ܵ�����
//	
//	*buf = g_pUsart3RxBuf;
//	*len = g_Usart3RxLen;
//	
//	return 1;
//}

/**
  * @brief  ���� ����
  * @param  buf     Ҫ���͵��ֽڴ�
  *         len     Ҫ���͵��ֽڴ����ȣ���λB
  * @retval void
  * @note   
  */
u8 USART3_Receive(u8 *len)
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
		delay_ms(1);
	}while(g_Usart3RxLen == 0 );
	
	do {
		endTime = GetCdvTimeTick();
		time = CalcTimeMS(endTime , startTime);
		if(preCnt == g_Usart3RxLen) {
			
		} else {
			startTime = endTime;
			time = 0;
			preCnt = g_Usart3RxLen;
		}
		delay_ms(1);
	}while(time < 3);
	
	//g_Usart3BufLen = 0;//��ֹ���ܵ�����
	
	//*buf = g_pUsart3RxBuf;
	*len = g_Usart3RxLen;
	
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
void USART3_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSMutexPend(&COM_SEM[2],0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	USART3_RxInit(rxbuf ,rxbufLen);
	USART3_Send(txbuf ,txlen);
	USART3_Receive(rxlen);
	USART3_RxDeInit();
	OSMutexPost (&COM_SEM[2],OS_OPT_POST_NO_SCHED,&err); 
}