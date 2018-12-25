
/**
  ******************************************************************************
  * @file    /user_define.h 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   define struct union variable and so on
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CDV_TYPEDEF_
#define  _CDV_TYPEDEF_

/*
 *ȫ�����Ͷ���
 */
//typedef            void        USER_VOID;
typedef            char        CDV_CHAR;                        /*  8-bit character                                     */
typedef  unsigned  short       CDV_WORD;                        /* 16-bit word                              */
typedef  unsigned  char        CDV_BOOLEAN;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char        CDV_INT08U;                      /*  8-bit unsigned integer                              */
typedef    signed  char        CDV_INT08S;                      /*  8-bit   signed integer                              */
typedef     const  char        CDV_INT08C;                      /*  8-bit   signed integer                              */
typedef  unsigned  short       CDV_INT16U;                      /* 16-bit unsigned integer                              */
typedef    signed  short       CDV_INT16S;                      /* 16-bit   signed integer                              */
typedef  unsigned  int         CDV_INT32U;                      /* 32-bit unsigned integer                              */
typedef    signed  int         CDV_INT32S;                      /* 32-bit   signed integer                              */
typedef  unsigned  long  long  CDV_INT64U;                      /* 64-bit unsigned integer                              */
typedef    signed  long  long  CDV_INT64S;                      /* 64-bit   signed integer                              */

typedef            float       CDV_FP32;                        /* 32-bit floating point                                */
typedef            double      CDV_FP64;                        /* 64-bit floating point                                */

typedef  volatile  CDV_INT08U  CDV_REG08;                       /*  8-bit register      �������ʹ������ʱ�ᱻ�޸�                                */
typedef  volatile  CDV_INT16U  CDV_REG16;                       /* 16-bit register                                      */
typedef  volatile  CDV_INT32U  CDV_REG32;                       /* 32-bit register                                      */
typedef  volatile  CDV_INT64U  CDV_REG64;                       /* 64-bit register                                      */

typedef enum
{ 
  FALSE  = 0,
  TRUE,
}BOOL;

typedef enum
{ 
  CDV_ONLINE  = 0, //����ģʽ����������ģʽ
  CDV_SCRIPT_SEND  = 1,//���ű�״̬
	//CDV_SCRIPT_RECV,//�սű�״̬
	CDV_FPGA,//FPGA����ģʽ
	/*�ѻ�ģʽ*/
	CDV_START, //�ѻ�ģʽ������״̬
	CDV_STPAUSE,//�ѻ�ģʽ��������ͣ
	CDV_RUN,//�ѻ�ģʽ������״̬
	CDV_PAUSE, //�ѻ�ģʽ����ͣ
	CDV_STOP, //�ѻ�ģʽ��ֹͣ
	CDV_WAIT,//�ѻ�ģʽ���������õȴ�	
	/*����ģʽ*/
//	CDV_DEBUG,//����ģʽ
	CDV_GET_ALL_WORKER,//��ȡ���й����б�
	CDV_GET_WORKER_SCRIPT,//��ȡ���˽ű��б�
	CDV_RUN_WORKER_SCRIPT,//ִ�й��˽ű�
	CDV_RECV,//���սű�
	CDV_WIFI,//wifi����ģʽ
	CDV_FPGA_INIT,//fpga��ʼ��
	CDV_HARD_FAULT,//Ӳ������
	CDV_NEW_ERROR,//�ڴ��������
	CDV_DEBUG_JUMP,//�ѻ����ԡ���ת
	CDV_WHILE_ERROR,//��ѭ��
	CDV_LOG_SEND,//log������
	CDV_GET_VERSION,//�ѻ�����ȡ�汾��
	CDV_FORMULA,//�ѻ�����ȡ�䷽����
	CDV_ASSERT,
	CDV_NULL,//�������
}CDV_STATUS;

typedef enum
{ 
  WORKER_LEN_ERR  = 0, 
	WORKER_CRC_ERR,
	OPT_SUCCESS,
	OPT_FAILURE,
	OPT_TCP_ERROR,
  
}RET_STATUS;


#include "stdio.h"
#include "stdint.h"

#endif

