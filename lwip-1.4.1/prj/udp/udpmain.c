/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#include "udpmain.h"

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"

#define UDP_SERVER_PORT			10	//����tcp server�Ķ˿�
//TCP����������
#define UDPSERVER_PRIO		  5
//�����ջ��С
#define UDPSERVER_STK_SIZE	128
//������ƿ�
OS_TCB	UdpServerTaskTCB;
//�����ջ
CPU_STK UDPSERVER_TASK_STK[UDPSERVER_STK_SIZE];
//#define UDPECHO_THREAD_PRIO  ( tskIDLE_PRIORITY + 5 )
u8_t closeTcp = 0;
//static struct netconn *conn;
//static struct netbuf *buf;
//static struct ip_addr *addr;
//static unsigned short port;
/*-----------------------------------------------------------------------------------*/
static void udpecho_thread(void *arg)
{
  err_t err, recv_err;
	struct netconn *conn = NULL;
  struct netbuf *inbuf = NULL;
  struct ip_addr *addr = NULL;
  unsigned short port;
  char* buf = NULL, *outBuf = NULL;
  u16_t buflen = 0;
  
  
  LWIP_UNUSED_ARG(arg);

  conn = netconn_new(NETCONN_UDP);
  if (conn!= NULL)
  {
    err = netconn_bind(conn, IP_ADDR_ANY, UDP_SERVER_PORT);
    if (err == ERR_OK)
    {
      while (1) 
      {
        recv_err = netconn_recv(conn, &inbuf);
      
        if (recv_err == ERR_OK) 
        {
          addr = netbuf_fromaddr(inbuf);
          port = netbuf_fromport(inbuf);
          netconn_connect(conn, addr, port);
					//netconn_connect(conn, IP_ADDR_BROADCAST, port);
          inbuf->addr.addr = 0;
					netbuf_data(inbuf, (void**)&buf, &buflen);
					if (!strcmp(buf, "are you npc")) {
						u16 port;
		        SPI_Flash_Read((CDV_INT08U *)&port, NET_ADDR + 4, sizeof(port));
						NEWCH(outBuf, 48);//���45
						sprintf(outBuf , "yes\r\nID:%d\r\nIP:%d.%d.%d.%d\r\nPORT:%d\r\n" , CascadeGetNativeNo(), 
		                lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3],port);
						
						//memcpy(outBuf, "yes", 4);
					  netbuf_ref(inbuf, outBuf, strlen(outBuf));
            netconn_send(conn,inbuf);
					} else if (!strcmp(buf, "close tcp"))	{
						closeTcp = 1;
					}
					//netconn_sendto(conn,inbuf, addr, port);//�޷�����
					netconn_disconnect(conn);
          netbuf_delete(inbuf);
        }
      }
			DELETE(outBuf);
    }
    else
    {
      netconn_delete(conn);
    }
  }
  else
  {
  }
}
/*-----------------------------------------------------------------------------------*/
u8 udpecho_init(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();//�����ٽ���
	//����TCP�ͻ�������
	OSTaskCreate((OS_TCB 	* )&UdpServerTaskTCB,		
				 (CPU_CHAR	* )"udp_Server task", 		
                 (OS_TASK_PTR )udpecho_thread, 			
                 (void		* )0,					
                 (OS_PRIO	  )UDPSERVER_PRIO,     
                 (CPU_STK   * )&UDPSERVER_TASK_STK[0],	
                 (CPU_STK_SIZE)UDPSERVER_STK_SIZE/10,	
                 (CPU_STK_SIZE)UDPSERVER_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	return err;				 
  //sys_thread_new("udpecho_thread", udpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE,UDPECHO_THREAD_PRIO );
}

//static unsigned short port;
/*-----------------------------------------------------------------------------------*/
void udpecho_find(CMD_ARG *arg)
{
  err_t err, recv_err;
	struct netconn *conn = NULL;
  struct netbuf *rbuf = NULL;
  struct ip_addr *addr = NULL;
  unsigned short port;
  char* buf = NULL, *outBuf = NULL;
	char * buffer = NULL;
	u16_t buffer_pointer = 0;
  u16_t buflen = 0;
	struct netbuf *sbuf = netbuf_new();
  conn = netconn_new(NETCONN_UDP);
	#if _NPC_VERSION_ == 3u
	#define FIND_BUFFER_LEN 1000
	#else
	#define FIND_BUFFER_LEN 200
	#endif
	NEWCH(buffer, FIND_BUFFER_LEN);
	
	
  if (conn!= NULL)
  {
		u32 tim = 0;
    err = netconn_bind(conn, IP_ADDR_ANY, 101);
		conn->recv_timeout = 10;  	//��ֹ�����߳� �ȴ�1000ms
		
		
    if (err == ERR_OK)
		{
			//netconn_connect(conn, IP_ADDR_BROADCAST, UDP_SERVER_PORT);//udp����ʱ����connect
			NEWCH(outBuf, 48);//���45
			sprintf(outBuf , "are you npc");
			netbuf_ref(sbuf, outBuf, strlen(outBuf));
			//netconn_send(conn,sbuf);
			netconn_sendto(conn, sbuf,  IP_ADDR_BROADCAST, UDP_SERVER_PORT);
			//netconn_disconnect(conn);
			
			DELETE(outBuf);
      while (1) 
      {
        recv_err = netconn_recv(conn, &rbuf);
				//recv_err = netconn_recvfrom(conn, &inbuf);
      
        if (recv_err == ERR_OK) 
        {
					do{
						addr = netbuf_fromaddr(rbuf);
						port = netbuf_fromport(rbuf);
						//netconn_connect(conn, addr, port);
						
						rbuf->addr.addr = 0;
						netbuf_data(rbuf, (void**)&buf, &buflen);
						
						if(buf && buflen && buffer_pointer + buflen < FIND_BUFFER_LEN) {
							MemCpy(buffer + buffer_pointer, buf, buflen);
							buffer_pointer += buflen;
						}
					}while(-1 !=netbuf_next(rbuf));
					
					netbuf_delete(rbuf);
					
					tim = 0;
        }
				else if(recv_err == ERR_TIMEOUT && ++tim>100)
				{
					break;
				}
				else/* if(recv_err == ERR_CLSD) */ //�ر�����
				{
					break;
				}
      }
			//netbuf_delete(rbuf);
    }
    else
    {
    }
    netconn_delete(conn);
  }
  else
  {
  }
	
	AddTxNoCrcPlus((CDV_INT08U*)buffer, buffer_pointer,arg);
	DELETE(buffer);
  netbuf_delete(sbuf);
}
#endif /* LWIP_NETCONN */
