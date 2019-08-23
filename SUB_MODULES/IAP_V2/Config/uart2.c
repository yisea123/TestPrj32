/**
  ******************************************************************************
  * @file    /uart2.c 
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
	
	#include "uart2.h"


#define EN_USART2_RX 1
#define EN_USART2_485 0

//485ģʽ����
#if _NPC_VERSION_ == 1u
	#undef EN_USART2_485
#endif

#if EN_USART1_485
#define USART2_TX_ENABLE		GPIO_SetBits(GPIOD,GPIO_Pin_5)	//485ģʽ����.0,����;1,����.
#define USART2_TX_DISABLE		GPIO_ResetBits(GPIOD,GPIO_Pin_5)	//485ģʽ����.0,����;1,����.
#else
#define USART2_TX_ENABLE
#define USART2_TX_DISABLE
#endif

u8 *g_pUart2RxBuf = NULL;
u8 g_Uart2BufLen = 0;
u8 g_Uart2RxLen = 0;

void USART2_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//��������ж�
	//NEW08U(g_pUart2RxBuf,len);
	g_pUart2RxBuf = buf;
	g_Uart2BufLen = len;
	g_Uart2RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
	
}

void USART2_RxDeInit(void)
{
	g_pUart2RxBuf = NULL;
	g_Uart2BufLen = 0;
	g_Uart2RxLen = 0;
}


/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
//CDV_INT32U fpga_i = 0;
void USART2_IRQHandler(void)                	             /*����1�жϷ������*/
{
#if 2 == MAIN_COM
	OS_ERR err;
	OSIntEnter();                                           /*�����ж�*/
	USARTx_IRQHandler(USART2);
	OSIntExit();    	    
#else
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
	{
	  res =USART_ReceiveData(USART2);//;��ȡ���յ�������USART3->DR
		
		(g_pUart2RxBuf)[g_Uart2RxLen]=res;
		g_Uart2RxLen++;
		
		if(g_Uart2RxLen >= g_Uart2BufLen)
			g_Uart2RxLen = 0;
		
	}
  OSIntExit();
#endif
//	CDV_INT08U Res;//, len = 0;
//	OSIntEnter();                                           /*�����ж�*/
//  tm1Re = 0;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   /*�����ж�()*/
//	{
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//		Res =USART_ReceiveData(USART2);                       /*(USART2->DR);	//��ȡ���յ�������*/
//			switch(g_cdvStat){
//				case CDV_RECV:
//					g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]++] = Res;//���浽����
//				  if (QUE_LEN <=  g_scriptRecv.len[g_scriptRecv.rxPos])
//						MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
//					break;
//				default:
//					USART_RX_BUF_ADD_CHAR(Res);
//					break;
//			}
//  } 
//	
//	OSIntExit();    	                                      /*�˳��ж�*/

} 

/*USART����*/
void USART2_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOD5����ΪUSART2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOD6����ΪUSART2
	
	//USART1�˿�����TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //GPIOD5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA9��PA10
  //RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA9��PA10
		
#if EN_USART2_485
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
#endif

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = wordLength;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = stopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = parity;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
  USART2_TX_DISABLE;
  DMA_Config(DMA1_Stream6,DMA_Channel_4,(CDV_INT32U)&USART2->DR,(CDV_INT32U)0,0);//DMA1,STEAM6,CH4,����Ϊ����2,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����  

}


void DMA_usart2Send(CDV_INT32U mar,CDV_INT16U ndtr){
	
	USART2_TX_ENABLE;
	DMA_MemoryTargetConfig(DMA1_Stream6, mar,DMA_Memory_0);	
	
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream6,ndtr);    //��ʼһ��DMA���䣡	  
  
  
  while(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)==RESET) {};
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
//	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	USART2_TX_DISABLE;
}


void USART2_Send(u8 *buf,u16 len)
{
	DMA_usart2Send((CDV_INT32U)buf, len);
}

/**
  * @brief  ���� ����
  * @param  buf     Ҫ���͵��ֽڴ�
  *         len     Ҫ���͵��ֽڴ����ȣ���λB
  * @retval void
  * @note   
  */
u8 USART2_Receive(u8 *len)
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
		
	}while(g_Uart2RxLen == 0 );
	
	do {
		endTime = GetCdvTimeTick();
		time = CalcTimeMS(endTime , startTime);
		if(preCnt == g_Uart2RxLen) {
			
		} else {
			startTime = endTime;
			time = 0;
			preCnt = g_Uart2RxLen;
		}
	}while(time < 3);
	
	//g_Uart2BufLen = 0;//��ֹ���ܵ�����
	
	//*buf = g_pUart2RxBuf;
	*len = g_Uart2RxLen;
	
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
void USART2_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	USART2_RxInit(rxbuf ,rxbufLen);
	USART2_Send(txbuf ,txlen);
	USART2_Receive(rxlen);
	USART2_RxDeInit();
	OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
}

