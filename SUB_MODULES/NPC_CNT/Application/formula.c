/**
  ******************************************************************************
  * @file    /formula.c 
  * @author  MMY
  * @version V0.0.1
  * @date    04-27-2016
  * @brief   public define and interface of new uart control of formula
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
	
	#include "formula.h"
	VAL_SCRIPT g_val = {0}, g_formulaVal = {0};//启动变量和配方变量
	
	void AppGetFormula(CDV_INT08U uartNo);
	
	void ReadFlashCmd(CDV_INT08U **buf, CDV_INT08U *len, CDV_INT32U flashAddr);
	RET_STATUS AppFormulaValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen);
	/**
  * @brief  获取当前flash地址的命令到buf
  *  
  * @param  scriptNo     待分析工人号
  *   
  * @retval 返回值说明
  *
  * @note   
  */
void ReadFlashCmd(CDV_INT08U **buf, CDV_INT08U *len, CDV_INT32U flashAddr) {

	#ifndef VIRTUAL_LOGIC
	SPI_Flash_Read(&g_run.len[scriptNo], g_run.startPos[scriptNo] + g_run.cmdPos[scriptNo][g_run.doNo[scriptNo]] + 2, 1);
	NEW08U(g_run.buf[scriptNo] , g_run.len[scriptNo]);
  SPI_Flash_Read(g_run.buf[scriptNo], g_run.startPos[scriptNo] + g_run.cmdPos[scriptNo][g_run.doNo[scriptNo]] + 3, (CDV_INT16U)(g_run.len[scriptNo]));	
	#else
	SPI_Flash_Read(len, flashAddr + 2, 1);
	NEW08U(*buf , *len);
  SPI_Flash_Read(*buf, flashAddr + 3, (CDV_INT16U)(*len));
	#endif

}
///**
//  * @brief  初始化脚本中每条cmd val的位置
//  *  
//  * @param  void
//  *   
//  * @retval 返回值说明,最大命令条数
//  *
//  * @note   pos不会变
//						20160317改成只有启动变量
//  */
//RET_STATUS InitValCmdPos(CDV_INT16U *valPos, CDV_INT16U *valNum , CDV_INT16U cmdNum , CDV_INT32U flashAddr , CDV_INT16U len){
//	CDV_INT08U chNum , res , fc2;
//	CDV_INT16U cmdNo = 0,pos = 0,posNum =0;
//	CDV_INT32U addr = flashAddr;
//	
//	if(NULL == valPos || 0 == cmdNum) {
//		return OPT_FAILURE;
//	}
//	
//	do{
//		pos = addr -flashAddr;
//		valPos[posNum] = pos;
//		SPI_Flash_Read(&chNum, addr + 2, 1);
//		SPI_Flash_Read(&res, addr + 3, 1);
//		SPI_Flash_Read(&fc2, addr + 5, 1);
//		if(0x05 == res)//判断是否启动设置
//			if(0x00 == fc2 /*|| 0x01 == fc2*/)
//				posNum ++;
//		cmdNo++;		
//		
////		if (OPT_SUCCESS == IsLogicScript(addr)) {
////		  addr = JumpLogicScript(addr + chNum + 3);
////		} else 
//		{
//		  addr += chNum + 3 ;
//		}
//	}while(addr -flashAddr < len && cmdNo < cmdNum);
//	
//	*valNum = posNum;
//	return OPT_SUCCESS;
//}
///**
//  * @brief  分析APP下发的变量设置串口命令并操作
//  *  
//  * @param  rxBuf     待分析字符串指针
//  *         rxlen       待分析字符串的长度
//  *   
//  * @retval 返回值说明
//  *
//  * @note   在串口接收中
//  */

