/**
  ******************************************************************************
  * @file    /uart2.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2019-8-2
  * @brief   a package of uart2 CONFIG
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
	
	#include "uart2.h"
	#include "dmax.h"

//485模式控制

  #define EN_USART2_485 1

#if EN_USART2_485
		#define USART2_TX_ENABLE		GPIO_SetBits(GPIOC,GPIO_Pin_5)	//485模式控制.0,接收;1,发送.
		#define USART2_TX_DISABLE		GPIO_ResetBits(GPIOC,GPIO_Pin_5)	//485模式控制.0,接收;1,发送.
    #define USART2_TX_DISABLED     (0 == GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5))
#else
		#define USART2_TX_ENABLE
		#define USART2_TX_DISABLE
    #define USART2_TX_DISABLED     1
#endif

//接收缓存区 	
QUEUE uart2_queue = {0};
/**
  *USART2dma接收使能
  */
void DMA_USART2_RecvEnable(u32 mar,u16 ndtr){
	//DMA_MemoryTargetConfig(DMA2_Stream1,mar,DMA_Memory_0);
	DMA_Enable(DMA1_Channel6,mar,ndtr);    //开始一次DMA传输！	  
}

// 重置接收DMA
void USART2_RxReset(void)
{
	USART_DMACmd(USART2,USART_DMAReq_Rx,DISABLE);

	DMA_USART2_RecvEnable((u32)(QUEUE_ING_BUF(uart2_queue)), QUEUE_BUF_LENGTH);
	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
}

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
			USART2->DR; //清USART_IT_IDLE标志  
		
			//关闭接收DMA  
			DMA_Cmd(DMA1_Channel6,DISABLE);  
			//清除标志位  
			//DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);  
		  DMA_ClearFlag(DMA1_FLAG_TC6);

			//获得接收帧帧长  
		  QUEUE_ING_LEN(uart2_queue) = QUEUE_BUF_LENGTH - DMA_GetCurrDataCounter(DMA1_Channel6);  
		  
		  QUEUE_ING_NEXT(uart2_queue);
			//DMA_usart2RecvEnable((u32)(QUEUE_ING_BUF(uart2_queue)), QUEUE_BUF_LENGTH);
		  //USART2_RxReset();
//		
//			//设置传输数据长度  
//			DMA_SetCurrDataCounter(DMA2_Stream1,g_Uart2BufLen);  
//			//打开DMA  
//			DMA_Cmd(DMA2_Stream1,ENABLE);  
	}
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)  
	{ 
		USART_ITConfig(USART2,USART_IT_TC,DISABLE);  
		USART2_TX_DISABLE;//切换到接收
		
	}
} 

void DMA1_Channel7_IRQHandler(void) {
	 //if(DMA_GetITStatus(DMA2_Stream6,DMA_IT_TCIF6) != RESET)  

// dma tc 中断
	 if(DMA_GetITStatus(DMA1_IT_TC7) != RESET)  
    {  
        /* 清除标志位 */
        //DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);  
				DMA_ClearFlag(DMA1_FLAG_TC7);
        /* 关闭DMA */
        DMA_Cmd(DMA1_Channel7,DISABLE);
        /* 打开发送完成中断,确保最后一个字节发送成功 */
        USART_ITConfig(USART2,USART_IT_TC,ENABLE);  
			
    }  
	
}


/**
  *USART2设置
  */
void USART2_Configuration(u32 bound, u16 wordLength, u16 stopBits, u16 parity) {

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*usart1配置*/
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOB,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);   //GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2/*|RCC_APB2Periph_AFIO需要用到外设的重映射功能时才需要使能AFIO的时钟，这里没有remap，p180*/,ENABLE);  	//使能USART2时钟
	USART_DeInit(USART2);  //复位串口2
