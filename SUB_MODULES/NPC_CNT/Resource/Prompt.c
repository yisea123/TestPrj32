/**
  ******************************************************************************
  * @file    /Prompt.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2-18-2016
  * @brief   a package of prompt function
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
	
	#include "Prompt.h"
	#define PROMPT_OVER 1
	#define PROMPT_ONGOING 0
	/*ȫ�ֱ���*/
	CDV_INT08U g_promptOver = 1;
	PROMPT_INFO g_promptInfo = {1 ,0 ,0,0};
	CDV_INT32U g_promptVar = 0;
	CDV_INT08U g_idCnt = 0; //����ʾ����ʱ����Ӧ�ò��ж��Ƿ���ͬһ����ʾ�����
	/*��������*/
	//RET_STATUS PromptSend(PROMPT_INFO promptInfo);
	RET_STATUS PromptSend(CDV_INT08U* txBuf, CDV_INT08U txLen, PROMPT_INFO prompt);
/**
  * @brief  ��ʾ�������
  *  
  * @param  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   
  */
//#define PROMPT_SEND_BUF_OFFSET      3//���溯���е�һ���궨�壬��ʾ��ǰ
RET_STATUS PromptCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo) {
	CDV_INT08U *txBuf = NULL ,len = 0 ,no /*, opt*/ , type /*, strLen*/,i;
	CDV_INT32U num;
	RET_STATUS ret = OPT_SUCCESS;
	PROMPT_INFO promptInfo;
	
	no   = rxBuf[1];//������2��
	type = rxBuf[2];//������3��FE/FF/ED
    promptInfo.over = PROMPT_ONGOING;
	promptInfo.no = no;
	promptInfo.uartNo = uartNo;
	
	switch (type ) {
		case 0xFE:
			promptInfo.varNo = rxBuf[3];//���ر���
			len = 9+1;
			NEW08U(txBuf , len);
			num = VarGet(rxBuf[3]);
			txBuf[3+1] = 0xFF;
			txBuf[4+1] = (CDV_INT08U)(num>>24);
			txBuf[5+1] = (CDV_INT08U)(num>>16);
			txBuf[6+1] = (CDV_INT08U)(num>>8);
			txBuf[7+1] = (CDV_INT08U)num;
			txBuf[8+1] = 0xED;
			ret = PromptSend(txBuf, len, promptInfo);
		    DELETE (txBuf);
		  break;
		case 0xED:
			len = 5+1;
			NEW08U(txBuf , len);
			txBuf[3+1] = 0xED;
			txBuf[4+1] = 0xED;
			ret = PromptSend(txBuf, len, promptInfo);
		    DELETE (txBuf);
			break;
		case 0xEE:
			promptInfo.varNo = rxBuf[3];//���ر���
			len = 9+1+5*(rxBuf[5 + rxBuf[4]])+1;
			NEW08U(txBuf , len);
			num = VarGet(rxBuf[3]);
			txBuf[3+1] = 0xFF;
			txBuf[4+1] = (CDV_INT08U)(num>>24);
			txBuf[5+1] = (CDV_INT08U)(num>>16);
			txBuf[6+1] = (CDV_INT08U)(num>>8);
			txBuf[7+1] = (CDV_INT08U)num;
			txBuf[8+1] = 0xFE;
		    txBuf[9+1] = rxBuf[5 + rxBuf[4]];//������
		    for(i = 0; i < txBuf[9+1]; i++ ) {
				txBuf[10+1 + i*5] = rxBuf[5 + rxBuf[4] +1 + i];//��������
				num = VarGet(txBuf[10+1 + i*5]);
				txBuf[11+1 + i*5] = (CDV_INT08U)(num>>24);
				txBuf[12+1 + i*5] = (CDV_INT08U)(num>>16);
				txBuf[13+1 + i*5] = (CDV_INT08U)(num>>8);
				txBuf[14+1 + i*5] = (CDV_INT08U)num;
			}
			ret = PromptSend(txBuf, len, promptInfo);
		    DELETE (txBuf);
			break;
		case 0xFD:
			len = 5+1+5*(rxBuf[3])+1;
			NEW08U(txBuf , len);
			txBuf[3+1] = 0xED;
			txBuf[4+1] = 0xFE;
		    txBuf[5+1] = rxBuf[3];//������
		    for(i = 0; i < txBuf[5+1]; i++ ) {
				txBuf[6+1 + i*5] = rxBuf[4 + i];//��������
				num = VarGet(txBuf[6+1 + i*5]);
				txBuf[7+1 + i*5] = (CDV_INT08U)(num>>24);
				txBuf[8+1 + i*5] = (CDV_INT08U)(num>>16);
				txBuf[9+1 + i*5] = (CDV_INT08U)(num>>8);
				txBuf[10+1 + i*5] = (CDV_INT08U)num;
			}
			ret = PromptSend(txBuf, len, promptInfo);
		    DELETE (txBuf);
			break;
		case 0xFC:
			//DW_CDV_VAR(rxBuf[3]) = g_promptVar;
		ValSet(rxBuf[3], g_promptVar);
			break;
		default:
		  ret = OPT_FAILURE;
		  break;
	}
	
	return ret;
}
/**
  * @brief  ��ʾ�����
  *  
  * @param  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   
  */
