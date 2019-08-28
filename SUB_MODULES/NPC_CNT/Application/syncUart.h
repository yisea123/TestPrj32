
/**
  ******************************************************************************
  * @file    /syncUart.h 
  * @author  MMY
  * @version V0.0.1
  * @date    04-06-2016
  * @brief   internal implement and interface of new uart control of sync
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _SYNC_UART_
#define  _SYNC_UART_

//#define SYNC_UART_DRIVE 1u

#include "cdv_include.h"

//���滺��Ľṹ
typedef struct tagCache{
	u8 flag;
	u8 len;
	u8 *buf;
}Cache;

//�����¼����ڼ�������ĵ���Ϣ
typedef struct tagUart{
	u8 no;
}Uart;



//��ͷβ��Ϣ���ͣ����ر�ʾ���������
u8 CmdSend(Cache* send, Uart uart);
//��ͷβ��Ϣ���ͣ����ر�ʾ���ظ����
u8 CmdSendReq(Cache* send, Cache* recv, Uart uart);
//����ͷβcrc�ķ���
u8 BufSend(Cache* send, Uart uart);

//�����߳�ר��
void ThreadCmdRecv(void);


#endif

