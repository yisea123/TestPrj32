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

#include "cdv_opt.h"


#define MODBUS_ADDR 0x10
/*
 *MODBUS定制定义
 *操作modbus特定资源的寄存器和线圈，非与外设资源打交道时可用
 */
 
 //modbus各缓存的数组大小
#define COIL_CHN		32
#define INCOIL_CHN	32
// 注意修改 var.h中的变量数不能超过HALF_REG_N
// HALF_REG_N 为 REG_N 一半
#if _NPC_VERSION_ == 1u || _NPC_VERSION_ == 2u
#define REG_N				800
#define HALF_REG_N	400
#elif _NPC_VERSION_ == 3u
#define REG_N				1000//640
#define HALF_REG_N	500
#endif
#define INREG_N			32

 /*只读线圈*/
// #define RES_I_NUM              CDV_IO_NUM
// #define RES_EXI_NUM            CDV_EXI_NUM
// #define 
// #define INCOIL_RES_I           0x00/*i输入口线圈起始地址*/ 
// #define INCOIL_RES_DIP         0xc0/*dip拨码开关线圈起始地址*/
// #define INCOIL_MOTOR_END       0xa0/*线圈起始地址*/
// #define INCOIL_MOTOR_ZERO      0xa8/*线圈起始地址*/
// #define INCOIL_UP_LIMIT        0xb0/*线圈起始地址*/
// #define INCOIL_DOWN_LIMIT      0xb8/*线圈起始地址*/
// #define INCOIL_RES_EXI         (INCOIL_RES_I + CDV_IO_NUM)/*EXTI输入线圈起始地址*/
// #define INCOIL_CFG_FPGA        0xd0/*线圈起始地址*/
// 
// #define INCOIL_WIFI_LINK_CHSF      28/*第28个字节，WIFI ID偏移incoil结构体首地址字节。指示连接状态*/
// /*通用部分*/
// #define INCOIL_P               g_modbusInCoil.coilCh/*incoil数组的首指针*/
// #define INCOIL_CH(CHSF)          g_modbusInCoil.coilCh[CHSF]/**/
// #define INCOIL_SET(CHSF, BITSF)  SET_INCOIL_ADDR(((CHSF)<<3) + (BITSF))/*指定i的线圈置1*/
// #define INCOIL_RESET(CHSF, BITSF)    RESET_INCOIL_ADDR(((CHSF)<<3) + (BITSF))/*指定i的线圈置0*/
// #define INCOIL_READ(CHSF, BITSF)    (READ_INCOIL_ADDR(((CHSF)<<3) + (BITSF))? 1 :0)/*读取指定i的线圈值*/
 
 
// #define MODBUS_SET_I(A)      SET_INCOIL_ADDR((A)+INCOIL_RES_I)/*指定i的线圈置1*/
// #define MODBUS_RESET_I(A)    RESET_INCOIL_ADDR((A)+INCOIL_RES_I)/*指定i的线圈置0*/
// #define MODBUS_READ_I(A)    (READ_INCOIL_ADDR(INCOIL_RES_I+(A))? 1 :0)/*读取指定i的线圈值*/
// 
// #define MODBUS_SET_EXI(A)      SET_INCOIL_ADDR((A)+INCOIL_RES_EXI)/*指定exti的线圈置1*/
// #define MODBUS_RESET_EXI(A)    RESET_INCOIL_ADDR((A)+INCOIL_RES_EXI)/*指定i的线圈置0*/
// #define MODBUS_READ_EXI(A)    (READ_INCOIL_ADDR(INCOIL_RES_EXI+(A))? 1 :0)/*读取指定i的线圈值*/

// #define MODBUS_SET_DIP(A)      SET_INCOIL_ADDR((A)+INCOIL_RES_DIP)/*指定dip的线圈置1*/
// #define MODBUS_RESET_DIP(A)    RESET_INCOIL_ADDR((A)+INCOIL_RES_DIP)/*指定dip的线圈置0*/
// #define MODBUS_READ_DIP(A)    (READ_INCOIL_ADDR(INCOIL_RES_DIP+(A))? 1 :0)/*读取指定dip的线圈值*/
 
