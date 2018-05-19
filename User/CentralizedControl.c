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
  * @brief  查系统信息
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   
  */

RET_STATUS CentralizedControl_QuerySysInfo(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	return OPT_FAILURE;
}

/**
  * @brief  查设备数据
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   
  */
CDV_INT32U onlyID = 0x00001234;
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
	
	if (MAX_NEW_BUF_SIZE < bytes + 8 || len != 8)
		return OPT_FAILURE;
	
	NEWCH(sndBuf, bytes + 8);
	MemCpy(sndBuf + offset, buf, 2);
	offset += 2;
	MemCpy(sndBuf + offset, &bytes, 4);
	offset += 4;
	MemCpy(sndBuf + offset, &onlyID, 4);
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
	AddTxNoCrc(sndBuf , offset, arg->uart);
	DELETE(sndBuf);
	return OPT_SUCCESS;
}

#endif // #if USE_CENTRALIZEDCONTROL == 1u
