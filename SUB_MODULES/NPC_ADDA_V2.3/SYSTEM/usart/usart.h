#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "user.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//用户驱动代码	   
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
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  		8  	//定义最大接收字节数 8
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART1_RX      1	  
#define RS485_TX_EN		PCout(5)	//485模式控制.0,接收;1,发送.
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART2_RX_BUF[100]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART_RX_STA;         		//接收状态标记	
extern u8  USART_RX_NUM;//接收计数�
extern u8  USART_RX_DATA1;
extern u8	bufbuf[8];
extern u8	buf[8];
extern u16 USART2_RX_CNT;

//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void Uart2_Receive_Data(u8 *buf, u8 *len);//获取串口数据
void uart_receive(void);//获取串口数据
void uart_send(void);//串口发送
u16 getCRC16(u8 *ptr,u16 len);
void USART2_Send(u8 *buf,u16 len);
//void AddTx(CDV_INT08U* txBuf, CDV_INT08U txLen, CDV_INT08U uartNo);
#endif


