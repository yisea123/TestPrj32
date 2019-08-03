#include "usart.h"	
//#include "user.h"
//#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//海盛电子
//修改日期:2014/1/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 嘉兴海盛电子有限公司
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 
  USART_SendData(USART2,(uint8_t)ch);   
	return ch;
}
#endif 

 
#if EN_USART2_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_RX_BUF[100];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART2_RX_CNT =0;
u8 USART2_REC_OVER;
u8 USART2_DMA_TX_Buf[DMA_Buf_lens]={0}; //ADC3 DMA采样缓存
u8 Flag_USART2_DMA_TX_Finished = 1;
u8 Flag_USART2_Send  = 0;
u16 time1;
u16 time2;

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u8 USART_RX_STA=0;       //接收状态标记	  
u8  USART_RX_NUM=0;//接收计数值
u16 times=0;
u8 len;	
u8 bufbuf[8]={0xA5,'C','4',4,0,0,0,'C'+'4'+4};
u8 buf[8]={0xA5,'C','4',4,0,0,0,'C'+'4'+4};	//默认切换2状态
u8 USART_RX_DATA1;
//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  

//DMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
//void DMA_Config1(DMA_Stream_TypeDef *DMA_Streamx,CDV_INT32U chx,CDV_INT32U par,CDV_INT32U mar,CDV_INT16U ndtr)
//{ 
// 
//	DMA_InitTypeDef  DMA_InitStructure;
//	
//	USART_DMACmd(USART2 , USART_DMAReq_Tx, ENABLE);
//  /* Enable USART2 DMA TX request */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

//  DMA_DeInit(DMA1_Channel7);
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&USART2->DR);//DMA外设地址
//  DMA_InitStructure.DMA_MemoryBaseAddr =  (uint32_t)USART2_DMA_TX_Buf;//DMA 存储器0地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//  DMA_InitStructure.DMA_BufferSize = 0;
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//  DMA_Init(DMA1_Channel7, &DMA_InitStructure);
//  DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
//} 
 void DMA_ConfigDir(DMA_Channel_TypeDef *DMA_Channelx,u32 par,u32 mar,u16 ndtr,u32 dir)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Channelx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Channelx);
	
	//while (DMA_GetCmdStatus(DMA_Channelx) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  //DMA_InitStructure.DMA_Channel = chx;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
  //DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
  DMA_InitStructure.DMA_MemoryBaseAddr = mar;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = dir;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//中等优先级
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Channelx, &DMA_InitStructure);//初始化DMA Stream
	

} 
//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void DMA_Enable(DMA_Channel_TypeDef *DMA_Channelx,u32 mar,u16 ndtr)
{
 
	DMA_Cmd(DMA_Channelx, DISABLE);                      //关闭DMA传输 
	DMA_Channelx->CNDTR = ndtr;                          // 更新内存长度
  DMA_Channelx->CMAR = mar;                            // 更新内存地址
	DMA_SetCurrDataCounter(DMA_Channelx,ndtr);          //数据传输量  
	DMA_Cmd(DMA_Channelx, ENABLE); 
}

/**
  *USART2发送
  */
int DMA_usart2Send(u32 mar,u16 ndtr){
	DMA_ClearFlag(DMA1_FLAG_TC7);
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Channel7,mar,ndtr);    //开始一次DMA传输！	  
  
	while(DMA_GetFlagStatus(DMA1_FLAG_TC7)==RESET) {};
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET){};
} 
//初始化IO 串口2  bound:波特率
void uart2_init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	//第二步：打开串口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);   //GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_AFIO,ENABLE);  	//使能USART2时钟
	USART_DeInit(USART2);  //复位串口2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PC5端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
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
   //第一步：USART 初始化设置
	
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	DMA_ConfigDir(DMA1_Channel7,(u32)&USART2->DR,(u32)0,0, DMA_DIR_PeripheralDST);//发送DMA配置

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
  /* Enable USART2 DMA TX request */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

//  DMA_DeInit(DMA1_Channel7);
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
//  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_DMA_TX_Buf;
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//  DMA_InitStructure.DMA_BufferSize = 0;
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//  DMA_Init(DMA1_Channel7, &DMA_InitStructure);
//  DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable USART2 DMA TX Finish Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
#if EN_USART2_RX		  //如果使能了接收  
   	//第五步：中断优先级设置
    //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   	//第四步：串口中断使能
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
#endif
  USART_Cmd(USART2, ENABLE);                    //使能串口 
  RS485_TX_EN=0;			//默认为接收模式
	//	DMA_Config1(DMA1_Stream6,DMA_Channel_4,(u32)&USART2->DR,(u32)Modbus_TX_buf,Modbus_TX_len);
  //	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //使能串口2的DMA发送
}