//RET_STATUS AppValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
//	CDV_INT08U row , type , len;
//	CDV_INT32U num;
//	row = rxBuf[2];//行号
//	type = rxBuf[3];//数值类型
//	
//	if (0xff == type) {//数值型，需要提取u32整型数值，或者是枚举型，提取枚举号
//		num = (rxBuf[4]<<24)+(rxBuf[5]<<16)+(rxBuf[6]<<8)+rxBuf[7];
//	} else {
//		num = type;
//	}
//	
//	if(g_val.num > row) {
//	  SPI_Flash_Read(&len, g_run.startPos[0] + g_val.pos[row] + 4, 1);
//	  ValSet(len , num);//修改行对应的变量值
//	  //ValSet(SCRIPT_POS_VAL_NO(0,row) , num);//修改行对应的变量值
//	}
//	return OPT_SUCCESS;
//}
///**
//  * @brief  初始化脚本中的配方变量的命令位置
//  *  
//  * @param  val 需要输出执行结果的结构体，用于保存配方变量在脚本中的位置
//						cmdNum 当前的脚本的命令条数
//						flashAddr 当前脚本的flash起始地址
//						len 当前脚本的长度
//  *   
//  * @retval 返回值说明,成功or失败
//  *
//  * @note   
//  */
//RET_STATUS InitFormulaValCmdPos(VAL_SCRIPT *val , CDV_INT16U cmdNum , CDV_INT32U flashAddr , CDV_INT16U len){
//	CDV_INT08U chNum , res , fc2;
//	CDV_INT16U cmdNo = 0,pos = 0,posNum = 0;
//	CDV_INT32U addr = flashAddr;
//	CDV_INT16U *formulaPos = NULL;//临时配方变量保存数组
//	
//	if(0 == cmdNum) {
//		return OPT_FAILURE;
//	}
//	
//	NEW16U(formulaPos, cmdNum);
//	
//	do{
//		pos = addr -flashAddr;
//		formulaPos[posNum] = pos;
//		SPI_Flash_Read(&chNum, addr + 2, 1);
//		SPI_Flash_Read(&res, addr + 3, 1);
//		SPI_Flash_Read(&fc2, addr + 5, 1);
//		if(0x05 == res)//判断是否配方变量
//			if(0x01 == fc2)
//				posNum++;
//		cmdNo++;
//		addr += chNum + 3 ;
//	} while (addr - flashAddr < len && cmdNo < cmdNum);
//	
//	//复制给数组
//	if(NULL == val->pos) {
//		NEW16U(val->pos, posNum);
//	}
//	memcpy(val->pos, formulaPos, posNum * 2);
//	val->num = posNum;
//	val->flashAddr = flashAddr;
//	
//	DELETE(formulaPos);
//	return OPT_SUCCESS;
//}

///**
//  * @brief  应用层获取配方变量
//  *  
//  * @param  scriptNo     待分析工人号
//  *   
//  * @retval 返回值说明
//  *
//  * @note   
//  */
//void AppGetFormula(CDV_INT08U uartNo) {
//	CDV_INT08U i = 0;
//	CDV_INT08U *txBuf = NULL;
//	//g_resUart = uartNo;
//	
//	if(OPT_SUCCESS != RunInit() ) {
//		return;
//	}
//	//SPI_Flash_Read(&len, g_run.startPos[0] + g_val.pos[row] + 4, 1);
//	APP_SET_NUM = 0;
//	for ( i = 0; i < g_val.num; i++){//发启动变量
//		ReadFlashCmd(&g_val.buf, &g_val.len, g_run.startPos[0]  + g_val.pos[i]);
//		ResParse (g_val.buf , g_val.len ,NULL , 0, uartNo);
//	}
//	
//	APP_SET_NUM = 0;
//	for ( i = 0; i < g_formulaVal.num; i++){//发配方变量
//		ReadFlashCmd(&g_formulaVal.buf, &g_formulaVal.len, g_formulaVal.flashAddr + g_formulaVal.pos[i]);
//		ResParse (g_formulaVal.buf , g_formulaVal.len ,NULL , 0, uartNo);
//	}
//	
//	NEW08U(txBuf , 4);//发结束
//	txBuf[0] = APP_ID;
//	txBuf[1] = 0x01;
//	txBuf[2] = 0xff;
//	txBuf[3] = 0x01;
//	AddRequestTx(txBuf , 4, uartNo);
//	DELETE (txBuf);
//}
///**
//  * @brief  应用层设置配方变量的值
//  *  
//  * @param  rxBuf     待分析字符串指针
//  *         rxlen       待分析字符串的长度
//  *   
//  * @retval 返回值说明
//  *
//  * @note   在串口接收中
//  */

