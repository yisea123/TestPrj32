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

const unsigned char chCRCHTalbe[] =                                 // CRC ��λ�ֽ�ֵ��
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

const unsigned char chCRCLTalbe[] =                                 // CRC ��λ�ֽ�ֵ��
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
        unsigned char chCRCHi = (crc >> 8) & 0xFF; // ��CRC�ֽڳ�ʼ��
        unsigned char chCRCLo = crc & 0xFF; // ��CRC�ֽڳ�ʼ��
        unsigned short wIndex;            // CRCѭ���е�����

        while (wDataLen--)
        {
                // ����CRC
                wIndex = chCRCLo ^ *pchMsg++ ;
                chCRCLo = chCRCHi ^ chCRCHTalbe[wIndex];
                chCRCHi = chCRCLTalbe[wIndex] ;
        }

        return ((chCRCHi << 8) | chCRCLo) ;
}
   
/**
  * @brief  ����FPGA����Ļ�����ʼ��
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   ʹ�ô��ڷ��ͻ����������գ����ڳ�������֮ǰ���У���ΪҪ���ٺܴ���ڴ�
  */
void FpgaRecvInit(void) {
	CDV_INT08U i;
	g_scriptRecv.doPos = 0;//flash����Ķ���
	g_scriptRecv.rxPos = 0;//���ڽ��յĶ���
	
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
  * @brief  ����FPGA����Ļ����ͷ�
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void FpgaRecvDeinit(void) {
		CDV_INT08U i;
	g_scriptRecv.doPos = 0;//flash����Ķ���
	g_scriptRecv.rxPos = 0;//���ڽ��յĶ���
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
  * @brief  ����FPGA����Ŀ���
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   ʹ��g_uartTx.QUEUE.txBuf��Ϊ���л��棬ʹ��g_modbusReg.reg��Ϊ�����н��ջ��泤��
  */
int ononononon = 0;
void FpgaRecvCtl(void) {
  /*������*/
	
//	SPEED_FLASH;
	if (1 == ononononon/*DIP_ON == READ_DIP_SW(1)*/) {//���ص������
		ScriptRecvCtl(FPGA_MOTOR , FPGA_LEN);
	} else if (2 == ononononon/*DIP_ON == READ_DIP_SW(2)*/) {//����IO����
		ScriptRecvCtl(FPGA_IO , FPGA_LEN);
	}
#ifndef	CDV_V1
    /*FPGA��������*/
	g_cdvStat = CDV_FPGA_INIT;
	
	//if(DIP_OFF == READ_DIP_SW(1) && DIP_OFF == READ_DIP_SW(2)/* && DIP_OFF == READ_DIP_SW(0)*/) {
	/*������Ӷ�ȡfpga������Ϣ��Ȼ����в�ͬ������*/
	#if ENABLE_FPGA_DOWN
		CH_CDV_DIP = 0x03;/*��λΪ��1��FPGA��1���ص���FPGA_MOTOR*/
		FpgaInit(CH_CDV_DIP);
	#endif
//	}
#endif
	
//	SPEED_FPGA;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USART_CACHE g_cacheTx = {NULL , 0  , 0 , 0};
OLCMD_CACHE g_olCache = {NULL , 0  , 0 , 0 , NULL};
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
  * @brief  ��CDV��һЩ��Ϣͨ�����ڷ��ͳ�ȥ
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void CDVInfoSend(CMD_ARG *arg) {
	CDV_INT08U i , no;
	char tmp[50]={0};
#ifdef __ENABLE_RTC
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
//	
	#endif
	
	
	AddTxNoCrcPlus(
	#define INFO_STR "\r\n"
	INFO_STR, strlen(INFO_STR), arg);
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
	AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
#if USE_CASCADE == 1u
	//if(slaveTableLen)
	{
		sprintf(tmp , "����id:%d\r\nversion:%d.%d.%d.%d\r\n" 
		,CascadeGetNativeNo(),version[1],version[2],version[3],version[4]
		);
		AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	}
#endif
	if(g_line.init)
	{
		sprintf(tmp , "�����߳�ʼ���ɹ�\r\n" );
	}
	else
	{
		sprintf(tmp , "�����߳�ʼ��ʧ��\r\n" );
	}
	AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	
	for(i = 0; i < WORKER_NUM ; i++) {
		if (IsTaskExist(WorkerTaskTCB[i])) {
			sprintf(tmp,"thread%d-worker%d:step:%d exitStep:%d status:%s\r\n", i, g_threadInfo[i].which, g_threadInfo[i].step, g_threadInfo[i].exitStep, WorkerStatusName[g_threadInfo[i].status]);
	        AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
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
  * @brief  ��CDV�ĵ�ǰ״̬ͨ�����ڷ��ͳ�ȥ
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void CDVUsartSend( CMD_ARG *arg) {
	int i, j;
	char *tmp=NULL;//[USART_RX_BUF_LENGTH]={0};
	NEWCH(tmp,USART_RX_BUF_LENGTH );
	for (i = 0; i < USART_RX_QUEUE_LENGTH; i++)
	{
		memset(tmp, 0, USART_RX_BUF_LENGTH);
		
		for (j = 0; j < g_uartRx.QUEUE.rxLen[i] && j < USART_RX_BUF_LENGTH; j++)
		{
			sprintf(tmp + 3*j , "%x " , g_uartRx.QUEUE.rxBuf[i][j]);
		}
		
		AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
		AddTxNoCrcPlus("\r\n", 2, arg);
	}
	
	memset(tmp, 0, USART_RX_BUF_LENGTH);
	sprintf(tmp + 3*j , "doPos:%d\r\nrxPos:%d\r\n" , g_uartRx.doPos, g_uartRx.rxPos);
	AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	DELETE(tmp);
}

/**
  * @brief  ��CDV�ļ�ʱͨ�����ڷ��ͳ�ȥ
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */

#ifdef  DEBUG_TIME
void NPCTimeSend( CMD_ARG *arg) {
	int i, j;
	char *tmp=NULL;//[USART_RX_BUF_LENGTH]={0};
	NEWCH(tmp,50 );

		memset(tmp, 0, 50);
	sprintf(tmp , "\r\ntime_cascade_map:%d00us\r\n" , time_cascade_map);
		AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	
	memset(tmp, 0, 50);
	sprintf(tmp , "\r\nwhich worker:%d\r\n" , time_which_worker);
	AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	
	for (i = 0; i < 100; i++)
	{
		memset(tmp, 0, 50);
		sprintf(tmp , "%d\t%d00us\r\n" , i, time_worker[i]);
		AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
	}
	
	
	DELETE(tmp);
}
#endif
/**
  * @brief  ���������������������
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxlen       �������ַ����ĳ���
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
			20160107����Ӵ��ں�
//����ʹ��addtx�ȣ������workermanage�������������ᵼ������
  */

RET_STATUS RecvParse(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CDV_INT08U uartNo, void *conn)
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
		/***********************ͨ�ô���******************************/
//		if(general_serial_count != 0 && 3 == uartNo)
//		{
//			ret = ResParse(rxBuf+ 2 , rxLen - 4 ,NULL , 2, uartNo);
//		}	
//		/*************************************************************/
//		else 
		if(0 == OnlineCmdCache(rxBuf , rxLen-2, uartNo, conn)) {
			//ChangeCdvStat(rxBuf , rxLen, uartNo);
			ret = OPT_FAILURE;
  	}
	} 
	else/* if (MAIN_COM == uartNo)*/
	{
		CMD_ARG arg;
		CmdArgInit(&arg);
		arg.uart = uartNo;
		arg.arg = conn;
		//�ж��Ƿ�ATָ��
		//AddTx(rxBuf , rxLen-2);
		if(0 == strncmp((CDV_INT08C*)rxBuf,"CDV INF",7))//ֻ����UCOS�̻߳��ܵ��ȵ�ʱ���ѯ
		{
			//Log_Write("Get inf" , LOG_EVENT);
			CDVInfoSend(&arg);
			
#if USE_NPC_NET == 1u
			EthInfoSend(&arg);
#endif
		}
//		else if(0 == strncmp((CDV_INT08C*)rxBuf,"CDV TIM",7)) {//���̼�ʱ
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
#ifdef  DEBUG_TIME
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"SET TIME",8)){//���ü�صĹ��˺� SET TIME 00
			int i;
			char tmp[50]={0};
			
			time_which_worker = atoi((CDV_INT08C*)rxBuf+9);
			
			sprintf(tmp, "set which worker = %d\r\n", time_which_worker);
			AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), &arg);
					
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"GET TIME",8)){//��ü���ֵ
			NPCTimeSend(&arg);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"CLEAR LOG",9)){//��ü���ֵ
			//Log_Clear();
			time_log_clear();
			AddTxNoCrcPlus("Log Clear success\r\n", 19, &arg);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"GET LOG",7)){//��ü���ֵ
			//Log_Send(&arg);
			time_log_send(&arg);
		}
