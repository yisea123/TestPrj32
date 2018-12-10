
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
/*debug���������ýṹ��*/
//typedef struct
//{
//	CDV_INT32U val;
//}DEBUG_DATA;
//extern DEBUG_DATA g_debugData;


	#include "VarExp.h"

typedef struct 
{
	CDV_INT08U hostid;//������
	CDV_INT08U uart;//ռ�ô���
	void *arg;     // ����������Եĸ�������
	CDV_INT16U len;//�����
	CDV_INT08U* buf;//����
	void* ptrWorker; //ָ���˽ṹ�壬�����߼��ȹ��˿���
	/*��Դ�ظ����*/
	CDV_INT16U reqlen;
	CDV_INT08U* reqbuf;
	/*���������¼���������ñ���*/
	CDV_INT16U speclen;
	CDV_INT08U* specbuf;
	ArithmeticStack stack;
}CMD_ARG;
#define CMD_ARG_CLEAR(parg) do {\
	(parg)->ptrWorker = NULL;\
}while(0)
void CmdArgInit(CMD_ARG *arg);
void CmdArgDelete(CMD_ARG *arg);


int ArithmeticEx(const char* inexp, const short expLen, CMD_ARG *arg);
/*
 *CDV������ض���
 */
//#define JBJDZ 0
#define CDV_ID 1
#define APP_ID 0xCD
#define TOAPP_ID 0xAD
#define KFC_IC 0xF1
#define YYC_IC 0xF2
#define ENABLE_FPGA_DOWN 0 // FPGA��չ

#define TCP_COM 0xEE
/*����FPGA����ʱ�Ķ���*/
#define FPGA_QUE_NUM 4
#define FPGA_QUE_LEN 4096
#define FPGA_LEN 464196 //XC6SLX16 
//#define FPGA_LEN 801462 //XC6SLX25 
/*����CDV�汾*/
//#define CDV_V1 1u // v1 ��һ��
#define CDV_V2_2//��Ե�ʱV1 ������
#define NPC_V2_2	//���v2 ��2.2Ӳ��
#define VIRTUAL_LOGIC 1u //ʹ�����߼���Դ
/*

2u�汾�޸�
stm32f4x7_eth_conf.h
	#define ETH_RXBUFNB        4
	#define ETH_TXBUFNB        2
lwipopts.h
	#define MEM_SIZE                4800
	#define MEMP_NUM_TCP_SEG        50
	#define PBUF_POOL_SIZE          7
	#define PBUF_POOL_BUFSIZE       512
	#define TCP_SND_BUF             (4*TCP_MSS)
	#define TCP_SND_QUEUELEN        (2* TCP_SND_BUF/TCP_MSS)
	#define TCP_WND                 (2*TCP_MSS)
startup_stm32f40_41xxx.s
	Heap_Size       EQU     0x0000C000

3u�汾�޸�
stm32f4x7_eth_conf.h
 #define ETH_RXBUFNB        10
 #define ETH_TXBUFNB        10
lwipopts.h
	#define MEM_SIZE                10000
	#define MEMP_NUM_TCP_SEG        110
	#define PBUF_POOL_SIZE          20
	#define PBUF_POOL_BUFSIZE       1520
	#define TCP_SND_BUF             (10*TCP_MSS)
	#define TCP_SND_QUEUELEN        (10* TCP_SND_BUF/TCP_MSS)
	#define TCP_WND                 (6*TCP_MSS)
startup_stm32f40_41xxx.s
	Heap_Size       EQU     0x00000200
*/
#define _NPC_VERSION_ 1u //cdv �汾1u �ϰ汾��Ӧ���Ѿ�ʧЧ�ˣ�2u 2.2Ӳ���汾��3u 2.3Ӳ���汾


#if _NPC_VERSION_ == 1u
		#if defined(CDV_V1)
			#define MAIN_COM 1//MainUsart//
		#else
			#define MAIN_COM 2//MainUsart//
		#endif
		
		#define USE_CASCADE  1u
//  #define ENABLE_PID 1u  // ������pid����
#elif _NPC_VERSION_ == 2u

  #define MAIN_COM 4//MainUsart//
	
	#define USE_NPC_NET  1u

  #define USE_CASCADE  1u
	
	#if USE_CASCADE == 1u
	
		#define USE_CASCADE_MAP 0u
		
	#endif

