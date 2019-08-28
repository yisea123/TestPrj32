/**
  ******************************************************************************
  * @file    /cdv_debug.c 
  * @author  MMY
  * @version V2.0.0
  * @date    30-July-2015
  * @brief   debug function and var and 
  * 
@verbatim  
20151221��DebugCmdSend�ı����ĳ��ȵ��жϹ��򣬸������һ���ֽ��ж�
20151224: DebugParse����һ����������
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "cdv_debug.h"
	



#if 0

void ChangeCdvStat(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo){
	switch(rxBuf[1]){ //������ɸѡ	
	  case 0:
			g_offlineUart = uartNo;
			switch(rxBuf[2]) {
				case 0x00:
					if(g_cdvStat == CDV_STOP || g_cdvStat == CDV_PAUSE) {
					  g_cdvStat = CDV_WAIT/*CDV_RUN*/;
					} else if(g_cdvStat == CDV_ONLINE) {
					  g_cdvStat = CDV_START;
//				      OSTaskResume((OS_TCB*)&WorkerManageTaskTCB,&err);
					}
					//AddTx(rxBuf , rxLen-2, uartNo);
					break;
				case 0x01:
					if(g_cdvStat == CDV_RUN) {
					  g_cdvStat = CDV_PAUSE;
						//AddTx(rxBuf , rxLen-2, uartNo);
					}
					
					
					break;
				case 0x02:
					if(g_cdvStat == CDV_START || g_cdvStat == CDV_RUN || g_cdvStat == CDV_PAUSE){
					  g_cdvStat = CDV_STOP;
					  //AddTx(rxBuf , rxLen-2, uartNo);
					}
	//				Log_Stop();
					break;
				default:
					break;
			}
			//rxBuf[0] = 0xAD;
			
		}
}


void TypeDebugRun(CDV_INT08U type, CDV_INT08U no, CDV_INT08U uartNo);


RET_STATUS ChangeDebugWorker(CDV_INT08U no, CDV_INT08U uartNo);
void GetVersion(CDV_INT08U uartNo);
void GetLineName(CDV_INT08U uartNo);

/**
  * @brief  ����APP�·������̵����������
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxlen       �������ַ����ĳ���
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��RecvParse��
  */

