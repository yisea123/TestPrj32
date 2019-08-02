#ifndef  _MODBUS_
#define  _MODBUS_
//#include "f4_includes.h"
#include "cdv_include.h"  

#define MULTIDEVICE_TEST  	0//多设备测试  1时响应本机地址+4的倍数
/*****************************功能码定义******************************************/
#define COIL_CHN   30
#define INCOIL_CHN 30
#define REG_N    600
#define INREG_N  10

/*
*MODBUS定制定义
*操作modbus特定资源的寄存器和线圈，非与外设资源打交道时可用
*/
//线圈地址分配
#define COIL_CTRL       	 0x00/*控制命令线圈起始地址*/ 
#define COIL_ULTRA    		 0x00/*超声启停线圈地址*/
#define COIL_HEAT      		 0x01/*加热控制线圈地址*/
#define COIL_MOTOR      		 0x02/*抛动电机控制线圈地址*/
#define COIL_MCP     	    0x03/*循环泵控制线圈地址*/
#define COIL_SOLENOIDVALVE1 0x04/*电磁阀1控制线圈地址*/
#define COIL_SOLENOIDVALVE2 0x05/*电磁阀2控制线圈地址*/
#define COIL_SOLENOIDVALVE3 0x06/*电磁阀3控制线圈地址*/
#define COIL_SWITCHOFF      0x07/*定时关机控制线圈地址*/
#define COIL_SWITCHON       0x08/*定时开机控制地址*/
#define COIL_WATERWARNING   0x09/*液位报警控制地址*/
#define COIL_OVER  			 0x0A/*过载报警控制地址*/
#define COIL_MODEBUTTON     0x10/*按键模式切换地址*/
#define COIL_CONSTANTPWRCTRLSWITCH   0x11/*恒功率功能开关线圈地址*/
#define COIL_POWERED		 	 0x12/*上电状态线圈地址*/
#define COIL_LCDENABLE 		 0x13/*液晶屏使能线圈地址*/
#define COIL_DEBUG 	   	 0x14/*调试状态切换线圈地址*/
#define COIL_AUTOSOLENOIDVALVE1 0x15/*电磁阀1自动控制线圈地址*/

/*通用部分*/
#define COIL_P               g_modbusInCoil.coilCh/*incoil数组的首指针*/
#define COIL_CH(CHSF)          g_modbusInCoil.coilCh[CHSF]/**/
#define COIL_SET(CHSF, BITSF)  SET_COIL_ADDR(((CHSF)<<3) + (BITSF))/*指定i的线圈置1*/
#define COIL_RESET(CHSF, BITSF)    RESET_COIL_ADDR(((CHSF)<<3) + (BITSF))/*指定i的线圈置0*/
#define COIL_READ(CHSF, BITSF)    (READ_COIL_ADDR(((CHSF)<<3) + (BITSF))? 1 :0)/*读取指定i的线圈值*/


/*线圈*/
#define COIL_O_CHNUM         20/*20个字节*/
#define COIL_MOTO_CHNUM      4
#define COIL_SAVE_CHNUM      1
#define COIL_MSG_CHNUM       4
#define COIL_O_CHSF          0x00
#define COIL_MOTO_CHSF       (COIL_O_CHSF+COIL_O_CHNUM)
#define COIL_SAVE_CHSF       (COIL_MOTO_CHSF+COIL_MOTO_CHNUM)
#define COIL_MSG_CHSF        (COIL_SAVE_CHSF+COIL_SAVE_CHNUM)




#define COIL_RES_O           0x00
#define COIL_RES_EXO         (COIL_RES_O + CSB_IO_NUM)
#define COIL_RES_MOTOR_EN    0xa0
#define COIL_RES_RUN_DIR     0xa8
#define COIL_RES_ZERO_DIR    0xb0
#define COIL_RES_MSG         (COIL_MSG_CHSF<<3 )
#define COIL_RES_SECTION_SENSOR    0xf0

