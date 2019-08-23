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
u8_t closeTcp = 0;
//u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	//TCP�ͻ��˽������ݻ�����
u8* tcp_server_recvbuf = NULL;	//TCP�ͻ��˽������ݻ�����

u8 tcp_server_flag;								//TCP���������ݷ��ͱ�־λ
struct netconn *gConn = NULL;
ip_addr_t g_ota_ipaddr = {0};

//TCP����������
#define TCPSERVER_PRIO		5
//�����ջ��С
#define TCPSERVER_STK_SIZE	300
//������ƿ�
OS_TCB	TcpServerTaskTCB;
//�����ջ
CPU_STK TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE];

#define SerialPutString(str) AddTxNoCrc((u8*)str, strlen(str), MAIN_COM)

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
void EthInfoSend(CDV_INT08U uartNo) {
	CDV_INT08U i , no;
	char tmp[50]={0};
	
	sprintf(tmp , "����ip:%d.%d.%d.%d\r\n" 
		,lwipdev.ip[0]
		,lwipdev.ip[1]
		,lwipdev.ip[2]
		,lwipdev.ip[3]
		);
		AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
	
	sprintf(tmp , "����mac:%02x.%02x.%02x.%02x.%02x.%02x\r\n" 
		,lwipdev.mac[0]
		,lwipdev.mac[1]
		,lwipdev.mac[2]
		,lwipdev.mac[3]
		,lwipdev.mac[4]
		,lwipdev.mac[5]
		);
		AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
	
  if(gConn)
	{
		ip_addr_t addr;
		u16_t port;
		
		netconn_getaddr(gConn, &addr, &port, 0);
		sprintf(tmp , "Զ��ip:%d.%d.%d.%d:%d\r\n" 
		,(addr.addr)&0xff,(addr.addr>>8)&0xff,(addr.addr>>16)&0xff,(addr.addr>>24)&0xff,port
		);
	}
	else
	{
		sprintf(tmp , "no client");
	}
	
	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
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
void http_server_serve(struct netconn *conn) 
{
	//CPU_SR_ALLOC();
	struct pbuf *q;
	u32 data_len = 0;
	
  struct netbuf *inbuf;
  err_t recv_err, err;
  char* buf;
  u16_t buflen;
  struct fs_file * file;
	conn->recv_timeout = 1000;  	//��ֹ�����߳� �ȴ�1000ms
	conn->send_timeout = 100;  	//��ֹ�����߳� �ȴ�10ms
	
	gConn = conn;
	tcp_server_recvbuf = UserMemPtr(CCM_TCP_SERVER);
	SerialPutString("connected\r\n");
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
	while(isLinkUp)
	{
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
		while(closeTcp) {
			//closeTcp = 0;
			//break;
			;
		}
		
		//�������
		recv_err = netconn_recv(conn, &inbuf);
		
		//SerialPutString("no recv\r\n");
		
		if (recv_err == ERR_OK)
		{
			//SerialPutString("recv ok\r\n");
			if (netconn_err(conn) == ERR_OK) 
			{
				netbuf_data(inbuf, (void**)&buf, &buflen);
				
				//////////////
				if (g_cdvStat == CDV_RECV&&TCP_COM == g_scriptRecv.port) {
					for(q=inbuf->p;q!=NULL;q=q->next)  //����������pbuf����
					{
						lenlenlen += q->len;
						if (QUE_LEN <= g_scriptRecv.len[g_scriptRecv.rxPos] + q->len)
						{
							while(SRP_QUE_HAD);
//							if(SRP_QUE_HAD)
//								goto TCP_SERVER_DELETE;
							MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
							
						}
						memcpy(&g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]], q->payload, q->len);
						g_scriptRecv.len[g_scriptRecv.rxPos] += q->len;
						
					}
				} else {
				//OS_CRITICAL_ENTER(); //���ж�
					memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //���ݽ��ջ���������
					for(q=inbuf->p;q!=NULL;q=q->next)  //����������pbuf����
					{
						//�ж�Ҫ������TCP_SERVER_RX_BUFSIZE�е������Ƿ����TCP_SERVER_RX_BUFSIZE��ʣ��ռ䣬�������
						//�Ļ���ֻ����TCP_SERVER_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
						if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) 
							memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//��������
						else 
							memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_SERVER_RX_BUFSIZE) 
							break; //����TCP�ͻ��˽�������,����	
					}
					//OS_CRITICAL_EXIT();  //���ж�
				}
				//////////////
				
//				if (QUE_LEN <=buflen)
//					continue;
				
				switch(g_cdvStat){
					case CDV_RECV:
//						if (TCP_COM == g_scriptRecv.port) {
//							if (QUE_LEN <= g_scriptRecv.len[g_scriptRecv.rxPos] + data_len)
//							{
//								//while(SRP_QUE_HAD);
//									MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
//							}
//							memcpy(&g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]], tcp_server_recvbuf, data_len);
//							g_scriptRecv.len[g_scriptRecv.rxPos] += data_len;
//							lenlenlen += data_len;
//						}
						break;
					default:
						{
							lenlenlen= 0;
							//CDV_INT16U crc;
							//crc = getCRC16((u8*)tcp_server_recvbuf,data_len-2); 
							AddTxNoCrc((CDV_INT08U*)tcp_server_recvbuf, data_len, MAIN_COM);
							//if((tcp_server_recvbuf[data_len-1] == ((crc >> 8) & 0xff))&&(tcp_server_recvbuf[data_len-2] == (crc & 0xff))){//crc chk
							if(0 == OnlineCmdCache((u8*)tcp_server_recvbuf , data_len, TCP_COM)) {
								;
							}
							//}
						}
						//	RecvParse(tcp_server_recvbuf,data_len, TCP_COM);
						break;
				}
				data_len = 0;
			}
		}
		else if(recv_err == ERR_TIMEOUT)
		{
			static int enable = 0;
			if(enable && OPT_FAILURE == TCP_ServerSend("alive", 5))
			{
				break;
			}
		}
		else/* if(recv_err == ERR_CLSD) */ //�ر�����
		{
			switch(g_cdvStat){
				case CDV_RECV:
					//if(g_scriptRecv.tmpLen == 0 && TCP_COM == g_olCache.uart)
					//	g_scriptRecv.len[g_scriptRecv.rxPos] += 1;
					
					break;
			}
			break;
		}
