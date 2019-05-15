/**
  ******************************************************************************
  * @file    /cdv_offline.c 
  * @author  MMY
  * @version V2.0.0
  * @date    8-July-2015
  * @brief   online function and var and 
  * 
@verbatim  
20151228: ȥ�����������ж��߼���С
20151228: NextScriptCmd��ԭ�����ж�����������Ƿ�ű������ĳ��ж�flash��ַƫ�ƴ�С
20160105�����սű���ˢ����һ��ʣ���ֽ�CopyCmdLen
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "offline.h"
	CDV_INT08U *OfflineScript = NULL;          //��ű����ڴ��
	CDV_INT32U OfflineScriptLen = 0;           //��ű����ڴ���С
	
	
#if 0
	void Mem_ReadLine(void);
	RET_STATUS Mem_RunScript(CDV_INT08U scriptNo , CDV_INT08U loop, CDV_INT08U uartNo);
	RET_STATUS Mem_NextScriptCmd(CDV_INT08U scriptNo , CDV_INT08U loop);
	CDV_INT32U Mem_GetNextCmdPos(CDV_INT32U flashAddr);
	RET_STATUS Mem_ReadScriptBuf(CDV_INT08U scriptNo);
	CDV_INT16U Mem_GetNoFromPos(CDV_INT32U startPos , CDV_INT32U currentPos);
	CDV_INT32U Mem_GetCmdPos(CDV_INT16U no , CDV_INT16U cmdNum , CDV_INT32U flashAddr);
	
//	WORKER_SCRIPT g_workerScript = {0};
	
	RUN_SCRIPT g_run = {0};
	WORKER_INFO g_exitInf = {0, {0, NULL}}, g_startInf = {0, {0, NULL}}, g_runInf = {0, {0, NULL}}, g_debugInf = {0, {0, NULL}}, g_formulaInf = {0, {0, NULL}};
	//DEBUG_SCRIPT g_debug = {0};
	CDV_INT08U g_offlineUart = 0;/*�洢�����������������ĸ����ڷ����*/
	

/**
  * @brief  ����APP�·��Ĵ����������
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         len       �������ַ����ĳ���
  *         txBuff    Ӧ���ַ���  
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS AppParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen)
{
	CDV_INT08U temp[2]={0,0};
	CDV_INT16U crc;
//	OS_ERR err;
	if(1){
		if(rxBuf[0] == APP_ID){//CDV��ַӦ��
			crc = getCRC16(rxBuf,rxLen-2); 
			temp[1] = crc & 0xff; 
			temp[0] = (crc >> 8) & 0xff;
			if((rxBuf[rxLen-1] == temp[0])&&(rxBuf[rxLen-2] == temp[1])){//crcУ�����Ӧ��
				switch(rxBuf[1]){ //������ɸѡ
					case 0:
						switch(rxBuf[2]) {
							case 0x00:
							  g_cdvStat = CDV_RUN;
								break;
							case 0x01:
								g_cdvStat = CDV_PAUSE;
								break;
							case 0x02:
								g_cdvStat = CDV_STOP;
								break;
							default:
								break;
						}
						
						break;
					case 1://������������
						switch(rxBuf[2]) {
							case 0xFF:
							  g_cdvStat = CDV_WAIT;
								break;
							default:
								AppValSet(rxBuf,rxLen);
								break;
						}
						
						break;
					default: //��������Ч��Ӧ��
						break; 
				}					
			} else {//crcУ�����
				temp[1] = crc & 0xff; 
				temp[0] = (crc >> 8) & 0xff;
			}
		}
		else{//��ַ����
			
		}
		
	}
	return OPT_SUCCESS;
}



/**
  * @brief  �ѻ�����ظ�
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
void OfflineRequest(CDV_INT08U no,CDV_INT08U errNo) {
	CDV_INT08U len , *txBuf = NULL;

	len = 3;//tx�ܳ���=6
	
	//txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(len));
	NEW08U(txBuf , len);
	txBuf[0] = APP_ID;
	txBuf[1] = no+0x80;
	txBuf[2] = errNo;
	AddTx(txBuf , len, g_offlineUart);
	
	DELETE(txBuf);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
  * @brief  �����������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   ������ʼ�ű�
  */
