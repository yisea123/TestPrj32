
/**
  ******************************************************************************
  * @file    /esp8266.h 
  * @author  MMY
  * @version V2.0.0
  * @date    1-September-2015
  * @brief   esp8266(wifi module) function and var and 
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */



#ifndef  _ESP_8266_
#define  _ESP_8266_
#include "cdv_include.h"

#define  BIT_SET(val , bit)      ((val) |= (0x01 << (bit)))
#define  BIT_RESET(val , bit)    ((val) &= (0xFF ^(0x01 << (bit))))
#define  BIT_READ(val , bit)     (((val) & (0x01 <<(bit)))? 1 :0)

#define  WIFI_LINK_NUM     5
#define  WIFI_LINK         g_wifiLink//INCOIL_CH (INCOIL_WIFI_LINK_CHSF)/*1���ֽڣ��ӵ͵��߷ֱ���id0-4�Ŀͻ�������״̬��ʶ�����һλ��*/
#define  WIFI_CONNECT(ID)  WIFI_LINK |= 0x01 << ((ID)>=WIFI_LINK_NUM ? 0 : (ID))//INCOIL_SET (INCOIL_WIFI_LINK_CHSF , (ID)>=WIFI_LINK_NUM ? 0 : (ID))/*����id0-4��bitΪ1����ʶ��id�ִ�������״̬*/
#define  WIFI_CLOSE(ID)    WIFI_LINK &= (0xFF ^(0x01 << ((ID)>=WIFI_LINK_NUM ? 0 : (ID))))/*����id0-4��bitΪ0����ʶ��id�ִ���δ����״̬*/
#define  SERVER_ON         BIT_SET(WIFI_LINK , 7)/*����id7��bitΪ1����ʾ����������*/
#define  SERVER_OFF        BIT_RESET(WIFI_LINK , 7)/*����id7��bitΪ1����ʾ�������ر�*/
#define  IS_SERVER_ON      ((WIFI_LINK) & (0x01 << (7)))


extern CDV_INT08U g_wifiLink;
/*
 *���ڽ�����ض���
 */

#define WIFI_RX_QUEUE_LENGTH 3     /*���ڽ��ն��г���*/
#define WIFI_RX_BUF_LENGTH 100    /*���ڽ��ճ���*/
#define WIFI_RX_QUEUE_SELF_ADD do{\
	if(0 == g_wifiRx.QUEUE.rxLen[g_wifiRx.rxPos])break;\
	(g_wifiRx.doPos!=MAX_VAL_ADD(g_wifiRx.rxPos,WIFI_RX_QUEUE_LENGTH))?MAX_SELF_ADD(g_wifiRx.rxPos,WIFI_RX_QUEUE_LENGTH):NULL;\
}while(0);
#define WIFI_RX_BUF_ADD_CHAR(ch) do{\
	g_wifiRx.QUEUE.rxBuf[g_wifiRx.rxPos][g_wifiRx.QUEUE.rxLen[g_wifiRx.rxPos]]=(CDV_INT08U)(ch);\
	g_wifiRx.QUEUE.rxLen[g_wifiRx.rxPos] = g_wifiRx.QUEUE.rxLen[g_wifiRx.rxPos]<WIFI_RX_BUF_LENGTH-1?1+g_wifiRx.QUEUE.rxLen[g_wifiRx.rxPos]:WIFI_RX_BUF_LENGTH-1;\
}while(0);
#define WIFI_RX_HAD  (g_wifiRx.rxPos!=g_wifiRx.doPos) /*&& 0 != g_wifiRx.QUEUE.rxLen[g_wifiRx.doPos]*/
#define WIFI_RX_QUEUE_DO_NEXT do{\
	g_wifiRx.QUEUE.rxLen[g_wifiRx.doPos]=0;\
	memset(g_wifiRx.QUEUE.rxBuf[g_wifiRx.doPos],0,WIFI_RX_BUF_LENGTH);\
	(g_wifiRx.rxPos!=g_wifiRx.doPos) ? MAX_SELF_ADD(g_wifiRx.doPos,WIFI_RX_QUEUE_LENGTH) : NULL;\
}while(0);
#define WIFI_RX_DO_ADDR g_wifiRx.QUEUE.rxBuf[g_wifiRx.doPos]
#define WIFI_RX_DO_LEN g_wifiRx.QUEUE.rxLen[g_wifiRx.doPos]
typedef struct{
	struct{
	  CDV_INT08U rxBuf[WIFI_RX_QUEUE_LENGTH][WIFI_RX_BUF_LENGTH];         /*���ն���*/
	  CDV_INT08U rxLen[WIFI_RX_QUEUE_LENGTH];              /*���ճ���*/	
	} QUEUE;

  CDV_INT08U rxPos;              /*�������λ��*/	
	CDV_INT08U doPos;              /*���鴦��λ��*/	
} WIFI_RX;

extern WIFI_RX g_wifiRx;

/*
 *���ڷ�����ض���
 */
#define WIFI_TX_QUEUE_LENGTH 3     /*���ڷ��Ͷ��г���*/
#define WIFI_TX_QUEUE_SELF_ADD do{\
	if(0 == g_wifiTx.QUEUE.txLen[g_wifiTx.txPos])break;\
	(g_wifiTx.doPos!=MAX_VAL_ADD(g_wifiTx.txPos,WIFI_TX_QUEUE_LENGTH))?MAX_SELF_ADD(g_wifiTx.txPos,WIFI_TX_QUEUE_LENGTH):NULL;\
}while(0);

