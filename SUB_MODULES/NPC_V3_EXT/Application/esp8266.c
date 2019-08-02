
/**
  ******************************************************************************
  * @file    /esp8266.c 
  * @author  MMY
  * @version V2.0.0
  * @date    1-September-2015
  * @brief   esp8266(wifi module) function and var and 
  * 
@verbatim  
20160202:wifi�Ļظ�ƥ���Ż����޸�Ϊ�ڽ��պ�����ƥ��
               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "esp8266.h"
/*
 *******************************************************************************
 * global varible section
 *******************************************************************************
 */
CDV_INT08U g_wifiLink = 0;
WIFI_RX g_wifiRx={{{0},{0}},0,0};
WIFI_TX g_wifiTx={{{0},{0}},0,0};
WIFI_CACHE g_wifiCache = {NULL , 0  , 0};
CDV_INT08U g_wifiNeedRequest = 0;
CDV_INT08U g_linkId = 0;
/*
 *******************************************************************************
 * function section
 *******************************************************************************
 */

/**
  * @brief  ��ӷ���atָ����Ͷ���
  *         
  *  
  * @param  reqCnt     �趨��Ҫ�õ��ڼ��λظ�cache
  *           
  *   
  * @retval ����ֵ˵��
  * 
  * @note   �����Ǳ߲���Ҫƥ�䣬����û�б���cache
  */
CDV_INT08U ATTx(CDV_INT08U reqCnt) {
	if(WIFI_TX_HAD) {
		return 0;
	}
	
	if(0 == WIFI_TX_DO_LEN || g_wifiCache.mark !=0)
		return 0;
	
	g_wifiCache.mark = reqCnt;
	return 1;
}

/**
  * @brief  AT������޸�ĩβΪ\r\n
  *  
  * @param  rxBuf     �����ַ���ָ��
  *         txLen     Ӧ���ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
void ATAdd(CDV_INT08U* rxBuf,CDV_INT08U txLen){
	WIFI_TX_BUF(txLen-2) = '\r'; 
	WIFI_TX_BUF(txLen-1) = '\n';
}

/**
  * @brief  AT�����,������8266��AT����
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��֤AT���ͳ�ȥ�����յ��ظ������ݽ����ⲿ������
  */
