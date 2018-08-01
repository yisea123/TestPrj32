/**
  ******************************************************************************
  * @file    /UniversalSCom.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-4-5
  * @brief   ͨ�ô���
  * 
@verbatim  

@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	
	#include "UniversalSCom.h"



/** @brief  ����id��ȡ���ں�
  * @param  ID
  * @retval ���ں�
  * @note   
  */
CDV_INT08U GetComNoFromID(CDV_INT08U id)   
{ 
	CDV_INT08U no = 0;
	
	switch (id)
	{
		case 1:
			no = 1;//no = 6;
			break;
		case 2:
			no = 1;
		  break;
		default:
			break;
	}

 	return no;
}
	

/** @brief  ���ʧ�ܼ���ֵ
  * @param  
  * @retval 
  * @note   
  */
void ClearUniversalCnt(void)   
{ 
	CDV_INT08U* UniversalCnt = UserMemPtr(CCM_UNIVERSALSCOM_CNT);
	memset(UniversalCnt, 0, 256);
}
	

/** @brief  ��������ͨѶ��������
  * @param  rxBuf  �����ַ���
	*         rxLen  ����
	*         uartNo ���������ĸ�����
  * @retval 
  * @note   
  */
RET_STATUS UniSerialComSetCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg)   
{ 
	RET_STATUS ret = OPT_FAILURE;
	CDV_INT08U no = 0;
	CDV_INT08U type = rxBuf[0];
	CDV_INT08U id = rxBuf[1];
	struct COM_PARAM param;
	param.bound = *(CDV_INT32U*)(rxBuf + 2);
	param.wordLength = *(CDV_INT32U*)(rxBuf + 6);
	param.stopBits = *(CDV_INT32U*)(rxBuf + 10);
	param.parity = *(CDV_INT32U*)(rxBuf + 14);
	
	switch (type)
	{
		case 0://����485
			no = GetComNoFromID(id);
		
		  if (OPT_SUCCESS == (ret = CorrectComParam(&param, no))){
			  USARTSet(param.bound, param.wordLength, param.stopBits, param.parity, no);
				
					ResRequest(arg->buf, arg->len, 0,0,arg, RC_CRC);
			}
			
			break;
		case 1:
			no = GetComNoFromID(id);
			SPI_Flash_Read((CDV_INT08U *)&param, USART_SET_ADDR(no - 1), sizeof(param));
			if (OPT_SUCCESS == (ret = ReturnComParam(&param, no)))
		    ResRequest(arg->buf, arg->len, (CDV_INT08U*)&param, sizeof(param), arg, RC_CRC);
			break;
		default:
			break;
	}
	
 	return ret;
}

/**
  * @brief  CSB ���� ת��
  *  
  * @param  ID     Ҫ������CSB�豸��
  *   
  * @retval void
  *
  * @note   
  */
RET_STATUS UniSerialSendCRC(const u8* txBuf, const u8 txLen,u8* rxBuf ,u8 rxbufLen,u8* rxLen , const CDV_INT08U uart)
{
	OS_ERR err;
	CDV_INT32S val = 0;
	u16 data;
	u8* sendBuf = NULL;
	u8 i = 0;
	
//	ASSERT(NULL != txBuf);
//	ASSERT(0 != txLen);
//	ASSERT(NULL != rxBuf);
//	ASSERT(0 != rxbufLen);
//	ASSERT(NULL != rxLen);
	
	if((NULL == txBuf) || (0 == txLen) || (NULL == rxBuf) || (0 == rxbufLen) || (NULL == rxLen))
		return OPT_FAILURE;
	
	NEW08U(sendBuf, txLen + 2);
	
	MemCpy(sendBuf, txBuf, txLen);
	
	data=getCRC16(sendBuf,txLen);
	sendBuf[txLen]=data & 0x00ff;
  sendBuf[txLen + 1]=(data >> 8) & 0x00ff;
	USARTTR(sendBuf ,txLen + 2 ,rxBuf ,rxbufLen , rxLen , uart);
	
	DELETE(sendBuf);

	if(*rxLen > 2) {
		data=getCRC16(rxBuf,*rxLen-2);
		if((rxBuf[*rxLen-2]==(data & 0x00ff))
			&& (rxBuf[*rxLen-1]==((data >> 8) & 0x00ff))) //crc
		{
			return OPT_SUCCESS;
		}
		else
		{
			return OPT_FAILURE;
		}
	}
	else
	{
		return OPT_FAILURE;
	}
}