//RET_STATUS AppFormulaValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
//	CDV_INT08U row , type , len;
//	CDV_INT32U num;
//	row = rxBuf[3];//行号
//	type = rxBuf[4];//数值类型
//	
//	if (0xff == type) {//数值型，需要提取u32整型数值，或者是枚举型，提取枚举号
//		num = (rxBuf[5]<<24)+(rxBuf[6]<<16)+(rxBuf[7]<<8)+rxBuf[8];
//	} else {
//		num = type;
//	}
//	
//	if(g_formulaVal.num > row) {
//	  SPI_Flash_Read(&len, g_formulaVal.flashAddr + g_formulaVal.pos[row] + 4, 1);
//	  ValFlashSet(len , num);//修改行对应的变量值
//	  //ValSet(SCRIPT_POS_VAL_NO(0,row) , num);//修改行对应的变量值
//	}
//	return OPT_SUCCESS;
//}
/**
  * @brief  分析APP下发的配方操作
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxlen       待分析字符串的长度
  *   
  * @retval 返回值说明
  *
  * @note   在DebugParse中
  */
RET_STATUS Formula_Cmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo){
	RET_STATUS ret = OPT_SUCCESS;
	
	switch(rxBuf[2]){ //功能码1筛选
		case 0://
			GetRecFormula(uartNo);
	    break;
		case 1://
			AppGetFormula(uartNo);
			break;
		case 2://
			SetRecFormula(rxBuf[3] , uartNo);
			break;
		case 3://
			AppFormulaValSet(rxBuf, rxLen);
			break;
		default://命令码无效不应答
			ret = OPT_FAILURE;
			break;
	}
	
	return ret;
}

/**
  * @brief  初始化脚本中每条cmd val的位置
  *  
  * @param  void
  *   
  * @retval 返回值说明,最大命令条数
  *
  * @note   pos不会变
						20160317改成只有启动变量
  */
RET_STATUS InitValCmdPos(void){
  CDV_INT08U no,parNo;//,i
	CDV_INT32U size = 0;//脚本大小，供跳转
	CDV_INT32U shift = 0, searchShift = 0;//偏移起始地址的字节数, 查询参数时的临时shift
	CDV_INT32U endAddr = 0,tmpAddr = 0;
	CDV_INT32U parEndAddr = 0;
	CDV_INT32U addr = GetResAddr(0x05, 0, 0, 1);//得到变量脚本的地址
	
	ClearArrayU32(&g_val.no);
	ClearArrayU32(&g_val.pos);
	ClearArrayU32(&g_formulaVal.no);
	ClearArrayU32(&g_formulaVal.pos);
	
	SPI_Flash_Read((CDV_INT08U*)&size, addr, 4);//读取种类脚本大小
	shift = 4;
	endAddr = addr + size + shift;
	shift += 1;
	//遍历变量
	while(addr + shift < endAddr) {
		tmpAddr = addr + shift;//启动变量配置地址
		SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//读取资源代号
		shift += 1;
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//读取资源脚本大小
	  shift += 2;
		searchShift = shift;
		shift += size;
		parEndAddr = addr + shift;
		//匹配启动变量
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, addr + searchShift, 1);//读取资源名称长度
		searchShift += 1;
		searchShift += size;
		while(addr + searchShift < parEndAddr) {
			SPI_Flash_Read((CDV_INT08U*)&parNo, addr + searchShift, 1);//读取参数代号
			searchShift += 1;
			size = 0;
			SPI_Flash_Read((CDV_INT08U*)&size, addr + searchShift, 2);//读取资源参数大小
			searchShift += 2;
			searchShift += size;
			
			if(parNo != 1)//是否是修改类型参数
				continue;
			if(size != 1)//长度必须是1
				continue;
			
			SPI_Flash_Read((CDV_INT08U*)&parNo, addr + searchShift - size, 1);//读取参数
			
			switch(parNo) {
				case 0x00://启动变量加入gval中
					ArrayU32Add(&g_val.no, no);
				  ArrayU32Add(&g_val.pos, tmpAddr);
					break;
				case 0x01://配方变量加入gformulaval中
					ArrayU32Add(&g_formulaVal.no, no);
				  ArrayU32Add(&g_formulaVal.pos, tmpAddr);
					break;
				default:
					break;
				
				
			}
			
		}
		
		
	}
	if(g_val.no.len != g_val.pos.len)
		return OPT_FAILURE;
		
	if(g_formulaVal.no.len != g_formulaVal.pos.len)
		return OPT_FAILURE;
	