RET_STATUS DebugParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo)
{
//	OS_ERR err;
	RET_STATUS ret = OPT_SUCCESS;
	CDV_STATUS tmpCdvStat;
	switch(rxBuf[1]){ //������ɸѡ
		case 0x0F://�޸�
			AppScriptSet(rxBuf, rxLen);
      //AddTx(rxBuf , rxLen-2);		//Ӧ�ò��ظ����������ﲻ�ӷ���
			break;
		case 0x0e://���̵���
			switch(rxBuf[2]) {
				case 0x00://��ת
					if(0xff == rxBuf[4]) {
					  tmpCdvStat = g_cdvStat;
					  g_cdvStat = CDV_DEBUG_JUMP;
					  //g_debugData.val = (rxBuf[5]<<3) + (rxBuf[6]<<2) + (rxBuf[7]<<1) + rxBuf[8];
					  DebugJump((rxBuf[5]<<3) + (rxBuf[6]<<2) + (rxBuf[7]<<1) + rxBuf[8], uartNo);
			  	  g_cdvStat = tmpCdvStat;
					}
					break;
				case 0x01://��һ��
          //DebugLast();
				  AddTx(rxBuf , rxLen-2, uartNo);
					break;
				case 0x02://��ȡ���˽ű�
					tmpCdvStat = g_cdvStat;
					g_cdvStat = CDV_GET_WORKER_SCRIPT;
				  //g_debugData.val = rxBuf[3];
					LED3 = LED_ON;
					GetWorkerTypeScript(rxBuf[3], rxBuf[4], uartNo);
				  //GetWorkerScript(rxBuf[3], uartNo);
			    LED3 = LED_OFF;
			    g_cdvStat = tmpCdvStat;
					break;
				case 0x03://��ȡ���й���
					tmpCdvStat = g_cdvStat;
					g_cdvStat = CDV_GET_ALL_WORKER;
					LED3 = LED_ON;
				
				  GetAllWorker(uartNo);
          //GetWorkerInfo();
			    LED3 = LED_OFF;
			    g_cdvStat = tmpCdvStat;
					break;
				case 0x04://ִ�й��˽ű�
					tmpCdvStat = g_cdvStat;
					g_cdvStat = CDV_RUN_WORKER_SCRIPT;
					LED3 = LED_ON;
				  DebugRun(uartNo);
			    LED3 = LED_OFF;
			    g_cdvStat = tmpCdvStat;
					break;
				case 0x05://�л����Թ���
					tmpCdvStat = g_cdvStat;
					g_cdvStat = CDV_GET_WORKER_SCRIPT;
				  //g_debugData.val = rxBuf[3];
					LED3 = LED_ON;
					ChangeTypeDebugWorker(rxBuf[3], rxBuf[4], uartNo);
					AddTx(rxBuf , rxLen-2, uartNo);
				  //GetWorkerScript(rxBuf[3], uartNo);
			    LED3 = LED_OFF;
			    g_cdvStat = tmpCdvStat;
					break;
				case 0x06://��ȡ�汾��
					tmpCdvStat = g_cdvStat;
					g_cdvStat = CDV_GET_VERSION;
				  
					LED3 = LED_ON;
					GetVersion(uartNo);

					GetLineName(uartNo);
			    LED3 = LED_OFF;
				  g_cdvStat = tmpCdvStat;
					break;
				default:
					//AddTx(rxBuf , rxLen-2);
					break;
			}
			break;
		case 0:
			g_offlineUart = uartNo;
			switch(rxBuf[2]) {
				case 0x00:
					if(g_cdvStat == CDV_STOP || g_cdvStat == CDV_PAUSE) {
					  //g_cdvStat = CDV_WAIT/*CDV_RUN*/;
						g_cdvStat = CDV_WAIT;  
						//AddTx(rxBuf , rxLen-2, uartNo);
					} else if(g_cdvStat == CDV_ONLINE) {
					  g_cdvStat = CDV_START;
						//AddTx(rxBuf , rxLen-2, uartNo);
//				      OSTaskResume((OS_TCB*)&WorkerManageTaskTCB,&err);
					}
					
					break;
				case 0x01:
					if(g_cdvStat == CDV_RUN) {
					  g_cdvStat = CDV_PAUSE;
						//AddTx(rxBuf , rxLen-2, uartNo);
					}
					
					
					break;
				case 0x02:
					if(g_cdvStat == CDV_START || g_cdvStat == CDV_RUN || g_cdvStat == CDV_PAUSE){
					  g_cdvStat = CDV_STOP;
					  //AddTx(rxBuf , rxLen-2, uartNo);
					}
					
	//				Log_Stop();
					break;
				default:
					break;
			}
			//rxBuf[0] = 0xAD;
			AddTx(rxBuf , rxLen-2, uartNo);
//			OSTaskResume((OS_TCB*)&WorkerManageTaskTCB,&err);
			break;
		case 1://������������
			switch(rxBuf[2]) {
//				case 0xFF:
//					//CDV_PAUSE
//					if(g_cdvStat == CDV_STPAUSE){
//					  g_tmpCdvStat = CDV_WAIT;
////					  OSTaskResume((OS_TCB*)&WorkerManageTaskTCB,&err);	
//					}
//					break;
				default:
					AppValSet(rxBuf,rxLen);
					break;
			}
			//AddTx(rxBuf , rxLen-2, uartNo);
			break;
		case 3:
			ret = PromptParse(rxBuf,rxLen);//CD 03 09 EE 0F
			break;
		case 4:
			//PromptCmd(rxBuf+2,rxLen-4);//CD 04 09 00 FE 05
			break;
		case 5:
			
			Log_Cmd(rxBuf,rxLen, uartNo);
			break;
		case 6:
			
			Ini_Cmd(rxBuf,rxLen, uartNo);
			break;
		case 0x10:
			tmpCdvStat = g_cdvStat;
					g_cdvStat = CDV_FORMULA;
			Formula_Cmd(rxBuf,rxLen, uartNo);
		g_cdvStat = tmpCdvStat;
			break;
		default: //��������Ч��Ӧ��
			ret = OPT_FAILURE;
			//AddTx(rxBuf , rxLen-2);
			break;
	}
	return ret;
}


/**
  * @brief  ��ת����
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxlen       �������ַ����ĳ���
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS DebugJump(CDV_INT08U no, CDV_INT08U uartNo) {
	CDV_INT08U *txBuf = NULL;
	if (OPT_FAILURE == ScriptDebugParse(no, uartNo)) {
	 /*���ͽ���������APP*/
	  NEW08U(txBuf , 3);
	  txBuf[0] = APP_ID;
	  txBuf[1] = 0x0e;
	  txBuf[2] = 0xf0;
	  AddTx(txBuf , 3, uartNo);
	  DELETE (txBuf);
	} else {
	  NEW08U(txBuf , 3);
	  txBuf[0] = APP_ID;
	  txBuf[1] = 0x0e;
	  txBuf[2] = 0xf1;
	  AddTx(txBuf , 3, uartNo);
	  DELETE (txBuf);
	}
	return OPT_SUCCESS;
}

/**
  * @brief  ���е��Թ�������һ��
  *  
  * @param  
  *   
  * @retval ����ֵ˵��
  *
  * @note   �����ڻ�ȡ�깤�����̺����
  */