RET_STATUS ATSend(CDV_INT08C* txBuf, CDV_INT08U txLen , const char* cmpBuf) {
	OS_ERR err;
	CDV_INT08U len = txLen + 2 ,i = 0;
	RET_STATUS ret = OPT_FAILURE;
	if(NULL == txBuf)
		return ret;
//	LED2=1;
		
	while(!WIFI_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
	}
	
	OSSemPend(&TX_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	WIFI_TX_ADD_WITH_LEN(len);//���ٿռ�
	memcpy(WIFI_TX_BUF_ADDR, txBuf , txLen);
	ATAdd(WIFI_TX_BUF_ADDR, len);
	while(!ATTx(1)){//�ȴ�Ӧ�����
		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1ms
		if(i++ > 50) {
			OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
			return ret;
		}
	}
	ATCache(cmpBuf , strlen(cmpBuf));//��wifi�ظ���ƥ���������cacha�����Ƚ�
	i=0;	
	WIFI_TX_QUEUE_SELF_ADD;
	
	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	while (IS_WIFI_NEED_REQUEST && i < 50){//�ȴ�Ӧ�����
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ10
		i++;
	}
	if(IS_WIFI_NEED_REQUEST) {
		g_wifiCache.mark = 0;
	} else {
		OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
		ret = OPT_SUCCESS;
	}
	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
	return ret;
//	LED2=0;
}
/**
  * @brief  com����ͣ�
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ͨ��atģ���Ҫ���͵�com����ͳ�ȥ����crc
  */
RET_STATUS COMSendNoCrc(const char* txBuf, unsigned char txLen , const char* cmpBuf)
{
	OS_ERR err;
	CDV_INT08U len = txLen , i = 0;
	RET_STATUS ret = OPT_FAILURE;
	if(NULL == txBuf || 0 == txLen)
		return ret;
//	LED2=1;
	while(!WIFI_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
	}
		
	OSSemPend(&TX_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	WIFI_TX_ADD_WITH_LEN(len);//���ٿռ�	
	memcpy(WIFI_TX_BUF_ADDR, txBuf , txLen);		
	//RequestAdd(WIFI_TX_BUF_ADDR, len);
	while(!ATTx(2)){//�ȴ�Ӧ�����
		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1ms
		if(i++ > 50){
			OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
			return ret;
		}
		//	break;
	}
	ATCache(cmpBuf , strlen(cmpBuf));//��wifi�ظ���ƥ���������cacha�����Ƚ�
	i=0;
	WIFI_TX_QUEUE_SELF_ADD;			
	
	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	while (IS_WIFI_NEED_REQUEST && i < 200){//�ȴ�Ӧ�����
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ10
		i++;
	}
	if(IS_WIFI_NEED_REQUEST) {
		g_wifiCache.mark = 0;
	} else {
		OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
		ret = OPT_SUCCESS;
	}
	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
	return ret;
//	LED2=0;
}
/**
  * @brief  com����ͣ�
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ͨ��atģ���Ҫ���͵�com����ͳ�ȥ����crc
  */
RET_STATUS COMSend(const char* txBuf, unsigned char txLen , const char* cmpBuf)
{
	OS_ERR err;
	CDV_INT08U len = txLen+2 , i = 0;
	RET_STATUS ret = OPT_FAILURE;
	if(NULL == txBuf || 0 == txLen)
		return ret;
//	LED2=1;
	while(!WIFI_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
	}
		
	OSSemPend(&TX_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	WIFI_TX_ADD_WITH_LEN(len);//���ٿռ�	
	memcpy(WIFI_TX_BUF_ADDR, txBuf , txLen);		
	RequestAdd(WIFI_TX_BUF_ADDR, len);
	while(!ATTx(2)){//�ȴ�Ӧ�����
		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1ms
		if(i++ > 50){
			OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
			return ret;
		}
		//	break;
	}
	ATCache(cmpBuf , strlen(cmpBuf));//��wifi�ظ���ƥ���������cacha�����Ƚ�
	i=0;
	WIFI_TX_QUEUE_SELF_ADD;			
	
	OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
	while (IS_WIFI_NEED_REQUEST && i < 200){//�ȴ�Ӧ�����
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ10
		i++;
	}
	if(IS_WIFI_NEED_REQUEST) {
		g_wifiCache.mark = 0;
	} else {
		OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
		ret = OPT_SUCCESS;
	}
	OSSemPost (&TX_SEM,OS_OPT_POST_1,&err);
	return ret;
//	LED2=0;
}
/**
  * @brief  AT�����
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��֤AT���ͳ�ȥ�����յ��ظ������ݽ����ⲿ������ͨ��atģ���Ҫ���͵�com����ͳ�ȥ����crc
  */
RET_STATUS ATCOMSendNoCrc(const char* txBuf, unsigned char txLen) {
//	const char** buf =  (const char**)(char**)&g_wifiCache.buf;
	OS_ERR err;

	char tmp[18]={0};
  if(0 == (WIFI_LINK & 0x07))
		return OPT_FAILURE;
		
	OSSemPend(&WIFI_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���	ʵ��Ϊʲô������ֵĻش�����
	sprintf(tmp , "%s%d,%d" , "AT+CIPSEND=" , g_linkId , txLen);
	
	do{
		while(WIFI_LINK && OPT_FAILURE == ATSend(tmp , strlen(tmp),"OK\r\n"));
	}while(WIFI_LINK && OPT_FAILURE == COMSendNoCrc(txBuf,txLen,"SEND OK\r\n"));

  OSSemPost (&WIFI_SEM,OS_OPT_POST_1,&err);
	return OPT_SUCCESS;

}
/**
  * @brief  AT�����
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��֤AT���ͳ�ȥ�����յ��ظ������ݽ����ⲿ������ͨ��atģ���Ҫ���͵�com����ͳ�ȥ����crc
  */
RET_STATUS ATCOMSend(const char* txBuf, unsigned char txLen) {
//	const char** buf =  (const char**)(char**)&g_wifiCache.buf;
	OS_ERR err;

	char tmp[18]={0};
  if(0 == (WIFI_LINK & 0x07))
		return OPT_FAILURE;
	
	while(!WIFI_CAN_DO) {//�������Ϊ����add��ʾ��������������ӵ�
		//OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
		return OPT_FAILURE;
	}
		
	OSSemPend(&WIFI_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���	ʵ��Ϊʲô������ֵĻش�����
	sprintf(tmp , "%s%d,%d" , "AT+CIPSEND=" , g_linkId , txLen+2);
	
	do{
		while(WIFI_LINK && OPT_FAILURE == ATSend(tmp , strlen(tmp),"OK\r\n"));
	}while(WIFI_LINK && OPT_FAILURE == COMSend(txBuf,txLen,"SEND OK\r\n"));

  OSSemPost (&WIFI_SEM,OS_OPT_POST_1,&err);
	return OPT_SUCCESS;

}

/**
  * @brief  AT�����,��Ҫ�ظ�
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��֤AT���ͳ�ȥ�����յ��ظ������ݽ����ⲿ������ͨ��atģ���Ҫ���͵�com����ͳ�ȥ����crc
20151224:* buf �ĳ� **buf
  */
RET_STATUS ATCOMSendReq(const char* txBuf, unsigned char txLen) {
//	const char** buf =  (const char**)(char**)&g_wifiCache.buf;
//	const char* len = (const char*)(char*)&g_wifiCache.len;
	char *pos = NULL;
	//RET_STATUS ret = OPT_FAILURE;
	
	char tmp[18]={0};
	if(NULL == txBuf || 0 == txLen)
		return OPT_FAILURE;
  do{
		sprintf(tmp , "%s%d,%d" , "AT+CIPSEND=" , g_linkId , txLen+2);
		
		do{
//			do{
			while (OPT_FAILURE == ATSend(tmp , strlen(tmp),"OK\r\n"));
//			}while(0 != strncmp((*buf) + *len-6,5));
	//		if(0 != strncmp(buf + *len-6,"OK\r\n>",5))//˵����������ģʽ
	//			return OPT_FAILURE;

		}while(OPT_FAILURE == COMSend(txBuf,txLen,"SEND OK\r\n"));
		
	
//		ATSend(tmp , strlen(tmp));
//		if(0 != strncmp(buf + *len-6,"OK\r\n>",5))//˵����������ģʽ
//			return OPT_FAILURE;

//		COMSend(txBuf,txLen);
//		if(0 != strncmp(buf + *len-9,"SEND OK\r\n",9))//˵����������ģʽ
//			return OPT_FAILURE;
  
		g_wifiCache.mark = 1;
		while (IS_WIFI_NEED_REQUEST);//�ȴ�Ӧ�����
		pos = strchr(txBuf,':')+1; 

  }while(0 != strncmp(pos,txBuf,txLen));//
		
	return OPT_SUCCESS;

}

/**
  * @brief  AT�ظ�����cache
  *         
  *  
  * @param  rxBuf     ��������
  *         rxLen     ���ճ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ����recv�߳��е�CDV_WIFIģʽ�У���rx����cache����ʾATSend�ظ����ս���������StartWifi����
  */
CDV_INT08U ATCache(CDV_INT08C* rxBuf, CDV_INT08U rxLen) {
//	OS_ERR err;
	if(rxBuf == NULL || rxLen == 0)
		return 0;
//	OSSemPend(&CACHE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	g_wifiCache.len = rxLen;
	
	NEW08U(g_wifiCache.buf, g_wifiCache.len+1);
	
	if(g_wifiCache.buf != NULL) { 
	    memcpy(g_wifiCache.buf , rxBuf , g_wifiCache.len);
		g_wifiCache.buf[g_wifiCache.len] = 0; //���һλ��0��Ϊ����������strstr�������
	}else{
	    g_wifiCache.len = 0;//Ӧ�û�������Ĵ��󾯸�
//		while(1)
//		printf;
//	  return 0;
	}
//	OSSemPost (&CACHE_SEM,OS_OPT_POST_1,&err);
//	if(g_wifiCache.mark)
//	    g_wifiCache.mark --;
	//g_wifiCache.mark = 0;
	return 1;
}

/**
  * @brief  cache�Ƚ�
  *         
  *  
  * @param  
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ����recv�߳��е�CDV_WIFIģʽ�У���rx��cache�Ƚ�
  */
CDV_INT08U CacheCmp(CDV_INT08C* rxBuf) {
//	OS_ERR err;
//	OSSemPend(&CACHE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	if(NULL != strstr(rxBuf,(CDV_INT08C*)g_wifiCache.buf) || NULL != strstr(rxBuf,"link is not")) {
		g_wifiCache.mark = 0;
//		OSSemPost (&CACHE_SEM,OS_OPT_POST_1,&err);
		return 1;
	}
//	OSSemPost (&CACHE_SEM,OS_OPT_POST_1,&err);
	return 0;
}

/**
  * @brief  ����wifi��������װ����
  *         
  *  
  * @param  txBuf     �����ַ����׵�ַָ��
  *         txLen     �����ַ�������,������crc
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   20151224:*buf �ĳ� **buf
  */
RET_STATUS StartWifiServer(void) {
//	const char** buf =  (const char**)(char**)&g_wifiCache.buf;
//	const char* len = (const char*)(char*)&g_wifiCache.len;
	RET_STATUS ret = OPT_FAILURE;
	while(1) 
	{
		if(g_linkId > 1)
		{
			ATSend("AT+RST" , 6 ,"OK");
			g_linkId= 0;
		}
		
		if(IS_SERVER_ON)//˵���Ѿ�����������
		{
			ret =  OPT_SUCCESS;
			break;
		}
		
		if (OPT_FAILURE == ATSend("AT" , 2 ,"OK"))
			break;
		ATSend("AT+RST" , 6 ,"OK");
		Log_Write("wifi sever not open" , LOG_WARNING);
//		ATSend("AT" , 2);//����ATָ��
//		//bufLen = strlen(buf);
//		/*tmpBuf = buf + bufLen-4;*/
//		if(0 != strncmp((*buf) + *len - 4,"OK",2))//˵��û��wifiģ��
//			break;

		//����Ҳ��Ϊ��ѯ����������ѯ��������û�п���
		//ATSend("AT+CIPMUX?" , 10);//���ö�����ģʽ
		//if(0 == strncmp(buf,"AT+CIPMUX?\r\r\n+CIPMUX:1\r\n\r\nOK",28))//
		//	return;
        if (OPT_FAILURE != ATSend("AT+CIPSTO=200" , 13 ,"OK")){
			ret =  OPT_SUCCESS;
			break;
		}
//		ATSend("AT+CIPSTO=200" , 13);//���ó�ʱʱ��//���뿪��������֮�����Կ��������жϷ�������û�п���
//		if(0 == strncmp((*buf)+ *len - 4,"OK",2))//˵���Ѿ�����������
//		{
//			ret =  OPT_SUCCESS;
//			break;
//		}

		//ATSend("AT+CWMODE?\r\n" , 10);//��ѯģʽ
		//if(0 != strncmp(buf,"+CWMODE:2\r\n\r\nOK",15)) {//˵��û�� ����APģʽ
		//	ATSend("AT+CWMODE=2" , 11);//�л�ģʽ��AP
		//	if(0 != strncmp(buf + strlen(buf)-4,"OK",2))//˵��û���л��ɹ�
		//		return;
		//	else
		//		ATSend("AT+RST" , 6);//
		//	return;
		//}
    if (OPT_FAILURE == ATSend("AT+CWSAP=\"HSCDV007\",\"12345678\",6,4" , 34 ,"OK"))//
			//if (OPT_FAILURE == ATSend("AT+CWSAP=\"HSCDV001\",\"\",6,0" , 26 ,"OK"))//
			break;
//		ATSend("AT+CWSAP=\"HSCDV002\",\"12345678\",6,4" , 34);//����wifi
//		if(0 != strncmp((*buf)+ *len - 4,"OK",2))//
//			break;

		

    if (OPT_FAILURE == ATSend("AT+CIPMODE=0" , 12 ,"OK"))
			break;
//		ATSend("AT+CIPMODE=0" , 12);//���÷�͸��ģʽ
//		if(0 != strncmp((*buf)+ *len - 4,"OK",2))//
//			break;

		
    if (OPT_FAILURE == ATSend("AT+CIPMUX=1" , 11 ,"OK"))
			break;
//		ATSend("AT+CIPMUX=1" , 11);//���ö�����ģʽ
//		if(0 != strncmp((*buf)+ *len - 4,"OK",2))//
//			break;
    if (OPT_FAILURE == ATSend("AT+CIPSERVER=1,8080" , 19 ,"OK"))
			break;
//		ATSend("AT+CIPSERVER=1,8080" , 19);//���÷�����ģʽ�������ȿ���������
//		if(0 != strncmp((*buf)+ *len - 4,"OK",2))//
//			break;
    if (OPT_FAILURE == ATSend("AT+CIPSTO=200" , 13 ,"OK"))
			break;
//		ATSend("AT+CIPSTO=200" , 13);//���ó�ʱʱ��//���뿪��������֮�����Կ��������жϷ�������û�п���
//		if(0 != strncmp((*buf)+ *len - 4,"OK",2))//
//			break;
		Log_Write("wifi sever openned" , LOG_WARNING);
		ret =  OPT_SUCCESS;
		break;
	}
	if(OPT_FAILURE == ret)
	{
		
		SERVER_OFF;
		WIFI_LINK = 0;
	}
	else
	{
		SERVER_ON;
	}
	
	return ret;
}




/**
  * @brief  wifi��������
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
RET_STATUS WifiParse(CDV_INT08C* txBuf, CDV_INT08U txLen) {
	//int id = 0;
	char *pos = NULL;
	RET_STATUS ret = OPT_FAILURE;
	
	pos = strstr(txBuf,"link is not");//�ͻ�������
	if (pos != NULL)
	{
		WIFI_LINK = 0;
	}
	
	pos = strstr(txBuf,"+IPD");//�ͻ�������
	if (pos != NULL)
	{
		if (pos[5] >=0x30 && pos[5]<= 0x39)//id
		{
			//pos = strchr(buf,':');
			//*pos = '\0';
			
			int len = atoi(pos+7);
			g_linkId = pos[5] - 0x30;
			pos = strchr(txBuf,':')+1; 
			ret = RecvParse((CDV_INT08U*)pos , len, 5);
			//return ret;
		}

	}
		
	if(0 == strncmp(txBuf,"AT",2))//wifi����ָ��
	{
		return OPT_SUCCESS;
	}
	else
	{
//		pos = strstr(txBuf,"+IPD");//�ͻ�������
//		if (pos != NULL)
//		{
//			if (pos[5] >=0x30 && pos[5]<= 0x39)//id
//			{
//				//pos = strchr(buf,':');
//				//*pos = '\0';
//				
//				int len = atoi(pos+7);
//				g_linkId = pos[5] - 0x30;
//				pos = strchr(txBuf,':')+1; 
//				ret = RecvParse((CDV_INT08U*)pos , len, 5);
//				return ret;
//			}

//		}
					
		pos = strstr(txBuf,"CONNECT");
		if (pos != NULL)
		{
			pos = strstr(txBuf,"FAIL");
			if (pos != NULL)
			{
				if (txBuf[0] >=0x30 && txBuf[0]<= 0x39)
				{
					int id = txBuf[0] - 0x30;
					//printf("%d CLOSED",id);
					WIFI_CLOSE(id);
					
					return ret;
				}
			}
			if (txBuf[0] >=0x30 && txBuf[0]<= 0x39)
			{
				int id = txBuf[0] - 0x30;
				//printf("%d CONNECT",id);
//				if(WIFI_LINK!=0)//�Ѿ����豸���ӵĻ���CDV����¼
//					return;
				WIFI_CONNECT(id);
				g_linkId = id;
				if(!IS_SERVER_ON)
				{
					WIFI_LINK = 0;
				}
				return ret;
			}
			
		}

		pos = strstr(txBuf,"CLOSED");
		if (pos != NULL)
		{
			if (txBuf[0] >=0x30 && txBuf[0]<= 0x39)
			{
				int id = txBuf[0] - 0x30;
				//printf("%d CLOSED",id);
				WIFI_CLOSE(id);
				
				return ret;
			}

		}
	}
	return OPT_SUCCESS;//��������ָ�˵���ܿ�����wifi�Ľ�������ҪATCache
}

void WifiTxQueueDoNext(void){
	DELETE(g_wifiTx.QUEUE.txBuf[g_wifiTx.doPos]);
	g_wifiTx.QUEUE.txLen[g_wifiTx.doPos]=0;

	if(g_wifiTx.txPos!=g_wifiTx.doPos) {
      MAX_SELF_ADD(g_wifiTx.doPos,WIFI_TX_QUEUE_LENGTH);
    }
}

void WifiTxAddWithLen(CDV_INT08U len) {
	if(len == 0)
		NewError();
		
    NEW08U(g_wifiTx.QUEUE.txBuf[g_wifiTx.txPos],len);
	g_wifiTx.QUEUE.txLen[g_wifiTx.txPos] = (len);
}
