/**
  ******************************************************************************
  * @file    /cdv_offline.c 
  * @author  MMY
  * @version V2.0.0
  * @date    8-July-2015
  * @brief   online function and var and 
  * 
@verbatim  
20151228: 去除走流程需判断逻辑大小
20151228: NextScriptCmd将原来的判断序号来决定是否脚本结束改成判断flash地址偏移大小
20160105：接收脚本后刷新上一条剩余字节CopyCmdLen
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
	CDV_INT08U *OfflineScript = NULL;          //存脚本的内存块
	CDV_INT32U OfflineScriptLen = 0;           //存脚本的内存块大小
	
	
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
	CDV_INT08U g_offlineUart = 0;/*存储本次启动命令是由哪个串口发起的*/
	

/**
  * @brief  分析APP下发的串口命令并操作
  *  
  * @param  rxBuf     待分析字符串指针
  *         len       待分析字符串的长度
  *         txBuff    应答字符串  
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS AppParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen)
{
	CDV_INT08U temp[2]={0,0};
	CDV_INT16U crc;
//	OS_ERR err;
	if(1){
		if(rxBuf[0] == APP_ID){//CDV地址应答
			crc = getCRC16(rxBuf,rxLen-2); 
			temp[1] = crc & 0xff; 
			temp[0] = (crc >> 8) & 0xff;
			if((rxBuf[rxLen-1] == temp[0])&&(rxBuf[rxLen-2] == temp[1])){//crc校验错误不应答
				switch(rxBuf[1]){ //命令码筛选
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
					case 1://启动运行设置
						switch(rxBuf[2]) {
							case 0xFF:
							  g_cdvStat = CDV_WAIT;
								break;
							default:
								AppValSet(rxBuf,rxLen);
								break;
						}
						
						break;
					default: //命令码无效不应答
						break; 
				}					
			} else {//crc校验出错
				temp[1] = crc & 0xff; 
				temp[0] = (crc >> 8) & 0xff;
			}
		}
		else{//地址错误
			
		}
		
	}
	return OPT_SUCCESS;
}



/**
  * @brief  脱机出错回复
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
void OfflineRequest(CDV_INT08U no,CDV_INT08U errNo) {
	CDV_INT08U len , *txBuf = NULL;

	len = 3;//tx总长度=6
	
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
  * @brief  清空运行数据
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   包括起始脚本
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
  * @brief  读取运行工人数据
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   包括起始脚本
						20160317添加了配方变量的初始化
  */
RET_STATUS RunInit(void) {
	CDV_INT08U i , iScr;
	CDV_INT08U runNum;
	CDV_INT16U len = 0;
	
	GetLineInfo(0);//初始化生产线
	if(!g_line.init) {
		return OPT_FAILURE;
	}
//	GetWorkerInfo();//20160328添加
//	
////	if(1 != g_startInf.infPos.len || 1 != g_exitInf.infPos.len)
////		return OPT_FAILURE;
//	
//	runNum = g_runInf.infPos.len + g_startInf.infPos.len + g_exitInf.infPos.len;//运行工人 + 启动、退出工人
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
//		/*读取脚本信息*/

//		SPI_Flash_Read((CDV_INT08U*)&len,SCRIP_LEN(iScr), 2);//读取脚本长度
//		g_run.totalLen[i] = len;
//		g_run.startPos[i] = SCRIPADDR(iScr);//按照不同的方式获取不同的起始地址

//		if(2 < len) {
//			g_run.cmdNum[i] =  TestCmdNum(g_run.startPos[i] , len);
//			if(0 == g_run.cmdNum[i])
//				return WORKER_LEN_ERR;
//			#ifndef VIRTUAL_LOGIC
//			NEW16U(g_run.cmdPos[i] , g_run.cmdNum[i]);
//			InitCmdPos(g_run.cmdPos[i] , g_run.cmdNum[i] , g_run.startPos[i] , len);//初始每条命令的位置
//			#else
//			//g_run.cmdPos[i] = g_run.startPos[i];//当前命令p指向工人脚本的起始位置
//			#endif
//		} else {
//			g_run.cmdNum[i] = 0;
//		}
//		ToScriptCmd( i , 0);//
//	}
//	/*初始化启动变量位置*/
//	//NEW16U(g_val.pos, g_run.cmdNum[0]);
//	
//	if(OPT_FAILURE == InitValCmdPos())
//		return OPT_FAILURE;
//    /*初始化配方变量位置*/
////	if(OPT_FAILURE == InitFormulaValCmdPos(&g_formulaVal, g_run.cmdNum[0] , g_run.startPos[0] , g_run.totalLen[0]))
////		return OPT_FAILURE;

//	//ToScriptCmd( 0 , g_val.num + g_formulaVal.num);
//	
  return OPT_SUCCESS;
}