void DebugRun(CDV_INT08U uartNo) {
//	CDV_INT08U *txBuf = NULL;
	
	for ( g_run.doNo[SCRIP_MAX_RUN] = 0; g_run.doNo[SCRIP_MAX_RUN] < g_run.cmdNum[SCRIP_MAX_RUN] ;) {
		if (OPT_FAILURE == ScriptDebugParse(g_run.doNo[SCRIP_MAX_RUN], uartNo)) {
		   
		} else {
		   g_run.doNo[SCRIP_MAX_RUN]++;
		}
	}	
//	return OPT_SUCCESS;
}


/**
  * @brief  ������ǰ���ִ��
  *  
  * @param  scriptNo     ���������˺�
  *   
  * @retval ����ֵ˵��
  *
  * @note   ���뱣֤ͬʱֻ��һ���߳��ڷ�����ͬscriptNo�Ľű�
            �ڼ�������λ��	�ڼ�������λ��	����ʣ���ֽ�	    ��Դ����	������1	        ������2	    ������3
            XX	            XX	            ʣ�೤�ȣ�00-FF��	00	      ��Դ�ţ�00-FF��	00�����˶�	FF
  */
RET_STATUS DebugCmdParse(CDV_INT08U uartNo) {
//	CDV_INT08U chNum , *buf = NULL;
	RET_STATUS ret;
  CDV_INT08U scriptNo = SCRIP_MAX_RUN;//ָ����Թ���
	//g_resUart = uartNo;
  CMD_ARG arg;
  arg.uart = uartNo;
	arg.len = 0;
	arg.buf =	NULL;	
	arg.ptrWorker = NULL;
	ret =  ResParse (g_run.buf[scriptNo] , g_run.len[scriptNo] , &arg);	
	//DELETE(buf);
	return ret;
}

/**
  * @brief  �ű�����������
  *  
  * @param  no     ����ȡ���˺�
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS ScriptDebugParse(CDV_INT08U no, CDV_INT08U uartNo) {
//	CDV_INT08U i;
    RET_STATUS ret;

	ToScriptCmd( SCRIP_MAX_RUN , no);
	ret = DebugCmdParse(uartNo);
//  MODBUS_SET_UPDATE;
	return ret;
}

/**
  * @brief  ��ȡ�ض����͹��˽ű�
  *  
  * @param  no     ����ȡ���˺�
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS GetWorkerTypeScript(CDV_INT08U type, CDV_INT08U no, CDV_INT08U uartNo) {
	
	CDV_INT08U i = 0;
	CDV_INT08U *txBuf = NULL;
	switch (type) {
		case 0:
			if ( no < g_startInf.infPos.len)
			    i = g_startInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		case 1:
			if ( no < g_exitInf.infPos.len)
			    i = g_exitInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		case 2://run
			if ( no < g_runInf.infPos.len)
			    i = g_runInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		case 3://debug
			if ( no < g_debugInf.infPos.len)
			    i = g_debugInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		case 4://formula
			if ( no < g_formulaInf.infPos.len)
			    i = g_formulaInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		default:
			i = 0;
			return OPT_FAILURE;
		
	}
   
	GetWorkerScript(i, uartNo);	 
 /*���ͽ���������APP*/
	NEW08U(txBuf , 3);
	//txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(3));
	txBuf[0] = TOAPP_ID;
	txBuf[1] = 0x0f;
	txBuf[2] = 0xff;
	AddRequestTx(txBuf , 3, uartNo);
	DELETE (txBuf);

	return OPT_SUCCESS;
}

