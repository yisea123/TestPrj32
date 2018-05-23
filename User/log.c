/**
  ******************************************************************************
  * @file    /log.c 
  * @author  MMY
  * @version V1.0.0
  * @date    3-Mar-2016
  * @brief   about log
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
	
#include "log.h"

Log_Inf logInf = {0};
	
//函数声明



//函数实现

//void Add_Warning(CDV_INT08U* buf, CDV_INT08U len){
//	
//	
//}

//void Add_Error(CDV_INT08U* buf, CDV_INT08U len){
//	
//	
//	
//}

//void Add_Event(CDV_INT08U* buf, CDV_INT08U len){
//	
//	
//}
/**
  * @brief  清除log记录
  *  
  * @param  
  *   
  * @retval 返回值说明
  *
  * @note   在Log_Cmd中
  */
void Log_Clear(void){
	CDV_INT08U i = 0, type;
	
	for(type = 0; type < LOG_TYPE_N; type++){
		for(i = 0; i < LOG_MEM_N ; i++){
			logInf.TYPE[type].point[i] = 0;
		}
		logInf.TYPE[type].memNo = 0;
	}
	logInf.isInit = 1;
	SPI_Flash_Write((CDV_INT08U *)&logInf, LOG_INF, sizeof(Log_Inf));//写信息
}
/**
  * @brief  清除type log记录
  *  
  * @param  
  *   
  * @retval 返回值说明
  *
  * @note   在Log_Cmd中
  */
void Log_ClearType(LOG_TYPE type){
	CDV_INT08U i = 0;
	ASSERT(type < LOG_TYPE_N);

	for(i = 0; i < LOG_MEM_N ; i++){
		logInf.TYPE[type].point[i] = 0;
	}
	logInf.TYPE[type].memNo = 0;

	logInf.isInit = 1;
	SPI_Flash_Write((CDV_INT08U *)&logInf, LOG_INF, sizeof(Log_Inf));//写信息
}


/**
  * @brief  log数据检查
  *  
  * @param  
  *   
  * @retval 返回值说明
  *
  * @note   
  */
void Log_Check(void){
	CDV_INT08U type;
	
	for(type = 0; type < LOG_TYPE_N; type++){
		if(logInf.TYPE[type].memNo >= LOG_MEM_N) {
			Log_Clear();
			break;
		}
	}
}

/**
  * @brief  log初始化
  * @param  
  * @retval 
  * @note   若已经初始化，则不再初始化
  */
void Log_Init(void){
	if(!logInf.isInit) {
		
		SPI_Flash_Read((CDV_INT08U *)&logInf, LOG_INF, sizeof(Log_Inf));//读信息
		Log_Check();
		logInf.isInit = 1;
	}
}

/**
  * @brief  获取log大小
  * @param  uartNo     串口
  * @retval 返回值说明
  * @note   在 中
  */
CDV_INT32S Log_GetSize(CMD_ARG *arg){
	CDV_INT08U i = 0 , type = 0;
	CDV_INT32U size = 0;
	
	Log_Init();
	
	for(type = 0; type < LOG_TYPE_N; type++){
		for(i = 0; i < LOG_MEM_N ; i++){
			size += logInf.TYPE[type].point[i];
		}
	}
	
	/*发送*/
	ResRequest(arg->buf, arg->len, (CDV_INT08U*)&size, 4, arg, RC_CRC);
	
	return size;
}

/**
  * @brief  获取log大小
  * @param  uartNo     串口
  * @retval 返回值说明
  * @note   在 中
  */
CDV_INT32S Log_GetSizeType(LOG_TYPE type, CMD_ARG *arg){
	CDV_INT08U i = 0;
	CDV_INT32S size = 0;
	ASSERT(type < LOG_TYPE_N);
	Log_Init();
	
	for(i = 0; i < LOG_MEM_N ; i++){
		size += logInf.TYPE[type].point[i];
	}
	
	/*发送*/
	ResRequest(arg->buf, arg->len, (CDV_INT08U*)&size, 4, arg, RC_CRC);
	
	return size;
}
/**
  * @brief  发送log给应用层
  *  
  * @param  uartNo     串口号
  *   
  * @retval 返回值说明
  *
  * @note   在Log_Cmd中
  */
