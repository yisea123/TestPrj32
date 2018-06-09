/**
  ******************************************************************************
  * @file    /mainUart.c 
  * @author  MMY
  * @version V0.0.1
  * @date    30-11-2015
  * @brief   a package of uart communicate with kfc and  yyc
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
	
	#include "mainUart.h"



u8 Para_Buf[48];
CDV_INT32U fpga_i = 0;
CDV_INT08U MainUsart = 0;

const unsigned char chCRCHTalbe[] =                                 // CRC 高位字节值表
{
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40
};

const unsigned char chCRCLTalbe[] =                                 // CRC 低位字节值表
{
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
0x41, 0x81, 0x80, 0x40
};


unsigned short MODBUS_CRC16(unsigned char* pchMsg, unsigned short wDataLen, unsigned short crc)
{
        unsigned char chCRCHi = (crc >> 8) & 0xFF; // 高CRC字节初始化
        unsigned char chCRCLo = crc & 0xFF; // 低CRC字节初始化
        unsigned short wIndex;            // CRC循环中的索引

        while (wDataLen--)
        {
                // 计算CRC
                wIndex = chCRCLo ^ *pchMsg++ ;
                chCRCLo = chCRCHi ^ chCRCHTalbe[wIndex];
                chCRCHi = chCRCLTalbe[wIndex] ;
        }

        return ((chCRCHi << 8) | chCRCLo) ;
}
   
/**
  * @brief  接收FPGA程序的环境初始化
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   使用串口发送缓存来做接收，放在程序启动之前进行，因为要开辟很大的内存
  */
void FpgaRecvInit(void) {
	CDV_INT08U i;
	g_scriptRecv.doPos = 0;//flash保存的队列
	g_scriptRecv.rxPos = 0;//串口接收的队列
	
	for (i = 0; i < QUE_NUM; i++) {
//		if(g_scriptRecv.buf[i]!=NULL) {
//			free(g_scriptRecv.buf[i]);
//			g_scriptRecv.buf[i] = NULL;
//		}
//		g_scriptRecv.buf[i] = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*QUE_LEN);
		NEW08U(g_scriptRecv.buf[i] , QUE_LEN);
    g_scriptRecv.len[i] = 0;
	}
  g_cdvStat = CDV_FPGA;
}

/**
  * @brief  接收FPGA程序的环境释放
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void FpgaRecvDeinit(void) {
		CDV_INT08U i;
	g_scriptRecv.doPos = 0;//flash保存的队列
	g_scriptRecv.rxPos = 0;//串口接收的队列
	g_cdvStat = CDV_ONLINE;
	for (i = 0; i < QUE_NUM; i++) {
//		if(g_scriptRecv.buf[i]!=NULL) {
//			free(g_scriptRecv.buf[i]);
//			g_scriptRecv.buf[i] = NULL;
//		}
		DELETE(g_scriptRecv.buf[i]);
    g_scriptRecv.len[i] = 0;
	}
}


/**
  * @brief  接收FPGA程序的控制
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   使用g_uartTx.QUEUE.txBuf作为队列缓存，使用g_modbusReg.reg作为队列中接收缓存长度
  */
