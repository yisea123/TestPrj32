/**
  ******************************************************************************
  * @file    /Cascade.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-29
  * @brief   级联
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
	
	
	#include "Cascade.h"
	
  //级联资源表——版本号
	
#if _NPC_VERSION_ == 1u
	CDV_INT08U version[] = {0/*id号*/, 2/*软件大版本*/, 1/*硬件大版本*/, 1/*硬件驱动版本*/, 19/*迭代小版本*/};
#elif _NPC_VERSION_ == 2u
	CDV_INT08U version[] = {0/*id号*/, 3/*软件大版本*/, 2/*硬件大版本*/, 2/*硬件驱动版本*/, 19/*迭代小版本*/};
#elif _NPC_VERSION_ == 3u
	CDV_INT08U version[] = {0/*id号*/, 3/*软件大版本*/, 2/*硬件大版本*/, 3/*硬件驱动版本*/, 13/*迭代小版本*/};
#endif
	
#if USE_CASCADE_STATIC == 1u
	#define CASCADE_BUF_MAX_LEN 256
	CDV_INT08U* cascade_cmd_buf = NULL; // 用于命令的静态buf，不动态分配
	CDV_INT08U* cascade_tmp_buf = NULL;
#endif
	
  //开启主级联口
	#define	CASCADE_EN 			CASCADE_CS_EN;FPGA1_CS_DS;FPGA2_CS_DS;FLASH_CS_DS;
	//关闭主级联口
	#define	CASCADE_DS 			CASCADE_CS_DS;
	//主级联口发数据
	#define CASCADE_ReadWriteByte(a)				SPIx_ReadWriteByte(SPI2,(a))

	//从机资源表
	CDV_INT08U *slaveTable = NULL;
	CDV_INT08U *slaveTable2 = NULL;
	CDV_INT08U slaveTableLen = 0;
	#define MAX_SLAVE_TABLE 50
	
  OLCMD_CACHE g_portCmdCache = {NULL , 0  , 0 , 0 , NULL};
	
	#define CASCADE_BUF_LEN 255
	//u8 cascade_recv_buf[CASCADE_BUF_LEN];//UserMemPtr(CCM_CASCADE_MODBUS);
	u8 *cascade_recv_buf = NULL;
	
	#if USE_OVERLAP
	MODBUS_Coil *g_coilCascade = NULL;
	MODBUS_Register *g_regCascade = NULL;
	#endif
	
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	/////////////Cascade Master Function///////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
	RET_STATUS NPC_NETParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	RET_STATUS ret = OPT_FAILURE;
	switch(buf[0]){//NPC地址应答
		case 0x01:
//		DebugParse(buf, len, arg->uart);
		  ret = NPC_NETQuery(buf+1, len-1, arg);
			break;
		case 0x10:
			ret = NPC_NETVeriSet(buf+1, len-1, arg);
			break;
		default:
			break;
	}
	
	return ret;
}
	
void GetTable(void)
{
	SPI_Flash_Read((CDV_INT08U *)&slaveTableLen, CHECK_VERION_ADDR1, 0X01);
	if(slaveTableLen > MAX_SLAVE_TABLE) {
		slaveTableLen = 0;
		return;
	}
	NEW08U(slaveTable2,slaveTableLen);	
	SPI_Flash_Read((CDV_INT08U *)slaveTable2, CHECK_VERION_ADDR, slaveTableLen);
}
//RET_STATUS NPC_NETVeriSet(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
//{
//	int i;
//	RET_STATUS ret = OPT_SUCCESS;
////	CDV_INT08U *Rx_len = NULL;
//	CDV_INT08U buf1[] = {0};
//	CDV_INT08U buf2[] = {0};
////	NEW08U(Rx_len,0x01);
//	
////	Rx_len =&len;
//	//CDV_INT08U *slaveTable = version;
//	//CDV_INT08U slaveTableLen = 5;
//	
//	
//	buf1[0] = len;
//	NEW08U(slaveTable,len);
//	for(i=0;i<len;i++)
//	{
//	  slaveTable[i] = buf[i];
//	}
// SPI_Flash_Write((CDV_INT08U *)buf1, CHECK_VERION_ADDR1,  0X01);
// delay_ms(2);
// SPI_Flash_Read((CDV_INT08U *)buf2, CHECK_VERION_ADDR1,  0X01);
// SPI_Flash_Write((CDV_INT08U *)slaveTable, CHECK_VERION_ADDR, len);
//delay_ms(2);	
//	ResRequest(arg->buf, arg->len, 0,0,arg);
////	DELETE(slaveTable);
//	return ret;
//}

RET_STATUS NPC_NETVeriSet(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	int i;
	RET_STATUS ret = OPT_SUCCESS;
	CDV_INT08U Rx_len;
	Rx_len = len;
	NEW08U(slaveTable,len);
	for(i=0;i<len;i++)
	{
	  slaveTable[i] = buf[i];
	}
 SPI_Flash_Write((CDV_INT08U *)&Rx_len, CHECK_VERION_ADDR1,  0X01);
 delay_ms(2);
 SPI_Flash_Write((CDV_INT08U *)slaveTable, CHECK_VERION_ADDR, len);
 delay_ms(2);	
 ResRequest(arg->buf, arg->len, 0,0,arg, RC_CRC);
 DELETE(slaveTable);
	GetTable();
	return ret;
}
//RET_STATUS NPC_NETQuery(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
//{
//	RET_STATUS ret = OPT_SUCCESS;
//	CDV_INT08U Rx_len ;
//	CDV_INT08U *slaveTable2 = NULL;
//	SPI_Flash_Read((CDV_INT08U *)&Rx_len, CHECK_VERION_ADDR1, 0X01);
//	delay_ms(1);
//	NEW08U(slaveTable2,Rx_len);
//	
//	SPI_Flash_Read((CDV_INT08U *)slaveTable2, CHECK_VERION_ADDR, Rx_len);
//	delay_ms(2);
////	for(i=0;i<5;i++)
////	{
////	version[i] = buf[i]; 
////	}
// ResRequest(arg->buf, arg->len-5, slaveTable2, Rx_len, arg);
// DELETE(slaveTable2);
// return ret;
//}
RET_STATUS NPC_NETQuery(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	RET_STATUS ret = OPT_SUCCESS;
	CDV_INT08U Rx_len ;
	SPI_Flash_Read((CDV_INT08U *)&Rx_len, CHECK_VERION_ADDR1, 0X01);
	if(Rx_len > MAX_SLAVE_TABLE)
		return OPT_FAILURE;
	NEW08U(slaveTable2,Rx_len);	
	SPI_Flash_Read((CDV_INT08U *)slaveTable2, CHECK_VERION_ADDR, Rx_len);
  ResRequest(arg->buf, arg->len-5, slaveTable2, Rx_len, arg, RC_CRC);
  // DELETE(slaveTable2);
  return ret;
}
	
/** @brief  查询有没有从机
  * @param  
  * @retval 存在1，不存在0
  * @note   
  */
CDV_INT08U Cascade_HaveSlaveTable(void)   
{ 
  if (slaveTableLen > 5)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}  

/** @brief  查询从机回复（等待）
  * @param  ppBuffer 查询成功保存从机回复的字符串
	*         pLen    查询成功保存从机回复的字符串长度
	*         cnt     查询失败的循环次数
  * @retval 返回值说明
  * @note   使用上应该在Cascade_Master_Write之后查询
	          请勿随意调用
  */
void Cascade_Master_Query(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT16U cnt)   
{ 
 	CDV_INT16U i = 0;    		
	CDV_INT08U len;
  OS_ERR err;	
	
	ASSERT(ppBuffer);
	ASSERT(pLen);
	

	CASCADE_EN;                            //使能器件  
	//DelayUS(1000);
	do{
		DelayUS(100);
		*pLen = CASCADE_ReadWriteByte(0xFF);   //循环读数  
		cnt--;
//		i++;
//		
//		if(i > 10 && 0x00 == *pLen)
//			break;
	} while((/*0xFF == *pLen ||*/ 0x00 == *pLen) && cnt);
	
	if(0x80 > *pLen && 0x00 != *pLen) {
		NEW08U(*ppBuffer, *pLen);
		
		for(i = 0; i < *pLen; i++)
		{
				(*ppBuffer)[i] = CASCADE_ReadWriteByte(0xFF);   //循环读数  
		}
	}
	else
	{
		*pLen = 0x00;
	}
	CASCADE_DS;                            //取消片选   

}  

/** @brief  查询从机回复（不等待）
  * @param  pBuffer          查询到的字符串
	*         NumByteToRead    查询的长度
  * @retval 返回值说明
  * @note   请勿随意调用
  */
void Cascade_Master_Read(CDV_INT08U* pBuffer, CDV_INT16U NumByteToRead)   
{ 
 	CDV_INT16U i;    		
  OS_ERR err;	
	
	ASSERT(pBuffer);
	

	CASCADE_EN;                            //使能器件  
	
	for(i = 0; i < NumByteToRead; i++)
	{
			pBuffer[i] = CASCADE_ReadWriteByte(0XFF);   //循环读数  
	}
	CASCADE_DS;                            //取消片选   

}  
/** @brief  主机发送命令
  * @param  pBuffer          发送的字符串
	*         NumByteToWrite   发送的字符串长度
  * @retval 返回值说明
  * @note   请勿随意调用
  */