TCP_SERVER_DELETE:
		  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
		
	}
  netbuf_delete(inbuf);
		
	//while((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA);
	gConn = NULL;
  tcp_server_recvbuf = NULL;
	
	while(g_cdvStat==CDV_RECV);//��ֹ������
	
}

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
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
//    /* Bind to port 80 (HTTP) with default IP address */
//    err = netconn_bind(conn, NULL, 80);
		/* Bind to port 80 (HTTP) with default IP address */
		u16 port = 60000;
		//SPI_Flash_Read((CDV_INT08U *)&port, NET_ADDR + 4, sizeof(port));
    err = netconn_bind(conn, NULL, port);
    conn->pcb.tcp->so_options |= SOF_KEEPALIVE;
		
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
	{
		char tmp[50]={0};
		sprintf(tmp , "�ȴ�Զ��ip:%d.%d.%d.%d����bin\r\n" 
		,(g_ota_ipaddr.addr)&0xff,(g_ota_ipaddr.addr>>8)&0xff,(g_ota_ipaddr.addr>>16)&0xff,(g_ota_ipaddr.addr>>24)&0xff
		);
		AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), MAIN_COM);
	}
      while(1) 
      {
        /* accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
					netconn_getaddr(newconn,&ipaddr,&port,0); //��ȡԶ��IP��ַ�Ͷ˿ںš��������԰�port�󶨵�һ���̶���port������30000
			    if(ipaddr.addr == g_ota_ipaddr.addr) {
          /* serve connection */
            http_server_serve(newconn);
          }
					/* Close the connection (server closes in HTTP) */
          netconn_close(newconn);
  
          /* delete connection */
          netconn_delete(newconn);
        }
      }
    }
    else
    {
      /* delete connection */
      netconn_delete(newconn);
      //printf("can not bind netconn");
    }
  }
  else
  {
    //printf("can not create netconn");
  }
}


//tcp����������
static void tcp_server_thread(void *arg)
{
	CPU_SR_ALLOC();
	u8 *tcp_server_sendbuf = NULL;	
  u16 tcp_server_sendlen = 0;
	u32 data_len = 0;
	struct pbuf *q;
	err_t err,recv_err;
	u8 remot_addr[4];
	struct netconn *conn, *newconn;
	static ip_addr_t ipaddr;
	static u16_t 			port;
	
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

			netconn_getaddr(newconn,&ipaddr,&port,0); //��ȡԶ��IP��ַ�Ͷ˿ں�
			
			remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
			remot_addr[2] = (uint8_t)(ipaddr.addr >> 16);
			remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
			remot_addr[0] = (uint8_t)(ipaddr.addr);
//			printf("����%d.%d.%d.%d�����Ϸ�����,�����˿ں�Ϊ:%d\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
			
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
							memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//��������
						else 
							memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_SERVER_RX_BUFSIZE) 
							break; //����TCP�ͻ��˽�������,����	
					}
					OS_CRITICAL_EXIT();  //���ж�
					///////////////////////////////////////////////////////////////////
					
					crc = getCRC16(tcp_server_recvbuf,data_len-2); 
					
					if((tcp_server_recvbuf[data_len-1] == ((crc >> 8) & 0xff))&&(tcp_server_recvbuf[data_len-2] == (crc & 0xff))){//crc chk
						if(0 == OnlineCmdCache(tcp_server_recvbuf , data_len-2, 0xEE)) {
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
  * @retval 
  * @note   д�뷢�ͻ��棬���巢����tcp_server_thread��
  *         �����еĵ�һ���ֽ�Ϊ����������
  */
RET_STATUS TCP_ServerSend(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite){  
	err_t err;
	OS_ERR os_err;
  //�ж���һ�������Ƿ������
	RET_STATUS ret = OPT_SUCCESS;
  OSSemPend(&TCP_TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &os_err); //�����ź���
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
	OSSemPost (&TCP_TX_SEM,OS_OPT_POST_1,&os_err);
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
  OSSemPend(&TCP_TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &os_err); //�����ź���
	NEW08U(sendBuf, sendLen);
	memcpy(sendBuf, pBuffer, NumByteToWrite);
	memcpy(sendBuf + NumByteToWrite, exBuf, exLen);
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
	OSSemPost (&TCP_TX_SEM,OS_OPT_POST_1,&os_err);
	return ret;
}