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
//485模式控制
#if EN_UART4_485
#define UART4_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_10)	//485模式控制.0,接收;1,发送.
#define UART4_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_10)	//485模式控制.0,接收;1,发送.
#else
#define UART4_TX_ENABLE
#define UART4_TX_DISABLE
#endif

//接收缓存区 	
u8 UART4_RX_BUF[16];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 UART4_RX_CNT=0; 
u8 UART4_REC_OVER=0;
//u32 rxlen=0;
u8 *g_pUart4RxBuf = NULL;
u8 g_Uart4BufLen = 0;
u8 g_Uart4RxLen = 0;
void UART4_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);//开启相关中断
	//NEW08U(g_pUart4RxBuf,len);
	g_pUart4RxBuf = buf;
	g_Uart4BufLen = len;
	g_Uart4RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断
	
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
//接收缓存区 	
u8 RS485_RX_BUF[16];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u32 RS485_RX_CNT=0;
void UART4_IRQHandler(void)
{
#if 4 == MAIN_COM
	OS_ERR err;
	OSIntEnter();                                           /*进入中断*/
	//MAIN_COM = 4;//171024 MMY
  USARTx_IRQHandler(UART4,4);
	OSIntExit();    	    
#else
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//接收到数据
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
//	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//接收到数据
//	{	 	
//		//Rec_Over=0;
//	  res =USART_ReceiveData(UART4);//;读取接收到的数据UART4->DR
//		RS485_RX_BUF[RS485_RX_CNT]=res;
//    RS485_RX_CNT=RS485_RX_CNT+1;
//    if(RS485_RX_CNT > 15)
//		  RS485_RX_CNT = 15;			
//	}
//  OSIntExit();
} 
/**
  *UART4设置
  */
void UART4_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1配置*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //复用为TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //复用为RX
	
	//UART4端口配置TX //RX
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
	
   //UART4 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = wordLength;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = stopBits;//一个停止位
	USART_InitStructure.USART_Parity = parity;//无奇偶校验位
//	
//	USART_InitStructure.USART_BaudRate = bound;//波特率设置
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(UART4, ENABLE);  //使能串口1 
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	//使能串口读
#if EN_UART4_RX	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
  UART4_TX_DISABLE;
  DMA_Config(DMA1_Stream4,DMA_Channel_4,(u32)&UART4->DR,(u32)0,0);
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送  

}
/**
  *UART4发送
  */
void DMA_usart4Send(CDV_INT32U mar,CDV_INT16U ndtr){
#if EN_UART4_485
	OS_ERR  err;
	CPU_SR_ALLOC();
#endif
	UART4_TX_ENABLE;
#if EN_UART4_485
	OSSchedLock(&err);
#endif
	DMA_MemoryTargetConfig(DMA1_Stream4,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream4,ndtr);    //开始一次DMA传输！	  
  
	while(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)==RESET) {};	
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
//	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	UART4_TX_DISABLE;
#if EN_UART4_485
	OSSchedUnlock(&err);
#endif
}

void UART4_Send(u8 *buf,u16 len)
{
	/*u16 t;
  for(t=0;t<len;t++)		//循环发送数据
	{
    while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {}; //等待发送结束
    USART_SendData(UART4,buf[t]); //发送数据
	}
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {}; //等待发送结束*/
	
	//RS485_RX_CNT=0;		//清零
	DMA_usart4Send((u32)buf, len);	
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	
	u32 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	//delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
			RS485_RX_BUF[i]=0;
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
		//Rec_Over=1;
	}
}

/**
  * @brief  命令 发送
  * @param  buf     要发送的字节串
  *         len     要发送的字节串长度，单位B
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
		delay_ms(1);
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
		delay_ms(1);
	}while(time < 3);
	
	//g_Uart4BufLen = 0;//禁止接受到数组
	
	//*buf = g_pUart4RxBuf;
	*len = g_Uart4RxLen;
	
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
void UART4_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSSemPend(&COM_SEM[3],0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	UART4_RxInit(rxbuf ,rxbufLen);
	UART4_Send(txbuf ,txlen);
	UART4_Receive(rxlen);
	UART4_RxDeInit();
	OSSemPost (&COM_SEM[3],OS_OPT_POST_1,&err); 
}