// #define MODBUS_SET_MOTOR_END(A)    SET_INCOIL_ADDR((A)+INCOIL_MOTOR_END)
// #define MODBUS_RESET_MOTOR_END(A)  RESET_INCOIL_ADDR((A)+INCOIL_MOTOR_END)
// #define MODBUS_READ_MOTOR_END(A)  (READ_INCOIL_ADDR(INCOIL_MOTOR_END+(A))? 1 :0) 
// #define RW_MOTOR_END          g_modbusInCoil.TYPE.motor.motorEnd
// 
// #define MODBUS_SET_MOTOR_ZERO(A)    SET_INCOIL_ADDR((A)+INCOIL_MOTOR_ZERO)
// #define MODBUS_RESET_MOTOR_ZERO(A)  RESET_INCOIL_ADDR((A)+INCOIL_MOTOR_ZERO)
// #define MODBUS_READ_MOTOR_ZERO(A)  (READ_INCOIL_ADDR(INCOIL_MOTOR_ZERO+(A))? 1 :0) 
// #define RW_MOTOR_ZERO           g_modbusInCoil.TYPE.motor.zero
// 
// #define MODBUS_SET_UP_LIMIT(A)    SET_INCOIL_ADDR((A)+INCOIL_UP_LIMIT)
// #define MODBUS_RESET_UP_LIMIT(A)  RESET_INCOIL_ADDR((A)+INCOIL_UP_LIMIT)
// #define MODBUS_READ_UP_LIMIT(A)  (READ_INCOIL_ADDR(INCOIL_UP_LIMIT+(A))? 1 :0) 
// #define RW_UP_LIMIT          g_modbusInCoil.TYPE.motor.upLimit
//		
// #define MODBUS_SET_DOWN_LIMIT(A)    SET_INCOIL_ADDR((A)+INCOIL_DOWN_LIMIT)
// #define MODBUS_RESET_DOWN_LIMIT(A)  RESET_INCOIL_ADDR((A)+INCOIL_DOWN_LIMIT)
// #define MODBUS_READ_DOWN_LIMIT(A)  (READ_INCOIL_ADDR(INCOIL_DOWN_LIMIT+(A))? 1 :0) 
// #define RW_DOWN_LIMIT          g_modbusInCoil.TYPE.motor.downLimit
  
// #define MODBUS_READ_CFG_FPGA(A)  (READ_INCOIL_ADDR(INCOIL_CFG_FPGA+(A))? 1 :0) 
// #define RW_CFG_FPGA          g_modbusInCoil.TYPE.cfg.fpga
 /*线圈*/
// #define COIL_O_CHNUM         20/*20个字节*/
// #define COIL_MOTO_CHNUM      4
// #define COIL_SAVE_CHNUM      1
// #define COIL_MSG_CHNUM       4
// #define COIL_O_CHSF          0x00
// #define COIL_MOTO_CHSF       (COIL_O_CHSF+COIL_O_CHNUM)
// #define COIL_SAVE_CHSF       (COIL_MOTO_CHSF+COIL_MOTO_CHNUM)
// #define COIL_MSG_CHSF        (COIL_SAVE_CHSF+COIL_SAVE_CHNUM)
// 
// 
// 
// 
// #define COIL_RES_O           0x00
// #define COIL_RES_EXO         (COIL_RES_O + CDV_IO_NUM)
// #define COIL_RES_MOTOR_EN    0xa0
// #define COIL_RES_RUN_DIR     0xa8
// #define COIL_RES_ZERO_DIR    0xb0
// #define COIL_RES_MSG         (COIL_MSG_CHSF<<3 )
// #define COIL_RES_SECTION_SENSOR    0xf0
 
