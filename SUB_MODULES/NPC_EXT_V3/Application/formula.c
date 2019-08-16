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
	VAL_SCRIPT g_val = {0}, g_formulaVal = {0};//�����������䷽����
	
	void AppGetFormula(CDV_INT08U uartNo);
	
	void ReadFlashCmd(CDV_INT08U **buf, CDV_INT08U *len, CDV_INT32U flashAddr);
	RET_STATUS AppFormulaValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen);
	/**
  * @brief  ��ȡ��ǰflash��ַ�����buf
  *  
  * @param  scriptNo     ���������˺�
  *   
  * @retval ����ֵ˵��
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
//  * @brief  ��ʼ���ű���ÿ��cmd val��λ��
//  *  
//  * @param  void
//  *   
//  * @retval ����ֵ˵��,�����������
//  *
//  * @note   pos�����
//						20160317�ĳ�ֻ����������
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
//		if(0x05 == res)//�ж��Ƿ���������
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
//  * @brief  ����APP�·��ı������ô����������
//  *  
//  * @param  rxBuf     �������ַ���ָ��
//  *         rxlen       �������ַ����ĳ���
//  *   
//  * @retval ����ֵ˵��
//  *
//  * @note   �ڴ��ڽ�����
//  */

//RET_STATUS AppValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
//	CDV_INT08U row , type , len;
//	CDV_INT32U num;
//	row = rxBuf[2];//�к�
//	type = rxBuf[3];//��ֵ����
//	
//	if (0xff == type) {//��ֵ�ͣ���Ҫ��ȡu32������ֵ��������ö���ͣ���ȡö�ٺ�
//		num = (rxBuf[4]<<24)+(rxBuf[5]<<16)+(rxBuf[6]<<8)+rxBuf[7];
//	} else {
//		num = type;
//	}
//	
//	if(g_val.num > row) {
//	  SPI_Flash_Read(&len, g_run.startPos[0] + g_val.pos[row] + 4, 1);
//	  ValSet(len , num);//�޸��ж�Ӧ�ı���ֵ
//	  //ValSet(SCRIPT_POS_VAL_NO(0,row) , num);//�޸��ж�Ӧ�ı���ֵ
//	}
//	return OPT_SUCCESS;
//}
///**
//  * @brief  ��ʼ���ű��е��䷽����������λ��
//  *  
//  * @param  val ��Ҫ���ִ�н���Ľṹ�壬���ڱ����䷽�����ڽű��е�λ��
//						cmdNum ��ǰ�Ľű�����������
//						flashAddr ��ǰ�ű���flash��ʼ��ַ
//						len ��ǰ�ű��ĳ���
//  *   
//  * @retval ����ֵ˵��,�ɹ�orʧ��
//  *
//  * @note   
//  */
//RET_STATUS InitFormulaValCmdPos(VAL_SCRIPT *val , CDV_INT16U cmdNum , CDV_INT32U flashAddr , CDV_INT16U len){
//	CDV_INT08U chNum , res , fc2;
//	CDV_INT16U cmdNo = 0,pos = 0,posNum = 0;
//	CDV_INT32U addr = flashAddr;
//	CDV_INT16U *formulaPos = NULL;//��ʱ�䷽������������
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
//		if(0x05 == res)//�ж��Ƿ��䷽����
//			if(0x01 == fc2)
//				posNum++;
//		cmdNo++;
//		addr += chNum + 3 ;
//	} while (addr - flashAddr < len && cmdNo < cmdNum);
//	
//	//���Ƹ�����
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
//  * @brief  Ӧ�ò��ȡ�䷽����
//  *  
//  * @param  scriptNo     ���������˺�
//  *   
//  * @retval ����ֵ˵��
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
//	for ( i = 0; i < g_val.num; i++){//����������
//		ReadFlashCmd(&g_val.buf, &g_val.len, g_run.startPos[0]  + g_val.pos[i]);
//		ResParse (g_val.buf , g_val.len ,NULL , 0, uartNo);
//	}
//	
//	APP_SET_NUM = 0;
//	for ( i = 0; i < g_formulaVal.num; i++){//���䷽����
//		ReadFlashCmd(&g_formulaVal.buf, &g_formulaVal.len, g_formulaVal.flashAddr + g_formulaVal.pos[i]);
//		ResParse (g_formulaVal.buf , g_formulaVal.len ,NULL , 0, uartNo);
//	}
//	
//	NEW08U(txBuf , 4);//������
//	txBuf[0] = APP_ID;
//	txBuf[1] = 0x01;
//	txBuf[2] = 0xff;
//	txBuf[3] = 0x01;
//	AddRequestTx(txBuf , 4, uartNo);
//	DELETE (txBuf);
//}
///**
//  * @brief  Ӧ�ò������䷽������ֵ
//  *  
//  * @param  rxBuf     �������ַ���ָ��
//  *         rxlen       �������ַ����ĳ���
//  *   
//  * @retval ����ֵ˵��
//  *
//  * @note   �ڴ��ڽ�����
//  */

