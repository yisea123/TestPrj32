// #include "sys.h"		    
 #include "rs485.h"	 
// #include "delay.h"
// #include "w25qxx.h"
// #include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//RS485驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
extern u32 FLASH_SIZE;
#if EN_UART4_RX   		//如果使能了接收   	  
//接收缓存区 	
u8 RS485_RX_BUF[16];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u32 RS485_RX_CNT=0; 
u32 FPGA_CODE_SIZE=0;//记录程序的长度
u8 Rec_Over=0;
const u32 FPGA1_CODE_ADD=0x700000;
const u32 FPGA2_CODE_ADD=0x755000;//FPGA第二个程序存储地址从340k开始
//u16 i=0;	
void UART4_IRQHandler(void)
{
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
		   Rec_Over=0;
	    res =USART_ReceiveData(UART4);//;读取接收到的数据UART4->DR
		RS485_RX_BUF[RS485_RX_CNT]=res;
        RS485_RX_CNT=RS485_RX_CNT+1;
        if(RS485_RX_CNT > 15)
		   RS485_RX_CNT = 15;			
	}  				
  OSIntExit();    	
} 
#endif										 
//初始化IO 串口4
//bound:波特率	  
void RS485_Init(u32 bound)
{  	 
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOH,ENABLE); //使能GPIOH  GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能USART4时钟
	
  //串口4引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOC10复用为UART4
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOC11复用为UART4
	
	//UART4
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOC10与GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化
	
	//PH15推挽输出，485模式控制  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOH,&GPIO_InitStructure); //初始化PH15
	

   //UART4初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b  ;//字长为8位数据格式()
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART4, &USART_InitStructure); //初始化串口4
	
  USART_Cmd(UART4, ENABLE);  //使能串口 4
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
#if EN_UART4_RX	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启接受中断

	//Usart6 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif	
	
	RS485_TX_ENABLE;				//默认为发送模式	
	DMA_Config(DMA1_Stream4,DMA_Channel_4,(u32)&UART4->DR,(u32)0,0);
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送  
}
void DMA_usart4Send(u32 mar,u16 ndtr){
//	OS_ERR err;
//	DMA_MemoryTargetConfig(DMA1_Stream4, mar,DMA_Memory_0);	
//	DMA_Enable(DMA1_Stream4,ndtr);    //开始一次DMA传输！	  
//  
//  while(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)==RESET);	
//	OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err); //延时	
//	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
	
	/////////////
	DMA_MemoryTargetConfig(DMA1_Stream4, mar,DMA_Memory_0);	
	
	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
	USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_ClearFlag(UART4, USART_FLAG_RXNE);
	
	DMA_Enable(DMA1_Stream4,ndtr);    //开始一次DMA传输！	  
  
  
  while(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)==RESET);	
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);
//	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
//	USART_ClearFlag(UART4, USART_FLAG_TC);
}

void USART4_Send_Data(u8 *buf,u32 len)
{
//	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);//开启接受中断
	RS485_TX_ENABLE;	
	RS485_RX_CNT=0;		//清零
	
	DMA_usart4Send((u32)buf, len);	
//	USART_ClearFlag(UART4, USART_FLAG_RXNE);
  RS485_TX_DISABLE;				//设置为接收模式		
	
//	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启接受中断
//	USART_ClearFlag(UART4, USART_FLAG_RXNE);
}
//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u16 len)
{
	u16 t;
	RS485_TX_ENABLE;			//设置为发送模式
	RS485_RX_CNT=0;	  
  for(t=0;t<len;t++)		//循环发送数据
	{
	  while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET){}; //等待发送结束		
    USART_SendData(UART4,buf[t]); //发送数据
		//buf[t]=0;
		//if(RS485_RX_CNT>0)
		//	RS485_RX_CNT=RS485_RX_CNT-1;
	}	 
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET); //等待发送结束		
	
	RS485_TX_DISABLE;				//设置为接收模式	
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
		Rec_Over=1;
	}
}

u32 uart4len=0;
u8 UART4_Receive_Data2(u8 *buf,u32 *len)
{

	  u8 i=0;
		*len=0;				//默认为0
		if(RS485_RX_CNT!=0)
		{
			 if(RS485_RX_CNT!=uart4len)
			 {	 
					uart4len=RS485_RX_CNT;
				  return 0;
			 }
       else
       {
					for(i=0;i<uart4len;i++)
					{
						buf[i]=RS485_RX_BUF[i];	
						RS485_RX_BUF[i]=0;
					}		
					*len=uart4len;	//记录本次数据长度
					RS485_RX_CNT=0;		//清零
					uart4len=0;
					Rec_Over=1;
					return 1;
			 }				 
		}	
		else
		{
			return 0;
		}	
}
