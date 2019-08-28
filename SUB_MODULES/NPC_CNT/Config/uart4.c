/**
  ******************************************************************************
  * @file    /uart4.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2019-8-2
  * @brief   a package of uart4 CONFIG
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2019 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "uart4.h"
	#include "dmax.h"

//485模式控制

  #define EN_UART4_485 0

#if EN_UART4_485
		#define UART4_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_5)	//485模式控制.0,接收;1,发送.
		#define UART4_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_5)	//485模式控制.0,接收;1,发送.
    #define UART4_TX_DISABLED     (0 == GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5))
#else
		#define UART4_TX_ENABLE
		#define UART4_TX_DISABLE
//    #define UART4_TX_DISABLED     1
#endif


//	#define DMA1_Stream4 DMA1_Stream4
//	#define DMA_Channel_4 DMA_Channel_4
//	#define DMA1_Stream2 DMA1_Stream2
//	#define DMA_Channel_4 DMA_Channel_4
	
//接收缓存区 	
QUEUE uart4_queue = {0};
/**
  *UART4dma接收使能
  */
void DMA_UART4_RecvEnable(u32 mar,u16 ndtr) {
	//DMA_MemoryTargetConfig(DMA2_Stream1,mar,DMA_Memory_0);
	DMA_Enable(DMA1_Stream2,mar,ndtr);    //开始一次DMA传输！	  
}

// 重置接收DMA
void UART4_RxReset(void) {
	USART_DMACmd(UART4,USART_DMAReq_Rx,DISABLE);

	DMA_UART4_RecvEnable((u32)(QUEUE_ING_BUF(uart4_queue)), QUEUE_BUF_LENGTH);
	
	USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
}

/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void) {
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)	{  
			UART4->SR;  
			UART4->DR; //清USART_IT_IDLE标志  
		
			//关闭接收DMA  
			DMA_Cmd(DMA1_Stream2,DISABLE);  
			//清除标志位  
			DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF2);  

			//获得接收帧帧长  
		  QUEUE_ING_LEN(uart4_queue) = QUEUE_BUF_LENGTH - DMA_GetCurrDataCounter(DMA1_Stream2);  
		  
		  QUEUE_ING_NEXT(uart4_queue);
			//DMA_usart2RecvEnable((u32)(QUEUE_ING_BUF(uart4_queue)), QUEUE_BUF_LENGTH);
		  UART4_RxReset();
//		
//			//设置传输数据长度  
//			DMA_SetCurrDataCounter(DMA2_Stream1,g_Uart2BufLen);  
//			//打开DMA  
//			DMA_Cmd(DMA2_Stream1,ENABLE);  
	}
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET) { 
		USART_ITConfig(UART4,USART_IT_TC,DISABLE);  
		UART4_TX_DISABLE;//切换到接收
		
	}
} 

void DMA1_Stream4_IRQHandler(void) {
	
	if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4) != RESET)  
	{
		/* 清除标志位 */
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);  
		/* 关闭DMA */
		DMA_Cmd(DMA1_Stream4,DISABLE);
		/* 打开发送完成中断,确保最后一个字节发送成功 */
		USART_ITConfig(UART4,USART_IT_TC,ENABLE);  
			
  }  
	
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
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(UART4, ENABLE);  //使能串口1 
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	//使能串口读
	//USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

  // dma tx
  UART4_TX_DISABLE;
  DMA_ConfigDir(DMA1_Stream4,DMA_Channel_4,(u32)&UART4->DR,(u32)0,0,DMA_DIR_MemoryToPeripheral);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);  // 打开dam tc中断，用于判断dma传输结束

	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 
	
	/* 清除标志位 */
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);  
		/* 关闭DMA */
		DMA_Cmd(DMA1_Stream4,DISABLE);
		
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
	DMA_ConfigDir(DMA1_Stream2,DMA_Channel_4,(u32)&UART4->DR,(u32)0,0,DMA_DIR_PeripheralToMemory);
	//USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
	

	UART4_RxReset();
	
}
/**
  *UART4发送
  */
int DMA_UART4_Send(u32 mar,u16 ndtr) {

	if(/*DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)==RESET 
		|| */USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET
	  || DMA_GetCmdStatus(DMA1_Stream4) != DISABLE)
		return -1; // 上次未发送完成
#if EN_UART4_485
	if(!UART4_TX_DISABLED)
		return - 1;// 未切换到接收，可能刚发送完
#endif
	UART4_TX_ENABLE;
	
	DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Stream4, mar, ndtr);    //开始一次DMA传输！	  
  
