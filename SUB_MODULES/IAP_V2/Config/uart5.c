/**
  ******************************************************************************
  * @file    /uart5.c 
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
	
	#include "uart5.h"

#define EN_UART5_RX 1
#define EN_UART5_485 0

//485ģʽ����
#if EN_UART5_485
#define UART5_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_12)	//485ģʽ����.0,����;1,����.
#define UART5_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_12)	//485ģʽ����.0,����;1,����.
#else
#define UART5_TX_ENABLE
#define UART5_TX_DISABLE
#endif

//���ջ����� 	
u8 UART5_RX_BUF[16];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 UART5_RX_CNT=0; 
u8 UART5_REC_OVER=0;
//u32 rxlen=0;
u8 *g_pUart5RxBuf = NULL;
u8 g_Uart5BufLen = 0;
u8 g_Uart5RxLen = 0;

void UART5_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);//��������ж�
	//NEW08U(g_pUart5RxBuf,len);
	g_pUart5RxBuf = buf;
	g_Uart5BufLen = len;
	g_Uart5RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�
	
}

void UART5_RxDeInit(void)
{
	g_pUart5RxBuf = NULL;
	g_Uart5BufLen = 0;
	g_Uart5RxLen = 0;
}
/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{            
#if 5 == MAIN_COM
	CDV_INT08U Res;
	OS_ERR err;
	OSIntEnter();                                           /*�����ж�*/
  USARTx_IRQHandler(UART5);
	
	OSIntExit();    	    
	
//	
//	CDV_INT08U Res;//, len = 0;
////	OS_ERR err;
//   // OSTmrStart(&tmr2,&err);                                 /*������ʱ�� 1*/
//	OSIntEnter();                                           /*�����ж�*/
//  tm2Re = 0;
//	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)   /*�����ж�()*/
//	{
//		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
//		Res =USART_ReceiveData(UART5);                       /*(USART2->DR);	//��ȡ���յ�������*/
//		switch(g_cdvStat){
//			case CDV_RECV:
//				g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]++] = Res;//���浽����
//				if (QUE_LEN <=  g_scriptRecv.len[g_scriptRecv.rxPos])
//					MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
//				break;
//			default:
//				USART_RX_BUF_ADD_CHAR(Res);
//				break;
//		}
//  }
//	
//	OSIntExit();    	                                      /*�˳��ж�*/
#else
	
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//���յ�����
	{
	  res =USART_ReceiveData(UART5);//;��ȡ���յ�������USART3->DR
		
		(g_pUart5RxBuf)[g_Uart5RxLen]=res;
		g_Uart5RxLen++;
		
		if(g_Uart5RxLen >= g_Uart5BufLen)
			g_Uart5RxLen = 0;
		
	}
  OSIntExit();
#endif
} 
/**
  *UART5����
  */
void UART5_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1����*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //����ΪTX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //����ΪRX
	
	//UART5�˿�����TX //RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
#if EN_UART5_485
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
#endif
	USART_Cmd(UART5, DISABLE);  //ʹ�ܴ���1 
	USART_DMACmd(UART5,USART_DMAReq_Tx,DISABLE);  //ʹ�ܴ���1��DMA����  
	
	
   //UART5 ��ʼ������
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
	USART_Init(UART5, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(UART5, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(UART5, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
#if EN_UART5_RX	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
  UART5_TX_DISABLE;
  DMA_Config(DMA1_Stream7,DMA_Channel_4,(u32)&UART5->DR,(u32)0,0);
	USART_DMACmd(UART5,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����  

}
/**
  *UART5����
  */
void DMA_usart5Send(CDV_INT32U mar,CDV_INT16U ndtr){
	
	UART5_TX_ENABLE;
	DMA_MemoryTargetConfig(DMA1_Stream7,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);
	USART_ClearFlag(UART5, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream7,ndtr);    //��ʼһ��DMA���䣡	  
  
	while(DMA_GetFlagStatus(DMA1_Stream7,DMA_FLAG_TCIF7)==RESET) {};	
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);
//	USART_ClearFlag(UART5, USART_FLAG_TC);
	
	UART5_TX_DISABLE;
}
/**
  * @brief  ���� ת��
  * @param  buf     Ҫ���͵��ֽڴ�
  *         len     Ҫ���͵��ֽڴ����ȣ���λB
  * @retval void
  * @note   
  */
void UART5_Send(u8 *buf,u16 len)
{
//	u16 t;
//  for(t=0;t<len;t++)		//ѭ����������
//	{
//    while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���
//    USART_SendData(UART5,buf[t]); //��������
//	}
//	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���
		
	DMA_usart5Send((CDV_INT32U)buf, len);
}
/**
  * @brief  ���� ����
  * @param  buf     Ҫ���͵��ֽڴ�
  *         len     Ҫ���͵��ֽڴ����ȣ���λB
  * @retval void
  * @note   
  */
u8 UART5_Receive(u8 *len)
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
		
	}while(g_Uart5RxLen == 0 );
	
	do {
		endTime = GetCdvTimeTick();
		time = CalcTimeMS(endTime , startTime);
		if(preCnt == g_Uart5RxLen) {
			
		} else {
			startTime = endTime;
			time = 0;
			preCnt = g_Uart5RxLen;
		}
	}while(time < 3);
	
	//g_Uart5BufLen = 0;//��ֹ���ܵ�����
	
	//*buf = g_pUart5RxBuf;
	*len = g_Uart5RxLen;
	
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
void UART5_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	UART5_RxInit(rxbuf ,rxbufLen);
	UART5_Send(txbuf ,txlen);
	UART5_Receive(rxlen);
	UART5_RxDeInit();
	OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
}
