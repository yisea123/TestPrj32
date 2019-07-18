#include "tcp_server.h"
#include "lwip/opt.h"
#include "lwip_comm.h"
//#include "led.h"
#include "lwip/sys.h"
#include "lwip/api.h"
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
 #include "tcp.h"

//u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	//TCP�ͻ��˽������ݻ�����
u8* tcp_server_recvbuf = NULL;	//TCP�ͻ��˽������ݻ�����

u8 tcp_server_flag;								//TCP���������ݷ��ͱ�־λ
struct netconn *gConn = NULL;

#if ENABLE_MULTI_TCP
  #define NETCONN_TAG 0xEE
	CDV_LIST *g_connected = NULL;
	//TCP��������
	#define TCPCONNECT_PRIO		5
	//�����ջ��С
	#define TCPCONNECT_STK_SIZE	300
	//������ƿ�
	OS_TCB	TcpConnectTaskTCB;
	//�����ջ
	CPU_STK TCPCONNECT_TASK_STK[TCPCONNECT_STK_SIZE];
#endif

//TCP����������
#define TCPSERVER_PRIO		5
//�����ջ��С
#define TCPSERVER_STK_SIZE	256
//������ƿ�
OS_TCB	TcpServerTaskTCB;
//�����ջ
CPU_STK TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE];

////TCP��������������
//#define SERVERSEND_PRIO		5
////�����ջ��С
//#define SERVERSEND_STK_SIZE	300
////������ƿ�
/**
  * @brief  ��CDV��һЩ��Ϣͨ�����ڷ��ͳ�ȥ
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void EthInfoSend(CMD_ARG *arg) {
	CDV_INT08U i , no;
	char tmp[50]={0};
	
	sprintf(tmp , "����ip:%d.%d.%d.%d\r\n" 
		,lwipdev.ip[0]
		,lwipdev.ip[1]
		,lwipdev.ip[2]
		,lwipdev.ip[3]
		);
		AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	
	sprintf(tmp , "����mac:%02x.%02x.%02x.%02x.%02x.%02x\r\n" 
		,lwipdev.mac[0]
		,lwipdev.mac[1]
		,lwipdev.mac[2]
		,lwipdev.mac[3]
		,lwipdev.mac[4]
		,lwipdev.mac[5]
		);
		AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	
#if ENABLE_MULTI_TCP
	if(g_connected) {
		CDV_LIST *connect = g_connected->head->next;
		ip_addr_t addr;
		u16_t port;
		
		while (connect->tail != connect) { 
			netconn_getaddr(connect->data, &addr, &port, 0);			
			sprintf(tmp , "%d.%d.%d.%d:%d\r\n" 
			,(addr.addr)&0xff,(addr.addr>>8)&0xff,(addr.addr>>16)&0xff,(addr.addr>>24)&0xff,port
			);
			AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
			connect = connect->next;
		}
		
	}
#else
  if(gConn)
	{
		ip_addr_t addr;
		u16_t port;
		
		netconn_getaddr(gConn, &addr, &port, 0);
		sprintf(tmp , "%d.%d.%d.%d:%d\r\n" 
		,(addr.addr)&0xff,(addr.addr>>8)&0xff,(addr.addr>>16)&0xff,(addr.addr>>24)&0xff,port
		);
		AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	}
#endif
	else
	{
		sprintf(tmp , "no client");
		AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	}
	
}
//OS_TCB	ServerSendTaskTCB;
////�����ջ
//CPU_STK SERVERSEND_TASK_STK[SERVERSEND_STK_SIZE];

/**
  * @brief serve tcp connection  
  * @param conn: pointer on connection structure 
  * @retval None
  */
  int lenlenlen;
