/**
  ******************************************************************************
  * @file    /serial_resource.c 
  * @author  MMY
  * @version V2.0.0
  * @date    2017-2-15
  * @brief   
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "serial_resource.h"
#include "CSBCtrl.h"

/**
  * @brief  分析串口资源命令
  *  
  * @param  rxBuf     字符串
  *         rxLen     字符串长度     
  *         
  *   
  * @retval 返回值说明
            OPT_SUCCESS命令执行完成
            OPT_FAILURE命令未执行完成，比如等待未等到
  *
  * @note   
  */
RET_STATUS SerialResCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo) { 
//	CDV_INT08U no , opt , type /*, strLen*/;
//	CDV_INT32U num;

//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];

//	switch(opt) {
//		case 0x00:/*读1ms*/
//		  
//			break;
//		case 0x01:/*读10ms*/
//		  
//			break;
//		case 0x02:/*读100ms*/
//		  
//			break;
//		case 0x03:/*读1s*/
//		  
//			break;
//		default:
//			break;
//		
//	}
	return OPT_SUCCESS;
}