//	for ( i = 0; i < g_formulaVal.no.len; i++){//初始化配方变量
//		ValFlashGet(g_formulaVal.no.p[i]);
//	}
	
	return OPT_SUCCESS;
}
/**
  * @brief  分析APP下发的变量设置串口命令并操作
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxlen       待分析字符串的长度
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS AppValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
	CDV_INT08U row , type/* , len*/;
	CDV_INT32U num;
	row = rxBuf[2];//行号
	type = rxBuf[3];//数值类型
	
	if (0xff == type) {//数值型，需要提取u32整型数值，或者是枚举型，提取枚举号
		num = (rxBuf[4]<<24)+(rxBuf[5]<<16)+(rxBuf[6]<<8)+rxBuf[7];
	} else {
		num = type;
	}
	
	if(g_val.pos.len > row) {
	  ValSet(g_val.no.p[row] , num);//修改行对应的变量值
	}
	return OPT_SUCCESS;
}
/**
  * @brief  应用层获取变量
  *  
  * @param  scriptNo     待分析工人号
valType  0x00启动变量
0x01 运行变量
  *   
  * @retval 返回值说明
  *
  * @note   根据valtype确定是什么变量
  */
void AppGetValType(const VAL_SCRIPT* val, const CDV_INT08U valType, const CDV_INT08U uartNo) {
	CDV_INT08U i = 0;
	CDV_INT08U *txBuf = NULL;
	CDV_INT08U len = 5;//, row ;
	CDV_INT08U no , parNo , type , strLen, parLen;
	CDV_INT32S num;
	
	CDV_INT32U size = 0;//脚本大小，供跳转
	CDV_INT32U shift = 0;//偏移起始地址的字节数, 查询参数时的临时shift
	CDV_INT32U endAddr = 0, strAddr, parAddr;
	CDV_INT32U addr;//得到变量脚本的地址

	for ( i = 0; i < val->no.len; i++){//发配方变量
		no = val->no.p[i];
		switch(valType){
			case 0x00:
		    num =0xffffffff;
		    ValSet(no , num);
			  break;
			case 0x01:
		    num = ValFlashGet(no);
			  break;
			default:
				continue;
		}
		//ValSet(no , num);
		//获取参数
		addr = val->pos.p[i];
		shift = 0;
		size = 0;
		//SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//读取资源代号
		shift += 1;
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//读取资源脚本大小
	  shift += 2;
		endAddr = addr + shift + size;
		//读取名称
		
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&strLen, addr + shift, 1);//读取资源名称长度
		shift += 1;
		strAddr = addr + shift;//名称地址保存
		//len += strLen;//发送长度增加
		
		shift += strLen;
		
		while(addr + shift < endAddr) {
			SPI_Flash_Read((CDV_INT08U*)&parNo, addr + shift, 1);//读取参数代号
			shift += 1;
			size = 0;
			SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//读取资源参数大小
			shift += 2;
			parAddr = addr + shift + 1;
			shift += size;
			
			if(parNo != 0)//是否是修改类型参数
				continue;
			
			parLen = size - 1;
			
			
			SPI_Flash_Read((CDV_INT08U*)&parNo, addr + shift - size, 1);//读取参数
			if(parNo == 0x01) {//数值
				type = 0xff;
				len = 9 + strLen;
			}
			else if(parNo == 0x00) {//枚举
				if(parLen%2  || (parLen >>1)>0x20)//枚举值的数量检查
					continue;
				
				if(0x01 == valType){//如果配方
					if(num > (parLen>>1)-1 || num < 0) {
						num = 0x00;
						ValFlashSet(no, num);
					}
					type = num;
				} else {
					type = 0x00;
				}
				len = 6 + strLen + parLen;
			}
			else
				continue;
			
			break;
			
			
		}
		
		if(addr + shift >= endAddr) {
			continue;
		}
		
		NEW08U(txBuf , len);
		txBuf[0] = TOAPP_ID;
		txBuf[1] = 0x01;
		txBuf[2] = i;//第几条设置命令
		txBuf[3] = valType;
		txBuf[4] = type;
		switch (type) {
			case 0xFF:
				txBuf[5] = (CDV_INT08U)(num>>24);
				txBuf[6] = (CDV_INT08U)(num>>16);
				txBuf[7] = (CDV_INT08U)(num>>8);
				txBuf[8] = (CDV_INT08U)num;
				SPI_Flash_Read(txBuf + 9, strAddr, strLen);
				break;
			default:
			//case 0x00:
			  txBuf[5] = (CDV_INT08U)(parLen>>1);
				SPI_Flash_Read(txBuf + 6, parAddr, parLen);
			  SPI_Flash_Read(txBuf + 6 + parLen, strAddr, strLen);
				
				break;
			
//				DELETE (txBuf);
//				continue;
		}
		AddRequestTx(txBuf , len , uartNo);
		DELETE (txBuf);
		//break;
	}
	
}
/**
  * @brief  应用层获取配方变量
  *  
  * @param  scriptNo     待分析工人号
  *   
  * @retval 返回值说明
  *
  * @note   
  */