void http_server_serve(
#if ENABLE_MULTI_TCP
  CDV_LIST* connected_list
#else
	struct netconn *conn
#endif
		) 
{
	//CPU_SR_ALLOC();
	struct pbuf *q = NULL;
	u32 data_len = 0;
	
  struct netbuf *inbuf = NULL;
  err_t recv_err, err;
  char* buf = NULL;
  u16_t buflen;
  struct fs_file * file = NULL;
	
#if ENABLE_MULTI_TCP
	CDV_LIST* connect = connected_list->head->next;
	struct netconn *conn = NULL;
#endif
	
	tcp_server_recvbuf = UserMemPtr(CCM_TCP_SERVER);
	
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
	while (isLinkUp) {
//		//������
//		if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //������Ҫ����
//		{
//			err = netconn_write(conn , tcp_server_sendbuf, tcp_server_sendlen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //����tcp_server_sendbuf�е�����
//			if(err == ERR_OK)
//			{
//				tcp_server_flag &= ~LWIP_SEND_DATA;
//			}
//			
//		}
		if(closeTcp) {
			closeTcp = 0;
			break;
		}
#if ENABLE_MULTI_TCP
		if (g_cdvStat == CDV_RECV) {
			conn = g_olCache.arg;
		} else if(connect){
		  conn = connect->data;
		} else {
			conn = NULL;
		}
#endif
		gConn = conn;
		
		if(conn) {
			//�������
			recv_err = netconn_recv(conn, &inbuf);
			
			if (recv_err == ERR_OK)
			{
				if (netconn_err(conn) == ERR_OK) 
				{
					
					netbuf_data(inbuf, (void**)&buf, &buflen);
					
					//////////////
					//OS_CRITICAL_ENTER(); //���ж�
						memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //���ݽ��ջ���������
						for(q=inbuf->p;q!=NULL;q=q->next)  //����������pbuf����
						{
							//�ж�Ҫ������TCP_SERVER_RX_BUFSIZE�е������Ƿ����TCP_SERVER_RX_BUFSIZE��ʣ��ռ䣬�������
							//�Ļ���ֻ����TCP_SERVER_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
							if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) 
								MemCpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//��������
							else 
								MemCpy(tcp_server_recvbuf+data_len,q->payload,q->len);
							
							data_len += q->len;  	
							
							if(data_len > TCP_SERVER_RX_BUFSIZE) 
								break; //����TCP�ͻ��˽�������,����	
						}
						//OS_CRITICAL_EXIT();  //���ж�
					//////////////
					
	//				if (QUE_LEN <=buflen)
	//					continue;
					
					switch(g_cdvStat){
						case CDV_RECV:
							if (TCP_COM == g_olCache.uart && conn == g_olCache.arg) {
								if (QUE_LEN <= g_scriptRecv.len[g_scriptRecv.rxPos] + data_len)
								{
									while(SRP_QUE_HAD);
										MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
								}
								MemCpy(&g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]], tcp_server_recvbuf, data_len);
								g_scriptRecv.len[g_scriptRecv.rxPos] += data_len;
								lenlenlen += data_len;
							}
							break;
						default:
							{
	//							CDV_INT16U crc;
	//							crc = getCRC16((u8*)tcp_server_recvbuf,data_len-2); 
	//							
	//							if((tcp_server_recvbuf[data_len-1] == ((crc >> 8) & 0xff))&&(tcp_server_recvbuf[data_len-2] == (crc & 0xff))){//crc chk
	//								if(0 == OnlineCmdCache((u8*)tcp_server_recvbuf , data_len-2, 0xEE)) {
	//									;
	//								}
	//							}
								RecvParse(tcp_server_recvbuf,data_len, TCP_COM, conn);
								connect->time = GetCdvTimeTick();
							}
							break;
					}
					data_len = 0;
				}
			}
			else if(recv_err == ERR_TIMEOUT)
			{
	//			static int enable = 0;
	//			if(enable && OPT_FAILURE == TCP_ServerSend("alive", 5))
	//			{
	//				break;
	//			}
				if(conn->state == NETCONN_WRITE) { // �����Ϲر�conn

				} else if(g_cdvStat!=CDV_RECV && CalcTimeMS(GetCdvTimeTick() , connect->time) < 5000) { // �����Ϲر�conn

					
				}else{
					//break;
						#if ENABLE_MULTI_TCP
									/* Close the connection (server closes in HTTP) */
									netconn_close(conn);
									
									/* delete connection */
									netconn_delete(conn);
									
									if (connect->head != connect && connect->tail != connect) 
										connect = LIST_Remove(connect);
						#else
									netbuf_delete(inbuf);
									break;
						#endif
				}
			}
			else/* if(recv_err == ERR_CLSD) */ //�ر�����
			{
//				switch(g_cdvStat){
//					case CDV_RECV:
//						if(g_scriptRecv.tmpLen == 0 && TCP_COM == g_olCache.uart)
//							g_scriptRecv.len[g_scriptRecv.rxPos] += 1;
//						//g_olCache.arg = NULL;
//						break;
//				}
				if(g_cdvStat==CDV_RECV ) { // �����Ϲر�conn
//						if(g_scriptRecv.tmpLen == 0 && TCP_COM == g_olCache.uart)
//							g_scriptRecv.len[g_scriptRecv.rxPos] += 1;
				} else {
	#if ENABLE_MULTI_TCP
				/* Close the connection (server closes in HTTP) */
				netconn_close(conn);
				
				/* delete connection */
				netconn_delete(conn);
				
				if (connect->head != connect && connect->tail != connect) 
				  connect = LIST_Remove(connect);
	#else
				netbuf_delete(inbuf);
				break;
	#endif
			  }
			}
			
			/* Delete the buffer (netconn_recv gives us ownership,
			so we have to make sure to deallocate the buffer) */
			netbuf_delete(inbuf);
			
		}//if(conn)
		
