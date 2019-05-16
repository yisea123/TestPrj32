
/**
  ******************************************************************************
  * @file    /mainUart.h 
  * @author  MMY
  * @version V0.0.1
  * @date    30-11-2015
  * @brief   a package of uart communicate with kfc and  yyc
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _MAIN_UART_
#define  _MAIN_UART_


#include "cdv_include.h" 

extern CDV_INT32U fpga_i;

extern CDV_INT08U MainUsart;
//void TIM3_Init(CDV_INT32U TIM_scale, CDV_INT32U TIM_Period);

/*�����������*/


void FpgaRecvInit(void);
void FpgaRecvDeinit(void);
void FpgaRecvCtl(void);



/*
 *���ڽ�����ض���
 */
#define MAX_VAL_ADD(a,max) (((a)<(max)-1)?1+(a):0)
#define MAX_SELF_ADD(a,max) (a) = ((a)<(max)-1)?1+(a):0
#define MAX_SELF_SUB(a,max) (a) = ((a)==0)?(max)-1:(a)-1

#if USE_MEMMNG == 1u
#define USART_RX_QUEUE_LENGTH 2     /*���ڽ��ն��г���*/
#define USART_RX_BUF_LENGTH 500    /*���ڽ��ճ���*/
#else
#define USART_RX_QUEUE_LENGTH 3     /*���ڽ��ն��г���*/
#define USART_RX_BUF_LENGTH 300    /*���ڽ��ճ���*/
#endif
#define USART_RX_QUEUE_SELF_ADD do{\
	if(0 == g_uartRx.QUEUE.rxLen[g_uartRx.rxPos])break;\
	(g_uartRx.doPos!=MAX_VAL_ADD(g_uartRx.rxPos,USART_RX_QUEUE_LENGTH))?MAX_SELF_ADD(g_uartRx.rxPos,USART_RX_QUEUE_LENGTH):NULL;\
	g_uartRx.QUEUE.rxLen[g_uartRx.rxPos]=0;\
}while(0);
#define USART_RX_BUF_ADD_CHAR(ch) do{\
	g_uartRx.QUEUE.rxBuf[g_uartRx.rxPos][g_uartRx.QUEUE.rxLen[g_uartRx.rxPos]]=(CDV_INT08U)(ch);\
	g_uartRx.QUEUE.rxLen[g_uartRx.rxPos] = g_uartRx.QUEUE.rxLen[g_uartRx.rxPos]<USART_RX_BUF_LENGTH-1?1+g_uartRx.QUEUE.rxLen[g_uartRx.rxPos]:USART_RX_BUF_LENGTH-1;\
}while(0);
#define USART_RX_HAD  (g_uartRx.rxPos!=g_uartRx.doPos) /*&& 0 != g_uartRx.QUEUE.rxLen[g_uartRx.doPos]*/
#define USART_RX_QUEUE_DO_NEXT do{\
	(g_uartRx.rxPos!=g_uartRx.doPos) ? MAX_SELF_ADD(g_uartRx.doPos,USART_RX_QUEUE_LENGTH) : NULL;\
}while(0);
//g_uartRx.QUEUE.rxLen[g_uartRx.doPos]=0;
#define USART_RX_DO_ADDR g_uartRx.QUEUE.rxBuf[g_uartRx.doPos]
#define USART_RX_DO_LEN g_uartRx.QUEUE.rxLen[g_uartRx.doPos]
#define USART_RX_LEN g_uartRx.QUEUE.rxLen[g_uartRx.rxPos]
typedef struct{
	struct{
	  CDV_INT08U rxBuf[USART_RX_QUEUE_LENGTH][USART_RX_BUF_LENGTH];         /*���ն���*/
	  CDV_INT16U rxLen[USART_RX_QUEUE_LENGTH];              /*���ճ���*/	
	} QUEUE;

  CDV_INT08U rxPos;              /*�������λ��*/	
	CDV_INT08U doPos;              /*���鴦��λ��*/	
} USART_RX;

extern USART_RX g_uartRx;

/*
 *���ڷ�����ض���
 */
#define USART_TX_QUEUE_LENGTH 3     /*���ڷ��Ͷ��г���*/
#define USART_TX_QUEUE_SELF_ADD do{\
	if(0 == g_uartTx.QUEUE.txLen[g_uartTx.txPos])break;\
	(g_uartTx.doPos!=MAX_VAL_ADD(g_uartTx.txPos,USART_TX_QUEUE_LENGTH))?MAX_SELF_ADD(g_uartTx.txPos,USART_TX_QUEUE_LENGTH):NULL;\
}while(0);
#define USART_CAN_DO (g_uartTx.doPos!=MAX_VAL_ADD(g_uartTx.txPos,USART_TX_QUEUE_LENGTH))

