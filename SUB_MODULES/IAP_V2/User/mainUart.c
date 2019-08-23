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
  * @brief  ��CDV��һЩ��Ϣͨ�����ڷ��ͳ�ȥ
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
void CDVInfoSend(CDV_INT08U uartNo) {
//	CDV_INT08U i , no;
//	char tmp[50]={0};
//#ifdef __ENABLE_RTC
//	RTC_TimeTypeDef RTC_TimeStruct;
//	RTC_DateTypeDef RTC_DateStruct;
////	
//	#endif
//	
////	sprintf(tmp , "%s\r\n%s%d %d\r\nwifi link:%x\r\n" 
////	,(CDV_INT08U*)cdv_error[g_cdvStat] 
////	,"start/stop:" 
////	,g_run.doNo[0] 
////	,g_run.doNo[1] 
////	,WIFI_LINK
////	);
//	
//	sprintf(tmp , "%s\r\n" 
//	,(CDV_INT08U*)cdv_error[g_cdvStat] 
//	);
//	
//	
//	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
//	for(i = 0; i < WORKER_NUM ; i++) {
//		if (IsTaskExist(WorkerTaskTCB[i])) {
//			sprintf(tmp,"thread%d-worker%d:step:%d exitStep:%d status:%s\r\n", i, g_threadInfo[i].which, g_threadInfo[i].step, g_threadInfo[i].exitStep, WorkerStatusName[g_threadInfo[i].status]);
//	        AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
//		}

//	}
//	
//	AddTxNoCrc(slaveTable, slaveTableLen, uartNo);
//	
//	
//	
////	if (timeEn) {
////		for(no = 0 ; no < g_run.howMany ; no++) {
////			sprintf(tmp,"%s%d\r\n","Worker :", no);
////			AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
////			for(i = 0; i < g_run.cmdNum[no] ; i++) {
////				sprintf(tmp,"%s%d:%d ms\r\n","No.", i , timeMS[no][i]);
////				AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
////			}
////		}
////	}
//	//AddTx((CDV_INT08U*)cdv_error[g_cdvStat] , strlen(cdv_error[g_cdvStat]), uartNo);
//	//AddTx((CDV_INT08U*)g_run.doNo, 20, uartNo);
//	//AddTx((CDV_INT08U*)&err_times1, 1, uartNo);
//	#ifdef __ENABLE_RTC
//	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
//	sprintf(tmp,"Time:%02d:%02d:%02d\r\n",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
//	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
//	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
//	sprintf(tmp,"Date:20%02d-%02d-%02d\r\n",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
//	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
//	sprintf(tmp,"Week:%d\r\n",RTC_DateStruct.RTC_WeekDay); 
//	AddTxNoCrc((CDV_INT08U*)tmp, strlen(tmp), uartNo);
//#endif
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

RET_STATUS RecvParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo) {
	RET_STATUS ret = OPT_SUCCESS;
	CDV_INT08U temp[2]={0,0};
	CDV_INT16U crc;

	crc = getCRC16(rxBuf,rxLen-2); 
	temp[1] = crc & 0xff; 
	temp[0] = (crc >> 8) & 0xff;
	
	if ((rxBuf[rxLen-1] == temp[0])&&(rxBuf[rxLen-2] == temp[1])) {//crc chk
		
    CentralizedControl_Ctrl(rxBuf, rxLen, uartNo);
		
	} else if (rxBuf && rxLen) {
		
		char* pos = strchr((char*)rxBuf,':') + 1;
		int par = atoi(pos);
		int slen = pos - (char*)rxBuf - 1;
			
		if (NULL == pos)
			return OPT_FAILURE;
		
		if (0 == strncmp((CDV_INT08C*)rxBuf,"CDV INF",7)) {//ֻ����UCOS�̻߳��ܵ��ȵ�ʱ���ѯ
			CDVInfoSend(uartNo);			
			EthInfoSend(uartNo);
		}
		else if (0 == strncmp((CDV_INT08C*)rxBuf,"GET USART",9)) {
			CDVUsartSend(uartNo);
		}
		else if (0 == strncmp((CDV_INT08C*)rxBuf,"CDV RESET",9)) {
			u8 in[3] = {0x00, 0X00, 0X00};
			u8 out[3] = {0x01, 0X01, 0X7F};
			WriteToInLed(in, 3);
			WriteToOutLed(out, 3);
			IAP_LoadApp(APPLICATION_ADDRESS);
			ResetCdv();
		}
		else if (0 == strncmp((CDV_INT08C*)rxBuf,"IAP",3)) {//IAP:1 1000
			Main_Menu(*(rxBuf + slen + 1), atoi((CDV_INT08C*)rxBuf + slen + 3), uartNo);
		}
		
	}

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
CDV_INT08U OnlineCmdCache(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CDV_INT08U uartNo) {
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
	    memcpy(g_olCache.buf , rxBuf , g_olCache.len);
		g_olCache.uart = uartNo;
	}else{
	    g_olCache.len = 0;//Ӧ�û�������Ĵ��󾯸�
		return 0;
	}
	
	g_olCache.mark = 1;
	//OSTaskResume((OS_TCB*)&TaskParseTCB,&err);		
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

	OSSemPend(&TX_SEM , 2 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	if(err == OS_ERR_TIMEOUT)
		return OPT_FAILURE;
	
	if(USART_CAN_DO){
		USART_TX_ADD_WITH_LEN(txLen);//���ٿռ�	
		memcpy(USART_TX_BUF_ADDR, txBuf , txLen);	
		USART_TX_QUEUE_SELF_ADD;			
	}
	
	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
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
void AddTxNoCrc(CDV_INT08U* txBuf, CDV_INT16U txLen, CDV_INT08U uartNo) {
	//OS_ERR err;
	//CDV_INT08U len = txLen;
	ASSERT(txBuf && txLen);
	if(NULL == txBuf || 0 == txLen)
		return;
	
	if(0xEE == uartNo)//tcp
	{
		while (OPT_FAILURE == TCP_ServerSend(txBuf, txLen));//TCP_ServerSend(txBuf, txLen);
	}
//	else if(0xFF == uartNo)//spi cascade
//	{
//		Cascade_Slave_Write(txBuf, txLen);
//	}	
	else if(MAIN_COM == uartNo)//main usart
	{
		while (OPT_FAILURE == MAINUSART_Send(txBuf, txLen));
	}
	else
	{
		USARTSend(txBuf, txLen, uartNo);
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
void AddTx(CDV_INT08U* txBuf, CDV_INT08U txLen, CDV_INT08U uartNo) {
	CDV_INT16U crc;
	CDV_INT08U *buf = NULL;
	
	ASSERT(txBuf && txLen);
	if(NULL == txBuf || 0 == txLen)
		return;

	crc = getCRC16(txBuf,txLen);
	//crc_ccitt(txBuf,txLen,0xffff);
	MODBUS_CRC16(txBuf,txLen, 0xFFFF);
	
	NEW08U(buf,txLen+2);
	

	memcpy(buf , txBuf , txLen);
	memcpy(buf + txLen, &crc, 2);
	
	
	if(0xEE == uartNo)
	{
		u32 clk;
		clk = ReadClock100ms();
	
		while (OPT_FAILURE == TCP_ServerSend(buf,txLen+2));
		
		clk = CalcCount(ReadClock100ms(), clk);
		clk = 0;
	}	
//	else if(0xFF == uartNo)
//	{
//		Cascade_Slave_Write(txBuf, txLen);
//	}
	else if(MAIN_COM == uartNo) //main usart
	{
		while (OPT_FAILURE == MAINUSART_Send(buf,txLen+2));
	}
	else
	{
		USARTSend(buf,txLen+2, uartNo);
	}
	
	DELETE(buf);
//	if(0xEE == uartNo)
//	{
//		u32 clk;
//		clk = ReadClock100ms();
//	
//		while (OPT_FAILURE == TCP_ServerSend(txBuf, txLen));
//		while (OPT_FAILURE == TCP_ServerSend((CDV_INT08U*)&crc, 2));
//		
//		clk = CalcCount(ReadClock100ms(), clk);
//		clk = 0;
//	}	
////	else if(0xFF == uartNo)
////	{
////		Cascade_Slave_Write(txBuf, txLen);
////	}
//	else if(MAIN_COM == uartNo) //main usart
//	{
//		while (OPT_FAILURE == MAINUSART_Send(txBuf, txLen));
//		while (OPT_FAILURE == MAINUSART_Send((CDV_INT08U*)&crc, 2));
//	}
//	else
//	{
//		USARTSend(txBuf, txLen, uartNo);
//		USARTSend((CDV_INT08U*)&crc, 2, uartNo);
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
//		//RequestAdd(USART_TX_BUF_ADDR, len);
//		memcpy(USART_TX_BUF_ADDR + txLen, &crc, 2);
//		USART_TX_QUEUE_SELF_ADD;			
//		
//	//} else {
//		//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
//	}
//	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
//	//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ//��ֹ8������һ����//�������Ϊ����add��ʾ��������������ӵ�
//	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	
}


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
void ScriptRecvInit(CDV_INT32U addr , CDV_INT32U len, CDV_INT08U port) {
	CDV_INT08U i;
	g_scriptRecv.doPos = 0;
	//g_scriptRecv.no = 0;
	g_scriptRecv.rxPos = 0;
	g_scriptRecv.totalLen = len;
	g_scriptRecv.tmpLen = 0;
	g_scriptRecv.addr = addr;
	g_scriptRecv.crc = 0xFFFF;
	g_scriptRecv.err = 0;
	g_scriptRecv.port = port;
	
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
	g_scriptRecv.crc = 0;
	g_scriptRecv.port = 0;
	g_cdvStat = CDV_ONLINE;
	
	for (i = 0; i < QUE_NUM; i++) {
		g_scriptRecv.buf[i] = NULL;
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
void ScriptRecvCtl(CDV_INT32U addr , CDV_INT32U len, CDV_INT08U port) {
	CDV_INT08U start = 0 ;
	CDV_INT32U cnt = 1 ;
	CDV_INT32U lastRxPos = 0;
	if(0 == addr || 0 == len)
		return;
	ScriptRecvInit(addr , len,port);
	
	while(1) {//����Ƿ��û�������FPGA�������ص�flash�Ĳ��뿪��
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
				cnt = 1;
			} else {//��ʱ�ж�
				cnt ++;
				if (cnt == 0x100000 )
					break;
			}
		} else if( 0 == start){
			if(0 !=g_scriptRecv.len[g_scriptRecv.rxPos]) {
				start = 1;
			}
		}
	}
	
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
		//ModbusRequest(65,00,uartNo);
		CDVParamInit();
	} else {
		//ModbusRequest(65,02,uartNo);
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

