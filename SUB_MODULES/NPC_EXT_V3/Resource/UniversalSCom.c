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
/**
  * @brief  CSB ���� ת��
  *  
  * @param  ID     Ҫ������CSB�豸��
  *   
  * @retval void
  *
  * @note   rxBuf ������ָ��
  */
RET_STATUS UniSerialSendCRC(u8* txBuf, const u8 txLen,u8** rxBuf,u16* rxLen , const u8 uart,BUF_OPT opt)
{
	static int stat = 0;
//	OS_ERR err;
	s32 val = 0;
	u16 data;
	u8* sendBuf = NULL;
	u8 i = 0;
	
	if((NULL == txBuf) || (0 == txLen) || (NULL == rxBuf) || (NULL == rxLen))
		return OPT_FAILURE;
	
	if(BUF_NEW == opt) {
	  NEW08U(sendBuf, txLen + 2);
	  MemCpy(sendBuf, txBuf, txLen);
	} else {
		sendBuf = txBuf;
	}
	
	
	data=getCRC16(sendBuf,txLen);
	MemCpy(sendBuf + txLen, &data, 2);
//	sendBuf[txLen]=data & 0x00ff;
//  sendBuf[txLen + 1]=(data >> 8) & 0x00ff;
	while(1 != USARTTR(sendBuf ,txLen + 2 ,rxBuf , rxLen , uart));
	
	if(BUF_NEW == opt) {
	  DELETE(sendBuf);
	}
	
	if(*rxLen > 2) {
		data=getCRC16(*rxBuf,*rxLen-2);
		
		if(data == *(u16*)(*rxBuf+*rxLen-2))
//			
//		if((rxBuf[*rxLen-2]==(data & 0x00ff))
//			&& (rxBuf[*rxLen-1]==((data >> 8) & 0x00ff))) //crc
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

/*
while(1) {
	if(0 != UniSerialSendCRC()) {
		 // do something
	}
}
*/
int UniSerialSendCRC2(u8* txBuf, const u8 txLen,u8** rxBuf,u16* rxLen , const u8 uart,BUF_OPT opt)
{
	static int stat = 0;
//	OS_ERR err;
	//s32 val = 0;
	u16 data;
	static u8* sendBuf = NULL;
	//u8 i = 0;
	static int ret;
	
	switch (stat) {
		case 0:
	
			if((NULL == txBuf) || (0 == txLen) || (NULL == rxBuf) || (NULL == rxLen))
				return -1;
			
			if(BUF_NEW == opt) {
				NEW08U(sendBuf, txLen + 2);
				MemCpy(sendBuf, txBuf, txLen);
			} else {
				sendBuf = txBuf;
			}
			
			
			data=getCRC16(sendBuf,txLen);
			MemCpy(sendBuf + txLen, &data, 2);
		//	sendBuf[txLen]=data & 0x00ff;
		//  sendBuf[txLen + 1]=(data >> 8) & 0x00ff;
			stat = 1;
			break;
		case 1:
			ret = USARTTR(sendBuf ,txLen + 2 ,rxBuf , rxLen , uart);
			if(1 == ret || 0==ret)
				stat = 2;
			
			break;
		case 2:
			stat = 0;
		
			if(BUF_NEW == opt) {
				DELETE(sendBuf);
			}
			
			if(*rxLen > 2) {
				data=getCRC16(*rxBuf,*rxLen-2);
				
				if(data == *(u16*)(*rxBuf+*rxLen-2))
		//			
		//		if((rxBuf[*rxLen-2]==(data & 0x00ff))
		//			&& (rxBuf[*rxLen-1]==((data >> 8) & 0x00ff))) //crc
				{
					return 1;
				}
				else
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
			break;
		default:
			stat = 0;
			break;
	}
	
	return 0;
}
///**
//  * @brief  CSB ���� ת��
//  *  
//  * @param  ID     Ҫ������CSB�豸��
//  *   
//  * @retval void
//  *
//  * @note   rxBuf ������ָ��
//  */
//RET_STATUS UniSerialSendCRC(u8* txBuf, const u8 txLen,u8* rxBuf,u16* rxLen , const u8 uart,BUF_OPT opt)
//{
////	OS_ERR err;
//	s32 val = 0;
//	u16 data;
//	u8* sendBuf = NULL;
//	u8 i = 0;
//	
//	if((NULL == txBuf) || (0 == txLen) || (NULL == rxBuf) || (NULL == rxLen))
//		return OPT_FAILURE;
//	
//	if(BUF_NEW == opt) {
//	  NEW08U(sendBuf, txLen + 2);
//	  MemCpy(sendBuf, txBuf, txLen);
//	} else {
//		sendBuf = txBuf;
//	}
//	
//	
//	data=getCRC16(sendBuf,txLen);
//	MemCpy(sendBuf + txLen, &data, 2);
////	sendBuf[txLen]=data & 0x00ff;
////  sendBuf[txLen + 1]=(data >> 8) & 0x00ff;
//	USARTTR(sendBuf ,txLen + 2 ,rxBuf , rxLen , uart);
//	
//	if(BUF_NEW == opt) {
//	  DELETE(sendBuf);
//	}
//	
//	if(*rxLen > 2) {
//		data=getCRC16(rxBuf,*rxLen-2);
//		
//		if(data == *(u16*)(rxBuf+*rxLen-2))
////			
////		if((rxBuf[*rxLen-2]==(data & 0x00ff))
////			&& (rxBuf[*rxLen-1]==((data >> 8) & 0x00ff))) //crc
//		{
//			return OPT_SUCCESS;
//		}
//		else
//		{
//			return OPT_FAILURE;
//		}
//	}
//	else
//	{
//		return OPT_FAILURE;
//	}
//}

/** @brief  ��Դ����(�°�)��
  * @param  rxBuf     ԭʼ�ַ���
  *         rxLen     ������
	*         para      ���Ӳ����ַ���
	*         paraLen   ������
            ctrl      0�� 1 crc
  * @retval ��
  * @note   ���շ��͵��ַ�������
	*                 ������rxLen byte������   paraLen   ��   2byte  ��
	*                                                    ��          ��
	*               rxBuf                para            ��    CRC   ��                                    
	*                 ��                  ��             ��          ��                                                                                                                                             
  *                                                                               
  */
void ResRequest(u8* rxBuf, u16 rxLen , u8* para, u16 paraLen, CMD_ARG *arg, REQUEST_CTRL ctrl)	{
	
	u16 txLen = 0;
	u16 crc = 0xFFFF;
	u8 *txBuf = NULL;
	
	if(NULL != rxBuf)
	{
		txLen += rxLen;
	}
	if(NULL != para)
	{
		txLen += paraLen;
	}
	if(ctrl) // crc �ݶ�
	{
		txLen += 2;
	}
	
	NEW08U(txBuf,txLen);
	
	if(rxLen > 0)
	  MemCpy(txBuf , rxBuf , rxLen);
	
	
	if(paraLen > 0)
		MemCpy(txBuf + rxLen , para , paraLen);
	
	if(ctrl) {
	  crc = MODBUS_CRC16(txBuf, rxLen + paraLen, crc);
		MemCpy(txBuf + rxLen + paraLen, &crc, 2);
	}
	
	arg->reqbuf = txBuf;
	arg->reqlen = txLen;
}

/** @brief  ��crc����
  * @param  
  * @retval 
  * @note   arg��
  */
void AddTxPlus(u8* txBuf, u16 txLen, CMD_ARG *arg) {
	u16 crc;
	
	u16 txRealLen;
	u8 *TX_BUF= NULL;
	u8 TX_Head[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	u8 uartNo = arg->uart;
	
	ASSERT(txBuf && txLen);
	if(NULL == txBuf || 0 == txLen || 0xFF == uartNo)
		return;

	crc = MODBUS_CRC16(txBuf,txLen, 0xFFFF);
	//crc = getCRC16(txBuf,txLen);
	
	
		TX_BUF = txBuf;
	
		USARTSend(TX_BUF, txLen, uartNo);
		USARTSend((u8*)&crc, 2, uartNo);
	
}