//#if EN_UART4_TCIF
//	//while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {TaskSched();};
//#else
//	while(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF4)==RESET) {};	
//		
//	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) {};
////	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF4);
////	USART_ClearFlag(UART4, USART_FLAG_TC);
//	
//  gstartTime = GetCdvTimeTick();
//	////delay_ms(10);
//	UART4_TX_DISABLE;
//#endif

return 0;
}



int UART4_Send(u8 *buf,u16 len) {
	return DMA_UART4_Send((u32)buf, len);
}

/////////////////////////////////////////////////////
/**
  * @brief  接收
  * @param  
  * @retval int 判断是否接收到，0 成功， -1 失败
  * @note   
  */
int UART4_Receive(void) {
	if (QUEUE_HAD(uart4_queue)) {
//		UART4->SR;  
//		UART4->DR; //清USART_IT_IDLE标志  
//		//关闭DMA  
//		DMA_Cmd(DMA1_Channel5,DISABLE);  
//		//清除标志位  
//		DMA_ClearFlag(DMA1_FLAG_TC5);  
		
		return 0;
	}
	
	return -1;
}

///**
//  * @brief  命令 发送并接收 的封装函数
//  * @param  txbuf  要发送的字符串
//  *         txlen  要发送的字符串长度
//  *         rxbuf  接收的字符串缓存指针
//  *         rxlen  接收到的字符串长度
//  * @retval int 1 接收到命令，0 可以发送，-1 正在接收
//  * @note   test 02 02 00 00 00 0E F9 FD
//  *         发送等待示例
//while（1） {
//	if(1 == UART4_TR()) {
//		 // do something
//	}
//}
//  */
//int UART4_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen) {
//	static int stat = 0;  // 记录状态
//	static u32 ticks = 0; // 计算超时
//	static u16 crc;
//	
//	if(stat == 0 && txbuf && txlen) { // 发送
//		QUEUE_CLEAR(uart4_queue);
//		UART4_RxReset();
//	  UART4_Send(txbuf ,txlen);
//		ticks = sys_ticks;
//		
//	}
//	
//	if(0 == (stat = UART4_Receive()) && rxlen) { //接收
//		rxbuf = QUEUE_DO_BUF(uart4_queue);
//		*rxlen = QUEUE_DO_LEN(uart4_queue);
//		
//		if(*rxlen > 2)
//		  crc = MODBUS_CRC16(rxbuf, -2+*rxlen,0xFFFF);
//		else
//			crc = 0;
//		
//		if(crc == *(u16*)(rxbuf+*rxlen-2))
//			return 1;// 正常，接收到数据
//		
//	}else if( CalcCount(sys_ticks, ticks) > 2) { // 接收超时
//		stat = 0;
//	}
//	//DelayTick(5);
//	return stat;// 无
//}


/**
  * @brief  命令 接收解析发送 的封装函数
  * @param  
  * @retval int 1 正常处理完成 0 接收到数据 -1 未接收到数据  -2 发送异常
  * @note   
  *         接收解析发送示例
while (1) {
  while ( 1 != UART4_RT) {
    // do something
  }
}
  */
	

int UART4_RT(int (*p_cmd)(u8 *,u16  ,u8** ,u16* )) {
	static int stat = 0;  // 记录状态
	//static u32 ticks = 0; // 计算超时
	static u16 crc;
	
	u8* rxbuf = NULL ;
	u16 rxlen = 0;
	
	static u8* rtbuf = NULL ;
	static u16 rtlen = 0;

	
	if(rtbuf && rtlen) {
		if (0 == UART4_Send(rtbuf ,rtlen)) {//发送完成
		  rtbuf = NULL ;
	    rtlen = 0;
	    return 1;// 正常，接收到数据
		} else {
			return -2; // 发送异常
		}
	}
	
	if(0 == (stat = UART4_Receive())) { //接收
		
		rxbuf = QUEUE_DO_BUF(uart4_queue);
		rxlen = QUEUE_DO_LEN(uart4_queue);
		
		if(rxlen > 2)
		  crc = MODBUS_CRC16(rxbuf, -2+rxlen,0xFFFF);
		else
			crc = 0;
		
		//if(crc == *(u16*)(rxbuf+rxlen-2)) {
			p_cmd(rxbuf,rxlen,&rtbuf,&rtlen);//调用命令处理，rtbuf用户自行处理
			
//			if(rtbuf && rtlen) {
//				UART4_Send(rtbuf ,rtlen);
//			}
//			return 1;// 正常，接收到数据
		//}
		QUEUE_DO_NEXT(uart4_queue);
	}
//	else if( CalcCount(sys_ticks, ticks) > 2) { // 接收超时
//		stat = 0;
//	}
	//DelayTick(5);
	return stat;// 无
}


