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
//485模式控制

#if _NPC_VERSION_ == 1u
	#undef EN_USART1_485
#endif

#if EN_USART1_485
	#if _NPC_VERSION_ == 2u
		#ifdef NPC_V2_2
			#define USART1_TX_ENABLE		GPIO_SetBits(GPIOB,GPIO_Pin_14)	//485模式控制.0,接收;1,发送.
			#define USART1_TX_DISABLE		GPIO_ResetBits(GPIOB,GPIO_Pin_14)	//485模式控制.0,接收;1,发送.
		#else
			#define USART1_TX_ENABLE		GPIO_SetBits(GPIOA,GPIO_Pin_11)	//485模式控制.0,接收;1,发送.
			#define USART1_TX_DISABLE		GPIO_ResetBits(GPIOA,GPIO_Pin_11)	//485模式控制.0,接收;1,发送.
		#endif
	#elif _NPC_VERSION_ == 3u
		#define USART1_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_9)	//485模式控制.0,接收;1,发送.
		#define USART1_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_9)	//485模式控制.0,接收;1,发送.
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
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//开启相关中断
	//NEW08U(g_pUsart1RxBuf,len);
	g_pUsart1RxBuf = buf;
	g_Usart1BufLen = len;
	g_Usart1RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	
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
void USART1_IRQHandler(void)                	             /*串口1中断服务程序*/
{
#if 1 == MAIN_COM
	OS_ERR err;
	OSIntEnter();                                           /*进入中断*/
	USARTx_IRQHandler(USART1);
	OSIntExit();    	    
#else
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
	{
	  res =USART_ReceiveData(USART1);//;读取接收到的数据USART3->DR
		
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
//  OSTmrStart(&tmr1,&err);                                 /*开启定时器 1*/
//	OSIntEnter();                                           /*进入中断*/

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)   /*接收中断()*/
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		Res =USART_ReceiveData(USART1);                       /*(USART1->DR);	//读取接收到的数据*/
//		switch(g_cdvStat){
//			case CDV_RECV:
//				g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]++] = Res;//保存到队列
//			  if (QUE_LEN <=  g_scriptRecv.len[g_scriptRecv.rxPos])
//					MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
//				break;
//			default:
//				USART_RX_BUF_ADD_CHAR(Res);
//				break;
//		}
//  } 
//	
//	OSIntExit();    	                                      /*退出中断*/

} 

/**
  *USART1设置
  */
void USART1_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {


#if _NPC_VERSION_ == 1u 
#if defined(CDV_V1)	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1); //GPIOB6复用为USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); //GPIOB7复用为USART1
	
	//USART1端口配置TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOB6与GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA9，PA10
  //RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //GPIOB6与GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA9，PA10
   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = wordLength;//USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = stopBits;//一个停止位
	USART_InitStructure.USART_Parity = parity;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//使能串口读
#if 1	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif

  DMA_Config(DMA2_Stream7,DMA_Channel_4,(CDV_INT32U)&USART1->DR,(CDV_INT32U)0,0);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 
#endif
	
	
#elif _NPC_VERSION_ == 2u
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1配置*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置TX
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
	
   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = wordLength;//USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = stopBits;//一个停止位
	USART_InitStructure.USART_Parity = parity;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//使能串口读
	#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	#endif
  USART1_TX_DISABLE;
  DMA_Config(DMA2_Stream7,DMA_Channel_4,(CDV_INT32U)&USART1->DR,(CDV_INT32U)0,0);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送    
	
#elif _NPC_VERSION_ == 3u

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*usart1配置*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置TX
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
	
   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = wordLength;//USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = stopBits;//一个停止位
	USART_InitStructure.USART_Parity = parity;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//使能串口读
	#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	#endif
  USART1_TX_DISABLE;
  DMA_Config(DMA2_Stream7,DMA_Channel_4,(CDV_INT32U)&USART1->DR,(CDV_INT32U)0,0);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送    
	

#endif
}
/**
  *USART1发送
  */
void DMA_usart1Send(CDV_INT32U mar,CDV_INT16U ndtr){
	
	USART1_TX_ENABLE;
	DMA_MemoryTargetConfig(DMA2_Stream7,mar,DMA_Memory_0);
	
	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	DMA_Enable(DMA2_Stream7,ndtr);    //开始一次DMA传输！	  
  
	while(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)==RESET) {};	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
//	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART1_TX_DISABLE;
}

void USART1_Send(u8 *buf,u16 len)
{
//	u16 t;
//  for(t=0;t<len;t++)		//循环发送数据
//	{
//    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {}; //等待发送结束
//    USART_SendData(USART1,buf[t]); //发送数据
//	}
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {}; //等待发送结束
//		
	DMA_usart1Send((CDV_INT32U)buf, len);
}

/**
  * @brief  命令 发送
  * @param  buf     要发送的字节串
  *         len     要发送的字节串长度，单位B
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
	
	//g_Usart1BufLen = 0;//禁止接受到数组
	
	//*buf = g_pUsart1RxBuf;
	*len = g_Usart1RxLen;
	
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
void USART1_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	USART1_RxInit(rxbuf ,rxbufLen);
	USART1_Send(txbuf ,txlen);
	USART1_Receive(rxlen);
	USART1_RxDeInit();
	OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
}