void AppGetFormula(CDV_INT08U uartNo) {
	//CDV_INT08U i = 0;
	CDV_INT08U *txBuf = NULL;
	
	AppGetValType(&g_val, 0x00, uartNo);
	
	AppGetValType(&g_formulaVal, 0x01, uartNo);
//	CDV_INT08U len = 5;//, row ;
//	CDV_INT08U no /*, opt */, type , strLen, parLen;
//	CDV_INT32S num;
//	
//	CDV_INT32U size = 0;//脚本大小，供跳转
//	CDV_INT32U shift = 0;//偏移起始地址的字节数, 查询参数时的临时shift
//	CDV_INT32U endAddr = 0, strAddr, parAddr;
//	CDV_INT32U addr;//得到变量脚本的地址

//	//APP_SET_NUM = 0;
//	for ( i = 0; i < g_val.no.len; i++){//发启动变量
//		no = g_val.no.p[i];
//		num =0xffffffff;
//		ValSet(no , num);

//		//获取参数
//		addr = g_val.pos.p[i];
//		shift = 0;
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//读取资源代号
//		shift += 1;
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//读取资源脚本大小
//	  shift += 2;
//		endAddr = addr + shift + size;
//		//读取名称
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&strLen, addr + shift, 1);//读取资源名称长度
//		shift += 1;
//		strAddr = addr + shift;//名称地址保存
//		//len += strLen;//发送长度增加
//		
//		shift += strLen;
//		
//		while(addr + shift < endAddr) {
//			SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//读取参数代号
//			shift += 1;
//			size = 0;
//			SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//读取资源参数大小
//			shift += 2;
//			parAddr = addr + shift + 1;
//			shift += size;
//			
//			if(no != 0)//是否是修改类型参数
//				continue;
//			
//			parLen = size - 1;
//			
//			
//			SPI_Flash_Read((CDV_INT08U*)&no, addr + shift - size, 1);//读取参数
//			if(no == 0x01) {
//				type = 0xff;
//				len = 9 + strLen;
//			}
//			else if(no == 0x00) {
//				if(parLen%2)
//					continue;
//				type = 0x00;
//				len = 6 + strLen + parLen;
//			}
//			else
//				continue;
//			
//			break;
//			
//			
//		}
//		
//		if(addr + shift >= endAddr) {
//			continue;
//		}
//		
//		
//		NEW08U(txBuf , len);
//		txBuf[0] = TOAPP_ID;
//		txBuf[1] = 0x01;
//		txBuf[2] = i;//第几条设置命令
//		txBuf[3] = 0x00;
//		txBuf[4] = type;
//		switch (type) {
//			case 0xFF:
//				txBuf[5] = (CDV_INT08U)(num>>24);
//				txBuf[6] = (CDV_INT08U)(num>>16);
//				txBuf[7] = (CDV_INT08U)(num>>8);
//				txBuf[8] = (CDV_INT08U)num;
//				SPI_Flash_Read(txBuf + 9, strAddr, strLen);
//				break;
//			default:
//			//case 0x00:
//			  txBuf[5] = (CDV_INT08U)(parLen>>1);
//				SPI_Flash_Read(txBuf + 6, parAddr, parLen);
//			  SPI_Flash_Read(txBuf + 6 + parLen, strAddr, strLen);
//				break;
//			
////				DELETE (txBuf);
////				continue;
//		}
//		AddRequestTx(txBuf , len , uartNo);
//		DELETE (txBuf);
//		//break;
//	}
//	
//	//APP_SET_NUM = 0;
//	for ( i = 0; i < g_formulaVal.no.len; i++){//发配方变量
//		no = g_formulaVal.no.p[0];

