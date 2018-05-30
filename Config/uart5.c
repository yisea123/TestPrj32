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

//485模式控制
#if EN_UART5_485
#define UART5_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_12)	//485模式控制.0,接收;1,发送.
#define UART5_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_12)	//485模式控制.0,接收;1,发送.
#else
#define UART5_TX_ENABLE
#define UART5_TX_DISABLE
#endif

#if 5 == CASCADE_USART
#define BUF_LEN 255
#else
#define BUF_LEN 16
#endif
//接收缓存区 	
u8 UART5_RX_BUF[BUF_LEN];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 UART5_RX_CNT=0; 
u8 UART5_REC_OVER=0;
//u32 rxlen=0;
u8 *g_pUart5RxBuf = UART5_RX_BUF;
u8 g_Uart5BufLen = BUF_LEN;
u8 g_Uart5RxLen = 0;

void UART5_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);//开启相关中断
	//NEW08U(g_pUart5RxBuf,len);
	g_pUart5RxBuf = buf;
	g_Uart5BufLen = len;
	g_Uart5RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断
	
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
	OSIntEnter();                                           /*进入中断*/
  USARTx_IRQHandler(UART5, 5);
	
	OSIntExit();    	    
	
//	
//	CDV_INT08U Res;//, len = 0;
////	OS_ERR err;
//   // OSTmrStart(&tmr2,&err);                                 /*开启定时器 1*/
//	OSIntEnter();                                           /*进入中断*/
//  tm2Re = 0;
//	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)   /*接收中断()*/
//	{
//		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
//		Res =USART_ReceiveData(UART5);                       /*(USART2->DR);	//读取接收到的数据*/
//		switch(g_cdvStat){
//			case CDV_RECV:
//				g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]++] = Res;//保存到队列
//				if (QUE_LEN <=  g_scriptRecv.len[g_scriptRecv.rxPos])
//					MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
//				break;
//			default:
//				USART_RX_BUF_ADD_CHAR(Res);
//				break;
//		}
//  }
//	
//	OSIntExit();    	                                      /*退出中断*/
#else
	
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//接收到数据
	{
	  res =USART_ReceiveData(UART5);//;读取接收到的数据USART3->DR
		
		(g_pUart5RxBuf)[g_Uart5RxLen]=res;
		g_Uart5RxLen++;
		
		if(g_Uart5RxLen >= g_Uart5BufLen)
			g_Uart5RxLen = 0;
		
	}
  OSIntExit();
#endif
} 
/**
  *UART5设置
  */
void UART5_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1配置*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //复用为TX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //复用为RX
	
	//UART5端口配置TX //RX
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
	USART_Cmd(UART5, DISABLE);  //使能串口1 
	USART_DMACmd(UART5,USART_DMAReq_Tx,DISABLE);  //使能串口1的DMA发送  
	
	
   //UART5 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = wordLength;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = stopBits;//一个停止位
	USART_InitStructure.USART_Parity = parity;//无奇偶校验位
//	USART_InitStructure.USART_BaudRate = bound;//波特率设置
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART5, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(UART5, ENABLE);  //使能串口1 
	
	USART_ClearFlag(UART5, USART_FLAG_TC);
	//使能串口读
#if EN_UART5_RX	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
  UART5_TX_DISABLE;
  DMA_Config(DMA1_Stream7,DMA_Channel_4,(u32)&UART5->DR,(u32)0,0);
	USART_DMACmd(UART5,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送  

}
/**
  *UART5发送
  */
void DMA_usart5Send(CDV_INT32U mar,CDV_INT16U ndtr){
	OS_ERR  err;
#if EN_UART5_485
	CPU_SR_ALLOC();
#endif
	UART5_TX_ENABLE;
#if EN_UART5_485
	OSSchedLock(&err);
#endif
	DMA_MemoryTargetConfig(DMA1_Stream7,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);
	USART_ClearFlag(UART5, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream7,ndtr);    //开始一次DMA传输！	  
  
	while(DMA_GetFlagStatus(DMA1_Stream7,DMA_FLAG_TCIF7)==RESET) {};	
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);
//	USART_ClearFlag(UART5, USART_FLAG_TC);
	
	UART5_TX_DISABLE;
#if EN_UART5_485
	OSSchedUnlock(&err);
#endif
}
/**
  * @brief  命令 转发
  * @param  buf     要发送的字节串
  *         len     要发送的字节串长度，单位B
  * @retval void
  * @note   
  */
void UART5_Send(u8 *buf,u16 len)
{
//	u16 t;
//  for(t=0;t<len;t++)		//循环发送数据
//	{
//    while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) {}; //等待发送结束
//    USART_SendData(UART5,buf[t]); //发送数据
//	}
//	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) {}; //等待发送结束
		
	DMA_usart5Send((CDV_INT32U)buf, len);
}
/**
  * @brief  命令 发送
  * @param  buf     要发送的字节串
  *         len     要发送的字节串长度，单位B
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
		delay_ms(1);
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
		delay_ms(1);
	}while(time < 3);
	
	//g_Uart5BufLen = 0;//禁止接受到数组
	
	//*buf = g_pUart5RxBuf;
	*len = g_Uart5RxLen;
	
	return 1;
}

/**
  * @brief  命令 发送及接收 的封装函数
  * @param  txbuf  要发送的字符串
  *         txlen  要发送的字符串长度
  *         rxbuf  接收的字符串缓存
  *         rxbufLen 接收字符串缓存的长度
  *         rxlen  接收到的字符串长度
  * @retval void
  * @note   
  */
void UART5_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	UART5_RxInit(rxbuf ,rxbufLen);
	UART5_Send(txbuf ,txlen);
	UART5_Receive(rxlen);
	UART5_RxDeInit();
	OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
}