//初始化IO 串口1 
//bound:波特率
void uart1_init(u32 bound){
    
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟
 //	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1)
	USART_DeInit(USART1);  //复位串口1
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

  //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ClearFlag(USART1, USART_FLAG_TC);
#if EN_USART1_RX		  //如果使能了接收  
   //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	
#endif
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	
//第六步：串口中断服务程序
void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		if (USART2_RX_CNT<100){
		USART2_RX_BUF[USART2_RX_CNT] = Res;
		time1 = u16TimeT;
		StartTime = u32TimeT100us;
    USART2_RX_CNT++;
		}
		
		Modbus_timeout_cnt=0;			
		Modbus_recevie_flag=1;
		
//		USART2_Send(USART2_RX_BUF,1);
		
		if (USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
    /* Disable the USART2 Transmit Complete interrupt */
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);
    Flag_USART2_DMA_TX_Finished = 1;
    }     
  /* If overrun condition occurs, clear the ORE flag a.nd recover communication */    
  if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) != RESET) {
    (void)USART_ReceiveData(USART2);
  }
		
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

		
  } 
  
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
}


/******************获取串口数据*******************/
void Uart2_Receive_Data(u8 *buf, u8 *len)
{
	u8 t;
	u32 rxlen=USART2_RX_CNT;
	u8* RxBuf = NULL;
	u16 data;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待20ms,连续超过20ms没有接收到一个数据,则认为接收结束
	if(rxlen==USART2_RX_CNT&&rxlen) //接收到了数据,且接收完成了
	{
		data=getCRC16(USART2_RX_BUF,USART2_RX_CNT-2);
		if((USART2_RX_BUF[USART2_RX_CNT-2]==(data & 0x00ff)) && (USART2_RX_BUF[USART2_RX_CNT-1]==((data >> 8) & 0x00ff))) {
		for(i=0;i<rxlen;i++)
			{
				buf[i]=USART2_RX_BUF[i];	
				USART2_RX_BUF[i]=0;
			}		
			
		 }
		else{
				memset(USART2_RX_BUF,0,USART2_RX_CNT*sizeof(CDV_INT08U));
				USART2_RX_CNT=0;
		}
		*len=USART2_RX_CNT;	//记录本次数据长度
		USART2_RX_CNT=0;		//清零
		USART2_REC_OVER=1;
	 }
	
//		for(i=0;i<1;i++)
//		{ 
//			if(USART2_RX_BUF[0]== 0x00&&USART2_RX_BUF[1]== 0x00&&USART2_RX_BUF[2]== 0x00&&USART2_RX_BUF[3]== 0x00&&USART2_RX_BUF[4]== 0x00&&USART2_RX_BUF[5]==(USART2_RX_CNT-6))
//			{
//				data=getCRC16(&USART2_RX_BUF[6],USART2_RX_CNT-8);
//	      if((USART2_RX_BUF[USART2_RX_CNT-2]==(data & 0x00ff)) && (USART2_RX_BUF[USART2_RX_CNT-1]==((data >> 8) & 0x00ff)))
//		
//				OnlineCmdCache(&USART4_RX_BUF[6], USART4_RX_CNT-6,4);
//			}
//		}	
		
	
//	if(2==USART_RX_STA) //判断结束标记
//	{	
//		len= USART_RX_BUF[0]+USART_RX_BUF[1]+USART_RX_BUF[2]+USART_RX_BUF[3]+USART_RX_BUF[4];
//		if(len==USART_RX_BUF[6]) //累加校验
//		{
//			if(0x01==USART_RX_BUF[5]) //ID为1 切换2 系统初始化
//			{
//				if('4'==USART_RX_BUF[1])
//				{
//					for(t=1;t<8;t++)
//					{
//						bufbuf[t]=USART_RX_BUF[t-1];
//						USART_RX_DATA1 = USART_RX_BUF[1];//获取控制命令参数，分别有‘1’~‘8’
//						USART_RX_STA=0;
//					}
//				}
//			}
//			
//			if(USART_RX_BUF[5]==bufbuf[6])//若ID相同，则不做处理
//			{
//				USART_RX_STA=0;//接收标记清除
//			}
//			else if(USART_RX_BUF[5] == (bufbuf[6]+1) || (USART_RX_BUF[5]==0 && 0xff==bufbuf[6]))//若ID编号正确，则保存
//			{
//				for(t=1;t<8;t++)
//				{
//					bufbuf[t]=USART_RX_BUF[t-1];
//					USART_RX_DATA1 = USART_RX_BUF[1];//获取控制命令参数，分别有‘1’~‘8’
//				}
//			}
//			else
//			{
//				USART_RX_STA=0;//接收标记清除
//			}				
//			
//		}
//		USART_RX_STA=0;
//	}
}
/******串口发送**********/
void uart_send(void)
{
	u8 t;
	for(t=0;t<8;t++)
	{
		USART_SendData(USART2, bufbuf[t]);//向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束  
	}


}

