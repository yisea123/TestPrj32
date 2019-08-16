/**
  ******************************************************************************
  * @file    /cdv_debug.c 
  * @author  MMY
  * @version V2.0.0
  * @date    30-July-2015
  * @brief   debug function and var and 
  * 
@verbatim  
20151221：DebugCmdSend改变中文长度的判断规则，根据最后一个字节判断
20151224: DebugParse的上一步当成心跳
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
	switch(rxBuf[1]){ //命令码筛选	
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
  * @brief  分析APP下发的流程调试命令并操作
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxlen       待分析字符串的长度
  *   
  * @retval 返回值说明
  *
  * @note   在RecvParse中
  */

RET_STATUS DebugParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo)
{
//	OS_ERR err;
	RET_STATUS ret = OPT_SUCCESS;
	CDV_STATUS tmpCdvStat;
	switch(rxBuf[1]){ //命令码筛选
		case 0x0F://修改
			AppScriptSet(rxBuf, rxLen);
      //AddTx(rxBuf , rxLen-2);		//应用层会回复，所以这里不加反馈
			break;
		case 0x0e://流程调试
			switch(rxBuf[2]) {
				case 0x00://跳转
					if(0xff == rxBuf[4]) {
					  tmpCdvStat = g_cdvStat;
					  g_cdvStat = CDV_DEBUG_JUMP;
					  //g_debugData.val = (rxBuf[5]<<3) + (rxBuf[6]<<2) + (rxBuf[7]<<1) + rxBuf[8];
					  DebugJump((rxBuf[5]<<3) + (rxBuf[6]<<2) + (rxBuf[7]<<1) + rxBuf[8], uartNo);
			  	  g_cdvStat = tmpCdvStat;
					}
					break;
				case 0x01://上一步
          //DebugLast();
				  AddTx(rxBuf , rxLen-2, uartNo);
					break;
				case 0x02://获取工人脚本
					tmpCdvStat = g_cdvStat;
					g_cdvStat = CDV_GET_WORKER_SCRIPT;
				  //g_debugData.val = rxBuf[3];
					LED3 = LED_ON;
					GetWorkerTypeScript(rxBuf[3], rxBuf[4], uartNo);
				  //GetWorkerScript(rxBuf[3], uartNo);
			    LED3 = LED_OFF;
			    g_cdvStat = tmpCdvStat;
					break;
				case 0x03://获取所有工人
					tmpCdvStat = g_cdvStat;
					g_cdvStat = CDV_GET_ALL_WORKER;
					LED3 = LED_ON;
				
				  GetAllWorker(uartNo);
          //GetWorkerInfo();
			    LED3 = LED_OFF;
			    g_cdvStat = tmpCdvStat;
					break;
				case 0x04://执行工人脚本
					tmpCdvStat = g_cdvStat;
					g_cdvStat = CDV_RUN_WORKER_SCRIPT;
					LED3 = LED_ON;
				  DebugRun(uartNo);
			    LED3 = LED_OFF;
			    g_cdvStat = tmpCdvStat;
					break;
				case 0x05://切换调试工人
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
				case 0x06://获取版本号
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
		case 1://启动运行设置
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
		default: //命令码无效不应答
			ret = OPT_FAILURE;
			//AddTx(rxBuf , rxLen-2);
			break;
	}
	return ret;
}


/**
  * @brief  跳转到步
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxlen       待分析字符串的长度
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS DebugJump(CDV_INT08U no, CDV_INT08U uartNo) {
	CDV_INT08U *txBuf = NULL;
	if (OPT_FAILURE == ScriptDebugParse(no, uartNo)) {
	 /*发送结束，告诉APP*/
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
  * @brief  运行调试工人流程一次
  *  
  * @param  
  *   
  * @retval 返回值说明
  *
  * @note   必须在获取完工人流程后进行
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
  * @brief  分析当前命令并执行
  *  
  * @param  scriptNo     待分析工人号
  *   
  * @retval 返回值说明
  *
  * @note   必须保证同时只有一个线程在分析相同scriptNo的脚本
            第几条（高位）	第几条（低位）	命令剩余字节	    资源类型	功能码1	        功能码2	    功能码3
            XX	            XX	            剩余长度（00-FF）	00	      资源号（00-FF）	00绝对运动	FF
  */
RET_STATUS DebugCmdParse(CDV_INT08U uartNo) {
//	CDV_INT08U chNum , *buf = NULL;
	RET_STATUS ret;
  CDV_INT08U scriptNo = SCRIP_MAX_RUN;//指向调试工人
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
  * @brief  脚本解析动作反
  *  
  * @param  no     待获取工人号
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
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
  * @brief  获取特定类型工人脚本
  *  
  * @param  no     待获取工人号
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
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
 /*发送结束，告诉APP*/
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
  * @brief  切换调试的工人
  *  
  * @param  no     待获取工人号
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
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
  * @brief  切换调试工人
  *  
  * @param  no     待获取工人号
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS ChangeDebugWorker(CDV_INT08U no, CDV_INT08U uartNo) {
	
	CDV_INT08U i = no/* ,*buf = NULL*/;
	
	CDV_INT16U len = 0;
	//DebugClear();
  if(!g_line.init) {
		return OPT_FAILURE;
	}

	SPI_Flash_Read((CDV_INT08U*)&len,SCRIP_LEN(i),2);//读取脚本长度
	g_run.startPos[SCRIP_MAX_RUN] = SCRIPADDR(i);

  /*发送给APP*/
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
  * @brief  获取工人脚本
  *  
  * @param  no     待获取工人号
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS GetWorkerScript(CDV_INT08U no, CDV_INT08U uartNo) {
	
	CDV_INT08U i = no /*,*buf = NULL*/;
	
	CDV_INT16U len = 0;
	//DebugClear();
  if(!g_line.init) {
		return OPT_FAILURE;
	}

	SPI_Flash_Read((CDV_INT08U*)&len,SCRIP_LEN(i),2);//读取脚本长度
	g_run.startPos[SCRIP_MAX_RUN] = SCRIPADDR(i);
 // g_debug.which = i;
  /*发送给APP*/
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
			(APP_SET_NUM)++; // 计数第几条
		} while (++g_run.doNo[SCRIP_MAX_RUN] < g_run.cmdNum[SCRIP_MAX_RUN] && g_cdvStat == CDV_GET_WORKER_SCRIPT);
		//while (NextScriptCmd(0 , 0) != OPT_FAILURE);
		g_run.doNo[SCRIP_MAX_RUN] = g_run.cmdNum[SCRIP_MAX_RUN] - 1;
	}
//	DELETE(buf);
	return OPT_SUCCESS;
}

/**
  * @brief  获取所有工人
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS GetAllWorker(CDV_INT08U uartNo) {
	OS_ERR err;
	CDV_INT08U i , *txBuf = NULL ,*buf = NULL,type;	
	CDV_INT16U len = 0;
	if(!g_line.init) {
		return OPT_FAILURE;
	}
	OSTimeDlyHMSM(0,0,0,1000,OS_OPT_TIME_HMSM_STRICT,&err); //延时
	APP_SET_NUM = 0; //发送时的工人项计数
	/*发送运行工人名+调试工人名，不发送启动工人名*/
	for (i = 0; i < SRP_NUM_RUN/* + SRP_NUM_DEBUG*/; i++) {

		SPI_Flash_Read((CDV_INT08U*)&len,SCRIP_NAME_LEN(i),1);//读取脚本名长度
		NEW08U(buf , len);
		SPI_Flash_Read(buf , SCRIP_NAME(i) , len);	
		SPI_Flash_Read((CDV_INT08U*)&type,SCRIP_TYPE(i),1);//读取工人类型
		
		NEW08U(txBuf , (len+5));
		txBuf[0] = TOAPP_ID;
		txBuf[1] = 0x0e;
		txBuf[2] = 0x03;
		txBuf[3] = (APP_SET_NUM)++;//第几条设置命令，不能用no
		
		txBuf[4] = type;
		memcpy(txBuf + 5, buf , len);//复制工人名
		AddRequestTx(txBuf , len + 5, uartNo);
		DELETE(txBuf);
		DELETE(buf);
	}
  /*发送结束，告诉APP*/
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
  * @brief  获取版本号
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

void GetVersion(CDV_INT08U uartNo) {
	CDV_INT08U *txBuf = NULL, version;
	SPI_Flash_Read((CDV_INT08U*)&version, VERSION_ADDR, 1);//读取版本号
	
	NEW08U(txBuf , 4);
	txBuf[0] = APP_ID;
	txBuf[1] = 0x0e;
	txBuf[2] = 0x06;
	txBuf[3] = version;
	
	AddTx(txBuf , 4, uartNo);
	DELETE (txBuf);
}

/**
  * @brief  获取生产线名称
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

void GetLineName(CDV_INT08U uartNo) {
	CDV_INT08U *txBuf = NULL, len;
	if(!g_line.init) {
		return;
	}
	SPI_Flash_Read((CDV_INT08U*)&len, SCRIP_LINE_NAME, 1);//读取名称长度
	
	NEW08U(txBuf , len + 3);
	SPI_Flash_Read(txBuf + 3, SCRIP_LINE_NAME+1, len);//读取名称
	txBuf[0] = APP_ID;
	txBuf[1] = 0x0e;
	txBuf[2] = 0x07;
	
	AddTx(txBuf , len + 3, uartNo);
	DELETE (txBuf);
}
/**
  * @brief  发送一条脚本给APP。分析当前命令并转换成串口命令发送给APP
  *  
  * @param  scriptNo     待分析工人号
  *   
  * @retval 返回值说明
  *
  * @note   必须保证同时只有一个线程在分析相同scriptNo的脚本
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
		
			dataLen = 4;//数值长度
			//strLen += 4;
			break;
		case 0xfe:
		
			dataLen = 1+buf[5]+1;//变量号+变量名长度+变量名 所钻字节
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
			dataLen = 8;//枚举名长度 字节
			break;
		default:
			dataLen = 0;
			//strLen += 8;
			break;
	}
	//OSSemPend(&TX_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	switch(buf[0]){
		case 0x09:/*显示命令*/

			dataLen = len - 5 - strLen;
			
			NEW08U(txBuf , len);
			
			txBuf[0] = TOAPP_ID;
			txBuf[1] = 0x0f;
			txBuf[2] = ((APP_SET_NUM)>>8);
			txBuf[3] = APP_SET_NUM;
			txBuf[4] = 0xed;		
			memcpy(txBuf + 5, (buf + len - strLen - 1) , strLen);//
			//memcpy(txBuf + 5, (buf + 4 )-(0xff == opt? -1 : 0) , strLen);//复制工人名
			memcpy(txBuf + 5+strLen, (buf + 4) , len - 5-strLen);//复制工人名
			break;
		default:
			//strLen = len - 4;
		    NEW08U(txBuf , strLen+dataLen+5);		
		  
			txBuf[0] = TOAPP_ID;
			txBuf[1] = 0x0f;
			txBuf[2] = ((APP_SET_NUM)>>8);
			txBuf[3] = APP_SET_NUM;
			txBuf[4] = type;		
			memcpy(txBuf + 5, buf + startPos , dataLen);//复制参数
			//memcpy(txBuf + 5, buf + 4 , strLen);//复制工人名
		    memcpy(txBuf + dataLen + 5, (buf + len - strLen - 1) , strLen);//复制流程注释
			break;
	}
	AddRequestTx(txBuf , strLen+dataLen+5 , uartNo);
	DELETE (txBuf);
	//DELETE (buf);
	return OPT_SUCCESS;
}