#elif _NPC_VERSION_ == 3u
  
  #define MAIN_COM 4//MainUsart//

	#undef ENABLE_FPGA_DOWN
	
	#define USE_PVD 0u // PVD ���籣��

	#if USE_PVD == 0u
	
		#define USE_EXTI_POWER_OFF 1u // �����жϱ���
		
		#if USE_EXTI_POWER_OFF == 1u
		
			#define USE_FLASH_BAK 1u // flash ����
			
		#endif
		
	#endif

	#define USE_LOWPOWER 0u // �͹���
	
	#define USE_MEMMNG 1u  // �ⲿsram����
	
	#define USE_COUNTER 1u //ʹ�ü����� 
	
	#define USE_NPC_NET  1u
	
  #define USE_CASCADE  1u
	
	#if USE_CASCADE == 1u
	
		#define USE_CASCADE_MAP 1u
		
	#endif

#endif


////��Դʹ�ö���
//#define USE_NPC_NET  1u

#if USE_NPC_NET == 1u

	#define USE_CENTRALIZEDCONTROL 1u
	
#endif

///*���ÿ���*/
//#if _NPC_VERSION_ >= 3u

//	#undef ENABLE_FPGA_DOWN
//	
//	#define USE_PVD 0u // PVD ���籣��

//	#if USE_PVD == 0u
//	
//		#define USE_EXTI_POWER_OFF 1u // �����жϱ���
//		
//		#if USE_EXTI_POWER_OFF == 1u
//		
//			#define USE_FLASH_BAK 1u // flash ����
//			
//		#endif
//		
//	#endif

//	#define USE_LOWPOWER 0u // �͹���
//	
//	#define USE_MEMMNG 1u  // �ⲿsram����
//	
//	#define USE_COUNTER 1u //ʹ�ü����� 
//	
//#endif

#define USE_OVERLAP 1u // ʹ���첽
/*LED����*/
//#if defined(CDV_V1)
//#define LED1 PDout(9)	  // LED1 ����ָʾ
//#define LED2 PDout(11)	// LED2	�澯ָʾ
//#define LED3 PDout(13)	// LED3 ����ָʾ
//#elif defined(CDV_V2)
//#define LED1 PHout(12)	// LED1 ����ָʾ
//#define LED2 PBout(12)	// LED2	�澯ָʾ
//#define LED3 PHout(11)	// LED3 ����ָʾ
//#endif

/*���ڲ�����*/
#define USART_SEND_GAP  20
#define USART1BOUND 115200
/*�ڴ��ַ����*/
//#define STARTSCRIP_NAME        0x051000//��ʼ�ű����ƴ洢��ַ
//#define STARTSCRIP_NAME_LEN    0x050010//��ʼ�ű����Ƴ��ȵ�ַ
//#define STARTSCRIPADDR       0x100000//��ʼ�ű��洢��ַ
//#define LOGICLEN              0x04000
//#define LOGICADDR(i)         0x300000 + LOGICLEN * (i)//�ű��洢��ַ 64
//#define SCROP_LOGIC          0x059000				          //�߼��ű�
//#define REG_ADDR             0x010000				          //�Ĵ���ֵ�洢��ַ
//#define INREG_ADDR			     0x030000				          //ֻ���Ĵ���ֵ�洢��ַ
//#define COIL_ADDR			       0x040000				          //��Ȧֵ�洢��ַ
//#define INCOIL_ADDR			     0x045000				          //ֻ����Ȧֵ�洢��ַ
//#define WORKER_MAX_SCRIPT    0x10000

#define SCRIP_LINE_LEN        0x200000//�ű�����2M
//#define INILEN                0x10000       //INI���ȣ��64KB
#define LINE_LEN              0x8000//���˽ű�����32KB
#define LOG_LEN               0x100000//log����1M

#define SCRIP_MAX_RUN        16
//#define SCRIP_MAX_DEBUG      11
// ���ش��� 1���ֽ�
#define VERSION_ADDR         0x100 
// OTA������ַ��3���ֽڣ������OTA����Ҫ����
#define OTA_ADDR             VERSION_ADDR + 1
#define WORKER_NUM           SCRIP_MAX_RUN
//#define SCRIP_NUM_RUN        0x050000                 //�ű���������
//#define SCRIP_NUM_DEBUG      0x050005                   //�ű���������
//#define SCRIP_NAME_LEN(i)    0x050010 + 0x00001 * (i) //�ű����Ƴ��ȵ�ַ
//#define SCRIP_LINE_NAME      0x050100                 //����������
//#define SCRIP_NAME(i)        0x051000 + 0x00100 * (i) //�ű����ƴ洢��ַ
//#define SCRIP_FORMULA        0x055000//���浱ǰʹ�õ��䷽
//#define SCRIPADDR(i)         0x100000 + SCRIPLEN * (i)//�ű��洢��ַ 8 + 24
#define SCRIP_LINE_ADDR      0x100000                 //�������߽ű��洢��ַ