/**
  * @brief  �л����ԵĹ���
  *  
  * @param  no     ����ȡ���˺�
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS ChangeTypeDebugWorker(CDV_INT08U type, CDV_INT08U no, CDV_INT08U uartNo) {
	
	CDV_INT32U i = 0;
	//CDV_INT08U i = 0;
	
	switch (type) {
		case 0:
			if ( no < g_startInf.infPos.len)
			    i = g_startInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		case 1:
			if ( no < g_exitInf.infPos.len)
			    i = g_exitInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		case 2://run
			if ( no < g_runInf.infPos.len)
			    i = g_runInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		case 3://debug
			if ( no < g_debugInf.infPos.len)
			    i = g_debugInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		case 4://formula
			if ( no < g_formulaInf.infPos.len)
			    i = g_formulaInf.infPos.p[no];
			else
				return OPT_FAILURE;
			break;
		default:
			i = 0;
			return OPT_FAILURE;
		
	}
   
	ChangeDebugWorker(i, uartNo);	 


	return OPT_SUCCESS;
}
/**
  * @brief  �л����Թ���
  *  
  * @param  no     ����ȡ���˺�
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS ChangeDebugWorker(CDV_INT08U no, CDV_INT08U uartNo) {
	
	CDV_INT08U i = no/* ,*buf = NULL*/;
	
	CDV_INT16U len = 0;
	//DebugClear();
  if(!g_line.init) {
		return OPT_FAILURE;
	}

	SPI_Flash_Read((CDV_INT08U*)&len,SCRIP_LEN(i),2);//��ȡ�ű�����
	g_run.startPos[SCRIP_MAX_RUN] = SCRIPADDR(i);

  /*���͸�APP*/
	if(2 < len) {
		g_run.cmdNum[SCRIP_MAX_RUN] =  TestCmdNum(g_run.startPos[SCRIP_MAX_RUN] , len);
	} else {
		g_run.cmdNum[SCRIP_MAX_RUN] = 0;
	}
	if (g_run.cmdNum[SCRIP_MAX_RUN] > 0) {
		#ifndef VIRTUAL_LOGIC
		NEW16U(g_debug.cmdPos,g_debug.cmdNum);
		InitCmdPos(g_debug.cmdPos , g_debug.cmdNum , g_debug.startPos , len);
		#else
		g_run.cmdPos[SCRIP_MAX_RUN] = g_run.startPos[SCRIP_MAX_RUN];
		#endif

		g_run.doNo[SCRIP_MAX_RUN] = g_run.cmdNum[SCRIP_MAX_RUN] - 1;
	}
	//DELETE(buf);
	return OPT_SUCCESS;
}
/**
  * @brief  ��ȡ���˽ű�
  *  
  * @param  no     ����ȡ���˺�
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS GetWorkerScript(CDV_INT08U no, CDV_INT08U uartNo) {
	
	CDV_INT08U i = no /*,*buf = NULL*/;
	
	CDV_INT16U len = 0;
	//DebugClear();
  if(!g_line.init) {
		return OPT_FAILURE;
	}

	SPI_Flash_Read((CDV_INT08U*)&len,SCRIP_LEN(i),2);//��ȡ�ű�����
	g_run.startPos[SCRIP_MAX_RUN] = SCRIPADDR(i);
 // g_debug.which = i;
  /*���͸�APP*/
	if(2 < len) {
		g_run.cmdNum[SCRIP_MAX_RUN] =  TestCmdNum(g_run.startPos[SCRIP_MAX_RUN] , len);
	} else {
		g_run.cmdNum[SCRIP_MAX_RUN] = 0;
	}
	if (g_run.cmdNum[SCRIP_MAX_RUN] > 0) {
		#ifndef VIRTUAL_LOGIC
		NEW16U(g_debug.cmdPos,g_debug.cmdNum);
		InitCmdPos(g_debug.cmdPos , g_debug.cmdNum , g_debug.startPos , len);
		#else
		g_run.cmdPos[SCRIP_MAX_RUN] = g_run.startPos[SCRIP_MAX_RUN];
		#endif
		(APP_SET_NUM) = 0;
	//ToScriptCmd( 0 , 0);//
		g_run.doNo[SCRIP_MAX_RUN] = 0;
		do {
			ToScriptCmd( SCRIP_MAX_RUN , g_run.doNo[SCRIP_MAX_RUN]);
			DebugCmdSend(uartNo);
			(APP_SET_NUM)++; // �����ڼ���
		} while (++g_run.doNo[SCRIP_MAX_RUN] < g_run.cmdNum[SCRIP_MAX_RUN] && g_cdvStat == CDV_GET_WORKER_SCRIPT);
		//while (NextScriptCmd(0 , 0) != OPT_FAILURE);
		g_run.doNo[SCRIP_MAX_RUN] = g_run.cmdNum[SCRIP_MAX_RUN] - 1;
	}
//	DELETE(buf);
	return OPT_SUCCESS;
}

