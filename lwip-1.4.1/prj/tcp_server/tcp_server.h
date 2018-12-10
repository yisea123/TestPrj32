#ifndef __TCP_SERVER_DEMO_H
#define __TCP_SERVER_DEMO_H




#include "sys.h"
#include "includes.h"
#include "cdv_include.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//NETCONN API��̷�ʽ��TCP���������Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   

//#if  USE_NPC_NET == 1u

#define ENABLE_MULTI_TCP    1u
 
#define TCP_SERVER_RX_BUFSIZE	2000	//����tcp server���������ݳ���
#define TCP_SERVER_PORT			8088	//����tcp server�Ķ˿�
#define LWIP_SEND_DATA			0x80	//���������ݷ���

//extern u8* tcp_server_recvbuf;	//TCP�������������ݻ�����
//extern u8 tcp_server_flag;			//TCP���������ݷ��ͱ�־λ

#if ENABLE_MULTI_TCP
extern CDV_LIST *g_connected;
#endif
void EthInfoSend(CDV_INT08U uartNo);
u8 tcp_server_init(void);			//TCP��������ʼ��(����TCP�������߳�)
RET_STATUS TCP_ServerSendPlus(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite, CMD_ARG *arg);
RET_STATUS TCP_ServerSend(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite);
RET_STATUS TCP_ServerSendEx(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite, CDV_INT08U* exBuf, CDV_INT16U exLen);
//#endif
#endif

