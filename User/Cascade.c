/**
  ******************************************************************************
  * @file    /Cascade.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-29
  * @brief   ����
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
	
  //������������
	#define	CASCADE_EN 			CASCADE_CS_EN;FPGA1_CS_DS;FPGA2_CS_DS;FLASH_CS_DS;
	//�ر���������
	#define	CASCADE_DS 			CASCADE_CS_DS;
	//�������ڷ�����
	#define CASCADE_ReadWriteByte(a)				SPIx_ReadWriteByte(SPI2,(a))

  //������Դ�����汾��
  CDV_INT08U version[] = {0/*id��*/, 3/*�����汾*/, 2/*Ӳ����汾*/, 3/*Ӳ�������汾*/, 6/*����С�汾*/};
	//�ӻ���Դ��
	CDV_INT08U *slaveTable = NULL;
	CDV_INT08U *slaveTable2 = NULL;
	CDV_INT08U slaveTableLen = 0;
	#define MAX_SLAVE_TABLE 50
	
  OLCMD_CACHE g_portCmdCache = {NULL , 0  , 0 , 0 , NULL};
	
	#define CASCADE_BUF_LEN 255
	//u8 g_cascadeBuf[CASCADE_BUF_LEN];//UserMemPtr(CCM_CASCADE_MODBUS);
	u8 *g_cascadeBuf = NULL;
	
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
	switch(buf[0]){//NPC��ַӦ��
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
	
/** @brief  ��ѯ��û�дӻ�
  * @param  
  * @retval ����1��������0
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

/** @brief  ��ѯ�ӻ��ظ����ȴ���
  * @param  ppBuffer ��ѯ�ɹ�����ӻ��ظ����ַ���
	*         pLen    ��ѯ�ɹ�����ӻ��ظ����ַ�������
	*         cnt     ��ѯʧ�ܵ�ѭ������
  * @retval ����ֵ˵��
  * @note   ʹ����Ӧ����Cascade_Master_Write֮���ѯ
	          �����������
  */
void Cascade_Master_Query(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT16U cnt)   
{ 
 	CDV_INT16U i = 0;    		
	CDV_INT08U len;
  OS_ERR err;	
	
	ASSERT(ppBuffer);
	ASSERT(pLen);
	

	CASCADE_EN;                            //ʹ������  
	//DelayUS(1000);
	do{
		DelayUS(100);
		*pLen = CASCADE_ReadWriteByte(0xFF);   //ѭ������  
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
				(*ppBuffer)[i] = CASCADE_ReadWriteByte(0xFF);   //ѭ������  
		}
	}
	else
	{
		*pLen = 0x00;
	}
	CASCADE_DS;                            //ȡ��Ƭѡ   

}  

/** @brief  ��ѯ�ӻ��ظ������ȴ���
  * @param  pBuffer          ��ѯ�����ַ���
	*         NumByteToRead    ��ѯ�ĳ���
  * @retval ����ֵ˵��
  * @note   �����������
  */
void Cascade_Master_Read(CDV_INT08U* pBuffer, CDV_INT16U NumByteToRead)   
{ 
 	CDV_INT16U i;    		
  OS_ERR err;	
	
	ASSERT(pBuffer);
	

	CASCADE_EN;                            //ʹ������  
	
	for(i = 0; i < NumByteToRead; i++)
	{
			pBuffer[i] = CASCADE_ReadWriteByte(0XFF);   //ѭ������  
	}
	CASCADE_DS;                            //ȡ��Ƭѡ   

}  
/** @brief  ������������
  * @param  pBuffer          ���͵��ַ���
	*         NumByteToWrite   ���͵��ַ�������
  * @retval ����ֵ˵��
  * @note   �����������
  */