///** @brief  ͨ�ô���modbus����
//  * @param  buf     �������ַ���ָ��
//  *         len     �������ַ����ĳ���
//  * @retval 
//  * @note   ���ֶζ�����ο���ͨ������.docx��
//  */
//RET_STATUS UniSerialModbusParse(CDV_INT08U* buf, CDV_INT08U len, CDV_INT08U uart, CMD_ARG *arg)
//{
//	CDV_INT08U* cmdBuf = NULL;
//	CDV_INT08U  cmdLen = 0;
//	
//	RET_STATUS ret       = OPT_FAILURE;	
//	//CDV_INT08U native    = CascadeGetNativeNo(); //������
//	CDV_INT08U devAddr   = buf[1];//����id
//	CDV_INT08U res       = buf[2];//�˿ں�
//	/*�����ֶ�*/
//	CDV_INT08U ftmp1     = buf[3];
//	CDV_INT08U ftmp2     = buf[4];
//	
//	CDV_INT08U fc        = buf[5];            /*������*/
//	CDV_INT08U regPos    = buf[6];            /*�Ĵ���λ��*/
//	CDV_INT08U datLen    = buf[7];            /*����λ����*/
//	CDV_INT08U* p_dat    = buf + 8;           /*��������*/
//	/*�����ֶ�*/
//	CDV_INT08U btmp1     = buf[8 + (datLen?(len - 11):4)];//(datLen?datLen<<ftmp1:2)];//datLen:0����4B��1��ֵ��len-11��B
//	CDV_INT08U prot      = buf[9 + (datLen?(len - 11):4)];   /*Э��*/
//	CDV_INT08U* p_pp     = buf + 10 + (datLen?(len - 11):4); /*Э�����*/
//	
//	u8 recv_buf[100];
//	u8 recv_len=0;

//	CDV_INT08U num     = 0;            /*�������ݳ���*/
//	CDV_INT08U* p_val  = NULL;              /*��������*/
//	CDV_INT08U tmp[4] = {0};
//	CDV_INT32S tmpVar = 0;
//	CDV_INT08U uartNo;
//	
//	ASSERT(NULL != buf);
//	ASSERT(0 != len);
//	
//	if(0x00 != prot)
//		return ret;
//	
//	//ѡ��˿ں�
////	switch(res)
////	{
////		case 1:
////			uartNo = 6;
////			break;
////		case 2:
////			uartNo = 6;
////			break;
////		default:
////			uartNo = 6;
////			break;
////		
////	}
//	uartNo = GetComNoFromID(res);
//	
//	if(!datLen) {                   /*0x00����*/
//		num = ftmp1;
//		tmpVar = VarGet(*(CDV_INT32U*)p_dat);
//		tmp[0] = tmpVar >> 8;
//		tmp[1] = tmpVar;
//		tmp[2] = tmpVar >> 24;
//		tmp[3] = tmpVar >> 16;
//		p_val = tmp;
//	} else {
//		num = (len - 11)>>1;//ftmp1;//datLen>>1;
//		p_val = p_dat;
//	}
//	
//	switch(fc) {
//		case 0x10:                        /*write reg*/
//			WriteRegisterCmd(devAddr, regPos, num, p_val, &cmdBuf, &cmdLen);
//			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
//			break;
//		case 0x03:                        /*read reg*/
//			ReadRegisterCmd(devAddr, regPos, num, &cmdBuf, &cmdLen);
//			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
//			//���������ҽ��ճɹ�
//			if(!datLen && OPT_SUCCESS == ret && devAddr == recv_buf[0] && 0x03 == recv_buf[1]) {
//				ret = ReadRegReqToVar(recv_buf, recv_len, 0, *(CDV_INT32U*)p_dat);
//			}
//			break;
//		case 0x04:                        /*read inreg*/
//			ReadInRegisterCmd(devAddr, regPos, num, &cmdBuf, &cmdLen);
//			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
//			//���������ҽ��ճɹ�
//			if(!datLen && OPT_SUCCESS == ret && devAddr == recv_buf[0] && 0x04 == recv_buf[1]) {
//				ret = ReadInRegReqToVar(recv_buf, recv_len, 0, *(CDV_INT32U*)p_dat);
//			}
//			break;
//		case 0x05:                        /*write coil*/
//			WriteCoilCmd(devAddr, regPos, *p_val, &cmdBuf, &cmdLen);
//			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
//			break;
//		case 0x02:                        /*read incoil*/
//			ReadInCoilCmd(devAddr, regPos, num, &cmdBuf, &cmdLen);
//			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
//			//���������ҽ��ճɹ�
//			if(!datLen && OPT_SUCCESS == ret && devAddr == recv_buf[0] && 0x02 == recv_buf[1]) {
//				ret = ReadInCoilReqToVar(recv_buf, recv_len, 0, *(CDV_INT32U*)p_dat);
//			}
//			break;
//		case 0x01:                        /*read coil*/
//			ReadCoilCmd(devAddr, regPos, num, &cmdBuf, &cmdLen);
//			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
//			//���������ҽ��ճɹ�
//			if(!datLen && OPT_SUCCESS == ret && devAddr == recv_buf[0] && 0x01 == recv_buf[1]) {
//				ret = ReadCoilReqToVar(recv_buf, recv_len, 0, *(CDV_INT32U*)p_dat);
//			}
//			break;
//		default:
//			break;
//	}
//	//�ظ����������ֵ
//	if(!datLen)
//	{
//		tmpVar = VarGet(*(CDV_INT32U*)p_dat);
//		ResRequest(arg->buf, arg->len, (CDV_INT08U*)&tmpVar, 4, arg);
//	}
//	else
//	{
//		ResRequest(arg->buf, arg->len, 0, 0, arg);
//	}
//	DELETE(cmdBuf);
//	return ret;
//}