/**
  * @brief  启动初始化数据
  *  
  * @param  void
  *   
  * @retval 返回值说明
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
	
	runNum = g_runInf.infPos.len + g_startInf.infPos.len + g_exitInf.infPos.len;//运行工人 + 启动、退出工人
	
	if(g_runInf.infPos.len >SCRIP_MAX_RUN-2)
		return OPT_FAILURE;
	
	for (i = 0; i < runNum; i++) {//初始化工人运行指针
		ToScriptCmd( i , 0);
	}
	
	
  return OPT_SUCCESS;
}
/**
  * @brief  启动设置脚本运行
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   0号脚本
  */
RET_STATUS StartScript(CDV_INT08U uartNo) {
	//CDV_INT16U no; //第几条命令
	u8 /*i = 0,*/ *txBuf = NULL;

	
	if (OPT_FAILURE == StartInit())
		return OPT_FAILURE;
//	if(OPT_SUCCESS != RunInit() ) {
//		return OPT_FAILURE;
//	}
	
	//APP_SET_NUM = 0; //发送时的启动设置项计数
	
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
  * @brief  退出脚本运行
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   0号脚本
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
  * @brief  脚本运行
  *  
  * @param  void
  *         ret   1 ，从0运行
  *               0， 继续运行
						loop 循环，1循环，0不循环
  *   
  * @retval 返回值说明OPT_SUCCESS继续执行，OPT_FAILURE停止执行
  *
  * @note   2-9号脚本
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
  * @brief  跳转到某脚本的某条命令
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   必须在初始化之后执行
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
  * @brief  得到莫条cmd的位置
  *  
  * @param  no第几条命令
						cmdNum该脚本有几条cmd
						flashAddr该脚本起始flash
						len该脚本长度
  *   
  * @retval 返回值说明,命令flash位置
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
  * @brief  通过cmd的位置得到第几步
  *  
  * @param  startPos 第一条命令的flash位置，即g_run.startPos[]
						currentPos 要查询的flash地址
  *   
  * @retval 返回值说明,所在脚本的第几条命令
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
  * @brief  某脚本下一条命令
  *  
  * @param  void
  *         loop 循环，1循环，0不循环
  *   
  * @retval OPT_SUCCESS 成功指向下一条命令
  *         OPT_FAILURE 失败
  *
  * @note   pos会跳转到下一条命令开始处 
20151228 将原来的判断序号来决定是否脚本结束改成判断flash地址偏移大小
  */

RET_STATUS NextScriptCmd(CDV_INT08U scriptNo , CDV_INT08U loop) {
//	if(timeEn/*imeN == scriptNo*/)
//		timeMS[scriptNo][g_run.doNo[scriptNo]] = EndCdvTime(scriptNo);//结束计数操作超过100会死机
		
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
		StartCdvTime(scriptNo);//开始计数
	
	ReadScriptBuf(scriptNo);
	
	g_run.doNo[scriptNo] = GetNoFromPos(g_run.startPos[scriptNo],g_run.cmdPos[scriptNo]);
	return OPT_SUCCESS;
}
/**
  * @brief  得到下一条cmd的位置
  *  
  * @param  当前命令的flash地址
						len该脚本长度
  *   
  * @retval 返回值说明,命令flash位置
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
  * @brief  某脚本序号自检，输出最大有效命令条数
  *  
  * @param  void
  *   
  * @retval 返回值说明,最大有效命令条数
  *
  * @note   pos不会变
  */
CDV_INT16U TestCmdNum(CDV_INT32U flashAddr , CDV_INT16U len) {	
	CDV_INT08U chNum;
	CDV_INT16U cmdNo = 0, no;
	CDV_INT32U addr = flashAddr;
	while(addr - flashAddr < len-2) {
		/*读取当前命令长度*/
		SPI_Flash_Read(&chNum, addr + 2, 1);
		SPI_Flash_Read((CDV_INT08U*)&no, addr, 2);

			cmdNo++;

		{
		  addr += chNum + 3 ;
		}
		
	}

  return cmdNo;//返回有效命令数
}
/**
  * @brief  某脚本序号复制，加入上一条剩余字节
  *  
  * @param  flashAddr：脚本起始flash地址
            len：脚本地址
  *   
  * @retval void
  *
  * @note   OperateScript中使用
20160521已弃用
  */
void CopyCmdLen(CDV_INT32U flashAddr , CDV_INT16U len) {
	CDV_INT08U chNum = 0;
	CDV_INT32U addr = flashAddr;
	
	while(addr - flashAddr < len-2) {
		
		SPI_Flash_Write(&chNum, addr + 1, 1);
		SPI_Flash_Read(&chNum, addr + 2, 1);/*读取当前命令长度*/
		addr += chNum + 3 ;
		
	}
}
/**
  * @brief  初始化,脚本中每条cmd的位置
  *  
  * @param  void
  *   
  * @retval 返回值说明,最大命令条数
  *
  * @note   pos不会变
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
RET_STATUS ScriptCmdParse(CDV_INT08U scriptNo, CDV_INT08U debug, CDV_INT08U uartNo) {
	RET_STATUS ret = OPT_SUCCESS;
//	if(timeEn/*imeN == scriptNo*/)
//		StartCdvTime(scriptNo);//开始计数
	//g_resUart = g_offlineUart;
	CMD_ARG arg;
	INIT_CLEAR(arg);
  arg.uart = uartNo;
	
	ret =  ResParse (g_run.buf[scriptNo] , g_run.len[scriptNo] , &arg);	