void Cascade_Master_Write(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{
	//CDV_INT08U* temp = NULL;
 	CDV_INT16U i;    		
  OS_ERR err;	
	//NEW08U(temp, NumByteToWrite);
	
	ASSERT(pBuffer);
	

	CASCADE_EN;                            //ʹ������  
	//DelayUS(1000);
	for(i = 0 ; i < NumByteToWrite; i++)
	{
			/*temp[i] = */CASCADE_ReadWriteByte(pBuffer[i]);   //ѭ������  
	}
	CASCADE_DS;                            //ȡ��Ƭѡ   

	//DELETE(temp);
}

/** @brief  �������ͽ��սӿڣ����ڼ����ź���
  * @param  pTxBuf          ���͵��ַ��� NULL ������
	*         txByte          ���͵��ַ�������
  *         ppRxBuf         ��ѯ�ɹ�����ӻ��ظ����ַ���
	*         pRxLen          ��ѯ�ɹ�����ӻ��ظ����ַ�������
	*         rxCnt           ��ѯʧ�ܵ�ѭ������
  * @retval ����ֵ˵��
  * @note   �ⲿ��������װ�����������ʹ�øýӿڣ������뼶�����ߵ������豸��Դ�ײ�����ͻ
  */
void CascadeMasterTR2(CDV_INT08U* pTxBuf, CDV_INT16U txByte, CDV_INT08U** ppRxBuf, CDV_INT08U* pRxLen, CDV_INT16U rxCnt)   
{
  OS_ERR err;	
	u8 i,dat;
	OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	SPEED_CASCADE;
	
	if(pTxBuf && txByte) {
	  Cascade_Master_Write(pTxBuf, txByte);
	}
	DelayUS(10000);//���ӻ����ж�һ�㷴Ӧ��ʱ��
	if(ppRxBuf && pRxLen) {
	  Cascade_Master_Query(ppRxBuf, pRxLen, rxCnt);
	}
	
	CASCADE_EN;
	for(i = 0 ; i < 3; i++)
	  dat = CASCADE_ReadWriteByte(0xFF);
	
	CASCADE_DS;
	DelayUS(10000);//���ӻ����ж�һ�㷴Ӧ��ʱ��
//	CASCADE_CS_EN;
//	
//	CASCADE_CS_DS;
	SPEED_FPGA;
	OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
}

/** @brief  �������ͽ��սӿڣ����ڼ����ź��� ��CRCУ��汾
  * @param  pTxBuf          ���͵��ַ��� NULL ������
	*         txByte          ���͵��ַ�������
  *         ppRxBuf         ��ѯ�ɹ�����ӻ��ظ����ַ���
	*         pRxLen          ��ѯ�ɹ�����ӻ��ظ����ַ�������
	*         rxCnt           ��ѯʧ�ܵ�ѭ������
  * @retval ����ֵ˵��
  * @note   �ⲿ��������װ�����������ʹ�øýӿڣ������뼶�����ߵ������豸��Դ�ײ�����ͻ
  */
void CascadeMasterTR(CDV_INT08U* pTxBuf, CDV_INT16U txByte, CDV_INT08U** ppRxBuf, CDV_INT08U* pRxLen, CDV_INT16U rxCnt)   
{
  u8 i,dat;
	CDV_INT08U slen = 0;
	CDV_INT16U crc;
  CDV_INT08U* sbuf = NULL;
	OS_ERR err;	
	
	OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
  SPEED_CASCADE;
	slen = txByte + 2 ;
	NEW08U(sbuf, slen);//�����crc
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
		DelayUS(10000);//���ӻ����ж�һ�㷴Ӧ��ʱ��
		if(ppRxBuf && pRxLen) {
			Cascade_Master_Query(ppRxBuf, pRxLen, rxCnt);
		}
		
		//��������Ƕ��Ƶģ�������Σ���֤spi�ӻ���������0x00
		CASCADE_EN;
		for(i = 0 ; i < 3; i++)
			dat = CASCADE_ReadWriteByte(0xFF);
		
		CASCADE_DS;
		DelayUS(10000);//���ӻ����ж�һ�㷴Ӧ��ʱ��

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
			NEW08U(sbuf, slen);//�����crc
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
	OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
}

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	/////////////Cascade Slave Function////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////

/** @brief  �ӻ���������
  * @param  pBuffer          ���͵��ַ���
	*         NumByteToWrite   ���͵��ַ�������
  * @retval RET_STATUS
  * @note   ���̰߳�ȫ���������ط��ã�����
  */
RET_STATUS Cascade_Slave_Write2(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{ 
  if(!pBuffer || !NumByteToWrite)
		return OPT_FAILURE;
	
	
	
	return SPI3_Send(pBuffer, NumByteToWrite);
}  

/** @brief  �ӻ���������
  * @param  pBuffer          ���͵��ַ���
	*         NumByteToWrite   ���͵��ַ�������
  * @retval RET_STATUS
  * @note   ���̰߳�ȫ���������ط��ã�����
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
	
	NEW08U(tmp, NumByteToWrite + 2);//�����crc
	MemCpy(tmp, pBuffer, NumByteToWrite);
	//crc
	crc = getCRC16(pBuffer, NumByteToWrite);
	tmp[NumByteToWrite] = crc & 0xff; 
	tmp[NumByteToWrite + 1] = (crc >> 8) & 0xff;
	ret = SPI3_Send(tmp, NumByteToWrite + 2);
  DELETE(tmp);
	return ret;
}  
/** @brief  �Ǳ��������ת��
  * @param  ppBuffer      ��ѯ������Դ��
	*         pLen          ��Դ����
  *         pCmdBuf       ��������
  *         NumByteOfCmd  ���������
  * @retval RET_STATUS
  * @note   ppBuffer��ʹ��ָ��
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
//		NEW08U(tmp, NumByteOfCmd + 6 + 2);//�����crc
//		memcpy(tmp, "TFCMD:", 6);
//		memcpy(tmp + 6, pCmdBuf, NumByteOfCmd);
//		//crc
//		crc = getCRC16(pCmdBuf, NumByteOfCmd);
//		tmp[NumByteOfCmd + 6] = crc & 0xff; 
//	  tmp[NumByteOfCmd + 6 + 1] = (crc >> 8) & 0xff;
		
		
		NEW08U(tmp, NumByteOfCmd + 6);//�����crc
		MemCpy(tmp, "TFCMD:", 6);
		MemCpy(tmp + 6, pCmdBuf, NumByteOfCmd);
		
		CascadeMasterTR((CDV_INT08U*)tmp, NumByteOfCmd + 6/* + 2*/, ppBuffer, pLen, 0xFFFF);
		
		DELETE(tmp);
		
		if(!*pLen || !*ppBuffer)
			return OPT_FAILURE;
		else
		  return OPT_SUCCESS;
	}
	
/** @brief  CascadeTranferCmd֮��Ľ���
  * @param  pBuf   ����
	*         len    �����
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
	
	
	
/** @brief  �ж���û�дӻ�
  * @param  void
  * @retval void
  * @note   ppBuffer��ʹ��ָ��
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
/** @brief  ��������,�ӻ���Դ���ѯ
  * @param  ppBuffer   ��ѯ������Դ��
	*         pLen       ��Դ����
  * @retval RET_STATUS
  * @note   ppBuffer��ʹ��ָ��
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

/** @brief  ������Դ��ϲ�,�ݹ����дӻ�
  * @param  ppBuffer   ��ѯ������Դ��
	*         pLen       ��Դ����
  * @retval RET_STATUS
  * @note   
  */
  void CascadeCombine(CDV_INT08U no) {
		
		CDV_INT08U *buf = NULL;
	  CDV_INT08U len = 0;

		if(OPT_SUCCESS == SlaveCheck())
		{
		  CascadeCheck(&buf, &len, no);//��������ʱע��
		}
			
		NEW08U(slaveTable, 5 + len);
		slaveTableLen = 5 + len;
		
		slaveTable[0] = no;
		MemCpy(slaveTable + 1, version, 4);
		if(NULL != buf && 0 != len)
			MemCpy(slaveTable + 5, buf, len);
		
		DELETE(buf);
	}
	