int ononononon = 0;
void FpgaRecvCtl(void) {
  /*调试用*/
	
//	SPEED_FLASH;
	if (1 == ononononon/*DIP_ON == READ_DIP_SW(1)*/) {//下载电机程序
		ScriptRecvCtl(FPGA_MOTOR , FPGA_LEN);
	} else if (2 == ononononon/*DIP_ON == READ_DIP_SW(2)*/) {//下载IO程序
		ScriptRecvCtl(FPGA_IO , FPGA_LEN);
	}
#ifndef	CDV_V1
    /*FPGA程序下载*/
	g_cdvStat = CDV_FPGA_INIT;
	
	//if(DIP_OFF == READ_DIP_SW(1) && DIP_OFF == READ_DIP_SW(2)/* && DIP_OFF == READ_DIP_SW(0)*/) {
	/*可以添加读取fpga配置信息，然后进行不同的配置*/
	#if ENABLE_FPGA_DOWN
		CH_CDV_DIP = 0x03;/*低位为第1个FPGA，1下载的是FPGA_MOTOR*/
		FpgaInit(CH_CDV_DIP);
	#endif
//	}
#endif
	
//	SPEED_FPGA;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USART_CACHE g_cacheTx = {NULL , 0  , 0 , 0};
OLCMD_CACHE g_olCache = {NULL , 0  , 0 , 0};
SCRIPT_RECV_QUE g_scriptRecv = {0};
CDV_INT08U g_needRequest = 0; 

USART_RX g_uartRx={{{0},{0}},0,0};
USART_TX g_uartTx={{{0},{0}},0,0};

char cdv_error[][20] = 
{ 
"ONLINE",
"SCRIPT_SEND",
"FPGA",
"START",
"STPAUSE",
"RUN",
"PAUSE",
"STOP",
"WAIT",
"GET_ALL_WORKER",
"GET_WORK_SCRIPT",
"RUN_WORK_SCRIPT",
"CDV_RECV",
"CDV_WIFI",
"CDV_FPGA_INIT",
"CDV_HARD_FAULT",
"CDV_NEW_ERROR",
"CDV_DEBUG_JUMP",
"CDV_WHILE_ERROR",
"CDV_LOG_SEND",
"CDV_GET_VERSION",
"CDV_FORMULA",
"CDV_ASSERT",
};

extern CDV_INT08U *slaveTable;
extern	CDV_INT08U slaveTableLen;
/**
  * @brief  将CDV的一些信息通过串口发送出去
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void CDVInfoSend(CDV_INT08U uartNo) {
	CDV_INT08U i , no;
	char tmp[50]={0};
#ifdef __ENABLE_RTC
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
//	
	#endif
	
	
	AddTxNoCrc(
	#define INFO_STR "\r\n"
	INFO_STR, strlen(INFO_STR), uartNo);
//	sprintf(tmp , "%s\r\n%s%d %d\r\nwifi link:%x\r\n" 
//	,(CDV_INT08U*)cdv_error[g_cdvStat] 
//	,"start/stop:" 
//	,g_run.doNo[0] 
//	,g_run.doNo[1] 
//	,WIFI_LINK
//	);
	
	sprintf(tmp , "%s\r\n" 
	,(CDV_INT08U*)cdv_error[g_cdvStat] 
	);
	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
#if _NPC_VERSION_ > 1u
	//if(slaveTableLen)
	{
		sprintf(tmp , "本机id:%d\r\n" 
		,CascadeGetNativeNo()
		);
		AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
	}
#endif
	if(g_line.init)
	{
		sprintf(tmp , "生产线初始化成功\r\n" );
	}
	else
	{
		sprintf(tmp , "生产线初始化失败\r\n" );
	}
	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
	
	for(i = 0; i < WORKER_NUM ; i++) {
		if (IsTaskExist(WorkerTaskTCB[i])) {
			sprintf(tmp,"thread%d-worker%d:step:%d exitStep:%d status:%s\r\n", i, g_threadInfo[i].which, g_threadInfo[i].step, g_threadInfo[i].exitStep, WorkerStatusName[g_threadInfo[i].status]);
	        AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
		}

	}
	
	
	
	
	
//	if (timeEn) {
//		for(no = 0 ; no < g_run.howMany ; no++) {
//			sprintf(tmp,"%s%d\r\n","Worker :", no);
//			AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
//			for(i = 0; i < g_run.cmdNum[no] ; i++) {
//				sprintf(tmp,"%s%d:%d ms\r\n","No.", i , timeMS[no][i]);
//				AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
//			}
//		}
//	}
	//AddTx((CDV_INT08U*)cdv_error[g_cdvStat] , strlen(cdv_error[g_cdvStat]), uartNo);
	//AddTx((CDV_INT08U*)g_run.doNo, 20, uartNo);
	//AddTx((CDV_INT08U*)&err_times1, 1, uartNo);
	#ifdef __ENABLE_RTC
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	sprintf(tmp,"Time:%02d:%02d:%02d\r\n",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	sprintf(tmp,"Date:20%02d-%02d-%02d\r\n",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
	sprintf(tmp,"Week:%d\r\n",RTC_DateStruct.RTC_WeekDay); 
	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
#endif
}

/**
  * @brief  将CDV的当前状态通过串口发送出去
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void CDVUsartSend(CDV_INT08U uartNo) {
	char tmp[USART_RX_BUF_LENGTH]={0};
	int i, j;
	for (i = 0; i < USART_RX_QUEUE_LENGTH; i++)
	{
		memset(tmp, 0, USART_RX_BUF_LENGTH);
		
		for (j = 0; j < g_uartRx.QUEUE.rxLen[i] && j < USART_RX_BUF_LENGTH; j++)
		{
			sprintf(tmp + 3*j , "%x " , g_uartRx.QUEUE.rxBuf[i][j]);
		}
		
		AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
		AddTxNoCrc("\r\n", 2, uartNo);
	}
	
	memset(tmp, 0, USART_RX_BUF_LENGTH);
	sprintf(tmp + 3*j , "doPos:%d\r\nrxPos:%d\r\n" , g_uartRx.doPos, g_uartRx.rxPos);
	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
}
/**
  * @brief  分析联机、调试命令并操作
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxlen       待分析字符串的长度
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
			20160107：添加串口号
//不能使用addtx等，因如果workermanage先申请了锁，会导致死锁
  */

RET_STATUS RecvParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo)
{
	vu32 local_start = 0;
	vu32 local_clk = 0;
	CDV_INT08U temp[2]={0,0};
	CDV_INT16U crc;
	RET_STATUS ret = OPT_SUCCESS;
//	OS_ERR err;
//  LED3 = LED_ON;
	crc = getCRC16(rxBuf,rxLen-2); 
	temp[1] = crc & 0xff; 
	temp[0] = (crc >> 8) & 0xff;
//	global_cnt1 ++;
	if((rxBuf[rxLen-1] == temp[0])&&(rxBuf[rxLen-2] == temp[1]))
	{//crc chk
//		global_start = GetCdvTimeTick();
//		
//			local_clk = CalcTimeMS(GetCdvTimeTick(), local_start);
//			
//			if(local_clk > 100) {
//				local_clk  = 100;
//			}
//			local_start = GetCdvTimeTick();
			
//		Log_CmdWrite(rxBuf , rxLen, uartNo);
		/***********************通用串口******************************/
//		if(general_serial_count != 0 && 3 == uartNo)
//		{
//			ret = ResParse(rxBuf+ 2 , rxLen - 4 ,NULL , 2, uartNo);
//		}	
//		/*************************************************************/
//		else 
		if(0 == OnlineCmdCache(rxBuf , rxLen-2, uartNo)) {
			//ChangeCdvStat(rxBuf , rxLen, uartNo);
			ret = OPT_FAILURE;
  	}
	} 
	else/* if (MAIN_COM == uartNo)*/
	{//判断是否AT指令
		//AddTx(rxBuf , rxLen-2);
		if(0 == strncmp((CDV_INT08C*)rxBuf,"CDV INF",7))//只能在UCOS线程还能调度的时候查询
		{
			//Log_Write("Get inf" , LOG_EVENT);
			CDVInfoSend(uartNo);
			
#if USE_NPC_NET
			EthInfoSend(uartNo);
#endif
		}
//		else if(0 == strncmp((CDV_INT08C*)rxBuf,"CDV TIM",7)) {//流程计时
//			timeEn = !timeEn;
//			if (timeEn) {
//				Log_Write("Enable tim" , LOG_EVENT);
//				temp[0] = 0X31;
//				temp[1] = 0X31;
//			} else {
//				Log_Write("Disable tim" , LOG_EVENT);
//				temp[0] = 0X30;
//				temp[1] = 0X30;
//			}
//			AddTx((CDV_INT08U*)temp, 2, uartNo);
//		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"GET USART",10)){
			CDVUsartSend(uartNo);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"LINE START",10)){
			AllWorkerCtrl(WORKER_LOOP);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"LINE STOP",9)){
			AllWorkerCtrl(WORKER_STOP);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"CDV RESET",9)){
			ResetCdv();
#ifdef __ENABLE_RTC
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"SETDATA",7)){//SETDATA 00 00 00
			//RTC_Set_Time(0,0,0,RTC_H12_AM);	//设置时间
		  RTC_Set_Date(atoi((CDV_INT08C*)rxBuf+8),atoi((CDV_INT08C*)rxBuf+11),atoi((CDV_INT08C*)rxBuf+14),1);		//设置日期
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"SETTIME",7)){//SETTIME 00 00 00
			RTC_Set_Time(atoi((CDV_INT08C*)rxBuf+8),atoi((CDV_INT08C*)rxBuf+11),atoi((CDV_INT08C*)rxBuf+14),RTC_H12_AM);	//设置时间
#endif
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf, "START TEST", 10)){//SETTIME 00 00 00
			RPressureCnt = 0;
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf, "GET TEST", 8)){//SETTIME 00 00 00
			int i;
			char tmp[50]={0};
			if(RPressureCnt >= 100){
				for(i = 0 ; i < 100 ; i++) {
					sprintf(tmp, "%lf\r\n", RPressureData[i]);
					AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
				}
			}else{
				sprintf(tmp, "%s\r\n", "not ready");
				AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
			}
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf, "SET NET", 7)){//SET NET 192.168.1.1:60000
			CDV_INT32S i = 0;
			CDV_CHAR ip[10]={0};
			CDV_INT16U port = 0;
			CDV_CHAR* chr = (CDV_CHAR*)rxBuf + 7;
			int n;
			//提取ip
			while(chr && i < 10) {
				n = atoi(++chr);
				
				if(n > 255)
					break;
				
				ip[i++] = n;
				chr = strchr(chr, '.');
			}
			//提取port
			chr = (CDV_CHAR*)rxBuf + 7;
			if(i == 4) {
				chr = strchr(chr, ':');
				n = atoi(++chr);
				
				if(n < 65536) {
				  port = n;
					MemCpy(ip + 4, &port, 2);
					i += 2;
				  SPI_Flash_Write((CDV_INT08U *)&ip, NET_ADDR, 6);
				  AddTxNoCrc("SUCCESS", 7, uartNo);
	        return ret;
				}
			}
		  AddTxNoCrc("FAILURE", 7, uartNo);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf, "SET NPC", 7)){//SET NPC 0:192.168.1.1:60000
			CDV_INT32S i = 0;
			CDV_CHAR ip[10]={0};
			CDV_INT16U port = 0;
			CDV_CHAR* chr = (CDV_CHAR*)rxBuf + 7;
			CDV_CHAR* tmp_chr = chr;
			int n, id;
			//提取id
			chr = strchr(chr, ':');
			if(chr && chr - (CDV_CHAR*)rxBuf > 8) {
			  id = atoi((CDV_CHAR*)rxBuf + 8);
			}
			//提取ip
			while(chr && i < 10) {
			  tmp_chr = chr;
				n = atoi(++chr);
				
				if(n > 255)
					break;
				
				ip[i++] = n;
				chr = strchr(chr, '.');
			}
			//提取port
			chr = tmp_chr;
			if(chr && i == 4) {
				chr = strchr(chr, ':');
				n = atoi(++chr);
				
				if(n >= 0 && n < 65536 && id >= 0 && id < 256) {
					SPI_Flash_Write((CDV_INT08U *)&id, CHECK_VERION_ADDR, 1);
				  port = n;
					MemCpy(ip + 4, &port, 2);
					i += 2;
				  SPI_Flash_Write((CDV_INT08U *)&ip, NET_ADDR, 6);
				  AddTxNoCrc("SUCCESS", 7, uartNo);
	        return ret;
				}
			}
		  AddTxNoCrc("FAILURE", 7, uartNo);
		} else if (0 == strncmp((CDV_INT08C*)rxBuf, "NPC FIND", 8)) {
			udpecho_find(uartNo);
		} else {///回复开发层
		  //OnlineRequest(rxBuf[1], 0xFF, 0xFF, uartNo);
	  }
		
	}
