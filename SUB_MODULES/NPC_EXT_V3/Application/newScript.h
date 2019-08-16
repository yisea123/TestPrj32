
/**
  ******************************************************************************
  * @file    /newScript.h 
  * @author  MMY
  * @version V0.0.1
  * @date    04-06-2016
  * @brief   internal implement and interface of new uart control of newScript
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _NEW_SCRIPT_
#define  _NEW_SCRIPT_

#include "cdv_include.h"

#define __LINE_LEN					 SCRIP_LINE_ADDR - 16
#define __LINE_ADDR          SCRIP_LINE_ADDR
#define LINE_MAX_LEN         SCRIP_LINE_LEN              //�����ߴ�С
#define SCRIP_NUM_RUN        g_line.workerAddr.len       //�ű���������
#define SCRIP_NAME_LEN(i)    GetConfigAddr(i) + 2        //�ű����Ƴ��ȵ�ַ
#define SCRIP_LINE_NAME      g_line.addr+4               //����������
#define SCRIP_YYC            g_line.endAddr+4            //Ӧ�ò�ű�����ʼ��ַ
#define SCRIP_NAME(i)        GetConfigAddr(i) + 3        //�ű����ƴ洢��ַ
#define SCRIP_LEN(i)         GetWorkerAddr(i)						 //�ű�����
#define SCRIPADDR(i)         GetWorkerAddr(i) + 2        //�ű���ַ
#define SCRIP_TYPE(i)        GetConfigAddr(i) - 1        //�ű�����
#define SCRIP_RES(i)         GetResAddr(i)               //�����Դ�ű�
#define SCRIP_ALARM					 g_line.AlarmAddr            //������Ϣ�ű�
#define SCRIP_MAP					   g_line.mapAddr            	 //ӳ����Ϣ�ű�
#define INI_LEN_ADDR(i)      GetResAddr0(0x0A, 0x00, 0xFF, 3)       //INI flash���ȴ洢��ַ
#define INI_ADDR(i)          GetResAddr0(0x0A, 0x00, 0xFF, 3) + 2   //INI flash�洢��ַ



typedef struct {
	CDV_INT08U init;                                       /*�Ƿ��Ѿ���ʼ��*/
	CDV_INT08U no;                                         /*����������*/
	CDV_INT32U addr;                                       /*��ǰ��������ʼ��ַ*/
	CDV_INT32U endAddr;                                    /*��ǰ�����߽�����ַ*/
	CDV_INT32U resAddr;                                    /*��ǰ��������Դ��ַ*/
	CDV_INT32U mapAddr;                                    /*��ǰ������ӳ���ַ*/
	CDV_INT32U AlarmAddr;                                  /*��ǰ�����߱�����ַ*/
	CDV_INT32U workAddr;
	ARRAY_U32  configAddr;                                 /*����ÿ���������õĵ�ַ*/
	ARRAY_U32  workerAddr;                                 /*����ÿ�����˵ĵ�ַ*/
} LINE_ADDR_INFO;//���������ߵ���Ϣflash�洢��ַ

extern LINE_ADDR_INFO g_line;

CDV_INT32U GetWorkerAddr(const CDV_INT08U workerNo);
CDV_INT32U GetConfigAddr(const CDV_INT08U workerNo);
CDV_INT32U GetResAddr(const CDV_INT08U resType);
CDV_INT32U GetResAddr0(const CDV_INT08U resType, const CDV_INT08U resNo, const CDV_INT08U resPar, const CDV_INT08U ctrl);
//CDV_INT32U GetResAddr(const CDV_INT08U resType, const CDV_INT08U resNo, const CDV_INT08U resPar, const CDV_INT08U ctrl);
RET_STATUS GetLineInfo(const CDV_INT08U no);


void Flash_Send(const CDV_INT32U addr , const CDV_INT32U len, CMD_ARG *arg);
RET_STATUS GetLineVersion(CDV_INT08U** buf, CDV_INT32U *len);
RET_STATUS GetYYCVersion(CDV_INT08U** buf, CDV_INT32U *len);
#endif


