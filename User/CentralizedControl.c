/**
  ******************************************************************************
  * @file    /CentralizedControl.c 
  * @author  MMY
  * @version V0.0.1
  * @date    05-17-2018
  * @brief   
	* 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "CentralizedControl.h"

#if USE_CENTRALIZEDCONTROL == 1u

#define MAX_NEW_BUF_SIZE 0x1000

	
/**
  * @brief  ��ϵͳ��Ϣ
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */

RET_STATUS CentralizedControl_QuerySysInfo(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	CDV_INT16U crc = 0xFFFF;
	CDV_INT16U bytes = 4/*�̼��汾*/
	                  +2/*Ӳ���汾*/
	                  +0/*Ӧ�ò�汾*/
	                  +6/*Mac��ַ*/
	                  +4/*Ψһʶ����*/
	                  +0/*���̽ű��汾*/
	                  +6/*����*/;
	CDV_INT08U* sndBuf = NULL;
	CDV_INT32U offset = 0;
  CDV_INT32U fieldslen = 0;
	
	CDV_INT08U* YYCVersionBuf = NULL;
	CDV_INT32U YYCVersionLen = 0;
	RET_STATUS ret1 = GetYYCVersion(&YYCVersionBuf, &YYCVersionLen);
	
	CDV_INT08U* lineVersionBuf = NULL;
	CDV_INT32U lineVersionLen = 0;
	RET_STATUS ret2 = GetLineVersion(&lineVersionBuf, &lineVersionLen);
	
	bytes += lineVersionLen + YYCVersionLen;
	
	if (MAX_NEW_BUF_SIZE < bytes + 6 || len != 2 || ret1 != OPT_SUCCESS || ret2 != OPT_SUCCESS) {
		DELETE(YYCVersionBuf);
	  DELETE(lineVersionBuf);
		return OPT_FAILURE;
	}
	
	NEWCH(sndBuf, bytes + 6);
	MemCpy(sndBuf + offset, buf, 2);
	offset += 2;
	MemCpy(sndBuf + offset, &bytes, 2);
	offset += 2;
	//�̼��汾
	fieldslen = 4;
	sndBuf[offset] = fieldslen;
	offset += 1;
	MemCpy(sndBuf + offset, version + 1, fieldslen);
	offset += fieldslen;
	//Ӳ���汾
	fieldslen = 2;
	sndBuf[offset] = fieldslen;
	offset += 1;
	MemCpy(sndBuf + offset, version + 2, fieldslen);
	offset += fieldslen;
	//Ӧ�ò�汾
	fieldslen = YYCVersionLen;
	sndBuf[offset] = fieldslen;
	offset += 1;
	MemCpy(sndBuf + offset, YYCVersionBuf, fieldslen);
	offset += fieldslen;
	//Mac
	fieldslen = 6;
	sndBuf[offset] = fieldslen;
	offset += 1;
	MemCpy(sndBuf + offset, lwipdev.mac, fieldslen);
	offset += fieldslen;
	//Ψһʶ����
	fieldslen = 4;
	sndBuf[offset] = fieldslen;
	offset += 1;
	MemCpy(sndBuf + offset, ONLY_ID_ADDR, fieldslen);
	offset += fieldslen;
	//���̽ű��汾
	fieldslen = lineVersionLen;
	sndBuf[offset] = fieldslen;
	offset += 1;
	MemCpy(sndBuf + offset, lineVersionBuf, fieldslen);
	offset += fieldslen;
	//CRC
	crc = MODBUS_CRC16(sndBuf, offset, crc);
	MemCpy(sndBuf + offset, &crc, 2);
	offset += 2;
	AddTxNoCrc(sndBuf , offset, arg->uart);
	DELETE(sndBuf);
	DELETE(YYCVersionBuf);
	DELETE(lineVersionBuf);
	return OPT_SUCCESS;
}

/**
  * @brief  ���豸����
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
//CDV_INT32U onlyID = 0x00001234;
RET_STATUS CentralizedControl_QueryDevData(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	CDV_INT16U varNum = *(CDV_INT16U*)(buf + 2) * 4;
	CDV_INT16U iNum = *(CDV_INT16U*)(buf + 4) + 7 >> 3;
	CDV_INT16U oNum = *(CDV_INT16U*)(buf + 6) + 7 >> 3;
	CDV_INT16U crc = 0xFFFF;
	CDV_INT32U bytes = varNum + iNum + oNum + 4;
	CDV_INT08U* sndBuf = NULL;
	CDV_INT32U offset = 0;
	CDV_INT08U* varAddr = (CDV_INT08U*)g_modbusReg.reg;
	CDV_INT08U* iAddr = (CDV_INT08U*)g_modbusInCoil.coilCh;
	CDV_INT08U* oAddr = (CDV_INT08U*)g_modbusCoil.coilCh;
	
	if (MAX_NEW_BUF_SIZE < bytes + 8 || len != 8 || oNum > COIL_CHN|| iNum > INCOIL_CHN || varNum > REG_N*2)
		return OPT_FAILURE;
	
	NEWCH(sndBuf, bytes + 8);
	MemCpy(sndBuf + offset, buf, 2);
	offset += 2;
	MemCpy(sndBuf + offset, &bytes, 4);
	offset += 4;
	MemCpy(sndBuf + offset, ONLY_ID_ADDR, 4);
	offset += 4;
	MemCpy(sndBuf + offset, varAddr, varNum);
	offset += varNum;
	MemCpy(sndBuf + offset, iAddr, iNum);
	offset += iNum;
	MemCpy(sndBuf + offset, oAddr, oNum);
	offset += oNum;
	crc = MODBUS_CRC16(sndBuf, offset, crc);
	MemCpy(sndBuf + offset, &crc, 2);
	offset += 2;
	//AddTxNoCrc(sndBuf , offset, arg->uart);
	ResRequest(sndBuf , offset, 0,0,arg, RC_NONE);
	DELETE(sndBuf);
	return OPT_SUCCESS;
}

/**
  * @brief  ϵͳ����
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
RET_STATUS CentralizedControl_SysOperation(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	return OPT_SUCCESS;
}

/**
  * @brief  ��������
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
void CentralizedControl_OTA(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	CDV_INT08C* fields = "OTA";
	SPI_Flash_Write((CDV_INT08U*)fields, OTA_ADDR, 3);
	ResetCdv();
}

#endif // #if USE_CENTRALIZEDCONTROL == 1u
