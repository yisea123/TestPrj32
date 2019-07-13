
/**
  ******************************************************************************
  * @file    /cdv_opt.h 
  * @author  MMY
  * @version V1.0.0
  * @date    25-May-2019
  * @brief   only define version and number
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CDV_OPT_
#define  _CDV_OPT_
/*
 *CDV外设相关定义
 */
//#define JBJDZ 0
#define CDV_ID 1
#define APP_ID 0xCD
#define TOAPP_ID 0xAD
#define KFC_IC 0xF1
#define YYC_IC 0xF2
#define ENABLE_FPGA_DOWN 0 // FPGA扩展

#define TCP_COM 0xEE
/*用作FPGA下载时的定义*/
#define FPGA_QUE_NUM 4
#define FPGA_QUE_LEN 4096
#define FPGA_LEN 464196 //XC6SLX16 
//#define FPGA_LEN 801462 //XC6SLX25 
/*定义CDV版本*/
//#define CDV_V1 1u // v1 第一版
#define CDV_V2_2//针对的时V1 第三版
#define NPC_V2_2	//针对v2 的2.2硬件
#define VIRTUAL_LOGIC 1u //使用需逻辑资源
/*

2u版本修改
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

3u版本修改
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
#define _NPC_VERSION_ 3u //cdv 版本1u 老版本，应该已经失效了；2u 2.2硬件版本；3u 2.3硬件版本


#if _NPC_VERSION_ == 1u
		#if defined(CDV_V1)
			#define MAIN_COM 1//MainUsart//
		#else
			#define MAIN_COM 2//MainUsart//
		#endif
		
		#define USE_CASCADE  1u
//  #define ENABLE_PID 1u  // 比例阀pid调节
#elif _NPC_VERSION_ == 2u

  #define MAIN_COM 4//MainUsart//
	
	//#undef ENABLE_FPGA_DOWN
	
	#define USE_NPC_NET  1u

  #define USE_CASCADE  1u
	
	#if USE_CASCADE == 1u
	
		#define USE_CASCADE_MAP 1u
		
	#endif

#elif _NPC_VERSION_ == 3u
  
  #define MAIN_COM 4//MainUsart//

	#undef ENABLE_FPGA_DOWN
	
	#define USE_PVD 0u // PVD 掉电保存

	#if USE_PVD == 0u
	
		#define USE_EXTI_POWER_OFF 1u // 掉电中断保存
		
		#if USE_EXTI_POWER_OFF == 1u
		
			#define USE_FLASH_BAK 1u // flash 备份
			
		#endif
		
	#endif

	#define USE_LOWPOWER 0u // 低功耗
	
	#define USE_MEMMNG 1u  // 外部sram管理
	
	#define USE_COUNTER 0u //使用计数器 
	
	#define USE_NPC_NET  0u
	
  #define USE_CASCADE  1u
	
	#if USE_CASCADE == 1u
	
		#define USE_CASCADE_MAP 1u
		
	#endif
	
	#define USE_WORKER_DEBUG 1u
	
	#define USE_16BIT_CMD 0u // 支持长指令

#endif


////资源使用定义
//#define USE_NPC_NET  1u

#if USE_NPC_NET == 1u

	#define USE_CENTRALIZEDCONTROL 1u
	
#endif


#define DEBUG_TIME 1
///*配置开关*/
//#if _NPC_VERSION_ >= 3u

//	#undef ENABLE_FPGA_DOWN
//	
//	#define USE_PVD 0u // PVD 掉电保存

//	#if USE_PVD == 0u
//	
//		#define USE_EXTI_POWER_OFF 1u // 掉电中断保存
//		
//		#if USE_EXTI_POWER_OFF == 1u
//		
//			#define USE_FLASH_BAK 1u // flash 备份
//			
//		#endif
//		
//	#endif

//	#define USE_LOWPOWER 0u // 低功耗
//	
//	#define USE_MEMMNG 1u  // 外部sram管理
//	
//	#define USE_COUNTER 1u //使用计数器 
//	
//#endif

#define USE_OVERLAP 1u // 使用异步
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

#if USE_MEMMNG == 1u
#define LINE_LEN              0xF000//工人脚本长度60KB
#else
#define LINE_LEN              0x8000//工人脚本长度32KB
#endif
#define LOG_LEN               0x100000//log长度1M

#define SCRIP_MAX_RUN        20//16
//#define SCRIP_MAX_DEBUG      11
// 下载次数 1个字节
#define VERSION_ADDR         0x100 
// OTA升级地址，3个字节，如果是OTA即需要升级
#define OTA_ADDR             VERSION_ADDR + 1
// OTA升级的IP地址
#define OTA_IP               OTA_ADDR + 10
#define WORKER_NUM           SCRIP_MAX_RUN
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


#endif

