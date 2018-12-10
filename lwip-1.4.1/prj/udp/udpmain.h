#ifndef __UDP_SERVER_DEMO_H
#define __UDP_SERVER_DEMO_H
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


//#if USE_NPC_NET == 1u
extern u8_t closeTcp;
u8 udp_init(void);			//TCP服务器初始化(创建TCP服务器线程)
u8 udpecho_init(void);
//RET_STATUS TCP_ServerSend(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite);
void udpecho_find( CDV_INT08U uartNo);
//#endif
#endif