/**
  * @brief  ��ȡ���й���
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS GetAllWorker(CDV_INT08U uartNo) {
	OS_ERR err;
	CDV_INT08U i , *txBuf = NULL ,*buf = NULL,type;	
	CDV_INT16U len = 0;
	if(!g_line.init) {
		return OPT_FAILURE;
	}
	OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
	APP_SET_NUM = 0; //����ʱ�Ĺ��������
	/*�������й�����+���Թ�����������������������*/
	for (i = 0; i < SRP_NUM_RUN/* + SRP_NUM_DEBUG*/; i++) {

		SPI_Flash_Read((CDV_INT08U*)&len,SCRIP_NAME_LEN(i),1);//��ȡ�ű�������
		NEW08U(buf , len);
		SPI_Flash_Read(buf , SCRIP_NAME(i) , len);	
		SPI_Flash_Read((CDV_INT08U*)&type,SCRIP_TYPE(i),1);//��ȡ��������
		
		NEW08U(txBuf , (len+5));
		txBuf[0] = TOAPP_ID;
		txBuf[1] = 0x0e;
		txBuf[2] = 0x03;
		txBuf[3] = (APP_SET_NUM)++;//�ڼ����������������no
		
		txBuf[4] = type;
		memcpy(txBuf + 5, buf , len);//���ƹ�����
		AddRequestTx(txBuf , len + 5, uartNo);
		DELETE(txBuf);
		DELETE(buf);
	}
  /*���ͽ���������APP*/
	NEW08U(txBuf , 3);
	//txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(3));
	txBuf[0] = TOAPP_ID;
	txBuf[1] = 0x0e;
	txBuf[2] = 0xff;
	AddRequestTx(txBuf , 3, uartNo);
	DELETE (txBuf);
	
	return OPT_SUCCESS;
}
/**
  * @brief  ��ȡ�汾��
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

void GetVersion(CDV_INT08U uartNo) {
	CDV_INT08U *txBuf = NULL, version;
	SPI_Flash_Read((CDV_INT08U*)&version, VERSION_ADDR, 1);//��ȡ�汾��
	
	NEW08U(txBuf , 4);
	txBuf[0] = APP_ID;
	txBuf[1] = 0x0e;
	txBuf[2] = 0x06;
	txBuf[3] = version;
	
	AddTx(txBuf , 4, uartNo);
	DELETE (txBuf);
}

/**
  * @brief  ��ȡ����������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

void GetLineName(CDV_INT08U uartNo) {
	CDV_INT08U *txBuf = NULL, len;
	if(!g_line.init) {
		return;
	}
	SPI_Flash_Read((CDV_INT08U*)&len, SCRIP_LINE_NAME, 1);//��ȡ���Ƴ���
	
	NEW08U(txBuf , len + 3);
	SPI_Flash_Read(txBuf + 3, SCRIP_LINE_NAME+1, len);//��ȡ����
	txBuf[0] = APP_ID;
	txBuf[1] = 0x0e;
	txBuf[2] = 0x07;
	
	AddTx(txBuf , len + 3, uartNo);
	DELETE (txBuf);
}
/**
  * @brief  ����һ���ű���APP��������ǰ���ת���ɴ�������͸�APP
  *  
  * @param  scriptNo     ���������˺�
  *   
  * @retval ����ֵ˵��
  *
  * @note   ���뱣֤ͬʱֻ��һ���߳��ڷ�����ͬscriptNo�Ľű�
  */

RET_STATUS DebugCmdSend(CDV_INT08U uartNo) {
//	OS_ERR err;
	CDV_INT08U /*no , opt ,*/ type , strLen, *txBuf=NULL , dataLen=0 , startPos = 4;
	CDV_INT08U  len = g_run.len[SCRIP_MAX_RUN], *buf = g_run.buf[SCRIP_MAX_RUN];

	//opt = buf[1];
	type = buf[3];
	strLen = buf[len - 1];
	switch (type) {
		case 0xff:
		
			dataLen = 4;//��ֵ����
			//strLen += 4;
			break;
		case 0xfe:
		
			dataLen = 1+buf[5]+1;//������+����������+������ �����ֽ�
			//strLen += 1;
			break;
		case 0xed:
			dataLen = 0;
			break;
		case 0x2f:
			dataLen = 4;
			startPos += 2;
			type = 0xff;
			break;
		case 0x2e:
			dataLen = 1+buf[7]+1;
			startPos += 2;
			type = 0xfe;
			break;
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
			dataLen = 8;//ö�������� �ֽ�
			break;
		default:
			dataLen = 0;
			//strLen += 8;
			break;
	}
	//OSSemPend(&TX_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	switch(buf[0]){
		case 0x09:/*��ʾ����*/

			dataLen = len - 5 - strLen;
			
			NEW08U(txBuf , len);
			
			txBuf[0] = TOAPP_ID;
			txBuf[1] = 0x0f;
			txBuf[2] = ((APP_SET_NUM)>>8);
			txBuf[3] = APP_SET_NUM;
			txBuf[4] = 0xed;		
			memcpy(txBuf + 5, (buf + len - strLen - 1) , strLen);//
			//memcpy(txBuf + 5, (buf + 4 )-(0xff == opt? -1 : 0) , strLen);//���ƹ�����
			memcpy(txBuf + 5+strLen, (buf + 4) , len - 5-strLen);//���ƹ�����
			break;
		default:
			//strLen = len - 4;
		    NEW08U(txBuf , strLen+dataLen+5);		
		  
			txBuf[0] = TOAPP_ID;
			txBuf[1] = 0x0f;
			txBuf[2] = ((APP_SET_NUM)>>8);
			txBuf[3] = APP_SET_NUM;
			txBuf[4] = type;		
			memcpy(txBuf + 5, buf + startPos , dataLen);//���Ʋ���
			//memcpy(txBuf + 5, buf + 4 , strLen);//���ƹ�����
		    memcpy(txBuf + dataLen + 5, (buf + len - strLen - 1) , strLen);//��������ע��
			break;
	}
	AddRequestTx(txBuf , strLen+dataLen+5 , uartNo);
	DELETE (txBuf);
	//DELETE (buf);
	return OPT_SUCCESS;
}