//	LED3 = LED_OFF;
	return ret;
}

/**
  * @brief  计算字符串的CRC值
  *  
  * @param  ptr     字符串的指针
  *         len     字符串的长度      
  *   
  * @retval CRC值
	*
	* @note   
  */

CDV_INT16U getCRC16(CDV_INT08U *ptr,CDV_INT16U len) 
{ 
    CDV_INT08U i; 
    CDV_INT16U crc = 0xFFFF; 
    if(len==0){
        len = 1;
    } 
    while(len--){   
        crc ^= *ptr; 
        for(i=0; i<8; i++){ 
            if(crc&1){ 
                crc >>= 1;  
                crc ^= 0xA001; 
        	}  
        	else{
                crc >>= 1;
        	} 
        }         
        ptr++; 
    }
    return(crc); 
} 

/**
  * @brief  累次计算字符串的CRC值
  *  
  * @param  ptr     字符串的指针
  *         len     字符串的长度      
  *   
  * @retval CRC值
	*
	* @note   
  */

CDV_INT16U oneCRC16(CDV_INT08U ch , CDV_INT16U crc) 
{ 
    CDV_INT08U i; 

		crc ^= ch; 
		for(i=0; i<8; i++){ 
				if(crc&1){ 
						crc >>= 1;  
						crc ^= 0xA001; 
			}  
			else{
						crc >>= 1;
			} 
		}         

    return(crc); 
} 

/**
  * @brief  对字符串进行modbus分析
  *  
  * @param  rxBuf     待分析字符串指针
  *         len       待分析字符串的长度
  *         txBuff    应答字符串  
  *   
  * @retval 返回值说明
  *
  * @note   RecvParse中
  */

RET_STATUS OnlineParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg){	

	//OS_ERR err;
	RET_STATUS ret;
	switch(rxBuf[1]){ //命令码筛选
		case 0x64://脚本联机
			ret = ResParse(rxBuf + 2 , rxLen - 2 , arg);
		  if(!arg->ptrWorker && 0 == arg->reqlen) {
				if(OPT_SUCCESS == ret) {
					ResRequest(arg->buf, arg->len, 0, 0, arg, RC_CRC);
				} else {
					if(arg->buf)
					arg->buf[1] += 0x80;
					ResRequest(arg->buf, arg->len, 0, 0, arg, RC_CRC);
				}
			}
			break;
		case 0x41://扩展功能------CDV功能
			ASSERT(!arg->ptrWorker);//确保不是在工人流程里
			OperateScript(rxBuf,rxLen, arg);
			break;
		case 0xfe://自定义读
			ASSERT(!arg->ptrWorker);
			ResRequest((CDV_INT08U*)cdv_error[g_cdvStat] , strlen(cdv_error[g_cdvStat]), 0, 0, arg, RC_CRC);
			break;
		default:
			ASSERT(!arg->ptrWorker);
			ResRequest(arg->buf, arg->len, 0, 0, arg, RC_CRC);
			break;
	}
	
	return ret;
}

/**
  * @brief  modbus应答根据rx和应答长度添加头和crc
  *  
  * @param  rxBuf     接收字符串指针
  *         txLen     应答字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   使用到全局变量,不能重入
  *         如果要给自定义字符串添加CRC，rxbuf传入自定义字符串即可
  */
void RequestAdd(CDV_INT08U* rxBuf,CDV_INT08U txLen){
	CDV_INT16U crc;
	USART_TX_BUF(0) = rxBuf[0];
	USART_TX_BUF(1) = rxBuf[1];
	
	crc = getCRC16(USART_TX_BUF_ADDR,txLen-2);
	USART_TX_BUF(txLen-2) = crc & 0xff; 
	USART_TX_BUF(txLen-1) = (crc >> 8) & 0xff;
}


/**
  * @brief  modbus操作脚本
  *         65
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxLen     待分析字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   定制型函数，此函数与main.c的usart_task()函数和stm32f4xx_it.c的USART1_IRQHandler()有关联
  */