/** @brief  ��Դ��,ֻ�б���
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
	
/** @brief  ��Դ���͸���λ��
  * @param  
  * @retval 
  * @note   
  */
	void CascadeTableSend(CDV_INT08U* buf, CDV_INT08U len,  CMD_ARG *arg) {
		ResRequest(buf, len, slaveTable, slaveTableLen, arg, RC_CRC);
	}
/** @brief  ��ȡ������
  * @param  
  * @retval �����š�0xFFû�б�����
  * @note   
  */
	CDV_INT08U CascadeGetNativeNo(void) {
		if(NULL == slaveTable2 || 0 == slaveTableLen)
			return 0xFF;
		
		return slaveTable2[0];
	}
	
	
	/**
  * @brief  �ӻ��������cache
  *         
  *  
  * @param  rxBuf     ��������
  *         rxLen     ���ճ���
  *           
  *   
  * @retval ����ֵ˵��0ʧ��1�ɹ�
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
  * @brief  �ӻ�������cache
  *         
  *  
  * @param  
  *           
  *   
  * @retval ����ֵ˵��
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


/** @brief  �ӻ��յ�����Ľ���
  * @param  pBuf   ����
	*         len    �����
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
			CascadeCombine(par + 1);//���+1Ϊ�������
			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		} 
		else if(0 == strncmp((CDV_INT08C*)pBuf, "TFCMD", pos - pBuf - 1))
		{
			
			if(!slaveTable)
			{
				return OPT_FAILURE;
			}
			
			if(CascadeGetNativeNo() != pos[3])//�Ǳ�����ת��
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
//				/*������Դ�ظ�����*/
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
//			CascadeCombine(par+1);//���+1Ϊ�������
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
	
	
/** @brief  �ӻ��յ�����Ľ���
  * @param  pBuf   ����
	*         len    �����
  * @retval RET_STATUS
  * @note   ��ȫ��CRC�жϣ���������ҪCRC
  */
  RET_STATUS CascadeSlaveParse2 (char* pBuf, CDV_INT08U len) {
		char* pos = strchr((char*)pBuf,':') + 1;
		int par = atoi(pos);
		
		CDV_INT16U crc;
		
		crc = getCRC16(pBuf, len-2); 
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
			CascadeCombine(par + 1);//���+1Ϊ�������
			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		} 
		else if(0 == strncmp((CDV_INT08C*)pBuf, "TFCMD", pos - pBuf - 1))
		{
			
			if(!slaveTable)
			{
				return OPT_FAILURE;
			}
			
			if(CascadeGetNativeNo() != pos[3])//�Ǳ�����ת��
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
//			CascadeCombine(par+1);//���+1Ϊ�������
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
	}REG_MAP;//����writereg�ı��
	
	struct
	{
		CDV_CHAR coil[COIL_CHN];
		CDV_CHAR addr[COIL_CHN];
		CDV_CHAR id[COIL_CHN];//һ��tab��idһ�£�0Ϊ����
		CDV_CHAR cnt[COIL_CHN];//�Ƿ���Ҫд����critical
		CDV_CHAR cnted[COIL_CHN];//
	}COIL_MAP;//����writecoil�ı��
	