//寄存器地址分配 读写
#define REG_CONSTANTPWR 		0x00/*恒功率寄存器地址*/ 
//#define REG_PWRGEARS 			0x01/*功率档位寄存器地址*/ 
#define REG_TARGETTEMP			0x01/*目标温度寄存器地址*/ 
#define REG_TARGETTEMP_LOWLIMIT	0x02/*目标温度(下限)寄存器地址*/   
#define REG_DATE				 	0x03/*日历日期寄存器起始地址*/ //2byte
#define REG_TIME				 	0x05/*日历时间寄存器起始地址*/ //2byte
#define REG_OSCILLATORMODEL   0x07/*振子型号寄存器地址*/	
#define REG_FREQUENCYCEN      0x08/*中心频率设置寄存器地址*/	
#define REG_FREQUENCYRANGE    0x09/*频率范围设置寄存器地址*/	
#define REG_SCANPERIODFRE     0x0A/*扫频周期设置寄存器地址*/
#define REG_POWER      		   0x0B/*功率设置寄存器地址*/	
#define REG_POWERAMP  		   0x0C/*功率振幅设置寄存器地址*/	
#define REG_SCANPERIODPWR     0x0D/*功率周期设置寄存器地址*/
#define REG_POWEREDGE     		0x0E/*功率梯形波沿设置寄存器地址*/
#define REG_UPPERLIMIT_PREADJ 0x0F/*功率调节上限设置寄存器地址*/
#define REG_LOWLIMIT_PREADJ   0x10/*功率调节下限设置寄存器地址*/
#define REG_OVERLOADCUR       0x11/*过载电流设置寄存器地址*/
#define REG_RATEDCUR 		   0x12/*额定电流寄存器地址*/
#define REG_TMPCOEFF_CONST	   0x13/*温度系数寄存器地址*/
#define REG_HEATPROTECT_DELAYTIME	0x14/*低液位时加热功能关闭延时时间寄存器地址*/
#define REG_FRE_ADJUST_RANGE	0x15/*频率调节范围寄存器地址*/
#define REG_PID_PWR_P 			0x16/*功率PID调节比例参数设置寄存器地址*/
#define REG_PID_PWR_I			0x17/*功率PID调节积分参数设置寄存器地址*/
#define REG_PID_PWR_D			0x18/*功率PID调节微分参数设置寄存器地址*/
#define REG_PID_PWR_CTRL_T		0x19/*功率PID调节控制周期设置寄存器地址*/
#define REG_PID_HEAT_P 			0x1A/*加热PID调节比例参数设置寄存器地址*/
#define REG_PID_HEAT_I			0x1B/*加热PID调节积分参数设置寄存器地址*/
#define REG_PID_HEAT_D			0x1C/*加热PID调节微分参数设置寄存器地址*/
#define REG_PID_HEAT_CTRL_T	0x1D/*加热PID调节控制周期设置寄存器地址*/


//输入寄存器		
#define INREG_CURTEMP			0x31/*当前温度寄存器地址*/ 
#define INREG_CURRENT			0x32/*当前电流寄存器起始地址*/ 
#define INREG_PHVALUE			0x33/*PH值寄存器起始地址*/ 
#define INREG_WATERLEVEL		0x34/*液位状态寄存器地址*/ 
#define INREG_RRATE 				0x35/*电阻率寄存器起始地址*/ 
#define INREG_CURPWR			   0x36/*当前功率寄存器地址*/ 
#define INREG_FRE_IN			   0x37/*当前电网频率寄存器地址*/ 
#define INREG_FRE_OUT		   0x38/*当前振子频率寄存器起始地址*/ 
#define INREG_POWER_FACTOR		0x3A/*功率因素寄存器起始地址*/ 
#define INREG_V_IN				0x3B/*当前电网电压寄存器地址*/ 
 