//	if(timeEn/*imeN == scriptNo*/)
//		timeMS[scriptNo][g_run.doNo[scriptNo]] = EndCdvTime(scriptNo);//结束计数操作超过100会死机
	CmdArgDelete(&arg);
	return ret;
}

/**
  * @brief  获取某脚本的当前命令
  *  
  * @param  scriptNo     待分析工人号
  *   
  * @retval 返回值说明
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





//////////////////////////////////////////////////////////////////////////////////////////内存专用


/**
  * @brief  mem脚本运行
  *  
  * @param  void
  *         ret   1 ，从0运行
  *               0， 继续运行
						loop 循环，1循环，0不循环
  *   
  * @retval 返回值说明OPT_SUCCESS继续执行，OPT_FAILURE停止执行
  *
  * @note   2-9号脚本
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
  * @brief  跳转到某脚本的某条命令
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   必须在初始化之后执行
  */
RET_STATUS Mem_ToScriptCmd(CDV_INT08U scriptNo , CDV_INT16U cmdNo) {
  CDV_INT32U staddr = g_run.startPos[scriptNo] - FLASH_WORK_ADDR;
	g_run.doNo[scriptNo] = cmdNo;

	g_run.cmdPos[scriptNo] = Mem_GetCmdPos(cmdNo , g_run.cmdNum[scriptNo] , staddr);
	
	Mem_ReadScriptBuf(scriptNo);
  return OPT_SUCCESS;
}
/**
  * @brief  某脚本下一条命令
  *  
  * @param  void
  *         loop 循环，1循环，0不循环
  *   
  * @retval OPT_SUCCESS 成功指向下一条命令
  *         OPT_FAILURE 失败
  *
  * @note   pos会跳转到下一条命令开始处
20151228 将原来的判断序号来决定是否脚本结束改成判断flash地址偏移大小
  */

RET_STATUS Mem_NextScriptCmd(CDV_INT08U scriptNo , CDV_INT08U loop) {
		
  CDV_INT32U staddr = g_run.startPos[scriptNo] - FLASH_WORK_ADDR;
	
	
//	if(timeEn/*imeN == scriptNo*/)
//		timeMS[scriptNo][g_run.doNo[scriptNo]] = EndCdvTime(scriptNo);//结束计数操作超过100会死机
	
	
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
		StartCdvTime(scriptNo);//开始计数
	Mem_ReadScriptBuf(scriptNo);
	
	g_run.doNo[scriptNo] = Mem_GetNoFromPos(staddr,g_run.cmdPos[scriptNo]);
	return OPT_SUCCESS;
}

/**
  * @brief  获取某脚本的当前命令
  *  
  * @param  scriptNo     待分析工人号
  *   
  * @retval 返回值说明
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
  * @brief  读生产线到内存
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   
  */