/**
  * @brief  ����APP�·������ô����������
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxlen       �������ַ����ĳ���(����crc
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS AppScriptSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
	CDV_INT08U row , type , allow=0;
	CDV_INT32U cmdPos;
	CDV_INT08U chNum , *buf = NULL;
	row = rxBuf[2]<<8 | rxBuf[3];//�к�
	type = rxBuf[4];//��ֵ����
	
	ToScriptCmd( SCRIP_MAX_RUN , row);
#ifdef RUN_IN_MEM
	cmdPos = g_run.cmdPos[SCRIP_MAX_RUN] + FLASH_WORK_ADDR;
#else
	cmdPos = g_run.cmdPos[SCRIP_MAX_RUN];
#endif
	SPI_Flash_Read(&chNum,  cmdPos + 2, 1);
	NEW08U(buf , chNum);
  SPI_Flash_Read(buf,  cmdPos + 3, (CDV_INT16U)(chNum));	
	
	
	if(0xff != buf[0]) {//��ʾ���Ͳ������޸�
		switch(type){
			case 0x00:
			case 0x01:
			case 0x02:
			case 0x03:
				switch(buf[3]) {
					case 0x00:
					case 0x01:
					case 0x02:
					case 0x03:
						allow = 1;
						break;
					default:
						break;
				}
				break;
			case 0xff:
				switch(buf[3]) {
					case 0xff:
						allow = 1;
						break;
					case 0x2f:
						SPI_Flash_Write(rxBuf+5, cmdPos + 9, (CDV_INT16U)(rxLen-7));
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}

	if(allow) {
	  SPI_Flash_Write(rxBuf+4, cmdPos + 6, (CDV_INT16U)(rxLen-6));
	}
	
	DELETE (buf);
	return OPT_SUCCESS;
}
/**
  * @brief  ��ȡ������Ϣ
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS GetWorkerInfo(void) {
	CDV_INT08U i/* ,*buf = NULL*/;
	CDV_INT08U type;
	//CDV_INT16U len = 0, crc;
	
	if(!g_line.init) {
		return OPT_FAILURE;
	}
	APP_SET_NUM = 0; //����ʱ�Ĺ��������
	ClearArrayU32(&g_startInf.infPos);
	ClearArrayU32(&g_exitInf.infPos);
	ClearArrayU32(&g_runInf.infPos);
	ClearArrayU32(&g_debugInf.infPos);
	ClearArrayU32(&g_formulaInf.infPos);
	/*�������й�����+���Թ�����������������������*/
	for (i = 0; i < SRP_NUM_RUN/* + SRP_NUM_DEBUG*/; i++) {
		
		SPI_Flash_Read((CDV_INT08U*)&type,SCRIP_TYPE(i),1);//��ȡ��������
		
//		type = buf[len-3];
		
		switch (type) {
			case 0:
				ArrayU32Add(&g_startInf.infPos, i);
				break;
			case 1:
				ArrayU32Add(&g_exitInf.infPos, i);
				break;
			case 2:
				ArrayU32Add(&g_runInf.infPos, i);
				break;
			case 3:
				ArrayU32Add(&g_debugInf.infPos, i);
				break;
			case 4:
				ArrayU32Add(&g_formulaInf.infPos, i);
				break;
			default:
				break;
		}
	}
	SPI_Flash_Read((CDV_INT08U*)&g_formulaInf.flag, SCRIP_FORMULA, 1);//��ȡ��ǰʹ�õ��䷽
	if(g_formulaInf.flag >= g_formulaInf.infPos.len) {
		g_formulaInf.flag = 0;
		SPI_Flash_Write((CDV_INT08U*)&g_formulaInf.flag, SCRIP_FORMULA, 1);//��ȡ��ǰʹ�õ��䷽
	}
	return OPT_SUCCESS;
}
/**
  * @brief  �л��䷽����
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS SetRecFormula(CDV_INT08U no, CDV_INT08U uartNo) {
	CDV_INT08U len = 5, *txBuf=NULL;
	if(no >= g_formulaInf.infPos.len)
		g_formulaInf.flag = 0;
	else
		g_formulaInf.flag = no;
		
	SPI_Flash_Write((CDV_INT08U*)&g_formulaInf.flag, SCRIP_FORMULA,1);//��ȡ��ǰʹ�õ��䷽
	NEW08U(txBuf , len);
	
	txBuf[0] = APP_ID;
	txBuf[1] = 0x10;
	txBuf[2] = 2;
	txBuf[3] = g_formulaInf.flag;
	txBuf[4] = 0xED;		
	
	AddTx(txBuf , len , uartNo);
	DELETE (txBuf);
	
	return OPT_SUCCESS;
}
/**
  * @brief  ��ȡ��ǰ�䷽����
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

CDV_INT08U GetRecFormula(CDV_INT08U uartNo) {
	CDV_INT08U len = 5, *txBuf=NULL;
	NEW08U(txBuf , len);
	
	txBuf[0] = APP_ID;
	txBuf[1] = 0x10;
	txBuf[2] = 0;
	txBuf[3] = g_formulaInf.flag;
	txBuf[4] = 0xED;		
	
	AddTx(txBuf , len , uartNo);
	DELETE (txBuf);
	return OPT_SUCCESS;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////���ù��˼��������̵Ŀ���/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////������MEM�еĽű�///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///** ��ָ�buf
//  * @brief  ��ָ�buf
//  * @param  
//  * @retval ����ֵ˵��
//  * @note   
//  */
//RET_STATUS ReadWorkerBuf(DEBUG_SCRIPT *debugWorker) {