//#define All_Parameters_addr  			0x0100		//所有参数查询	 
//#define CT_water_valve_Ctrl				0x0100		//清洗箱加水阀控制
//#define CT_drain_valve_Ctrl				0x0101		//清洗箱排水阀控制
//#define LS_drain_valve_Ctrl				0x0102		//储液箱排水阀控制
//#define HT_Ctrl										0x0103		//加热管控制
//#define MDP_Ctrl									0x0104		//磁力泵控制
//#define Motor_Ctrl								0x0105		//电机控制
//#define Save_Set_Parameters				0x0106		//保存设置参数
//#define Model_set_addr								0x0107		//振子型号设置
//#define Fre_change_set_addr						0x0108		//频率变化设置
//#define Fre_upperlimit_set_addr				0x010A		//追频上限设置
//#define Fre_lowerlimit_set_addr				0x010C		//追频下限设置
//#define ALL_Ctrl_Parameters				0x0200		//所以参数设置


#define Modbus_max_send_buf 256//130   //最大帧长
#define Modbus_max_recevie_buf 256//130 
#define MODBUSCMDBUF_NUM 5	//串口命令缓存数量大小
/*****************************************************************************/
/******************************************************************************/
//                            MODBUS相关参数定义
/********************************************************************************/
/*******************************************************************************/