/**
  * @brief  分析APP下发的设置串口命令并操作
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxlen       待分析字符串的长度(包括crc
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS AppScriptSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
	CDV_INT08U row , type , allow=0;
	CDV_INT32U cmdPos;
	CDV_INT08U chNum , *buf = NULL;
	row = rxBuf[2]<<8 | rxBuf[3];//行号
	type = rxBuf[4];//数值类型
	
	ToScriptCmd( SCRIP_MAX_RUN , row);
#ifdef RUN_IN_MEM
	cmdPos = g_run.cmdPos[SCRIP_MAX_RUN] + FLASH_WORK_ADDR;
#else
	cmdPos = g_run.cmdPos[SCRIP_MAX_RUN];
#endif
	SPI_Flash_Read(&chNum,  cmdPos + 2, 1);
	NEW08U(buf , chNum);
  SPI_Flash_Read(buf,  cmdPos + 3, (CDV_INT16U)(chNum));	
	
	
	if(0xff != buf[0]) {//显示类型不允许修改
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
  * @brief  获取工人信息
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS GetWorkerInfo(void) {
	CDV_INT08U i/* ,*buf = NULL*/;
	CDV_INT08U type;
	//CDV_INT16U len = 0, crc;
	
	if(!g_line.init) {
		return OPT_FAILURE;
	}
	APP_SET_NUM = 0; //发送时的工人项计数
	ClearArrayU32(&g_startInf.infPos);
	ClearArrayU32(&g_exitInf.infPos);
	ClearArrayU32(&g_runInf.infPos);
	ClearArrayU32(&g_debugInf.infPos);
	ClearArrayU32(&g_formulaInf.infPos);
	/*发送运行工人名+调试工人名，不发送启动工人名*/
	for (i = 0; i < SRP_NUM_RUN/* + SRP_NUM_DEBUG*/; i++) {
		
		SPI_Flash_Read((CDV_INT08U*)&type,SCRIP_TYPE(i),1);//读取工人类型
		
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
	SPI_Flash_Read((CDV_INT08U*)&g_formulaInf.flag, SCRIP_FORMULA, 1);//读取当前使用的配方
	if(g_formulaInf.flag >= g_formulaInf.infPos.len) {
		g_formulaInf.flag = 0;
		SPI_Flash_Write((CDV_INT08U*)&g_formulaInf.flag, SCRIP_FORMULA, 1);//读取当前使用的配方
	}
	return OPT_SUCCESS;
}
/**
  * @brief  切换配方工人
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS SetRecFormula(CDV_INT08U no, CDV_INT08U uartNo) {
	CDV_INT08U len = 5, *txBuf=NULL;
	if(no >= g_formulaInf.infPos.len)
		g_formulaInf.flag = 0;
	else
		g_formulaInf.flag = no;
		
	SPI_Flash_Write((CDV_INT08U*)&g_formulaInf.flag, SCRIP_FORMULA,1);//读取当前使用的配方
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
  * @brief  读取当前配方工人
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
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
///////////////////////////////////////////////调用工人及工人流程的控制/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////依赖于MEM中的脚本///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///** 读指令到buf
//  * @brief  读指令到buf
//  * @param  
//  * @retval 返回值说明
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
//  * @brief  跳转到工人的某条命令
//  * @param  void
//  * @retval 返回值说明
//  * @note   必须在初始化之后执行
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
//  * @brief  初始化调试工人
//  * @param  no     待获取工人号
//  * @retval 返回值说明
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
//	/*读取脚本信息*/
//	
//	debugWorker->startPos = SCRIPADDR(no);//起始地址
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