// #define MODBUS_SET_O(A)      SET_COIL_ADDR((A)+COIL_RES_O)
// #define MODBUS_RESET_O(A)    RESET_COIL_ADDR((A)+COIL_RES_O)
// #define MODBUS_READ_O(A)    (READ_COIL_ADDR(COIL_RES_O+(A))? 1 :0)
// 
// #define MODBUS_SET_EXO(A)      SET_COIL_ADDR((A)+COIL_RES_EXO)
// #define MODBUS_RESET_EXO(A)    RESET_COIL_ADDR((A)+COIL_RES_EXO)
// #define MODBUS_READ_EXO(A)    (READ_COIL_ADDR(COIL_RES_EXO+(A))? 1 :0)
 
// #define MODBUS_SET_MSG(A)    SET_COIL_ADDR((A)+COIL_RES_MSG)
// #define MODBUS_RESET_MSG(A)  RESET_COIL_ADDR((A)+COIL_RES_MSG)
// #define MODBUS_READ_MSG(A)  (READ_COIL_ADDR(COIL_RES_MSG+(A))? 1 :0) 
 
// #define MODBUS_SET_MOTOR_EN(A)    SET_COIL_ADDR((A)+COIL_RES_MOTOR_EN)
// #define MODBUS_RESET_MOTOR_EN(A)  RESET_COIL_ADDR((A)+COIL_RES_MOTOR_EN)
// #define MODBUS_READ_MOTOR_EN(A)  (READ_COIL_ADDR(COIL_RES_MOTOR_EN+(A))? 1 :0) 
// #define RW_MOTOR_EN          g_modbusCoil.TYPE.ctl.motorEnable
// 
// #define MODBUS_SET_RUN_DIR(A)    SET_COIL_ADDR((A)+COIL_RES_RUN_DIR)
// #define MODBUS_RESET_RUN_DIR(A)  RESET_COIL_ADDR((A)+COIL_RES_RUN_DIR)
// #define MODBUS_READ_RUN_DIR(A)  (READ_COIL_ADDR(COIL_RES_RUN_DIR+(A))? 1 :0) 
// #define RW_RUN_DIR           g_modbusCoil.TYPE.ctl.runDir
// 
// #define MODBUS_SET_ZERO_DIR(A)    SET_COIL_ADDR((A)+COIL_RES_ZERO_DIR)
// #define MODBUS_RESET_ZERO_DIR(A)  RESET_COIL_ADDR((A)+COIL_RES_ZERO_DIR)
// #define MODBUS_READ_ZERO_DIR(A)  (READ_COIL_ADDR(COIL_RES_ZERO_DIR+(A))? 1 :0) 
// #define RW_ZERO_DIR          g_modbusCoil.TYPE.ctl.zeroDir

// #define MODBUS_SET_RETURN    SET_COIL_ADDR(0xb8)/*motorReturn:1*/
// #define MODBUS_RESET_RETURN  RESET_COIL_ADDR(0xb8)
// #define MODBUS_READ_RETURN   (READ_COIL_ADDR(0xb8)? 1 :0) 
// #define MODBUS_SET_FRONT     SET_COIL_ADDR(0xb9)/*motorFront:1*/
// #define MODBUS_RESET_FRONT   RESET_COIL_ADDR(0xb9)
// #define MODBUS_READ_FRONT    (READ_COIL_ADDR(0xb9)? 1 :0) 
// #define MODBUS_SET_BACK      SET_COIL_ADDR(0xba)/*motorBack:1*/
// #define MODBUS_RESET_BACK    RESET_COIL_ADDR(0xba)
// #define MODBUS_READ_BACK     (READ_COIL_ADDR(0xba)? 1 :0) 
// #define MODBUS_SET_ABS       SET_COIL_ADDR(0xbb)/*motorAbs:1*/
// #define MODBUS_RESET_ABS     RESET_COIL_ADDR(0xbb)
// #define MODBUS_READ_ABS      (READ_COIL_ADDR(0xbb)? 1 :0)
// #define MODBUS_SET_ARC       SET_COIL_ADDR(0xbc)/*motorArc:1*/
// #define MODBUS_RESET_ARC     RESET_COIL_ADDR(0xbc)
// #define MODBUS_READ_ARC      (READ_COIL_ADDR(0xbc)? 1 :0)
// #define MODBUS_SET_UPDATE       SET_COIL_ADDR(0xbf)/*update:1*/
// #define MODBUS_RESET_UPDATE     RESET_COIL_ADDR(0xbf)
// #define MODBUS_READ_UPDATE      (READ_COIL_ADDR(0xbf)? 1 :0)
 