//#define Current_Power_high     		MODBUS_REG_ADDR(All_Parameters_addr)    	//当前功率
//#define Current_Power_low    				MODBUS_REG_ADDR(All_Parameters_addr+1)
//#define Network_Voltage_high  		MODBUS_REG_ADDR(All_Parameters_addr+2)  	//电源电压
//#define Network_Voltage_low	  			MODBUS_REG_ADDR(All_Parameters_addr+3)
//#define now_Current_high	  		  MODBUS_REG_ADDR(All_Parameters_addr+4)		//当前电流
//#define now_Current_low	  		  		MODBUS_REG_ADDR(All_Parameters_addr+5)
//#define mains_fre_high	  		  	MODBUS_REG_ADDR(All_Parameters_addr+6)		//电源频率
//#define mains_fre_low		  		  		MODBUS_REG_ADDR(All_Parameters_addr+7)
//#define Power_Factor_high					MODBUS_REG_ADDR(All_Parameters_addr+8)		//功率因数
//#define Power_Factor_low						MODBUS_REG_ADDR(All_Parameters_addr+9)
//#define Phase_Position_high				MODBUS_REG_ADDR(All_Parameters_addr+10)		//电源相位
//#define Phase_Position_low					MODBUS_REG_ADDR(All_Parameters_addr+11)
//#define Output_Current_high				MODBUS_REG_ADDR(All_Parameters_addr+12)		//输出电流
//#define Output_Current_low					MODBUS_REG_ADDR(All_Parameters_addr+13)
//#define Transducer_Power		  		MODBUS_REG_ADDR(All_Parameters_addr+14)		//换能器电压
//#define Master_Frequncy_high   		MODBUS_REG_ADDR(All_Parameters_addr+15)		//基准频率
//#define Master_Frequncy_low   			MODBUS_REG_ADDR(All_Parameters_addr+16)
//#define Water_Temperature 				MODBUS_REG_ADDR(All_Parameters_addr+17)		//水箱温度
////#define Operating_Condition				MODBUS_REG_ADDR(All_Parameters_addr+18)		//运行指示地址 00:正常 01:过载保护报警 02:过欠压保护报警
//#define Current_Date_high					MODBUS_REG_ADDR(All_Parameters_addr+19)		//当前日期
//#define Current_Date_low						MODBUS_REG_ADDR(All_Parameters_addr+20)
//#define Current_Time_high					MODBUS_REG_ADDR(All_Parameters_addr+21)		//当前时间
//#define Current_Time_low						MODBUS_REG_ADDR(All_Parameters_addr+22)
//#define	Boot_Time_high						MODBUS_REG_ADDR(All_Parameters_addr+23)		//定时开机时间
//#define	Boot_Time_low								MODBUS_REG_ADDR(All_Parameters_addr+24)
//#define Off_Time_high							MODBUS_REG_ADDR(All_Parameters_addr+25)		//定时关机时间
//#define Off_Time_low								MODBUS_REG_ADDR(All_Parameters_addr+26)	
//#define Water_Level								MODBUS_REG_ADDR(All_Parameters_addr+27)		//水位
//#define SP_Power									MODBUS_REG_ADDR(All_Parameters_addr+28)		//功率设置参数
//#define SP_MF											MODBUS_REG_ADDR(All_Parameters_addr+29)		//基准频率设置参数
//#define SP_SR											MODBUS_REG_ADDR(All_Parameters_addr+30)		//扫频范围设置参数
//#define SP_TP											MODBUS_REG_ADDR(All_Parameters_addr+31)		//功率档位设置参数
//#define SP_Temp										MODBUS_REG_ADDR(All_Parameters_addr+32)		//温度设置参数
//#define Runtime_high							MODBUS_REG_ADDR(All_Parameters_addr+33)		//运行时间
//#define Runtime_low									MODBUS_REG_ADDR(All_Parameters_addr+34)	
//#define Model											MODBUS_REG_ADDR(All_Parameters_addr+35)		//振子型号
//#define Fre_change_high						MODBUS_REG_ADDR(All_Parameters_addr+36)		//频率变化
//#define Fre_change_low							MODBUS_REG_ADDR(All_Parameters_addr+37)
//#define Fre_upperlimit_high				MODBUS_REG_ADDR(All_Parameters_addr+38)		//追频上限
//#define Fre_upperlimit_low					MODBUS_REG_ADDR(All_Parameters_addr+39)	
//#define Fre_lowerlimit_high				MODBUS_REG_ADDR(All_Parameters_addr+40)		//追频下限
//#define Fre_lowerlimit_low					MODBUS_REG_ADDR(All_Parameters_addr+41)
//#define Modul_Mode								MODBUS_REG_ADDR(All_Parameters_addr+42)		//调制模式	
//#define Constant_PwrCtrl_Switch		MODBUS_REG_ADDR(All_Parameters_addr+43)		//恒功率控制模式查询	
//#define Rated_Cur									MODBUS_REG_ADDR(All_Parameters_addr+44)		//额定电流
//#define Constant_PwrCtrl_Scale		MODBUS_REG_ADDR(All_Parameters_addr+45)		//功率变化
//#define Overload_Cur							MODBUS_REG_ADDR(All_Parameters_addr+46)		//过载电流
//#define Power_Amp									MODBUS_REG_ADDR(All_Parameters_addr+47)		//功率振幅
//#define Power_Period							MODBUS_REG_ADDR(All_Parameters_addr+48)		//功率周期
//#define fre_Period								MODBUS_REG_ADDR(All_Parameters_addr+49)		//频率扫频周期
//#define mains_cur_high	  		  	MODBUS_REG_ADDR(All_Parameters_addr+50)		//电源电流
//#define mains_cur_low	  		  			MODBUS_REG_ADDR(All_Parameters_addr+51)
//#define oscillator_cur						MODBUS_REG_ADDR(All_Parameters_addr+54)		//振子电流
//#define Temp_Ctrl									MODBUS_REG_ADDR(All_Parameters_addr+55)		//加热
//#define ScalePwr									MODBUS_REG_ADDR(All_Parameters_addr+56)
//#define Interface_Num							MODBUS_REG_ADDR(All_Parameters_addr+57)		//当前界面号
//#define TmpCoeff_Const						MODBUS_REG_ADDR(All_Parameters_addr+58)		//温度系数
//#define Tmp_Offset								MODBUS_REG_ADDR(All_Parameters_addr+59)		//温度补偿
//#define POR_State									MODBUS_REG_ADDR(All_Parameters_addr+60)		//上电状态
//#define Opera_Mode								MODBUS_REG_ADDR(All_Parameters_addr+61)		//操作模式
//#define FreAdj_Range							MODBUS_REG_ADDR(All_Parameters_addr+62)		//频率可调范围
//#define OFF_time_high						  MODBUS_REG_ADDR(All_Parameters_addr+63)		//关机计时
//#define OFF_time_low						  	MODBUS_REG_ADDR(All_Parameters_addr+64)









/******************************************************************************/
/*
 *MODBUS数据结构和操作定义
 */ 
#define LITTLE_ENDIAN
/*用于通讯*/