///** 下一条命令
//  * @brief  下一条命令
//  * @param  void
//  *         loop 循环，1循环，0不循环
//  * @retval OPT_SUCCESS 成功指向下一条命令
//  *         OPT_FAILURE 失败
//  * @note   pos会跳转到下一条命令开始处
//20151228 将原来的判断序号来决定是否脚本结束改成判断flash地址偏移大小
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
//  * @brief  分析当前命令并执行
//  *  
//  * @param  scriptNo     待分析工人号
//  *   
//  * @retval 返回值说明
//  *
//  * @note   必须保证同时只有一个线程在分析相同scriptNo的脚本
//            第几条（高位）	第几条（低位）	命令剩余字节	    资源类型	功能码1	        功能码2	    功能码3
//            XX	            XX	            剩余长度（00-FF）	00	      资源号（00-FF）	00绝对运动	FF
//  */
//RET_STATUS WorkerCmdParse(DEBUG_SCRIPT *debugWorker , CDV_INT08U uartNo) {
//	RET_STATUS ret = OPT_SUCCESS;
//	ret =  ResParse (debugWorker->buf , debugWorker->len ,(void* )&debugWorker->which, 0, uartNo);
//	return ret;
//}


///** mem脚本运行
//  * @brief  mem脚本运行
//  * @param  void
//  *         ret   1 ，从0运行
//  *               0， 继续运行
//						loop 循环，1循环，0不循环
//  * @retval 返回值说明OPT_SUCCESS继续执行，OPT_FAILURE停止执行
//  * @note   2-9号脚本
//  */