//RET_STATUS AppFormulaValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
//	CDV_INT08U row , type , len;
//	CDV_INT32U num;
//	row = rxBuf[3];//�к�
//	type = rxBuf[4];//��ֵ����
//	
//	if (0xff == type) {//��ֵ�ͣ���Ҫ��ȡu32������ֵ��������ö���ͣ���ȡö�ٺ�
//		num = (rxBuf[5]<<24)+(rxBuf[6]<<16)+(rxBuf[7]<<8)+rxBuf[8];
//	} else {
//		num = type;
//	}
//	
//	if(g_formulaVal.num > row) {
//	  SPI_Flash_Read(&len, g_formulaVal.flashAddr + g_formulaVal.pos[row] + 4, 1);
//	  ValFlashSet(len , num);//�޸��ж�Ӧ�ı���ֵ
//	  //ValSet(SCRIPT_POS_VAL_NO(0,row) , num);//�޸��ж�Ӧ�ı���ֵ
//	}
//	return OPT_SUCCESS;
//}
/**
  * @brief  ����APP�·����䷽����
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxlen       �������ַ����ĳ���
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��DebugParse��
  */
RET_STATUS Formula_Cmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo){
	RET_STATUS ret = OPT_SUCCESS;
	
	switch(rxBuf[2]){ //������1ɸѡ
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
		default://��������Ч��Ӧ��
			ret = OPT_FAILURE;
			break;
	}
	
	return ret;
}

/**
  * @brief  ��ʼ���ű���ÿ��cmd val��λ��
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��,�����������
  *
  * @note   pos�����
						20160317�ĳ�ֻ����������
  */