void RunClear(void) {
	CDV_INT08U i;
	while(g_run.howMany > SCRIP_MAX_RUN);
	for (i = 0; i < SCRIP_MAX_RUN/*g_run.howMany*/; i++) {
		#ifndef VIRTUAL_LOGIC
		DELETE(g_run.cmdPos[i]);		
		#endif
		DELETE(g_run.buf[i]);
	}

	g_run.howMany = 0;
	
	
//	DELETE(g_val.pos);
//	g_val.num = 0;
}	


/**
  * @brief  ��ȡ���й�������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   ������ʼ�ű�
						20160317������䷽�����ĳ�ʼ��
  */
RET_STATUS RunInit(void) {
	CDV_INT08U i , iScr;
	CDV_INT08U runNum;
	CDV_INT16U len = 0;
	
	GetLineInfo(0);//��ʼ��������
	if(!g_line.init) {
		return OPT_FAILURE;
	}
//	GetWorkerInfo();//20160328���
//	
////	if(1 != g_startInf.infPos.len || 1 != g_exitInf.infPos.len)
////		return OPT_FAILURE;
//	
//	runNum = g_runInf.infPos.len + g_startInf.infPos.len + g_exitInf.infPos.len;//���й��� + �������˳�����
//	
//	if(runNum>SCRIP_MAX_RUN)
//		return OPT_FAILURE;
	
//	RunClear();
	
//	g_run.howMany = runNum;
//	memset(g_run.doNo , 0 , runNum);
//	memset(g_run.totalLen , 0 , runNum);
	
	Mem_ReadLine();
	
	
//	for (i = 0; i < runNum; i++) {
//		len = 0;
//		switch(i){
//			case 0:
//				iScr = g_startInf.infPos.p[0];
//				break;
//			case 1:
//				iScr = g_exitInf.infPos.p[0];
//				break;
//			default:
//				iScr = g_runInf.infPos.p[i-2];
//				break;
//		}
//		
//		/*��ȡ�ű���Ϣ*/

//		SPI_Flash_Read((CDV_INT08U*)&len,SCRIP_LEN(iScr), 2);//��ȡ�ű�����
//		g_run.totalLen[i] = len;
//		g_run.startPos[i] = SCRIPADDR(iScr);//���ղ�ͬ�ķ�ʽ��ȡ��ͬ����ʼ��ַ

//		if(2 < len) {
//			g_run.cmdNum[i] =  TestCmdNum(g_run.startPos[i] , len);
//			if(0 == g_run.cmdNum[i])
//				return WORKER_LEN_ERR;
//			#ifndef VIRTUAL_LOGIC
//			NEW16U(g_run.cmdPos[i] , g_run.cmdNum[i]);
//			InitCmdPos(g_run.cmdPos[i] , g_run.cmdNum[i] , g_run.startPos[i] , len);//��ʼÿ�������λ��
//			#else
//			//g_run.cmdPos[i] = g_run.startPos[i];//��ǰ����pָ���˽ű�����ʼλ��
//			#endif
//		} else {
//			g_run.cmdNum[i] = 0;
//		}
//		ToScriptCmd( i , 0);//
//	}
//	/*��ʼ����������λ��*/
//	//NEW16U(g_val.pos, g_run.cmdNum[0]);
//	
//	if(OPT_FAILURE == InitValCmdPos())
//		return OPT_FAILURE;
//    /*��ʼ���䷽����λ��*/
////	if(OPT_FAILURE == InitFormulaValCmdPos(&g_formulaVal, g_run.cmdNum[0] , g_run.startPos[0] , g_run.totalLen[0]))
////		return OPT_FAILURE;

//	//ToScriptCmd( 0 , g_val.num + g_formulaVal.num);
//	
  return OPT_SUCCESS;
}


/**
  * @brief  ������ʼ������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
RET_STATUS StartInit(void) {
	CDV_INT08U i ;
	CDV_INT08U runNum;
	
//	for(i = 0 ; i < 32;i++)
//	{
//		RESET_MSG(i);
//	}
	ClearMsg();
	
	runNum = g_runInf.infPos.len + g_startInf.infPos.len + g_exitInf.infPos.len;//���й��� + �������˳�����
	
	if(g_runInf.infPos.len >SCRIP_MAX_RUN-2)
		return OPT_FAILURE;
	
	for (i = 0; i < runNum; i++) {//��ʼ����������ָ��
		ToScriptCmd( i , 0);
	}
	
	
  return OPT_SUCCESS;
}
/**
  * @brief  �������ýű�����
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   0�Žű�
  */
