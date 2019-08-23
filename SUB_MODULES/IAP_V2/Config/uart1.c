/**
  ******************************************************************************
  * @file    /Uart1.c 
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
	
	#include "uart1.h"

#define EN_USART1_RX 1
#define EN_USART1_485 1
//485ģʽ����

#if _NPC_VERSION_ == 1u
	#undef EN_USART1_485
#endif

#if EN_USART1_485
	#if _NPC_VERSION_ == 2u
		#ifdef NPC_V2_2
			#define USART1_TX_ENABLE		GPIO_SetBits(GPIOB,GPIO_Pin_14)	//485ģʽ����.0,����;1,����.
			#define USART1_TX_DISABLE		GPIO_ResetBits(GPIOB,GPIO_Pin_14)	//485ģʽ����.0,����;1,����.
		#else
			#define USART1_TX_ENABLE		GPIO_SetBits(GPIOA,GPIO_Pin_11)	//485ģʽ����.0,����;1,����.
			#define USART1_TX_DISABLE		GPIO_ResetBits(GPIOA,GPIO_Pin_11)	//485ģʽ����.0,����;1,����.
		#endif
	#elif _NPC_VERSION_ == 3u
		#define USART1_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_9)	//485ģʽ����.0,����;1,����.
		#define USART1_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_9)	//485ģʽ����.0,����;1,����.
	#endif
#else
	#define USART1_TX_ENABLE
	#define USART1_TX_DISABLE
#endif


u8 *g_pUsart1RxBuf = NULL;
u8 g_Usart1BufLen = 0;
u8 g_Usart1RxLen = 0;

void USART1_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//��������ж�
	//NEW08U(g_pUsart1RxBuf,len);
	g_pUsart1RxBuf = buf;
	g_Usart1BufLen = len;
	g_Usart1RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
	
}

void USART1_RxDeInit(void)
{
	g_pUsart1RxBuf = NULL;
	g_Usart1BufLen = 0;
	g_Usart1RxLen = 0;
}


/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)                	             /*����1�жϷ������*/
{
#if 1 == MAIN_COM
	OS_ERR err;
	OSIntEnter();                                           /*�����ж�*/
	USARTx_IRQHandler(USART1);
	OSIntExit();    	    
#else
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
	{
	  res =USART_ReceiveData(USART1);//;��ȡ���յ�������USART3->DR
		
		(g_pUsart1RxBuf)[g_Usart1RxLen]=res;
		g_Usart1RxLen++;
		
		if(g_Usart1RxLen >= g_Usart1BufLen)
			g_Usart1RxLen = 0;
		
	}
  OSIntExit();
#endif
//	//CPU_SR_ALLOC();
//	CDV_INT08U Res;//, len = 0;
////	CDV_INT16U crc;
//	OS_ERR err;
//  OSTmrStart(&tmr1,&err);                                 /*������ʱ�� 1*/
//	OSIntEnter();                                           /*�����ж�*/

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)   /*�����ж�()*/
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		Res =USART_ReceiveData(USART1);                       /*(USART1->DR);	//��ȡ���յ�������*/
//		switch(g_cdvStat){
//			case CDV_RECV:
//				g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]++] = Res;//���浽����
//			  if (QUE_LEN <=  g_scriptRecv.len[g_scriptRecv.rxPos])
//					MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
//				break;
//			default:
//				USART_RX_BUF_ADD_CHAR(Res);
//				break;
//		}
//  } 
//	
//	OSIntExit();    	                                      /*�˳��ж�*/

} 

/**
  *USART1����
  */
void USART1_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {


#if _NPC_VERSION_ == 1u 
#if defined(CDV_V1)	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1); //GPIOB6����ΪUSART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); //GPIOB7����ΪUSART1
	
	//USART1�˿�����TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOB6��GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA9��PA10
  //RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //GPIOB6��GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA9��PA10
   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = wordLength;//USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = stopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = parity;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
#if 1	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif

  DMA_Config(DMA2_Stream7,DMA_Channel_4,(CDV_INT32U)&USART1->DR,(CDV_INT32U)0,0);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA���� 
#endif
	
	
#elif _NPC_VERSION_ == 2u
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1����*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
  //RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	#if EN_USART1_485
		#ifdef NPC_V2_2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
		#else
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		#endif

	#endif
	
   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = wordLength;//USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = stopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = parity;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
	#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

	#endif
  USART1_TX_DISABLE;
  DMA_Config(DMA2_Stream7,DMA_Channel_4,(CDV_INT32U)&USART1->DR,(CDV_INT32U)0,0);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����    
	
#elif _NPC_VERSION_ == 3u

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1����*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
  //RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	#if EN_USART1_485
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	#endif
	
   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = wordLength;//USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = stopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = parity;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
	#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

	#endif
  USART1_TX_DISABLE;
  DMA_Config(DMA2_Stream7,DMA_Channel_4,(CDV_INT32U)&USART1->DR,(CDV_INT32U)0,0);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����    
	

#endif
}
/**
  *USART1����
  */
void DMA_usart1Send(CDV_INT32U mar,CDV_INT16U ndtr){
	
	USART1_TX_ENABLE;
	DMA_MemoryTargetConfig(DMA2_Stream7,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	DMA_Enable(DMA2_Stream7,ndtr);    //��ʼһ��DMA���䣡	  
  
	while(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)==RESET) {};	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
//	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART1_TX_DISABLE;
}

void USART1_Send(u8 *buf,u16 len)
{
//	u16 t;
//  for(t=0;t<len;t++)		//ѭ����������
//	{
//    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���
//    USART_SendData(USART1,buf[t]); //��������
//	}
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���
//		
	DMA_usart1Send((CDV_INT32U)buf, len);
}

/**
  * @brief  ���� ����
  * @param  buf     Ҫ���͵��ֽڴ�
  *         len     Ҫ���͵��ֽڴ����ȣ���λB
  * @retval void
  * @note   
  */
u8 USART1_Receive(u8 *len)
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
		
	}while(g_Usart1RxLen == 0 );
	
	do {
		endTime = GetCdvTimeTick();
		time = CalcTimeMS(endTime , startTime);
		if(preCnt == g_Usart1RxLen) {
			
		} else {
			startTime = endTime;
			time = 0;
			preCnt = g_Usart1RxLen;
		}
	}while(time < 3);
	
	//g_Usart1BufLen = 0;//��ֹ���ܵ�����
	
	//*buf = g_pUsart1RxBuf;
	*len = g_Usart1RxLen;
	
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
void USART1_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	USART1_RxInit(rxbuf ,rxbufLen);
	USART1_Send(txbuf ,txlen);
	USART1_Receive(rxlen);
	USART1_RxDeInit();
	OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
}