/** @brief  �������� �Ĵ���
  * @param  buf     ����
            len     buf����
            inReg   �Ĵ��������ַ
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
/** @brief  �������� �Ĵ���
  * @param  buf     ����
            len     buf����
            inReg   �Ĵ��������ַ
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
/** @brief  �������� �Ĵ���
  * @param  buf     ����
            len     buf����
            reg   �Ĵ��������ַ
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
/** @brief  �������� �Ĵ���
  * @param  buf     ����
            len     buf����
            reg   �Ĵ��������ַ
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
/** @brief  �������� ��Ȧ
  * @param  buf     ����
            len     buf����
            inCoil   �Ĵ��������ַ
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

/** @brief  �������� ��Ȧ
  * @param  buf     ����
            len     buf����
            inCoil  �Ĵ��������ַ
            addr    coil����ʼ��ַ0~
  * @retval 
  * @note   ��������Ȧ����
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
	sta = (localaddr >> 3);//����Ȧ�ĳ�ʼchar
	sf = localaddr & 0x07;
	
	//�����ֽ�
	for(i = 0; i < numCh; i++) {
		inCoil[sta+i] = (inCoil[sta+i] & ~(0xFF<<sf)) | pInCoil[i]<<(sf);
		if (sf /*&& i + 1 < numCh*/) 
			inCoil[sta+i+1] = (inCoil[sta+i+1] & ~(0xFF>>(8-sf))) | pInCoil[i]>>(8-sf);
	}
	
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  �������� ��Ȧ
  * @param  buf     ����
            len     buf����
            inCoil  �Ĵ��������ַ
            addr    coil����ʼ��ַ0~
  * @retval 
  * @note   ������Ȧ����
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
	sta = (localaddr >> 3);//����Ȧ�ĳ�ʼchar
	sf = localaddr & 0x07;
  endnum = num - (numCh - 1) * 8;//���һ���ֽڵ���Ȧ����
	//�����ֽ�
	for(i = 0; i < numCh; i++) {
		//��λ����
		firstCh = (inCoil[sta+i] & ~(0xFF<<sf)) | pInCoil[i]<<(sf);
		
		if (sf /*&& i + 1 < numCh*/) {
			secondCh = (inCoil[sta+i+1] & ~(0xFF>>(8-sf))) | pInCoil[i]>>(8-sf);
		} else {
			secondCh = inCoil[sta+i+1];
		}
		
		
		if(i + 1 == numCh && endnum) {//���һ���ֽڸ�������
			/*
			���һ
						h              l
			first 0 0 0 0  0 0 0 0
											 |sf |    
							| endnum |     
			�����
			second            first
			h		           l  h              l
			0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0
						            				 |  sf |    
						 	     |  endnum     |     
			*/
		  if(8 - sf >= endnum) {//���ڵ���endnum������˵������firstCh���ӣ�����sta + endnum������
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

/** @brief  �������� ��Ȧ
  * @param  buf     ����
            len     buf����
            coil   �Ĵ��������ַ
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
/** @brief  �������� ��Ȧ
  * @param  buf     ����
            len     buf����
            coil   �Ĵ��������ַ
  * @retval 
  * @note   ��������Ȧ����
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
	sta = (localaddr >> 3);//����Ȧ�ĳ�ʼchar
	sf = localaddr & 0x07;
	
	//�����ֽ�
	for(i = 0; i < numCh; i++) {
		coil[sta+i] = (coil[sta+i] & ~(0xFF<<sf)) | pCoil[i]<<(sf);
		if (sf/*i + 1 < numCh*/)
			coil[sta+i+1] = (coil[sta+i+1] & ~(0xFF>>(8-sf))) | pCoil[i]>>(8-sf);
	}
	
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  �������� ��Ȧ
  * @param  buf     ����
            len     buf����
            coil   �Ĵ��������ַ
  * @retval 
  * @note   ��������Ȧ����
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
	sta = (localaddr >> 3);//����Ȧ�ĳ�ʼchar
	sf = localaddr & 0x07;
  endnum = num - (numCh - 1) * 8;//���һ���ֽڵ���Ȧ����
	
	//�����ֽ�
//	for(i = 0; i < numCh; i++) {
//		coil[sta+i] = (coil[sta+i] & ~(0xFF<<sf)) | pCoil[i]<<(sf);
//		if (sf/*i + 1 < numCh*/)
//			coil[sta+i+1] = (coil[sta+i+1] & ~(0xFF>>(8-sf))) | pCoil[i]>>(8-sf);
//	}
	for(i = 0; i < numCh; i++) {
		//��λ����
		firstCh = (coil[sta+i] & ~(0xFF<<sf)) | pCoil[i]<<(sf);
		
		if (sf /*&& i + 1 < numCh*/) {
			secondCh = (coil[sta+i+1] & ~(0xFF>>(8-sf))) | pCoil[i]>>(8-sf);
		} else {
			secondCh = coil[sta+i+1];
		}
		
		
		if(i + 1 == numCh && endnum) {//���һ���ֽڸ�������
		  if(8 - sf >= endnum) {//���ڵ���endnum������˵������firstCh���ӣ�����sta + endnum������
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

/** @brief  ��Ȧ����
  * @param  buf     Դ
            bufaddr Դ��ʼ��ַ
            coil    Ŀ��
            coiladdrĿ����ʼ��ַ
            tonum   ������Ȧ��
  * @retval RET_STATUS
  * @note   
  */
RET_STATUS CoilToCoil(CDV_INT08U* buf, CDV_INT08U bufaddr, CDV_INT08U* coil, CDV_INT16U coiladdr, CDV_INT16U tonum){
	//CDV_INT08U fc = buf[1];��
	CDV_INT08U buf_sta_ch = bufaddr >> 3;
	CDV_INT08U buf_sf = bufaddr & 0x07;
	CDV_INT08U coil_sta_ch = (coiladdr >> 3);//����Ȧ�ĳ�ʼchar
	CDV_INT08U coil_sf = coiladdr & 0x07;
	CDV_INT16U num = tonum;
	CDV_INT08U numCh = (num + 7) / 8;//��׼���ȣ�sf = 0
	CDV_INT08U i;
	CDV_INT08U firstCh, secondCh;
	CDV_INT08U* pCoil = (CDV_INT08U*)(buf + buf_sta_ch);
	RET_STATUS ret = OPT_FAILURE;
	CDV_INT16U buf_end_num;
	
	ASSERT(buf  && coil);

	if(buf_sf) {//����sf�����⴦���һ���ֽ��е�����
		if(8-buf_sf>num)//�ж������ǲ��Ǻ���
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
		coil_sta_ch = ((coiladdr+(8 - buf_sf)) >> 3);//����Ȧ�ĳ�ʼchar
	  coil_sf = (coiladdr+(8 - buf_sf)) & 0x07;
	}
	/////////
  buf_end_num = num - (numCh - 1) * 8;//���һ���ֽڵ���Ȧ����
	

	for(i = 0; i < numCh; i++) {
		//��λ����
		firstCh = (coil[coil_sta_ch+i] & ~(0xFF<<coil_sf)) | pCoil[i]<<(coil_sf);
		
		if (coil_sf /*&& i + 1 < numCh*/) {
			secondCh = (coil[coil_sta_ch+i+1] & ~(0xFF>>(8-coil_sf))) | pCoil[i]>>(8-coil_sf);
		} else {
			secondCh = coil[coil_sta_ch+i+1];
		}
		
		
		if(i + 1 == numCh && buf_end_num) {//���һ���ֽڸ�������
		  if(8 - coil_sf >= buf_end_num) {//���ڵ���endnum������˵������firstCh���ӣ�����sta + endnum������
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
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  ��Ȧ�Ƚ�
  * @param  �ο�CoilToCoil
  * @retval 0 ��ͬ��1 ����ͬ
  * @note   
  */
int CoilCmp(CDV_INT08U* buf, CDV_INT08U bufaddr, CDV_INT08U* coil, CDV_INT16U coiladdr, CDV_INT16U tonum){
	CDV_INT08U coil_sta_ch = (coiladdr >> 3);//����Ȧ�ĳ�ʼchar
	CDV_INT08U coil_sf = coiladdr & 0x07;
	CDV_INT08U coil_numCh = (tonum - 8 + coil_sf + 7) / 8 + (coil_sf ? 1 :0);//��׼���ȣ�sf = 0
	int ret = 0,i = 0;
	CDV_INT08U* tmp_buf = NULL;
	
	ASSERT(buf  && coil);
	
	NEWCH(tmp_buf, coil_numCh);
	
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
	
	DELETE(tmp_buf);
	return ret;
}

/** @brief  ��ȡ�ӻ��ļĴ���������
  * @param  pBuffer      ��ѯ����ֵ����ĵط�
	*         id           ������
  *         addr         ��ѯ��ʼ��ַ
  *         num          ��ѯ������
            uart         �ӻ����ڴ���
  * @retval RET_STATUS
  * @note   CascadeModbusֻ������һ���߳���
  */

	RET_STATUS CascadeModbus_ReadReg2(CDV_INT08U* pBuffer, CDV_INT16U startaddr, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = g_cascadeBuf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT16U* pReg = (CDV_INT16U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadRegisterCmd(id, addr, num, &cmdBuf, &cmdLen);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToReg2(recvBuf, recvLen, pReg, startaddr);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  ��ȡ�ӻ��ļĴ���������
  * @param  pBuffer      ��ѯ����ֵ����ĵط�
	*         id           ������
  *         addr         ��ѯ��ʼ��ַ
  *         num          ��ѯ������
            uart         �ӻ����ڴ���
  * @retval RET_STATUS
  * @note   CascadeModbusֻ������һ���߳���
  */

	RET_STATUS CascadeModbus_ReadReg(CDV_INT08U* pBuffer, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = g_cascadeBuf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT16U* pReg = (CDV_INT16U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadRegisterCmd(id, addr, num, &cmdBuf, &cmdLen);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToReg(recvBuf, recvLen, pReg);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  ��ȡ�ӻ���ֻ���Ĵ���������
  * @param  pBuffer      ��ѯ����ֵ����ĵط�
	*         id           ������
  *         addr         ��ѯ��ʼ��ַ
  *         num          ��ѯ������
            uart         �ӻ����ڴ���
  * @retval RET_STATUS
  * @note   CascadeModbusֻ������һ���߳���
  */

	RET_STATUS CascadeModbus_ReadInReg2(CDV_INT08U* pBuffer, CDV_INT16U startaddr, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = g_cascadeBuf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT16U* pInReg = (CDV_INT16U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInRegisterCmd(id, addr, num, &cmdBuf, &cmdLen);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInReg2(recvBuf, recvLen, pInReg, startaddr);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  ��ȡ�ӻ���ֻ���Ĵ���������
  * @param  pBuffer      ��ѯ����ֵ����ĵط�
	*         id           ������
  *         addr         ��ѯ��ʼ��ַ
  *         num          ��ѯ������
            uart         �ӻ����ڴ���
  * @retval RET_STATUS
  * @note   CascadeModbusֻ������һ���߳���
  */

	RET_STATUS CascadeModbus_ReadInReg(CDV_INT08U* pBuffer, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = g_cascadeBuf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT16U* pInReg = (CDV_INT16U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInRegisterCmd(id, addr, num, &cmdBuf, &cmdLen);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInReg(recvBuf, recvLen, pInReg);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  ��ȡ�ӻ�����Ȧ������
  * @param  pBuffer      ��ѯ����ֵ����ĵط�
	*         id           ������
  *         addr         ��ѯ��ʼ��ַ
  *         num          ��ѯ������
            uart         �ӻ����ڴ���
  * @retval RET_STATUS
  * @note   CascadeModbusֻ������һ���߳���
  */

	RET_STATUS CascadeModbus_ReadCoil2(CDV_INT08U* pBuffer, CDV_INT16U startaddr, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = g_cascadeBuf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT08U* pCoil = (CDV_INT08U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadCoilCmd(id, addr, num, &cmdBuf, &cmdLen);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToCoil3(recvBuf, recvLen, pCoil, startaddr, num);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  ��ȡ�ӻ�����Ȧ������
  * @param  pBuffer      ��ѯ����ֵ����ĵط�
	*         id           ������
  *         addr         ��ѯ��ʼ��ַ
  *         num          ��ѯ������
            uart         �ӻ����ڴ���
  * @retval RET_STATUS
  * @note   CascadeModbusֻ������һ���߳���
  */

	RET_STATUS CascadeModbus_ReadCoil(CDV_INT08U* pBuffer, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = g_cascadeBuf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT08U* pCoil = (CDV_INT08U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadCoilCmd(id, addr, num, &cmdBuf, &cmdLen);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToCoil(recvBuf, recvLen, pCoil);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  ��ȡ�ӻ���ֻ����Ȧ������
  * @param  pBuffer      ��ѯ����ֵ����ĵط�
	          startaddr    �������ʼ��ַ
	*         id           ������
  *         addr         ��ѯ��ʼ��ַ
  *         num          ��ѯ������
            uart         �ӻ����ڴ���
  * @retval RET_STATUS
  * @note   CascadeModbusֻ������һ���߳���
  */

	RET_STATUS CascadeModbus_ReadInCoil2(CDV_INT08U* pBuffer, CDV_INT16U startaddr, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = g_cascadeBuf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT08U* pInCoil = (CDV_INT08U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInCoilCmd(id, addr, num, &cmdBuf, &cmdLen);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInCoil3(recvBuf, recvLen, pInCoil, startaddr, num);
		}
		DELETE(cmdBuf);
		return ret;
	}
/** @brief  ��ȡ�ӻ���ֻ����Ȧ������
  * @param  pBuffer      ��ѯ����ֵ����ĵط�
	*         id           ������
  *         addr         ��ѯ��ʼ��ַ
  *         num          ��ѯ������
            uart         �ӻ����ڴ���
  * @retval RET_STATUS
  * @note   CascadeModbusֻ������һ���߳���
  */

	RET_STATUS CascadeModbus_ReadInCoil(CDV_INT08U* pBuffer, const CDV_INT08U id, const CDV_INT16U addr, const CDV_INT16U num, const CDV_INT08U uart) {
		CDV_INT08U* recvBuf = g_cascadeBuf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvLen = 0;
		CDV_INT08U* pInCoil = (CDV_INT08U*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInCoilCmd(id, addr, num, &cmdBuf, &cmdLen);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, uart);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInCoil(recvBuf, recvLen, pInCoil);
		}
		DELETE(cmdBuf);
		return ret;
	}
	//�ӻ�����
		CDV_INT16U gp_val[6] = {50, 100, 150, 200, 250, 300};
		CDV_INT08U gp_coil[2] = {0xf0, 0x00};
	void CascadeModbus_AllUpdate(void) {
		CDV_INT08U id;
		CDV_INT08U* cmdBuf = NULL;
	  CDV_INT08U  cmdLen = 0;
		CDV_INT08U* recvBuf = g_cascadeBuf;//UserMemPtr(CCM_CASCADE_MODBUS);
		CDV_INT08U recvLen = 0;
		CDV_INT16U p_val[6];
		CDV_INT08U p_coil[2];
		ASSERT(recvBuf);
		MemCpy(p_coil,gp_coil,2);
		MemCpy(p_val,gp_val,12);
		
		WriteMultiRegisterCmd(2, 0x10, 6, (CDV_INT08U*)p_val, &cmdBuf, &cmdLen);
		UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, CASCADE_USART);
		delay_ms(10);
		WriteMultiCoilCmd(3, 0, 10, (CDV_INT08U*)p_coil, &cmdBuf, &cmdLen);
		UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, CASCADE_BUF_LEN, &recvLen, CASCADE_USART);
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
		u8 type;	//��ע���ͣ�I:0 0:1 DA:2 AD:3 ����
		u8 host;//������
		u16 typeno;//������ʼDA��
		u16 localaddr;//������ӳ���ַ
		u16 remoteaddr;//Զ�̵�ַ��һ����0
		u16 remotenum;//����������DA������
		u16 len;//���ȣ�����һ��DA��ռ��Դ����ַ���ĳ��ȣ���ռ���ٸ���Ȧ/�Ĵ���
		//u8 spec;//��ע����int?short?
		u8 dowrite[3];
		u8* wbuf;//��д��buf
	} ;
	
	struct CASCADE_MAP* CascadeMap = NULL;
	CDV_INT08U CascadeMapLen = 0;

	
/** @brief  ӳ���ʼ��
  * @param  void
  * @retval RET_STATUS
  * @note   �ο�˵���ĵ�
  */
	
	RET_STATUS CascadeModbus_MapInit(void) {
		#define MAP_LINE_LEN 6	
		#define REG_SHIFT  CDV_VAR_NUM * 2
		#define INREG_SHIFT  18                       //����ADDA
		#define COIL_SHIFT  CDV_O_NUM + CDV_EXO_NUM*CDV_FPGA_NUM
		#define INCOIL_SHIFT  CDV_I_NUM + CDV_EXI_NUM*CDV_FPGA_NUM
    RET_STATUS ret;
		//struct CASCADE_MAP* map = CascadeMap;
		/////////////////////////////
		CDV_INT32U size = 0;//�ű���С������ת
		CDV_INT32U shift = 0;//ƫ����ʼ��ַ���ֽ���
		CDV_INT32U lineMapAddr = SCRIP_MAP;//������ӳ�����ʼ��ַ
	  CDV_INT32U lineMapLen = 0;//������ӳ��ĳ���
		CDV_INT08U every_map[MAP_LINE_LEN];
		CDV_INT08U i;
		CDV_INT16U regshift = REG_SHIFT;
		CDV_INT16U inregshift = INREG_SHIFT;
		CDV_INT16U coilshift = COIL_SHIFT;
		CDV_INT16U incoilshift = INCOIL_SHIFT;
		
		if(!g_line.init) 
			return OPT_FAILURE;
		
		SPI_Flash_Read((CDV_INT08U*)&size, lineMapAddr + shift, 4);//��ȡ������ӳ��ű���С
		lineMapLen = size;
		shift += 4;
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, lineMapAddr + shift, 1);//��ȡ������ӳ�����
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
		  SPI_Flash_Read((CDV_INT08U*)&every_map, lineMapAddr + shift, MAP_LINE_LEN);//��ȡһ��ӳ��
		  CascadeMap[i].type = every_map[0];
			CascadeMap[i].host = every_map[1];
			CascadeMap[i].typeno = *(CDV_INT16U*)(every_map + 2);
			CascadeMap[i].remotenum = *(CDV_INT16U*)(every_map + 4);/*ͨ������*/
			CascadeMap[i].remoteaddr = 0;
			shift += MAP_LINE_LEN;
			switch (CascadeMap[i].type) {
				case 0://I
			    CascadeMap[i].localaddr = incoilshift;/*��������ӳ���ַ*/
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
		
		NEWCH(g_cascadeBuf, CASCADE_BUF_LEN);

#if USE_OVERLAP
	  NEWCH(g_coilCascade, sizeof(MODBUS_Coil));
	  NEWCH(g_regCascade, sizeof(MODBUS_Register));
#endif
		/////////////////////////////���ӻ��Ƿ����
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
  * @brief  O�첽ˢ��
  *  
  * @param  coilVal ��Ȧ��ֵ�������Ѿ���modbus���������
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   					
  */

	RET_STATUS CascadeOverlapOWrite(CDV_INT08U host, CDV_INT16U remoteaddr, CDV_INT32U num, CDV_INT08U* coilVal) { 
		RET_STATUS ret =OPT_SUCCESS;
		CDV_INT08U* cmdBuf = NULL;
		CDV_INT08U  cmdLen = 0;
		CDV_INT08U recvBuf[20] = {0};
		CDV_INT08U  recvLen = 0;
		
		WriteMultiCoilCmd(host, remoteaddr, num, coilVal, &cmdBuf, &cmdLen);
    
		ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, 20, &recvLen, CASCADE_USART);
		
		if(OPT_SUCCESS == ret && host == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = OPT_SUCCESS;
		} else {
			ret = OPT_FAILURE;
		}
	  
		DELETE(cmdBuf);
		
		return ret;
	}
	
/** @brief  ��ȡ�ӻ��ļĴ���������
  * @param  pBuffer      ��ѯ����ֵ����ĵط�
	*         id           ������
  *         addr         ��ѯ��ʼ��ַ
  *         num          ��ѯ������
            uart         �ӻ����ڴ���
  * @retval RET_STATUS
  * @note   CascadeModbusֻ������һ���߳���
	          ϵͳ����ʱ�ȼ�����޹��أ��޹���ϵͳ��������
  */

	RET_STATUS CascadeModbus_Map(void) {
		CDV_INT08U i;
		RET_STATUS ret = OPT_SUCCESS;
		struct CASCADE_MAP* map = CascadeMap;
		//ASSERT(map);
		//ASSERT(g_coilCascade);//�˺���������ִ�У�����Ҫ
		
		if(!g_line.init || !map || !g_coilCascade) 
			return ret;
		
		for( i = 0; i < CascadeMapLen; i++) {
			switch (map[i].type) {
				case 0://I
					ret = CascadeModbus_ReadInCoil2((CDV_INT08U*)g_modbusInCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				case 1://O
#if USE_OVERLAP
				  ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_coilCascade->coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(CoilCmp((CDV_INT08U*)g_coilCascade->coilCh, map[i].localaddr, (CDV_INT08U*)g_modbusCoil.coilCh, map[i].localaddr, map[i].remotenum)) {
						CDV_INT08U *tmp_coil_val = NULL;
						NEWCH(tmp_coil_val, map[i].remotenum / 8 + 3);
						CoilToCoil(g_modbusCoil.coilCh, map[i].localaddr, tmp_coil_val, 0, map[i].remotenum);
						ret = CascadeOverlapOWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_coil_val);
						DELETE(tmp_coil_val);
					}
#else
					ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_modbusCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 2://DA
					ret = CascadeModbus_ReadReg2((CDV_INT08U*)g_modbusReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
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
	
/** @brief  ����map���ӻ��Ƿ��й���
  * @param  void
  * @retval RET_STATUS
  * @note   �޹��أ�ϵͳ��������
  */

	RET_STATUS CascadeModbus_MapCheck(void) {
		CDV_INT08U i;
		RET_STATUS ret = OPT_FAILURE;
		struct CASCADE_MAP* map = CascadeMap;
		
		if(!g_line.init || !map || !g_coilCascade) 
			return OPT_FAILURE;
		
		for( i = 0; i < CascadeMapLen; i++) {
			switch (map[i].type) {
				case 0://I
					ret = CascadeModbus_ReadInCoil2((CDV_INT08U*)g_modbusInCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				case 1://O
#if USE_OVERLAP
				  ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_coilCascade->coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(CoilCmp((CDV_INT08U*)g_coilCascade->coilCh, map[i].localaddr, (CDV_INT08U*)g_modbusCoil.coilCh, map[i].localaddr, map[i].remotenum)) {
						CDV_INT08U *tmp_coil_val = NULL;
						NEWCH(tmp_coil_val, map[i].remotenum / 8 + 3);
						CoilToCoil(g_modbusCoil.coilCh, map[i].localaddr, tmp_coil_val, 0, map[i].remotenum);
						ret = CascadeOverlapOWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_coil_val);
						DELETE(tmp_coil_val);
					}
#else
					ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_modbusCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 2://DA
					ret = CascadeModbus_ReadReg2((CDV_INT08U*)g_modbusReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
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
	
/** @brief  �õ�һ��������Դ��localaddr
  * @param  type          ����
	*         hosttypeno     host���ڵ���ţ�����2�Ż���da1
  *         host          ������
  *         retno         ����no
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
	
/** @brief  �õ�һ��������Դ��localaddr
  * @param  type          ���� I:0/O:1/DA:2/AD:3 
	*         typeno        ��ţ������������������
	*         localaddr     ������ӳ���ַ
	*         remoteaddr    �ӻ������ڵ�ַ
  *         host          �ӻ���������
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
				shift += map[i].remotenum * map[i].len;/*ƫ��������*/
				continue;
			}
			
			if (localaddr)
			  *localaddr = map[i].localaddr + (typeno - shift);/*�õ�������ӳ���ַ*/
			
			if (remoteaddr)
			  *remoteaddr = map[i].remoteaddr + (typeno - shift);/*�õ��ӻ������*/
			
			if (host)
			  *host = map[i].host;/*�õ��ӻ���������*/
			
			return OPT_SUCCESS;
		}
		
		return OPT_FAILURE;
	}
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	/////////////Cascade Usart Function////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
/** @brief  �Ǳ��������ת��
  * @param  ppBuffer      ��ѯ������Դ��
	*         pLen          ��Դ����
  *         pCmdBuf       ��������
  *         NumByteOfCmd  ���������
  * @retval RET_STATUS
  * @note   ppBuffer��ʹ��ָ��
  */

	RET_STATUS CascadeUsartTranferCmd(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT08U* pCmdBuf, CDV_INT16U NumByteOfCmd) {
		CDV_INT08U* tmp = NULL;
		CDV_INT16U crc;		
		
		ASSERT(NULL != pCmdBuf);
		ASSERT(0 != NumByteOfCmd);
		ASSERT(NULL != pLen);

		crc = getCRC16(pCmdBuf, NumByteOfCmd);

		
		NEW08U(tmp, NumByteOfCmd + 2);//�����crc
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
  * @brief  I�ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��
            OPT_SUCCESS����ִ�����
            OPT_FAILURE����δִ����ɣ�����ȴ�δ�ȵ�
  *
  * @note   ����id 01 64 01 ��Դ�ţ�4B��	00�ȴ�	ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	�ַ���
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
		//host no��ӳ��ת��
		ret = CascadeModbus_MapFind(0, *(CDV_INT32U*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0xFF:/*��ѯ*/
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
  * @brief  O�ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   ����id 01 64 02 ��Դ�ţ�00-FF��	00���	ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	�ַ���
		                                FE	�������Ÿ�λ	�������ŵ�λ							
            
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
		//host no��ӳ��ת��
		ret = CascadeModbus_MapFind(1, no, host, &localaddr, &remoteaddr);
		
		if(OPT_FAILURE == ret)
			return ret;
			
					
//		if(no!=0 && no != 5 && (0 == READ_COIL_ADDR(localaddr -1)))
//			no = no;
		////
		switch(opt) {
			case 0x00:/*���*/			
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

					ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, 20, &recvLen, CASCADE_USART);
												
					if(OPT_SUCCESS == ret && host == recvBuf[0] && 0x80 > recvBuf[1]) {
					  ret = OPT_SUCCESS;
					} else {
						ret = OPT_FAILURE;
					}
				
					DELETE(cmdBuf);
#endif
				}
				break;
			case 0x04:/*��ѯ*/
			case 0xFF:/*��ѯ*/
#if USE_OVERLAP
			  ret = OPT_SUCCESS;
#else
				ret = CascadeModbus_ReadCoil2((CDV_INT08U*)g_modbusCoil.coilCh, localaddr, host, remoteaddr, 1, CASCADE_USART);
#endif
				if(OPT_SUCCESS == ret) {
//					bit = ORead(localaddr); // ���o����addr0��ʼ������������
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
  * @brief  ad�ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   ����id 01 64 0C ��Դ�ţ�00-FF��	00���	ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	�ַ���
		                                FE	�������Ÿ�λ	�������ŵ�λ							
            
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
		//host no��ӳ��ת��
		ret = CascadeModbus_MapFind(2, *(CDV_INT32U*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0x02:/*���*/
				{
					CDV_INT08U* cmdBuf = NULL;
					CDV_INT08U  cmdLen = 0;
					CDV_INT16U  reg = 0;
					CDV_INT08U  recvBuf[30] = {0};
					CDV_INT08U  recvLen = 0;
					
					reg = ArithmeticEx((char*)buf + 10, len - 10, arg) ;
					
					WriteMultiRegisterCmd(host, remoteaddr, 1, (CDV_INT08U*)&reg, &cmdBuf, &cmdLen);
					ret = UniSerialSendCRC(cmdBuf, cmdLen, recvBuf, 30, &recvLen, CASCADE_USART);
				  
					if(OPT_SUCCESS == ret && host == recvBuf[0] && 0x80 > recvBuf[1]) {
						ret = OPT_SUCCESS;
					} else {
						ret = OPT_FAILURE;
					}
					
					DELETE(cmdBuf);
				}
				break;
			case 0xFF:/*��ѯ*/
				ret = CascadeModbus_ReadReg2((CDV_INT08U*)g_modbusReg.reg, localaddr, host, remoteaddr, 1, CASCADE_USART);
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
  * @brief  ad�ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   ����id 01 64 0B ��Դ�ţ�00-FF��	00���	ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	�ַ���
		                                FE	�������Ÿ�λ	�������ŵ�λ							
            
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
		//host no��ӳ��ת��
		ret = CascadeModbus_MapFind(3, *(CDV_INT32U*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0x04:/*��ѯ*/
			case 0xFF:/*��ѯ*/
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
/** @brief  CascadeTranferCmd֮��Ľ���
  * @param  pBuf   ����
	*         len    �����
  * @retval RET_STATUS
  * @note   0x01	0x10	�������ݳ���	����ID	ԭָ��
  */
  RET_STATUS TFUsartCmdParse (CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg) {
		RET_STATUS ret = OPT_SUCCESS;
		
	u32 gstartTime ,gendTime , gtime;
		gstartTime = GetCdvTimeTick();
		switch(buf[6]) {
			case 0x00:/*��������*/
				
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
		gendTime = GetCdvTimeTick();
		gtime = CalcCount(gendTime , gstartTime);
		return ret;
	}
