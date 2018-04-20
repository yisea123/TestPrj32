#include "tcp_server.h"
#include "lwip/opt.h"
#include "lwip_comm.h"
//#include "led.h"
#include "lwip/sys.h"
#include "lwip/api.h"
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
 

//u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	//TCP客户端接收数据缓冲区
u8* tcp_server_recvbuf = NULL;	//TCP客户端接收数据缓冲区

u8 tcp_server_flag;								//TCP服务器数据发送标志位
struct netconn *gConn = NULL;

//TCP服务器任务
#define TCPSERVER_PRIO		5
//任务堆栈大小
#define TCPSERVER_STK_SIZE	300
//任务控制块
OS_TCB	TcpServerTaskTCB;
//任务堆栈
CPU_STK TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE];

////TCP服务器发送任务
//#define SERVERSEND_PRIO		5
////任务堆栈大小
//#define SERVERSEND_STK_SIZE	300
////任务控制块
/**
  * @brief  将CDV的一些信息通过串口发送出去
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
	
  if(gConn)
	{
		ip_addr_t addr;
		u16_t port;
		
		netconn_getaddr(gConn, &addr, &port, 0);
		sprintf(tmp , "%d.%d.%d.%d:%d\r\n" 
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
////任务堆栈
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
	conn->recv_timeout = 1000;  	//禁止阻塞线程 等待1000ms
	conn->send_timeout = 10;  	//禁止阻塞线程 等待10ms
	
	gConn = conn;
	tcp_server_recvbuf = UserMemPtr(CCM_TCP_SERVER);
	
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
	while(isLinkUp)
	{
//		//处理发送
//		if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //有数据要发送
//		{
//			err = netconn_write(conn , tcp_server_sendbuf, tcp_server_sendlen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //发送tcp_server_sendbuf中的数据
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
		
		//处理接收
		recv_err = netconn_recv(conn, &inbuf);
		
		if (recv_err == ERR_OK)
		{
			if (netconn_err(conn) == ERR_OK) 
			{
				
				netbuf_data(inbuf, (void**)&buf, &buflen);
				
				//////////////
				//OS_CRITICAL_ENTER(); //关中断
					memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //数据接收缓冲区清零
					for(q=inbuf->p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
					{
						//判断要拷贝到TCP_SERVER_RX_BUFSIZE中的数据是否大于TCP_SERVER_RX_BUFSIZE的剩余空间，如果大于
						//的话就只拷贝TCP_SERVER_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
						if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) 
							MemCpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//拷贝数据
						else 
							MemCpy(tcp_server_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_SERVER_RX_BUFSIZE) 
							break; //超出TCP客户端接收数组,跳出	
					}
					//OS_CRITICAL_EXIT();  //开中断
				//////////////
				
//				if (QUE_LEN <=buflen)
//					continue;
				
				switch(g_cdvStat){
					case CDV_RECV:
						if (TCP_COM == g_olCache.uart) {
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
							RecvParse(tcp_server_recvbuf,data_len, TCP_COM);
						}
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
		else/* if(recv_err == ERR_CLSD) */ //关闭连接
		{
			switch(g_cdvStat){
				case CDV_RECV:
					if(g_scriptRecv.tmpLen == 0 && TCP_COM == g_olCache.uart)
						g_scriptRecv.len[g_scriptRecv.rxPos] += 1;
					
					break;
			}
			break;
		}
		
		  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
		
	}
  netbuf_delete(inbuf);
		
	//while((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA);
	gConn = NULL;
  tcp_server_recvbuf = NULL;
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
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
//    /* Bind to port 80 (HTTP) with default IP address */
//    err = netconn_bind(conn, NULL, 80);
		/* Bind to port 80 (HTTP) with default IP address */
		u16 port;
		SPI_Flash_Read((CDV_INT08U *)&port, NET_ADDR + 4, sizeof(port));
    err = netconn_bind(conn, NULL, port);
    
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        /* accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
				
        if(accept_err == ERR_OK)
        {
          /* serve connection */
          http_server_serve(newconn);

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