RET_STATUS StartScript(CDV_INT08U uartNo) {
	//CDV_INT16U no; //�ڼ�������
	u8 /*i = 0,*/ *txBuf = NULL;

	
	if (OPT_FAILURE == StartInit())
		return OPT_FAILURE;
//	if(OPT_SUCCESS != RunInit() ) {
//		return OPT_FAILURE;
//	}
	
	//APP_SET_NUM = 0; //����ʱ���������������
	
	if(OPT_SUCCESS == ChangeTypeDebugWorker(4, g_formulaInf.flag, uartNo)) {
	  DebugRun(uartNo);
	}
	
//	if(g_run.doNo[SCRIP_MAX_RUN] != g_run.cmdNum[SCRIP_MAX_RUN])
//		return OPT_FAILURE;
	
	
	
	while (OPT_SUCCESS == RunScript (0 , 0, uartNo));
	
	if(CDV_STOP == g_cdvStat)
		return OPT_FAILURE;
//	if(g_run.doNo[0] != g_run.cmdNum[0])
//		return OPT_FAILURE;
	
	
	NEW08U(txBuf , 3);
	txBuf[0] = APP_ID;
	txBuf[1] = 0x00;
	txBuf[2] = 0xff;
	AddTx(txBuf , 3 , uartNo);
	DELETE (txBuf);
	
  return OPT_SUCCESS;
}
	
/**
  * @brief  �˳��ű�����
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   0�Žű�
  */
RET_STATUS EndScript(CDV_INT08U uartNo) {
	u8 *txBuf = NULL;	

	while (OPT_SUCCESS == RunScript (1 , 0, uartNo));
	
	NEW08U(txBuf , 3);
	txBuf[0] = APP_ID;
	txBuf[1] = 0x00;
	txBuf[2] = 0xfe;
	AddTx(txBuf , 3 , uartNo);
	DELETE (txBuf);
    return OPT_SUCCESS;
}

//  u16 timeMS[20][200] = {0};
//  u8 timeEn = 0;
/**
  * @brief  �ű�����
  *  
  * @param  void
  *         ret   1 ����0����
  *               0�� ��������
						loop ѭ����1ѭ����0��ѭ��
  *   
  * @retval ����ֵ˵��OPT_SUCCESS����ִ�У�OPT_FAILUREִֹͣ��
  *
  * @note   2-9�Žű�
  */

RET_STATUS RunScript(CDV_INT08U scriptNo , CDV_INT08U loop, CDV_INT08U uartNo) {
#if RUN_IN_MEM
	return Mem_RunScript( scriptNo ,  loop,  uartNo);
#else
	if (0 == g_run.cmdNum[scriptNo]) {
		return OPT_FAILURE;
	}
	if(OPT_SUCCESS == ScriptCmdParse(scriptNo,0, uartNo)) {
	    return NextScriptCmd(scriptNo, loop);
	} else {
		return OPT_SUCCESS;
	}
#endif
}

/**
  * @brief  ��ת��ĳ�ű���ĳ������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   �����ڳ�ʼ��֮��ִ��
  */
RET_STATUS ToScriptCmd(CDV_INT08U scriptNo , CDV_INT16U cmdNo) {

	#if RUN_IN_MEM
	return Mem_ToScriptCmd( scriptNo ,  cmdNo);
	#else
	
	g_run.doNo[scriptNo] = cmdNo;
	#ifdef VIRTUAL_LOGIC
	g_run.cmdPos[scriptNo] = GetCmdPos(cmdNo , g_run.cmdNum[scriptNo] , g_run.startPos[scriptNo]);
	
	#endif
	ReadScriptBuf(scriptNo);
	return OPT_SUCCESS;
	#endif
  
}

