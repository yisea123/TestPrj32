// #include "sys.h"		    
 #include "rs232uart5.h"	 
// #include "delay.h"
// #include "w25qxx.h"
// #include "key.h"
 	 
//extern u32 FLASH_SIZE;
//#if EN_USART4_RX   		//如果使能了接收   	  
//接收缓存区 	
u8 DG_UART5_RX_BUF[32];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u32 DG_UART5_RX_CNT=0; 
u8 DG_UART5_RX_OVER;
u8 DG_Receive_Data[16];
u32 DG_Receive_Len;
//**********************************************
//
//**********************************************
void UART5_IRQHandler(void)
{                                    /*退出中断*/	
	CDV_INT08U Res;//, len = 0;
//	OS_ERR err;
   // OSTmrStart(&tmr2,&err);                                 /*开启定时器 1*/
	OSIntEnter();                                           /*进入中断*/
  tm2Re = 0;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)   /*接收中断()*/
	{
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
		Res =USART_ReceiveData(UART5);                       /*(USART2->DR);	//读取接收到的数据*/
		switch(g_cdvStat){
			default:
				WIFI_RX_BUF_ADD_CHAR(Res);
//					if('\n' == Res)//因ipd指令中会有0d0a
//						if(g_wifiRx.QUEUE.rxLen[g_wifiRx.rxPos]>2) {
//						if( '\r' == g_wifiRx.QUEUE.rxBuf[g_wifiRx.rxPos][g_wifiRx.QUEUE.rxLen[g_wifiRx.rxPos] -2]) {
//							WIFI_RX_QUEUE_SELF_ADD;
//							if(WIFI_RX_HAD) {
//								OSTaskResume((OS_TCB*)&UsartRecvTaskTCB,&err);
//							}
//						}
//					}
				break;
		}
  }
	
	OSIntExit();    	                                      /*退出中断*/

} 
//#endif										 
//初始化IO 串口5
//bound:波特率	  
void DG_UART5_Init(u32 bound)
{  	 
	
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能USART2时钟
	
  //串口2引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOD5复用为UART2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOD6复用为UART2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOD5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
  //RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
//	//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //
	
   //UART2初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART5, &USART_InitStructure); //初始化串口2
	
  USART_Cmd(UART5, ENABLE);  //使能串口 2
	
	USART_ClearFlag(UART5, USART_FLAG_TC);
	
#if EN_UART5_RX	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启接受中断

	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif	
  DMA_Config(DMA1_Stream7,DMA_Channel_4,(CDV_INT32U)&UART5->DR,(CDV_INT32U)0,0);
	USART_DMACmd(UART5,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送  
}

void DMA_usart5Send(CDV_INT32U mar,CDV_INT16U ndtr){
	DMA_MemoryTargetConfig(DMA1_Stream7, mar,DMA_Memory_0);	
	DMA_Enable(DMA1_Stream7,ndtr);    //开始一次DMA传输！	  
  
  while(DMA_GetFlagStatus(DMA1_Stream7,DMA_FLAG_TCIF7)==RESET);	
	
	DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);
}
//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void DG_UART5_Send_Data(u8 *buf,u32 len)
{
	DMA_usart5Send((CDV_INT32U)buf, len);
//	u16 t;
//  for(t=0;t<len;t++)		//循环发送数据
//	{
//	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //等待发送结束		
//    USART_SendData(UART5,buf[t]); //发送数据
//	}	 
//	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //等待发送结束		 
}
//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void DG_UART5_Send_Data11111111(u8 *buf,u32 len)
{
	u16 t;
  for(t=0;t<len;t++)		//循环发送数据
	{
	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //等待发送结束		
    USART_SendData(UART5,buf[t]); //发送数据
	}	 
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //等待发送结束		 
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void DG_UART5_Receive_Data(u8 *buf,u32 *len)
{
	u32 rxlen=DG_UART5_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(20);		//等待20ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==DG_UART5_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=DG_UART5_RX_BUF[i];	
			DG_UART5_RX_BUF[i]=0;
		}		
		*len=DG_UART5_RX_CNT;	//记录本次数据长度
		DG_UART5_RX_CNT=0;		//清零
		DG_UART5_RX_OVER=1;   //接收完成
	}
}

