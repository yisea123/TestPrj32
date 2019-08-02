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
	
	#include "uart2.h"
	#include "dmax.h"
	
	#define RECV_MSG_NUM 1
	//OS_Q Recv_Msg;
	#define _NPC_VERSION_ 3u
	
#define EN_USART2_RX 1
#define EN_USART2_485 1

#if _NPC_VERSION_ == 3u
  #define EN_USART2_DMA 1u
	#define EN_USART2_TCIF 1u
#endif

//485ģʽ����

#if EN_USART2_485
		#define USART2_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_5)	//485ģʽ����.0,����;1,����.
		#define USART2_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_5)	//485ģʽ����.0,����;1,����.
    #define USART2_TX_DISABLED     (0 == GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5))
#endif

//���ջ����� 	
//u8 USART2_RX_BUF[16];  	//���ջ���,���64���ֽ�.
////���յ������ݳ���
//u8 USART2_RX_CNT=0; 
//u8 USART2_REC_OVER=0;
//////////
//u8 *g_pUart2RxBuf = NULL;
//u8 g_Uart2BufLen = 0;
//u8 g_Uart2RxLen = 0;
//u32 rxlen=0;
QUEUE uart2_queue = {0};
/**
  *USART2dma����ʹ��
  */
void DMA_usart2RecvEnable(u32 mar,u16 ndtr){
	//DMA_MemoryTargetConfig(DMA2_Stream1,mar,DMA_Memory_0);
	DMA_Enable(DMA1_Channel6,mar,ndtr);    //��ʼһ��DMA���䣡	  
}

// һ��һ�յ�ʹ��
void USART2_RxInit(void)
{
#if EN_USART2_DMA
	//OS_ERR err;
	USART_DMACmd(USART2,USART_DMAReq_Rx,DISABLE);
#else
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
#endif

#if EN_USART2_DMA
		//DMA_usart2RecvEnable((u32)buf, len);
		QUEUE_CLEAR(uart2_queue);
		DMA_usart2RecvEnable((u32)(QUEUE_ING_BUF(uart2_queue)), QUEUE_BUF_LENGTH);
		USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
//		OSQFlush(&Recv_Msg, &err);
#else
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
#endif
	//}
	
}

void USART2_RxDeInit(void)
{
//	g_pUart2RxBuf = NULL;
//	g_Uart2BufLen = 0;
//	g_Uart2RxLen = 0;
}

u32 gstartTime ,gendTime , gtime;


/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
	{  
			USART2->SR;  
			USART2->DR; //��USART_IT_IDLE��־  
		
			//�رս���DMA  
			DMA_Cmd(DMA1_Channel6,DISABLE);  
			//�����־λ  
			//DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  
		  DMA_ClearFlag(DMA1_FLAG_TC6);

			//��ý���֡֡��  
		  QUEUE_ING_LEN(uart2_queue) = QUEUE_BUF_LENGTH - DMA_GetCurrDataCounter(DMA1_Channel6);  
		  
		  QUEUE_ING_NEXT(uart2_queue);
			//DMA_usart2RecvEnable((u32)(QUEUE_ING_BUF(uart2_queue)), QUEUE_BUF_LENGTH);
//		
//			//���ô������ݳ���  
//			DMA_SetCurrDataCounter(DMA2_Stream1,g_Uart2BufLen);  
//			//��DMA  
//			DMA_Cmd(DMA2_Stream1,ENABLE);  
	}
#if EN_USART2_TCIF
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)  
	{ 
		USART_ITConfig(USART2,USART_IT_TC,DISABLE);  
		USART2_TX_DISABLE;//�л�������
		
	}
#endif
} 

void DMA1_Channel7_IRQHandler(void) {
	 //if(DMA_GetITStatus(DMA2_Stream6,DMA_IT_TCIF6) != RESET)  

// dma tc �ж�
	 if(DMA_GetITStatus(DMA1_IT_TC7) != RESET)  
    {  
        /* �����־λ */
        //DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);  
				DMA_ClearFlag(DMA1_FLAG_TC7);
        /* �ر�DMA */
        DMA_Cmd(DMA1_Channel7,DISABLE);
        /* �򿪷�������ж�,ȷ�����һ���ֽڷ��ͳɹ� */
        USART_ITConfig(USART2,USART_IT_TC,ENABLE);  
			
    }  
	
}


/**
  *USART2����
  */
void USART2_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*usart1����*/
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOB,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);   //GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2/*|RCC_APB2Periph_AFIO��Ҫ�õ��������ӳ�书��ʱ����Ҫʹ��AFIO��ʱ�ӣ�����û��remap��p180*/,ENABLE);  	//ʹ��USART2ʱ��
	USART_DeInit(USART2);  //��λ����2
