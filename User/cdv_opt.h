
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
#define _NPC_VERSION_ 3u //cdv �汾1u �ϰ汾��Ӧ���Ѿ�ʧЧ�ˣ�2u 2.2Ӳ���汾��3u 2.3Ӳ���汾


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
	
	//#undef ENABLE_FPGA_DOWN
	
	#define USE_NPC_NET  1u

  #define USE_CASCADE  1u
	
	#if USE_CASCADE == 1u
	
		#define USE_CASCADE_MAP 1u
		
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
	
	#define USE_COUNTER 0u //ʹ�ü����� 
	
	#define USE_NPC_NET  1u
	
  #define USE_CASCADE  1u
	
	#if USE_CASCADE == 1u
	
		#define USE_CASCADE_MAP 1u
		
	#endif
	
	#define USE_WORKER_DEBUG 1u
	
	#define USE_16BIT_CMD 0u // ֧�ֳ�ָ��

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

#if USE_MEMMNG == 1u
#define LINE_LEN              0xF000//���˽ű�����60KB
#else
#define LINE_LEN              0x8000//���˽ű�����32KB
#endif
#define LOG_LEN               0x100000//log����1M

#define SCRIP_MAX_RUN        16
//#define SCRIP_MAX_DEBUG      11
// ���ش��� 1���ֽ�
#define VERSION_ADDR         0x100 
// OTA������ַ��3���ֽڣ������OTA����Ҫ����
#define OTA_ADDR             VERSION_ADDR + 1
// OTA������IP��ַ
#define OTA_IP               OTA_ADDR + 10
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


#endif

