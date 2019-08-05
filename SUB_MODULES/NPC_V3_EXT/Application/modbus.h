/**
  ******************************************************************************
  * @file    /modbus.h 
  * @author  MMY
  * @version V2.0.0
  * @date    4-July-2015
  * @brief   modbus function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _MODBUS_
#define  _MODBUS_

#include "_opt.h"


#define MODBUS_ADDR 0x10
/*
 *MODBUS定制定义
 *操作modbus特定资源的寄存器和线圈，非与外设资源打交道时可用
 */
 
 //modbus各缓存的数组大小
#define COIL_CHN		32
#define INCOIL_CHN	32
// 注意修改 var.h中的变量数不能超过HALF_REG_N,为扩展DA预留
// HALF_REG_N 为 REG_N 一半
#if _NPC_VERSION_ == 1u || _NPC_VERSION_ == 2u
#define REG_N				800
#define HALF_REG_N	400
#elif _NPC_VERSION_ == 3u

#ifdef  DEBUG_TIME
#define REG_N				1600//1000+500/*用于记录*///640
#define HALF_REG_N	800//500
#else
#define REG_N				1000/*用于记录*///640
#define HALF_REG_N	500
#endif
#endif
#define INREG_N			32
/*
 *MODBUS数据结构和操作定义
 */ 
//#define LITTLE_ENDIAN
///*用于通讯*/
//#if defined(BIG_ENDIAN) && !defined(LITTLE_ENDIAN)

//#define TOBIG16U(A)   (A)
//#define TOBIG32U(A)    (A)

//#elif defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)

//#define TOBIG16U(A)    ((((u16)(A) & 0xff00) >> 8) | \
//                              (((u16)(A) & 0x00ff) << 8))
//#define TOBIG32U(A)    ((((u32)(A) & 0xff000000) >> 24) | \
//                             (((u32)(A) & 0x00ff0000) >> 8) | \
//                             (((u32)(A) & 0x0000ff00) << 8) | \
//                             (((u32)(A) & 0x000000ff) << 24))

//#else

//#error "Either BIG_ENDIAN or LITTLE_ENDIAN must be #defined, but not both."

//#endif
/*以上没有存在意义*/
#define HIGH16U(A) (((u16)(A) & 0xff00) >> 8)
#define LOW16U(A)  ((u16)(A) & 0x00ff)
#define ENDIAN_TF16U(A)    ((((u16)(A) & 0xff00) >> 8) | \
                              (((u16)(A) & 0x00ff) << 8))
#define ENDIAN_TF(A,N)   
												




#include "_include.h"
//#pragma pack(1)
/*modbus寄存器结构体*/
typedef union
{
  struct 
  {
		struct
		{
			u16 valL;                               /*!< word:     80~9f  变量                           */
			u16 valH;
		} var[HALF_REG_N];
  } TYPE;                                          /*!< Structure used for resource  access             */
  u16 reg[REG_N];                           /*!< Type      used for word access                  */
} MODBUS_Register;

extern MODBUS_Register g_modbusReg;

#define MODBUS_REG_ADDR(A) g_modbusReg.reg[(A)]
/*modbus输入寄存器结构体*/
typedef union
{                                  /*!< Structure used for bit  access                  */
	struct
	{
		u16 valL;                               /*!< word:     80~9f  变量                           */
		u16 valH;
	} var[10];
  u16 reg[INREG_N];                          /*!< Type      used for word access                  */
} MODBUS_Input_Register;

extern MODBUS_Input_Register g_modbusInReg;

#define MODBUS_INREG_ADDR(A) g_modbusInReg.reg[(A)]