void OperateScript(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CMD_ARG *arg){
	CDV_INT08U len = 0 , *txBuf = NULL ,version;//nameLen ,
	CDV_INT16U crc;

//	OS_ERR err;
	//u8 a[2] = {0};
//	CPU_SR_ALLOC();
	switch((rxBuf[2]<<8) + rxBuf[3]){
		case 0x0000://加载生产线
			
//			if(rxLen < 10)//该命令长度必须大于等于10
//				return;
//			g_scriptInfo.no = rxBuf[4];
//			if(g_scriptInfo.no >= SRP_NUM_RUN)
//				return;
		
		  AllWorkerCtrl(WORKER_STOP);
			
			g_scriptInfo.addr = __LINE_ADDR;
//			nameLen = rxBuf[5];//脚本名称长度
			g_scriptInfo.len = (rxBuf[7]<<24) + (rxBuf[6]<<16) + (rxBuf[5]<<8) + rxBuf[4] + 2;//生产线长度
			SPI_Flash_Write((CDV_INT08U *)&g_scriptInfo.len, __LINE_LEN, 4);
//			SPI_Flash_Write(rxBuf,SCRIP_NAME(g_scriptInfo.no),(CDV_INT16U)rxLen);//存储完整命令串，供程序识别有效程序
			
//			if(nameLen > rxLen- 8)//名称长度不符
//				return;
			if(g_scriptInfo.len >= LINE_MAX_LEN || 2 >= g_scriptInfo.len)//包含校验位
				return;
			
//			SPEED_FLASH;
		  g_scriptInfo.tmpLen = 0;
			DELETE(g_scriptInfo.buf);
			
			len = rxLen;//tx总长度=rx
			//delay_ms(900);
			AddTx(rxBuf , len-2, arg->uart);//回复上位机，已收录到
			
			ScriptRecvCtl(g_scriptInfo.addr , g_scriptInfo.len);
			ScriptCrcChk(g_scriptInfo.addr , g_scriptInfo.len, arg->uart);
			
			//更新版本号
			SPI_Flash_Read((CDV_INT08U*)&version, VERSION_ADDR, 1);//读取版本号
			version++;
			SPI_Flash_Write((CDV_INT08U *)&version,VERSION_ADDR,1);
			
//			SPEED_FPGA;
			
			break;
		case 0x0001://脚本读
//			g_scriptInfo.no = rxBuf[4];
//		    SPI_Flash_Read(&len,SCRIP_NAME_LEN(g_scriptInfo.no),1);//读取脚本名长度
//			
//		
//			if(len < 10)//该命令长度必须大于等于10
//				return;
				
			NEW08U(txBuf , 10);
			MemCpy(txBuf , rxBuf, 4);
//			SPI_Flash_Read(txBuf , SCRIP_NAME(g_scriptInfo.no) , len);		
//			crc = getCRC16(txBuf,len-2); 
//			
//			if((txBuf[len-1] != ((crc >> 8) & 0xff))||(txBuf[len-2] != (crc & 0xff))) {//校验存储的命令串的crc
//				DELETE(txBuf);
//				return;
//			}
			SPI_Flash_Read((CDV_INT08U *)&g_scriptInfo.len, __LINE_LEN, 4);
			txBuf[4] = g_scriptInfo.len;
			txBuf[5] = g_scriptInfo.len >> 8;
			txBuf[6] = g_scriptInfo.len >> 16;
			txBuf[7] = g_scriptInfo.len >> 24;
		
			AddTx(txBuf , 8, arg->uart);
			delay_ms(50);
			
		  g_scriptInfo.addr = __LINE_ADDR;
			
		  g_scriptInfo.tmpLen = 0;
			
			Flash_Send(g_scriptInfo.addr, g_scriptInfo.len, arg->uart);
					
			DELETE(txBuf);
			break;		
case 0x0002://压缩包读
      if(!g_line.init)
				return;
			NEW08U(txBuf , arg->len + 4);
			MemCpy(txBuf , arg->buf, arg->len);

			SPI_Flash_Read((CDV_INT08U *)&g_scriptInfo.len, SCRIP_YYC-4, 4);
      g_scriptInfo.len += 2;
			txBuf[arg->len] = g_scriptInfo.len;
			txBuf[arg->len+1] = g_scriptInfo.len >> 8;
			txBuf[arg->len+2] = g_scriptInfo.len >> 16;
			txBuf[arg->len+3] = g_scriptInfo.len >> 24;
		  g_scriptInfo.addr = SCRIP_YYC;
			g_scriptInfo.tmpLen = 0;
			AddTx(txBuf , arg->len + 4, arg->uart);
			//delay_ms(50);	
			DELETE(txBuf);
{
			int i;
			CDV_INT08U ch;
				crc = 0xFFFF;
			for(i = 0; i< g_scriptInfo.len - 2; i++) {
				SPI_Flash_Read(&ch, i + g_scriptInfo.addr, 1);
				crc = oneCRC16(ch, crc); 
			}
}
		  
			
		  
			
			Flash_Send(g_scriptInfo.addr, g_scriptInfo.len - 2, arg->uart);

      AddTxNoCrc((CDV_INT08U*)&crc ,2 ,arg->uart );
				
			break;		
//		case 0x0003://设置调试脚本数量
//			len = (rxBuf[4]<<8) + rxBuf[5];
//		
//		    if(len <= SCRIP_MAX_DEBUG) {
//				SRP_NUM_DEBUG = len;
//			    SPI_Flash_Write((CDV_INT08U *)&SRP_NUM_DEBUG,SCRIP_NUM_DEBUG,2);
//				len = rxLen;//tx总长度=rx 原样应答
//				AddTx(rxBuf , len-2, uartNo);
////				MODBUS_SET_SAVE_REG;
////				MODBUS_RESET_SAVE_INREG;
////				USART_TX_ADD_WITH_LEN(len);//开辟空间
////				memcpy(USART_TX_BUF_ADDR , rxBuf , len);
////				USART_TX_QUEUE_SELF_ADD;		
//			}			
//			
//			break;
			
		case 0x0004://INI写
			if(rxLen < 7)//该命令长度必须大于等于7
				return;
			g_scriptInfo.no = rxBuf[4];
			g_scriptInfo.addr = INI_ADDR(g_scriptInfo.no);
			g_scriptInfo.len = (rxBuf[5]<<8) + rxBuf[6];//长度
			SPI_Flash_Write((CDV_INT08U *)&g_scriptInfo.len,INI_LEN_ADDR(g_scriptInfo.no),2);
			
			if(0 >= g_scriptInfo.len)//
				return;
				
			AddTx(rxBuf , rxLen-2, arg->uart);//回复上位机，已收录到
			
			ScriptRecvCtl(g_scriptInfo.addr , g_scriptInfo.len);
			break;
		case 0x0005://INI读
			g_scriptInfo.no = rxBuf[4];
		    SPI_Flash_Read((CDV_INT08U *)&g_scriptInfo.len,INI_LEN_ADDR(g_scriptInfo.no),2);
			
			if(g_scriptInfo.len < 10)//该命令长度必须大于等于10
				return;
				
			NEW08U(txBuf , 7);
			MemCpy(txBuf , rxBuf , 5);
			txBuf[5] = (CDV_INT08U)(g_scriptInfo.len>>8);
			txBuf[6] = (CDV_INT08U)(g_scriptInfo.len);
			AddTx(txBuf , 7, arg->uart);
			
			
		    g_scriptInfo.addr = INI_ADDR(g_scriptInfo.no);
			
			Flash_Send(g_scriptInfo.addr, g_scriptInfo.len, arg->uart);
					
			DELETE(txBuf);
			break;
		case 0x0006://写版本号
		  SPI_Flash_Write((CDV_INT08U *)&rxBuf[4],VERSION_ADDR,1);
			break;
//		case 0x0007://写生产线名称
//			if(rxBuf[4] < 0xff)
//		    SPI_Flash_Write(rxBuf + 4,SCRIP_LINE_NAME,rxBuf[4] + 1);
//			break;
		case 0x0010://FPGA脚本下载
			AddTx(rxBuf , rxLen-2, arg->uart);
//			switch(rxBuf[4]) {
//				case 0:
//			    ScriptRecvCtl(FPGA_MOTOR , FPGA_LEN);
//					break;
//				case 1:
//					ScriptRecvCtl(FPGA_IO , FPGA_LEN);
//					break;
//				default:
//					break;
//			}
			ScriptRecvCtl(FPGA_MOTOR , FPGA_LEN);
		  AddTx(rxBuf , rxLen-2, arg->uart);
			break;
		case 0x0011://FPGA脚本上传
      Flash_Send(FPGA_MOTOR , FPGA_LEN, arg->uart);
			break;
		case 0xFF00://RESET
			CLI();
			NVIC_SystemReset();
			break;
		default:
			break;
	}	
}
/**
  * @brief  联机操作出错回复
  *         
  *  
  * @param  no     功能码
  *         err     错误码
  *           
  *   
  * @retval 返回值说明
  *
  * @note   
  */