//#define INI_LEN_ADDR(i)      0x300000 + INILEN * (i)       //INI flash���ȴ洢��ַ
//#define INI_ADDR(i)          0x300000 + INILEN * (i) + 2   //INI flash�洢��ַ

//#define SCRIP_FRE            0x630000                 //��Ƶ���ű�
//#define SCRIP_ALARM					 SCRIP_RES            		//������Ϣ�ű�
#define FPGA_MOTOR           0x700000                 //FPGAmotor����洢��ַ ����340*1024
#define FPGA_IO              0x700000 + 0x80000//350*1024      //FPGAIO����洢��ַ


/*��Դ�Ĳ�������*/
#define RESOURCE_BASE		      0x500000                 //��Դ�洢����ַ�����ȿ�����1M����
#define SCRIP_GONGGE		      RESOURCE_BASE            //����ű�
#define SCRIP_TRAY            SCRIP_GONGGE+100         //���̽ű�
#define MOTOPAR_ADDR		      SCRIP_TRAY+100				   //��������洢��ַ
#define MOTO_LINE_PAR_ADDR		MOTOPAR_ADDR+1000				 //ֱ�߲岹�����洢��ַ
#define MOTO_ARC_PAR_ADDR		  MOTO_LINE_PAR_ADDR+1000	 //Բ���岹�����洢��ַ
#define IPAR_ADDR             0x21000                  //I�����洢��ַ//180410:ԭ0x1FF00
#define VAL_STADDR            0x1F000                  //����ֵ�����ʼ��ַ//180410:0x2000Ϊ�������ݵ�ַ
#define USART_ADDR            0x1EF00                  //�������ô洢��ַ
#define NET_ADDR              USART_ADDR - 0x100       //�������ô洢��ַ

#define DAC_PAR_ADDR		      MOTO_ARC_PAR_ADDR+1000	 //DAC����
#define ADC_PAR_ADDR		      DAC_PAR_ADDR+100	       //ADC����
#define PID_PAR_ADDR		      ADC_PAR_ADDR+1000	       //PID����
#define CHECK_FLASH_ADDR		  PID_PAR_ADDR+1000	       //FLASHУ�����
#define CHECK_VERION_ADDR		  CHECK_FLASH_ADDR+500	   //�汾������
#define CHECK_VERION_ADDR1		CHECK_VERION_ADDR+100	
//#define PVD_ADDR              CHECK_VERION_ADDR1+100   //PVD�����ַ������δ֪

#define LOG_INF_ADDR          0x600000                  //log��Ϣ�洢��ַ
#include "cdv_include.h"    /*���ļ�ͷ�ᵼ�¶��岻�����ڱ��h�ļ���*/

//#define NEW08U(A,len) if((A)!=NULL) { free(A);(A)=NULL;} {(A) = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(len));} if(NULL == (A)) {NewError();}
#define NEWCH(A,len) NewMemory((void **)&(A) , (len))
#define RENEWCH(A,len) ReNewMemory((void **)&(A) , (len))
#define NEW08U(A,len) NewMemory((void **)&(A) , sizeof(CDV_INT08U)*(len))
#define NEW16U(A,len) NewMemory((void **)&(A) , sizeof(CDV_INT16U)*(len))//if((A)!=NULL) { free(A);(A)=NULL;} {(A) = (CDV_INT16U*)malloc(sizeof(CDV_INT16U)*(len));} if(NULL == (A)) {NewError();}
#define NEW32U(A,len) NewMemory((void **)&(A) , sizeof(CDV_INT32U)*(len))
//#define NEW32U(A,len) if((A)!=NULL) { free(A);(A)=NULL;} {(A) = (CDV_INT32U*)malloc(sizeof(CDV_INT32U)*(len));} if(NULL == (A)) {NewError();}
//#define NEW08UP(A,len) if((A)!=NULL) { free(A);(A)=NULL;} {(A) = (CDV_INT08U**)malloc(sizeof(CDV_INT08U*)*(len));} if(NULL == (A)) {NewError();}
//#define NEW16UP(A,len) if((A)!=NULL) { free(A);(A)=NULL;} {(A) = (CDV_INT16U**)malloc(sizeof(CDV_INT16U*)*(len));} if(NULL == (A)) {NewError();}
//#define NEW32UP(A,len) if((A)!=NULL) { free(A);(A)=NULL;} {(A) = (CDV_INT32U**)malloc(sizeof(CDV_INT32U*)*(len));} if(NULL == (A)) {NewError();}
//#define DELETE(A) if((A)!=NULL) { free(A);(A)=NULL;}
#define DELETE(A) DelMemory((void **)&(A))
#define CLI() __set_PRIMASK(1) /*�ر������ж�NVIC_SystemReset();// ��λ*/
#define SEI() __set_PRIMASK(0)

