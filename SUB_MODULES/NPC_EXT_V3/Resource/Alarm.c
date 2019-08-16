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

#include "Alarm.h"
AlarmInfo strAlarmInfo [ALARM_NUM];
int nAlarmCount = 10;
u8 Alarmflag = 0;

/** @brief  Apply 1.0的命令解析
  * @param  buf     待分析字符串指针
  *         len       待分析字符串的长度
  * @retval 返回值说明
  * @note   
  */
RET_STATUS ApplyParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg)
{
	OS_ERR err;
	RET_STATUS ret = OPT_FAILURE;
	switch(rxBuf[1]){
		case 0x64:
	ret = App1Parse(rxBuf + 2 , rxLen - 2 , arg);
		break;
		case 0x65:
			;
		break;
			
	}
	return ret;
}

RET_STATUS App1Parse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg)
{
	OS_ERR err;
	RET_STATUS ret = OPT_FAILURE;
switch(rxBuf[0]){  //CDV地址应答
		case 0X30:
				AlarmQuery( rxBuf, rxLen, arg);
			break;
		case 0X31:
				ResParaRequest(rxBuf, rxLen, 0, 0, arg->uart);
				AlarmCancel(rxBuf[1]);
				
			break;
		default:
			break;	
		
	}
 return ret;
}
/**
  * @brief  读取PH信息代号
  *  
  * @param  ID     要操作的仪设备号
  *   
  * @retval void
  *
  * @note   查询间隔不能小于500ms（检测仪规定的）
  */
void AlarmQuery11(u8 ID)
{
	CDV_INT32S val = 0;
	u16 data;
	u8 send_buf[16]={0x02,0x30,0x00,0x00,0x00,0x00,0x41,0xFD};
	u8 recv_buf[16] = {0};
	u8 recv_len=0;
	
	if((ID>32)||(ID<1))
		send_buf[2]=0x01;
	else
	  send_buf[2]=ID;
	
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
  //UART4_Send(send_buf,8);
	//ResParaRequest( rxBuf ,rxLen , send_buf, 8, CDV_INT08U uartNo);
	delay_ms(100);
	//RS485_Receive_Data(recv_buf,&recv_len);
	data=getCRC16(recv_buf,recv_len-2);
	if((recv_buf[recv_len-2]==(data & 0x00ff)) && (recv_buf[recv_len-1]==((data >> 8) & 0x00ff)))
	{
	  val=(recv_buf[3]<<8)+recv_buf[4];
	}
	
}





int FlashToAlarmStruct(void)
{
	u32 i,ArlarmNum,ArlarmSize;
	u8 size[255];
	
	SPI_Flash_Read((CDV_INT08U*)&size, SCRIP_ALARM, 255);
	ArlarmSize = size[0]+(size[1]<<8)+(size[2]<<16)+(size[3]<<24);
	ArlarmNum = ArlarmSize/20;
	if(ArlarmSize%20 == 0)
	{
	  i =0;
		for(i=0;i<ArlarmNum;i++)
		{
			strAlarmInfo[i].cAlarmID = *&(size[4+20*i])+(*&(size[5+20*i])<<8)+(*&(size[6+20*i])<<16)+(*&(size[7+20*i])<<24);
			strAlarmInfo[i].cAlarmNo = *&(size[8+20*i])+(*&(size[9+20*i])<<8)+(*&(size[10+20*i])<<16)+(*&(size[11+20*i])<<24);
			strAlarmInfo[i].iAlarmGrade = *&(size[12+20*i])+(*&(size[13+20*i])<<8)+(*&(size[14+20*i])<<16)+(*&(size[15+20*i])<<24);
			strAlarmInfo[i].iValMin = *&(size[16+20*i])+(*&(size[17+20*i])<<8)+(*&(size[18+20*i])<<16)+(*&(size[19+20*i])<<24);
			strAlarmInfo[i].iValMax =  *&(size[20+20*i])+(*&(size[21+20*i])<<8)+(*&(size[22+20*i])<<16)+(*&(size[23+20*i])<<24);
		
		}
	 
	}

	return 0;
}


/***************报警处理*****************/
void AlarmCancel(u8 no)
{ 
	if(no == 0xFF)
		
	  WorkerControl(no,	WORKER_EXIT);  
	else
		WorkerControl(no , WORKER_ONCE);
	
}


/***************报警查询*****************/
void AlarmQuery(u8* rxBuf,u8 rxLen,CMD_ARG *arg)
{
	int i = 0;
	int Para_len = 0;
	int cValNo;
	u8 Para_Buf[ALARM_NUM];
	for( i = 0; i < nAlarmCount; i++)
	{
		 cValNo = strAlarmInfo[i].cAlarmNo;
		if (cValNo>=0 && cValNo<= 255)
		{
			//int iVal = VarGet(cValNo);
			int iVal = cValNo;
			int iValMax = strAlarmInfo[i].iValMax;
			int iValMin = strAlarmInfo[i].iValMin;
			if (iVal < iValMin || iVal > iValMax)
				{
					
					char cAlarmNo = strAlarmInfo[i].cAlarmID;
					Para_Buf[Para_len] = cAlarmNo;
					Para_len++;	
					Alarmflag = 1;
				}
		}
		else
		{
			Alarmflag = 0;
			break;
		}
	}
	ResRequest(arg->buf, arg->len, Para_Buf, Para_len, arg, RC_CRC);
	
			
}

u32 Alarm_flag(u8 flag)
{
	u32 state = 0xffffff;
 if (flag ==1)
	 state = 0x000002;
 else
	 state = 0x000003;
	return state; 
}