//	//串口1对应引脚复用映射
//	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART2); //复用为TX
//	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART2); //复用为RX
//	//USART2端口配置TX //RX
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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PC5端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
	//第三步：串口引脚   串口2 PA2	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2

	//串口2  PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化PA3

  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位
   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = wordLength;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = stopBits;//一个停止位
	USART_InitStructure.USART_Parity = parity;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	//USART_OverSampling8Cmd
	USART_Cmd(USART2, ENABLE);  //使能串口 
	
	USART_ClearFlag(USART2, USART_FLAG_TC);

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、


  USART2_TX_DISABLE; // 接收模式
	// 发送DMA配置
	DMA_ConfigDir(DMA1_Channel7,(u32)&USART2->DR,(u32)0,0, DMA_DIR_PeripheralDST);//发送DMA配置

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);  // 打开dam tc中断，用于判断dma传输结束

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送 
	// 接收DMA配置
//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	DMA_ConfigDir(DMA1_Channel6,(u32)&USART2->DR,(u32)0,0,DMA_DIR_PeripheralSRC);//接收DMA
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
}
/**
  *USART2发送
  */
int DMA_USART2_Send(u32 mar,u16 ndtr){

	if(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
		return -1; // 上次未发送完成
	
	if(!USART2_TX_DISABLED)
		return - 1;// 未切换到接收，可能刚发送完
	
	USART2_TX_ENABLE;
	
	DMA_ClearFlag(DMA1_FLAG_TC7);
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Channel7,mar,ndtr);    //开始一次DMA传输！	  
  
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
//	////delay_ms(10);
//	USART2_TX_DISABLE;
//#endif

return 0;
}



int USART2_Send(u8 *buf,u16 len)
{
	return DMA_USART2_Send((u32)buf, len);
}

/////////////////////////////////////////////////////
/**
  * @brief  接收
  * @param  
  * @retval int 判断是否接收到，0 成功， -1 失败
  * @note   
  */
int USART2_Receive(void)
{
	if (QUEUE_HAD(uart2_queue)) {
//		USART2->SR;  
//		USART2->DR; //清USART_IT_IDLE标志  
//		//关闭DMA  
//		DMA_Cmd(DMA1_Channel6,DISABLE);  
//		//清除标志位  
//		DMA_ClearFlag(DMA1_FLAG_TC6);  
		
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
  * @retval int    2 crc未通过 ，1 接收到命令，0 超时、可以发送，-1 正在接收，-2 等待其他buf
  * @note   test 02 02 00 00 00 0E F9 FD
  *         发送等待示例
while（1） {
	if(1 == USART2_TR()) {
		 // do something
	}
}
  */
int USART2_TR(u8 *txbuf,u16 txlen ,u8** rxbuf ,u16* rxlen)
{
	static int stat = 0;  // 记录状态
	static u8* dobuf = NULL; //记录正在处理的buf
	static u32 ticks = 0; // 计算超时
	static u16 crc;
	
	
	if(stat && dobuf != txbuf)
		return -2; // 等待其他buf
	
	if(stat == 0 && txbuf && txlen) { // 发送
		QUEUE_CLEAR(uart2_queue);
		USART2_RxReset();
	  USART2_Send(txbuf ,txlen);
		ticks = sys_ticks;
		dobuf = txbuf;
	}
	
	if(0 == (stat = USART2_Receive()) && rxlen) { //接收
		*rxbuf = QUEUE_DO_BUF(uart2_queue);
		*rxlen = QUEUE_DO_LEN(uart2_queue);
		dobuf = NULL;
		
//		if(*rxlen > 2)
//		  crc = MODBUS_CRC16(*rxbuf, -2+*rxlen,0xFFFF);
//		else
//			crc = 0;
//		
//		if(crc == *(u16*)(*rxbuf+*rxlen-2))
//			return 1;// 正常，接收到数据
//		else
//			return 2;// crc 未通过
		return 1;// 正常，接收到数据
		
	}else if( CalcCount(sys_ticks, ticks) > 50) { // 接收超时,190806：DA命令 2ms会出错，子模块处理速度还需优化
		dobuf = NULL;
		stat = 0;
	}
	//DelayTick(5);
	return stat;// 无
}