// #define MODBUS_SET_SAVE_REG       SET_COIL_ADDR(0xc0)/*save reg:1*/
// #define MODBUS_RESET_SAVE_REG     RESET_COIL_ADDR(0xc0)
// #define MODBUS_READ_SAVE_REG      (READ_COIL_ADDR(0xc0)? 1 :0)

// #define MODBUS_SET_SAVE_INREG       SET_COIL_ADDR(0xc1)/*save inreg:1*/
// #define MODBUS_RESET_SAVE_INREG     RESET_COIL_ADDR(0xc1)
// #define MODBUS_READ_SAVE_INREG      (READ_COIL_ADDR(0xc1)? 1 :0)
// 
// #define MODBUS_SET_SAVE_COIL       SET_COIL_ADDR(0xc2)/*save coil:1*/
// #define MODBUS_RESET_SAVE_COIL     RESET_COIL_ADDR(0xc2)
// #define MODBUS_READ_SAVE_COIL      (READ_COIL_ADDR(0xc2)? 1 :0)
// 
// #define MODBUS_SET_SAVE_INCOIL       SET_COIL_ADDR(0xc3)/*save incoil:1*/
// #define MODBUS_RESET_SAVE_INCOIL     RESET_COIL_ADDR(0xc3)
// #define MODBUS_READ_SAVE_INCOIL      (READ_COIL_ADDR(0xc3)? 1 :0)

// #define MODBUS_SET_SECTION_SENSOR(A)    SET_COIL_ADDR((A)+COIL_RES_SECTION_SENSOR)
// #define MODBUS_RESET_SECTION_SENSOR(A)  RESET_COIL_ADDR((A)+COIL_RES_SECTION_SENSOR)
// #define MODBUS_READ_SECTION_SENSOR(A)   (READ_COIL_ADDR(COIL_RES_SECTION_SENSOR+(A))? 1 :0)
// #define RW_SECTION_SENSOR_ENABLE        g_modbusCoil.TYPE.sectionSensor.enable
 
 /*只读寄存器*/
// #define R_MOTOR_POS(A)      (CDV_INT32S)((g_modbusInReg.TYPE.motor[(A)].posL)| (g_modbusInReg.TYPE.motor[(A)].posH<<16))
// #define R_MOTOR_PPOS(A)      &(g_modbusInReg.TYPE.motor[(A)].posL)
// #define WRITE_MOTOR_POS(A,POS) memcpy( R_MOTOR_PPOS(A) , &(POS) , 4)
// #define CLEAR_MOTOR_POS(A)     memset( R_MOTOR_PPOS(A) , 0 , 4)
// #define READ_MOTOR_POS(A,POS) memcpy( &(POS) , R_MOTOR_PPOS(A) , 4)
// 
// #define R_DST_POS(A)      (CDV_INT32S)((g_modbusInReg.TYPE.dstPos[(A)].posL)| (g_modbusInReg.TYPE.dstPos[(A)].posH<<16))
// #define R_DST_PPOS(A)      &(g_modbusInReg.TYPE.dstPos[(A)].posL)
// #define WRITE_DST_POS(A,POS) memcpy( R_DST_PPOS(A) , &(POS) , 4)
// #define CLEAR_DST_POS(A)     memset( R_DST_PPOS(A) , 0 , 4)
// #define READ_DST_POS(A,POS) memcpy( &(POS) , R_DST_PPOS(A) , 4)
 
// #define R_WORKER_RUN_NUM     g_modbusInReg.TYPE.workerRunNum
// #define R_APP_SET_NUM        g_modbusInReg.TYPE.appSetNum
 /*寄存器*/