/**
  * @brief  �õ�Ī��cmd��λ��
  *  
  * @param  no�ڼ�������
						cmdNum�ýű��м���cmd
						flashAddr�ýű���ʼflash
						len�ýű�����
  *   
  * @retval ����ֵ˵��,����flashλ��
  *
  * @note   
  */
CDV_INT32U GetCmdPos(CDV_INT16U no , CDV_INT16U cmdNum , CDV_INT32U flashAddr) {
	CDV_INT08U chNum;
	CDV_INT16U cmdNo = 0;
	CDV_INT32U addr = flashAddr;
	
	if(0 == cmdNum) {
		return addr;
	}
	
	while(cmdNo < cmdNum && no != cmdNo) {
		SPI_Flash_Read(&chNum, addr + 2, 1);
		
		{
		  addr += chNum + 3 ;
		}
		
		cmdNo++;
	}
	
	if (no == cmdNo) {
		return addr;
	} else {
		return flashAddr;
	}	
}

/**
  * @brief  ͨ��cmd��λ�õõ��ڼ���
  *  
  * @param  startPos ��һ�������flashλ�ã���g_run.startPos[]
						currentPos Ҫ��ѯ��flash��ַ
  *   
  * @retval ����ֵ˵��,���ڽű��ĵڼ�������
  *
  * @note   
  */
CDV_INT16U GetNoFromPos(CDV_INT32U startPos , CDV_INT32U currentPos) {
	CDV_INT08U chNum;
	CDV_INT16U cmdNo = 0;
	CDV_INT32U addr = startPos;
	
	while(addr < currentPos) {
		SPI_Flash_Read(&chNum, addr + 2, 1);
		addr += chNum + 3 ;
		cmdNo++;
	}
	
	if (addr == currentPos) {
		return cmdNo;
	} else {
		return 0;
	}	
}

/**
  * @brief  ĳ�ű���һ������
  *  
  * @param  void
  *         loop ѭ����1ѭ����0��ѭ��
  *   
  * @retval OPT_SUCCESS �ɹ�ָ����һ������
  *         OPT_FAILURE ʧ��
  *
  * @note   pos����ת����һ�����ʼ�� 
20151228 ��ԭ�����ж�����������Ƿ�ű������ĳ��ж�flash��ַƫ�ƴ�С
  */

RET_STATUS NextScriptCmd(CDV_INT08U scriptNo , CDV_INT08U loop) {
//	if(timeEn/*imeN == scriptNo*/)
//		timeMS[scriptNo][g_run.doNo[scriptNo]] = EndCdvTime(scriptNo);//����������������100������
		
	#ifdef VIRTUAL_LOGIC
		g_run.cmdPos[scriptNo] = GetNextCmdPos(g_run.cmdPos[scriptNo]);
		#endif
	if(g_run.cmdPos[scriptNo] - g_run.startPos[scriptNo] >= g_run.totalLen[scriptNo]) {
		if(loop) {
			#ifdef VIRTUAL_LOGIC
			g_run.cmdPos[scriptNo] = g_run.startPos[scriptNo];
			#endif
			g_run.doNo[scriptNo] = 0;	
		} else {
			return OPT_FAILURE;
		}
	}	else {
		
		g_run.doNo[scriptNo]++;
	}
	
	if(timeEn/*imeN == scriptNo*/)
		StartCdvTime(scriptNo);//��ʼ����
	
	ReadScriptBuf(scriptNo);
	
	g_run.doNo[scriptNo] = GetNoFromPos(g_run.startPos[scriptNo],g_run.cmdPos[scriptNo]);
	return OPT_SUCCESS;
}
/**
  * @brief  �õ���һ��cmd��λ��
  *  
  * @param  ��ǰ�����flash��ַ
						len�ýű�����
  *   
  * @retval ����ֵ˵��,����flashλ��
  *
  * @note   
  */
CDV_INT32U GetNextCmdPos(CDV_INT32U flashAddr) {
	CDV_INT08U chNum;
	CDV_INT32U addr = flashAddr; 
	SPI_Flash_Read(&chNum, addr + 2, 1);
	
	{
	  addr += chNum + 3 ;
	}

	return addr;
}

/**
  * @brief  ĳ�ű�����Լ죬��������Ч��������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��,�����Ч��������
  *
  * @note   pos�����
  */
