/**
  ******************************************************************************
  * @file    /uart1.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2019-8-2
  * @brief   a package of uart1 CONFIG
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
	
	#include "uart1.h"
	#include "dmax.h"

//485模式控制

  #define EN_USART1_485 0

#if EN_USART1_485
		#define USART1_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_5)	//485模式控制.0,接收;1,发送.
		#define USART1_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_5)	//485模式控制.0,接收;1,发送.
    #define USART1_TX_DISABLED     (0 == GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5))
#else
		#define USART1_TX_ENABLE
		#define USART1_TX_DISABLE
//    #define USART1_TX_DISABLED     1
#endif

//接收缓存区 	
QUEUE uart1_queue = {0};
/**
  *USART1dma接收使能
  */
void DMA_USART1_RecvEnable(u32 mar,u16 ndtr) {
	//DMA_MemoryTargetConfig(DMA2_Stream1,mar,DMA_Memory_0);
	DMA_Enable(DMA1_Channel5,mar,ndtr);    //开始一次DMA传输！	  
}

// 重置接收DMA
void USART1_RxReset(void) {
	USART_DMACmd(USART1,USART_DMAReq_Rx,DISABLE);

	DMA_USART1_RecvEnable((u32)(QUEUE_ING_BUF(uart1_queue)), QUEUE_BUF_LENGTH);
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
}

/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void) {
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)	{  
			USART1->SR;  
			USART1->DR; //清USART_IT_IDLE标志  
		
			//关闭接收DMA  
			DMA_Cmd(DMA1_Channel5,DISABLE);  
			//清除标志位  
			//DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  
		  DMA_ClearFlag(DMA1_FLAG_TC5);

			//获得接收帧帧长  
		  QUEUE_ING_LEN(uart1_queue) = QUEUE_BUF_LENGTH - DMA_GetCurrDataCounter(DMA1_Channel5);  
		  
		  QUEUE_ING_NEXT(uart1_queue);
			//DMA_usart2RecvEnable((u32)(QUEUE_ING_BUF(uart1_queue)), QUEUE_BUF_LENGTH);
		  USART1_RxReset();
//		
//			//设置传输数据长度  
//			DMA_SetCurrDataCounter(DMA2_Stream1,g_Uart2BufLen);  
//			//打开DMA  
//			DMA_Cmd(DMA2_Stream1,ENABLE);  
	}
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET) { 
		USART_ITConfig(USART1,USART_IT_TC,DISABLE);  
		USART1_TX_DISABLE;//切换到接收
		
	}
} 

void DMA1_Channel4_IRQHandler(void) {
	 //if(DMA_GetITStatus(DMA2_Stream6,DMA_IT_TCIF6) != RESET)  

// dma tc 中断
	 if(DMA_GetITStatus(DMA1_IT_TC4) != RESET) {  
        /* 清除标志位 */
        //DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);  
				DMA_ClearFlag(DMA1_FLAG_TC4);
        /* 关闭DMA */
        DMA_Cmd(DMA1_Channel4,DISABLE);
        /* 打开发送完成中断,确保最后一个字节发送成功 */
        USART_ITConfig(USART1,USART_IT_TC,ENABLE);  
			
    }  
	
}


/**
  *USART1设置
  */
void USART1_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*usart1配置*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);   //GPIOA时钟
	//RCC_APB1PeriphClockCmd(/*|RCC_APB2Periph_AFIO需要用到外设的重映射功能时才需要使能AFIO的时钟，这里没有remap，p180*/,ENABLE);  	//使能USART1时钟
	USART_DeInit(USART1);  //复位串口2

#if EN_USART1_485
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PC5端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
	//第三步：串口引脚   串口2 PA2	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2

	//串口2  PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化PA3

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);//复位串口2
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);//停止复位
   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = wordLength;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = stopBits;//一个停止位
	USART_InitStructure.USART_Parity = parity;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	//USART_OverSampling8Cmd
	USART_Cmd(USART1, ENABLE);  //使能串口 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、


  USART1_TX_DISABLE; // 接收模式
	// 发送DMA配置
	DMA_ConfigDir(DMA1_Channel4,(u32)&USART1->DR,(u32)0,0, DMA_DIR_PeripheralDST);//发送DMA配置

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);  // 打开dam tc中断，用于判断dma传输结束

	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 
	// 接收DMA配置
//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	DMA_ConfigDir(DMA1_Channel5,(u32)&USART1->DR,(u32)0,0,DMA_DIR_PeripheralSRC);//接收DMA
	
	USART1_RxReset();
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
}
/**
  *USART1发送
  */
