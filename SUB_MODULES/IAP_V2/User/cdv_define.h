
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
 *全局类型定义
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

typedef  volatile  CDV_INT08U  CDV_REG08;                       /*  8-bit register      这个变量使用中随时会被修改                                */
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
  CDV_ONLINE  = 0, //联机模式，开机启动模式
  CDV_SCRIPT_SEND  = 1,//发脚本状态
	//CDV_SCRIPT_RECV,//收脚本状态
	CDV_FPGA,//FPGA下载模式
	/*脱机模式*/
	CDV_START, //脱机模式，启动状态
	CDV_STPAUSE,//脱机模式，启动暂停
	CDV_RUN,//脱机模式，运行状态
	CDV_PAUSE, //脱机模式，暂停
	CDV_STOP, //脱机模式，停止
	CDV_WAIT,//脱机模式，启动设置等待	
	/*调试模式*/
//	CDV_DEBUG,//调试模式
	CDV_SEND,
	CDV_GET_ALL_WORKER,//获取所有工人列表
	CDV_GET_WORKER_SCRIPT,//获取工人脚本列表
	CDV_RUN_WORKER_SCRIPT,//执行工人脚本
	CDV_RECV,//接收脚本
	CDV_WIFI,//wifi接收模式
	CDV_FPGA_INIT,//fpga初始化
	CDV_HARD_FAULT,//硬件错误
	CDV_NEW_ERROR,//内存申请出错
	CDV_DEBUG_JUMP,//脱机调试。跳转
	CDV_WHILE_ERROR,//死循环
	CDV_LOG_SEND,//log发送中
	CDV_GET_VERSION,//脱机，获取版本号
	CDV_FORMULA,//脱机，获取配方变量
	CDV_ASSERT,
	CDV_NULL,//其他情况
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
/*发送接收脚本用结构体*/
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
	CDV_INT08U uart;//占用串口
	CDV_INT08U len;//命令长度
	CDV_INT08U* buf;//命令
	void* ptrWorker; //指向工人结构体，用于逻辑等工人控制
	/*资源回复相关*/
	CDV_INT08U reqlen;
	CDV_INT08U* reqbuf;
	/*特殊操作记录，例如设置变量*/
	CDV_INT08U speclen;
	CDV_INT08U* specbuf;
}CMD_ARG;
void CmdArgDelete(CMD_ARG *arg);
/*
 *CDV外设相关定义
 */
//#define JBJDZ 0
#define CDV_ID 1
#define APP_ID 0xCD
#define TOAPP_ID 0xAD
#define ENABLE_FPGA_DOWN 1
#define TCP_COM 0xEE
/*定义CDV版本*/
//#define CDV_V1 1
#define CDV_V2_2
#define NPC_V2_2	//2.2硬件
#define VIRTUAL_LOGIC 1 //使用需逻辑资源

#define _NPC_VERSION_ 3u//cdv 版本

#if _NPC_VERSION_ == 1u

//  #define ENABLE_PID 1u  // 比例阀pid调节
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
//资源使用定义

/*串口波特率*/
#define USART_SEND_GAP  20
#define USART1BOUND 115200
/*内存地址定义*/

#define SCRIP_LINE_LEN        0x500000//脚本长度5M
//#define INILEN                0x10000       //INI长度，最长64KB
#define LINE_LEN              0x8000//工人脚本长度32KB
#define LOG_LEN               0x100000//log长度1M

#define SCRIP_MAX_RUN        20
//#define SCRIP_MAX_DEBUG      11
#define VERSION_ADDR         0x100
#define OTA_ADDR             VERSION_ADDR + 1
// OTA升级的IP地址
#define OTA_IP               OTA_ADDR + 10


#define WORKER_NUM           (SCRIP_MAX_RUN - 2)

#define SCRIP_LINE_ADDR      0x100000                 //新生产线脚本存储地址

/*资源的参数保存*/
#define USART_ADDR            0x1EF00                  //串口设置存储地址
#define NET_ADDR              USART_ADDR - 0x100       //网络设置存储地址

#define LOG_INF_ADDR          0x600000                  //log信息存储地址
#include "cdv_include.h"    /*放文件头会导致定义不能用在别的h文件中*/

#define NEWCH(A,len) NewMemory((void **)&(A) , (len))
#define NEW08U(A,len) NewMemory((void **)&(A) , sizeof(CDV_INT08U)*(len))
#define NEW16U(A,len) NewMemory((void **)&(A) , sizeof(CDV_INT16U)*(len))//if((A)!=NULL) { free(A);(A)=NULL;} {(A) = (CDV_INT16U*)malloc(sizeof(CDV_INT16U)*(len));} if(NULL == (A)) {NewError();}
#define NEW32U(A,len) NewMemory((void **)&(A) , sizeof(CDV_INT32U)*(len))

#define DELETE(A) DelMemory((void **)&(A))
#define CLI() __set_PRIMASK(1) /*关闭所有中断NVIC_SystemReset();// 复位*/
#define SEI() __set_PRIMASK(0)

#define INIT_CLEAR(ARG) memset(&ARG, 0, sizeof(ARG))
#define ONLY_ID_ADDR (u32*)(0x1FFF7A10)
/*
 *CDV 资源定义
 */

#define  BIT_SET(val , bit)      ((val) |= (0x01 << (bit)))
#define  BIT_RESET(val , bit)    ((val) &= ((~0x0) ^(0x01 << (bit))))
#define  BIT_READ(val , bit)     (((val) & (0x01 <<(bit)))? 1 :0)


//软件定时器//因为用到别的文件所以放include下面，总结，自身定义，放include上，使用到外部定义，放include下面
extern CDV_INT32U tm1Re,tm2Re,tm3Re;
extern OS_TMR 	tmr1;		//定时器1
extern OS_TMR	  tmr2;		//定时器2
extern OS_TMR	  tmr3;		//定时器3

#define NORMAL_TASK_PRIO    5
/*任务优先级*//*任务堆栈大小*/
#define START_TASK_PRIO 3
#define START_STK_SIZE 128         //128-256
#define USART_RECV_TASK_PRIO 5
#define USART_RECV_STK_SIZE 128   //128-256
#define USART_SEND_TASK_PRIO 5
#define USART_SEND_STK_SIZE 128  //128-256
#define CDV_REFRESH_TASK_PRIO 5
#define CDV_REFRESH_STK_SIZE 128


/*任务控制块*/
extern OS_TCB StartTaskTCB;
extern OS_TCB UsartRecvTaskTCB;
extern OS_TCB UsartSendTaskTCB;
extern OS_TCB CdvRefreshTaskTCB;
extern OS_TCB WorkerManageTaskTCB;
extern OS_TCB TaskParseTCB;

/*信号量*/
extern OS_SEM TX_SEM , MOTO_SEM , MEM_SEM; //定义一个信号量，用于访问共享资源
extern OS_SEM GENERAL_SERIAL_SEM;
extern OS_SEM TCP_TX_SEM;

#endif

