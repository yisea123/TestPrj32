	    
 #include "rs485_uart3.h"	 
 //#include "delay.h"
 
// #include "os.h"	 
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
#if EN_USART3_RX   		//如果使能了接收   	  

#endif										 

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void USART3_Send(u8 *buf,u32 len)
{
	USART3_TX_ENABLE;	
	USART3_RX_CNT=0;		//清零
	DMA_usart3Send((u32)buf, len);	
USART3_TX_DISABLE;				//设置为接收模式		
}


//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void USART3_Send_Data111111111(u8 *buf,u32 len)
{
	u16 t;
	USART3_TX_ENABLE;			//设置为发送模式
	//delay_ms(10);
//	OS_CRITICAL_ENTER();
  for(t=0;t<len;t++)		//循环发送数据
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {}; //等待发送结束		
      USART_SendData(USART3,buf[t]); //发送数据
		//buf[t]=0;
		//if(RS485_RX_CNT>0)
		//	RS485_RX_CNT=RS485_RX_CNT-1;
	}	
//   OS_CRITICAL_EXIT();                           //退出临界区	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {}; //等待发送结束		
//	RS485_RX_CNT=0;	  
	//delay_ms(10);
	USART3_TX_DISABLE;				//设置为接收模式	
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
u8 USART3_Receive_Data2(u8 *buf,u32 *len)
{
//	u32 rxlen=USART3_RX_CNT;
//	u8 i=0;
//	*len=0;				//默认为0
//	delay_ms(50);		//等待50ms,连续超过20ms没有接收到一个数据,则认为接收结束
//	if(rxlen==USART3_RX_CNT&&rxlen)//接收到了数据,且接收完成了
//	{
//		for(i=0;i<rxlen;i++)
//		{
//			buf[i]=USART3_RX_BUF[i];	
//			USART3_RX_BUF[i]=0;
//		}		
//		*len=USART3_RX_CNT;	//记录本次数据长度
//		USART3_RX_CNT=0;		//清零
//		USART3_REC_OVER=1;
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}	
	  u8 i=0;
		*len=0;				//默认为0
		if(USART3_RX_CNT!=0)
		{
			 if(USART3_RX_CNT!=rxlen)
			 {	 
					rxlen=USART3_RX_CNT;
				  return 0;
			 }
       else
       {
					for(i=0;i<rxlen;i++)
					{
						buf[i]=USART3_RX_BUF[i];	
						USART3_RX_BUF[i]=0;
					}		
					*len=rxlen;	//记录本次数据长度
					USART3_RX_CNT=0;		//清零
					rxlen=0;
					USART3_REC_OVER=1;
					return 1;
			 }				 
		}	
		else
		{
			return 0;
		}	
}

