/**
  ******************************************************************************
  * @file    /PH.c 
  * @author  MMY
  * @version V0.0.1
  * @date    12-9-2016
  * @brief   a package of PH function
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */

#include "PH.h"

/**
  * @brief  读取PH值
  *  
  * @param  ID     要操作的PH仪设备号
  *   
  * @retval void
  *
  * @note   查询间隔不能小于500ms（检测仪规定的）
  */
CDV_INT32S PH_ReadValue(u8 ID)
{
	CDV_INT32S val = 0;
	u16 data;
	u8 send_buf[11]={0x01,0x03,0x00,0x00,0x00,0x01,0x84,0x0A};
	u8 recv_buf[16] = {0};
	u8 recv_len=0;
	
	if((ID>32)||(ID<1))
		send_buf[0]=0x01;
	else
	  send_buf[0]=ID;
	
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
//  UART4_Send(send_buf,8);
//	delay_ms(100);
//	RS485_Receive_Data(recv_buf,&recv_len);
	USARTTR(send_buf, 8, recv_buf, 16, &recv_len, 4);
	
	
	data=getCRC16(recv_buf,recv_len-2);
	if((recv_buf[recv_len-2]==(data & 0x00ff)) && (recv_buf[recv_len-1]==((data >> 8) & 0x00ff)))
	{
	  val=(recv_buf[3]<<8)+recv_buf[4];
	}
	//delay_ms(300);
	return val;
}