void OnlineRequest(CDV_INT08U no,CDV_INT08U res,CDV_INT08U resNo, CDV_INT08U uartNo) {
	CDV_INT08U len , *txBuf =NULL;
	len = 4;
	NEW08U(txBuf , len);
	txBuf[0] = CDV_ID;
	txBuf[1] = no+0x80;
	txBuf[2] = res;
	txBuf[3] = resNo;
	AddTx(txBuf , len , uartNo);
	DELETE(txBuf);
}
/**
  * @brief  modbus功能码出错回复
  *         
  *  
  * @param  no     功能码
  *         err     错误码
  *           
  *   
  * @retval 返回值说明
  *
  * @note   
  */
void ModbusRequest(CDV_INT08U no,CDV_INT08U errNo, CDV_INT08U uartNo) {
	CDV_INT08U len , *txBuf =NULL;
//	OS_ERR err;
	switch(no) {
//		case 00:
//			len = 3;//tx总长度=5
//		  txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*len);
//		  txBuf[0] = CDV_ID;
//			txBuf[1] = 0x00+0x80;
//			txBuf[2] = errNo;
//		  
////		len = 5;//tx总长度=5
////		USART_TX_ADD_WITH_LEN(len);//开辟空间
////		USART_TX_BUF(0) = CDV_ID;
////		USART_TX_BUF(1) = 0x00+0x80;
////		USART_TX_BUF(2) = errNo;
////		RequestAdd(USART_TX_BUF_ADDR, len);
////		USART_TX_QUEUE_SELF_ADD;	
////		OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
//			break;
//		case 01:
//			break;
		case 65:
			len = 6;//tx总长度=8
			//txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*len);
			NEW08U(txBuf , len);
		  txBuf[0] = CDV_ID;
			txBuf[1] = 0x41+0x80;
			txBuf[2] = 00;
		  txBuf[3] = 00;
		 // txBuf[4] = g_scriptInfo.no;
		  txBuf[5] = errNo;
//			len = 8;//tx总长度=8
//		USART_TX_ADD_WITH_LEN(len);//开辟空间
//		USART_TX_BUF(0) = CDV_ID;
//		USART_TX_BUF(1) = 0x41+0x80;
//		USART_TX_BUF(2) = 00;
//		USART_TX_BUF(3) = 00;
//		USART_TX_BUF(4) = g_scriptInfo.no;
//		USART_TX_BUF(5) = errNo;
//		RequestAdd(USART_TX_BUF_ADDR, len);
//		USART_TX_QUEUE_SELF_ADD;
//		OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
			break;
		default:
			len = 3;//tx总长度=5
		  //txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*len);
		  NEW08U(txBuf , len);
		  txBuf[0] = CDV_ID;
			txBuf[1] = no+0x80;
			txBuf[2] = errNo;
			break;
	}
	AddTx(txBuf , len , uartNo);
	DELETE(txBuf);
}
/**
  * @brief  比较do的发送字符串和接收字符串是否一样
  *         
  *  
  * @param  no     功能码
  *         err     错误码
  *           
  *   
  * @retval 返回值说明
  *
  * @note   用于CDV_START状态下启动变量设置的命令回复比较
  */
CDV_INT08U RequestCmp(void) {
	CDV_INT08U len, i;
	if(g_cacheTx.len != USART_RX_DO_LEN)
		return 0;
	len = g_cacheTx.len;
	
//	if(*(USART_RX_DO_ADDR + 0) != TOAPP_ID)
//		return 0;
		
	for (i = 0 ; i < len ; i++ ) {
		if(*(USART_RX_DO_ADDR + i) != *(g_cacheTx.buf + i))
			return 0;
	}
	g_cacheTx.mark = 0;
	return 1;
}

/**
  * @brief  保存需要回复的发送指令
  *         
  *  
  * @param  no     功能码
  *         err     错误码
  *           
  *   
  * @retval 返回值说明
  *
  * @note   
  */
CDV_INT08U NeedRequestTx(CDV_INT08U uartNo) {
	if(USART_TX_HAD) {
		return 0;
	}
	
	if(0 == USART_TX_DO_LEN || g_cacheTx.mark !=0)
		return 0;
	
	g_cacheTx.len = USART_TX_DO_LEN;
	g_cacheTx.uartNo = uartNo;
//	if(g_cacheTx.buf != NULL) { 
//		free(g_cacheTx.buf);
//		g_cacheTx.buf = NULL;
//	}
//	
//	g_cacheTx.buf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(g_cacheTx.len));
	NEW08U(g_cacheTx.buf , g_cacheTx.len);
	MemCpy(g_cacheTx.buf , USART_TX_DO_ADDR , g_cacheTx.len);
	g_cacheTx.mark = 1;
	return 1;
}
/**
  * @brief  串口放入cache
  *         
  *  
  * @param  rxBuf     接收命令
  *         rxLen     接收长度
  *           
  *   
  * @retval 返回值说明0失败1成功
  *
  * @note   
  */
CDV_INT08U OnlineCmdCache(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo) {
	OS_ERR err;
	
	ASSERT(rxBuf && rxLen);
	if(rxBuf == NULL || rxLen == 0)
		return 0;
	
//	while(g_olCache.mark)
//		delay_ms(3);//还未处理好
	
	if(g_olCache.mark)//还未处理好
		return 0;
		
	g_olCache.len = rxLen;
	
//	if(g_olCache.buf != NULL) { 
//		free(g_olCache.buf);
//		g_olCache.buf = NULL;
//	}
//	
//	g_olCache.buf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(g_olCache.len));
	NEW08U(g_olCache.buf,g_olCache.len);
	if(g_olCache.buf != NULL) { 
	    MemCpy(g_olCache.buf , rxBuf , g_olCache.len);
		g_olCache.uart = uartNo;
	}else{
	    g_olCache.len = 0;//应该换成下面的错误警告
		return 0;
	}
	
	g_olCache.mark = 1;
	OSTaskResume((OS_TCB*)&TaskParseTCB,&err);		
	return 1;
}
/**
  * @brief  清联机命令cache
  *         
  *  
  * @param  
  *           
  *   
  * @retval 返回值说明
  *
  * @note   
  */
CDV_INT08U ClearOnlineCmdCache(void) {
	DELETE(g_olCache.buf);
	g_olCache.len = 0;
	g_olCache.uart = 0;
	g_olCache.mark = 0;		
	return 1;
}

/** @brief  主串口发送命令
  * @param  
  * @retval 
  * @note   
  */