int DMA_USART1_Send(u32 mar,u16 ndtr) {

	if(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
		return -1; // 上次未发送完成
#if EN_USART1_485
	if(!USART1_TX_DISABLED)
		return - 1;// 未切换到接收，可能刚发送完
#endif
	USART1_TX_ENABLE;
	
	DMA_ClearFlag(DMA1_FLAG_TC4);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Channel4,mar,ndtr);    //开始一次DMA传输！	  
  
//#if EN_USART1_TCIF
//	//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {TaskSched();};
//#else
//	while(DMA_GetFlagStatus(DMA2_Stream6,DMA_FLAG_TCIF6)==RESET) {};	
//		
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) {};
////	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
////	USART_ClearFlag(USART1, USART_FLAG_TC);
//	
//  gstartTime = GetCdvTimeTick();
//	//delay_ms(10);
//	USART1_TX_DISABLE;
//#endif

return 0;
}



int USART1_Send(u8 *buf,u16 len) {
	return DMA_USART1_Send((u32)buf, len);
}

/////////////////////////////////////////////////////
/**
  * @brief  接收
  * @param  
  * @retval int 判断是否接收到，0 成功， -1 失败
  * @note   
  */
int USART1_Receive(void) {
	if (QUEUE_HAD(uart1_queue)) {
//		USART1->SR;  
//		USART1->DR; //清USART_IT_IDLE标志  
//		//关闭DMA  
//		DMA_Cmd(DMA1_Channel5,DISABLE);  
//		//清除标志位  
//		DMA_ClearFlag(DMA1_FLAG_TC5);  
		
		return 0;
	}
	
	return -1;
}

/**
  * @brief  命令 发送并接收 的封装函数
  * @param  txbuf  要发送的字符串
  *         txlen  要发送的字符串长度
  *         rxbuf  接收的字符串缓存指针
  *         rxlen  接收到的字符串长度
  * @retval int 1 接收到命令，0 可以发送，-1 正在接收
  * @note   test 02 02 00 00 00 0E F9 FD
  *         发送等待示例
while（1） {
	if(1 == USART1_TR()) {
		 // do something
	}
}
  */
int USART1_TR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u16* rxlen) {
	static int stat = 0;  // 记录状态
	static u32 ticks = 0; // 计算超时
	static u16 crc;
	
	if(stat == 0 && txbuf && txlen) { // 发送
		QUEUE_CLEAR(uart1_queue);
		USART1_RxReset();
	  USART1_Send(txbuf ,txlen);
		ticks = sys_ticks;
		
	}
	
	if(0 == (stat = USART1_Receive()) && rxlen) { //接收
		rxbuf = QUEUE_DO_BUF(uart1_queue);
		*rxlen = QUEUE_DO_LEN(uart1_queue);
		
		if(*rxlen > 2)
		  crc = MODBUS_CRC16(rxbuf, -2+*rxlen,0xFFFF);
		else
			crc = 0;
		
		if(crc == *(u16*)(rxbuf+*rxlen-2))
			return 1;// 正常，接收到数据
		
	}else if( CalcCount(sys_ticks, ticks) > 2) { // 接收超时
		stat = 0;
	}
	//DelayTick(5);
	return stat;// 无
}


/**
* @brief  命令 接收解析发送 的封装函数
  * @param  
  * @retval 
  * @note   
  *         接收解析发送示例
  */
	

int USART1_RT(int (*p_cmd)(u8 *,u16  ,u8* ,u16* )) {
	static int stat = 0;  // 记录状态
	static u32 ticks = 0; // 计算超时
	static u16 crc;
	
	u8* rxbuf = NULL ;
	u16 rxlen = 0;
	
	u8* rtbuf = NULL ;
	u16 rtlen = 0;
//	if(stat == 0 && txbuf && txlen) { // 发送
//		QUEUE_CLEAR(uart1_queue);
//		USART1_RxReset();
//	  USART1_Send(txbuf ,txlen);
//		ticks = sys_ticks;
//		
//	}
	
	if(0 == (stat = USART1_Receive())) { //接收
		
		rxbuf = QUEUE_DO_BUF(uart1_queue);
		rxlen = QUEUE_DO_LEN(uart1_queue);
		
		if(rxlen > 2)
		  crc = MODBUS_CRC16(rxbuf, -2+rxlen,0xFFFF);
		else
			crc = 0;
		
		if(crc == *(u16*)(rxbuf+rxlen-2)) {
			p_cmd(rxbuf,rxlen,rtbuf,&rtlen);//调用命令处理
			
			if(rtbuf && rtlen) {
				USART1_Send(rtbuf ,rtlen);
			}
			return 1;// 正常，接收到数据
		}
		
	}
//	else if( CalcCount(sys_ticks, ticks) > 2) { // 接收超时
//		stat = 0;
//	}
	//DelayTick(5);
	return stat;// 无
}


