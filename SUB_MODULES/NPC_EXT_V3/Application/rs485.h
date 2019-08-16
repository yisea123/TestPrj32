#ifndef __RS485_H
#define __RS485_H			 	 
#include "cdv_include.h"	 								  
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
	  	
extern u8 RS485_RX_BUF[16]; 		//接收缓冲,最大64个字节
extern u32 RS485_RX_CNT;   			//接收到的数据长度
extern u8 DG_UART5_RX_OVER;     //接收完成标记
extern u8 Rec_Over;
//模式控制
#define RS485_TX_ENABLE		GPIO_SetBits(GPIOH,GPIO_Pin_15)	//485模式控制.0,接收;1,发送.
#define RS485_TX_DISABLE		GPIO_ResetBits(GPIOH,GPIO_Pin_15)	//485模式控制.0,接收;1,发送.

//如果想串口中断接收，设置EN_UART4_RX为1，否则设置为0
#define EN_UART4_RX 	1			//0,不接收;1,接收.

														 
void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u16 len);
void USART4_Send_Data(u8 *buf,u32 len);
void RS485_Receive_Data(u8 *buf,u8 *len); 
u8 UART4_Receive_Data2(u8 *buf,u32 *len);
void RS485_Receive_OVER(u8 *buf,u32 *len);
#endif	   