RET_STATUS MAINUSART_Send(CDV_INT08U* txBuf, CDV_INT16U txLen){  
	OS_ERR err;
	//CDV_INT08U len = txLen;
	
	while(!USART_CAN_DO) {//这句是因为连续add显示变量会死机而添加的
		//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //延时
		return OPT_FAILURE;
	}

	OSSemPend(&TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
	if(err == OS_ERR_TIMEOUT)
		return OPT_FAILURE;
	
	if(USART_CAN_DO){
		USART_TX_ADD_WITH_LEN(txLen);//开辟空间	
		MemCpy(USART_TX_BUF_ADDR, txBuf , txLen);	
		USART_TX_QUEUE_SELF_ADD;			
	}
	
	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	
	return OPT_SUCCESS;
}
/** @brief  主串口发送命令
  * @param  
  * @retval 
  * @note   
  */
RET_STATUS MAINUSART_SendEx(CDV_INT08U* txBuf, CDV_INT16U txLen, CDV_INT08U* exBuf, CDV_INT16U exLen){  
	OS_ERR err;
	//CDV_INT08U len = txLen;
	
	while(!USART_CAN_DO) {//这句是因为连续add显示变量会死机而添加的
		//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //延时
		return OPT_FAILURE;
	}

	OSSemPend(&TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
	if(err == OS_ERR_TIMEOUT)
		return OPT_FAILURE;
	
	if(USART_CAN_DO){
		USART_TX_ADD_WITH_LEN(txLen + exLen);//开辟空间	
		MemCpy(USART_TX_BUF_ADDR, txBuf , txLen);	
		MemCpy(USART_TX_BUF_ADDR + txLen, exBuf , exLen);	
		USART_TX_QUEUE_SELF_ADD;			
	}
	
	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	
	return OPT_SUCCESS;
}
/**
  * @brief  不需要回复的串口命令发送准备函数
  *         
  *  
  * @param  txBuf     发送字符串首地址指针
  *         txLen     发送字符串长度,不包括crc
  *           
  *   
  * @retval 返回值说明
  *
  * @note   
  */
void AddTxNoCrc(CDV_INT08U* txBuf, CDV_INT16U txLen, CDV_INT08U uartNo) {
	//OS_ERR err;
	CDV_INT08U txRealLen;
	CDV_INT08U *TX_BUF= NULL;
	CDV_INT08U TX_Head[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	
	//CDV_INT08U len = txLen;
	//ASSERT(txBuf && txLen);
	if(NULL == txBuf || 0 == txLen || 0xFF == uartNo)
		return;
	
	if(txBuf[0] != 0xF2){
		TX_BUF = txBuf;
	}else{//包头
		TX_Head[5] = txLen;
		txRealLen=txLen+6;
		NEW08U(TX_BUF,txRealLen);
		MemCpy(TX_BUF , TX_Head, 6);
		if(txLen>0)
		MemCpy(TX_BUF+6 , txBuf, txLen);	
		txLen = txRealLen;
	}
	
	if(TCP_COM == uartNo)//tcp
	{
		while (OPT_FAILURE == TCP_ServerSend(TX_BUF, txLen));//TCP_ServerSend(txBuf, txLen);
	}
//	else if(0xFF == uartNo)//spi cascade
//	{
//		Cascade_Slave_Write(txBuf, txLen);
//	}	
	else if(MAIN_COM == uartNo)//main usart
	{
		while (OPT_FAILURE == MAINUSART_Send(TX_BUF, txLen));
	}
	else
	{
		USARTSend(txBuf, txLen, uartNo);
	}
	
	if(txBuf[0] == 0xF2)
	{
		DELETE(TX_BUF);
	}
	
//	while(!USART_CAN_DO) {//这句是因为连续add显示变量会死机而添加的
//		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //延时
//	}
//	
//	OSSemPend(&TX_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
//	if(USART_CAN_DO){
//		
//		USART_TX_ADD_WITH_LEN(len);//开辟空间	
//		memcpy(USART_TX_BUF_ADDR, txBuf , txLen);		
////		RequestAdd(USART_TX_BUF_ADDR, len);
//		USART_TX_QUEUE_SELF_ADD;			
//		
//	//} else {
//		//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //延时
//	}
//	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
//	//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //延时//防止8个工人一起发送//这句是因为连续add显示变量会死机而添加的
//	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	
}
/**
  * @brief  不需要回复的串口命令发送准备函数
  *         
  *  
  * @param  txBuf     发送字符串首地址指针
  *         txLen     发送字符串长度,不包括crc
  *           
  *   
  * @retval 返回值说明
  *
  * @note   
  */

//void AddTx1(CDV_INT08U* txBuf, CDV_INT08U txLen, CDV_INT08U uartNo) {
//	OS_ERR err;
//	CDV_INT08U len = txLen + 2;
//	CDV_INT16U crc;
//	CDV_INT08U txRealLen;
//	CDV_INT08U *TX_BUF= NULL;
//	CDV_INT08U TX_Head[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
//	
//	//ASSERT(txBuf && txLen);
//	if(NULL == txBuf || 0 == txLen)
//		return;
//	if(txBuf[0] != 0xF2)
//	{
//	 if(0xFF == uartNo)
//	{
//		Cascade_Slave_Write(txBuf, txLen);
//		return;
//	}
//	else if(0xEE == uartNo)
//	{
//		TCP_ServerSend(txBuf, txLen);
//		return;
//	}
//	
//	while(!USART_CAN_DO) {//这句是因为连续add显示变量会死机而添加的
//		//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //延时
//		return;
//	}
//	
//	OSSemPend(&TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
//	if(err == OS_ERR_TIMEOUT)
//		return;
//	if(USART_CAN_DO){
//		
//		USART_TX_ADD_WITH_LEN(len);//开辟空间	
//		memcpy(USART_TX_BUF_ADDR, txBuf , txLen);		
//		RequestAdd(USART_TX_BUF_ADDR, len);
//		USART_TX_QUEUE_SELF_ADD;			
//		
//	//} else {
//		//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //延时
//	}
//	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
//	//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //延时//防止8个工人一起发送//这句是因为连续add显示变量会死机而添加的
//	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
//	
//}
//	else
// {
//	TX_Head[5] = txLen+2;
//  txRealLen=txLen+6;
//	NEW08U(TX_BUF,txRealLen);
//	memcpy(TX_BUF , TX_Head, 6);
//	if(txLen>0)
//	memcpy(TX_BUF+6 , txBuf, txLen);	
//	crc = getCRC16(TX_BUF+6,txLen);
//	
//	//crc_ccitt(txBuf,txLen,0xffff);
//	MODBUS_CRC16(txBuf,txLen, 0xFFFF);
//	if(0xEE == uartNo)
//	{
////		while (OPT_FAILURE == TCP_ServerSend(txBuf, txLen));
////		while (OPT_FAILURE == TCP_ServerSend((CDV_INT08U*)&crc, 2));
//		//while (OPT_FAILURE == TCP_ServerSendEx(txBuf, txLen, (CDV_INT08U*)&crc, 2));
//	}	
////	else if(0xFF == uartNo)
////	{
////		Cascade_Slave_Write(txBuf, txLen);
////	}
//	else if(MAIN_COM == uartNo) //main usart
//	{
////		while (OPT_FAILURE == MAINUSART_Send(txBuf, txLen));
////		while (OPT_FAILURE == MAINUSART_Send((CDV_INT08U*)&crc, 2));
//		while (OPT_FAILURE == MAINUSART_SendEx(TX_BUF, txRealLen, (CDV_INT08U*)&crc, 2));
//		
//		
//	}
//	else
//	{
//		USARTSend(txBuf, txLen, uartNo);
//		USARTSend((CDV_INT08U*)&crc, 2, uartNo);
//	}
//	DELETE(TX_BUF);
//}
//	
//}
void AddTx(CDV_INT08U* txBuf, CDV_INT08U txLen, CDV_INT08U uartNo) {
	CDV_INT16U crc;
	
	CDV_INT08U txRealLen;
	CDV_INT08U *TX_BUF= NULL;
	CDV_INT08U TX_Head[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	
	
	ASSERT(txBuf && txLen);
	if(NULL == txBuf || 0 == txLen || 0xFF == uartNo)
		return;

	crc = MODBUS_CRC16(txBuf,txLen, 0xFFFF);
	//crc = getCRC16(txBuf,txLen);
	
	
	if(txBuf[0] != 0xF2){
		TX_BUF = txBuf;
	}else{//包头
		TX_Head[5] = txLen+2;
		txRealLen=txLen+6;
		NEW08U(TX_BUF,txRealLen);
		MemCpy(TX_BUF , TX_Head, 6);
		if(txLen>0)
		MemCpy(TX_BUF+6 , txBuf, txLen);	
		txLen = txRealLen;
	}
	
	if(TCP_COM == uartNo)
	{
		while (OPT_FAILURE == TCP_ServerSendEx(TX_BUF, txLen, (CDV_INT08U*)&crc, 2));
	}	
	else if(MAIN_COM == uartNo) //main usart
	{
		while (OPT_FAILURE == MAINUSART_SendEx(TX_BUF, txLen, (CDV_INT08U*)&crc, 2));
	}
	else
	{
		USARTSend(TX_BUF, txLen, uartNo);
		USARTSend((CDV_INT08U*)&crc, 2, uartNo);
	}
	
	if(txBuf[0] == 0xF2)
	{
		DELETE(TX_BUF);
	}
}
void AddTx111(CDV_INT08U* txBuf, CDV_INT08U txLen, CDV_INT08U uartNo) {
	CDV_INT16U crc;
	
	ASSERT(txBuf && txLen);
	if(NULL == txBuf || 0 == txLen)
		return;

	//getCRC16(txBuf,txLen);
	//crc_ccitt(txBuf,txLen,0xffff);
	crc = MODBUS_CRC16(txBuf,txLen, 0xFFFF);
	if(TCP_COM == uartNo)
	{
//		while (OPT_FAILURE == TCP_ServerSend(txBuf, txLen));
//		while (OPT_FAILURE == TCP_ServerSend((CDV_INT08U*)&crc, 2));
		while (OPT_FAILURE == TCP_ServerSendEx(txBuf, txLen, (CDV_INT08U*)&crc, 2));
	}	
//	else if(0xFF == uartNo)
//	{
//		Cascade_Slave_Write(txBuf, txLen);
//	}
	else if(MAIN_COM == uartNo) //main usart
	{
//		while (OPT_FAILURE == MAINUSART_Send(txBuf, txLen));
//		while (OPT_FAILURE == MAINUSART_Send((CDV_INT08U*)&crc, 2));
		while (OPT_FAILURE == MAINUSART_SendEx(txBuf, txLen, (CDV_INT08U*)&crc, 2));
		
		
	}
	else
	{
		USARTSend(txBuf, txLen, uartNo);
		USARTSend((CDV_INT08U*)&crc, 2, uartNo);
	}
//	
//	while(!USART_CAN_DO) {//这句是因为连续add显示变量会死机而添加的
//		//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //延时
//		return;
//	}
//	
//	OSSemPend(&TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
//	if(err == OS_ERR_TIMEOUT)
//		return;
//	if(USART_CAN_DO){
//		
//		USART_TX_ADD_WITH_LEN(len);//开辟空间	
//		memcpy(USART_TX_BUF_ADDR, txBuf , txLen);		
//		//RequestAdd(USART_TX_BUF_ADDR, len);
//		memcpy(USART_TX_BUF_ADDR + txLen, &crc, 2);
//		USART_TX_QUEUE_SELF_ADD;			
//		
//	//} else {
//		//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //延时
//	}
//	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
//	//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //延时//防止8个工人一起发送//这句是因为连续add显示变量会死机而添加的
//	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	
}


///**
//  * @brief  不需要回复的串口命令发送准备函数
//  *         
//  *  
//  * @param  txBuf     发送字符串首地址指针
//  *         txLen     发送字符串长度,不包括crc
//  *           
//  *   
//  * @retval 返回值说明
//  *
//  * @note   
//  */
//void AddRequestTx(CDV_INT08U* txBuf, CDV_INT08U txLen, CDV_INT08U uartNo) {
//	OS_ERR err;
//	CDV_INT08U len = txLen + 2;
//	if(NULL == txBuf || 0 == txLen)
//		return;
//	
////	if(5 == uartNo/*g_whichUart*/) {
////		ATCOMSend((CDV_INT08C*)(CDV_INT08S*)txBuf , txLen);
////		//ATCOMSendReq((CDV_INT08C*)(CDV_INT08S*)txBuf , txLen);
////		return;
////	}
//	if(0xFF == uartNo)
//	{
//		Cascade_Slave_Write(txBuf, txLen);
//		return;
//	}	else if(0xEE == uartNo)
//	{
//		TCP_ServerSend(txBuf, txLen);
//		return;
//	}
//	
//	while(!USART_CAN_DO) {//这句是因为连续add显示变量会死机而添加的
//		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //延时
//	}
//	
//	OSSemPend(&TX_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
//	if(USART_CAN_DO){
//		USART_TX_ADD_WITH_LEN(len);//开辟空间
//		memcpy(USART_TX_BUF_ADDR, txBuf , txLen);
//		RequestAdd(USART_TX_BUF_ADDR, len);
//		while(!NeedRequestTx(uartNo));
//		USART_TX_QUEUE_SELF_ADD;
//	}
//	
//	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
//	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
//	while (IS_TX_NEED_REQUEST);//等待应答完成
//}
/*****************************************************************脚本接收专用******************************************************************/
/**
  * @brief  接收脚本的环境初始化
  *  
  * @param  addr 保存的flash地址
            len 脚本的长度
  *   
  * @retval void
  *
  * @note   g_scriptRecv
  */
void ScriptRecvInit(CDV_INT32U addr , CDV_INT32U len) {
	CDV_INT08U i;
	g_scriptRecv.doPos = 0;
	//g_scriptRecv.no = 0;
	g_scriptRecv.rxPos = 0;
	g_scriptRecv.totalLen = len;
	g_scriptRecv.tmpLen = 0;
	g_scriptRecv.addr = addr;
	
//	for (i = 0; i < QUE_NUM; i++) {
////		DELETE(g_scriptRecv.buf[i]);
////		g_scriptRecv.buf[i] = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*QUE_LEN);
//		NEW08U(g_scriptRecv.buf[i], QUE_LEN);
//        g_scriptRecv.len[i] = 0;
//	}
	
	for (i = 0; i < QUE_NUM; i++) {
		g_scriptRecv.buf[i] = UserMemPtr(CCM_RECV_BUF + i * QUE_LEN);
    g_scriptRecv.len[i] = 0;
	}
	
	
    g_cdvStat = CDV_RECV;
}

/**
  * @brief  接收脚本的环境释放
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   g_scriptRecv
  */
void ScriptRecvDeinit(void) {
//	CDV_INT08U ch;
//	CDV_INT16U crc = 0xFFFF , tmpCrc;
	CDV_INT32U i ;
	
//	for(i = 0; i< g_scriptRecv.totalLen - 2; i++) {
//		SPI_Flash_Read(&ch, i + g_scriptRecv.addr, 1);
//		crc = oneCRC16(ch, crc); 
//	}
//	
//	SPI_Flash_Read(&ch, i + g_scriptRecv.addr, 1);
//	tmpCrc = ch;
//	SPI_Flash_Read(&ch, i+1 + g_scriptRecv.addr, 1);
//	tmpCrc |= ch<<8;
//	
//	if(crc == tmpCrc) {
//		ModbusRequest(65,00);
//	} else {
//		ModbusRequest(65,02);
//	}
	
	g_scriptRecv.doPos = 0;
	//g_scriptRecv.no = 0;
	g_scriptRecv.rxPos = 0;
	g_scriptRecv.totalLen = 0;
	g_scriptRecv.tmpLen = 0;
	g_scriptRecv.addr = 0;
	g_cdvStat = CDV_ONLINE;
	
	for (i = 0; i < QUE_NUM; i++) {
		g_scriptRecv.buf[i] = NULL;
    g_scriptRecv.len[i] = 0;
	}
	
}


/**
  * @brief  接收脚本的控制
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   g_scriptRecv
  */
void ScriptRecvCtl(CDV_INT32U addr , CDV_INT32U len) {
	OS_ERR err;
	CDV_INT08U start = 0 ;
	//CDV_INT32U cnt = 1 ;
	CDV_INT32U lastRxPos = 0;
	CDV_INT32U startTime ,endTime , time;
	if(0 == addr || 0 == len)
		return;
	ScriptRecvInit(addr , len);
	OS_TaskSuspend((OS_TCB*)&CdvRefreshTaskTCB,&err);
	
	while(1) {//检测是否用户开启了FPGA程序下载到flash的拨码开关
		//  ASSERT(g_scriptRecv.doPos < QUE_NUM);
	  if(g_scriptRecv.tmpLen + g_scriptRecv.len[g_scriptRecv.doPos] >=  g_scriptRecv.totalLen) {//拨码开关拨到了停止下载FPGA程序的位置			
			if (0 != g_scriptRecv.len[g_scriptRecv.doPos]) {
				SPI_Flash_Write(g_scriptRecv.buf[g_scriptRecv.doPos], g_scriptRecv.tmpLen + g_scriptRecv.addr, g_scriptRecv.totalLen - g_scriptRecv.tmpLen);
				g_scriptRecv.tmpLen += g_scriptRecv.len[g_scriptRecv.doPos];//可以比较一下有没有接多
			}
			break;
		} else if(SRP_QUE_HAD) {
			SPI_Flash_Write(g_scriptRecv.buf[g_scriptRecv.doPos], g_scriptRecv.tmpLen + g_scriptRecv.addr, g_scriptRecv.len[g_scriptRecv.doPos]);
			//0.22s
			g_scriptRecv.tmpLen += g_scriptRecv.len[g_scriptRecv.doPos];
			g_scriptRecv.len[g_scriptRecv.doPos] = 0;//队列缓存长度清0
			MAX_SELF_ADD(g_scriptRecv.doPos, QUE_NUM);
		} else if(start){
			if(g_scriptRecv.len[g_scriptRecv.rxPos] != lastRxPos) {
				lastRxPos = g_scriptRecv.len[g_scriptRecv.rxPos];
				//cnt = 1;
				startTime = GetCdvTimeTick();
			} else {//超时判断
				//cnt ++;
				endTime = GetCdvTimeTick();
		    time = CalcTimeMS(endTime , startTime);
//				if (cnt == 0x500000 )
//					break;
				if (time >  1000)
					break;
			}
		} else if( 0 == start){
			if(0 !=g_scriptRecv.len[g_scriptRecv.rxPos]) {
				start = 1;
			}
		}
	}
	
	OSTaskResume((OS_TCB*)&CdvRefreshTaskTCB,&err);
	ScriptRecvDeinit();
	
}

/**
  * @brief  接收脚本crc检验，并输出给串口
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void ScriptCrcChk(CDV_INT32U addr , CDV_INT32U len, CDV_INT08U uartNo) {
	CDV_INT08U ch;
	CDV_INT16U crc = 0xFFFF , tmpCrc;
	CDV_INT32U i ;
	
	if(0 == addr || len <= 2)
		return ;
	
	for(i = 0; i< len - 2; i++) {
		SPI_Flash_Read(&ch, i + addr, 1);
		crc = oneCRC16(ch, crc); 
	}
	
	SPI_Flash_Read(&ch, i + addr, 1);
	tmpCrc = ch;
	SPI_Flash_Read(&ch, i+1 + addr, 1);
	tmpCrc |= ch<<8;
	
	if(crc == tmpCrc) {
		ModbusRequest(65,00,uartNo);
		CDVParamInit();
	} else {
		ModbusRequest(65,02,uartNo);
	}
}


/**/
void UsartTxQueueDoNext(void){
//	DELETE(g_uartTx.QUEUE.txBuf[g_uartTx.doPos]);
//	g_uartTx.QUEUE.txLen[g_uartTx.doPos]=0;
	if(g_uartTx.txPos!=g_uartTx.doPos) {MAX_SELF_ADD(g_uartTx.doPos,USART_TX_QUEUE_LENGTH);}
}

void UsartTxAddWithLen(CDV_INT16U len) {
//	if(g_uartTx.QUEUE.txBuf[g_uartTx.txPos]!= NULL){
//		free(g_uartTx.QUEUE.txBuf[g_uartTx.txPos]);
//		g_uartTx.QUEUE.txBuf[g_uartTx.txPos] = NULL;
//	}
//	
//	if(g_uartTx.QUEUE.txBuf[g_uartTx.txPos]!= NULL)
//		NewError();
	if(len == 0)
		NewError();
	
//	g_uartTx.QUEUE.txBuf[g_uartTx.txPos] = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(len));
//	if(g_uartTx.QUEUE.txBuf[g_uartTx.txPos]== NULL)
//		NewError();
    NEW08U(g_uartTx.QUEUE.txBuf[g_uartTx.txPos],len);
	g_uartTx.QUEUE.txLen[g_uartTx.txPos] = (len);
}