RET_STATUS PromptSend(CDV_INT08U* txBuf, CDV_INT08U txLen, PROMPT_INFO prompt) {
	OS_ERR err;
	CDV_INT08U/* *txBuf = NULL ,len = 3 ,*/i = 0,delayS = 1;
	RET_STATUS ret = OPT_SUCCESS;
	
	
	//NEW08U(txBuf , len);
	txBuf[0] = APP_ID;
	txBuf[1] = 0x03;
	txBuf[2] = prompt.no;//���ں�
	txBuf[3] = g_idCnt++;
	
	OSSemPend(&PROMPT_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	
	//if (PROMPT_OVER == g_promptInfo.over) {
	
		g_promptInfo = prompt;
		AddTx(txBuf , txLen , prompt.uartNo);
		
		i = 0;
		while (PROMPT_ONGOING == g_promptInfo.over  && CDV_STOP != g_cdvStat/* && i < delayS*/ && CDV_DEBUG_JUMP != g_cdvStat) {//�ȴ����շ���
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
			i++;
			if(i == delayS){
				AddTx(txBuf , txLen , prompt.uartNo);
				i = 0;
			}
		}
	//} else {
		
	//}
	
//	if(i == delayS)
//		ret = OPT_FAILURE;
	
	OSSemPost (&PROMPT_SEM,OS_OPT_POST_1,&err);
	//DELETE (txBuf);
	return ret;
}

/**
  * @brief  ��ʾ�����
  *  
  * @param  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   
  */
RET_STATUS PromptSend1(PROMPT_INFO prompt) {
	OS_ERR err;
	CDV_INT08U *txBuf = NULL ,len = 3 ,i = 0,delayS = 3;
	RET_STATUS ret = OPT_SUCCESS;
	
	
	NEW08U(txBuf , len);
	txBuf[0] = APP_ID;
	txBuf[1] = 0x03;
	txBuf[2] = prompt.no;//���ں�
	OSSemPend(&PROMPT_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	
	//if (PROMPT_OVER == g_promptInfo.over) {
	
		g_promptInfo = prompt;
		AddTx(txBuf , len , prompt.uartNo);
		
		i = 0;
		while (PROMPT_ONGOING == g_promptInfo.over/* && i < delayS*/) {//�ȴ����շ���
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
			i++;
			if(i == delayS){
				AddTx(txBuf , len , prompt.uartNo);
				i = 0;
			}
		}
	//} else {
		
	//}
	
//	if(i == delayS)
//		ret = OPT_FAILURE;
	
	OSSemPost (&PROMPT_SEM,OS_OPT_POST_1,&err);
	DELETE (txBuf);
	return ret;
}

/**
  * @brief  ��ʾ�������
  *         
  * @param  
  *         
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *         
  * @note   ֻ���ڽ��ܽ���debugparseһ���ط�����������
  */
RET_STATUS PromptParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen) {
	CDV_INT08U no , type , dataLen , isVarRet = 0 ;
	CDV_INT32U num;
	/*Ԥ�ж�*/
	if(PROMPT_OVER == g_promptInfo.over) {
		return OPT_FAILURE;
	}
	/*��ȡ��Ϣ*/
	no   = rxBuf[2];//��Դ��
	type = rxBuf[4];//������3��FE/FF/ED
	switch(type) {
		case 0xED:
			dataLen = 0;
			break;
		case 0xFF:
			dataLen = 4;
			num = (CDV_INT32S)((rxBuf[5]<<24) | (rxBuf[6]<<16) | (rxBuf[7]<<8) | rxBuf[8]);
			isVarRet = 1;
			break;
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
			dataLen = 0;
			num = type;
			isVarRet = 1;
			break;
		default:
			return OPT_FAILURE;
	}
	/*����*/
	if(no == g_promptInfo.no) {
		if(isVarRet)
		  //DW_CDV_VAR(g_promptInfo.varNo) = num;
		ValSet(g_promptInfo.varNo, num);
		g_promptInfo.over = PROMPT_OVER;
		if(0xEE == rxBuf[5 + dataLen])
		  g_promptVar = rxBuf[6 + dataLen];
		return OPT_SUCCESS;
	}
	
	return OPT_FAILURE;
}

/**
  * @brief  ����APP�·���INI�������
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxlen       �������ַ����ĳ���
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��DebugParse��
  */
void Ini_Cmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo){
	CDV_INT08U /*nameLen ,len = 0 ,*/ *txBuf = NULL/* ,version*/;
	CDV_STATUS tmpCdvStat;  
//	CDV_INT16U crc;
//	OS_ERR err;
	//u8 a[2] = {0};
//	CPU_SR_ALLOC();
	switch(rxBuf[2]){
		
		case 1://INI��
			g_scriptInfo.no = 1;
  		g_scriptInfo.addr = INI_LEN_ADDR(g_scriptInfo.no);
		
		  if(g_scriptInfo.addr == 0)//������ȱ�����ڵ���10
				return;
			
		  SPI_Flash_Read((CDV_INT08U *)&g_scriptInfo.len,g_scriptInfo.addr,2);

//			NEW08U(txBuf , 7);
//			memcpy(txBuf , rxBuf , 5);
//			txBuf[5] = (CDV_INT08U)(g_scriptInfo.len>>8);
//			txBuf[6] = (CDV_INT08U)(g_scriptInfo.len);
//			AddTx(txBuf , 7, uartNo);
//			DELETE(txBuf);
			
			NEW08U(txBuf , 8);
			txBuf[0] = APP_ID;
			txBuf[1] = 0x86;
			txBuf[2] = 0x01;
			txBuf[3] = 0xFF;
			
			txBuf[4] = (CDV_INT08U)(g_scriptInfo.len>>24);
			txBuf[5] = (CDV_INT08U)(g_scriptInfo.len>>16);
			txBuf[6] = (CDV_INT08U)(g_scriptInfo.len>>8);
			txBuf[7] = (CDV_INT08U)(g_scriptInfo.len);
			AddTx(txBuf , 8, uartNo);
			DELETE (txBuf);
			
			break;
		case 2://INI��
			tmpCdvStat = g_cdvStat;
			g_cdvStat = CDV_LOG_SEND;
			LED3 = LED_ON;
		  g_scriptInfo.addr = INI_ADDR(g_scriptInfo.no);
		
			if(g_scriptInfo.len != 2) {
			  Flash_Send(g_scriptInfo.addr, g_scriptInfo.len, uartNo);
			}
			LED3 = LED_OFF;
		  g_cdvStat = tmpCdvStat;
			
			break;
		
		default:
			break;
	}	
}