#define HIGH16U(A) (((CDV_INT16U)(A) & 0xff00) >> 8)
#define LOW16U(A)  ((CDV_INT16U)(A) & 0x00ff)
#define HIGH32U(A) (((CSB_INT32U)(A) & 0xffff0000) >> 16)
#define LOW32U(A)  ((CSB_INT32U)(A) & 0x0000ffff)
//#include "f4_includes.h"

/*modbus输入寄存器结构体*/
typedef union
{                                  /*!< Structure used for bit  access                  */
  CDV_WORD reg[INREG_N];                          /*!< Type      used for word access                  */
} MODBUS_Input_Register;

extern MODBUS_Input_Register g_modbusInReg;

#define MODBUS_INREG_ADDR(A) g_modbusInReg.reg[(A)]

/*modbus寄存器结构体*/
typedef struct 
{
  CDV_INT08U NumUltra;                         /*联机超声波数量*/
  CDV_WORD reg[100];                           /*!< 寄存器 */
} MODBUS_Register;

extern MODBUS_Register g_modbusReg;

#define MODBUS_REG_ADDR(A) g_modbusReg.reg[(A)]


/*modbus线圈结构体*/
typedef struct
{
  CDV_CHAR coilCh[50];                     /*!< Type      used for word access                  */
} MODBUS_Coil;

extern MODBUS_Coil g_modbusCoil;
#define CH_CDV_O(A)          g_o[A]

