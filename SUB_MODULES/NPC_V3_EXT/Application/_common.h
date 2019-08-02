
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




#ifndef  _COMMON_
#define  _COMMON_

//#include "_typedef.h"

//#include "_opt.h"

#include "_include.h"    /*放文件头会导致定义不能用在别的h文件中*/

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


void DelayUS(u32 cnt);
	
#define _DEBUG_NPC_
#ifdef  _DEBUG_NPC_

/**
  * @brief  自定义ASSERT
  * @param  expr: 表达式
  * @retval None
  */
  #define ASSERT(expr) ((expr) ? (void)0 : assert((uint8_t *)__FILE__, (uint8_t *)__FUNCTION__, __LINE__))
	#define ASSERT_PLUS(expr, str, len, arg) ((expr) ? (void)0 : assert_plus((uint8_t *)__FILE__, (uint8_t *)__FUNCTION__, __LINE__, str, len, arg))
/* Exported functions ------------------------------------------------------- */
  void assert(uint8_t* file, uint8_t* function, uint32_t line);
	void assert_plus(uint8_t* file, uint8_t* function, uint32_t line, char* str, int len);
	#define NPC_PRINT(str) com_print(str)
	void com_print(const char* str);
#else
  #define ASSERT(expr) ((void)0)
	#define NPC_PRINT(str) ((void)0)
		
	
#endif /* USE_FULL_ASSERT */
	//crc
	
	unsigned short MODBUS_CRC16(unsigned char* pchMsg, unsigned short wDataLen, unsigned short crc);
		
	// ticks
extern volatile uint32_t sys_ticks;
u32 CalcCount(u32 end_count , u32 start_count);

#endif