#if ENABLE_MULTI_TCP
//		if(g_cdvStat==CDV_RECV) {// 190516 �ر���������
////						if(g_scriptRecv.tmpLen == 0 && TCP_COM == g_olCache.uart)
////							g_scriptRecv.len[g_scriptRecv.rxPos] += 1;
//	#if ENABLE_MULTI_TCP 
//				  if(g_olCache.arg != connect->data) {
//						/* Close the connection (server closes in HTTP) */
//						netconn_close(connect->data);
//						
//						/* delete connection */
//						netconn_delete(connect->data);
//						
//						if (connect->head != connect && connect->tail != connect) 
//							connect = LIST_Remove(connect);
//					}
//	#endif
//			
//				}

		//if(g_cdvStat!=CDV_RECV) {
			if (connect->tail == connect->next || connect->tail == connect) {
				connect = connect->head->next;
			} else {
				connect = connect->next;
			}
	  //}
			
#endif
	} // while(isLinkUp)
	
	
		
	//while((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA);
	gConn = NULL;
  tcp_server_recvbuf = NULL;
#if ENABLE_MULTI_TCP
	connect = connect->head->next;
	
	while (connect->tail != connect) { 
		conn = connect->data;
    netconn_close(conn);
    netconn_delete(conn);
		connect = LIST_Remove(connect);
	}
#else
	/* Close the connection (server closes in HTTP) */
  netconn_close(conn);
  
  /* delete connection */
  netconn_delete(conn);
	
#endif
}

#if ENABLE_MULTI_TCP
static void netconn_connected_thread(void *arg) {
	while(1)
	  http_server_serve((CDV_LIST *)arg);
}

//����TCP�����߳�
//����ֵ:0 TCP�����������ɹ�
//		���� TCP����������ʧ��
u8 tcp_connected_init(void *arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();//�����ٽ���
	//����TCP�ͻ�������
	OSTaskCreate((OS_TCB 	* )&TcpConnectTaskTCB,		
				 (CPU_CHAR	* )"tcp_connect task", 		
                 (OS_TASK_PTR )netconn_connected_thread, 			
                 (void		* )arg,					
                 (OS_PRIO	  )TCPCONNECT_PRIO,     
                 (CPU_STK   * )&TCPCONNECT_TASK_STK[0],	
                 (CPU_STK_SIZE)TCPCONNECT_STK_SIZE/10,	
                 (CPU_STK_SIZE)TCPCONNECT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	return err;				 
}
#endif
/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void netconn_server_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err, accept_err;
	ip_addr_t ipaddr;
	u16_t port;
#if ENABLE_MULTI_TCP
  static char connected = 0;
	CDV_LIST *connected_list = NULL;
#endif
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
		/* Bind to port 80 (HTTP) with default IP address */
		u16 port/* = 60000*/;
		SPI_Flash_Read((CDV_INT08U *)&port, NET_ADDR + 4, sizeof(port));
    err = netconn_bind(conn, NULL, port);
    conn->pcb.tcp->so_options |= SOF_KEEPALIVE;
		
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
#if ENABLE_MULTI_TCP
	    connected_list = LIST_Cteate();
			g_connected = connected_list;
#endif
      while(1) 
      {
        /* accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
				
        if(accept_err == ERR_OK)
        {
					if(g_cdvStat == CDV_RECV)
					{
					  /* Close the connection (server closes in HTTP) */
            netconn_close(newconn);
  
            /* delete connection */
            netconn_delete(newconn);
						continue;
						
					}
					netconn_getaddr(newconn,&ipaddr,&port,0); //��ȡԶ��IP��ַ�Ͷ˿ں�
					newconn->recv_timeout = 10;  	//��ֹ�����߳� �ȴ�10ms
					newconn->send_timeout = 5000;  	//��ֹ�����߳� �ȴ�10ms
          /* serve connection */
#if ENABLE_MULTI_TCP
					LIST_AddTail(connected_list, newconn,/* sizeof(newconn),*/ NETCONN_TAG);
          if(!connected) {
						tcp_connected_init(connected_list);
						connected = 1;
					}
#else
          http_server_serve(newconn);
#endif
//					/* Close the connection (server closes in HTTP) */
//          netconn_close(newconn);
//  
//          /* delete connection */
//          netconn_delete(newconn);
				}
      }
    }
    else
    {
      /* delete connection */
      netconn_delete(conn);
      //printf("can not bind netconn");
    }
  }
  else
  {
    //printf("can not create netconn");
  }
}