//		num = ValFlashGet(no);
//			
//		//ValSet(no , num);
//		//获取参数
//		addr = g_formulaVal.pos.p[i];
//		shift = 0;
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//读取资源代号
//		shift += 1;
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//读取资源脚本大小
//	  shift += 2;
//		endAddr = addr + shift + size;
//		//读取名称
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&strLen, addr + shift, 1);//读取资源名称长度
//		shift += 1;
//		strAddr = addr + shift;//名称地址保存
//		//len += strLen;//发送长度增加
//		
//		shift += strLen;
//		
//		while(addr + shift < endAddr) {
//			SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//读取参数代号
//			shift += 1;
//			size = 0;
//			SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//读取资源参数大小
//			shift += 2;
//			parAddr = addr + shift + 1;
//			shift += size;
//			
//			if(no != 0)//是否是修改类型参数
//				continue;
//			
//			parLen = size - 1;
//			
//			
//			SPI_Flash_Read((CDV_INT08U*)&no, addr + shift - size, 1);//读取参数
//			if(no == 0x01) {
//				type = 0xff;
//				len = 9 + strLen;
//			}
//			else if(no == 0x00) {
//				if(parLen%2)
//					continue;
//				type = 0x00;
//				len = 6 + strLen + parLen;
//			}
//			else
//				continue;
//			
//			break;
//			
//			
//		}
//		
//		if(addr + shift >= endAddr) {
//			continue;
//		}
//		
//		NEW08U(txBuf , len);
//		txBuf[0] = TOAPP_ID;
//		txBuf[1] = 0x01;
//		txBuf[2] = i;//第几条设置命令
//		txBuf[3] = 0x01;
//		txBuf[4] = type;
//		switch (type) {
//			case 0xFF:
//				txBuf[5] = (CDV_INT08U)(num>>24);
//				txBuf[6] = (CDV_INT08U)(num>>16);
//				txBuf[7] = (CDV_INT08U)(num>>8);
//				txBuf[8] = (CDV_INT08U)num;
//				SPI_Flash_Read(txBuf + 9, strAddr, strLen);
//				break;
//			default:
//			//case 0x00:
//			  txBuf[5] = (CDV_INT08U)(parLen>>1);
//				SPI_Flash_Read(txBuf + 6, parAddr, parLen);
//			  SPI_Flash_Read(txBuf + 6 + parLen, strAddr, strLen);
//				
//				break;
//			
////				DELETE (txBuf);
////				continue;
//		}
//		AddRequestTx(txBuf , len , uartNo);
//		DELETE (txBuf);
//		//break;
//	}
	
	NEW08U(txBuf , 4);//发结束
	txBuf[0] = TOAPP_ID;
	txBuf[1] = 0x01;
	txBuf[2] = 0xff;
	txBuf[3] = 0x01;
	AddRequestTx(txBuf , 4, uartNo);
	DELETE (txBuf);
}
/**
  * @brief  应用层设置配方变量的值
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxlen       待分析字符串的长度
  *   
  * @retval 返回值说明
  *
  * @note   在串口接收中
  */

RET_STATUS AppFormulaValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
	CDV_INT08U row , type/* , len*/;
	CDV_INT32U num;
	row = rxBuf[3];//行号
	type = rxBuf[4];//数值类型
	
	if (0xff == type) {//数值型，需要提取u32整型数值，或者是枚举型，提取枚举号
		num = (rxBuf[5]<<24)+(rxBuf[6]<<16)+(rxBuf[7]<<8)+rxBuf[8];
	} else {
		num = type;
	}
	
	if(g_formulaVal.pos.len > row) {
	  ValFlashSet(g_formulaVal.no.p[row] , num);//修改行对应的变量值
	}
	
	return OPT_SUCCESS;
}
