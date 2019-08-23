
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




#ifndef  _CDV_DEFINE_
#define  _CDV_DEFINE_

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
	CDV_SEND,
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
extern CDV_STATUS g_cdvStat/*,g_tmpCdvStat*/;
extern CDV_INT08U g_nowDoRes;
/*���ͽ��սű��ýṹ��*/
typedef struct
{
	CDV_INT08U no;
	CDV_INT32U len, tmpLen;
	CDV_INT32U addr;
	CDV_INT08U *buf;
}SCRIPT_INFO;
extern SCRIPT_INFO g_scriptInfo;


typedef struct 
{
	CDV_INT08U uart;//ռ�ô���
	CDV_INT08U len;//�����
	CDV_INT08U* buf;//����
	void* ptrWorker; //ָ���˽ṹ�壬�����߼��ȹ��˿���
	/*��Դ�ظ����*/
	CDV_INT08U reqlen;
	CDV_INT08U* reqbuf;
	/*���������¼���������ñ���*/
	CDV_INT08U speclen;
	CDV_INT08U* specbuf;
}CMD_ARG;
void CmdArgDelete(CMD_ARG *arg);
/*
 *CDV������ض���
 */
//#define JBJDZ 0
#define CDV_ID 1
#define APP_ID 0xCD
#define TOAPP_ID 0xAD
#define ENABLE_FPGA_DOWN 1
#define TCP_COM 0xEE
/*����CDV�汾*/
//#define CDV_V1 1
#define CDV_V2_2
#define NPC_V2_2	//2.2Ӳ��
#define VIRTUAL_LOGIC 1 //ʹ�����߼���Դ

#define _NPC_VERSION_ 3u//cdv �汾

#if _NPC_VERSION_ == 1u

//  #define ENABLE_PID 1u  // ������pid����
		#if defined(CDV_V1)
			#define MAIN_COM 1//MainUsart//
		#else
			#define MAIN_COM 2//MainUsart//
		#endif
#elif _NPC_VERSION_ == 2u

#define USE_NPC_NET  1
#define MAIN_COM 4
#elif _NPC_VERSION_ == 3u
#define USE_NPC_NET  1
#define MAIN_COM 4
#endif
//��Դʹ�ö���

/*���ڲ�����*/
#define USART_SEND_GAP  20
#define USART1BOUND 115200
/*�ڴ��ַ����*/

#define SCRIP_LINE_LEN        0x500000//�ű�����5M
//#define INILEN                0x10000       //INI���ȣ��64KB
#define LINE_LEN              0x8000//���˽ű�����32KB
#define LOG_LEN               0x100000//log����1M

#define SCRIP_MAX_RUN        20
//#define SCRIP_MAX_DEBUG      11
#define VERSION_ADDR         0x100
#define OTA_ADDR             VERSION_ADDR + 1
// OTA������IP��ַ
#define OTA_IP               OTA_ADDR + 10


#define WORKER_NUM           (SCRIP_MAX_RUN - 2)

#define SCRIP_LINE_ADDR      0x100000                 //�������߽ű��洢��ַ

/*��Դ�Ĳ�������*/
#define USART_ADDR            0x1EF00                  //�������ô洢��ַ
#define NET_ADDR              USART_ADDR - 0x100       //�������ô洢��ַ

#define LOG_INF_ADDR          0x600000                  //log��Ϣ�洢��ַ
#include "cdv_include.h"    /*���ļ�ͷ�ᵼ�¶��岻�����ڱ��h�ļ���*/

#define NEWCH(A,len) NewMemory((void **)&(A) , (len))
#define NEW08U(A,len) NewMemory((void **)&(A) , sizeof(CDV_INT08U)*(len))
#define NEW16U(A,len) NewMemory((void **)&(A) , sizeof(CDV_INT16U)*(len))//if((A)!=NULL) { free(A);(A)=NULL;} {(A) = (CDV_INT16U*)malloc(sizeof(CDV_INT16U)*(len));} if(NULL == (A)) {NewError();}
#define NEW32U(A,len) NewMemory((void **)&(A) , sizeof(CDV_INT32U)*(len))

#define DELETE(A) DelMemory((void **)&(A))
#define CLI() __set_PRIMASK(1) /*�ر������ж�NVIC_SystemReset();// ��λ*/
#define SEI() __set_PRIMASK(0)

#define INIT_CLEAR(ARG) memset(&ARG, 0, sizeof(ARG))
#define ONLY_ID_ADDR (u32*)(0x1FFF7A10)
/*
 *CDV ��Դ����
 */

#define  BIT_SET(val , bit)      ((val) |= (0x01 << (bit)))
#define  BIT_RESET(val , bit)    ((val) &= ((~0x0) ^(0x01 << (bit))))
#define  BIT_READ(val , bit)     (((val) & (0x01 <<(bit)))? 1 :0)


//�����ʱ��//��Ϊ�õ�����ļ����Է�include���棬�ܽᣬ�����壬��include�ϣ�ʹ�õ��ⲿ���壬��include����
extern CDV_INT32U tm1Re,tm2Re,tm3Re;
extern OS_TMR 	tmr1;		//��ʱ��1
extern OS_TMR	  tmr2;		//��ʱ��2
extern OS_TMR	  tmr3;		//��ʱ��3

#define NORMAL_TASK_PRIO    5
/*�������ȼ�*//*�����ջ��С*/
#define START_TASK_PRIO 3
#define START_STK_SIZE 128         //128-256
#define USART_RECV_TASK_PRIO 5
#define USART_RECV_STK_SIZE 128   //128-256
#define USART_SEND_TASK_PRIO 5
#define USART_SEND_STK_SIZE 128  //128-256
#define CDV_REFRESH_TASK_PRIO 5
#define CDV_REFRESH_STK_SIZE 128


/*������ƿ�*/
extern OS_TCB StartTaskTCB;
extern OS_TCB UsartRecvTaskTCB;
extern OS_TCB UsartSendTaskTCB;
extern OS_TCB CdvRefreshTaskTCB;
extern OS_TCB WorkerManageTaskTCB;
extern OS_TCB TaskParseTCB;

/*�ź���*/
extern OS_SEM TX_SEM , MOTO_SEM , MEM_SEM; //����һ���ź��������ڷ��ʹ�����Դ
extern OS_SEM GENERAL_SERIAL_SEM;
extern OS_SEM TCP_TX_SEM;

#endif

