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
  * @brief  ����������Դ����
  *  
  * @param  rxBuf     �ַ���
  *         rxLen     �ַ�������     
  *         
  *   
  * @retval ����ֵ˵��
            OPT_SUCCESS����ִ�����
            OPT_FAILURE����δִ����ɣ�����ȴ�δ�ȵ�
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
//		case 0x00:/*��1ms*/
//		  
//			break;
//		case 0x01:/*��10ms*/
//		  
//			break;
//		case 0x02:/*��100ms*/
//		  
//			break;
//		case 0x03:/*��1s*/
//		  
//			break;
//		default:
//			break;
//		
//	}
	return OPT_SUCCESS;
}



