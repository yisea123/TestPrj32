
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


#include "stdio.h"
#include "stdint.h"

#endif