//	u16_t g_port = 0;
//	u8 g_remot_addr[4] = {0};
//tcp����������
static void tcp_server_thread(void *arg)
{
	CPU_SR_ALLOC();
	u8 *tcp_server_sendbuf = NULL;	
  u16 tcp_server_sendlen = 0;
	u32 data_len = 0;
	struct pbuf *q;
	err_t err,recv_err;
	struct netconn *conn, *newconn;
	static ip_addr_t ipaddr;

	LWIP_UNUSED_ARG(arg);

	conn = netconn_new(NETCONN_TCP);  //����һ��TCP����
	netconn_bind(conn,IP_ADDR_ANY,TCP_SERVER_PORT);  //�󶨶˿� 8�Ŷ˿�
	netconn_listen(conn);  		//�������ģʽ
	conn->recv_timeout = 10;  	//��ֹ�����߳� �ȴ�10ms
	while (1) 
	{
		err = netconn_accept(conn,&newconn);  //������������
		if(err==ERR_OK)newconn->recv_timeout = 10;

		if (err == ERR_OK)    //���������ӵ�����
		{ 
			struct netbuf *recvbuf;

//			netconn_getaddr(newconn,&ipaddr,&g_port,0); //��ȡԶ��IP��ַ�Ͷ˿ں�
//			
//			g_remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
//			g_remot_addr[2] = (uint8_t)(ipaddr.addr >> 16);
//			g_remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
//			g_remot_addr[0] = (uint8_t)(ipaddr.addr);
////			printf("����%d.%d.%d.%d�����Ϸ�����,�����˿ں�Ϊ:%d\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
			
			while(1)
			{
//				if ((ETH->DMASR&ETH_DMASR_RBUS)!=(u32)RESET)  
//		{ 
//			ETH->DMASR = ETH_DMASR_RBUS;//���ETH DMA��RBUSλ 
//			ETH->DMARPDR=0;//�ָ�DMA����
//		}
		
				if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //������Ҫ����
				{
					err = netconn_write(newconn , tcp_server_sendbuf, tcp_server_sendlen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //����tcp_server_sendbuf�е�����
					if(err != ERR_OK)
					{
//						printf("����ʧ��\r\n");
					}
					tcp_server_flag &= ~LWIP_SEND_DATA;
				}
				
				if((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)  	//���յ�����
				{		
					CDV_INT16U crc;
					OS_CRITICAL_ENTER(); //���ж�
					memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //���ݽ��ջ���������
					for(q=recvbuf->p;q!=NULL;q=q->next)  //����������pbuf����
					{
						//�ж�Ҫ������TCP_SERVER_RX_BUFSIZE�е������Ƿ����TCP_SERVER_RX_BUFSIZE��ʣ��ռ䣬�������
						//�Ļ���ֻ����TCP_SERVER_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
						if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) 
							MemCpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//��������
						else 
							MemCpy(tcp_server_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_SERVER_RX_BUFSIZE) 
							break; //����TCP�ͻ��˽�������,����	
					}
					OS_CRITICAL_EXIT();  //���ж�
					///////////////////////////////////////////////////////////////////
					
					crc = getCRC16(tcp_server_recvbuf,data_len-2); 
					
					if((tcp_server_recvbuf[data_len-1] == ((crc >> 8) & 0xff))&&(tcp_server_recvbuf[data_len-2] == (crc & 0xff))){//crc chk
						if(0 == OnlineCmdCache(tcp_server_recvbuf , data_len-2, TCP_COM, conn)) {
							;
						}
					}
					///////////////////////////////////////////////////////////////////
					data_len=0;  //������ɺ�data_lenҪ���㡣	
//					printf("%s\r\n",tcp_server_recvbuf);  //ͨ�����ڷ��ͽ��յ�������
					netbuf_delete(recvbuf);
					
				}else if(recv_err == ERR_CLSD)  //�ر�����
				{
					netconn_close(newconn);
					netconn_delete(newconn);
//					printf("����:%d.%d.%d.%d�Ͽ��������������\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3]);
					break;
				}
			}
//			g_port = 0;
//			memset(g_remot_addr, 0, 4);
		}
	}
}


//����TCP�������߳�
//����ֵ:0 TCP�����������ɹ�
//		���� TCP����������ʧ��
u8 tcp_server_init(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();//�����ٽ���
	//����TCP�ͻ�������
	OSTaskCreate((OS_TCB 	* )&TcpServerTaskTCB,		
				 (CPU_CHAR	* )"tcp_Server task", 		
                 (OS_TASK_PTR )netconn_server_thread, 			
                 (void		* )0,					
                 (OS_PRIO	  )TCPSERVER_PRIO,     
                 (CPU_STK   * )&TCPSERVER_TASK_STK[0],	
                 (CPU_STK_SIZE)TCPSERVER_STK_SIZE/10,	
                 (CPU_STK_SIZE)TCPSERVER_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	return err;				 
}


/** @brief  ��������
  * @param  
  * @retval OPT_TCP_ERROR tcp�쳣����ֹͣ����
            OPT_SUCCESS ���ͳɹ�
            OPT_FAILURE ����ʧ�ܣ����Գ����ٴη���
  * @note   д�뷢�ͻ��棬���巢����tcp_server_thread��
  *         �����еĵ�һ���ֽ�Ϊ����������
  */
RET_STATUS TCP_ServerSend(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite){  
	err_t err;
	OS_ERR os_err;
  //�ж���һ�������Ƿ������
	RET_STATUS ret = OPT_SUCCESS;
  OSMutexPend(&TCP_TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &os_err); //�����ź���
//	NEW08U(tcp_server_sendbuf, NumByteToWrite);
//	memcpy(tcp_server_sendbuf, pBuffer, NumByteToWrite);
	//����˳��Ҫ�Ķ�
//	tcp_server_sendlen = NumByteToWrite;
	/////////////////////////////
	//err = netconn_write(gConn , tcp_server_sendbuf, tcp_server_sendlen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //����tcp_server_sendbuf�е�����
  if(gConn)
	err = netconn_write(gConn , pBuffer, NumByteToWrite/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //����tcp_server_sendbuf�е�����
  
	if (err != ERR_OK) {
		if (err != ERR_TIMEOUT) {
			ret = OPT_TCP_ERROR;
		}else {
		  ret = OPT_FAILURE;
		}
	}
	///////////////////////////////////
	OSMutexPost (&TCP_TX_SEM,OS_OPT_POST_NO_SCHED,&os_err);
	return ret;
}

/** @brief  ��������
  * @param  
  * @retval OPT_TCP_ERROR tcp�쳣����ֹͣ����
            OPT_SUCCESS ���ͳɹ�
            OPT_FAILURE ����ʧ�ܣ����Գ����ٴη���
  * @note   д�뷢�ͻ��棬���巢����tcp_server_thread��
  *         �����еĵ�һ���ֽ�Ϊ����������
  */
RET_STATUS TCP_ServerSendPlus(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite, CMD_ARG *arg) {
	err_t err;
	OS_ERR os_err;
  //�ж���һ�������Ƿ������
	RET_STATUS ret = OPT_SUCCESS;
	ASSERT(arg);
  OSMutexPend(&TCP_TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &os_err); //�����ź���
	
  if(arg->arg)
	  err = netconn_write(arg->arg , pBuffer, NumByteToWrite/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //����tcp_server_sendbuf�е�����
  
	if (err != ERR_OK) {
		if (err != ERR_TIMEOUT) {
			ret = OPT_TCP_ERROR;
		} else {
		  ret = OPT_FAILURE;
		}
	}
	
	OSMutexPost(&TCP_TX_SEM,OS_OPT_POST_NO_SCHED,&os_err);
	return ret;
}

/** @brief  ��������
  * @param  
  * @retval 
  * @note   д�뷢�ͻ��棬���巢����tcp_server_thread��
  *         �����еĵ�һ���ֽ�Ϊ����������
  */
RET_STATUS TCP_ServerSendEx(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite, CDV_INT08U* exBuf, CDV_INT16U exLen){  
	err_t err;
	OS_ERR os_err;
	CDV_INT08U *sendBuf = NULL;
	CDV_INT16U sendLen = NumByteToWrite + exLen;
  //�ж���һ�������Ƿ������
	RET_STATUS ret = OPT_SUCCESS;
  OSMutexPend(&TCP_TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &os_err); //�����ź���
	NEW08U(sendBuf, sendLen);
	MemCpy(sendBuf, pBuffer, NumByteToWrite);
	MemCpy(sendBuf + NumByteToWrite, exBuf, exLen);
	//����˳��Ҫ�Ķ�
//	tcp_server_sendlen = NumByteToWrite;
	/////////////////////////////
	//err = netconn_write(gConn , tcp_server_sendbuf, tcp_server_sendlen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //����tcp_server_sendbuf�е�����
  if(gConn)
	  err = netconn_write(gConn , sendBuf, sendLen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //����tcp_server_sendbuf�е�����
	
  DELETE(sendBuf);
	
	if (err != ERR_OK)
		ret = OPT_FAILURE;
	///////////////////////////////////
	OSMutexPost (&TCP_TX_SEM,OS_OPT_POST_NO_SCHED,&os_err);
	
	return ret;
}
/** @brief  ��������
  * @param  
  * @retval 
  * @note   д�뷢�ͻ��棬���巢����tcp_server_thread��
  *         �����еĵ�һ���ֽ�Ϊ����������
  */
RET_STATUS TCP_ServerSendExPlus(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite, CDV_INT08U* exBuf, CDV_INT16U exLen, CMD_ARG *arg){  
	err_t err;
	OS_ERR os_err;
	CDV_INT08U *sendBuf = NULL;
	CDV_INT16U sendLen = NumByteToWrite + exLen;
  //�ж���һ�������Ƿ������
	RET_STATUS ret = OPT_SUCCESS;
	ASSERT(arg);
  OSMutexPend(&TCP_TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &os_err); //�����ź���
	NEW08U(sendBuf, sendLen);
	MemCpy(sendBuf, pBuffer, NumByteToWrite);
	MemCpy(sendBuf + NumByteToWrite, exBuf, exLen);
	//����˳��Ҫ�Ķ�
//	tcp_server_sendlen = NumByteToWrite;
	/////////////////////////////
	//err = netconn_write(gConn , tcp_server_sendbuf, tcp_server_sendlen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //����tcp_server_sendbuf�е�����
  if(arg->arg)
	  err = netconn_write(arg->arg , sendBuf, sendLen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //����tcp_server_sendbuf�е�����
	
  DELETE(sendBuf);
	
	if (err != ERR_OK)
		ret = OPT_FAILURE;
	///////////////////////////////////
	OSMutexPost (&TCP_TX_SEM,OS_OPT_POST_NO_SCHED,&os_err);
	
	return ret;
}
u32_t GetIpFromArg(CMD_ARG *arg){  
	ip_addr_t ipaddr = {0};
	u16_t port = 0;
	if(arg)
	netconn_getaddr((struct netconn*)(arg->arg),&ipaddr,&port,0); //��ȡԶ��IP��ַ�Ͷ˿ں�
  
	return ipaddr.addr;
}