#endif
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"GET USART",9)){
			CDVUsartSend(&arg);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"LINE START",10)){
			//AllWorkerCtrl(WORKER_LOOP);
			ManagerControl(WORKER_ONCE);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"LINE STOP",9)){
			//AllWorkerCtrl(WORKER_STOP);
			ManagerControl(WORKER_STOP);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"CDV RESET",9)){
			ResetCdv();
#ifdef __ENABLE_RTC
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"SETDATA",7)){//SETDATA 00 00 00
			//RTC_Set_Time(0,0,0,RTC_H12_AM);	//����ʱ��
		  RTC_Set_Date(atoi((CDV_INT08C*)rxBuf+8),atoi((CDV_INT08C*)rxBuf+11),atoi((CDV_INT08C*)rxBuf+14),1);		//��������
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf,"SETTIME",7)){//SETTIME 00 00 00
			RTC_Set_Time(atoi((CDV_INT08C*)rxBuf+8),atoi((CDV_INT08C*)rxBuf+11),atoi((CDV_INT08C*)rxBuf+14),RTC_H12_AM);	//����ʱ��
#endif
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf, "START TEST", 10)){//���Ա�������
			RPressureCnt = 0;
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf, "GET TEST", 8)){//���Ա�������
			int i;
			char tmp[50]={0};
			if(RPressureCnt >= 100){
				for(i = 0 ; i < 100 ; i++) {
					sprintf(tmp, "%lf\r\n", RPressureData[i]);
					AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), &arg);
				}
			}else{
				sprintf(tmp, "%s\r\n", "not ready");
				AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), &arg);
			}
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf, "SET NET", 7)){//SET NET 192.168.1.1:60000
			CDV_INT32S i = 0;
			CDV_CHAR ip[10]={0};
			CDV_INT16U port = 0;
			CDV_CHAR* chr = (CDV_CHAR*)rxBuf + 7;
			int n;
			//��ȡip
			while(chr && i < 10) {
				n = atoi(++chr);
				
				if(n > 255)
					break;
				
				ip[i++] = n;
				chr = strchr(chr, '.');
			}
			//��ȡport
			chr = (CDV_CHAR*)rxBuf + 7;
			if(i == 4) {
				chr = strchr(chr, ':');
				n = atoi(++chr);
				
				if(n < 65536) {
				  port = n;
					MemCpy(ip + 4, &port, 2);
					i += 2;
				  SPI_Flash_Write((CDV_INT08U *)&ip, NET_ADDR, 6);
				  AddTxNoCrcPlus("SUCCESS", 7, &arg);
	        return ret;
				}
			}
		  AddTxNoCrcPlus("FAILURE", 7, &arg);
		}
		else if(0 == strncmp((CDV_INT08C*)rxBuf, "SET NPC", 7)){//SET NPC 0:192.168.1.1:60000
			CDV_INT32S i = 0;
			CDV_CHAR ip[10]={0};
			CDV_INT16U port = 0;
			CDV_CHAR* chr = (CDV_CHAR*)rxBuf + 7;
			CDV_CHAR* tmp_chr = chr;
			int n, id;
			//��ȡid
			chr = strchr(chr, ':');
			if(chr && chr - (CDV_CHAR*)rxBuf > 8) {
			  id = atoi((CDV_CHAR*)rxBuf + 8);
			}
			//��ȡip
			while(chr && i < 10) {
			  tmp_chr = chr;
				n = atoi(++chr);
				
				if(n > 255)
					break;
				
				ip[i++] = n;
				chr = strchr(chr, '.');
			}
			//��ȡport
			chr = tmp_chr;
			if(chr && i == 4) {
				chr = strchr(chr, ':');
				n = atoi(++chr);
				
				if(n >= 0 && n < 65536 && id >= 0 && id < 256) {
					CDV_INT08U tmp = 5;//������������ã�����Ĭ����0xFF
					SPI_Flash_Write((CDV_INT08U *)&tmp, CHECK_VERION_ADDR1, 1);
					SPI_Flash_Write((CDV_INT08U *)&id, CHECK_VERION_ADDR, 1);
				  port = n;
					MemCpy(ip + 4, &port, 2);
					i += 2;
				  SPI_Flash_Write((CDV_INT08U *)&ip, NET_ADDR, 6);
				  AddTxNoCrcPlus("SUCCESS", 7, &arg);
	        return ret;
				}
			}
		  AddTxNoCrcPlus("FAILURE", 7, &arg);
#if USE_NPC_NET == 1u
		} else if (0 == strncmp((CDV_INT08C*)rxBuf, "NPC FIND", 8)) {
			udpecho_find(&arg);
#endif
		}	else if(0 == strncmp((CDV_INT08C*)rxBuf,"MEM INF",7)){//�鿴�ڴ�ʹ��
			int used, perused;
			char tmp[50]={0};
			
			used = memmng_used();
		  perused = memmng_perused();
			
			sprintf(tmp, "used %d\r\nperused %d\%%\r\n", used, perused);
			AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), &arg);
					
		} else {///�ظ�������
		  //OnlineRequest(rxBuf[1], 0xFF, 0xFF, uartNo);
	  }
		CmdArgDelete(&arg);
	}
