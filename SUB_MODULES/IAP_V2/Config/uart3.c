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

//485模式控制
#if EN_USART3_485
#if _NPC_VERSION_ == 1u
#define USART3_TX_ENABLE		GPIO_SetBits(GPIOA,GPIO_Pin_3)	//485模式控制.0,接收;1,发送.
#define USART3_TX_DISABLE		GPIO_ResetBits(GPIOA,GPIO_Pin_3)	//485模式控制.0,接收;1,发送.

#else

#define USART3_TX_ENABLE		GPIO_SetBits(GPIOF,GPIO_Pin_0)	//485模式控制.0,接收;1,发送.
#define USART3_TX_DISABLE		GPIO_ResetBits(GPIOF,GPIO_Pin_0)	//485模式控制.0,接收;1,发送.

#endif
#endif
//接收缓存区 	
u8 USART3_RX_BUF[16];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 USART3_RX_CNT=0; 
u8 USART3_REC_OVER=0;

u8 *g_pUsart3RxBuf = NULL;
u8 g_Usart3BufLen = 0;
u8 g_Usart3RxLen = 0;

void USART3_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//开启相关中断
	//NEW08U(g_pUsart3RxBuf,len);
	g_pUsart3RxBuf = buf;
	g_Usart3BufLen = len;
	g_Usart3RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断
	
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
	OSIntEnter();                                           /*进入中断*/
  USARTx_IRQHandler(USART3);
	OSIntExit();    	    
#else
	
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{
	  res =USART_ReceiveData(USART3);//;读取接收到的数据USART3->DR
		
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
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
//	{
//	  res =USART_ReceiveData(USART3);//;读取接收到的数据USART3->DR
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
  *USART3设置
  */
void USART3_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {
 #if _NPC_VERSION_ == 1u 
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1配置*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	
  //串口3引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	
	//USART3    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA2，PA3
	
#if EN_USART3_485
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PG8
#endif
	
   //USART3 初始化设置
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
	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	//使能串口读
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
  USART3_TX_DISABLE;
  DMA_Config(DMA1_Stream3,DMA_Channel_4,(u32)&USART3->DR,(u32)0,0);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 
#else

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1配置*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //复用为TX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //复用为RX
	
	//USART3端口配置TX //RX
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
	
   //USART3 初始化设置
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
	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	//使能串口读
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
  USART3_TX_DISABLE;
  DMA_Config(DMA1_Stream3,DMA_Channel_4,(u32)&USART3->DR,(u32)0,0);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送  
#endif
}
/**
  *USART3发送
  */
void DMA_usart3Send(CDV_INT32U mar,CDV_INT16U ndtr){
	
	USART3_TX_ENABLE;
	DMA_MemoryTargetConfig(DMA1_Stream3,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream3,ndtr);    //开始一次DMA传输！	  
  
	while(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)==RESET) {};	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
//	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART3_TX_DISABLE;
}

void USART3_Send(u8 *buf,u16 len)
{
//	u16 t;
//  for(t=0;t<len;t++)		//循环发送数据
//	{
//    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {}; //等待发送结束
//    USART_SendData(USART3,buf[t]); //发送数据
//	}
//	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {}; //等待发送结束
	DMA_usart3Send((CDV_INT32U)buf, len);
}

//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void USART3_Receive_Data(u8 *buf,u32 *len)
{
	u32 rxlen=USART3_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(20);		//等待20ms,连续超过20ms没有接收到一个数据,则认为接收结束
	if(rxlen==USART3_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART3_RX_BUF[i];	
			USART3_RX_BUF[i]=0;
		}		
		*len=USART3_RX_CNT;	//记录本次数据长度
		USART3_RX_CNT=0;		//清零
		USART3_REC_OVER=1;
	}
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
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
//	g_Usart3BufLen = 0;//禁止接受到数组
//	
//	*buf = g_pUsart3RxBuf;
//	*len = g_Usart3RxLen;
//	
//	return 1;
//}

/**
  * @brief  命令 发送
  * @param  buf     要发送的字节串
  *         len     要发送的字节串长度，单位B
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
	}while(time < 3);
	
	//g_Usart3BufLen = 0;//禁止接受到数组
	
	//*buf = g_pUsart3RxBuf;
	*len = g_Usart3RxLen;
	
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
void USART3_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	USART3_RxInit(rxbuf ,rxbufLen);
	USART3_Send(txbuf ,txlen);
	USART3_Receive(rxlen);
	USART3_RxDeInit();
	OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
}