/** @brief  ͨ�ô���modbus����
  * @param  buf     �������ַ���ָ��
  *         len     �������ַ����ĳ���
  * @retval 
  * @note   ���ֶζ�����ο���ͨ������.docx��
  */
RET_STATUS UniSerialModbusParse(CDV_INT08U* buf, CDV_INT08U len, CDV_INT08U uart, CMD_ARG *arg)
{
	CDV_INT08U* cmdBuf = NULL;
	CDV_INT08U  cmdLen = 0;
	
	RET_STATUS ret       = OPT_FAILURE;	
	//CDV_INT08U native    = CascadeGetNativeNo(); //������
	CDV_INT08U devAddr   = buf[1];//����id
	CDV_INT08U res       = buf[2];//�˿ں�
	/*�����ֶ�*/
	CDV_INT08U ftmp1     = buf[3];//�Ĵ���λ��
	CDV_INT08U ftmp2     = buf[4];//�󶨱�����
	
	CDV_INT08U fc        = buf[5];            /*������*/
	CDV_INT16U regPos    = (buf[6]<<8) + buf[7];            /*�Ĵ���λ��*/
	CDV_INT08U datLen    = buf[8];            /*����λ����*/
	CDV_INT08U* p_dat    = buf + 9;           /*��������*/
	/*�����ֶ�*/
	CDV_INT08U btmp1     = buf[9 + (len - 12)];//(datLen?datLen<<ftmp1:2)];//datLen:0����4B��1��ֵ��len-11��B
	CDV_INT08U prot      = buf[10 + (len - 12)];   /*Э��*/
	CDV_INT08U* p_pp     = buf + 11 + (len - 12); /*Э�����*/
	
	CDV_INT08U isRead = 0;
	
	u8 recv_buf[100];
	u8 recv_len=0;

	CDV_INT08U num     = 0;            /*�������ݳ���*/
	CDV_INT08U* p_val  = NULL;              /*��������*/
	CDV_INT08U tmp[4] = {0};
	CDV_INT32S tmpVar = 0;
	CDV_INT08U uartNo;
		
  CDV_INT08U* UniversalCnt = UserMemPtr(CCM_UNIVERSALSCOM_CNT);
	
	ASSERT(NULL != buf);
	ASSERT(0 != len);
	ASSERT(len>=11);
	
	if(0x00 != prot)
		return ret;
	
	if(5 < UniversalCnt[devAddr]) {
		UniversalCnt[devAddr]++;
		//delay_ms(20);
//		if(0x01 == fc || 0x02 == fc || 0x03 == fc || 0x04 == fc)
//		{
//			ValSet(ftmp2, -1);//�˲������п��ޣ����������
//		}
//		SET_INCOIL_ADDR(99+devAddr);
		return OPT_SUCCESS;//ͨѶ��ͨ������
	}	
	
	uartNo = GetComNoFromID(res);
	
	
	num = ftmp1;
	if(datLen) {                   /*0x00����*/
		//tmpVar = VarGet(*(CDV_INT32U*)p_dat);
		tmpVar = ArithmeticEx((char*)p_dat, datLen, arg);
		
	} else {
//		num = (len - 11)>>1;//ftmp1;//datLen>>1;
//		p_val = p_dat;
		tmpVar = 0;
	}
	
	tmp[0] = tmpVar ;
	tmp[1] = tmpVar >> 8;//�޸�2017-10-25
	tmp[2] = tmpVar >> 16;
	tmp[3] = tmpVar >> 24;
	p_val = tmp;
	
	
	switch(fc) {
		case 0x10:                        /*write multi reg*/
			WriteMultiRegisterCmd(devAddr, regPos, num, p_val, &cmdBuf, &cmdLen);
			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
			break;
		case 0x06:                        /*write reg*/
			WriteRegisterCmd(devAddr, regPos, *(CDV_INT16U*)p_val, &cmdBuf, &cmdLen);
			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
			break;
		case 0x03:                        /*read reg*/
			ReadRegisterCmd(devAddr, regPos, num, &cmdBuf, &cmdLen);
			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
			//���������ҽ��ճɹ�
			if(!datLen && OPT_SUCCESS == ret && devAddr == recv_buf[0] && 0x03 == recv_buf[1]) {
				ret = ReadRegReqToVar(recv_buf, recv_len, 0, ftmp2);
				isRead = 1;
			}
			break;
		case 0x04:                        /*read inreg*/
			ReadInRegisterCmd(devAddr, regPos, num, &cmdBuf, &cmdLen);
			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
			//���������ҽ��ճɹ�
			if( OPT_SUCCESS == ret && devAddr == recv_buf[0] && 0x04 == recv_buf[1]) {
				ret = ReadInRegReqToVar(recv_buf, recv_len, 0, ftmp2);
				isRead = 1;
			}
			break;
		case 0x05:                        /*write coil*/
			WriteCoilCmd(devAddr, regPos, *p_val, &cmdBuf, &cmdLen);
			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
		
//		if(cmdBuf[2] == 0x02 && cmdBuf[3] == 0x0F)
//			break;
			break;
		case 0x02:                        /*read incoil*/
			ReadInCoilCmd(devAddr, regPos, num, &cmdBuf, &cmdLen);
			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
			//���������ҽ��ճɹ�
			if( OPT_SUCCESS == ret && devAddr == recv_buf[0] && 0x02 == recv_buf[1]) {
				ret = ReadInCoilReqToVar(recv_buf, recv_len, 0, ftmp2);
				isRead = 1;
			}
			break;
		case 0x01:                        /*read coil*/
			ReadCoilCmd(devAddr, regPos, num, &cmdBuf, &cmdLen);
			ret = UniSerialSendCRC(cmdBuf, cmdLen, recv_buf, 100, &recv_len, uartNo);
			//���������ҽ��ճɹ�
			if( OPT_SUCCESS == ret && devAddr == recv_buf[0] && 0x01 == recv_buf[1]) {
				ret = ReadCoilReqToVar(recv_buf, recv_len, 0, ftmp2);
				isRead = 1;
			}
			break;
		default:
			break;
	}
	//��ȡ������Ҫ�ظ����������ֵ
	if(!arg->ptrWorker) {
		if(isRead)
		{
			tmpVar = VarGet(ftmp2);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)&tmpVar, 4, arg, RC_CRC);
		}
		else
		{
			ResRequest(arg->buf, arg->len, 0, 0, arg, RC_CRC);
		}
	}
	
	DELETE(cmdBuf);
	
	if(OPT_SUCCESS == ret)
	{
//		RESET_INCOIL_ADDR(99+devAddr);
		UniversalCnt[devAddr] = 0;
	}
	else
	{
		UniversalCnt[devAddr]++;
	}
	
	if(0x05 == fc || 0x10 == fc || 0x06 == fc)
		return ret;
	else
		return OPT_SUCCESS;//ret;//171014����ʧ�ܲ����ȣ���ֹ��ͨ��ʱ�����̲�ִ��
}