/**
  * @brief  计算字符串的CRC值
  *  
  * @param  ptr     字符串的指针
  *         len     字符串的长度      
  *   
  * @retval CRC值
	*
	* @note   
  */

u16 getCRC16(u8 *ptr,u16 len) 
{ 
    CDV_INT08U i; 
    CDV_INT16U crc = 0xFFFF; 
    if(len==0){
        len = 1;
    } 
    while(len--){   
        crc ^= *ptr; 
        for(i=0; i<8; i++){ 
            if(crc&1){ 
                crc >>= 1;  
                crc ^= 0xA001; 
        	}  
        	else{
                crc >>= 1;
        	} 
        }         
        ptr++; 
    } 
    return(crc); 
} 

/**
  * Function Name  : USART2_Data_Send
  * Description    : None
  * Input          : None
  * Output         : None
  * Return         : None
  */
void USART2_Data_Send(u8 *buf ,u16 len) {
  if(Flag_USART2_Send){
    if(Flag_USART2_DMA_TX_Finished == 1){
      Flag_USART2_DMA_TX_Finished = 0;
      DMA1_Channel7->CMAR  = (uint32_t)&buf[0];
      DMA1_Channel7->CNDTR = len;  // len
      DMA_Cmd(DMA1_Channel7, ENABLE);
      Flag_USART2_Send = 0;
    }
  }
}

void USART2_Send(u8 *buf,u16 len)
{
	
	u16 t ;
	u16 t2 =0,t3,t4,t1 = u16TimeT;
//	{
//		static int cnt = 0;
//		if(cnt>=100)
//		{
////			memset(TestTimer,0,sizeof(u16)*100);	
////			memset(TestTimer,0,sizeof(u16)*100);	
//			cnt = 0;			
//		}
//		if(u32TimeT100us>StartTime)	
//		{			
//			TestTimer[cnt]=u32TimeT100us-StartTime;
//			TestTimer2[cnt]=u32TimeT100us-StartTime2;
//		}
//		else
//		{
//			TestTimer[cnt]=1000000000+u32TimeT100us-StartTime;
//			TestTimer2[cnt]=1000000000+u32TimeT100us-StartTime2;
//		}
//		cnt++;
//	}
	RS485_TX_EN=1;
	DMA_usart2Send((u32)buf,len);
//  for(t=0;t<len;t++)		//循环发送数据
//	{ 
//    while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {}; //等待发送结束
//    USART_SendData(USART2,buf[t]); //发送数据
//	}
//	t2 = u16TimeT;
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {}; //等待发送结束
//	delay_us(1);
//	t3 = t2-t1;
//	
//	time2 = u16TimeT;
//	t4 =	time2 - time1;
	
	
	RS485_TX_EN=0;	
//	DMA_usart1Send((CDV_INT32U)buf, len);
}

/**
  * Function Name  : DMA1_Channel7_IRQHandler
  * Description    : This function handles DMA1 Channel 7 interrupt request.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void DMA1_Channel7_IRQHandler(void) 
{
  if(DMA_GetITStatus(DMA1_IT_TC7)) {
    /* USART2 DMA 传输完成 */
    DMA_ClearITPendingBit(DMA1_IT_TC7);
    DMA_Cmd(DMA1_Channel7, DISABLE);
    /* Enable USART2 Transmit complete interrupt */
    USART_ITConfig(USART2, USART_IT_TC, ENABLE); 
  }
}
