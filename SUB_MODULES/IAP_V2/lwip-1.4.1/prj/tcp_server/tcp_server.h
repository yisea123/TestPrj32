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

 
#define TCP_SERVER_RX_BUFSIZE	2000	//����tcp server���������ݳ���
#define TCP_SERVER_PORT			8088	//����tcp server�Ķ˿�
#define LWIP_SEND_DATA			0x80	//���������ݷ���
 #include "tcp.h"
extern u8_t closeTcp;
extern ip_addr_t g_ota_ipaddr;
//extern u8* tcp_server_recvbuf;	//TCP�������������ݻ�����
//extern u8 tcp_server_flag;			//TCP���������ݷ��ͱ�־λ
void EthInfoSend(CDV_INT08U uartNo);
u8 tcp_server_init(void);			//TCP��������ʼ��(����TCP�������߳�)
RET_STATUS TCP_ServerSend(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite);
#endif