//	u16_t g_port = 0;
//	u8 g_remot_addr[4] = {0};
//tcp服务器任务
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

	conn = netconn_new(NETCONN_TCP);  //创建一个TCP链接
	netconn_bind(conn,IP_ADDR_ANY,TCP_SERVER_PORT);  //绑定端口 8号端口
	netconn_listen(conn);  		//进入监听模式
	conn->recv_timeout = 10;  	//禁止阻塞线程 等待10ms
	while (1) 
	{
		err = netconn_accept(conn,&newconn);  //接收连接请求
		if(err==ERR_OK)newconn->recv_timeout = 10;

		if (err == ERR_OK)    //处理新连接的数据
		{ 
			struct netbuf *recvbuf;

//			netconn_getaddr(newconn,&ipaddr,&g_port,0); //获取远端IP地址和端口号
//			
//			g_remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
//			g_remot_addr[2] = (uint8_t)(ipaddr.addr >> 16);
//			g_remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
//			g_remot_addr[0] = (uint8_t)(ipaddr.addr);
////			printf("主机%d.%d.%d.%d连接上服务器,主机端口号为:%d\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
			
			while(1)
			{
//				if ((ETH->DMASR&ETH_DMASR_RBUS)!=(u32)RESET)  
//		{ 
//			ETH->DMASR = ETH_DMASR_RBUS;//清除ETH DMA的RBUS位 
//			ETH->DMARPDR=0;//恢复DMA接收
//		}
		
				if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //有数据要发送
				{
					err = netconn_write(newconn , tcp_server_sendbuf, tcp_server_sendlen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //发送tcp_server_sendbuf中的数据
					if(err != ERR_OK)
					{
//						printf("发送失败\r\n");
					}
					tcp_server_flag &= ~LWIP_SEND_DATA;
				}
				
				if((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)  	//接收到数据
				{		
					CDV_INT16U crc;
					OS_CRITICAL_ENTER(); //关中断
					memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //数据接收缓冲区清零
					for(q=recvbuf->p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
					{
						//判断要拷贝到TCP_SERVER_RX_BUFSIZE中的数据是否大于TCP_SERVER_RX_BUFSIZE的剩余空间，如果大于
						//的话就只拷贝TCP_SERVER_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
						if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) 
							MemCpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//拷贝数据
						else 
							MemCpy(tcp_server_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_SERVER_RX_BUFSIZE) 
							break; //超出TCP客户端接收数组,跳出	
					}
					OS_CRITICAL_EXIT();  //开中断
					///////////////////////////////////////////////////////////////////
					
					crc = getCRC16(tcp_server_recvbuf,data_len-2); 
					
					if((tcp_server_recvbuf[data_len-1] == ((crc >> 8) & 0xff))&&(tcp_server_recvbuf[data_len-2] == (crc & 0xff))){//crc chk
						if(0 == OnlineCmdCache(tcp_server_recvbuf , data_len-2, TCP_COM)) {
							;
						}
					}
					///////////////////////////////////////////////////////////////////
					data_len=0;  //复制完成后data_len要清零。	
//					printf("%s\r\n",tcp_server_recvbuf);  //通过串口发送接收到的数据
					netbuf_delete(recvbuf);
					
				}else if(recv_err == ERR_CLSD)  //关闭连接
				{
					netconn_close(newconn);
					netconn_delete(newconn);
//					printf("主机:%d.%d.%d.%d断开与服务器的连接\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3]);
					break;
				}
			}
//			g_port = 0;
//			memset(g_remot_addr, 0, 4);
		}
	}
}


//创建TCP服务器线程
//返回值:0 TCP服务器创建成功
//		其他 TCP服务器创建失败
u8 tcp_server_init(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();//进入临界区
	//创建TCP客户端任务
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
	OS_CRITICAL_EXIT();	//退出临界区
	return err;				 
}


/** @brief  发送命令
  * @param  
  * @retval 
  * @note   写入发送缓存，具体发送在tcp_server_thread里
  *         缓存中的第一个字节为后面的命令长度
  */
RET_STATUS TCP_ServerSend(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite){  
	err_t err;
	OS_ERR os_err;
  //判断上一次命令是否发送完成
	RET_STATUS ret = OPT_SUCCESS;
  OSSemPend(&TCP_TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &os_err); //请求信号量
//	NEW08U(tcp_server_sendbuf, NumByteToWrite);
//	memcpy(tcp_server_sendbuf, pBuffer, NumByteToWrite);
	//下面顺序不要改动
//	tcp_server_sendlen = NumByteToWrite;
	/////////////////////////////
	//err = netconn_write(gConn , tcp_server_sendbuf, tcp_server_sendlen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //发送tcp_server_sendbuf中的数据
  if(gConn)
	err = netconn_write(gConn , pBuffer, NumByteToWrite/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //发送tcp_server_sendbuf中的数据
  
	if (err != ERR_OK)
		ret = OPT_FAILURE;
	///////////////////////////////////
	OSSemPost (&TCP_TX_SEM,OS_OPT_POST_1,&os_err);
	return ret;
}

/** @brief  发送命令
  * @param  
  * @retval 
  * @note   写入发送缓存，具体发送在tcp_server_thread里
  *         缓存中的第一个字节为后面的命令长度
  */
RET_STATUS TCP_ServerSendEx(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite, CDV_INT08U* exBuf, CDV_INT16U exLen){  
	err_t err;
	OS_ERR os_err;
	CDV_INT08U *sendBuf = NULL;
	CDV_INT16U sendLen = NumByteToWrite + exLen;
  //判断上一次命令是否发送完成
	RET_STATUS ret = OPT_SUCCESS;
  OSSemPend(&TCP_TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &os_err); //请求信号量
	NEW08U(sendBuf, sendLen);
	MemCpy(sendBuf, pBuffer, NumByteToWrite);
	MemCpy(sendBuf + NumByteToWrite, exBuf, exLen);
	//下面顺序不要改动
//	tcp_server_sendlen = NumByteToWrite;
	/////////////////////////////
	//err = netconn_write(gConn , tcp_server_sendbuf, tcp_server_sendlen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //发送tcp_server_sendbuf中的数据
  if(gConn)
	  err = netconn_write(gConn , sendBuf, sendLen/*strlen((char*)tcp_server_sendbuf)*/, NETCONN_COPY); //发送tcp_server_sendbuf中的数据
	
  DELETE(sendBuf);
	
	if (err != ERR_OK)
		ret = OPT_FAILURE;
	///////////////////////////////////
	OSSemPost (&TCP_TX_SEM,OS_OPT_POST_1,&os_err);
	
	return ret;
}