/*modbus线圈结构体*/
typedef union
{

  struct
  {
		struct
		{
			u8 O0:1;                        
			u8 O1:1;                       
			u8 O2:1;                       
			u8 O3:1;                       
			u8 O4:1;                       
			u8 O5:1;                       
			u8 O6:1;                       
			u8 O7:1;                       
			u8 O8:1;                      
			u8 O9:1;
			u8 O10:1;
			u8 O11:1;
			u8 O12:1;
			u8 O13:1;
			u8 O14:1;
			u8 O15:1;
			
			u8 O16:1;                        
			u8 O17:1;                       
			u8 O18:1;                       
			u8 O19:1;                       
			u8 O20:1;                       
			u8 O21:1;                       
			u8 O22:1;                       
			u8 O23:1;                       
			u8 O24:1;                      
			u8 O25:1;
			u8 O26:1;
			u8 O27:1;
			u8 O28:1;
			u8 O29:1;
			u8 O30:1;
			u8 O31:1;
			
		} O;                                     /*!< bit:     0~9f  speed          */
		
  } TYPE;                                  /*!< Structure used for bit  access                  */
  u8 coilCh[COIL_CHN];                     /*!< Type      used for word access                  */
} MODBUS_Coil;

extern MODBUS_Coil g_modbusCoil;

#define MODBUS_COIL_CH(A) g_modbusCoil.coilCh[(A)]
#if 1
#define SET_COIL_ADDR(A) do{\
	__disable_irq();\
	(MODBUS_COIL_CH((A) >> 3)) |= (0x01 <<((A) & 0x07));\
	__enable_irq();\
}while(0);

