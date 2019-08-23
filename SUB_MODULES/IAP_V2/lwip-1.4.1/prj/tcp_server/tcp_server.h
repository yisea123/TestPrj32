#ifndef __TCP_SERVER_DEMO_H
#define __TCP_SERVER_DEMO_H
#include "sys.h"
#include "includes.h"
#include "cdv_include.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//NETCONN API编程方式的TCP服务器测试代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/8/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   

 
#define TCP_SERVER_RX_BUFSIZE	2000	//定义tcp server最大接收数据长度
#define TCP_SERVER_PORT			8088	//定义tcp server的端口
#define LWIP_SEND_DATA			0x80	//定义有数据发送
 #include "tcp.h"
extern u8_t closeTcp;
extern ip_addr_t g_ota_ipaddr;
//extern u8* tcp_server_recvbuf;	//TCP服务器接收数据缓冲区
//extern u8 tcp_server_flag;			//TCP服务器数据发送标志位
void EthInfoSend(CDV_INT08U uartNo);
u8 tcp_server_init(void);			//TCP服务器初始化(创建TCP服务器线程)
RET_STATUS TCP_ServerSend(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite);
#endif