#define INIT_CLEAR(ARG) memset(&ARG, 0, sizeof(ARG))
	
#define ONLY_ID_ADDR (u32*)(0x1FFF7A10)
/*
 *CDV ��Դ����
 */
/*I*/
//typedef struct
//{	
//	GPIO_TypeDef * port;//����˿�
//	CDV_INT16U pin;//����˿ں�
//}CDV_IO;
/*CDV��Դȫ�ֱ�������CDVInit�г�ʼ��*/
//#if defined(CDV_V1)
//extern CDV_IO g_cdvI[16];
//extern CDV_IO g_cdvO[16];
//#elif defined(CDV_V2)
//extern CDV_IO g_cdvI[20];
//extern CDV_IO g_cdvO[20];
//#endif

//extern CDV_IO g_dipSW[6];

#define  BIT_SET(val , bit)      ((val) |= (0x01 << (bit)))
#define  BIT_RESET(val , bit)    ((val) &= ((~0x0) ^(0x01 << (bit))))
#define  BIT_READ(val , bit)     (((val) & (0x01 <<(bit)))? 1 :0)

#define SET_COIL(P, N) do{\
	*((u8*)P + ((N) >> 3)) |= (0x01 <<((N) & 0x07));\
}while(0);
#define RESET_COIL(P, N) do{\
	*((u8*)P + ((N) >> 3)) &= (0xFF ^(0x01 <<((N) & 0x07)));\
}while(0);
#define READ_COIL(P, N) ((*((u8*)P + ((N) >> 3)) & (0x01 <<((N) & 0x07)))? 1 :0)


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
#define WORKER_MANAGE_TASK_PRIO 5
#define WORKER_MANAGE_STK_SIZE 128 //128-256//cmdparseǶ�ױȽ���
#define WORKER_TASK_PRIO 5
#define WORKER_STK_SIZE 400         //128-256//cmdparseǶ�ױȽ���
#define PARSE_TASK_PRIO 5
#define PARSE_STK_SIZE 256         //128-256//cmdparseǶ�ױȽ���

#define CDV_VALVE_TASK_PRIO 5//�������ڷ�
#define CDV_VALVE_STK_SIZE 128
#define CDV_MOTO_TASK_PRIO 5
#define CDV_MOTO_STK_SIZE 128

/*������ƿ�*/
extern OS_TCB StartTaskTCB;
extern OS_TCB UsartRecvTaskTCB;
extern OS_TCB UsartSendTaskTCB;
extern OS_TCB CdvRefreshTaskTCB;
extern OS_TCB WorkerManageTaskTCB;
extern OS_TCB WorkerTaskTCB[WORKER_NUM];
extern OS_TCB TaskParseTCB;

extern OS_TCB CdvValveTaskTCB;
extern OS_TCB CdvMotoTaskTCB;

extern const char* workerName[];
//extern CDV_INT08U workerId[];
extern CPU_STK WORKER_TASK_STK[WORKER_NUM][WORKER_STK_SIZE];//�����ջ
//extern CDV_INT08U g_whichUart;
/*�ź���*/
extern OS_SEM TX_SEM, WORKER_SEM , MOTO_SEM , MEM_SEM , MSG_SEM , LIST_SEM; //����һ���ź��������ڷ��ʹ�����Դ, PROMPT_SEM  , CACHE_SEM , WIFI_SEM, VAL_SEM, SPI_SEM 
extern OS_SEM COIL_SEM;
extern OS_SEM COM_SEM[6];
extern OS_SEM TCP_TX_SEM;


//#define WORKER_RUN_NUM     g_workerRunNum
//#define APP_SET_NUM        g_appSetNum
//extern CDV_WORD g_workerRunNum;
//extern CDV_WORD g_appSetNum;

//#define SRP_NUM_RUN       g_numRun                         /*���нű�����*/
//#define SRP_NUM_DEBUG     g_numDebug                       /*���Խű�����*/
//#define DBG_NO_GET        g_getNo                           /*��ȡ�Ľű���*/
//extern CDV_WORD g_numRun;
//extern CDV_WORD g_numDebug;
//extern CDV_WORD g_getNo;



extern double RPressureData[100];
extern int RPressureCnt;




#endif

