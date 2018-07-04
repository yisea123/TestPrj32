/**
  ******************************************************************************
  * @file    /Cascade.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-29
  * @brief   
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CASCADE_
#define  _CASCADE_

#include "cdv_include.h"

#if _NPC_VERSION_ > 1u

#if _NPC_VERSION_ == 2u
#define CASCADE_USART 3
#elif _NPC_VERSION_ == 3u
#define CASCADE_USART 6
#endif

#define DA_NUM  Dac_no
#define AD_NUM  Adc_no
#define O_NUM  CDV_O_NUM + CDV_EXO_NUM*CDV_FPGA_NUM
#define I_NUM  CDV_I_NUM + CDV_EXI_NUM*CDV_FPGA_NUM
	
	extern CDV_INT08U version[];
	
//CDV_INT08U Cascade_HaveSlaveTable(void);
//void CascadeCombine(CDV_INT08U no);
//void CascadeLocal(CDV_INT08U no);
RET_STATUS CascadeSlaveParse (char* pBuf, CDV_INT08U len);
void CascadeTableSend(CDV_INT08U* buf, CDV_INT08U len,  CMD_ARG *arg);
CDV_INT08U CascadeGetNativeNo(void);
//RET_STATUS CascadeTranferCmd(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT08U* pCmdBuf, CDV_INT16U NumByteOfCmd);
//RET_STATUS TFCmdParse (CDV_INT08U* pBuf, CDV_INT08U len);
//RET_STATUS Cascade_Slave_Write(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite);
	RET_STATUS CascadeModbus_MapInit(void);
	RET_STATUS CascadeModbus_Map(void);
	
	RET_STATUS CoilToCoil(CDV_INT08U* buf, CDV_INT08U bufaddr, CDV_INT08U* coil, CDV_INT16U coiladdr, CDV_INT16U tonum);
	int CoilCmp(CDV_INT08U* buf, CDV_INT08U bufaddr, CDV_INT08U* coil, CDV_INT16U coiladdr, CDV_INT16U tonum);
//
CDV_INT08U PortCmdCache(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo) ;
RET_STATUS TFUsartCmdParse (CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);
RET_STATUS NPC_NETParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);	
RET_STATUS NPC_NETVeriSet(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);
RET_STATUS NPC_NETQuery(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);

void GetTable(void);
#endif
#endif