CDV_INT16U TestCmdNum(CDV_INT32U flashAddr , CDV_INT16U len) {	
	CDV_INT08U chNum;
	CDV_INT16U cmdNo = 0, no;
	CDV_INT32U addr = flashAddr;
	while(addr - flashAddr < len-2) {
		/*��ȡ��ǰ�����*/
		SPI_Flash_Read(&chNum, addr + 2, 1);
		SPI_Flash_Read((CDV_INT08U*)&no, addr, 2);

			cmdNo++;

		{
		  addr += chNum + 3 ;
		}
		
	}

  return cmdNo;//������Ч������
}
/**
  * @brief  ĳ�ű���Ÿ��ƣ�������һ��ʣ���ֽ�
  *  
  * @param  flashAddr���ű���ʼflash��ַ
            len���ű���ַ
  *   
  * @retval void
  *
  * @note   OperateScript��ʹ��
20160521������
  */
void CopyCmdLen(CDV_INT32U flashAddr , CDV_INT16U len) {
	CDV_INT08U chNum = 0;
	CDV_INT32U addr = flashAddr;
	
	while(addr - flashAddr < len-2) {
		
		SPI_Flash_Write(&chNum, addr + 1, 1);
		SPI_Flash_Read(&chNum, addr + 2, 1);/*��ȡ��ǰ�����*/
		addr += chNum + 3 ;
		
	}
}
/**
  * @brief  ��ʼ��,�ű���ÿ��cmd��λ��
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��,�����������
  *
  * @note   pos�����
  */