// #define RW_MOTOR(A)          g_modbusReg.TYPE.motor[(A)]
// #define R_MOTOR_STEP(A)    (CDV_INT32S)(g_modbusReg.TYPE.motor[(A)].stepL | (g_modbusReg.TYPE.motor[(A)].stepH<<16))
// #define R_MOTOR_PSTEP(A)      &(g_modbusReg.TYPE.motor[(A)].stepL)
// #define WRITE_MOTOR_STEP(A,STEP) memcpy( R_MOTOR_PSTEP(A) , &(STEP) , 4)
// #define RW_VAR(A)            g_modbusReg.TYPE.var[(A)].valL
// #define R_VAR(A)    (CDV_INT32S)(g_modbusReg.TYPE.var[(A)].valL | (g_modbusReg.TYPE.var[(A)].valH<<16))
// #define R_PVAR(A)      &(g_modbusReg.TYPE.var[(A)].valL)
// #define WRITE_VAR(A,VAL) memcpy( R_PVAR(A) , &(VAL) , 4) 
  
// #define RW_STAVAR(A)         g_modbusReg.TYPE.staVar[(A)].val
// #define RW_TIMER(A)          g_modbusReg.TYPE.timer[(A)].val
// #define RW_SRP_NUM_RUN       g_modbusReg.TYPE.scriptInfo.numRun                         /*运行脚本数量*/
// #define RW_SRP_NUM_DEBUG     g_modbusReg.TYPE.scriptInfo.numDebug                       /*调试脚本数量*/
// #define RW_DBG_NO_GET        g_modbusReg.TYPE.debugInfo.getNo                           /*获取的脚本号*/
// #define RW_SECTION_SENSOR_I(A)          g_modbusReg.TYPE.sectionSensor[(A)].iNo         /*分段传感器i号*/
// #define RW_SECTION_SENSOR_VAL(A)        g_modbusReg.TYPE.sectionSensor[(A)].valNo       /*分段传感器变量号*/
// #define RW_SECTION_SENSOR_LIMIT(A)      g_modbusReg.TYPE.sectionSensor[(A)].limit       /*分段传感器最大计数值*/ 
/*
 *MODBUS数据结构和操作定义
 */ 
//#define LITTLE_ENDIAN
///*用于通讯*/
//#if defined(BIG_ENDIAN) && !defined(LITTLE_ENDIAN)

//#define TOBIG16U(A)   (A)
//#define TOBIG32U(A)    (A)

//#elif defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)

//#define TOBIG16U(A)    ((((CDV_INT16U)(A) & 0xff00) >> 8) | \
//                              (((CDV_INT16U)(A) & 0x00ff) << 8))
//#define TOBIG32U(A)    ((((CDV_INT32U)(A) & 0xff000000) >> 24) | \
//                             (((CDV_INT32U)(A) & 0x00ff0000) >> 8) | \
//                             (((CDV_INT32U)(A) & 0x0000ff00) << 8) | \
//                             (((CDV_INT32U)(A) & 0x000000ff) << 24))

//#else

//#error "Either BIG_ENDIAN or LITTLE_ENDIAN must be #defined, but not both."

//#endif
/*以上没有存在意义*/
#define HIGH16U(A) (((CDV_INT16U)(A) & 0xff00) >> 8)
#define LOW16U(A)  ((CDV_INT16U)(A) & 0x00ff)
#define ENDIAN_TF16U(A)    ((((CDV_INT16U)(A) & 0xff00) >> 8) | \
                              (((CDV_INT16U)(A) & 0x00ff) << 8))
#define ENDIAN_TF(A,N)   
															
#include "cdv_include.h"
//#pragma pack(1)
/*modbus寄存器结构体*/
typedef union
{
  struct 
  {
		struct
		{
			CDV_WORD valL;                               /*!< word:     80~9f  变量                           */
			CDV_WORD valH;
		} var[HALF_REG_N];
  } TYPE;                                          /*!< Structure used for resource  access             */
  CDV_WORD reg[REG_N];                           /*!< Type      used for word access                  */
} MODBUS_Register;

extern MODBUS_Register g_modbusReg;

