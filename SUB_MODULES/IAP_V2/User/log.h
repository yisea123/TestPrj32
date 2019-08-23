
/**
  ******************************************************************************
  * @file    /log.h
  * @author  MMY
  * @version V1.0.0
  * @date    3-Mar-2016
  * @brief   about log
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */


#ifndef  _LOG_
#define  _LOG_

#include "cdv_include.h"

//#define LOG_INF_LEN     0x100
#define LOG_INF         LOG_INF_ADDR// + (TYPE)*LOG_INF_LEN//LOG��Ϣ�洢��ַ
#define LOG_INF_LEN     sizeof(Log_Inf)//�ṹ���С
#define LOG_ADDR        (LOG_INF + LOG_INF_LEN)                  //log�洢��ַ
#define LOG_MEM_LEN     ((LOG_LEN - LOG_INF_LEN)/( LOG_TYPE_N * LOG_MEM_N ))   //0x1000//�������С
#define LOG_MEM_N       0x10//������������ÿ������
#define LOG_MEM(TYPE,N) (LOG_ADDR + ((N) + (TYPE)*LOG_MEM_N)*LOG_MEM_LEN)//�����ַ��������,������
#define LOG_SEND_LEN    0x200//LOG���ͻ����С
//#define       0x3//LOG������������LOG_TYPEƥ��

typedef enum {
	
	LOG_ERROR = 0,
	LOG_WARNING,
	LOG_EVENT,
	LOG_TYPE_N,
}LOG_TYPE;


typedef struct{
	CDV_INT08U isInit;
	struct {
	  CDV_INT08U memNo;//��ǰ�ǵڼ���mem
	  CDV_INT16U point[LOG_MEM_N];//mem��ָ��
	}TYPE[LOG_TYPE_N];
}Log_Inf;


BOOL Log_Write(CDV_CHAR* buf, LOG_TYPE type);
//void Add_Warning(CDV_INT08U* buf, CDV_INT08U len);
//void Add_Error(CDV_INT08U* buf, CDV_INT08U len);
//void Add_Event(CDV_INT08U* buf, CDV_INT08U len);

RET_STATUS Log_Cmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
void Log_CmdWrite(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
#endif