RET_STATUS InitCmdPos(CDV_INT16U *cmdPos, CDV_INT16U cmdNum , CDV_INT32U flashAddr , CDV_INT16U len) {
	CDV_INT08U sf=0, chNum;
	CDV_INT16U cmdNo = 0,pos = 0;
	
	if(NULL == cmdPos || 0 == cmdNum) {
		return OPT_FAILURE;
	}
	
	do{
		pos += sf;
		cmdPos[cmdNo] = pos;
		SPI_Flash_Read(&chNum, flashAddr + pos + 2, 1);
		sf = chNum + 3;
		cmdNo++;
	}while(pos+sf < len-2 && cmdNo < cmdNum);
	//g_workerScript.scriptPosNum = cmdNo;
	
	return OPT_SUCCESS;
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
RET_STATUS ScriptCmdParse(CDV_INT08U scriptNo, CDV_INT08U debug, CDV_INT08U uartNo) {
	RET_STATUS ret = OPT_SUCCESS;
//	if(timeEn/*imeN == scriptNo*/)
//		StartCdvTime(scriptNo);//��ʼ����
	//g_resUart = g_offlineUart;
	CMD_ARG arg;
	INIT_CLEAR(arg);
  arg.uart = uartNo;
	
	ret =  ResParse (g_run.buf[scriptNo] , g_run.len[scriptNo] , &arg);	
//	if(timeEn/*imeN == scriptNo*/)
//		timeMS[scriptNo][g_run.doNo[scriptNo]] = EndCdvTime(scriptNo);//����������������100������
	CmdArgDelete(&arg);
	return ret;
}

/**
  * @brief  ��ȡĳ�ű��ĵ�ǰ����
  *  
  * @param  scriptNo     ���������˺�
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
RET_STATUS ReadScriptBuf(CDV_INT08U scriptNo) {

	RET_STATUS ret;
	#ifndef VIRTUAL_LOGIC
	SPI_Flash_Read(&g_run.len[scriptNo], g_run.startPos[scriptNo] + g_run.cmdPos[scriptNo][g_run.doNo[scriptNo]] + 2, 1);
	NEW08U(g_run.buf[scriptNo] , g_run.len[scriptNo]);
  SPI_Flash_Read(g_run.buf[scriptNo], g_run.startPos[scriptNo] + g_run.cmdPos[scriptNo][g_run.doNo[scriptNo]] + 3, (CDV_INT16U)(g_run.len[scriptNo]));	
	#else
	SPI_Flash_Read(&g_run.len[scriptNo], g_run.cmdPos[scriptNo] + 2, 1);
	NEW08U(g_run.buf[scriptNo] , g_run.len[scriptNo]);
  SPI_Flash_Read(g_run.buf[scriptNo], g_run.cmdPos[scriptNo] + 3, (CDV_INT16U)(g_run.len[scriptNo]));	
	#endif
	return ret;
}





//////////////////////////////////////////////////////////////////////////////////////////�ڴ�ר��


/**
  * @brief  mem�ű�����
  *  
  * @param  void
  *         ret   1 ����0����
  *               0�� ��������
						loop ѭ����1ѭ����0��ѭ��
  *   
  * @retval ����ֵ˵��OPT_SUCCESS����ִ�У�OPT_FAILUREִֹͣ��
  *
  * @note   2-9�Žű�
  */

RET_STATUS Mem_RunScript(CDV_INT08U scriptNo , CDV_INT08U loop, CDV_INT08U uartNo) {

	if (0 == g_run.cmdNum[scriptNo]) {
		return OPT_FAILURE;
	}
	if(OPT_SUCCESS == ScriptCmdParse(scriptNo,0, uartNo)) {		
	    return Mem_NextScriptCmd(scriptNo, loop);
	} else {
		return OPT_SUCCESS;
	}

}
/**
  * @brief  ��ת��ĳ�ű���ĳ������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   �����ڳ�ʼ��֮��ִ��
  */
RET_STATUS Mem_ToScriptCmd(CDV_INT08U scriptNo , CDV_INT16U cmdNo) {
  CDV_INT32U staddr = g_run.startPos[scriptNo] - FLASH_WORK_ADDR;
	g_run.doNo[scriptNo] = cmdNo;

	g_run.cmdPos[scriptNo] = Mem_GetCmdPos(cmdNo , g_run.cmdNum[scriptNo] , staddr);
	
	Mem_ReadScriptBuf(scriptNo);
  return OPT_SUCCESS;
}
/**
  * @brief  ĳ�ű���һ������
  *  
  * @param  void
  *         loop ѭ����1ѭ����0��ѭ��
  *   
  * @retval OPT_SUCCESS �ɹ�ָ����һ������
  *         OPT_FAILURE ʧ��
  *
  * @note   pos����ת����һ�����ʼ��
20151228 ��ԭ�����ж�����������Ƿ�ű������ĳ��ж�flash��ַƫ�ƴ�С
  */

RET_STATUS Mem_NextScriptCmd(CDV_INT08U scriptNo , CDV_INT08U loop) {
		
  CDV_INT32U staddr = g_run.startPos[scriptNo] - FLASH_WORK_ADDR;
	
	
//	if(timeEn/*imeN == scriptNo*/)
//		timeMS[scriptNo][g_run.doNo[scriptNo]] = EndCdvTime(scriptNo);//����������������100������
	
	
  g_run.cmdPos[scriptNo] = Mem_GetNextCmdPos(g_run.cmdPos[scriptNo]);

	if(g_run.cmdPos[scriptNo] - staddr >= g_run.totalLen[scriptNo]) {
		if(loop) {

			g_run.cmdPos[scriptNo] = staddr;

			g_run.doNo[scriptNo] = 0;	
		} else {
			return OPT_FAILURE;
		}
	}	else {
		
		g_run.doNo[scriptNo]++;
	}
	if(timeEn/*imeN == scriptNo*/)
		StartCdvTime(scriptNo);//��ʼ����
	Mem_ReadScriptBuf(scriptNo);
	
	g_run.doNo[scriptNo] = Mem_GetNoFromPos(staddr,g_run.cmdPos[scriptNo]);
	return OPT_SUCCESS;
}

/**
  * @brief  ��ȡĳ�ű��ĵ�ǰ����
  *  
  * @param  scriptNo     ���������˺�
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
RET_STATUS Mem_ReadScriptBuf(CDV_INT08U scriptNo) {

	RET_STATUS ret;
	
	Mem_Read(&g_run.len[scriptNo], g_run.cmdPos[scriptNo] + 2, 1);
	NEW08U(g_run.buf[scriptNo] , g_run.len[scriptNo]);
  Mem_Read(g_run.buf[scriptNo], g_run.cmdPos[scriptNo] + 3, (CDV_INT16U)(g_run.len[scriptNo]));	

	return ret;
}

#endif

/**
  * @brief  �������ߵ��ڴ�
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
#define NEWCCM(A,len) A=mymalloc(SRAMCCM,len)
void Mem_ReadLine(void) {
	CDV_INT32U size = 0;//�ű���С������ת
	
	SPI_Flash_Read((CDV_INT08U*)&size, FLASH_WORK_ADDR, 4);//��ȡ�����ߴ�С
	if(size >= LINE_LEN)
		return;
	OfflineScriptLen = size + 4;
	

	myfree(SRAMCCM,OfflineScript);
	OfflineScript=mymalloc(SRAMCCM,size + 4);
	//NEWCCM(OfflineScript, size + 4);
	
  SPI_Flash_Read(OfflineScript, FLASH_WORK_ADDR, size + 4);//��ȡ������
}	



/**
  * @brief  ĳ�ű�����Լ죬��������Ч��������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��,�����Ч��������
  *
  * @note   pos�����
  */
CDV_INT16U Mem_TestCmdNum(CDV_INT32U flashAddr , CDV_INT16U len) {	
#if USE_16BIT_CMD == 1u
	CDV_INT16U chNum;
#else
	CDV_INT08U chNum;
#endif
	CDV_INT16U cmdNo = 0, no;
	CDV_INT32U addr = flashAddr;
	while(addr - flashAddr < len-2) {
		/*��ȡ��ǰ�����*/
//#ifdef  _DEBUG_NPC_
//		Mem_Read(&chNum, addr + 2, 1);
//		Mem_Read((CDV_INT08U*)&no, addr, 2);
//#else
#if USE_16BIT_CMD == 1u
	chNum = *(CDV_INT16U*)(SCRIPT_GETADDR(addr + 2));
	//no = *(CDV_INT16U*)(SCRIPT_GETADDR(addr));//�����
//#endif	
		
		cmdNo++;
		addr += chNum + 4 ;
#else
    	chNum = *(CDV_INT08U*)(SCRIPT_GETADDR(addr + 2));	
		cmdNo++;
		addr += chNum + 3 ;
#endif
	}

  return cmdNo;//������Ч������
}
	
/**
  * @brief  �õ���һ��cmd��λ��
  *  
  * @param  ��ǰ�����flash��ַ
						len�ýű�����
  *   
  * @retval ����ֵ˵��,����flashλ��
  *
  * @note   
  */
CDV_INT32U Mem_GetNextCmdPos(CDV_INT32U flashAddr) {
#if USE_16BIT_CMD == 1u
	CDV_INT16U chNum;
	CDV_INT32U addr = flashAddr; 
//#ifdef  _DEBUG_NPC_
//	Mem_Read(&chNum, addr + 2, 1);
//#else
	chNum = *(CDV_INT16U*)(SCRIPT_GETADDR(addr + 2));
//#endif	
	addr += chNum + 4 ;
//addr += chNum + 3 ;
	return addr;
#else
	CDV_INT08U chNum;
	CDV_INT32U addr = flashAddr; 
//#ifdef  _DEBUG_NPC_
//	Mem_Read(&chNum, addr + 2, 1);
//#else
	chNum = *(CDV_INT08U*)(SCRIPT_GETADDR(addr + 2));
//#endif	
	addr += chNum + 3 ;

	return addr;
#endif
}

/**
  * @brief  ͨ��cmd��λ�õõ��ڼ���
  *  
  * @param  startPos ��һ�������flashλ�ã���g_run.startPos[]
						currentPos Ҫ��ѯ��flash��ַ
  *   
  * @retval ����ֵ˵��,���ڽű��ĵڼ�������
  *
  * @note   
  */
CDV_INT16U Mem_GetNoFromPos(CDV_INT32U startPos , CDV_INT32U currentPos) {
#if USE_16BIT_CMD == 1u
	CDV_INT16U chNum;
	CDV_INT16U cmdNo = 0;
	CDV_INT32U addr = startPos;
	
	while(addr < currentPos) {
//#ifdef  _DEBUG_NPC_
//	Mem_Read(&chNum, addr + 2, 1);
//#else
	chNum = *(CDV_INT16U*)(SCRIPT_GETADDR(addr + 2));
//#endif	
		addr += chNum + 4 ;
		cmdNo++;
	}
#else
	CDV_INT08U chNum;
	CDV_INT16U cmdNo = 0;
	CDV_INT32U addr = startPos;
	
	while(addr < currentPos) {
//#ifdef  _DEBUG_NPC_
//	Mem_Read(&chNum, addr + 2, 1);
//#else
	chNum = *(CDV_INT08U*)(SCRIPT_GETADDR(addr + 2));
//#endif	
		addr += chNum + 3 ;
		cmdNo++;
	}
#endif
	if (addr == currentPos) {
		return cmdNo;
	} else {
		return 0;
	}
}

/**
  * @brief  �õ�Ī��cmd��λ��
  *  
  * @param  no�ڼ�������
						cmdNum�ýű��м���cmd
						flashAddr�ýű���ʼflash
						len�ýű�����
  *   
  * @retval ����ֵ˵��,����flashλ��
  *
  * @note   
  */
CDV_INT32U Mem_GetCmdPos(CDV_INT16U no , CDV_INT16U cmdNum , CDV_INT32U flashAddr) {
#if USE_16BIT_CMD == 1u
	CDV_INT16U chNum;
	CDV_INT16U cmdNo = 0;
	CDV_INT32U addr = flashAddr;
	
	if(0 == cmdNum) {
		return addr;
	}
	
	while(cmdNo < cmdNum && no != cmdNo) {
//#ifdef  _DEBUG_NPC_
//	Mem_Read(&chNum, addr + 2, 1);
//#else
	chNum = *(CDV_INT16U*)(SCRIPT_GETADDR(addr + 2));
//#endif	
		{
		  addr += chNum + 4 ;
		}
		
		cmdNo++;
	}
	#else
	CDV_INT08U chNum;
	CDV_INT16U cmdNo = 0;
	CDV_INT32U addr = flashAddr;
	
	if(0 == cmdNum) {
		return addr;
	}
	
	while(cmdNo < cmdNum && no != cmdNo) {
//#ifdef  _DEBUG_NPC_
//	Mem_Read(&chNum, addr + 2, 1);
//#else
	chNum = *(CDV_INT08U*)(SCRIPT_GETADDR(addr + 2));
//#endif	
		{
		  addr += chNum + 3 ;
		}
		
		cmdNo++;
	}
	#endif
	
	if (no == cmdNo) {
		return addr;
	} else {
		return flashAddr;
	}	
}

/**
  * @brief  ��ָ��mem��ַ��ȡ����
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
void Mem_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
// 	u16 i;    		
//	
//	for(i=0; i < NumByteToRead; i++)
//	{
//			pBuffer[i]=OfflineScript[ReadAddr + i];   //ѭ������  
//	}
	ASSERT(OfflineScript && ReadAddr + NumByteToRead <= OfflineScriptLen);
//	if(OfflineScript == NULL || ReadAddr + NumByteToRead > OfflineScriptLen){
//		return;
//	}
	
	MemCpy(pBuffer, OfflineScript + ReadAddr, NumByteToRead);
	
}  


void Mem_Read_Ptr(void** ReadPtr, u32 ReadAddr)
{
	*ReadPtr = (void*)(OfflineScript + ReadAddr);
}

/**
  * @brief  ����mem+ƫ�Ƶ�ַ
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
u8* Mem_Ptr(u32 offsetAddr)   
{
	
	ASSERT(OfflineScript && offsetAddr <= OfflineScriptLen);
//	if(OfflineScript == NULL || offsetAddr > OfflineScriptLen){
//		return NULL;
//	}
	
	return OfflineScript + offsetAddr;
	
}  

//////////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  ��ȡ���й�������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   ������ʼ�ű�
						20160317������䷽�����ĳ�ʼ��
  */
RET_STATUS LineInit(void) {
	CDV_INT08U i , iScr;
	CDV_INT08U runNum;
	CDV_INT16U len = 0;
	
	GetLineInfo(0);//��ʼ��������
	
	if(!g_line.init) {
		return OPT_FAILURE;
	}
	
#if USE_CASCADE_MAP == 1u
	CascadeModbus_MapInit();
#endif
	Mem_ReadLine();
  return OPT_SUCCESS;
}
