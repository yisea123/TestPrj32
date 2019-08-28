/**
  ******************************************************************************
  * @file    /CSBCtrl.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-2-15
  * @brief   a package of CSBCtrl function,one of serial resource
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

#include "CSBCtrl.h"
#define CSB_COM 6
/**
  * @brief  CSB 命令 转发
  *  
  * @param  ID     要操作的CSB设备号
  *   
  * @retval void
  *
  * @note   
  */
RET_STATUS CSB_Cmd(u8 ID ,u16 addr ,u8 sw)
{
	OS_ERR err;
	CDV_INT32S val = 0;
	u16 data;
	u8 send_buf[8]={0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00};
	u8 recv_buf[100];
	u8 recv_len=0;
	//RET_STATUS ret;

	send_buf[0]=ID;
	send_buf[2]= (addr >> 8) & 0x00ff;
	send_buf[3]= addr & 0x00ff;
	send_buf[4]= sw ? 0xff : 0x00;
	
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
	

	USARTTR(send_buf ,8 ,recv_buf ,100 ,&recv_len , CSB_COM);

	if(recv_len > 2) {
		data=getCRC16(recv_buf,recv_len-2);
		if((recv_buf[recv_len-2]==(data & 0x00ff)) && (recv_buf[recv_len-1]==((data >> 8) & 0x00ff))) //crc
		{
			//val=(recv_buf[3]<<8)+recv_buf[4];
			return OPT_SUCCESS;
		}
		else
		{
			return OPT_FAILURE;
		}
	} 
	else 
	{
		return OPT_FAILURE;
	}
	//delay_ms(300);
	//return val;
}
/**
  * @brief  CSB 命令 转发
  *  
  * @param  ID     要操作的CSB设备号
  *   
  * @retval void
  *
  * @note   
  */
CDV_INT08U CSBTurn(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo){	

	OS_ERR err;
	RET_STATUS ret;
	u8 recv_buf[100];
	u8 recv_len=0;
	
	USARTTR(rxBuf ,rxLen ,recv_buf ,100 ,&recv_len, CSB_COM);
	
	ResParaRequest(NULL, 0, recv_buf, recv_len, uartNo);
	
	return 0;
}
/**
  * @brief  分析超声波控制串口资源命令
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
RET_STATUS CSBCtrlCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo) { 
	CDV_INT08U no , opt , type /*, strLen*/;
	CDV_INT32U num;
	RET_STATUS ret = OPT_SUCCESS;

	no   = rxBuf[0];
	opt  = rxBuf[1];
	type = rxBuf[2];
	
  
	if (0xff == type) {//数值型
		num = (CDV_INT32S)((rxBuf[3]<<24) | (rxBuf[4]<<16) | (rxBuf[5]<<8) | rxBuf[6]);
	} else if (0xfe == type){//变量型
		if(rxBuf[3] >= CDV_VAR_NUM)
		  return OPT_FAILURE;
		num = VarGet(rxBuf[3]);
	} else {
		num = type;
	}
	
	switch(opt) {
		case 0x40://超声启动
		  ret = CSB_Cmd(num ,0x00 ,1);
			break;
		case 0x41://超声停止
		  ret = CSB_Cmd(num ,0x00 ,0);
			break;
		case 0x42://循环启动
		  ret = CSB_Cmd(num ,0x03 ,1);
			break;
		case 0x43://循环停止
		  ret = CSB_Cmd(num ,0x03 ,0);
			break;
		case 0x44://加热启动
		  ret = CSB_Cmd(num ,0x01 ,1);
			break;
		case 0x45://加热停止
		  ret = CSB_Cmd(num ,0x01 ,0);
			break;
		default:
			break;
		
	}
	return ret;
}