#define NEWCCM(A,len) A=mymalloc(SRAMCCM,len)
void Mem_ReadLine(void) {
	CDV_INT32U size = 0;//脚本大小，供跳转
	
	SPI_Flash_Read((CDV_INT08U*)&size, FLASH_WORK_ADDR, 4);//读取生产线大小
	if(size >= LINE_LEN)
		return;
	OfflineScriptLen = size + 4;
	

	myfree(SRAMCCM,OfflineScript);
	OfflineScript=mymalloc(SRAMCCM,size + 4);
	//NEWCCM(OfflineScript, size + 4);
	
  SPI_Flash_Read(OfflineScript, FLASH_WORK_ADDR, size + 4);//读取生产线
}	



/**
  * @brief  某脚本序号自检，输出最大有效命令条数
  *  
  * @param  void
  *   
  * @retval 返回值说明,最大有效命令条数
  *
  * @note   pos不会变
  */
CDV_INT16U Mem_TestCmdNum(CDV_INT32U flashAddr , CDV_INT16U len) {	
	CDV_INT08U chNum;
	CDV_INT16U cmdNo = 0, no;
	CDV_INT32U addr = flashAddr;
	while(addr - flashAddr < len-2) {
		/*读取当前命令长度*/
//#ifdef  _DEBUG_NPC_
//		Mem_Read(&chNum, addr + 2, 1);
//		Mem_Read((CDV_INT08U*)&no, addr, 2);
//#else
	chNum = *(CDV_INT08U*)(SCRIPT_GETADDR(addr + 2));
	//no = *(CDV_INT16U*)(SCRIPT_GETADDR(addr));//多余的
//#endif	
		
		cmdNo++;
		addr += chNum + 3 ;
	}

  return cmdNo;//返回有效命令数
}
	
/**
  * @brief  得到下一条cmd的位置
  *  
  * @param  当前命令的flash地址
						len该脚本长度
  *   
  * @retval 返回值说明,命令flash位置
  *
  * @note   
  */
CDV_INT32U Mem_GetNextCmdPos(CDV_INT32U flashAddr) {
	CDV_INT08U chNum;
	CDV_INT32U addr = flashAddr; 
//#ifdef  _DEBUG_NPC_
//	Mem_Read(&chNum, addr + 2, 1);
//#else
	chNum = *(CDV_INT08U*)(SCRIPT_GETADDR(addr + 2));
//#endif	
	addr += chNum + 3 ;

	return addr;
}

/**
  * @brief  通过cmd的位置得到第几步
  *  
  * @param  startPos 第一条命令的flash位置，即g_run.startPos[]
						currentPos 要查询的flash地址
  *   
  * @retval 返回值说明,所在脚本的第几条命令
  *
  * @note   
  */
CDV_INT16U Mem_GetNoFromPos(CDV_INT32U startPos , CDV_INT32U currentPos) {
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
	
	if (addr == currentPos) {
		return cmdNo;
	} else {
		return 0;
	}
}

/**
  * @brief  得到莫条cmd的位置
  *  
  * @param  no第几条命令
						cmdNum该脚本有几条cmd
						flashAddr该脚本起始flash
						len该脚本长度
  *   
  * @retval 返回值说明,命令flash位置
  *
  * @note   
  */
CDV_INT32U Mem_GetCmdPos(CDV_INT16U no , CDV_INT16U cmdNum , CDV_INT32U flashAddr) {
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
	
	if (no == cmdNo) {
		return addr;
	} else {
		return flashAddr;
	}	
}

/**
  * @brief  从指定mem地址读取数据
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
//			pBuffer[i]=OfflineScript[ReadAddr + i];   //循环读数  
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
  * @brief  返回mem+偏移地址
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
  * @brief  读取运行工人数据
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   包括起始脚本
						20160317添加了配方变量的初始化
  */
RET_STATUS LineInit(void) {
	CDV_INT08U i , iScr;
	CDV_INT08U runNum;
	CDV_INT16U len = 0;
	
	GetLineInfo(0);//初始化生产线
	
	if(!g_line.init) {
		return OPT_FAILURE;
	}
	
#if _NPC_VERSION_ > 2u
	CascadeModbus_MapInit();
#endif
	Mem_ReadLine();
  return OPT_SUCCESS;
}
