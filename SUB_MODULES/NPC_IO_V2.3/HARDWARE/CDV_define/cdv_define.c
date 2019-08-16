/**
  ******************************************************************************
  * @file    /user_define.c 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   global variable 
	*
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
#include "cdv_define.h"



CDV_INT08U g_nowDoRes = 0;   /*标记现在进入的资源号*/
SCRIPT_INFO g_scriptInfo;                               /*CDV脚本状态结构体*/
//DEBUG_DATA g_debugData;
/*软件定时器*/
CDV_INT32U tm1Re = 0,tm2Re = 0,tm3Re = 0;

	//TEST
double RPressureData[100] = {0};
int RPressureCnt = 0;
void DelMemory(void **p) {
	
	if(*p!=NULL) { 
      free(*p);
      *p = NULL;
    }	
}

//void NewError(void ) {
//	u16 i = 0;
//    u8	j = 0;

////	LED3 = LED_ON;
////	Log_Write("NEW ERROR" , LOG_ERROR);
//	while(1)
//		if(0 == i++) {
//			if(0 ==  j++)
////			LED2 = ~LED2;		//呼吸灯;
//			;
//		}
//}
void CmdArgDelete(CMD_ARG *arg)
{
	DELETE(arg->reqbuf);
	DELETE(arg->specbuf);
	INIT_CLEAR(*arg);
}

//void NewMemory(void **p , size_t size ) {

//	if(0 == size)
//		return;

//	if(*p!=NULL) { 
//      free(*p);
//      *p = NULL;
//    }
//	
//    *p = (malloc(size));
//	
//}