#define MODBUS_COIL_CH(A) g_modbusOCoil.coilCh[(A)]
#define SET_COIL_ADDR(A) do{\
	(MODBUS_COIL_CH((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);
#define RESET_COIL_ADDR(A) do{\
	(MODBUS_COIL_CH((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);
#define READ_COIL_ADDR(A) (MODBUS_COIL_CH((A) >> 3)) & (0x01 <<((A) & 0x07))
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
#define RESET_COIL_ADDR(A) do{\
	(MODBUS_COIL_CH((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);

/*modbus线圈结构体*/
typedef union
{
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
} MODBUS_OCoil;

extern MODBUS_OCoil g_modbusOCoil;

//#define MODBUS_COIL_CH(A) g_modbusCoil.coilCh[(A)]
#define COIL_REFRESH(A,B) if(B)SET_COIL_ADDR(A)else RESET_COIL_ADDR(A);//线圈更新 B为0：线圈清零 B为非零：线圈置位

#define READ_O(A) (((CH_CDV_O((A) >> 3)) & (0x01 <<((A) & 0x07)))? 1 :0)

#define READ_COIL_ADDR(A) (MODBUS_COIL_CH((A) >> 3)) & (0x01 <<((A) & 0x07))

extern CDV_INT08U Debug_mode;
extern CDV_INT08U Modbus_slave_addr;    //目标从机地址
extern CDV_INT08U Function_Code;		//主机模式功能码
extern CDV_INT16U Modbus_slave_reg_addr;	//从机寄存器地址
extern CDV_INT16U Modbus_slave_reg_num;	//从机寄存器数
extern CDV_INT16U Modbus_reg_val1;//写入从机寄存器的值，若Modbus_slave_reg_num=1，则写入的是16位数据，Modbus_reg_val2无效
extern CDV_INT16U Modbus_reg_val2;//若Modbus_slave_reg_num=2，则写入的是32位数据，Modbus_reg_val1为高16，Modbus_reg_val2位低16
extern CDV_REG08 Modbus_recevie_flag;
extern CDV_REG32 Modbus_timeout_cnt;
//extern CDV_INT08U Modbus_RX_buf[Modbus_max_recevie_buf];
extern CDV_INT08U Modbus_RX_buf[Modbus_max_recevie_buf];
extern CDV_REG08 ModbusCmdBuf_Cnt,ModbusCmdBuf_CntMax,ModbusCmdExecution_State;
extern CDV_INT08U Modbus_TX_buf[Modbus_max_send_buf];
//extern CDV_INT08U Modbus_RX_len;
extern CDV_INT08U Modbus_RX_len;
extern CDV_REG08 Modbus_cmd_flag,Modbus_exe_flag,Modbus_Process_Flag;
extern CDV_INT08U Modbus_SendWait_flag;
extern CDV_REG16 Modbus_SendWait_Cnt;
extern CDV_INT08U USART_TX_BUF_ADDR[Modbus_max_send_buf];
extern CDV_INT08U USART_TX_BUF[Modbus_max_send_buf];
extern CDV_REG08 Tx_flag;
extern CDV_INT08U Modbus_ReadRegister_num;//?????????
extern CDV_INT16U Modbus_ReadRegister_value[100];//?????????
extern CDV_INT08U Modbus_ReadRegister_ture,AlarmB_Flag_last;
extern CDV_INT08U Modbus_local_UI;
extern float TEMP_last;
extern CDV_INT08U Command_Number;//MODBUS???????
extern CDV_INT08U Interface_Number,Interface_Number0,Interface_Number_last;//MODBUS??????????
extern CDV_REG08 All_Parameters_flag;

//#define Modbus_addr  0x01  //本机地址
extern CDV_INT08U Modbus_addr;//本机地址
extern CDV_INT08U Modbus_mode;//0:从机模式 1:主机模式
extern CDV_INT16U Ctrl_commond_addr;
extern CDV_INT16U Ctrl_num;
extern CDV_INT08U slave_number[10],HeartBeat_slave_request_flag,slave_number_dis[10];
extern CDV_REG08 HeartBeat_flag,Interface_refresh_flag;//心跳包发送标志
extern CDV_REG16 HeartBeat_timeout;//心跳包发送间隔计数
extern CDV_REG08	slave_display;//从机号显示标志
extern CDV_REG16	slave_count;//从机号显示计时
extern CDV_REG08	slave_display2;	

//CDV_INT16U getCRC16(CDV_INT08U *ptr,CDV_INT16U len) ;
//CDV_INT08U ModbusParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen);
void RequestAdd(CDV_INT08U* txBuf,CDV_INT08U txLen);
void ReadCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void ReadInCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void ReadRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void ReadInRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void WriteCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void WriteRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void WriteMultiCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void WriteMultiRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void AddTx(CDV_INT08U* txBuf, CDV_INT08U txLen);
void RequestAdd(CDV_INT08U* rxBuf,CDV_INT08U txLen);
void modbus(void);
void Modbus_cmd(void);
void Modbus_exe(void);
CDV_INT16U getCRC16(CDV_INT08U *ptr,CDV_INT16U len);
void Modbus_EleParm_Measure(CDV_INT16U addr);
void Host_Mode(void);
void Hostmode_datahandle(CDV_INT08U Modbus_slave_addr0);
void Modbus_EleParm_Set(CDV_INT16U addr);
void WriteMultiRegister_hostmode(CDV_INT08U Modbus_slave_addr0,CDV_INT16U addr,CDV_INT16U num,CDV_INT16U reg_val1,CDV_INT16U reg_val2);
void WriteMultiRegister_handle(CDV_INT08U* rxBuf,CDV_INT08U rxLen,CDV_INT16U addr,CDV_INT16U num);
void WriteMultiRegister_hostmode2(CDV_INT08U Modbus_slave_addr0,CDV_INT16U addr,CDV_INT16U num);
void Modbus_EleParm_Set2(CDV_INT16U addr);
void HeartBeat(void);
void HeartBeat_slave(void);
void HeartBeat_host_send(void);
void HeartBeat_host_handle(void);
void ReadRegister_hostmode(CDV_INT08U Modbus_slave_addr0,CDV_INT16U addr,CDV_INT16U num);
void WriteCoilCmdParse(CDV_INT16U addr);
void Modbus_Cmd_Receive(void);
void WriteMultiRegisterCmdParse(CDV_INT16U addr);
void ReadRegisterCmdParse(CDV_INT16U addr,CDV_INT16U num);
void ReadInRegisterCmdParse(CDV_INT16U addr,CDV_INT16U num);
void Register_Init(void);
void Debug_DataMeasure(void);
#endif