//	//����1��Ӧ���Ÿ���ӳ��
//	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART2); //����ΪTX
//	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART2); //����ΪRX
//	//USART2�˿�����TX //RX
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOG,&GPIO_InitStructure);
//	
//#if EN_USART2_485
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//#endif
#if EN_USART2_485
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PC5�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
	//����������������   ����2 PA2	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2

	//����2  PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��PA3

  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ
   //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = wordLength;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = stopBits;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = parity;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	//USART_OverSampling8Cmd
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	//ʹ�ܴ��ڶ�
#if EN_USART2_RX	
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
#endif

  USART2_TX_DISABLE;
  //DMA_Config(DMA2_Stream6,DMA_Channel_5,(u32)&USART2->DR,(u32)0,0);
	DMA_ConfigDir(DMA1_Channel7,(u32)&USART2->DR,(u32)0,0, DMA_DIR_PeripheralDST);//����DMA����
#if EN_USART2_TCIF
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);  // ��dam tc�жϣ������ж�dma�������
#endif

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA���� 
	
#if EN_USART2_DMA
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	DMA_ConfigDir(DMA1_Channel6,(u32)&USART2->DR,(u32)0,0,DMA_DIR_PeripheralSRC);//����DMA
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
#endif 

}
/**
  *USART2����
  */
int DMA_usart2Send(u32 mar,u16 ndtr){

	if(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
		return -1; // �ϴ�δ�������
	
#if EN_USART2_TCIF
	if(!USART2_TX_DISABLED)
		return - 1;// δ�л������գ����ܸշ�����
#endif
	
	
	USART2_TX_ENABLE;
	
	DMA_ClearFlag(DMA1_FLAG_TC7);
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Channel7,mar,ndtr);    //��ʼһ��DMA���䣡	  
  
//#if EN_USART2_TCIF
//	//while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {TaskSched();};
//#else
//	while(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)==RESET) {};	
//		
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {};
////	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
////	USART_ClearFlag(USART2, USART_FLAG_TC);
//	
//  gstartTime = GetCdvTimeTick();
//	//delay_ms(10);
//	USART2_TX_DISABLE;
//#endif

return 0;
}



int USART2_Send(u8 *buf,u16 len)
{
	return DMA_usart2Send((u32)buf, len);
}

/////////////////////////////////////////////////////
/**
  * @brief  ����
  * @param  
  * @retval int �ж��Ƿ���յ���0 �ɹ��� -1 ʧ��
  * @note   
  */
int USART2_Receive(void)
{
	if (QUEUE_HAD(uart2_queue)) {
//		USART2->SR;  
//		USART2->DR; //��USART_IT_IDLE��־  
//		//�ر�DMA  
//		DMA_Cmd(DMA1_Channel6,DISABLE);  
//		//�����־λ  
//		DMA_ClearFlag(DMA1_FLAG_TC6);  
		
		return 0;
	}
	
	return -1;
}

/**
  * @brief  ���� ���ͼ����� �ķ�װ����
  * @param  txbuf  Ҫ���͵��ַ���
  *         txlen  Ҫ���͵��ַ�������
  *         rxbuf  ���յ��ַ�������
  *         rxlen  ���յ����ַ�������
  * @retval int 1 ���յ����0 ���Է��ͣ�-1 ���ڽ���
  * @note   test 02 02 00 00 00 0E F9 FD
  */
int USART2_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen)
{
	static int stat = 0;  // ��¼״̬
	static u32 ticks = 0; // ���㳬ʱ
	static u16 crc;
	
	if(stat == 0 && txbuf && txlen) {
	  USART2_RxInit();
	  USART2_Send(txbuf ,txlen);
		ticks = sys_ticks;
		
	}
	if(0 == (stat = USART2_Receive()) && rxlen) {
	  USART2_RxDeInit();
		rxbuf = QUEUE_DO_BUF(uart2_queue);
		*rxlen = QUEUE_DO_LEN(uart2_queue);
		if(*rxlen > 2)
		  crc = MODBUS_CRC16(rxbuf, -2+*rxlen,0xFFFF);
		else
			crc = 0;
		stat = 0;
		
		if(crc == *(u16*)(rxbuf+*rxlen-2))
			return 1;// ���������յ�����
		
	}else if( CalcCount(sys_ticks, ticks) > 2) { // ��ʱ
	  USART2_RxDeInit();
		
		stat = 0;
	}
	//DelayTick(5);
	return stat;// ��
}