//	RET_STATUS ret;
//	
//	Mem_Read(&debugWorker->len, debugWorker->cmdPos + 2, 1);
//	NEW08U(debugWorker->buf , debugWorker->len);
//  Mem_Read(debugWorker->buf, debugWorker->cmdPos + 3, (CDV_INT16U)(debugWorker->len));	

//	return ret;
//}


///**
//  * @brief  ��ת�����˵�ĳ������
//  * @param  void
//  * @retval ����ֵ˵��
//  * @note   �����ڳ�ʼ��֮��ִ��
//  */
//RET_STATUS ToWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT16U cmdNo) {
//  CDV_INT32U staddr = debugWorker->startPos - FLASH_WORK_ADDR;
//	debugWorker->doNo = cmdNo;

//	debugWorker->cmdPos = Mem_GetCmdPos(cmdNo , debugWorker->cmdNum , staddr);
//	
//	ReadWorkerBuf(debugWorker);
//  return OPT_SUCCESS;
//}



///**
//  * @brief  ��ʼ�����Թ���
//  * @param  no     ����ȡ���˺�
//  * @retval ����ֵ˵��
//  * @note   
//  */

//RET_STATUS InitWorker(const CDV_INT08U no, DEBUG_SCRIPT *debugWorker) {
//	
////	CDV_INT08U i = no/* ,*buf = NULL*/;
//	CDV_INT32U staddr;
//	CDV_INT16U len = 0;
//	//DebugClear();
//  if(!g_line.init) {
//		return OPT_FAILURE;
//	}
//	
//	///////////////////////////
//	
//	len = 0;
//	/*��ȡ�ű���Ϣ*/
//	
//	debugWorker->startPos = SCRIPADDR(no);//��ʼ��ַ
//	Mem_Read((CDV_INT08U*)&len, SCRIP_LEN(no) - FLASH_WORK_ADDR, 2);
//  debugWorker->totalLen = len;
//	
//	if(2 < len) {
//		debugWorker->cmdNum =  Mem_TestCmdNum(debugWorker->startPos - FLASH_WORK_ADDR , len);
//		if(0 == debugWorker->cmdNum)
//			return WORKER_LEN_ERR;
//	} else {
//		debugWorker->cmdNum = 0;
//	}
//	
//	ToWorkerCmd(debugWorker, 0);//
//	
//	return OPT_SUCCESS;
//}
//	

///** ��һ������
//  * @brief  ��һ������
//  * @param  void
//  *         loop ѭ����1ѭ����0��ѭ��
//  * @retval OPT_SUCCESS �ɹ�ָ����һ������
//  *         OPT_FAILURE ʧ��
//  * @note   pos����ת����һ�����ʼ��
//20151228 ��ԭ�����ж�����������Ƿ�ű������ĳ��ж�flash��ַƫ�ƴ�С
//  */

//RET_STATUS NextWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT08U loop) {
//		
//  CDV_INT32U staddr = debugWorker->startPos - FLASH_WORK_ADDR;
//	
//	
//  debugWorker->cmdPos = Mem_GetNextCmdPos(debugWorker->cmdPos);

//	if(debugWorker->cmdPos - staddr >= debugWorker->totalLen) {
//		if(loop) {

//			debugWorker->cmdPos = staddr;