RET_STATUS InitValCmdPos(void){
  CDV_INT08U no,parNo;//,i
	CDV_INT32U size = 0;//�ű���С������ת
	CDV_INT32U shift = 0, searchShift = 0;//ƫ����ʼ��ַ���ֽ���, ��ѯ����ʱ����ʱshift
	CDV_INT32U endAddr = 0,tmpAddr = 0;
	CDV_INT32U parEndAddr = 0;
	CDV_INT32U addr = GetResAddr(0x05, 0, 0, 1);//�õ������ű��ĵ�ַ
	
	ClearArrayU32(&g_val.no);
	ClearArrayU32(&g_val.pos);
	ClearArrayU32(&g_formulaVal.no);
	ClearArrayU32(&g_formulaVal.pos);
	
	SPI_Flash_Read((CDV_INT08U*)&size, addr, 4);//��ȡ����ű���С
	shift = 4;
	endAddr = addr + size + shift;
	shift += 1;
	//��������
	while(addr + shift < endAddr) {
		tmpAddr = addr + shift;//�����������õ�ַ
		SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//��ȡ��Դ����
		shift += 1;
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//��ȡ��Դ�ű���С
	  shift += 2;
		searchShift = shift;
		shift += size;
		parEndAddr = addr + shift;
		//ƥ����������
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, addr + searchShift, 1);//��ȡ��Դ���Ƴ���
		searchShift += 1;
		searchShift += size;
		while(addr + searchShift < parEndAddr) {
			SPI_Flash_Read((CDV_INT08U*)&parNo, addr + searchShift, 1);//��ȡ��������
			searchShift += 1;
			size = 0;
			SPI_Flash_Read((CDV_INT08U*)&size, addr + searchShift, 2);//��ȡ��Դ������С
			searchShift += 2;
			searchShift += size;
			
			if(parNo != 1)//�Ƿ����޸����Ͳ���
				continue;
			if(size != 1)//���ȱ�����1
				continue;
			
			SPI_Flash_Read((CDV_INT08U*)&parNo, addr + searchShift - size, 1);//��ȡ����
			
			switch(parNo) {
				case 0x00://������������gval��
					ArrayU32Add(&g_val.no, no);
				  ArrayU32Add(&g_val.pos, tmpAddr);
					break;
				case 0x01://�䷽��������gformulaval��
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
	
//	for ( i = 0; i < g_formulaVal.no.len; i++){//��ʼ���䷽����
//		ValFlashGet(g_formulaVal.no.p[i]);
//	}
	
	return OPT_SUCCESS;
}
/**
  * @brief  ����APP�·��ı������ô����������
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxlen       �������ַ����ĳ���
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS AppValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
	CDV_INT08U row , type/* , len*/;
	CDV_INT32U num;
	row = rxBuf[2];//�к�
	type = rxBuf[3];//��ֵ����
	
	if (0xff == type) {//��ֵ�ͣ���Ҫ��ȡu32������ֵ��������ö���ͣ���ȡö�ٺ�
		num = (rxBuf[4]<<24)+(rxBuf[5]<<16)+(rxBuf[6]<<8)+rxBuf[7];
	} else {
		num = type;
	}
	
	if(g_val.pos.len > row) {
	  ValSet(g_val.no.p[row] , num);//�޸��ж�Ӧ�ı���ֵ
	}
	return OPT_SUCCESS;
}
/**
  * @brief  Ӧ�ò��ȡ����
  *  
  * @param  scriptNo     ���������˺�
valType  0x00��������
0x01 ���б���
  *   
  * @retval ����ֵ˵��
  *
  * @note   ����valtypeȷ����ʲô����
  */
void AppGetValType(const VAL_SCRIPT* val, const CDV_INT08U valType, const CDV_INT08U uartNo) {
	CDV_INT08U i = 0;
	CDV_INT08U *txBuf = NULL;
	CDV_INT08U len = 5;//, row ;
	CDV_INT08U no , parNo , type , strLen, parLen;
	CDV_INT32S num;
	
	CDV_INT32U size = 0;//�ű���С������ת
	CDV_INT32U shift = 0;//ƫ����ʼ��ַ���ֽ���, ��ѯ����ʱ����ʱshift
	CDV_INT32U endAddr = 0, strAddr, parAddr;
	CDV_INT32U addr;//�õ������ű��ĵ�ַ

	for ( i = 0; i < val->no.len; i++){//���䷽����
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
		//��ȡ����
		addr = val->pos.p[i];
		shift = 0;
		size = 0;
		//SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//��ȡ��Դ����
		shift += 1;
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//��ȡ��Դ�ű���С
	  shift += 2;
		endAddr = addr + shift + size;
		//��ȡ����
		
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&strLen, addr + shift, 1);//��ȡ��Դ���Ƴ���
		shift += 1;
		strAddr = addr + shift;//���Ƶ�ַ����
		//len += strLen;//���ͳ�������
		
		shift += strLen;
		
		while(addr + shift < endAddr) {
			SPI_Flash_Read((CDV_INT08U*)&parNo, addr + shift, 1);//��ȡ��������
			shift += 1;
			size = 0;
			SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//��ȡ��Դ������С
			shift += 2;
			parAddr = addr + shift + 1;
			shift += size;
			
			if(parNo != 0)//�Ƿ����޸����Ͳ���
				continue;
			
			parLen = size - 1;
			
			
			SPI_Flash_Read((CDV_INT08U*)&parNo, addr + shift - size, 1);//��ȡ����
			if(parNo == 0x01) {//��ֵ
				type = 0xff;
				len = 9 + strLen;
			}
			else if(parNo == 0x00) {//ö��
				if(parLen%2  || (parLen >>1)>0x20)//ö��ֵ���������
					continue;
				
				if(0x01 == valType){//����䷽
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
		txBuf[2] = i;//�ڼ�����������
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
  * @brief  Ӧ�ò��ȡ�䷽����
  *  
  * @param  scriptNo     ���������˺�
  *   
  * @retval ����ֵ˵��
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
//	CDV_INT32U size = 0;//�ű���С������ת
//	CDV_INT32U shift = 0;//ƫ����ʼ��ַ���ֽ���, ��ѯ����ʱ����ʱshift
//	CDV_INT32U endAddr = 0, strAddr, parAddr;
//	CDV_INT32U addr;//�õ������ű��ĵ�ַ

//	//APP_SET_NUM = 0;
//	for ( i = 0; i < g_val.no.len; i++){//����������
//		no = g_val.no.p[i];
//		num =0xffffffff;
//		ValSet(no , num);

//		//��ȡ����
//		addr = g_val.pos.p[i];
//		shift = 0;
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//��ȡ��Դ����
//		shift += 1;
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//��ȡ��Դ�ű���С
//	  shift += 2;
//		endAddr = addr + shift + size;
//		//��ȡ����
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&strLen, addr + shift, 1);//��ȡ��Դ���Ƴ���
//		shift += 1;
//		strAddr = addr + shift;//���Ƶ�ַ����
//		//len += strLen;//���ͳ�������
//		
//		shift += strLen;
//		
//		while(addr + shift < endAddr) {
//			SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//��ȡ��������
//			shift += 1;
//			size = 0;
//			SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//��ȡ��Դ������С
//			shift += 2;
//			parAddr = addr + shift + 1;
//			shift += size;
//			
//			if(no != 0)//�Ƿ����޸����Ͳ���
//				continue;
//			
//			parLen = size - 1;
//			
//			
//			SPI_Flash_Read((CDV_INT08U*)&no, addr + shift - size, 1);//��ȡ����
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
//		txBuf[2] = i;//�ڼ�����������
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
//	for ( i = 0; i < g_formulaVal.no.len; i++){//���䷽����
//		no = g_formulaVal.no.p[0];

//		num = ValFlashGet(no);
//			
//		//ValSet(no , num);
//		//��ȡ����
//		addr = g_formulaVal.pos.p[i];
//		shift = 0;
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//��ȡ��Դ����
//		shift += 1;
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//��ȡ��Դ�ű���С
//	  shift += 2;
//		endAddr = addr + shift + size;
//		//��ȡ����
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&strLen, addr + shift, 1);//��ȡ��Դ���Ƴ���
//		shift += 1;
//		strAddr = addr + shift;//���Ƶ�ַ����
//		//len += strLen;//���ͳ�������
//		
//		shift += strLen;
//		
//		while(addr + shift < endAddr) {
//			SPI_Flash_Read((CDV_INT08U*)&no, addr + shift, 1);//��ȡ��������
//			shift += 1;
//			size = 0;
//			SPI_Flash_Read((CDV_INT08U*)&size, addr + shift, 2);//��ȡ��Դ������С
//			shift += 2;
//			parAddr = addr + shift + 1;
//			shift += size;
//			
//			if(no != 0)//�Ƿ����޸����Ͳ���
//				continue;
//			
//			parLen = size - 1;
//			
//			
//			SPI_Flash_Read((CDV_INT08U*)&no, addr + shift - size, 1);//��ȡ����
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
//		txBuf[2] = i;//�ڼ�����������
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
	
	NEW08U(txBuf , 4);//������
	txBuf[0] = TOAPP_ID;
	txBuf[1] = 0x01;
	txBuf[2] = 0xff;
	txBuf[3] = 0x01;
	AddRequestTx(txBuf , 4, uartNo);
	DELETE (txBuf);
}
/**
  * @brief  Ӧ�ò������䷽������ֵ
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxlen       �������ַ����ĳ���
  *   
  * @retval ����ֵ˵��
  *
  * @note   �ڴ��ڽ�����
  */

RET_STATUS AppFormulaValSet(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
	CDV_INT08U row , type/* , len*/;
	CDV_INT32U num;
	row = rxBuf[3];//�к�
	type = rxBuf[4];//��ֵ����
	
	if (0xff == type) {//��ֵ�ͣ���Ҫ��ȡu32������ֵ��������ö���ͣ���ȡö�ٺ�
		num = (rxBuf[5]<<24)+(rxBuf[6]<<16)+(rxBuf[7]<<8)+rxBuf[8];
	} else {
		num = type;
	}
	
	if(g_formulaVal.pos.len > row) {
	  ValFlashSet(g_formulaVal.no.p[row] , num);//�޸��ж�Ӧ�ı���ֵ
	}
	
	return OPT_SUCCESS;
}
