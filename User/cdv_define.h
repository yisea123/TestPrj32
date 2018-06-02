
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
/*debug传递数据用结构体*/
//typedef struct
//{
//	CDV_INT32U val;
//}DEBUG_DATA;
//extern DEBUG_DATA g_debugData;


	#include "VarExp.h"

typedef struct 
{
	CDV_INT08U hostid;//主机号
	CDV_INT08U uart;//占用串口
	CDV_INT16U len;//命令长度
	CDV_INT08U* buf;//命令
	void* ptrWorker; //指向工人结构体，用于逻辑等工人控制
	/*资源回复相关*/
	CDV_INT16U reqlen;
	CDV_INT08U* reqbuf;
	/*特殊操作记录，例如设置变量*/
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
 *CDV外设相关定义
 */
//#define JBJDZ 0
#define CDV_ID 1
#define APP_ID 0xCD
#define TOAPP_ID 0xAD
#define KFC_IC 0xF1
#define YYC_IC 0xF2
#define ENABLE_FPGA_DOWN 0

#define MAIN_COM 4//MainUsart//
#define TCP_COM 0xEE
/*用作FPGA下载时的定义*/
#define FPGA_QUE_NUM 4
#define FPGA_QUE_LEN 4096
#define FPGA_LEN 464196 //XC6SLX16 
//#define FPGA_LEN 801462 //XC6SLX25 
/*定义CDV版本*/
//#define CDV_V1 1u
#define CDV_V2_2
#define VIRTUAL_LOGIC 1u //使用需逻辑资源

#define _NPC_VERSION_ 3u//cdv 版本1u 老版本，应该已经失效了；2u 2.2硬件版本；3u 2.3硬件版本


#if _NPC_VERSION_ == 3u
#undef ENABLE_FPGA_DOWN
#endif


#if _NPC_VERSION_ < 3u
  #define ENABLE_PID 1u
#endif

//资源使用定义
#define USE_NPC_NET  1u

#if USE_NPC_NET == 1u
#define USE_CENTRALIZEDCONTROL 1u
#endif

/*配置开关*/
#if _NPC_VERSION_ >= 3u
#define USE_PVD 1u
#define USE_LOWPOWER 1u
#define USE_MEMMNG 1u
#endif

#define USE_FLASH_BAK 0u
/*LED定义*/
//#if defined(CDV_V1)
//#define LED1 PDout(9)	  // LED1 运行指示
//#define LED2 PDout(11)	// LED2	告警指示
//#define LED3 PDout(13)	// LED3 故障指示
//#elif defined(CDV_V2)
//#define LED1 PHout(12)	// LED1 运行指示
//#define LED2 PBout(12)	// LED2	告警指示
//#define LED3 PHout(11)	// LED3 故障指示
//#endif

/*串口波特率*/
#define USART_SEND_GAP  20
#define USART1BOUND 115200
/*内存地址定义*/
//#define STARTSCRIP_NAME        0x051000//起始脚本名称存储地址
//#define STARTSCRIP_NAME_LEN    0x050010//起始脚本名称长度地址
//#define STARTSCRIPADDR       0x100000//起始脚本存储地址
//#define LOGICLEN              0x04000
//#define LOGICADDR(i)         0x300000 + LOGICLEN * (i)//脚本存储地址 64
//#define SCROP_LOGIC          0x059000				          //逻辑脚本
//#define REG_ADDR             0x010000				          //寄存器值存储地址
//#define INREG_ADDR			     0x030000				          //只读寄存器值存储地址
//#define COIL_ADDR			       0x040000				          //线圈值存储地址
//#define INCOIL_ADDR			     0x045000				          //只读线圈值存储地址
//#define WORKER_MAX_SCRIPT    0x10000

#define SCRIP_LINE_LEN        0x200000//脚本长度2M
//#define INILEN                0x10000       //INI长度，最长64KB
#define LINE_LEN              0x8000//工人脚本长度32KB
#define LOG_LEN               0x100000//log长度1M

#define SCRIP_MAX_RUN        16
//#define SCRIP_MAX_DEBUG      11
// 下载次数 1个字节
#define VERSION_ADDR         0x100 
// OTA升级地址，3个字节，如果是OTA即需要升级
#define OTA_ADDR             VERSION_ADDR + 1
#define WORKER_NUM           (SCRIP_MAX_RUN - 2)
//#define SCRIP_NUM_RUN        0x050000                 //脚本运行数量
//#define SCRIP_NUM_DEBUG      0x050005                   //脚本调试数量
//#define SCRIP_NAME_LEN(i)    0x050010 + 0x00001 * (i) //脚本名称长度地址
//#define SCRIP_LINE_NAME      0x050100                 //生产线名称
//#define SCRIP_NAME(i)        0x051000 + 0x00100 * (i) //脚本名称存储地址
//#define SCRIP_FORMULA        0x055000//保存当前使用的配方
//#define SCRIPADDR(i)         0x100000 + SCRIPLEN * (i)//脚本存储地址 8 + 24
#define SCRIP_LINE_ADDR      0x100000                 //新生产线脚本存储地址

//#define INI_LEN_ADDR(i)      0x300000 + INILEN * (i)       //INI flash长度存储地址
//#define INI_ADDR(i)          0x300000 + INILEN * (i) + 2   //INI flash存储地址

//#define SCRIP_FRE            0x630000                 //变频器脚本
//#define SCRIP_ALARM					 SCRIP_RES            		//报警信息脚本
#define FPGA_MOTOR           0x700000                 //FPGAmotor程序存储地址 长度340*1024
#define FPGA_IO              0x700000 + 0x80000//350*1024      //FPGAIO程序存储地址


/*资源的参数保存*/
#define RESOURCE_BASE		      0x500000                 //资源存储基地址，长度控制在1M以内
#define SCRIP_GONGGE		      RESOURCE_BASE            //宫格脚本
#define SCRIP_TRAY            SCRIP_GONGGE+100         //料盘脚本
#define MOTOPAR_ADDR		      SCRIP_TRAY+100				   //电机参数存储地址
#define MOTO_LINE_PAR_ADDR		MOTOPAR_ADDR+1000				 //直线插补参数存储地址
#define MOTO_ARC_PAR_ADDR		  MOTO_LINE_PAR_ADDR+1000	 //圆弧插补参数存储地址
#define IPAR_ADDR             0x21000                  //I参数存储地址//180410:原0x1FF00
#define VAL_STADDR            0x1F000                  //变量值存放起始地址//180410:0x2000为变量备份地址
#define USART_ADDR            0x1EF00                  //串口设置存储地址
#define NET_ADDR              USART_ADDR - 0x100       //网络设置存储地址

#define DAC_PAR_ADDR		      MOTO_ARC_PAR_ADDR+1000	 //DAC参数
#define ADC_PAR_ADDR		      DAC_PAR_ADDR+100	       //ADC参数
#define PID_PAR_ADDR		      ADC_PAR_ADDR+1000	       //PID参数
#define CHECK_FLASH_ADDR		  PID_PAR_ADDR+1000	       //FLASH校验参数
#define CHECK_VERION_ADDR		  CHECK_FLASH_ADDR+500	   //版本号设置
#define CHECK_VERION_ADDR1		CHECK_VERION_ADDR+100	
//#define PVD_ADDR              CHECK_VERION_ADDR1+100   //PVD保存地址，长度未知

#define LOG_INF_ADDR          0x600000                  //log信息存储地址
#include "cdv_include.h"    /*放文件头会导致定义不能用在别的h文件中*/

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
#define CLI() __set_PRIMASK(1) /*关闭所有中断NVIC_SystemReset();// 复位*/
#define SEI() __set_PRIMASK(0)

#define INIT_CLEAR(ARG) memset(&ARG, 0, sizeof(ARG))
	
#define ONLY_ID_ADDR (u32*)(0x1FFF7A10)
/*
 *CDV 资源定义
 */
/*I*/
//typedef struct
//{	
//	GPIO_TypeDef * port;//输出端口
//	CDV_INT16U pin;//输出端口号
//}CDV_IO;
/*CDV资源全局变量，在CDVInit中初始化*/
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
#define WORKER_MANAGE_TASK_PRIO 5
#define WORKER_MANAGE_STK_SIZE 128 //128-256//cmdparse嵌套比较深
#define WORKER_TASK_PRIO 5
#define WORKER_STK_SIZE 400         //128-256//cmdparse嵌套比较深
#define PARSE_TASK_PRIO 5
#define PARSE_STK_SIZE 256         //128-256//cmdparse嵌套比较深

#define CDV_VALVE_TASK_PRIO 5//比例调节阀
#define CDV_VALVE_STK_SIZE 128
#define CDV_MOTO_TASK_PRIO 5
#define CDV_MOTO_STK_SIZE 128

/*任务控制块*/
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
extern CPU_STK WORKER_TASK_STK[WORKER_NUM][WORKER_STK_SIZE];//任务堆栈
//extern CDV_INT08U g_whichUart;
/*信号量*/
extern OS_SEM TX_SEM, WORKER_SEM , MOTO_SEM , MEM_SEM , MSG_SEM; //定义一个信号量，用于访问共享资源, PROMPT_SEM  , CACHE_SEM , WIFI_SEM, VAL_SEM, SPI_SEM 
extern OS_SEM GENERAL_SERIAL_SEM;
//extern OS_SEM COM_SEM[6];
extern OS_SEM TCP_TX_SEM;


//#define WORKER_RUN_NUM     g_workerRunNum
//#define APP_SET_NUM        g_appSetNum
//extern CDV_WORD g_workerRunNum;
//extern CDV_WORD g_appSetNum;

//#define SRP_NUM_RUN       g_numRun                         /*运行脚本数量*/
//#define SRP_NUM_DEBUG     g_numDebug                       /*调试脚本数量*/
//#define DBG_NO_GET        g_getNo                           /*获取的脚本号*/
//extern CDV_WORD g_numRun;
//extern CDV_WORD g_numDebug;
//extern CDV_WORD g_getNo;



extern double RPressureData[100];
extern int RPressureCnt;




#endif

