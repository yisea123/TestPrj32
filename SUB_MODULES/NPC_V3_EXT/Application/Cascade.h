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

#include "_include.h"

#define DA_NUM  0
#define AD_NUM  0
#define O_NUM  0
#define I_NUM  0

	extern u8 version[];
	u8 CascadeGetNativeNo(void);
//#if USE_CASCADE == 1u

#define HOST_USART 1
#define CASCADE_USART 2


	
	
//u8 Cascade_HaveSlaveTable(void);
//void CascadeCombine(u8 no);
//void CascadeLocal(u8 no);
u8 ClearPortCmdCache(void);
RET_STATUS CascadeSlaveParse (char* pBuf, u8 len);
void CascadeTableSend(u8* buf, u8 len,  CMD_ARG *arg);

//RET_STATUS CascadeTranferCmd(u8** ppBuffer, u8* pLen, u8* pCmdBuf, u16 NumByteOfCmd);
//RET_STATUS TFCmdParse (u8* pBuf, u8 len);
//RET_STATUS Cascade_Slave_Write(u8* pBuffer, u16 NumByteToWrite);
	RET_STATUS CascadeModbus_MapInit(u8* buf, u16 len);
	RET_STATUS CascadeModbus_Map(void);
	RET_STATUS CascadeModbus_MapFindInExp(
	                    const u8 type,
										  const u16 typeno,
											u16* localaddr,
										  u16* remoteaddr,
											u8* host);
	
	RET_STATUS CoilToCoil(u8* buf, u8 bufaddr, u8* coil, u16 coiladdr, u16 tonum);
	int CoilCmp(u8* buf, u16 bufaddr, u8* coil, u16 coiladdr, u16 tonum);
//
u8 PortCmdCache(u8* rxBuf, u8 rxLen, u8 uartNo) ;
RET_STATUS TFUsartCmdParse (u8* buf, u16 len, CMD_ARG *arg);
RET_STATUS NPC_NETParse(u8* buf, u8 len, CMD_ARG *arg);	
RET_STATUS NPC_NETVeriSet(u8* buf, u8 len, CMD_ARG *arg);
RET_STATUS NPC_NETQuery(u8* buf, u8 len, CMD_ARG *arg);

void GetTable(void);
RET_STATUS CascadeSlaveParse2 (char* pBuf, u8 len);
											
int CascadeMap_Init (u8* buf, u16 len, CMD_ARG *arg);
int CascadeModbus_Map_Stat(void);
int Cascade_Host_Stat(void);
int Cascade_Host_Transfer(u8* buf, u16 len, u8** rtbuf,u16* rtlen);
											
#endif