void Cascade_Master_Write(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{
	//CDV_INT08U* temp = NULL;
 	CDV_INT16U i;    		
  OS_ERR err;	
	//NEW08U(temp, NumByteToWrite);
	
	ASSERT(pBuffer);
	

	CASCADE_EN;                            //使能器件  
	//DelayUS(1000);
	for(i = 0 ; i < NumByteToWrite; i++)
	{
			/*temp[i] = */CASCADE_ReadWriteByte(pBuffer[i]);   //循环读数  
	}
	CASCADE_DS;                            //取消片选   

	//DELETE(temp);
}

/** @brief  主机发送接收接口，用于加入信号量
  * @param  pTxBuf          发送的字符串 NULL 不发送
	*         txByte          发送的字符串长度
  *         ppRxBuf         查询成功保存从机回复的字符串
	*         pRxLen          查询成功保存从机回复的字符串长度
	*         rxCnt           查询失败的循环次数
  * @retval 返回值说明
  * @note   外部主级联封装发送命令必须使用该接口，或者与级联总线的其他设备资源易产生冲突
  */
void CascadeMasterTR2(CDV_INT08U* pTxBuf, CDV_INT16U txByte, CDV_INT08U** ppRxBuf, CDV_INT08U* pRxLen, CDV_INT16U rxCnt)   
{
  OS_ERR err;	
	u8 i,dat;
	OSMutexPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	SPEED_CASCADE;
	
	if(pTxBuf && txByte) {
	  Cascade_Master_Write(pTxBuf, txByte);
	}
	DelayUS(10000);//给从机的中断一点反应的时间
	if(ppRxBuf && pRxLen) {
	  Cascade_Master_Query(ppRxBuf, pRxLen, rxCnt);
	}
	
	CASCADE_EN;
	for(i = 0 ; i < 3; i++)
	  dat = CASCADE_ReadWriteByte(0xFF);
	
	CASCADE_DS;
	DelayUS(10000);//给从机的中断一点反应的时间
//	CASCADE_CS_EN;
//	
//	CASCADE_CS_DS;
	SPEED_FPGA;
	OSMutexPost (&MOTO_SEM,OS_OPT_POST_NO_SCHED,&err);
}

/** @brief  主机发送接收接口，用于加入信号量 ，CRC校验版本
  * @param  pTxBuf          发送的字符串 NULL 不发送
	*         txByte          发送的字符串长度
  *         ppRxBuf         查询成功保存从机回复的字符串
	*         pRxLen          查询成功保存从机回复的字符串长度
	*         rxCnt           查询失败的循环次数
  * @retval 返回值说明
  * @note   外部主级联封装发送命令必须使用该接口，或者与级联总线的其他设备资源易产生冲突
  */
void CascadeMasterTR(CDV_INT08U* pTxBuf, CDV_INT16U txByte, CDV_INT08U** ppRxBuf, CDV_INT08U* pRxLen, CDV_INT16U rxCnt)   
{
  u8 i,dat;
	CDV_INT08U slen = 0;
	CDV_INT16U crc;
  CDV_INT08U* sbuf = NULL;
	OS_ERR err;	
	
	OSMutexPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
  SPEED_CASCADE;
	slen = txByte + 2 ;
	NEW08U(sbuf, slen);//最后是crc
	MemCpy(sbuf, pTxBuf, txByte);
	//crc
	crc = getCRC16(pTxBuf, txByte);
	sbuf[txByte] = crc & 0xff; 
	sbuf[txByte + 1] = (crc >> 8) & 0xff;
	
	
	if(sbuf && slen)
	{
	  Cascade_Master_Write(sbuf, slen);
	}
	DELETE(sbuf);
	
	do
	{
		DelayUS(10000);//给从机的中断一点反应的时间
		if(ppRxBuf && pRxLen) {
			Cascade_Master_Query(ppRxBuf, pRxLen, rxCnt);
		}
		
		//下面这个是定制的，多读几次，保证spi从机出来的是0x00
		CASCADE_EN;
		for(i = 0 ; i < 3; i++)
			dat = CASCADE_ReadWriteByte(0xFF);
		
		CASCADE_DS;
		DelayUS(10000);//给从机的中断一点反应的时间

		//crc chk
		sbuf = *ppRxBuf;
		slen = *pRxLen;
		
		if(!sbuf || 2 > slen)
			break;
		
		crc = getCRC16(sbuf, slen-2); 
		
		if((sbuf[slen-1] == ((crc >> 8) & 0xff)) && (sbuf[slen-2] == (crc & 0xff)))
		{
			*pRxLen -= 2;
			break;
		}
		else
		{
			sbuf = NULL;
			slen = 8;
			NEW08U(sbuf, slen);//最后是crc
		  MemCpy(sbuf, "GETRQ:", 6);
			//crc
			crc = getCRC16(sbuf, 6);
			sbuf[6] = crc & 0xff; 
			sbuf[7] = (crc >> 8) & 0xff;
			
			if(sbuf && slen)
			{
				Cascade_Master_Write(sbuf, slen);
			}
			DELETE(sbuf);
		}
	}while(1);
	SPEED_FPGA;
	OSMutexPost (&MOTO_SEM,OS_OPT_POST_NO_SCHED,&err);
}

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	/////////////Cascade Slave Function////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////

/** @brief  从机发送命令
  * @param  pBuffer          发送的字符串
	*         NumByteToWrite   发送的字符串长度
  * @retval RET_STATUS
  * @note   非线程安全，如果多个地方用，加锁
  */
RET_STATUS Cascade_Slave_Write2(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{ 
  if(!pBuffer || !NumByteToWrite)
		return OPT_FAILURE;
	
	
	
	return SPI3_Send(pBuffer, NumByteToWrite);
}  

/** @brief  从机发送命令
  * @param  pBuffer          发送的字符串
	*         NumByteToWrite   发送的字符串长度
  * @retval RET_STATUS
  * @note   非线程安全，如果多个地方用，加锁
  */
RET_STATUS Cascade_Slave_Write(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{ 
  CDV_INT08U* tmp = NULL;
	CDV_INT16U crc;
	RET_STATUS ret;
	
	if(!pBuffer || !NumByteToWrite)
		return OPT_FAILURE;
	
	ASSERT(pBuffer);
	ASSERT(NumByteToWrite);
	
	NEW08U(tmp, NumByteToWrite + 2);//最后是crc
	MemCpy(tmp, pBuffer, NumByteToWrite);
	//crc
	crc = getCRC16(pBuffer, NumByteToWrite);
	tmp[NumByteToWrite] = crc & 0xff; 
	tmp[NumByteToWrite + 1] = (crc >> 8) & 0xff;
	ret = SPI3_Send(tmp, NumByteToWrite + 2);
  DELETE(tmp);
	return ret;
}  
/** @brief  非本机命令的转发
  * @param  ppBuffer      查询到的资源表
	*         pLen          资源表长度
  *         pCmdBuf       待发命令
  *         NumByteOfCmd  待发命令长度
  * @retval RET_STATUS
  * @note   ppBuffer需使用指针
  */
	RET_STATUS CascadeTranferCmd(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT08U* pCmdBuf, CDV_INT16U NumByteOfCmd) {
		CDV_INT08U* tmp = NULL;
		CDV_INT16U crc;
	
		if(slaveTableLen < 6)
			return OPT_FAILURE;
		
		
		ASSERT(NULL != pCmdBuf);
		ASSERT(0 != NumByteOfCmd);
		ASSERT(NULL != pLen);
//		
//		NEW08U(tmp, NumByteOfCmd + 6 + 2);//最后是crc
//		memcpy(tmp, "TFCMD:", 6);
//		memcpy(tmp + 6, pCmdBuf, NumByteOfCmd);
//		//crc
//		crc = getCRC16(pCmdBuf, NumByteOfCmd);
//		tmp[NumByteOfCmd + 6] = crc & 0xff; 
//	  tmp[NumByteOfCmd + 6 + 1] = (crc >> 8) & 0xff;
		
		
		NEW08U(tmp, NumByteOfCmd + 6);//最后是crc
		MemCpy(tmp, "TFCMD:", 6);
		MemCpy(tmp + 6, pCmdBuf, NumByteOfCmd);
		
		CascadeMasterTR((CDV_INT08U*)tmp, NumByteOfCmd + 6/* + 2*/, ppBuffer, pLen, 0xFFFF);
		
		DELETE(tmp);
		
		if(!*pLen || !*ppBuffer)
			return OPT_FAILURE;
		else
		  return OPT_SUCCESS;
	}
	
/** @brief  CascadeTranferCmd之后的解析
  * @param  pBuf   命令
	*         len    命令长度
  * @retval RET_STATUS
  * @note   
  */
  RET_STATUS TFCmdParse (CDV_INT08U* pBuf, CDV_INT08U len) {
		char *pos = NULL;
		RET_STATUS ret = OPT_FAILURE;
		
		if(!pBuf || !len)
			return ret;
		
		if(pos = strstr((char *)pBuf,"VARSET:"))
		{
			ValSet((CDV_INT08U)pos[7], *(CDV_INT32S*)(pos + 8));
			ret = OPT_SUCCESS;
		}
		else if(pos = strstr((char *)pBuf,"ERROR"))
		{
      ret = OPT_FAILURE;
		}
		else if(len <= 6)
		{
			ret = OPT_FAILURE;
		}
		else if(pBuf[1] > 0x80)
		{
			ret = OPT_FAILURE;
		}
		else
		{
			ret = OPT_SUCCESS;
		}
		return ret;
	}
	
	
	
/** @brief  判断有没有从机
  * @param  void
  * @retval void
  * @note   ppBuffer需使用指针
  */
	RET_STATUS SlaveCheck(void) {
		CDV_INT08U *buf = NULL;
	  CDV_INT08U len = 0;
		RET_STATUS ret = OPT_FAILURE;

		CascadeMasterTR((CDV_INT08U*)"EXIST:", 6, &buf, &len, 0x10);
		
		if(5 <= len && (0 == strncmp((CDV_INT08C*)buf, "EXIST", 5)))
			ret = OPT_SUCCESS;

		DELETE(buf);
    return ret;
	}
/** @brief  主机命令,从机资源表查询
  * @param  ppBuffer   查询到的资源表
	*         pLen       资源表长度
  * @retval RET_STATUS
  * @note   ppBuffer需使用指针
  */
	void CascadeCheck(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT08U num) {
		char tmp[10]={0};
		ASSERT(NULL != pLen);
		
		sprintf(tmp, "CHECK:%02d", num);

		CascadeMasterTR((CDV_INT08U*)tmp, 8, ppBuffer, pLen, 0xffff);
		
		if(*pLen && *ppBuffer)
		{
			if(*ppBuffer[0] != num + 1)
			{
				*pLen = 0;
			}
			
		}
	}

/** @brief  主从资源表合并,递归所有从机
  * @param  ppBuffer   查询到的资源表
	*         pLen       资源表长度
  * @retval RET_STATUS
  * @note   
  */
  void CascadeCombine(CDV_INT08U no) {
		
		CDV_INT08U *buf = NULL;
	  CDV_INT08U len = 0;

		if(OPT_SUCCESS == SlaveCheck())
		{
		  CascadeCheck(&buf, &len, no);//本机测试时注释
		}
			
		NEW08U(slaveTable, 5 + len);
		slaveTableLen = 5 + len;
		
		slaveTable[0] = no;
		MemCpy(slaveTable + 1, version, 4);
		if(NULL != buf && 0 != len)
			MemCpy(slaveTable + 5, buf, len);
		
		DELETE(buf);
	}
	
/** @brief  资源表,只有本机
  * @param  
  * @retval 
  * @note   
  */
  void CascadeLocal(CDV_INT08U no) {
		NEW08U(slaveTable, 5);
		slaveTableLen = 5;
		slaveTable[0] = no;
		MemCpy(slaveTable + 1, version, 4);
	}
	
/** @brief  资源表发送给上位机
  * @param  
  * @retval 
  * @note   
  */
	void CascadeTableSend(CDV_INT08U* buf, CDV_INT08U len,  CMD_ARG *arg) {
		ResRequest(buf, len, slaveTable, slaveTableLen, arg, RC_CRC);
	}
/** @brief  获取本机号
  * @param  
  * @retval 本机号。0xFF没有本机号
  * @note   
  */
	CDV_INT08U CascadeGetNativeNo(void) {
		if(NULL == slaveTable2 || 0 == slaveTableLen)
			return 0xFF;
		
		return slaveTable2[0];
	}
	
	
	/**
  * @brief  从机命令放入cache
  *         
  *  
  * @param  rxBuf     接收命令
  *         rxLen     接收长度
  *           
  *   
  * @retval 返回值说明0失败1成功
  *
  * @note   
  */
CDV_INT08U PortCmdCache(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo) {
	OS_ERR err;
	if(rxBuf == NULL || rxLen == 0)
		return 0;
	
	if(g_portCmdCache.mark)
		return 0;
	
	g_portCmdCache.len = rxLen;
	NEW08U(g_portCmdCache.buf,g_portCmdCache.len);
	MemCpy(g_portCmdCache.buf , rxBuf , g_portCmdCache.len);
	g_portCmdCache.uart = uartNo;
	g_portCmdCache.mark = 1;
	return 1;
}
/**
  * @brief  从机清命令cache
  *         
  *  
  * @param  
  *           
  *   
  * @retval 返回值说明
  *
  * @note   
  */
CDV_INT08U ClearPortCmdCache(void) {
	DELETE(g_portCmdCache.buf);
	g_portCmdCache.len = 0;
	g_portCmdCache.uart = 0;
	g_portCmdCache.mark = 0;		
	return 1;
}


/** @brief  从机收到命令的解析
  * @param  pBuf   命令
	*         len    命令长度
  * @retval RET_STATUS
  * @note   
  */
  RET_STATUS CascadeSlaveParse (char* pBuf, CDV_INT08U len) {
		char* pos = strchr((char*)pBuf,':') + 1;
		int par = atoi(pos);
			
		if(NULL == pos)
			return OPT_FAILURE;
		
		if(0 == strncmp((CDV_INT08C*)pBuf, "CHECK", pos - pBuf - 1))
		{
			CascadeCombine(par + 1);//序号+1为本机序号
			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		} 
		else if(0 == strncmp((CDV_INT08C*)pBuf, "TFCMD", pos - pBuf - 1))
		{
			
			if(!slaveTable)
			{
				return OPT_FAILURE;
			}
			
			if(CascadeGetNativeNo() != pos[3])//非本机，转发
			{
			  CDV_INT08U* recvBuf = NULL;
			  CDV_INT08U  recvLen = 0;
			  CascadeTranferCmd(&recvBuf, &recvLen, (CDV_INT08U*)pos, len - (pos - pBuf));
				Cascade_Slave_Write(recvBuf, recvLen);
				DELETE(recvBuf);
			}
			else
			{
//				CMD_ARG arg;
//				INIT_CLEAR(arg);
//				arg.uart = 0xFF;
//				arg.len = len - (pos - pBuf);
//				arg.buf =	(CDV_INT08U*)pos;

//				CmdParse(arg.buf, arg.len, &arg);
//				
//				/*处理资源回复请求*/
//				if(OPT_FAILURE == DoSpecRequest(&arg))
//				  DoResRequest(&arg);
//				
//				CmdArgDelete(&arg);
				CDV_INT16U crc;
				CDV_INT08U* buf = (CDV_INT08U*)pos;
				CDV_INT16U bufLen = len - (pos - pBuf);
				
				crc = getCRC16(buf, bufLen-2); 
				if((buf[bufLen-1] == ((crc >> 8) & 0xff)) && (buf[bufLen-2] == (crc & 0xff)))//crc chk
				{
					if(0 == PortCmdCache(buf, bufLen-2, 0xFF)) 
					{
						return OPT_FAILURE;
					}
				}
				else
				{
					Cascade_Slave_Write((CDV_INT08U*)"ERROR", 5);
			    return OPT_FAILURE;
				}
		
				
			}
//			CascadeCombine(par+1);//序号+1为本机序号
//			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		}
		else if(0 == strncmp((CDV_INT08C*)pBuf, "EXIST", pos - pBuf - 1))
		{
			Cascade_Slave_Write((CDV_INT08U*)"EXIST", 5);
		}
		else
		{
			Cascade_Slave_Write((CDV_INT08U*)"ERROR", 5);
			return OPT_FAILURE;
		}
		return OPT_SUCCESS;
	}
	
	
/** @brief  从机收到命令的解析
  * @param  pBuf   命令
	*         len    命令长度
  * @retval RET_STATUS
  * @note   有全局CRC判断，后续不需要CRC
  */
  RET_STATUS CascadeSlaveParse2 (char* pBuf, CDV_INT08U len) {
		char* pos = strchr((char*)pBuf,':') + 1;
		int par = atoi(pos);
		
		CDV_INT16U crc;
		
		crc = getCRC16((CDV_INT08U*)pBuf, len-2); 
		if((pBuf[len-1] == ((crc >> 8) & 0xff)) && (pBuf[len-2] == (crc & 0xff)))//crc chk
		{
		}
		else
		{
			Cascade_Slave_Write((CDV_INT08U*)"ERROR", 5);
			return OPT_FAILURE;
		}
		
		if(NULL == pos)
			return OPT_FAILURE;
		
		if(0 == strncmp((CDV_INT08C*)pBuf, "CHECK", pos - pBuf - 1))
		{
			CascadeCombine(par + 1);//序号+1为本机序号
			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		} 
		else if(0 == strncmp((CDV_INT08C*)pBuf, "TFCMD", pos - pBuf - 1))
		{
			
			if(!slaveTable)
			{
				return OPT_FAILURE;
			}
			
			if(CascadeGetNativeNo() != pos[3])//非本机，转发
			{
			  CDV_INT08U* recvBuf = NULL;
			  CDV_INT08U  recvLen = 0;
			  CascadeTranferCmd(&recvBuf, &recvLen, (CDV_INT08U*)pos, len - (pos - pBuf));
				Cascade_Slave_Write(recvBuf, recvLen);
				DELETE(recvBuf);
			}
			else
			{
				if(0 == PortCmdCache((CDV_INT08U*)pos, len - (pos - pBuf) - 2, 0xFF)) 
				{
					return OPT_FAILURE;
				}
			}
//			CascadeCombine(par+1);//序号+1为本机序号
//			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		}
		else if(0 == strncmp((CDV_INT08C*)pBuf, "EXIST", pos - pBuf - 1))
		{
			Cascade_Slave_Write((CDV_INT08U*)"EXIST", 5);
		}
		else if(0 == strncmp((CDV_INT08C*)pBuf, "GETRQ", pos - pBuf - 1))
		{
			SPI3_ReSend();
		}
		else
		{
			Cascade_Slave_Write((CDV_INT08U*)"ERROR", 5);
			return OPT_FAILURE;
		}
		return OPT_SUCCESS;
	}
	
	
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	/////////////Cascade Modbus Function///////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
	
	struct
	{
		CDV_WORD reg[REG_N];
		CDV_WORD addr[REG_N];
		CDV_CHAR id[REG_N];
		CDV_CHAR cnt[REG_N];
		CDV_CHAR cnted[REG_N];
	}REG_MAP;//用于writereg的标记
	
	struct
	{
		CDV_CHAR coil[COIL_CHN];
		CDV_CHAR addr[COIL_CHN];
		CDV_CHAR id[COIL_CHN];//一个tab的id一致，0为本机
		CDV_CHAR cnt[COIL_CHN];//是否需要写，用critical
		CDV_CHAR cnted[COIL_CHN];//
	}COIL_MAP;//用于writecoil的标记
	
/** @brief  读到变量 寄存器
  * @param  buf     命令
            len     buf长度
            inReg   寄存器保存地址
  * @retval 
  * @note
  */
RET_STATUS BufToInReg2(CDV_INT08U* buf, CDV_INT08U len, CDV_INT16U* inReg, CDV_INT16U addr){
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT16U* pInReg = (CDV_INT16U*)(buf + 3);
	RET_STATUS ret = OPT_FAILURE;
	
	ASSERT(buf && len && inReg);
	
	if(0x04 != fc || len < numCh + 3)
		return ret;
	
	
	for (i = 0;i < numCh/2; i++) {
	  inReg[addr + i] = ENDIAN_TF16U(pInReg[i]);
	}
	
	ret = OPT_SUCCESS;
	return ret;
}
/** @brief  读到变量 寄存器
  * @param  buf     命令
            len     buf长度
            inReg   寄存器保存地址
  * @retval 
  * @note
  */
RET_STATUS BufToInReg(CDV_INT08U* buf, CDV_INT08U len, CDV_INT16U* inReg){
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT16U* pInReg = (CDV_INT16U*)(buf + 3);
	RET_STATUS ret = OPT_FAILURE;
	
	ASSERT(buf && len && inReg);
	
	if(0x04 != fc || len < numCh + 3)
		return ret;
	
	
	for (i = 0;i < numCh/2; i++) {
	  inReg[i] = ENDIAN_TF16U(pInReg[i]);
	}
	
	ret = OPT_SUCCESS;
	return ret;
}
/** @brief  读到变量 寄存器
  * @param  buf     命令
            len     buf长度
            reg   寄存器保存地址
  * @retval 
  * @note
  */
RET_STATUS BufToReg2(CDV_INT08U* buf, CDV_INT08U len, CDV_INT16U* reg, CDV_INT16U addr){
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT16U* pReg = (CDV_INT16U*)(buf + 3);
	RET_STATUS ret = OPT_FAILURE;
	
	ASSERT(buf && len && reg);
	
	if(0x03 != fc || len < numCh + 3)
		return ret;
	
	
	for (i = 0;i < numCh/2; i++) {
	  reg[addr + i] = ENDIAN_TF16U(pReg[i]);
	}
	
	ret = OPT_SUCCESS;
	return ret;
}
/** @brief  读到变量 寄存器
  * @param  buf     命令
            len     buf长度
            reg   寄存器保存地址
  * @retval 
  * @note
  */
RET_STATUS BufToReg(CDV_INT08U* buf, CDV_INT08U len, CDV_INT16U* reg){
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT16U* pReg = (CDV_INT16U*)(buf + 3);
	RET_STATUS ret = OPT_FAILURE;
	
	ASSERT(buf && len && reg);
	
	if(0x03 != fc || len < numCh + 3)
		return ret;
	
	
	for (i = 0;i < numCh/2; i++) {
	  reg[i] = ENDIAN_TF16U(pReg[i]);
	}
	
	ret = OPT_SUCCESS;
	return ret;
}
/** @brief  读到变量 线圈
  * @param  buf     命令
            len     buf长度
            inCoil   寄存器保存地址
  * @retval 
  * @note
  */
RET_STATUS BufToInCoil(CDV_INT08U* buf, CDV_INT08U len, CDV_INT08U* inCoil){
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT08U* pInCoil = (CDV_INT08U*)(buf + 3);
	RET_STATUS ret = OPT_FAILURE;
	
	ASSERT(buf && len && inCoil);
	
	if(0x02 != fc || len < numCh + 3)
		return ret;
	
	MemCpy(inCoil, pInCoil, numCh);
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  读到变量 线圈
  * @param  buf     命令
            len     buf长度
            inCoil  寄存器保存地址
            addr    coil的起始地址0~
  * @retval 
  * @note   不考虑线圈数量
  */
RET_STATUS BufToInCoil2(CDV_INT08U* buf, CDV_INT08U len, CDV_INT08U* inCoil, CDV_INT16U addr) {
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT08U* pInCoil = (CDV_INT08U*)(buf + 3);
	CDV_INT16U remoteaddr = 0;
	CDV_INT16U localaddr = addr;
	RET_STATUS ret = OPT_FAILURE;
	CDV_INT16U	sta, sf;
	
	ASSERT(buf && len && inCoil);
	
	if(0x02 != fc || len < numCh + 3)
		return ret;
	
	/////////
	sta = (localaddr >> 3);//读线圈的初始char
	sf = localaddr & 0x07;
	
	//所处字节
	for(i = 0; i < numCh; i++) {
		inCoil[sta+i] = (inCoil[sta+i] & ~(0xFF<<sf)) | pInCoil[i]<<(sf);
		if (sf /*&& i + 1 < numCh*/) 
			inCoil[sta+i+1] = (inCoil[sta+i+1] & ~(0xFF>>(8-sf))) | pInCoil[i]>>(8-sf);
	}
	
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  读到变量 线圈
  * @param  buf     命令
            len     buf长度
            inCoil  寄存器保存地址
            addr    coil的起始地址0~
  * @retval 
  * @note   考虑线圈数量
  */
RET_STATUS BufToInCoil3(CDV_INT08U* buf, CDV_INT08U len, CDV_INT08U* inCoil, CDV_INT16U addr, CDV_INT16U num) {
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT08U firstCh, secondCh;
	CDV_INT08U* pInCoil = (CDV_INT08U*)(buf + 3);
	CDV_INT16U remoteaddr = 0;
	CDV_INT16U localaddr = addr;
	RET_STATUS ret = OPT_FAILURE;
	CDV_INT16U	sta, sf, endnum;
	
	ASSERT(buf && len && inCoil);
	
	if(0x02 != fc || len < numCh + 3)
		return ret;
	
	/////////
	sta = (localaddr >> 3);//读线圈的初始char
	sf = localaddr & 0x07;
  endnum = num - (numCh - 1) * 8;//最后一个字节的线圈数量
	//所处字节
	for(i = 0; i < numCh; i++) {
		//移位复制
		firstCh = (inCoil[sta+i] & ~(0xFF<<sf)) | pInCoil[i]<<(sf);
		
		if (sf /*&& i + 1 < numCh*/) {
			secondCh = (inCoil[sta+i+1] & ~(0xFF>>(8-sf))) | pInCoil[i]>>(8-sf);
		} else {
			secondCh = inCoil[sta+i+1];
		}
		
		
		if(i + 1 == numCh && endnum) {//最后一个字节复制特殊
			/*
			情况一
						h              l
			first 0 0 0 0  0 0 0 0
											 |sf |    
							| endnum |     
			情况二
			second            first
			h		           l  h              l
			0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0
						            				 |  sf |    
						 	     |  endnum     |     
			*/
		  if(8 - sf >= endnum) {//大于等于endnum数量，说明复制firstCh足矣，保留sta + endnum的数量
				inCoil[sta+i] = (inCoil[sta+i]  & (0xFF<<(sf + endnum)))   | (firstCh & (0xFF >> (8-sf - endnum)));
			} else {
				inCoil[sta+i] = firstCh;
				inCoil[sta+i+1] = (inCoil[sta+i+1]  & (0xFF<<(sf + endnum-8)))   | (secondCh & (0xFF >> (16-sf - endnum)));
			}
		} else {
		  inCoil[sta+i] = firstCh;
		  inCoil[sta+i+1] = secondCh;
		}
	}
	
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  读到变量 线圈
  * @param  buf     命令
            len     buf长度
            coil   寄存器保存地址
  * @retval 
  * @note
  */
RET_STATUS BufToCoil(CDV_INT08U* buf, CDV_INT08U len, CDV_INT08U* coil){
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT08U* pCoil = (CDV_INT08U*)(buf + 3);
	RET_STATUS ret = OPT_FAILURE;
	
	ASSERT(buf && len && coil);
	
	if(0x01 != fc || len < numCh + 3)
		return ret;
	
	MemCpy(coil, pCoil, numCh);
	
	ret = OPT_SUCCESS;
	return ret;
}
/** @brief  读到变量 线圈
  * @param  buf     命令
            len     buf长度
            coil   寄存器保存地址
  * @retval 
  * @note   不考虑线圈数量
  */
RET_STATUS BufToCoil2(CDV_INT08U* buf, CDV_INT08U len, CDV_INT08U* coil, CDV_INT16U addr){
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT08U* pCoil = (CDV_INT08U*)(buf + 3);
	CDV_INT16U remoteaddr = 0;
	CDV_INT16U localaddr = addr;
	RET_STATUS ret = OPT_FAILURE;
	CDV_INT16U	sta, sf;
	
	ASSERT(buf && len && coil);
	
	if(0x01 != fc || len < numCh + 3)
		return ret;
	
	/////////
	sta = (localaddr >> 3);//读线圈的初始char
	sf = localaddr & 0x07;
	
	//所处字节
	for(i = 0; i < numCh; i++) {
		coil[sta+i] = (coil[sta+i] & ~(0xFF<<sf)) | pCoil[i]<<(sf);
		if (sf/*i + 1 < numCh*/)
			coil[sta+i+1] = (coil[sta+i+1] & ~(0xFF>>(8-sf))) | pCoil[i]>>(8-sf);
	}
	
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  读到变量 线圈
  * @param  buf     命令
            len     buf长度
            coil   寄存器保存地址
  * @retval 
  * @note   不考虑线圈数量
  */
RET_STATUS BufToCoil3(CDV_INT08U* buf, CDV_INT08U len, CDV_INT08U* coil, CDV_INT16U addr, CDV_INT16U num){
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U i;
	CDV_INT08U firstCh, secondCh;
	CDV_INT08U* pCoil = (CDV_INT08U*)(buf + 3);
	CDV_INT16U remoteaddr = 0;
	CDV_INT16U localaddr = addr;
	RET_STATUS ret = OPT_FAILURE;
	CDV_INT16U	sta, sf, endnum;
	
	ASSERT(buf && len && coil);
	
	if(0x01 != fc || len < numCh + 3)
		return ret;
	
	/////////
	sta = (localaddr >> 3);//读线圈的初始char
	sf = localaddr & 0x07;
  endnum = num - (numCh - 1) * 8;//最后一个字节的线圈数量
	
	//所处字节
//	for(i = 0; i < numCh; i++) {
//		coil[sta+i] = (coil[sta+i] & ~(0xFF<<sf)) | pCoil[i]<<(sf);
//		if (sf/*i + 1 < numCh*/)
//			coil[sta+i+1] = (coil[sta+i+1] & ~(0xFF>>(8-sf))) | pCoil[i]>>(8-sf);
//	}
	for(i = 0; i < numCh; i++) {
		//移位复制
		firstCh = (coil[sta+i] & ~(0xFF<<sf)) | pCoil[i]<<(sf);
		
		if (sf /*&& i + 1 < numCh*/) {
			secondCh = (coil[sta+i+1] & ~(0xFF>>(8-sf))) | pCoil[i]>>(8-sf);
		} else {
			secondCh = coil[sta+i+1];
		}
		
		
		if(i + 1 == numCh && endnum) {//最后一个字节复制特殊
		  if(8 - sf >= endnum) {//大于等于endnum数量，说明复制firstCh足矣，保留sta + endnum的数量
				coil[sta+i] = (coil[sta+i]  & (0xFF<<(sf + endnum)))   | (firstCh & (0xFF >> (8-sf - endnum)));
			} else {
				coil[sta+i] = firstCh;
				coil[sta+i+1] = (coil[sta+i+1]  & (0xFF<<(sf + endnum-8)))   | (secondCh & (0xFF >> (16-sf - endnum)));
			}
		} else {
		  coil[sta+i] = firstCh;
		  coil[sta+i+1] = secondCh;
		}
	}
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  线圈复制
  * @param  buf     源
            bufaddr 源起始地址
            coil    目标
            coiladdr目标起始地址
            tonum   复制线圈数
  * @retval RET_STATUS
  * @note   
  */
RET_STATUS CoilToCoil(CDV_INT08U* buf, CDV_INT08U bufaddr, CDV_INT08U* coil, CDV_INT16U coiladdr, CDV_INT16U tonum){
	//CDV_INT08U fc = buf[1];、
	CDV_INT08U buf_sta_ch = bufaddr >> 3;
	CDV_INT08U buf_sf = bufaddr & 0x07;
	CDV_INT08U coil_sta_ch = (coiladdr >> 3);//读线圈的初始char
	CDV_INT08U coil_sf = coiladdr & 0x07;
	CDV_INT16U num = tonum;
	CDV_INT08U numCh = (num + 7) / 8;//标准长度，sf = 0
	CDV_INT08U i;
	CDV_INT08U firstCh, secondCh;
	CDV_INT08U* pCoil = (CDV_INT08U*)(buf + buf_sta_ch);
	RET_STATUS ret = OPT_FAILURE;
	CDV_INT16U buf_end_num;
	CPU_SR_ALLOC();
	
	ASSERT(buf  && coil);

	CPU_CRITICAL_ENTER();
	
	if(buf_sf) {//存在sf，特殊处理第一个字节中的数据
		if(8-buf_sf>num)//判断数量是不是很少
			i = (0xFF>>(8-(buf_sf+num)));
		else
			i = 0xFF;
		
		if(buf_sf >= coil_sf) {
		  coil[coil_sta_ch] = (coil[coil_sta_ch] & ~(((0xFF<<buf_sf)&i)>>(buf_sf-coil_sf))) | (pCoil[0]&i)>>buf_sf<<coil_sf;
		}
		else {
			coil[coil_sta_ch] = (coil[coil_sta_ch] & ~(((0xFF<<buf_sf)&i)<<(coil_sf-buf_sf))) | (pCoil[0]&i)>>buf_sf<<coil_sf;
			
			if(coil_sf + num > 8) {
			  coil[coil_sta_ch + 1] = (coil[coil_sta_ch + 1] & ~(((0xFF<<buf_sf)&i)>>(8 - coil_sf + buf_sf))/*0xFF<<(coil_sf - buf_sf)*/) | (pCoil[0]&i)>>(8 - coil_sf + buf_sf);
			}
		}
		
		buf_sta_ch += 1;
		pCoil = (CDV_INT08U*)(buf + buf_sta_ch);
		num -= 8 - buf_sf;
		numCh = (num + 7) / 8;
		coil_sta_ch = ((coiladdr+(8 - buf_sf)) >> 3);//读线圈的初始char
	  coil_sf = (coiladdr+(8 - buf_sf)) & 0x07;
	}
	/////////
  buf_end_num = num - (numCh - 1) * 8;//最后一个字节的线圈数量
	

	for(i = 0; i < numCh; i++) {
		//移位复制
		firstCh = (coil[coil_sta_ch+i] & ~(0xFF<<coil_sf)) | pCoil[i]<<(coil_sf);
		
		if (coil_sf /*&& i + 1 < numCh*/) {
			secondCh = (coil[coil_sta_ch+i+1] & ~(0xFF>>(8-coil_sf))) | pCoil[i]>>(8-coil_sf);
		} else {
			secondCh = coil[coil_sta_ch+i+1];
		}
		
		
		if(i + 1 == numCh && buf_end_num) {//最后一个字节复制特殊
		  if(8 - coil_sf >= buf_end_num) {//大于等于endnum数量，说明复制firstCh足矣，保留sta + endnum的数量
				coil[coil_sta_ch+i] = (coil[coil_sta_ch+i]  & (0xFF<<(coil_sf + buf_end_num)))   | (firstCh & (0xFF >> (8-coil_sf - buf_end_num)));
			} else {
				coil[coil_sta_ch+i] = firstCh;
				coil[coil_sta_ch+i+1] = (coil[coil_sta_ch+i+1]  & (0xFF<<(coil_sf + buf_end_num-8)))   | (secondCh & (0xFF >> (16-coil_sf - buf_end_num)));
			}
		} else {
		  coil[coil_sta_ch+i] = firstCh;
		  coil[coil_sta_ch+i+1] = secondCh;
		}
	}
	
	OS_CRITICAL_EXIT_NO_SCHED();
	
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  线圈比较
  * @param  参考CoilToCoil
  * @retval 0 相同，1 不相同
  * @note   
  */
int CoilCmp(CDV_INT08U* buf, CDV_INT16U bufaddr, CDV_INT08U* coil, CDV_INT16U coiladdr, CDV_INT16U tonum){
	CDV_INT08U coil_sta_ch = (coiladdr >> 3);//读线圈的初始char
	CDV_INT08U coil_sf = coiladdr & 0x07;
	CDV_INT08U coil_numCh = (tonum - 8 + coil_sf + 7) / 8 + (coil_sf ? 1 :0);//标准长度，sf = 0
	int ret = 0,i = 0;
	
#if USE_CASCADE_STATIC == 1u
	CDV_INT08U *tmp_buf = cascade_tmp_buf;
#else
	CDV_INT08U* tmp_buf = NULL;
#endif
	
	ASSERT(buf  && coil);
	
#if USE_CASCADE_STATIC == 0u
	NEWCH(tmp_buf, coil_numCh);
#endif
	
	MemCpy(tmp_buf, coil + coil_sta_ch, coil_numCh);

	CoilToCoil(buf, bufaddr, tmp_buf, coil_sf, tonum);
	
	for(i = 0; i< coil_numCh; i++) {
		if((CDV_INT08C*)tmp_buf[i] != (CDV_INT08C*)(coil + coil_sta_ch)[i]) {
			ret = 1;
			break;
		}
	}
//	if(0 == strncmp((CDV_INT08C*)tmp_buf, (CDV_INT08C*)(coil + coil_sta_ch), coil_numCh)) {
//		ret = 0;
//	}

#if USE_CASCADE_STATIC == 0u
	DELETE(tmp_buf);
#endif
	return ret;
}

/** @brief   寄存器比较
  * @param  参考CoilToCoil
  * @retval 0 相同，1 不相同
  * @note   
  */
int RegCmp(CDV_INT16U* buf, CDV_INT16U bufaddr, CDV_INT16U* reg, CDV_INT16U regaddr, CDV_INT16U tonum){
	int ret = 0,i = 0;

	for(i = 0; i< tonum; i++) {
		if(buf[i+bufaddr] != reg[i+regaddr]) {
			ret = 1;
			break;
		}
	}
	
	return ret;
}


/** @brief  读取从机的寄存器到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
  */

	RET_STATUS CascadeModbus_ReadReg2(CDV_INT08U* pBuffer, CDV_INT16U startaddr, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		CDV_INT08U *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		CDV_INT08U* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT16U* pReg = (CDV_INT16U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadRegisterCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart,opt);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToReg2(recvBuf, recvLen, pReg, startaddr);
		}
		#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
		#endif
		return ret;
	}
/** @brief  读取从机的寄存器到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
  */

	RET_STATUS CascadeModbus_ReadReg(CDV_INT08U* pBuffer, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT16U* pReg = (CDV_INT16U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadRegisterCmd(id, addr, num, &cmdBuf, &cmdLen,BUF_NEW);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart,BUF_NEW);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToReg(recvBuf, recvLen, pReg);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  读取从机的只读寄存器到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
  */

	RET_STATUS CascadeModbus_ReadInReg2(CDV_INT08U* pBuffer, CDV_INT16U startaddr, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		CDV_INT08U *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		CDV_INT08U* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT16U* pInReg = (CDV_INT16U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInRegisterCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart,opt);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInReg2(recvBuf, recvLen, pInReg, startaddr);
		}
		#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
		#endif
		return ret;
	}
/** @brief  读取从机的只读寄存器到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
  */

	RET_STATUS CascadeModbus_ReadInReg(CDV_INT08U* pBuffer, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT16U* pInReg = (CDV_INT16U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInRegisterCmd(id, addr, num, &cmdBuf, &cmdLen,BUF_NEW);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart,BUF_NEW);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInReg(recvBuf, recvLen, pInReg);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  读取从机的线圈到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
  */

	RET_STATUS CascadeModbus_ReadCoil2(CDV_INT08U* pBuffer, CDV_INT16U startaddr, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		CDV_INT08U *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		CDV_INT08U* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT08U* pCoil = (CDV_INT08U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadCoilCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart,opt);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToCoil3(recvBuf, recvLen, pCoil, startaddr, num);
		}
		#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
		#endif
		return ret;
	}
/** @brief  读取从机的线圈到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
  */

	RET_STATUS CascadeModbus_ReadCoil(CDV_INT08U* pBuffer, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT08U* pCoil = (CDV_INT08U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadCoilCmd(id, addr, num, &cmdBuf, &cmdLen,BUF_NEW);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart,BUF_NEW);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToCoil(recvBuf, recvLen, pCoil);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  读取从机的只读线圈到本机
  * @param  pBuffer      查询到的值保存的地方
	          startaddr    保存的起始地址
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
  */

	RET_STATUS CascadeModbus_ReadInCoil2(CDV_INT08U* pBuffer, CDV_INT16U startaddr, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		CDV_INT08U *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		CDV_INT08U* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT08U* pInCoil = (CDV_INT08U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInCoilCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart,opt);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInCoil3(recvBuf, recvLen, pInCoil, startaddr, num);
		}
#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
#endif
		return ret;
	}
/** @brief  读取从机的只读线圈到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
  */

	RET_STATUS CascadeModbus_ReadInCoil(CDV_INT08U* pBuffer, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT08U* pInCoil = (CDV_INT08U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInCoilCmd(id, addr, num, &cmdBuf, &cmdLen,BUF_NEW);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart,BUF_NEW);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInCoil(recvBuf, recvLen, pInCoil);
		}
		DELETE(cmdBuf);
		return ret;
	}
	//从机更新
		CDV_INT16U gp_val[6] = {50, 100, 150, 200, 250, 300};
		CDV_INT08U gp_coil[2] = {0xf0, 0x00};
	void CascadeModbus_AllUpdate(void) {
		CDV_INT08U id;
		CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
		CDV_INT08U recvLen = 0;
		CDV_INT16U p_val[6];
		CDV_INT08U p_coil[2];
		ASSERT(recvBuf);
		MemCpy(p_coil,gp_coil,2);
		MemCpy(p_val,gp_val,12);
		
		WriteMultiRegisterCmd(2, 0x10, 6, (CDV_INT08U*)p_val, &cmdBuf, &cmdLen,BUF_NEW);
		UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, CASCADE_USART,BUF_NEW);
		delay_ms(10);
		WriteMultiCoilCmd(3, 0, 10, (CDV_INT08U*)p_coil, &cmdBuf, &cmdLen,BUF_NEW);
		UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, CASCADE_USART,BUF_NEW);
		delay_ms(10);
		DELETE(cmdBuf);
		
		id = 2;
		CascadeModbus_ReadReg((CDV_INT08U*)&g_modbusReg.reg[600], id, 0x10, 6, CASCADE_USART);
		delay_ms(10);
		CascadeModbus_ReadInReg((CDV_INT08U*)&g_modbusInReg.reg[20], id, 0, 6, CASCADE_USART);
		delay_ms(10);
		id = 3;
		CascadeModbus_ReadCoil((CDV_INT08U*)&g_modbusCoil.coilCh[20], id, 0, 10, CASCADE_USART);
		delay_ms(10);
		CascadeModbus_ReadInCoil((CDV_INT08U*)&g_modbusInCoil.coilCh[20], id, 0,10, CASCADE_USART);
		delay_ms(10);
	}
	
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	/////////////Cascade Map Function//////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
	struct CASCADE_MAP{
		u8 type;	//标注类型：I:0 0:1 DA:2 AD:3 ……
		u8 host;//主机号
		u16 typeno;//例：起始DA号
		u16 localaddr;//主机的映射基址
		u16 remoteaddr;//远程地址，一般是0
		u16 remotenum;//数量：比如DA的数量
		u16 len;//长度：比如一个DA所占资源（地址）的长度，即占多少个线圈/寄存器
		//u8 spec;//标注类型int?short?
		u8 dowrite[3];
		u8* wbuf;//待写的buf
	} ;
	
	struct CASCADE_MAP* CascadeMap = NULL;
	CDV_INT08U CascadeMapLen = 0;

	
/** @brief  映射初始化
  * @param  void
  * @retval RET_STATUS
  * @note   参考说明文档
  */
	
	RET_STATUS CascadeModbus_MapInit(void) {
		#define MAP_LINE_LEN 6	
		#define REG_SHIFT  CDV_VAR_NUM * 2
		#define INREG_SHIFT  18                       //放在ADDA
		#define COIL_SHIFT  CDV_O_NUM + CDV_EXO_NUM*CDV_FPGA_NUM
		#define INCOIL_SHIFT  CDV_I_NUM + CDV_EXI_NUM*CDV_FPGA_NUM
    RET_STATUS ret;
		//struct CASCADE_MAP* map = CascadeMap;
		/////////////////////////////
		CDV_INT32U size = 0;//脚本大小，供跳转
		CDV_INT32U shift = 0;//偏移起始地址的字节数
		CDV_INT32U lineMapAddr = SCRIP_MAP;//生产线映射的起始地址
	  CDV_INT32U lineMapLen = 0;//生产线映射的长度
		CDV_INT08U every_map[MAP_LINE_LEN];
		CDV_INT08U i;
		CDV_INT16U regshift = REG_SHIFT;
		CDV_INT16U inregshift = INREG_SHIFT;
		CDV_INT16U coilshift = COIL_SHIFT;
		CDV_INT16U incoilshift = INCOIL_SHIFT;
		
		if(!g_line.init) 
			return OPT_FAILURE;
		
		SPI_Flash_Read((CDV_INT08U*)&size, lineMapAddr + shift, 4);//读取生产线映射脚本大小
		lineMapLen = size;
		shift += 4;
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, lineMapAddr + shift, 1);//读取生产线映射个数
		CascadeMapLen = size;
		NEWCH(CascadeMap, CascadeMapLen * sizeof(struct CASCADE_MAP));
		shift += 1;
		size = 0;
		
		if (lineMapLen < CascadeMapLen * MAP_LINE_LEN + 1)
		{
			CascadeMapLen = 0;
		  DELETE(CascadeMap);
			return OPT_FAILURE;
		}
		
		
		
		for( i = 0; i < CascadeMapLen; i++) {
		  SPI_Flash_Read((CDV_INT08U*)&every_map, lineMapAddr + shift, MAP_LINE_LEN);//读取一行映射
		  CascadeMap[i].type = every_map[0];
			CascadeMap[i].host = every_map[1];
			CascadeMap[i].typeno = *(CDV_INT16U*)(every_map + 2);
			CascadeMap[i].remotenum = *(CDV_INT16U*)(every_map + 4);/*通道数量*/
			CascadeMap[i].remoteaddr = 0;
			shift += MAP_LINE_LEN;
			switch (CascadeMap[i].type) {
				case 0://I
			    CascadeMap[i].localaddr = incoilshift;/*在主机的映射基址*/
				  CascadeMap[i].len = 1;
					incoilshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(CDV_INT16U*)(every_map + 4);
				  break;
				case 1://O
					CascadeMap[i].localaddr = coilshift;
				  CascadeMap[i].len = 1;
					coilshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(CDV_INT16U*)(every_map + 4);
				  break;
				case 2://DA
					CascadeMap[i].localaddr = regshift;
				  CascadeMap[i].len = 1;
					regshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(CDV_INT16U*)(every_map + 4);
				  break;
				case 3://AD
					CascadeMap[i].localaddr = inregshift;
				  CascadeMap[i].len = 1;
					inregshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(CDV_INT16U*)(every_map + 4);
				  break;
				default:
					break;
			}
			
		}
		
		NEWCH(cascade_recv_buf, CASCADE_BUF_LEN);
		
#if USE_CASCADE_STATIC == 1u
		NEWCH(cascade_cmd_buf, CASCADE_BUF_MAX_LEN);
		NEWCH(cascade_tmp_buf, CASCADE_BUF_MAX_LEN);
#endif
		
		
#if USE_OVERLAP
	  NEWCH(g_coilCascade, sizeof(MODBUS_Coil));
	  NEWCH(g_regCascade, sizeof(MODBUS_Register));
#endif
		/////////////////////////////检测从机是否挂载
		if(DIP_ON == READ_DIP_SW(2))
		{
			if(OPT_FAILURE == CascadeModbus_Map()) {
				OUT_DisPlay(0xFF280B48);
				//OUT_DisPlay(0xFF511751);
				while(1);
			}
		}
		/////////////////////////////
		return OPT_SUCCESS;
	}
	
	
/**
  * @brief  O异步刷新
  *  
  * @param  coilVal 线圈的值，必须已经如modbus那样整理好
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   					
  */

	RET_STATUS CascadeOverlapOWrite(CDV_INT08U host, CDV_INT16U remoteaddr, CDV_INT32U num, CDV_INT08U* coilVal) { 
		RET_STATUS ret =OPT_SUCCESS;
#if USE_CASCADE_STATIC == 1u
		CDV_INT08U *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		CDV_INT08U* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
		CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvBuf[20] = {0};
		CDV_INT08U  recvLen = 0;
		
		WriteMultiCoilCmd(host, remoteaddr, num, coilVal, &cmdBuf, &cmdLen,opt);
    
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, 20, &recvLen, CASCADE_USART,opt);
		
		if(OPT_SUCCESS == ret && host == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = OPT_SUCCESS;
		} else {
			ret = OPT_FAILURE;
		}
#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
#endif
		return ret;
	}
	/**
  * @brief  DA异步刷新
  *  
  * @param  regVal 值，必须已经如modbus那样整理好，不要用modbus的数组，会改变值
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   					
  */

	RET_STATUS CascadeOverlapDAWrite(CDV_INT08U host, CDV_INT16U remoteaddr, CDV_INT32U num, CDV_INT08U* regVal) { 
		RET_STATUS ret =OPT_SUCCESS;
#if USE_CASCADE_STATIC == 1u
		CDV_INT08U *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		CDV_INT08U* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
		CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvBuf[100] = {0};
		CDV_INT08U  recvLen = 0;
		
		WriteMultiRegisterCmd(host, remoteaddr, num, regVal, &cmdBuf, &cmdLen,opt);
		//WriteMultiCoilCmd(host, remoteaddr, num, coilVal, &cmdBuf, &cmdLen);
    
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, 100, &recvLen, CASCADE_USART,opt);
		
		if(OPT_SUCCESS == ret && host == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = OPT_SUCCESS;
		} else {
			ret = OPT_FAILURE;
		}
#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
#endif
		return ret;
	}
	
/** @brief  读取从机的寄存器到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
	          系统启动时先检测有无挂载，无挂载系统不能启动
  */

	RET_STATUS CascadeModbus_Map(void) {
		CDV_INT08U i;
		RET_STATUS ret = OPT_SUCCESS;
		struct CASCADE_MAP* map = CascadeMap;
#if USE_CASCADE_STATIC == 1u
		CDV_INT08U *tmp_buf = cascade_tmp_buf;
#endif
		//ASSERT(map);
		//ASSERT(g_coilCascade);//此函数无条件执行，不需要
		
		if(!g_line.init || !map || !g_coilCascade || !g_regCascade) 
			return ret;
		
		for( i = 0; i < CascadeMapLen; i++) {
			switch (map[i].type) {
				case 0://I
					ret = CascadeModbus_ReadInCoil2((CDV_INT08U*)g_modbusInCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				case 1://O
#if USE_OVERLAP
				  ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_coilCascade->coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(OPT_SUCCESS == ret && CoilCmp((CDV_INT08U*)g_coilCascade->coilCh, map[i].localaddr, (CDV_INT08U*)g_modbusCoil.coilCh, map[i].localaddr, map[i].remotenum)) {
#if USE_CASCADE_STATIC == 0u			
					  CDV_INT08U *tmp_buf = NULL;
						NEWCH(tmp_buf, map[i].remotenum / 8 + 3);
#endif
					ASSERT(map[i].localaddr >= CDV_O_NUM);
						CoilToCoil((CDV_INT08U*)(g_modbusCoil.coilCh), map[i].localaddr, tmp_buf, 0, map[i].remotenum);
						ret = CascadeOverlapOWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_buf);
#if USE_CASCADE_STATIC == 0u
					  DELETE(tmp_buf);
#endif
					}
#else
					ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_modbusCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 2://DA
#if USE_OVERLAP
				  ret = CascadeModbus_ReadReg2((CDV_INT08U*)g_regCascade->reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(OPT_SUCCESS == ret && RegCmp(g_regCascade->reg, map[i].localaddr, g_modbusReg.reg, map[i].localaddr, map[i].remotenum)) {
#if USE_CASCADE_STATIC == 0u	
						CDV_INT08U *tmp_buf = NULL;
						NEWCH(tmp_buf, 2*map[i].remotenum);
#endif
						MemCpy(tmp_buf, g_modbusReg.reg + map[i].localaddr, 2*map[i].remotenum);
						ret = CascadeOverlapDAWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_buf);
#if USE_CASCADE_STATIC == 0u	
						DELETE(tmp_buf);
#endif
					}
#else
					ret = CascadeModbus_ReadReg2((CDV_INT08U*)g_modbusReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 3://AD
					ret = CascadeModbus_ReadInReg2((CDV_INT08U*)g_modbusInReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				default:
					break;
			}
			
			if(ret == OPT_FAILURE)
				return ret;
			
			//DelayTick(5);
			delay_ms(1);
		}
		return ret;
	}
	
/** @brief  利用map检查从机是否有挂载
  * @param  void
  * @retval RET_STATUS
  * @note   无挂载，系统不能启动
  */

	RET_STATUS CascadeModbus_MapCheck(void) {
		CDV_INT08U i;
		RET_STATUS ret = OPT_FAILURE;
		struct CASCADE_MAP* map = CascadeMap;
		
		if(!g_line.init || !map || !g_coilCascade || !g_regCascade) 
			return OPT_FAILURE;
		
		for( i = 0; i < CascadeMapLen; i++) {
			switch (map[i].type) {
				case 0://I
					ret = CascadeModbus_ReadInCoil2((CDV_INT08U*)g_modbusInCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				case 1://O
#if USE_OVERLAP
				  ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_coilCascade->coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(OPT_SUCCESS == ret && CoilCmp((CDV_INT08U*)g_coilCascade->coilCh, map[i].localaddr, (CDV_INT08U*)g_modbusCoil.coilCh, map[i].localaddr, map[i].remotenum)) {
						CDV_INT08U *tmp_coil_val = NULL;
						NEWCH(tmp_coil_val, map[i].remotenum / 8 + 3);
						CoilToCoil((CDV_INT08U*)(g_modbusCoil.coilCh), map[i].localaddr, tmp_coil_val, 0, map[i].remotenum);
						ret = CascadeOverlapOWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_coil_val);
						DELETE(tmp_coil_val);
					}
#else
					ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_modbusCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 2://DA
#if USE_OVERLAP
				  ret = CascadeModbus_ReadReg2((CDV_INT08U*)g_regCascade->reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(OPT_SUCCESS == ret && RegCmp(g_regCascade->reg, map[i].localaddr, g_modbusReg.reg, map[i].localaddr, map[i].remotenum)) {
						//CDV_INT08U *tmp_coil_val = NULL;
						//NEWCH(tmp_coil_val, map[i].remotenum / 8 + 3);
						//CoilToCoil((CDV_INT08U*)(g_modbusCoil.coilCh), map[i].localaddr, tmp_coil_val, 0, map[i].remotenum);
						ret = CascadeOverlapDAWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, (CDV_INT08U*)(g_regCascade->reg + map[i].localaddr));
						//DELETE(tmp_coil_val);
					}
#else
					ret = CascadeModbus_ReadReg2((CDV_INT08U*)g_modbusReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 3://AD
					ret = CascadeModbus_ReadInReg2((CDV_INT08U*)g_modbusInReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				default:
					break;
			}
			
			if(ret == OPT_FAILURE)
				return ret;
			
			delay_ms(1);
		}
		return ret;
	}
	
/** @brief  得到一个具体资源的localaddr
  * @param  type          类型
	*         hosttypeno     host所在的序号，比如2号机的da1
  *         host          主机号
  *         retno         返回no
  * @retval RET_STATUS
  * @note   
  */

	
	RET_STATUS CascadeModbus_MapFind(
	                    const CDV_INT08U type,
										  const CDV_INT16U hosttypeno,
										  const CDV_INT08U host,
											CDV_INT16U* localaddr,
										  CDV_INT16U* remoteaddr) {
		CDV_INT08U i;
		struct CASCADE_MAP* map = CascadeMap;
		
		if (!g_line.init || !map) 
			return OPT_FAILURE;
		

		
		for (i = 0; i < CascadeMapLen; i++) {
			if (map[i].type != type || map[i].host != host)
				continue;
			
			if (hosttypeno >= map[i].remotenum)
				break;
			
			if (localaddr)
			  *localaddr = map[i].localaddr + hosttypeno;
			
			if (remoteaddr)
			  *remoteaddr = map[i].remoteaddr + hosttypeno;
			
//			if (typeno < map[i].typeno || typeno >= map[i].typeno + map[i].remotenum)
//				continue;
//			if (localaddr)
//			  *localaddr = map[i].localaddr + typeno - map[i].typeno;
//			
//			if (remoteaddr)
//			  *remoteaddr = map[i].remoteaddr + typeno - map[i].typeno;
			
			return OPT_SUCCESS;
		}
		
		return OPT_FAILURE;
	}
	
/** @brief  得到一个具体资源的localaddr
  * @param  type          类型 I:0/O:1/DA:2/AD:3 
	*         typeno        序号（包括主机与非主机）
	*         localaddr     主机的映射地址
	*         remoteaddr    从机的所在地址
  *         host          从机的主机号
  * @retval RET_STATUS
  * @note   
  */

	RET_STATUS CascadeModbus_MapFindInExp(
	                    const CDV_INT08U type,
										  const CDV_INT16U typeno,
											CDV_INT16U* localaddr,
										  CDV_INT16U* remoteaddr,
											CDV_INT08U* host) {
												
												
		CDV_INT08U i;
		struct CASCADE_MAP* map = CascadeMap;
		CDV_INT16U shift = 0;
		//CDV_INT16U len = 0;
		
		if (!g_line.init || !map) 
			return OPT_FAILURE;
		
		switch (type) {
			case 0://I
				shift = I_NUM;
				break;
			case 1://O
				shift = O_NUM;
				break;
			case 2://DA
				shift = DA_NUM;
				break;
			case 3://AD
				shift = AD_NUM;
				break;
			default:
				break;
		}
		
		if (typeno < shift)
			return OPT_FAILURE;
		
		for (i = 0; i < CascadeMapLen; i++) {
			if (map[i].type != type)
				continue;
			
			if (typeno >= shift + map[i].remotenum) {
				shift += map[i].remotenum * map[i].len;/*偏移量增加*/
				continue;
			}
			
			if (localaddr)
			  *localaddr = map[i].localaddr + (typeno - shift);/*得到主机的映射地址*/
			
			if (remoteaddr)
			  *remoteaddr = map[i].remoteaddr + (typeno - shift);/*得到从机的序号*/
			
			if (host)
			  *host = map[i].host;/*得到从机的主机号*/
			
			return OPT_SUCCESS;
		}
		
		return OPT_FAILURE;
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	/////////////Cascade Usart Function////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
/** @brief  非本机命令的转发
  * @param  ppBuffer      查询到的资源表
	*         pLen          资源表长度
  *         pCmdBuf       待发命令
  *         NumByteOfCmd  待发命令长度
  * @retval RET_STATUS
  * @note   ppBuffer需使用指针
  */

	RET_STATUS CascadeUsartTranferCmd(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT08U* pCmdBuf, CDV_INT16U NumByteOfCmd) {
		CDV_INT08U* tmp = NULL;
		CDV_INT16U crc;		
		
		ASSERT(NULL != pCmdBuf);
		ASSERT(0 != NumByteOfCmd);
		ASSERT(NULL != pLen);

		crc = getCRC16(pCmdBuf, NumByteOfCmd);

		
		NEW08U(tmp, NumByteOfCmd + 2);//最后是crc
		MemCpy(tmp, pCmdBuf, NumByteOfCmd);
		tmp[NumByteOfCmd] = crc & 0xff; 
	  tmp[NumByteOfCmd + 1] = (crc >> 8) & 0xff;
		NEW08U(*ppBuffer, 0xff);
		USARTTR((CDV_INT08U*)tmp, NumByteOfCmd + 2, *ppBuffer,0xff, pLen, CASCADE_USART);
		DELETE(tmp);
		
		if(!*pLen || !*ppBuffer)
			return OPT_FAILURE;
		else
		{
			*pLen -= 2;
			crc = getCRC16(*ppBuffer, *pLen);
			if(((crc&0xFF) == (*ppBuffer)[*pLen]) && ((crc>>8)&0xFF) == (*ppBuffer)[*pLen + 1] )	
				return OPT_SUCCESS;
			else
				return OPT_FAILURE;
		}
	}
	
/**
  * @brief  I脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明
            OPT_SUCCESS命令执行完成
            OPT_FAILURE命令未执行完成，比如等待未等到
  *
  * @note   主机id 01 64 01 资源号（4B）	00等待	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
  */

	RET_STATUS TFICmd(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg) { 
		//OS_ERR err;
		CDV_INT08U opt, type;
		CDV_INT16U localaddr, remoteaddr;
		CDV_INT32U no , num , val;
		//IO_VAL bit;
		RET_STATUS ret =OPT_SUCCESS;
		CDV_INT08U host = buf[0];

		no  = *(CDV_INT32U*)(buf + 4);
		opt = buf[8];
		type = buf[9];
		num  = *(CDV_INT32U*)(buf + 10);
    ////
		//host no的映射转化
		ret = CascadeModbus_MapFind(0, *(CDV_INT32U*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0xFF:/*查询*/
#if USE_OVERLAP
			  ret = OPT_SUCCESS;
#else
				ret = CascadeModbus_ReadInCoil2((CDV_INT08U*)g_modbusInCoil.coilCh, localaddr, host, 0, 1, CASCADE_USART);
#endif
			  if(OPT_SUCCESS == ret) {
//					bit = IRead(localaddr);
//					val = (BIT_1 == bit ? 1 : 0);
					val = READ_INCOIL_ADDR(localaddr);
					ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg,RC_CRC);
				}
				break;
			default:
				break;
		}

		return ret;
	}

/**
  * @brief  O脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   主机id 01 64 02 资源号（00-FF）	00输出	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
		                                FE	变量代号高位	变量代号低位							
            
  */

	RET_STATUS TFOCmd(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg) { 
   // OS_ERR err;
		CDV_INT08U opt, type;
		CDV_INT16U localaddr, remoteaddr;
		CDV_INT32U no , num , val;
		//IO_VAL bit;
		RET_STATUS ret =OPT_SUCCESS;
		CDV_INT08U host = buf[0];
		no  = *(CDV_INT32U*)(buf + 4);
		opt = buf[8];
		type = buf[9];
		//num  = *(CDV_INT32U*)(buf + 10);
    ////
		//host no的映射转化
		ret = CascadeModbus_MapFind(1, no, host, &localaddr, &remoteaddr);
		
		if(OPT_FAILURE == ret)
			return ret;
			
					
//		if(no!=0 && no != 5 && (0 == READ_COIL_ADDR(localaddr -1)))
//			no = no;
		////
		switch(opt) {
			case 0x00:/*输出*/			
				{
					CDV_INT08U  coil = 0;
#if !USE_OVERLAP
					CDV_INT08U* cmdBuf = NULL;
					CDV_INT08U  cmdLen = 0;
					CDV_INT08U recvBuf[20] = {0};
					CDV_INT08U  recvLen = 0;
#endif
//					bit = (ArithmeticEx((char*)buf + 10, len - 10, arg)) ? BIT_1 : BIT_0;
//					coil = bit;
					coil = (ArithmeticEx((char*)buf + 10, len - 10, arg)) ? 1 : 0;

#if USE_OVERLAP
					if(coil) {
						SET_COIL_ADDR(localaddr);
				  }
					else {
						RESET_COIL_ADDR(localaddr);
					}
#else
					WriteMultiCoilCmd(host, remoteaddr, 1, &coil, &cmdBuf, &cmdLen);

					ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, 20, &recvLen, CASCADE_USART,BUF_NEW);
												
					if(OPT_SUCCESS == ret && host == recvBuf[0] && 0x80 > recvBuf[1]) {
					  ret = OPT_SUCCESS;
					} else {
						ret = OPT_FAILURE;
					}
				
					DELETE(cmdBuf);
#endif
				}
				break;
			case 0x04:/*查询*/
			case 0xFF:/*查询*/
#if USE_OVERLAP
			  ret = OPT_SUCCESS;
#else
				ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_modbusCoil.coilCh, localaddr, host, remoteaddr, 1, CASCADE_USART);
#endif
				if(OPT_SUCCESS == ret) {
//					bit = ORead(localaddr); // 如果o不是addr0开始的则不能这样用
//					val = (BIT_1 == bit ? 1 : 0);
					val = READ_COIL_ADDR(localaddr);
					ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
				}
				break;
			default:
				break;
			
		}

		return ret;
	}
/**
  * @brief  ad脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   主机id 01 64 0C 资源号（00-FF）	00输出	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
		                                FE	变量代号高位	变量代号低位							
            
  */

	RET_STATUS TFDACmd(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg) { 
    OS_ERR err;
		CDV_INT08U opt, type;
		CDV_INT16U localaddr, remoteaddr;
		CDV_INT32U no , num , val;
		IO_VAL bit;
		RET_STATUS ret =OPT_SUCCESS;
		CDV_INT08U host = buf[0];
		no  = *(CDV_INT32U*)(buf + 4);
		opt = buf[8];
		type = buf[9];
		num  = *(CDV_INT32U*)(buf + 10);
    ////
		//host no的映射转化
		ret = CascadeModbus_MapFind(2, *(CDV_INT32U*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0x02:/*输出*/
				{
					CDV_INT16U  reg = 0;
#if !USE_OVERLAP
					CDV_INT08U* cmdBuf = NULL;
					CDV_INT08U  cmdLen = 0;
					CDV_INT08U  recvBuf[30] = {0};
					CDV_INT08U  recvLen = 0;
#endif
					reg = ArithmeticEx((char*)buf + 10, len - 10, arg) ;
#if USE_OVERLAP
					g_modbusReg.reg[localaddr] = reg;
#else
					WriteMultiRegisterCmd(host, remoteaddr, 1, (CDV_INT08U*)&reg, &cmdBuf, &cmdLen);
					ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, 30, &recvLen, CASCADE_USART,BUF_NEW);
				  
					if(OPT_SUCCESS == ret && host == recvBuf[0] && 0x80 > recvBuf[1]) {
						ret = OPT_SUCCESS;
					} else {
						ret = OPT_FAILURE;
					}
					
					DELETE(cmdBuf);
#endif
				}
				break;
			case 0xFF:/*查询*/
#if USE_OVERLAP
			  ret = OPT_SUCCESS;
#else
				ret = CascadeModbus_ReadReg2((CDV_INT08U*)g_modbusReg.reg, localaddr, host, remoteaddr, 1, CASCADE_USART);
#endif	
			if(OPT_SUCCESS == ret) {
					val = g_modbusReg.reg[localaddr];
					ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
				}
				break;
			default:
				break;
			
		}

		return ret;
	}
	/**
  * @brief  ad脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   主机id 01 64 0B 资源号（00-FF）	00输出	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
		                                FE	变量代号高位	变量代号低位							
            
  */

	RET_STATUS TFADCmd(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg) { 
    OS_ERR err;
		CDV_INT08U opt, type;
		CDV_INT16U localaddr, remoteaddr;
		CDV_INT32U no , num , val;
		IO_VAL bit;
		RET_STATUS ret =OPT_SUCCESS;
		CDV_INT08U host = buf[0];
		no  = *(CDV_INT32U*)(buf + 4);
		opt = buf[8];
		type = buf[9];
		num  = *(CDV_INT32U*)(buf + 10);
    ////
		//host no的映射转化
		ret = CascadeModbus_MapFind(3, *(CDV_INT32U*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0x04:/*查询*/
			case 0xFF:/*查询*/
				ret = CascadeModbus_ReadInReg2((CDV_INT08U*)g_modbusInReg.reg, localaddr, host, remoteaddr, 1, CASCADE_USART);
				if(OPT_SUCCESS == ret) {
					val = g_modbusInReg.reg[localaddr];
					ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
				}
				break;
			default:
				break;
		}

		return ret;
	}
/** @brief  CascadeTranferCmd之后的解析
  * @param  pBuf   命令
	*         len    命令长度
  * @retval RET_STATUS
  * @note   0x01	0x10	报文数据长度	主机ID	原指令
  */
  RET_STATUS TFUsartCmdParse (CDV_INT08U* buf, CDV_INT16U len, CMD_ARG *arg) {
		RET_STATUS ret = OPT_SUCCESS;
		
	//u32 gstartTime ,gendTime , gtime;
	//	gstartTime = GetCdvTimeTick();
		switch(buf[6]) {
			case 0x00:/*主轴命令*/
				
				break;
			case 0x01:/*I*/
				ret = TFICmd(buf + 3, len - 3, arg);
				break;
			case 0x02:/*O*/
				ret = TFOCmd(buf + 3, len - 3, arg);
				break;
			case 0x0B:
				ret = TFADCmd(buf + 3, len - 3, arg);
				break;
			case 0x0C:
				ret = TFDACmd(buf + 3, len - 3, arg);
				break;
			default:
				break;
		}
		
		if(!arg->ptrWorker && 0 == arg->reqlen){
			if(OPT_SUCCESS == ret) {
				ResRequest(arg->buf, arg->len, 0, 0, arg, RC_CRC);
			} else {
				if(arg->buf)
				arg->buf[1] += 0x80;
				ResRequest(arg->buf, arg->len, 0, 0, arg, RC_CRC);
			}
		}
		//gendTime = GetCdvTimeTick();
		//gtime = CalcCount(gendTime , gstartTime);
		return ret;
	}