#define USART_TX_HAD  (g_uartTx.txPos!=g_uartTx.doPos) /*&& 0 != g_uartTx.QUEUE.txLen[g_uartTx.doPos]*/
/*do��ǰbuf��������0���ж���һ�����Ƿ������ڽ��յģ��������+1�����򲻱�*/
#define USART_TX_QUEUE_DO_NEXT UsartTxQueueDoNext()
/*tx��ǰbuf���볤��A������A����ǰbuf����*/
#define USART_TX_ADD_WITH_LEN(A) UsartTxAddWithLen(A)
#define USART_TX_BUF(A) g_uartTx.QUEUE.txBuf[g_uartTx.txPos][(A)]
#define USART_TX_BUF_ADDR g_uartTx.QUEUE.txBuf[g_uartTx.txPos]
#define USART_TX_DO_ADDR g_uartTx.QUEUE.txBuf[g_uartTx.doPos]
#define USART_TX_DO_LEN g_uartTx.QUEUE.txLen[g_uartTx.doPos]


typedef struct{
	struct{
	  CDV_INT08U* txBuf[USART_TX_QUEUE_LENGTH];         /*���Ͷ���*/
	  CDV_INT16U txLen[USART_TX_QUEUE_LENGTH];              /*���ͳ���*/	
	} QUEUE;
  CDV_INT08U txPos;              /*���鷢��λ��*/	
	CDV_INT08U doPos;              /*���鴦��λ��*/	
} USART_TX;

extern USART_TX g_uartTx;

/*�ű��������*/
#define QUE_NUM 2
#define QUE_LEN 0x1000
#define SRP_QUE_HAD  (g_scriptRecv.rxPos!=g_scriptRecv.doPos)
typedef struct
{
	CDV_INT08U /*no, */rxPos, doPos;
	CDV_INT16U len[QUE_NUM];
	CDV_INT32U totalLen, tmpLen;
	CDV_INT32U addr;
	CDV_INT08U *buf[QUE_NUM];
}SCRIPT_RECV_QUE;

extern SCRIPT_RECV_QUE g_scriptRecv;

/*
 *����ƥ����ض���
 */
#define IS_TX_NEED_REQUEST g_cacheTx.mark
typedef struct{

	CDV_INT08U* buf;         /*���Ͷ���*/
	CDV_INT16U len;          /*���鴦��λ��*/	
	CDV_INT08U mark;         /*�ظ����*/
	CDV_INT08U uartNo;
} USART_CACHE;
extern USART_CACHE g_cacheTx;
extern CDV_INT08U g_needRequest;    

/*
 * ���������
 */
 #define HAVE_ONLINE_CMD g_olCache.mark
typedef struct{

	CDV_INT08U* buf;         /*���Ͷ���*/
	CDV_INT16U len;          /*���鴦��λ��*/	
	CDV_INT08U uart;         /*��Ǵ��ĺŴ��ڷ�����*/
	CDV_INT08U mark;         /*�ظ����*/
	void *arg;               /*������*/
} OLCMD_CACHE;
extern OLCMD_CACHE g_olCache;
extern OLCMD_CACHE g_portCmdCache;

unsigned short MODBUS_CRC16(unsigned char* pchMsg, unsigned short wDataLen, unsigned short crc);
RET_STATUS RecvParse(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CDV_INT08U uartNo, void *arg);
CDV_INT16U getCRC16(CDV_INT08U *ptr,CDV_INT16U len) ;
CDV_INT16U oneCRC16(CDV_INT08U ch , CDV_INT16U crc) ;
RET_STATUS OnlineParse(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg);
void RequestAdd(CDV_INT08U* rxBuf,CDV_INT16U txLen);
void OperateScript(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CMD_ARG *arg);
//void OnlineRequest(CDV_INT08U no,CDV_INT08U res,CDV_INT08U resNo, CDV_INT08U uartNo);
void ModbusRequest(CDV_INT08U no,CDV_INT08U err, CMD_ARG *arg);
CDV_INT08U RequestCmp(void);
CDV_INT08U NeedRequestTx(CDV_INT08U uartNo);
void AddTxNoCrcPlus(CDV_INT08U* txBuf, CDV_INT16U txLen, CMD_ARG *arg);
void AddTxNoCrc(CDV_INT08U* txBuf, CDV_INT16U txLen, CDV_INT08U uartNo);
void AddTx(CDV_INT08U* txBuf, CDV_INT16U txLen, CDV_INT08U uartNo);
void AddTxPlus(CDV_INT08U* txBuf, CDV_INT16U txLen,  CMD_ARG *arg);
void ScriptRecvInit(CDV_INT32U addr , CDV_INT32U len);
void ScriptRecvDeinit(void);
void ScriptRecvCtl(CDV_INT32U addr , CDV_INT32U len);
void ScriptCrcChk(CDV_INT32U addr , CDV_INT32U len, CMD_ARG *arg);
void UsartTxQueueDoNext(void);
void UsartTxAddWithLen(CDV_INT16U len);
CDV_INT08U OnlineCmdCache(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CDV_INT08U uartNo, void *conn);
CDV_INT08U ClearOnlineCmdCache(void);
unsigned short crc_ccitt(unsigned char *q, int len, unsigned short crc);
//////extends

extern u8 Para_Buf[48];
#endif

