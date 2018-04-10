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

//485模式控制
#if EN_USART1_485
#define USART2_TX_ENABLE		GPIO_SetBits(GPIOD,GPIO_Pin_5)	//485模式控制.0,接收;1,发送.
#define USART2_TX_DISABLE		GPIO_ResetBits(GPIOD,GPIO_Pin_5)	//485模式控制.0,接收;1,发送.
#else
#define USART2_TX_ENABLE
#define USART2_TX_DISABLE
#endif

u8 *g_pUart2RxBuf = NULL;
u8 g_Uart2BufLen = 0;
u8 g_Uart2RxLen = 0;

void USART2_RxInit(u8* buf ,u8 len)
{
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//开启相关中断
	//NEW08U(g_pUart2RxBuf,len);
	g_pUart2RxBuf = buf;
	g_Uart2BufLen = len;
	g_Uart2RxLen = 0;
	
	if(NULL !=buf && 0 < len)
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	
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
void USART2_IRQHandler(void)                	             /*串口1中断服务程序*/
{
#if 2 == MAIN_COM
	OS_ERR err;
	OSIntEnter();                                           /*进入中断*/
	USARTx_IRQHandler(USART2,2);
	OSIntExit();    	    
#else
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{
	  res =USART_ReceiveData(USART2);//;读取接收到的数据USART3->DR
		
		(g_pUart2RxBuf)[g_Uart2RxLen]=res;
		g_Uart2RxLen++;
		
		if(g_Uart2RxLen >= g_Uart2BufLen)
			g_Uart2RxLen = 0;
		
	}
  OSIntExit();
#endif
//	CDV_INT08U Res;//, len = 0;
//	OSIntEnter();                                           /*进入中断*/
//  tm1Re = 0;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   /*接收中断()*/
//	{
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//		Res =USART_ReceiveData(USART2);                       /*(USART2->DR);	//读取接收到的数据*/
//			switch(g_cdvStat){
//				case CDV_RECV:
//					g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]++] = Res;//保存到队列
//				  if (QUE_LEN <=  g_scriptRecv.len[g_scriptRecv.rxPos])
//						MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
//					break;
//				default:
//					USART_RX_BUF_ADD_CHAR(Res);
//					break;
//			}
//  } 
//	
//	OSIntExit();    	                                      /*退出中断*/

} 

/*USART设置*/
void USART2_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOD5复用为USART2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOD6复用为USART2
	
	//USART1端口配置TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //GPIOD5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PA9，PA10
  //RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PA9，PA10
		
#if EN_USART2_485
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
#endif

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = wordLength;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = stopBits;//一个停止位
	USART_InitStructure.USART_Parity = parity;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	USART_Cmd(USART2, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	//使能串口读
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
  USART2_TX_DISABLE;
  DMA_Config(DMA1_Stream6,DMA_Channel_4,(CDV_INT32U)&USART2->DR,(CDV_INT32U)0,0);//DMA1,STEAM6,CH4,外设为串口2,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送  

}


void DMA_usart2Send(CDV_INT32U mar,CDV_INT16U ndtr){
#if EN_USART2_485
	CPU_SR_ALLOC();
#endif
	USART2_TX_ENABLE;
#if EN_USART2_485
	OS_CRITICAL_ENTER();
#endif
	DMA_MemoryTargetConfig(DMA1_Stream6, mar,DMA_Memory_0);	
	
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream6,ndtr);    //开始一次DMA传输！	  
  
  
  while(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)==RESET) {};
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {};
//	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
//	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	USART2_TX_DISABLE;
#if EN_USART2_485
	OS_CRITICAL_EXIT();
#endif
}


void USART2_Send(u8 *buf,u16 len)
{
	DMA_usart2Send((CDV_INT32U)buf, len);
}

/**
  * @brief  命令 发送
  * @param  buf     要发送的字节串
  *         len     要发送的字节串长度，单位B
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
		delay_ms(1);
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
		delay_ms(1);
	}while(time < 3);
	
	//g_Uart2BufLen = 0;//禁止接受到数组
	
	//*buf = g_pUart2RxBuf;
	*len = g_Uart2RxLen;
	
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
void USART2_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen)
{
	OS_ERR err;
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	USART2_RxInit(rxbuf ,rxbufLen);
	USART2_Send(txbuf ,txlen);
	USART2_Receive(rxlen);
	USART2_RxDeInit();
	OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
}