#define MODBUS_REG_ADDR(A) g_modbusReg.reg[(A)]
/*modbus输入寄存器结构体*/
typedef union
{                                  /*!< Structure used for bit  access                  */
	struct
	{
		CDV_WORD valL;                               /*!< word:     80~9f  变量                           */
		CDV_WORD valH;
	} var[10];
  CDV_WORD reg[INREG_N];                          /*!< Type      used for word access                  */
} MODBUS_Input_Register;

extern MODBUS_Input_Register g_modbusInReg;

#define MODBUS_INREG_ADDR(A) g_modbusInReg.reg[(A)]


/*modbus线圈结构体*/
typedef union
{
//	struct
//  {
//		struct
//		{
//			CDV_CHAR O0:1;                        
//			CDV_CHAR O1:1;                       
//			CDV_CHAR O2:1;                       
//			CDV_CHAR O3:1;                       
//			CDV_CHAR O4:1;                       
//			CDV_CHAR O5:1;                       
//			CDV_CHAR O6:1;                       
//			CDV_CHAR O7:1;                       
//			CDV_CHAR O8:1;                      
//			CDV_CHAR O9:1;
//			CDV_CHAR O10:1;
//			CDV_CHAR O11:1;
//			CDV_CHAR O12:1;
//			CDV_CHAR O13:1;
//			CDV_CHAR O14:1;
//			CDV_CHAR O15:1;
//			
//			CDV_CHAR O16:1;                        
//			CDV_CHAR O17:1;                       
//			CDV_CHAR O18:1;                       
//			CDV_CHAR O19:1;                       
//			CDV_CHAR O20:1;                       
//			CDV_CHAR O21:1;                       
//			CDV_CHAR O22:1;                       
//			CDV_CHAR O23:1;                       
//			CDV_CHAR O24:1;                      
//			CDV_CHAR O25:1;
//			CDV_CHAR O26:1;
//			CDV_CHAR O27:1;
//			CDV_CHAR O28:1;
//			CDV_CHAR O29:1;
//			CDV_CHAR O30:1;
//			CDV_CHAR O31:1;
//			
//		} O[10];                                     /*!< bit:     0~9f  speed          */
//		
//  } TYPE1;  
  struct
  {
		struct
		{
			CDV_CHAR O0:1;                        
			CDV_CHAR O1:1;                       
			CDV_CHAR O2:1;                       
			CDV_CHAR O3:1;                       
			CDV_CHAR O4:1;                       
			CDV_CHAR O5:1;                       
			CDV_CHAR O6:1;                       
			CDV_CHAR O7:1;                       
			CDV_CHAR O8:1;                      
			CDV_CHAR O9:1;
			CDV_CHAR O10:1;
			CDV_CHAR O11:1;
			CDV_CHAR O12:1;
			CDV_CHAR O13:1;
			CDV_CHAR O14:1;
			CDV_CHAR O15:1;
			
			CDV_CHAR O16:1;                        
			CDV_CHAR O17:1;                       
			CDV_CHAR O18:1;                       
			CDV_CHAR O19:1;                       
			CDV_CHAR O20:1;                       
			CDV_CHAR O21:1;                       
			CDV_CHAR O22:1;                       
			CDV_CHAR O23:1;                       
			CDV_CHAR O24:1;                      
			CDV_CHAR O25:1;
			CDV_CHAR O26:1;
			CDV_CHAR O27:1;
			CDV_CHAR O28:1;
			CDV_CHAR O29:1;
			CDV_CHAR O30:1;
			CDV_CHAR O31:1;
			
		} O;                                     /*!< bit:     0~9f  speed          */
		
  } TYPE;                                  /*!< Structure used for bit  access                  */
  CDV_CHAR coilCh[COIL_CHN];                     /*!< Type      used for word access                  */
} MODBUS_Coil;

extern MODBUS_Coil g_modbusCoil;