//RET_STATUS RunWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT08U loop) {

//	if(OPT_SUCCESS == WorkerCmdParse(debugWorker, loop)) {		
//	    return NextWorkerCmd(debugWorker, loop);
//	} else {
//		return OPT_SUCCESS;
//	}

//}
///**
//  * @brief  调用工人
//  *  
//  * @param  
//  *   
//  * @retval 返回值说明
//  *
//  * @note   必须在获取完工人流程后进行
//  */

//void RunWorker(CDV_INT08U no) {

//  DEBUG_SCRIPT worker = {0, 0, 0, 0, 0, 0, NULL, 0};
//	
//	if(OPT_SUCCESS != InitWorker(no, &worker))
//    return;		
//	
//	while (OPT_SUCCESS == RunWorkerCmd (&worker , 0))
//	{
//		//if(暂停)
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
//  * @brief  工人线程控制
//  *  
//  * @param  
//  *   
//  * @retval 返回值说明
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
//  * @brief  工人线程分配
//  *  
//  * @param  
//  *   
//  * @retval 返回值说明
//  *
//  * @note   
//  */

//void WorkerThreadRun(CDV_INT08U tNo, CDV_INT08U no) {
//	OS_ERR err;
//	
//  g_threadInfo[tNo].which = no;
//	g_threadInfo[tNo].status = WORKER_RUN;
//	//TaskState 0xff为退出线程，即线程函数执行完毕or主动的del
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