//	LED3 = LED_OFF;
	return ret;
}

/**
  * @brief  �����ַ�����CRCֵ
  *  
  * @param  ptr     �ַ�����ָ��
  *         len     �ַ����ĳ���      
  *   
  * @retval CRCֵ
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
  * @brief  �۴μ����ַ�����CRCֵ
  *  
  * @param  ptr     �ַ�����ָ��
  *         len     �ַ����ĳ���      
  *   
  * @retval CRCֵ
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
  * @brief  ���ַ�������modbus����
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         len       �������ַ����ĳ���
  *         txBuff    Ӧ���ַ���  
  *   
  * @retval ����ֵ˵��
  *
  * @note   RecvParse��
  */

RET_STATUS OnlineParse(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg){	

	//OS_ERR err;
	RET_STATUS ret;
	switch(rxBuf[1]){ //������ɸѡ
		case 0x64://�ű�����
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
		case 0x41://��չ����------CDV����
			ASSERT(!arg->ptrWorker);//ȷ�������ڹ���������
			OperateScript(rxBuf,rxLen, arg);
			break;
		case 0xfe://�Զ����
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
  * @brief  modbusӦ�����rx��Ӧ�𳤶����ͷ��crc
  *  
  * @param  rxBuf     �����ַ���ָ��
  *         txLen     Ӧ���ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ʹ�õ�ȫ�ֱ���,��������
  *         ���Ҫ���Զ����ַ������CRC��rxbuf�����Զ����ַ�������
  */
void RequestAdd(CDV_INT08U* rxBuf,CDV_INT16U txLen){
	CDV_INT16U crc;
	USART_TX_BUF(0) = rxBuf[0];
	USART_TX_BUF(1) = rxBuf[1];
	
	crc = getCRC16(USART_TX_BUF_ADDR,txLen-2);
	USART_TX_BUF(txLen-2) = crc & 0xff; 
	USART_TX_BUF(txLen-1) = (crc >> 8) & 0xff;
}


/**
  * @brief  modbus�����ű�
  *         65
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   �����ͺ������˺�����main.c��usart_task()������stm32f4xx_it.c��USART1_IRQHandler()�й���
  */
void OperateScript(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CMD_ARG *arg){
	CDV_INT08U len = 0 , *txBuf = NULL ,version;//nameLen ,
	CDV_INT16U crc;

//	OS_ERR err;
	//u8 a[2] = {0};
//	CPU_SR_ALLOC();
	switch((rxBuf[2]<<8) + rxBuf[3]){
		case 0x0000://����������
			
//			if(rxLen < 10)//������ȱ�����ڵ���10
//				return;
//			g_scriptInfo.no = rxBuf[4];
//			if(g_scriptInfo.no >= SRP_NUM_RUN)
//				return;
		  
		  //ManagerControl(WORKER_STOP);
			#if USE_WORKER_DEBUG == 1u
			AllDebugStateSet(DEBUG_DISABLE);//�ȹص�����Ȼ�˲���
			#endif
			
			AllWorkerCtrl(WORKER_STOP);
		
			
			g_scriptInfo.addr = __LINE_ADDR;
//			nameLen = rxBuf[5];//�ű����Ƴ���
			g_scriptInfo.len = (rxBuf[7]<<24) + (rxBuf[6]<<16) + (rxBuf[5]<<8) + rxBuf[4] + 2;//�����߳���
			SPI_Flash_Write((CDV_INT08U *)&g_scriptInfo.len, __LINE_LEN, 4);
//			SPI_Flash_Write(rxBuf,SCRIP_NAME(g_scriptInfo.no),(CDV_INT16U)rxLen);//�洢���������������ʶ����Ч����
			
//			if(nameLen > rxLen- 8)//���Ƴ��Ȳ���
//				return;
			if(g_scriptInfo.len >= LINE_MAX_LEN || 2 >= g_scriptInfo.len)//����У��λ
				return;
			
//			SPEED_FLASH;
		  g_scriptInfo.tmpLen = 0;
			DELETE(g_scriptInfo.buf);
			
			len = rxLen;//tx�ܳ���=rx
			//delay_ms(900);
			AddTxPlus(rxBuf , len-2, arg);//�ظ���λ��������¼��
			
			ScriptRecvCtl(g_scriptInfo.addr , g_scriptInfo.len);
			ScriptCrcChk(g_scriptInfo.addr , g_scriptInfo.len, arg);
			
			//���°汾��
			SPI_Flash_Read((CDV_INT08U*)&version, VERSION_ADDR, 1);//��ȡ�汾��
			version++;
			SPI_Flash_Write((CDV_INT08U *)&version,VERSION_ADDR,1);
			
//			SPEED_FPGA;
			
			break;
		case 0x0001://�ű���
//			g_scriptInfo.no = rxBuf[4];
//		    SPI_Flash_Read(&len,SCRIP_NAME_LEN(g_scriptInfo.no),1);//��ȡ�ű�������
//			
//		
//			if(len < 10)//������ȱ�����ڵ���10
//				return;
				
			NEW08U(txBuf , 10);
			MemCpy(txBuf , rxBuf, 4);
//			SPI_Flash_Read(txBuf , SCRIP_NAME(g_scriptInfo.no) , len);		
//			crc = getCRC16(txBuf,len-2); 
//			
//			if((txBuf[len-1] != ((crc >> 8) & 0xff))||(txBuf[len-2] != (crc & 0xff))) {//У��洢�������crc
//				DELETE(txBuf);
//				return;
//			}
			SPI_Flash_Read((CDV_INT08U *)&g_scriptInfo.len, __LINE_LEN, 4);
			txBuf[4] = g_scriptInfo.len;
			txBuf[5] = g_scriptInfo.len >> 8;
			txBuf[6] = g_scriptInfo.len >> 16;
			txBuf[7] = g_scriptInfo.len >> 24;
		
			AddTxPlus(txBuf , 8, arg);
			delay_ms(50);
			
		  g_scriptInfo.addr = __LINE_ADDR;
			
		  g_scriptInfo.tmpLen = 0;
			
			Flash_Send(g_scriptInfo.addr, g_scriptInfo.len, arg);
					
			DELETE(txBuf);
			break;		
case 0x0002://ѹ������
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
			AddTxPlus(txBuf , arg->len + 4, arg);
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
		  
			
		  
			
			Flash_Send(g_scriptInfo.addr, g_scriptInfo.len - 2, arg);

      AddTxNoCrcPlus((CDV_INT08U*)&crc ,2 ,arg );
				
			break;		
//		case 0x0003://���õ��Խű�����
//			len = (rxBuf[4]<<8) + rxBuf[5];
//		
//		    if(len <= SCRIP_MAX_DEBUG) {
//				SRP_NUM_DEBUG = len;
//			    SPI_Flash_Write((CDV_INT08U *)&SRP_NUM_DEBUG,SCRIP_NUM_DEBUG,2);
//				len = rxLen;//tx�ܳ���=rx ԭ��Ӧ��
//				AddTx(rxBuf , len-2, uartNo);
////				MODBUS_SET_SAVE_REG;
////				MODBUS_RESET_SAVE_INREG;
////				USART_TX_ADD_WITH_LEN(len);//���ٿռ�
////				memcpy(USART_TX_BUF_ADDR , rxBuf , len);
////				USART_TX_QUEUE_SELF_ADD;		
//			}			
//			
//			break;
			
		case 0x0004://INIд
			if(rxLen < 7)//������ȱ�����ڵ���7
				return;
			g_scriptInfo.no = rxBuf[4];
			g_scriptInfo.addr = INI_ADDR(g_scriptInfo.no);
			g_scriptInfo.len = (rxBuf[5]<<8) + rxBuf[6];//����
			SPI_Flash_Write((CDV_INT08U *)&g_scriptInfo.len,INI_LEN_ADDR(g_scriptInfo.no),2);
			
			if(0 >= g_scriptInfo.len)//
				return;
				
			AddTxPlus(rxBuf , rxLen-2, arg);//�ظ���λ��������¼��
			
			ScriptRecvCtl(g_scriptInfo.addr , g_scriptInfo.len);
			break;
		case 0x0005://INI��
			g_scriptInfo.no = rxBuf[4];
		    SPI_Flash_Read((CDV_INT08U *)&g_scriptInfo.len,INI_LEN_ADDR(g_scriptInfo.no),2);
			
			if(g_scriptInfo.len < 10)//������ȱ�����ڵ���10
				return;
				
			NEW08U(txBuf , 7);
			MemCpy(txBuf , rxBuf , 5);
			txBuf[5] = (CDV_INT08U)(g_scriptInfo.len>>8);
			txBuf[6] = (CDV_INT08U)(g_scriptInfo.len);
			AddTxPlus(txBuf , 7, arg);
			
			g_scriptInfo.addr = INI_ADDR(g_scriptInfo.no);
			
			Flash_Send(g_scriptInfo.addr, g_scriptInfo.len, arg);
					
			DELETE(txBuf);
			break;
		case 0x0006://д�汾��
		  SPI_Flash_Write((CDV_INT08U *)&rxBuf[4],VERSION_ADDR,1);
			break;
//		case 0x0007://д����������
//			if(rxBuf[4] < 0xff)
//		    SPI_Flash_Write(rxBuf + 4,SCRIP_LINE_NAME,rxBuf[4] + 1);
//			break;
		case 0x0010://FPGA�ű�����
			AddTxPlus(rxBuf , rxLen-2, arg);
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
		  AddTxPlus(rxBuf , rxLen-2, arg);
			break;
		case 0x0011://FPGA�ű��ϴ�
      Flash_Send(FPGA_MOTOR , FPGA_LEN, arg);
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
  * @brief  ������������ظ�
  *         
  *  
  * @param  no     ������
  *         err     ������
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   û���ã�������
  */
//void OnlineRequest(CDV_INT08U no,CDV_INT08U res,CDV_INT08U resNo, CDV_INT08U uartNo) {
//	CDV_INT08U len , *txBuf =NULL;
//	len = 4;
//	NEW08U(txBuf , len);
//	txBuf[0] = CDV_ID;
//	txBuf[1] = no+0x80;
//	txBuf[2] = res;
//	txBuf[3] = resNo;
//	AddTx(txBuf , len , uartNo);
//	DELETE(txBuf);
//}
/**
  * @brief  modbus���������ظ�
  *         
  *  
  * @param  no     ������
  *         err     ������
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
void ModbusRequest(CDV_INT08U no,CDV_INT08U errNo, CMD_ARG *arg) {
	CDV_INT08U len , *txBuf =NULL;
//	OS_ERR err;
	switch(no) {
//		case 00:
//			len = 3;//tx�ܳ���=5
//		  txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*len);
//		  txBuf[0] = CDV_ID;
//			txBuf[1] = 0x00+0x80;
//			txBuf[2] = errNo;
//		  
////		len = 5;//tx�ܳ���=5
////		USART_TX_ADD_WITH_LEN(len);//���ٿռ�
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
			len = 6;//tx�ܳ���=8
			//txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*len);
			NEW08U(txBuf , len);
		  txBuf[0] = CDV_ID;
			txBuf[1] = 0x41+0x80;
			txBuf[2] = 00;
		  txBuf[3] = 00;
		 // txBuf[4] = g_scriptInfo.no;
		  txBuf[5] = errNo;
//			len = 8;//tx�ܳ���=8
//		USART_TX_ADD_WITH_LEN(len);//���ٿռ�
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
			len = 3;//tx�ܳ���=5
		  //txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*len);
		  NEW08U(txBuf , len);
		  txBuf[0] = CDV_ID;
			txBuf[1] = no+0x80;
			txBuf[2] = errNo;
			break;
	}
	AddTxPlus(txBuf , len , arg);
	DELETE(txBuf);
}

/**
  * @brief  modbus���������ظ�
  *         
  *  
  * @param  no     ������
  *         err     ������
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
void ModbusRequestPlus(CDV_INT08U no,CDV_INT08U errNo, CMD_ARG *arg) {
	CDV_INT08U len , *txBuf =NULL;
	CDV_INT16U crc;
	switch(no) {
		case 65:
			len = 6;//tx�ܳ���=8
			NEW08U(txBuf , len + 2);
		  txBuf[0] = CDV_ID;
			txBuf[1] = 0x41+0x80;
			txBuf[2] = 00;
		  txBuf[3] = 00;
		  txBuf[5] = errNo;
			break;
		default:
			len = 3;//tx�ܳ���=5
		  NEW08U(txBuf , len + 2);
		  txBuf[0] = CDV_ID;
			txBuf[1] = no+0x80;
			txBuf[2] = errNo;
			break;
	}
	//AddTx(txBuf , len , uartNo);
	crc = MODBUS_CRC16(txBuf , len, 0xFFFF);
	MemCpy(txBuf + len, &crc, 2);
	AddTxNoCrcPlus(txBuf , len+2 , arg);
	DELETE(txBuf);
}
/**
  * @brief  �Ƚ�do�ķ����ַ����ͽ����ַ����Ƿ�һ��
  *         
  *  
  * @param  no     ������
  *         err     ������
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ����CDV_START״̬�������������õ�����ظ��Ƚ�
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
  * @brief  ������Ҫ�ظ��ķ���ָ��
  *         
  *  
  * @param  no     ������
  *         err     ������
  *           
  *   
  * @retval ����ֵ˵��
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
  * @brief  ���ڷ���cache
  *         
  *  
  * @param  rxBuf     ��������
  *         rxLen     ���ճ���
  *           
  *   
  * @retval ����ֵ˵��0ʧ��1�ɹ�
  *
  * @note   
  */
CDV_INT08U OnlineCmdCache(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CDV_INT08U uartNo, void *arg) {
	OS_ERR err;
	
	ASSERT(rxBuf && rxLen);
	if(rxBuf == NULL || rxLen == 0)
		return 0;
	
//	while(g_olCache.mark)
//		delay_ms(3);//��δ�����
	
	if(g_olCache.mark)//��δ�����
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
		g_olCache.arg = arg;
	}else{
	  g_olCache.len = 0;//Ӧ�û�������Ĵ��󾯸�
		return 0;
	}
	
	g_olCache.mark = 1;
	// OSTaskResume((OS_TCB*)&TaskParseTCB,&err);		// ��recv��������
	return 1;
}
/**
  * @brief  ����������cache
  *         
  *  
  * @param  
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
CDV_INT08U ClearOnlineCmdCache(void) {
	DELETE(g_olCache.buf);
	g_olCache.len = 0;
	g_olCache.uart = 0;
	g_olCache.arg = NULL;
	g_olCache.mark = 0;		
	return 1;
}

/** @brief  �����ڷ�������
  * @param  
  * @retval 
  * @note   
  */
RET_STATUS MAINUSART_Send(CDV_INT08U* txBuf, CDV_INT16U txLen){  
	OS_ERR err;
	//CDV_INT08U len = txLen;
	
	while(!USART_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
		//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
		return OPT_FAILURE;
	}

	OSMutexPend(&TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	if(err == OS_ERR_TIMEOUT)
		return OPT_FAILURE;
	
	if(USART_CAN_DO){
		USART_TX_ADD_WITH_LEN(txLen);//���ٿռ�	
		MemCpy(USART_TX_BUF_ADDR, txBuf , txLen);	
		USART_TX_QUEUE_SELF_ADD;			
	}
	
	OSMutexPost (&TX_SEM,OS_OPT_POST_NO_SCHED,&err);
	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	
	return OPT_SUCCESS;
}
/** @brief  �����ڷ�������
  * @param  
  * @retval 
  * @note   
  */
RET_STATUS MAINUSART_SendEx(CDV_INT08U* txBuf, CDV_INT16U txLen, CDV_INT08U* exBuf, CDV_INT16U exLen){  
	OS_ERR err;
	//CDV_INT08U len = txLen;
	
	while(!USART_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
		//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
		return OPT_FAILURE;
	}

	OSMutexPend(&TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	if(err == OS_ERR_TIMEOUT)
		return OPT_FAILURE;
	
	if(USART_CAN_DO){
		USART_TX_ADD_WITH_LEN(txLen + exLen);//���ٿռ�	
		MemCpy(USART_TX_BUF_ADDR, txBuf , txLen);	
		MemCpy(USART_TX_BUF_ADDR + txLen, exBuf , exLen);	
		USART_TX_QUEUE_SELF_ADD;			
	}
	
	OSMutexPost (&TX_SEM,OS_OPT_POST_NO_SCHED,&err);
	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	
	return OPT_SUCCESS;
}

/**
  * @brief  ����Ҫ�ظ��Ĵ��������׼������
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
void AddTxNoCrcPlus(CDV_INT08U* txBuf, CDV_INT16U txLen, CMD_ARG *arg) {
	CDV_INT08U txRealLen;
	CDV_INT08U *TX_BUF= NULL;
	CDV_INT08U TX_Head[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	CDV_INT08U uartNo = arg->uart;
	
	if(NULL == txBuf || 0 == txLen || 0xFF == uartNo)
		return;
	
	if(txBuf[0] != 0xF2){
		TX_BUF = txBuf;
	}else{//��ͷ
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
		while (OPT_FAILURE == TCP_ServerSendPlus(TX_BUF, txLen, arg));//TCP_ServerSend(txBuf, txLen);
	}
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
}

/**
  * @brief  ����Ҫ�ظ��Ĵ��������׼������
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   �һ��ᶼ�ĳ� AddTxNoCrcPlus
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
	}else{//��ͷ
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
	
//	while(!USART_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
//		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
//	}
//	
//	OSSemPend(&TX_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
//	if(USART_CAN_DO){
//		
//		USART_TX_ADD_WITH_LEN(len);//���ٿռ�	
//		memcpy(USART_TX_BUF_ADDR, txBuf , txLen);		
////		RequestAdd(USART_TX_BUF_ADDR, len);
//		USART_TX_QUEUE_SELF_ADD;			
//		
//	//} else {
//		//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
//	}
//	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
//	//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ//��ֹ8������һ����//�������Ϊ����add��ʾ��������������ӵ�
//	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	
}
/**
  * @brief  ����Ҫ�ظ��Ĵ��������׼������
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
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
//	while(!USART_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
//		//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
//		return;
//	}
//	
//	OSSemPend(&TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
//	if(err == OS_ERR_TIMEOUT)
//		return;
//	if(USART_CAN_DO){
//		
//		USART_TX_ADD_WITH_LEN(len);//���ٿռ�	
//		memcpy(USART_TX_BUF_ADDR, txBuf , txLen);		
//		RequestAdd(USART_TX_BUF_ADDR, len);
//		USART_TX_QUEUE_SELF_ADD;			
//		
//	//} else {
//		//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
//	}
//	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
//	//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ//��ֹ8������һ����//�������Ϊ����add��ʾ��������������ӵ�
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
/** @brief  ��crc����
  * @param  
  * @retval 
  * @note   arg��
  */
void AddTxPlus(CDV_INT08U* txBuf, CDV_INT16U txLen, CMD_ARG *arg) {
	CDV_INT16U crc;
	
	CDV_INT16U txRealLen;
	CDV_INT08U *TX_BUF= NULL;
	CDV_INT08U TX_Head[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	CDV_INT08U uartNo = arg->uart;
	
	ASSERT(txBuf && txLen);
	if(NULL == txBuf || 0 == txLen || 0xFF == uartNo)
		return;

	crc = MODBUS_CRC16(txBuf,txLen, 0xFFFF);
	//crc = getCRC16(txBuf,txLen);
	
	
	if(txBuf[0] != 0xF2){
		TX_BUF = txBuf;
	}else{//��ͷ
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
		while (OPT_FAILURE == TCP_ServerSendExPlus(TX_BUF, txLen, (CDV_INT08U*)&crc, 2,arg));
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
/** @brief  ��crc����
  * @param  
  * @retval 
  * @note   ��ͨ��
  */
void AddTx(CDV_INT08U* txBuf, CDV_INT16U txLen, CDV_INT08U uartNo) {
	CDV_INT16U crc;
	
	CDV_INT16U txRealLen;
	CDV_INT08U *TX_BUF= NULL;
	CDV_INT08U TX_Head[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	
	
	ASSERT(txBuf && txLen);
	if(NULL == txBuf || 0 == txLen || 0xFF == uartNo)
		return;

	crc = MODBUS_CRC16(txBuf,txLen, 0xFFFF);
	//crc = getCRC16(txBuf,txLen);
	
	
	if(txBuf[0] != 0xF2){
		TX_BUF = txBuf;
	}else{//��ͷ
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
//void AddTx111(CDV_INT08U* txBuf, CDV_INT08U txLen, CDV_INT08U uartNo) {
//	CDV_INT16U crc;
//	
//	ASSERT(txBuf && txLen);
//	if(NULL == txBuf || 0 == txLen)
//		return;

//	//getCRC16(txBuf,txLen);
//	//crc_ccitt(txBuf,txLen,0xffff);
//	crc = MODBUS_CRC16(txBuf,txLen, 0xFFFF);
//	if(TCP_COM == uartNo)
//	{
////		while (OPT_FAILURE == TCP_ServerSend(txBuf, txLen));
////		while (OPT_FAILURE == TCP_ServerSend((CDV_INT08U*)&crc, 2));
//		while (OPT_FAILURE == TCP_ServerSendEx(txBuf, txLen, (CDV_INT08U*)&crc, 2));
//	}	
////	else if(0xFF == uartNo)
////	{
////		Cascade_Slave_Write(txBuf, txLen);
////	}
//	else if(MAIN_COM == uartNo) //main usart
//	{
////		while (OPT_FAILURE == MAINUSART_Send(txBuf, txLen));
////		while (OPT_FAILURE == MAINUSART_Send((CDV_INT08U*)&crc, 2));
//		while (OPT_FAILURE == MAINUSART_SendEx(txBuf, txLen, (CDV_INT08U*)&crc, 2));
//		
//		
//	}
//	else
//	{
//		USARTSend(txBuf, txLen, uartNo);
//		USARTSend((CDV_INT08U*)&crc, 2, uartNo);
//	}
////	
////	while(!USART_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
////		//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
////		return;
////	}
////	
////	OSSemPend(&TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
////	if(err == OS_ERR_TIMEOUT)
////		return;
////	if(USART_CAN_DO){
////		
////		USART_TX_ADD_WITH_LEN(len);//���ٿռ�	
////		memcpy(USART_TX_BUF_ADDR, txBuf , txLen);		
////		//RequestAdd(USART_TX_BUF_ADDR, len);
////		memcpy(USART_TX_BUF_ADDR + txLen, &crc, 2);
////		USART_TX_QUEUE_SELF_ADD;			
////		
////	//} else {
////		//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
////	}
////	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
////	//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ//��ֹ8������һ����//�������Ϊ����add��ʾ��������������ӵ�
////	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
//	
//}


///**
//  * @brief  ����Ҫ�ظ��Ĵ��������׼������
//  *         
//  *  
//  * @param  txBuf     �����ַ����׵�ַָ��
//  *         txLen     �����ַ�������,������crc
//  *           
//  *   
//  * @retval ����ֵ˵��
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
//	while(!USART_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
//		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
//	}
//	
//	OSSemPend(&TX_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
//	if(USART_CAN_DO){
//		USART_TX_ADD_WITH_LEN(len);//���ٿռ�
//		memcpy(USART_TX_BUF_ADDR, txBuf , txLen);
//		RequestAdd(USART_TX_BUF_ADDR, len);
//		while(!NeedRequestTx(uartNo));
//		USART_TX_QUEUE_SELF_ADD;
//	}
//	
//	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
//	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
//	while (IS_TX_NEED_REQUEST);//�ȴ�Ӧ�����
//}
/*****************************************************************�ű�����ר��******************************************************************/
/**
  * @brief  ���սű��Ļ�����ʼ��
  *  
  * @param  addr �����flash��ַ
            len �ű��ĳ���
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
  * @brief  ���սű��Ļ����ͷ�
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
		//g_scriptRecv.buf[i] = NULL;//��Ϊʱ�̶���ַ���������ð�
    g_scriptRecv.len[i] = 0;
	}
	
}


/**
  * @brief  ���սű��Ŀ���
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
	CDV_INT32U time1 ,time2;
	if(0 == addr || 0 == len)
		return;
	ScriptRecvInit(addr , len);
	OS_TaskSuspend((OS_TCB*)&CdvRefreshTaskTCB,&err);
	
	//startTime = GetCdvTimeTick();
	time1 = GetCdvTimeTick();
	
	while(1) {//����Ƿ��û�������FPGA�������ص�flash�Ĳ��뿪��
		//  ASSERT(g_scriptRecv.doPos < QUE_NUM);
	  if(g_scriptRecv.tmpLen + g_scriptRecv.len[g_scriptRecv.doPos] >=  g_scriptRecv.totalLen) {//���뿪�ز�����ֹͣ����FPGA�����λ��			
			if (0 != g_scriptRecv.len[g_scriptRecv.doPos]) {
				SPI_Flash_Write(g_scriptRecv.buf[g_scriptRecv.doPos], g_scriptRecv.tmpLen + g_scriptRecv.addr, g_scriptRecv.totalLen - g_scriptRecv.tmpLen);
				g_scriptRecv.tmpLen += g_scriptRecv.len[g_scriptRecv.doPos];//���ԱȽ�һ����û�нӶ�
			}
			break;
		} else if(SRP_QUE_HAD) {
			SPI_Flash_Write(g_scriptRecv.buf[g_scriptRecv.doPos], g_scriptRecv.tmpLen + g_scriptRecv.addr, g_scriptRecv.len[g_scriptRecv.doPos]);
			//0.22s
			g_scriptRecv.tmpLen += g_scriptRecv.len[g_scriptRecv.doPos];
			g_scriptRecv.len[g_scriptRecv.doPos] = 0;//���л��泤����0
			MAX_SELF_ADD(g_scriptRecv.doPos, QUE_NUM);
		} else if(start){
			if(g_scriptRecv.len[g_scriptRecv.rxPos] != lastRxPos) {
				lastRxPos = g_scriptRecv.len[g_scriptRecv.rxPos];
				//cnt = 1;
				startTime = GetCdvTimeTick();
			} else {//��ʱ�ж�
				//cnt ++;
				endTime = GetCdvTimeTick();
		    time = CalcTimeMS(endTime , startTime);
//				if (cnt == 0x500000 )
//					break;
				if (time >  5000)
					break;
			}
		} else if( 0 == start){ // һ�����ݶ�û�ӵ�
			if(0 !=g_scriptRecv.len[g_scriptRecv.rxPos]) {
				start = 1;
				//startTime = GetCdvTimeTick();
			}else{
//				endTime = GetCdvTimeTick();
//		    time = CalcTimeMS(endTime , startTime);
//				if (time >  10000)
//					break;
		    time2 = CalcTimeMS(GetCdvTimeTick() , time1);
				if (time2 >  5000) // ����5Sû�����ݣ�ֱ���˳�
					break;
			}
		}
	}
	
	OSTaskResume((OS_TCB*)&CdvRefreshTaskTCB,&err);
	ScriptRecvDeinit();
	
}

/**
  * @brief  ���սű�crc���飬�����������
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void ScriptCrcChk(CDV_INT32U addr , CDV_INT32U len, CMD_ARG *arg) {
	CDV_INT08U ch;
	CDV_INT16U crc = 0xFFFF , tmpCrc;
	CDV_INT32U i ;
	CDV_INT08U uartNo = arg->uart;
	
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
		ModbusRequestPlus(65,00,arg);
		//CDVParamInit();
		delay_ms(500);
#if USE_EXTI_POWER_OFF == 1u
		FlashBak_BackUp();
#endif
	} else {
		ModbusRequestPlus(65,02,arg);
	}
		ResetCdv();
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