#define MODBUS_COIL_CH(A) g_modbusCoil.coilCh[(A)]
#define SET_COIL_ADDR(A) do{\
	OS_ERR err;\
	OSSemPend(&COIL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err);\
	(MODBUS_COIL_CH((A) >> 3)) |= (0x01 <<((A) & 0x07));\
	OSSemPost (&COIL_SEM,OS_OPT_POST_1,&err);\
}while(0);

#define RESET_COIL_ADDR(A) do{\
	OS_ERR err;\
	OSSemPend(&COIL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err);\
	(MODBUS_COIL_CH((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
	OSSemPost (&COIL_SEM,OS_OPT_POST_1,&err);\
}while(0);
#define READ_COIL_ADDR(A) (((MODBUS_COIL_CH((A) >> 3)) & (0x01 <<((A) & 0x07))) ? 1 : 0)
/*modbus输入线圈结构体*/
typedef union
{
  struct
  {
		struct
		{
			CDV_CHAR I0:1;                        
			CDV_CHAR I1:1;                       
			CDV_CHAR I2:1;                       
			CDV_CHAR I3:1;                       
			CDV_CHAR I4:1;                       
			CDV_CHAR I5:1;                       
			CDV_CHAR I6:1;                       
			CDV_CHAR I7:1;                       
			CDV_CHAR I8:1;                      
			CDV_CHAR I9:1;
			CDV_CHAR I10:1;
			CDV_CHAR I11:1;
			CDV_CHAR I12:1;
			CDV_CHAR I13:1;
			CDV_CHAR I14:1;
			CDV_CHAR I15:1;
			
			CDV_CHAR I16:1;                        
			CDV_CHAR I17:1;                       
			CDV_CHAR I18:1;                       
			CDV_CHAR I19:1;                       
			CDV_CHAR I20:1;                       
			CDV_CHAR I21:1;                       
			CDV_CHAR I22:1;                       
			CDV_CHAR I23:1;                       
			CDV_CHAR I24:1;                      
			CDV_CHAR I25:1;
			CDV_CHAR I26:1;
			CDV_CHAR I27:1;
			CDV_CHAR I28:1;
			CDV_CHAR I29:1;
			CDV_CHAR I30:1;
			CDV_CHAR I31:1;
			        
		} I;                                    /*!< bit:     0 ~ 9f  speed          */
  } TYPE;                                   /*!< Structure used for bit  access                  */
  CDV_CHAR coilCh[INCOIL_CHN];                            /*!< Type      used for word access                  */
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




//CDV_INT16U getCRC16(CDV_INT08U *ptr,CDV_INT16U len) ;
CDV_INT08U ModbusParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg/*, CDV_INT08U uartNo*/);
//void RequestAdd(CDV_INT08U* rxBuf,CDV_INT08U txLen);

 /************************************************************
	********************串口发送函数*****************************
	************************************************************/
void WriteRegisterCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U val, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
void WriteMultiRegisterCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U* regVal, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
void ReadRegisterCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
void ReadInRegisterCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
void WriteCoilCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U val, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
void WriteMultiCoilCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U* coilVal, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
void ReadInCoilCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
void ReadCoilCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
 /************************************************************
	********************串口接收解析函数*************************
	************************************************************/
int ReadCoilReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
int ReadInCoilReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
int ReadRegisterReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
int ReadInRegisterReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
int WriteCoilReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
int WriteRegisterReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
int WriteMultiCoilReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
int WriteMultiRegisterReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen);
  /************************************************************
	********************串口反馈解析函数*************************
	************************************************************/
RET_STATUS ReadCoilReqToVar(CDV_INT08U* buf,CDV_INT08U len, CDV_INT08U bit, CDV_INT32U var);
RET_STATUS ReadInCoilReqToVar(CDV_INT08U* buf,CDV_INT08U len, CDV_INT08U bit, CDV_INT32U var);
RET_STATUS ReadRegReqToVar(CDV_INT08U* buf,CDV_INT08U len, CDV_INT08U reg, CDV_INT32U var);
RET_STATUS ReadInRegReqToVar(CDV_INT08U* buf,CDV_INT08U len, CDV_INT08U reg, CDV_INT32U var);

#endif
