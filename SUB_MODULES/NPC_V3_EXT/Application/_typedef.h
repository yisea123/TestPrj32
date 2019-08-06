
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




#ifndef  _TYPEDEF_
#define  _TYPEDEF_

typedef enum
{ 
  FALSE  = 0,
  TRUE,
}BOOL;


typedef enum {
	BUF_NONE = 0,
	BUF_NEW,
	
}BUF_OPT;

typedef enum  {
	RC_NONE= 0,
	RC_CRC,
	
}REQUEST_CTRL;

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
	OPT_SUCCESS= 0,
	OPT_FAILURE,
  
}RET_STATUS;

typedef struct 
{
	unsigned char hostid;//主机号
	unsigned char uart;//占用串口
	void *arg;     // 配合其他属性的附加内容
	unsigned short len;//命令长度
	unsigned char* buf;//命令
	void* ptrWorker; //指向工人结构体，用于逻辑等工人控制
	/*资源回复相关*/
	unsigned short reqlen;
	unsigned char* reqbuf;
	/*特殊操作记录，例如设置变量*/
	unsigned short speclen;
	unsigned char* specbuf;
	//ArithmeticStack stack;
}CMD_ARG;


/*
 * 联机命令缓存
 */
 #define HAVE_ONLINE_CMD g_olCache.mark
typedef struct{

	unsigned char* buf;         /*发送队列*/
	unsigned short len;          /*数组处理位置*/	
	unsigned char uart;         /*标记从哪号串口发来的*/
	unsigned char mark;         /*回复标记*/
	void *arg;               /*特殊标记*/
} OLCMD_CACHE;
extern OLCMD_CACHE g_olCache;
extern OLCMD_CACHE g_portCmdCache;

// define 的东西不需要提前声明，在使用前（c）声明即可

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

// bit operation

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

//////

#define PORT_A ((GPIO_TypeDef *) GPIOA_BASE)
#define PORT_B ((GPIO_TypeDef *) GPIOB_BASE)
#define PORT_C ((GPIO_TypeDef *) GPIOC_BASE)
#define PORT_D ((GPIO_TypeDef *) GPIOD_BASE)
#define PORT_E ((GPIO_TypeDef *) GPIOE_BASE)
#define PORT_F ((GPIO_TypeDef *) GPIOF_BASE)
#define PORT_G ((GPIO_TypeDef *) GPIOG_BASE)
#define PORT_H ((GPIO_TypeDef *) GPIOH_BASE)
#define PORT_I ((GPIO_TypeDef *) GPIOI_BASE)

#define PIN_0  ((uint16_t)0x0001)
#define PIN_1  ((uint16_t)0x0002)
#define PIN_2  ((uint16_t)0x0004)
#define PIN_3  ((uint16_t)0x0008)
#define PIN_4  ((uint16_t)0x0010)
#define PIN_5  ((uint16_t)0x0020)
#define PIN_6  ((uint16_t)0x0040)
#define PIN_7  ((uint16_t)0x0080)
#define PIN_8  ((uint16_t)0x0100)
#define PIN_9  ((uint16_t)0x0200)
#define PIN_10 ((uint16_t)0x0400)
#define PIN_11 ((uint16_t)0x0800)
#define PIN_12 ((uint16_t)0x1000)
#define PIN_13 ((uint16_t)0x2000)
#define PIN_14 ((uint16_t)0x4000)
#define PIN_15 ((uint16_t)0x8000)


/////////////////////////////////////////////////

#define HIGH16U(A) (((u16)(A) & 0xff00) >> 8)
#define LOW16U(A)  ((u16)(A) & 0x00ff)
#define ENDIAN_TF16U(A)    ((((u16)(A) & 0xff00) >> 8) | \
                              (((u16)(A) & 0x00ff) << 8))
#define ENDIAN_TF(A,N)   

#endif

