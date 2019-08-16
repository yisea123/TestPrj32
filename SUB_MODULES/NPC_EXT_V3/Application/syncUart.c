/**
  ******************************************************************************
  * @file    /syncUart.c 
  * @author  MMY
  * @version V0.0.1
  * @date    04-06-2016
  * @brief   public define and interface of new uart control of sync
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "syncUart.h"
	//��
	#define _MASTER "{[(M)]}"
	//����
	#define _BACK "{[(B)]}"
	//β
	#define _TAIL "{[(T)]}"
	//�ظ�
	#define _REPLY "{[(R)]}"
	//ȫ�ֽ��ջ���
	typedef struct tagFifo{
		Cache *cache;//���Կ��ٶ�������������ڽ�����䣬ÿ��cache��buf��С256
		u8 no;//�ж��ٸ�������
		u8 dealCache;//��ǰ�����cache
		u8 recvCache;//��ǰ���յ�cache
		u8 dealPos;//��ǰ����Ļ�����λ��
		u8 recvPos;//��ǰ���յĻ�����λ��
		u8 flag;
	}Fifo;
	
	Fifo g_fifo = {NULL, 0, 0, 0, 0, 0, 0	};//���ջ���
	Cache g_sendCache = {0, 0, NULL} , g_recvCache = {0, 0, NULL} , g_parseCache = {0, 0, NULL};//���ͻ���
	//fifo��ʼ��
	u8 FifoInit(Fifo *fifo, u8 no);
	//����fifo
	u8 FifoDeInit(Fifo *fifo);

	//����
	//#ifdef SYNC_UART_DRIVE
	//����ѡ����
	u8 UartCmdSend(Cache* send, Uart uart);

	//#endif
	//����������
	u8 UartCmdRecv(void);



///////////////////////////////////////////////////////////////
/**
  * @brief  cache��������
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
	//����cache����Ȩ
	u8 CacheSemPend(Cache* cache)
	{
		OS_ERR err;
		while(1)
		{
			OSSemPend(&CACHE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
			
			if((*cache).flag)
			{
				OSSemPost(&CACHE_SEM,OS_OPT_POST_1,&err);
				continue;
			}
			else
			{			
				(*cache).flag = 1;
			}
			
			OSSemPost(&CACHE_SEM,OS_OPT_POST_1,&err);
			break;
		}
	}
	//�ͷ�cache����Ȩ
	u8 CacheSemPost(Cache* cache)
	{
		OS_ERR err;
		while(1)
		{
			OSSemPend(&CACHE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
			
			(*cache).flag = 0;
			
			OSSemPost(&CACHE_SEM,OS_OPT_POST_1,&err);
		}
		
	}
	
	
/**
  * @brief  fifo��������
  *  
  * @param  no��ʼ����������
  *   
  * @retval 
  *
  * @note   ��ʼ�������ٵ�
  */
	//����fifo����Ȩ
	void FifoSemPend(Fifo* fifo)
	{
		OS_ERR err;
		while(1)
		{
			OSSemPend(&CACHE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
			
			if(fifo->flag)
			{
				OSSemPost(&CACHE_SEM,OS_OPT_POST_1,&err);
				continue;
			}
			else
			{			
				fifo->flag = 1;
			}
			
			OSSemPost(&CACHE_SEM,OS_OPT_POST_1,&err);
			break;
			
		}
		
	}
	//�ͷ�fifo����Ȩ
	void FifoSemPost(Fifo* fifo)
	{
		OS_ERR err;
		while(1)
		{
			OSSemPend(&CACHE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
			
			fifo->flag = 0;
			
			OSSemPost(&CACHE_SEM,OS_OPT_POST_1,&err);
		}
		
	}
	//fifo��ʼ��
	u8 FifoInit(Fifo *fifo, u8 no)
	{
		int i = 0;
		if(fifo == NULL || no == 0)
			return 0;
		
		FifoSemPend(fifo);
		
		if(fifo->cache != NULL)
			FifoDeInit(fifo);
		
		NEWCH(fifo->cache, sizeof(Cache) * no );//��ʼ��cache
		(fifo)->no = no;//�ж��ٸ�������
		(fifo)->dealCache = 0;//��ǰ�����cache
		(fifo)->recvCache = 0;//��ǰ���յ�cache
		(fifo)->dealPos = 0;//��ǰ����Ļ�����λ��
		(fifo)->recvPos = 0;//��ǰ���յĻ�����λ��
		(fifo)->flag = 0;
		
		for(i = 0; i < no; i++)
		{
			(fifo)->cache[i].buf = NULL;
			(fifo)->cache[i].flag = 0;
			(fifo)->cache[i].len = 255;		
			NEW08U((fifo)->cache[i].buf, (fifo)->cache[i].len );			
		}
		
		FifoSemPost(fifo);
		return 1;
	}
	//����fifo
	u8 FifoDeInit(Fifo *fifo)
	{
		int i = 0;
		
		if(fifo == NULL)
			return 0;
		
		if(fifo->cache == NULL)
			return 1;
		
		FifoSemPend(fifo);
		
		for(i = 0; i< (*fifo).no; i++)//�����cache�е�buf
		{
			if((fifo)->cache[i].buf != NULL)
			{
				//CacheSemPend(&((fifo)->cache[i]));
				DELETE((fifo)->cache[i].buf);
				//(fifo)->cache[i].flag = 0;
				(fifo)->cache[i].len = 0;
				//CacheSemPost(&((fifo)->cache[i]));
			}
		}
		
		DELETE((fifo)->cache);//����fifo�е�cache
		(fifo)->no = 0;//�ж��ٸ�������
		(fifo)->dealCache = 0;//��ǰ�����cache
		(fifo)->recvCache = 0;//��ǰ���յ�cache
		(fifo)->dealPos = 0;//��ǰ����Ļ�����λ��
		(fifo)->recvPos = 0;//��ǰ���յĻ�����λ��
		
		FifoSemPost(fifo);
		return 1;
	}
	//fifo���Ӵ����ֽ�
	#define MAX_VAL_ADD(a,max) (((a)<(max)-1)?1+(a):0)
#define MAX_SELF_ADD(a,max) (a) = ((a)<(max)-1)?1+(a):0
#define MAX_SELF_SUB(a,max) (a) = ((a)==0)?(max)-1:(a)-1
	u8 FifoAdd(Fifo *fifo , u8 ch)
	{
		int no, pos;
		
		if(fifo == NULL || fifo->cache == NULL)
			return 0;
		
		FifoSemPend(fifo);
		
		no = (fifo)->recvCache;
		pos = (fifo)->recvPos;
		
		if(pos < (fifo)->cache[no].len)
		{
			
		}
		else
		{
			MAX_SELF_ADD(no, (fifo)->no);
			pos = 0;
		}
		
		(fifo)->cache[no].buf[pos] = ch;
		(fifo)->recvCache = no;//��ǰ���յ�cache
		(fifo)->recvPos = ++pos;//��ǰ���յĻ�����λ��
		
		FifoSemPost(fifo);
		return 1;
	}
/**
  * @brief  ��ͷβ��Ϣ����
  *  
  * @param  send�������ݣ�uart�����¼���Ϣ
  *   
  * @retval ���ر�ʾ���������
  *
  * @note   ����UartCmdSend,���з��ͻ���
  */
	u8 CmdSend(Cache* send, Uart uart)
	{
		
		Cache *tmpCache = &g_sendCache;
		u16 crc;
		u8 pos;
		
		if(send->len > 255 || send->len <= 0)
			return 0;
		CacheSemPend(&g_sendCache);
		
		crc = getCRC16(send->buf, send->len);
		tmpCache->len = send->len + 2 + 7*2;
		NEW08U(tmpCache->buf, tmpCache->len);
		//׼�����ʹ�������ͷ��β��crc
		pos = 0;
		memcpy(tmpCache->buf + pos, _MASTER, 7);
		pos += 7;
		memcpy(tmpCache->buf + pos, send->buf, send->len);
		pos += send->len;
		memcpy(tmpCache->buf + pos, &crc, 2);
		pos += 2;
		memcpy(tmpCache->buf + pos, _TAIL, 7);
		
		UartCmdSend(tmpCache, uart);
		
		while(tmpCache->len);//�ȴ������յ�
		
		CacheSemPost(&g_sendCache);
	}

/**
  * @brief  ��ͷβ��Ϣ����
  *         
  * @param  send�������ݣ�uart�����¼���Ϣ
  *   
  * @retval ���ر�ʾ���ظ����
  *
  * @note   
  */
	u8 CmdSendReq(Cache* send, Cache* recv, Uart uart)
	{
		Cache *tmpCache = &g_sendCache;
		u16 crc;
		u8 pos;
		
		if(send->len > 239 || send->len <= 0)
			return 0;
		while(1)
		{
			CacheSemPend(&g_sendCache);
			
			crc = getCRC16(send->buf, send->len);
			tmpCache->len = send->len + 2 + 7*2;
			NEW08U(tmpCache->buf, tmpCache->len);
			//׼�����ʹ�������ͷ��β��crc
			pos = 0;
			memcpy(tmpCache->buf + pos, _MASTER, 7);
			pos += 7;
			memcpy(tmpCache->buf + pos, send->buf, send->len);
			pos += send->len;
			memcpy(tmpCache->buf + pos, &crc, 2);
			pos += 2;
			memcpy(tmpCache->buf + pos, _TAIL, 7);
			
			
			CacheSemPend(&g_recvCache);//���ڷ����ͻظ���ͷ��һ�������Կ�����������
			g_recvCache.len = 0;
			UartCmdSend(tmpCache, uart);
			
			while(tmpCache->len);//�ȴ������յ�
			
			
			CacheSemPost(&g_sendCache);
			
			while(!g_recvCache.len);//�ȴ��ظ�
			
			if(g_recvCache.len == g_sendCache.len)
				if(0 == strncmp((CDV_INT08C*)g_recvCache.buf, (CDV_INT08C*)g_sendCache.buf, g_sendCache.len))//ֻ����UCOS�̻߳��ܵ��ȵ�ʱ���ѯ
				{
					DELETE(g_recvCache.buf);
					DELETE(g_sendCache.buf);
					g_recvCache.len = 0;
					g_sendCache.len = 0;
					CacheSemPost(&g_recvCache);
					break;
				}
					
			CacheSemPost(&g_recvCache);
		}
	}

/**
  * @brief  ����ͷβcrc�ķ���
  *  
  * @param  send�������ݣ�uart�����¼���Ϣ
  *   
  * @retval 
  *
  * @note   
  */
	u8 BufSend(Cache* send, Uart uart)
	{
		Cache *tmpCache = &g_sendCache;
		
		if(send->len > 255 || send->len <= 0)
			return 0;
		CacheSemPend(&g_sendCache);
		
		tmpCache->len = send->len;
		NEW08U(tmpCache->buf, tmpCache->len);
		
		memcpy(tmpCache->buf, send->buf, send->len);
		
		UartCmdSend(tmpCache, uart);
		
		while(tmpCache->len);//�ȴ������յ�
		
		CacheSemPost(&g_sendCache);
	}


/**
  * @brief  �����߳�ר��
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
	u8 FifoCompareBuf(Cache* cmd)
	{
		Fifo *fifo = &g_fifo;
		u8 no, pos, cnt = 0,i;
		u16 cmdPos;
		if((*fifo).dealCache == (*fifo).recvCache &&
			(*fifo).dealPos == (*fifo).recvPos)
			return 0;
		if(cmd == NULL)
			return 0;
		no = (*fifo).dealCache;
		pos = (*fifo).dealPos;
		
		for( ; no <= (*fifo).recvCache ; no++)
		{
			for ( ; pos < (*fifo).recvPos && pos < (*fifo).cache[no].len ; pos++)
			{
				switch(cnt)
				{
					case 0:
						if('{' == (*fifo).cache[no].buf[pos])
						{
							cnt++;
						}
						break;
					case 1:
						if('[' == (*fifo).cache[no].buf[pos])
						{
							cnt++;
						}
						else
						{
							cnt = 0;
						}
						break;
					case 2:
						if('(' == (*fifo).cache[no].buf[pos])
						{
							cnt++;
						}
						else
						{
							cnt = 0;
						}
						break;
					case 3:
						if('T' == (*fifo).cache[no].buf[pos])
						{
							cnt++;
						}
						else
						{
							cnt = 0;
						}
						break;
					case 4:
						if(')' == (*fifo).cache[no].buf[pos])
						{
							cnt++;
						}
						else
						{
							cnt = 0;
						}
						break;
					case 5:
						if(']' == (*fifo).cache[no].buf[pos])
						{
							cnt++;
						}
						else
						{
							cnt = 0;
						}
						break;
					case 6:
						if('}' == (*fifo).cache[no].buf[pos])
						{
							if(no == (*fifo).recvCache)
							{
								(*cmd).len = pos - (*fifo).dealPos + 1;
								NEW08U((*cmd).buf, (*cmd).len);
								memcpy((*cmd).buf, (*fifo).cache[no].buf+(*fifo).dealPos, (*cmd).len);
							}
							else
							{
								(*cmd).len = ((*fifo).recvCache - no-1)*(*fifo).cache[0].len
								+ (*fifo).cache[0].len - (*fifo).dealPos +pos +1;
								NEW08U((*cmd).buf, (*cmd).len);
								memcpy((*cmd).buf, (*fifo).cache[(*fifo).dealCache].buf+(*fifo).dealPos, (*fifo).cache[0].len - (*fifo).dealPos);
								cmdPos = (*cmd).len;
								for(i = 0; i< (*fifo).recvCache - no-1; i++)
								{
									memcpy((*cmd).buf+cmdPos, (*fifo).cache[(*fifo).dealCache+i+1].buf, (*fifo).cache[(*fifo).dealCache+i+1].len);
								  cmdPos +=(*fifo).cache[(*fifo).dealCache+i+1].len;
								}
								
								memcpy((*cmd).buf+cmdPos, (*fifo).cache[no].buf, pos +1);
							
								
								
							}
							
							return 1;
						}
						else
						{
							cnt = 0;
						}
						break;
				}
			}
		}
		return 0;
	}
	
	
	
	void ThreadCmdRecv(void)
	{
		
	}




/**
  * @brief  ����������ѡ����
  *  
  * @param  send�������ݣ�uart�����¼���Ϣ
  *   
  * @retval 
  *
  * @note   
  */

	u8 UartCmdSend(Cache* send, Uart uart)
	{
		OS_ERR err;
		OSSemPend(&SYNC_UART_SEM, 0, OS_OPT_PEND_BLOCKING, 0, &err); //�����ź���
	
		switch(uart.no)
		{
			case 1:
				DMA_usart1Send((CDV_INT32U)(*send).buf, (*send).len);
				break;
			case 5:
				DMA_usart5Send((CDV_INT32U)(*send).buf, (*send).len);
				break;
			default:
				break;
			
		}
		
		OSSemPost(&SYNC_UART_SEM, OS_OPT_POST_1, &err);
		return 1;
	}

/**
  * @brief  ������������������뻺��
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
	u8 UartCmdRecv(void)
	{
		//FifoAdd(&g_fifo, );
		return 1;
		
	}