//			debugWorker->doNo = 0;	
//		} else {
//			return OPT_FAILURE;
//		}
//	}	else {
//		
//		debugWorker->doNo++;
//	}
//	
//	ReadWorkerBuf(debugWorker);
//	
//	debugWorker->doNo = Mem_GetNoFromPos(staddr,debugWorker->cmdPos);
//	return OPT_SUCCESS;
//}



///**
//  * @brief  ������ǰ���ִ��
//  *  
//  * @param  scriptNo     ���������˺�
//  *   
//  * @retval ����ֵ˵��
//  *
//  * @note   ���뱣֤ͬʱֻ��һ���߳��ڷ�����ͬscriptNo�Ľű�
//            �ڼ�������λ��	�ڼ�������λ��	����ʣ���ֽ�	    ��Դ����	������1	        ������2	    ������3
//            XX	            XX	            ʣ�೤�ȣ�00-FF��	00	      ��Դ�ţ�00-FF��	00�����˶�	FF
//  */
//RET_STATUS WorkerCmdParse(DEBUG_SCRIPT *debugWorker , CDV_INT08U uartNo) {
//	RET_STATUS ret = OPT_SUCCESS;
//	ret =  ResParse (debugWorker->buf , debugWorker->len ,(void* )&debugWorker->which, 0, uartNo);
//	return ret;
//}


///** mem�ű�����
//  * @brief  mem�ű�����
//  * @param  void
//  *         ret   1 ����0����
//  *               0�� ��������
//						loop ѭ����1ѭ����0��ѭ��
//  * @retval ����ֵ˵��OPT_SUCCESS����ִ�У�OPT_FAILUREִֹͣ��
//  * @note   2-9�Žű�
//  */

//RET_STATUS RunWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT08U loop) {

//	if(OPT_SUCCESS == WorkerCmdParse(debugWorker, loop)) {		
//	    return NextWorkerCmd(debugWorker, loop);
//	} else {
//		return OPT_SUCCESS;
//	}

//}
///**
//  * @brief  ���ù���
//  *  
//  * @param  
//  *   
//  * @retval ����ֵ˵��
//  *
//  * @note   �����ڻ�ȡ�깤�����̺����
//  */

//void RunWorker(CDV_INT08U no) {

//  DEBUG_SCRIPT worker = {0, 0, 0, 0, 0, 0, NULL, 0};
//	
//	if(OPT_SUCCESS != InitWorker(no, &worker))
//    return;		
//	
//	while (OPT_SUCCESS == RunWorkerCmd (&worker , 0))
//	{
//		//if(��ͣ)
//		
//	}
//	
////	for ( worker.doNo = 0; worker.doNo < worker.cmdNum ;) {
////		if (OPT_FAILURE == RunWorkerCmd(worker.doNo, uartNo)) {

////		} else {
////		   worker.doNo++;
////		}
////	}	
////	return OPT_SUCCESS;
//}
//void WorkerTask(void *p_arg) {
//	
//	RunWorker(0);
//}

///**
//  * @brief  �����߳̿���
//  *  
//  * @param  
//  *   
//  * @retval ����ֵ˵��
//  *
//  * @note   
//  */

//void WorkerControl(CDV_INT08U no, WORKER_STATUS status) {

//	int i = 0;
//	
//	for( i = 0; i < SCRIP_MAX_RUN; i++)
//	{
//		if (no == g_threadInfo[i].which)
//		{
//			g_threadInfo[i].status = status;
//			break;
//		}
//		
//	}
//  
//}


///**
//  * @brief  �����̷߳���
//  *  
//  * @param  
//  *   
//  * @retval ����ֵ˵��
//  *
//  * @note   
//  */

//void WorkerThreadRun(CDV_INT08U tNo, CDV_INT08U no) {
//	OS_ERR err;
//	
//  g_threadInfo[tNo].which = no;
//	g_threadInfo[tNo].status = WORKER_RUN;
//	//TaskState 0xffΪ�˳��̣߳����̺߳���ִ�����or������del
//	if(OS_TASK_STATE_DEL == WorkerTaskTCB[tNo].TaskState || NULL == WorkerTaskTCB[tNo].StkPtr)
//	{
//	  OSTaskCreate((OS_TCB* )&WorkerTaskTCB[tNo],
//				(CPU_CHAR* )workerName[tNo],
//				(OS_TASK_PTR )WorkerTask,
//				(void* )&g_threadInfo[tNo],
//				(OS_PRIO )WORKER_TASK_PRIO,
//				(CPU_STK* )&WORKER_TASK_STK[tNo][0],
//				(CPU_STK_SIZE )WORKER_STK_SIZE/10,
//				(CPU_STK_SIZE )WORKER_STK_SIZE,
//				(OS_MSG_QTY )0,
//				(OS_TICK )0,
//				(void* )0,
//				(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//				(OS_ERR* )&err);	
//							
//	}
//}