#define RESET_COIL_ADDR(A) do{\
	__disable_irq();\
	(MODBUS_COIL_CH((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
	__enable_irq();\
}while(0);
//#define SET_COIL_ADDR(A) do{\
//	OS_ERR err;\
//	OSMutexPend(&COIL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err);\
//	(MODBUS_COIL_CH((A) >> 3)) |= (0x01 <<((A) & 0x07));\
//	OSMutexPost (&COIL_SEM,OS_OPT_POST_NO_SCHED,&err);\
//}while(0);

//#define RESET_COIL_ADDR(A) do{\
//	OS_ERR err;\
//	OSMutexPend(&COIL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err);\
//	(MODBUS_COIL_CH((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
//	OSMutexPost (&COIL_SEM,OS_OPT_POST_NO_SCHED,&err);\
//}while(0);
#define READ_COIL_ADDR(A) (((MODBUS_COIL_CH((A) >> 3)) & (0x01 <<((A) & 0x07))) ? 1 : 0)
#else
#define SET_COIL_ADDR(A) do{\
	(MODBUS_COIL_CH((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);

#define RESET_COIL_ADDR(A) do{\
	(MODBUS_COIL_CH((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);
#define READ_COIL_ADDR(A) (((MODBUS_COIL_CH((A) >> 3)) & (0x01 <<((A) & 0x07))) ? 1 : 0)

#endif

/*modbus输入线圈结构体*/
typedef union
{
  struct
  {
		struct
		{
			u8 I0:1;                        
			u8 I1:1;                       
			u8 I2:1;                       
			u8 I3:1;                       
			u8 I4:1;                       
			u8 I5:1;                       
			u8 I6:1;                       
			u8 I7:1;                       
			u8 I8:1;                      
			u8 I9:1;
			u8 I10:1;
			u8 I11:1;
			u8 I12:1;
			u8 I13:1;
			u8 I14:1;
			u8 I15:1;
			
			u8 I16:1;                        
			u8 I17:1;                       
			u8 I18:1;                       
			u8 I19:1;                       
			u8 I20:1;                       
			u8 I21:1;                       
			u8 I22:1;                       
			u8 I23:1;                       
			u8 I24:1;                      
			u8 I25:1;
			u8 I26:1;
			u8 I27:1;
			u8 I28:1;
			u8 I29:1;
			u8 I30:1;
			u8 I31:1;
			        
		} I;                                    /*!< bit:     0 ~ 9f  speed          */
  } TYPE;                                   /*!< Structure used for bit  access                  */
  u8 coilCh[INCOIL_CHN];                            /*!< Type      used for word access                  */
} MODBUS_Input_Coil;

extern MODBUS_Input_Coil g_modbusInCoil;

#define MODBUS_INCOIL_CH(A) g_modbusInCoil.coilCh[(A)]
#define SET_INCOIL_ADDR(A) do{\
	(MODBUS_INCOIL_CH((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);

#define RESET_INCOIL_ADDR(A) do{\
	(MODBUS_INCOIL_CH((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);
#define READ_INCOIL_ADDR(A) (((MODBUS_INCOIL_CH((A) >> 3)) & (0x01 <<((A) & 0x07))) ? 1 : 0)




//u16 getCRC16(u8 *ptr,u16 len) ;
u8 ModbusParse(u8* rxBuf, u8 rxLen, CMD_ARG *arg/*, u8 uartNo*/);
//void RequestAdd(u8* rxBuf,u8 txLen);

 /************************************************************
	********************串口发送函数*****************************
	************************************************************/
void WriteRegisterCmd(u8 dev, u16 addr, u16 val, 
     u8** cmdBuf,u8* cmdLen,BUF_OPT flag);
void WriteMultiRegisterCmd(u8 dev, u16 addr, u16 num, 
     u8* regVal, u8** cmdBuf,u8* cmdLen,BUF_OPT flag);
void ReadRegisterCmd(u8 dev, u16 addr, u16 num, 
     u8** cmdBuf,u8* cmdLen,BUF_OPT flag);
void ReadInRegisterCmd(u8 dev, u16 addr, u16 num, 
     u8** cmdBuf,u8* cmdLen,BUF_OPT flag);
void WriteCoilCmd(u8 dev, u16 addr, u16 val, 
     u8** cmdBuf,u8* cmdLen,BUF_OPT flag);
void WriteMultiCoilCmd(u8 dev, u16 addr, u16 num, 
     u8* coilVal, u8** cmdBuf,u8* cmdLen,BUF_OPT flag);
void ReadInCoilCmd(u8 dev, u16 addr, u16 num, 
     u8** cmdBuf,u8* cmdLen,BUF_OPT flag);
void ReadCoilCmd(u8 dev, u16 addr, u16 num, 
     u8** cmdBuf,u8* cmdLen,BUF_OPT flag);
 /************************************************************
	********************串口接收解析函数*************************
	************************************************************/
int ReadCoilReqCmd(u8* rxBuf,u8 rxLen, u8** cmdBuf,u8* cmdLen);
int ReadInCoilReqCmd(u8* rxBuf,u8 rxLen, u8** cmdBuf,u8* cmdLen);
int ReadRegisterReqCmd(u8* rxBuf,u8 rxLen, u8** cmdBuf,u8* cmdLen);
int ReadInRegisterReqCmd(u8* rxBuf,u8 rxLen, u8** cmdBuf,u8* cmdLen);
int WriteCoilReqCmd(u8* rxBuf,u8 rxLen, u8** cmdBuf,u8* cmdLen);
int WriteRegisterReqCmd(u8* rxBuf,u8 rxLen, u8** cmdBuf,u8* cmdLen);
int WriteMultiCoilReqCmd(u8* rxBuf,u8 rxLen, u8** cmdBuf,u8* cmdLen);
int WriteMultiRegisterReqCmd(u8* rxBuf,u8 rxLen, u8** cmdBuf,u8* cmdLen);
  /************************************************************
	********************串口反馈解析函数*************************
	************************************************************/
RET_STATUS ReadCoilReqToVar(u8* buf,u8 len, u8 bit, s32 *var);
RET_STATUS ReadInCoilReqToVar(u8* buf,u8 len, u8 bit, s32 *var);
RET_STATUS ReadRegReqToVar(u8* buf,u8 len, u8 reg, s32 *var);
RET_STATUS ReadInRegReqToVar(u8* buf,u8 len, u8 reg, s32 *var);

#endif