#define WIFI_CAN_DO (g_wifiTx.doPos!=MAX_VAL_ADD(g_wifiTx.txPos,WIFI_TX_QUEUE_LENGTH))
//#define WIFI_TX_BUF_ADD_CHAR(ch) do{\
//	g_wifiTx.QUEUE.txBuf[g_wifiTx.txPos][g_wifiTx.QUEUE.txLen[g_wifiTx.txPos]]=(CDV_INT08U)(ch);\
//	g_wifiTx.QUEUE.txLen[g_wifiTx.txPos] = g_wifiTx.QUEUE.txLen[g_wifiTx.txPos]<WIFI_TX_BUF_LENGTH-1?1+g_wifiTx.QUEUE.txLen[g_wifiTx.txPos]:WIFI_TX_BUF_LENGTH-1;\
//}while(0);
#define WIFI_TX_HAD  (g_wifiTx.txPos!=g_wifiTx.doPos) /*&& 0 != g_wifiTx.QUEUE.txLen[g_wifiTx.doPos]*/
/*do��ǰbuf��������0���ж���һ�����Ƿ������ڽ��յģ��������+1�����򲻱�*/
#define WIFI_TX_QUEUE_DO_NEXT WifiTxQueueDoNext()
/*tx��ǰbuf���볤��A������A����ǰbuf����*/
#define WIFI_TX_ADD_WITH_LEN(A) WifiTxAddWithLen(A)
//#define WIFI_TX_QUEUE_DO_NEXT do{\
//	g_wifiTx.QUEUE.txLen[g_wifiTx.doPos]=0;\
//	if(g_wifiTx.QUEUE.txBuf[g_wifiTx.doPos]!= NULL){\
//		free(g_wifiTx.QUEUE.txBuf[g_wifiTx.doPos]);\
//		g_wifiTx.QUEUE.txBuf[g_wifiTx.doPos] = NULL;}\
//	(g_wifiTx.txPos!=g_wifiTx.doPos) ? MAX_SELF_ADD(g_wifiTx.doPos,WIFI_TX_QUEUE_LENGTH) : NULL;\
//}while(0);
///*tx��ǰbuf���볤��A������A����ǰbuf����*/
//#define WIFI_TX_ADD_WITH_LEN(A) do{\
//	if(g_wifiTx.QUEUE.txBuf[g_wifiTx.txPos]!= NULL){\
//		free(g_wifiTx.QUEUE.txBuf[g_wifiTx.txPos]);\
//		g_wifiTx.QUEUE.txBuf[g_wifiTx.txPos] = NULL;}\
//	g_wifiTx.QUEUE.txBuf[g_wifiTx.txPos] = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(A));\
//  g_wifiTx.QUEUE.txLen[g_wifiTx.txPos] = (A);\
//}while(0);
#define WIFI_TX_BUF(A) g_wifiTx.QUEUE.txBuf[g_wifiTx.txPos][(A)]
#define WIFI_TX_BUF_ADDR g_wifiTx.QUEUE.txBuf[g_wifiTx.txPos]
#define WIFI_TX_DO_ADDR g_wifiTx.QUEUE.txBuf[g_wifiTx.doPos]
#define WIFI_TX_DO_LEN g_wifiTx.QUEUE.txLen[g_wifiTx.doPos]


typedef struct{
	struct{
	  CDV_INT08U* txBuf[WIFI_TX_QUEUE_LENGTH];         /*���Ͷ���*/
	  CDV_INT08U txLen[WIFI_TX_QUEUE_LENGTH];              /*���ͳ���*/	
	} QUEUE;
  CDV_INT08U txPos;              /*���鷢��λ��*/	
	CDV_INT08U doPos;              /*���鴦��λ��*/	
} WIFI_TX;

extern WIFI_TX g_wifiTx;


/*
 *����ƥ����ض���
 */
#define IS_WIFI_NEED_REQUEST g_wifiCache.mark
typedef struct{

	CDV_INT08U* buf;         /*���Ͷ���*/
	CDV_INT08U len;          /*���鴦��λ��*/	
	CDV_INT08U mark;         /*�ظ����*/
} WIFI_CACHE;
extern WIFI_CACHE g_wifiCache;
extern CDV_INT08U g_wifiNeedRequest;    

RET_STATUS ATSend(CDV_INT08C* txBuf, CDV_INT08U txLen , const char* cmpBuf);
RET_STATUS ATCOMSendNoCrc(const char* txBuf, unsigned char txLen);
RET_STATUS ATCOMSend(const char* txBuf, unsigned char txLen);
RET_STATUS ATCOMSendReq(const char* txBuf, unsigned char txLen);
CDV_INT08U ATCache(CDV_INT08C* rxBuf, CDV_INT08U rxLen);
CDV_INT08U CacheCmp(CDV_INT08C* rxBuf);
RET_STATUS StartWifiServer(void);
RET_STATUS WifiParse(CDV_INT08C* txBuf, CDV_INT08U txLen);
void WifiTxQueueDoNext(void);
void WifiTxAddWithLen(CDV_INT08U len) ;
#endif