void Log_Send(CMD_ARG *arg){
	CDV_INT08U iSendMem, type;
	CDV_INT16U sendPt;
	CDV_INT16U crc;
	CDV_INT08U* sendBuf = UserMemPtr(CCM_SEND_BUF);
	OS_ERR err;
	int i;
	crc = 0xFFFF;
	
	
	Log_Init();
	//OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 1s
//	SPEED_FLASH;
	
	for(type = 0; type < LOG_TYPE_N; type++){
		iSendMem = logInf.TYPE[type].memNo;
		
		if(logInf.TYPE[type].memNo >= LOG_MEM_N)
			continue;
		
		do{
			MAX_SELF_ADD(iSendMem,LOG_MEM_N);
			sendPt = 0;
			//将当前mem发送
			while(sendPt < logInf.TYPE[type].point[iSendMem]){
				if(sendPt + LOG_SEND_LEN > logInf.TYPE[type].point[iSendMem]) {
					SPI_Flash_Read(sendBuf, LOG_MEM(type, iSendMem) + sendPt,logInf.TYPE[type].point[iSendMem]-sendPt);
					AddTxNoCrc(sendBuf ,logInf.TYPE[type].point[iSendMem]-sendPt ,arg->uart );
					////////////////
//					for(i = 0; i< logInf.TYPE[type].point[iSendMem]-sendPt; i++) {
//						crc = oneCRC16(sendBuf[i], crc); 
//					}
					crc = MODBUS_CRC16(sendBuf, logInf.TYPE[type].point[iSendMem]-sendPt, crc);
					////////////////////////
				} else {
					SPI_Flash_Read(sendBuf, LOG_MEM(type, iSendMem) + sendPt,LOG_SEND_LEN);
					AddTxNoCrc(sendBuf ,LOG_SEND_LEN ,arg->uart );
					////////////////
//					for(i = 0; i< LOG_SEND_LEN; i++) {
//						crc = oneCRC16(sendBuf[i], crc); 
//					}
					crc = MODBUS_CRC16(sendBuf, LOG_SEND_LEN, crc);
					////////////////////////
				}
				sendPt += LOG_SEND_LEN;
				//OSTimeDlyHMSM(0,0,0,15,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			
			
		}while(iSendMem != logInf.TYPE[type].memNo);
	}
	////////////////////////////////////////////////////////////
//  SPEED_FPGA;
	
  AddTxNoCrc((CDV_INT08U*)&crc ,2 ,arg->uart );
}

/**
  * @brief  测试测试测试
  *  
  * @param  uartNo     串口号
  *   
  * @retval 返回值说明
  *
  * @note   在Log_Cmd中
  */
//CDV_INT08U sendBuf[1024] = {0};
void Log_Send1(CMD_ARG *arg){
	CDV_INT32U i;
	CDV_INT32U max = 0x100000 / LOG_SEND_LEN;
	CDV_INT08U* sendBuf = UserMemPtr(CCM_SEND_BUF);
	
//	SPEED_FLASH;
	
	for(i = 0; i < max; i++){
		AddTxNoCrc(sendBuf ,LOG_SEND_LEN ,arg->uart );
	}
	
 // SPEED_FPGA;
}


/**
  * @brief  发送指定类型log给应用层
  *  
  * @param  uartNo     串口号
  *   
  * @retval 返回值说明
  *
  * @note   在Log_Cmd中
  */
void Log_SendType(LOG_TYPE type, CMD_ARG *arg){
	CDV_INT08U iSendMem;
	CDV_INT16U sendPt;
	CDV_INT08U sendBuf[LOG_SEND_LEN];
	OS_ERR err;
	CDV_INT16U crc;
	int i;
	crc = 0xFFFF;
	
	ASSERT(type < LOG_TYPE_N);
	
	Log_Init();
	
	//OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 1s
	
	iSendMem = logInf.TYPE[type].memNo;
	
	if(iSendMem >= LOG_MEM_N)
		return;
	
	do{
		MAX_SELF_ADD(iSendMem,LOG_MEM_N);
		sendPt = 0;
		//将当前mem发送
		while(sendPt < logInf.TYPE[type].point[iSendMem]){
			if(sendPt + LOG_SEND_LEN > logInf.TYPE[type].point[iSendMem]) {
				SPI_Flash_Read(sendBuf, LOG_MEM(type, iSendMem) + sendPt,logInf.TYPE[type].point[iSendMem]-sendPt);
				AddTxNoCrc(sendBuf ,logInf.TYPE[type].point[iSendMem]-sendPt ,arg->uart );
				////////////////
					for(i = 0; i< logInf.TYPE[type].point[iSendMem]-sendPt; i++) {
						crc = oneCRC16(sendBuf[i], crc); 
					}
					////////////////////////
			} else {
				SPI_Flash_Read(sendBuf, LOG_MEM(type, iSendMem) + sendPt,LOG_SEND_LEN);
				AddTxNoCrc(sendBuf ,LOG_SEND_LEN ,arg->uart );
				////////////////
					for(i = 0; i< logInf.TYPE[type].point[iSendMem]-sendPt; i++) {
						crc = oneCRC16(sendBuf[i], crc); 
					}
					////////////////////////
			//	OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 1s
			}
			sendPt += LOG_SEND_LEN;
		}
		
	}while(iSendMem != logInf.TYPE[type].memNo);
	
		  
  AddTxNoCrc((CDV_INT08U*)&crc ,2 ,arg->uart );
}

/**
  * @brief  按类型写log
  *  
  * @param  buf     待写入的字符串指针，不能存在\0字节
  *         len     待写入的字符串的长度,不能超过50B
  *         type    写入的log类型
  *   
  * @retval 返回值说明 BOOL enum枚举型；FALSE 写入失败；TRUE 写入成功
  *
  * @note   
  */
BOOL Log_Write(CDV_CHAR* buf, LOG_TYPE type){
	//CDV_INT08U rn[2] = { '\r', '\n'};
#ifdef __ENABLE_RTC
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
#endif
  CDV_CHAR tmp[150]={0};
	CDV_INT08U tmpL = 0;
	CDV_INT08U len = strlen(buf);
	CDV_CHAR logType[][10] = 
	{ 
	"error",
	"warning",
	"event",
	};
	
	ASSERT(type < LOG_TYPE_N);
	
	if(len > 100)
		return FALSE;
#ifdef __ENABLE_RTC
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	sprintf(tmp,"\r\n 20%02d-%02d-%02d %02d:%02d:%02d+%s+%s",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date
	,RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds, logType[type] , buf);
#else
	sprintf(tmp,"\r\n +%s+%s", logType[type] , buf);
#endif
	
	tmpL = strlen(tmp);
	
	//ASSERT(tmpL <= LOG_MEM_LEN);
	
	if(tmpL > LOG_MEM_LEN)
		return FALSE;
	
	Log_Init();
	
	if(tmpL > LOG_MEM_LEN - logInf.TYPE[type].point[logInf.TYPE[type].memNo]) {//保存的大小+\r\n超过本mem
		MAX_SELF_ADD(logInf.TYPE[type].memNo,LOG_MEM_N);
		logInf.TYPE[type].point[logInf.TYPE[type].memNo] = 0;
	} else {
//		if(logInf.point >= LOG_MEM_LEN){
//			MAX_VAL_ADD(logInf.memNo,LOG_MEM_N);
//			logInf.point = 0;
//		}
	}
	
	if(logInf.TYPE[type].memNo >= LOG_MEM_N)
		return FALSE;
	//保存到flash
	SPI_Flash_Write((CDV_INT08U*)tmp, LOG_MEM(type , logInf.TYPE[type].memNo) + logInf.TYPE[type].point[logInf.TYPE[type].memNo], tmpL);
	logInf.TYPE[type].point[logInf.TYPE[type].memNo] += tmpL;
//	SPI_Flash_Write(rn, LOG_MEM(type, logInf.TYPE[type].memNo) + logInf.TYPE[type].point[logInf.TYPE[type].memNo], 2);//添加\r\n
//	logInf.TYPE[type].point[logInf.TYPE[type].memNo] += 2;
	SPI_Flash_Write((CDV_INT08U *)&logInf, LOG_INF, sizeof(Log_Inf));//写信息
	
	//sprintf(tmp,"\r\n %s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
	
	return TRUE;
}


/**
  * @brief  分析APP下发的LOG命令并操作
  *
  * @param  rxBuf     待分析字符串指针
  *         rxlen       待分析字符串的长度
  *
  * @retval 返回值说明
  *
  * @note   //02 40 
  */
RET_STATUS Log_Cmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) {
	RET_STATUS ret = OPT_SUCCESS;
	CDV_STATUS tmpCdvStat;  
	switch(rxBuf[0]) { //功能码1筛选
		case 0:
			if (rxBuf[1] < LOG_TYPE_N)
  			Log_ClearType(rxBuf[1]);
		  break;
		case 1://获取log大小
			Log_GetSize(arg);
			break;
		case 2://获取所有log
		{	
			u32 clk;
		  tmpCdvStat = g_cdvStat;
			g_cdvStat = CDV_LOG_SEND;
		
			clk = ReadClock100ms();
		
		
		  Log_Send(arg);
		
		  clk = CalcCount(ReadClock100ms(), clk);
		
		  g_cdvStat = tmpCdvStat;
		}
			break;
		case 3:
			if (rxBuf[1] < LOG_TYPE_N)
			  Log_GetSizeType(rxBuf[1], arg);
		  break;
		case 4:
//			Log_Write("log_TEST WARNING" , LOG_WARNING);
//			Log_Write("log+TEST ERROR" , LOG_ERROR);
//			Log_Write("log=TEST EVENT" , LOG_EVENT);
			if (rxBuf[1] < LOG_TYPE_N)
			{
				tmpCdvStat = g_cdvStat;
				g_cdvStat = CDV_LOG_SEND;
				Log_SendType(rxBuf[1], arg);
				g_cdvStat = tmpCdvStat;
			}
			break;
		case 5:
			if(strlen((CDV_CHAR*)(rxBuf + 2)) <= rxLen - 2)
  			Log_Write((CDV_CHAR*)(rxBuf + 2), rxBuf[1]);
		default: //命令码无效不应答
			ret = OPT_FAILURE;
			break;
	}
	
	return ret;
}

void Log_CmdWrite(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg){
	CDV_INT08U i;
	char *tmp = NULL;
	
	if(rxLen > 20 || rxLen <=0)
		return;
	
	NEW08U(tmp, rxLen*5);
	memset(tmp, rxLen*5, 0);
	
	for( i = 0; i < rxLen; i++){
		sprintf(tmp , "%s%02x " 
		,tmp 
		,rxBuf[i]
		);
	}
	
	Log_Write(tmp , LOG_EVENT);
	
	DELETE(tmp);
	
}

