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
	
	#define CASCADE_DATA_LEN 512
	/*
	������incoil + inreg + coil + reg
	д��Ҫˢ�´�cascade_coil��ַ��ʼ��cascade_coil_chlen + cascade_reg_chlen�ֽ�
	����Ҫ���ʹ�cascade_incoil��ַ��ʼ��cascade_incoil_chlen + cascade_inreg_chlen�ֽ�
	*/
  u8 cascade_data[CASCADE_DATA_LEN] = {0}; 
	u8* cascade_incoil = NULL;
	u16 cascade_incoil_chlen = 0;
	u8* cascade_coil = NULL;
	u16 cascade_coil_chlen = 0;
	u8* cascade_inreg = NULL;
	u16 cascade_inreg_chlen = 0;
	u8* cascade_reg = NULL;
	u16 cascade_reg_chlen = 0;
	//
  //������Դ�����汾��
	u8 version[] = {0/*id��*/, 3/*�����汾*/, 2/*Ӳ����汾*/, 3/*Ӳ�������汾*/, 13/*����С�汾*/};

	
#if USE_CASCADE_STATIC == 1u
	#define CASCADE_BUF_MAX_LEN 256
	u8* cascade_cmd_buf = NULL; // ��������ľ�̬buf������̬����
	u8* cascade_tmp_buf = NULL;
#endif
	
  //������������
	#define	CASCADE_EN 			CASCADE_CS_EN;FPGA1_CS_DS;FPGA2_CS_DS;FLASH_CS_DS;
	//�ر���������
	#define	CASCADE_DS 			CASCADE_CS_DS;
	//�������ڷ�����
	#define CASCADE_ReadWriteByte(a)				SPIx_ReadWriteByte(SPI2,(a))

	//�ӻ���Դ��
	u8 *slaveTable = NULL;
	u8 *slaveTable2 = NULL;
	u8 slaveTableLen = 0;
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
	/////////////Cascade Modbus Function///////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
	struct
	{
		u16 reg[REG_N];
		u16 addr[REG_N];
		u8 id[REG_N];
		u8 cnt[REG_N];
		u8 cnted[REG_N];
	}REG_MAP;//����writereg�ı��
	
	struct
	{
		u8 coil[COIL_CHN];
		u8 addr[COIL_CHN];
		u8 id[COIL_CHN];//һ��tab��idһ�£�0Ϊ����
		u8 cnt[COIL_CHN];//�Ƿ���Ҫд����critical
		u8 cnted[COIL_CHN];//
	}COIL_MAP;//����writecoil�ı��
	
/** @brief  �������� �Ĵ���
  * @param  buf     ����
            len     buf����
            inReg   �Ĵ��������ַ
  * @retval 
  * @note
  */
RET_STATUS BufToInReg2(u8* buf, u8 len, u16* inReg, u16 addr){
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u16* pInReg = (u16*)(buf + 3);
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
RET_STATUS BufToInReg(u8* buf, u8 len, u16* inReg){
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u16* pInReg = (u16*)(buf + 3);
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
RET_STATUS BufToReg2(u8* buf, u8 len, u16* reg, u16 addr){
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u16* pReg = (u16*)(buf + 3);
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
RET_STATUS BufToReg(u8* buf, u8 len, u16* reg){
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u16* pReg = (u16*)(buf + 3);
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
RET_STATUS BufToInCoil(u8* buf, u8 len, u8* inCoil){
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u8* pInCoil = (u8*)(buf + 3);
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
RET_STATUS BufToInCoil2(u8* buf, u8 len, u8* inCoil, u16 addr) {
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u8* pInCoil = (u8*)(buf + 3);
	u16 remoteaddr = 0;
	u16 localaddr = addr;
	RET_STATUS ret = OPT_FAILURE;
	u16	sta, sf;
	
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
RET_STATUS BufToInCoil3(u8* buf, u8 len, u8* inCoil, u16 addr, u16 num) {
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u8 firstCh, secondCh;
	u8* pInCoil = (u8*)(buf + 3);
	u16 remoteaddr = 0;
	u16 localaddr = addr;
	RET_STATUS ret = OPT_FAILURE;
	u16	sta, sf, endnum;
	
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
RET_STATUS BufToCoil(u8* buf, u8 len, u8* coil){
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u8* pCoil = (u8*)(buf + 3);
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
RET_STATUS BufToCoil2(u8* buf, u8 len, u8* coil, u16 addr){
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u8* pCoil = (u8*)(buf + 3);
	u16 remoteaddr = 0;
	u16 localaddr = addr;
	RET_STATUS ret = OPT_FAILURE;
	u16	sta, sf;
	
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
RET_STATUS BufToCoil3(u8* buf, u8 len, u8* coil, u16 addr, u16 num){
	u8 fc = buf[1];
	u8 numCh = buf[2];
	u8 i;
	u8 firstCh, secondCh;
	u8* pCoil = (u8*)(buf + 3);
	u16 remoteaddr = 0;
	u16 localaddr = addr;
	RET_STATUS ret = OPT_FAILURE;
	u16	sta, sf, endnum;
	
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
RET_STATUS CoilToCoil(u8* buf, u8 bufaddr, u8* coil, u16 coiladdr, u16 tonum){
	//u8 fc = buf[1];��
	u8 buf_sta_ch = bufaddr >> 3;
	u8 buf_sf = bufaddr & 0x07;
	u8 coil_sta_ch = (coiladdr >> 3);//����Ȧ�ĳ�ʼchar
	u8 coil_sf = coiladdr & 0x07;
	u16 num = tonum;
	u8 numCh = (num + 7) / 8;//��׼���ȣ�sf = 0
	u8 i;
	u8 firstCh, secondCh;
	u8* pCoil = (u8*)(buf + buf_sta_ch);
	RET_STATUS ret = OPT_FAILURE;
	u16 buf_end_num;
	
	ASSERT(buf  && coil);

	__disable_irq();
	
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
		pCoil = (u8*)(buf + buf_sta_ch);
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
	
	__enable_irq();
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  ��Ȧ�Ƚ�
  * @param  �ο�CoilToCoil
  * @retval 0 ��ͬ��1 ����ͬ
  * @note   
  */
int CoilCmp(u8* buf, u16 bufaddr, u8* coil, u16 coiladdr, u16 tonum){
	u8 coil_sta_ch = (coiladdr >> 3);//����Ȧ�ĳ�ʼchar
	u8 coil_sf = coiladdr & 0x07;
	u8 coil_numCh = (tonum - 8 + coil_sf + 7) / 8 + (coil_sf ? 1 :0);//��׼���ȣ�sf = 0
	int ret = 0,i = 0;
	
#if USE_CASCADE_STATIC == 1u
	u8 *tmp_buf = cascade_tmp_buf;
#else
	u8* tmp_buf = NULL;
#endif
	
	ASSERT(buf  && coil);
	
#if USE_CASCADE_STATIC == 0u
	NEWCH(tmp_buf, coil_numCh);
#endif
	
	MemCpy(tmp_buf, coil + coil_sta_ch, coil_numCh);

	CoilToCoil(buf, bufaddr, tmp_buf, coil_sf, tonum);
	
	for(i = 0; i< coil_numCh; i++) {
		if(/*(u8*)*/tmp_buf[i] !=/* (u8*)*/(coil + coil_sta_ch)[i]) {
			ret = 1;
			break;
		}
	}
//	if(0 == strncmp((sc8*)tmp_buf, (sc8*)(coil + coil_sta_ch), coil_numCh)) {
//		ret = 0;
//	}

#if USE_CASCADE_STATIC == 0u
	DELETE(tmp_buf);
#endif
	return ret;
}

/** @brief   �Ĵ����Ƚ�
  * @param  �ο�CoilToCoil
  * @retval 0 ��ͬ��1 ����ͬ
  * @note   
  */
int RegCmp(u16* buf, u16 bufaddr, u16* reg, u16 regaddr, u16 tonum){
	int ret = 0,i = 0;

	for(i = 0; i< tonum; i++) {
		if(buf[i+bufaddr] != reg[i+regaddr]) {
			ret = 1;
			break;
		}
	}
	
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

	RET_STATUS CascadeModbus_ReadReg2(u8* pBuffer, u16 startaddr, const u8 id, const u16 addr, const u16 num, const u8 uart) {
		u8* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  u8  cmdLen = 0;
		u16 recvLen = 0;
		u16* pReg = (u16*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadRegisterCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,opt);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToReg2(recvBuf, recvLen, pReg, startaddr);
		}
		#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
		#endif
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

	RET_STATUS CascadeModbus_ReadReg(u8* pBuffer, const u8 id, const u16 addr, const u16 num, const u8 uart) {
		u8* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  u8* cmdBuf = NULL;
	  u8  cmdLen = 0;
		u16 recvLen = 0;
		u16* pReg = (u16*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadRegisterCmd(id, addr, num, &cmdBuf, &cmdLen,BUF_NEW);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,BUF_NEW);
		
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

	RET_STATUS CascadeModbus_ReadInReg2(u8* pBuffer, u16 startaddr, const u8 id, const u16 addr, const u16 num, const u8 uart) {
		u8* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  u8  cmdLen = 0;
		u16 recvLen = 0;
		u16* pInReg = (u16*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInRegisterCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,opt);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInReg2(recvBuf, recvLen, pInReg, startaddr);
		}
		#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
		#endif
		return ret;
	}
	
	int CascadeModbus_ReadInReg3(u8* pBuffer, u16 startaddr, const u8 id, const u16 addr, const u16 num, const u8 uart) {
		static u8* recvBuf = NULL;//cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  static u8  cmdLen = 0;
		static u16 recvLen = 0;
		u16* pInReg = (u16*)pBuffer;
		static int ret;
		static int stat = 0;
		
		switch(stat) {
			case 0:
				//ASSERT(recvBuf&&pBuffer);
				ReadInRegisterCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
				stat = 1;
				break;
			case 1:
				ret = UniSerialSendCRC2(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,opt);
				if (0 != ret)
					stat = 2;
				break;
			case 2:
				if(1 == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
					BufToInReg2(recvBuf, recvLen, pInReg, startaddr);
				}
				#if USE_CASCADE_STATIC == 0u
				DELETE(cmdBuf);
				#endif
				stat = 0;
				return ret;
				break;
			default:
				stat = 0;
				break;
		}
		return 0;
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

	RET_STATUS CascadeModbus_ReadInReg(u8* pBuffer, const u8 id, const u16 addr, const u16 num, const u8 uart) {
		u8* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  u8* cmdBuf = NULL;
	  u8  cmdLen = 0;
		u16 recvLen = 0;
		u16* pInReg = (u16*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInRegisterCmd(id, addr, num, &cmdBuf, &cmdLen,BUF_NEW);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,BUF_NEW);
		
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

	RET_STATUS CascadeModbus_ReadCoil2(u8* pBuffer, u16 startaddr, const u8 id, const u16 addr, const u16 num, const u8 uart) {
		u8* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  u8  cmdLen = 0;
		u16 recvLen = 0;
		u8* pCoil = (u8*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadCoilCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,opt);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToCoil3(recvBuf, recvLen, pCoil, startaddr, num);
		}
		#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
		#endif
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

	RET_STATUS CascadeModbus_ReadCoil(u8* pBuffer, const u8 id, const u16 addr, const u16 num, const u8 uart) {
		u8* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  u8* cmdBuf = NULL;
	  u8  cmdLen = 0;
		u16 recvLen = 0;
		u8* pCoil = (u8*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadCoilCmd(id, addr, num, &cmdBuf, &cmdLen,BUF_NEW);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,BUF_NEW);
		
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

	RET_STATUS CascadeModbus_ReadInCoil2(u8* pBuffer, u16 startaddr, const u8 id, const u16 addr, const u16 num, const u8 uart) {
		u8* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  u8  cmdLen = 0;
		u16 recvLen = 0;
		u8* pInCoil = (u8*)pBuffer;
		RET_STATUS ret;
		
		ASSERT(recvBuf&&pBuffer);
		ReadInCoilCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
		
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,opt);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInCoil3(recvBuf, recvLen, pInCoil, startaddr, num);
		}
#if USE_CASCADE_STATIC == 0u
		DELETE(cmdBuf);
#endif
		return ret;
	}
	
	int CascadeModbus_ReadInCoil3(u8* pBuffer, u16 startaddr, const u8 id, const u16 addr, const u16 num, const u8 uart) {
	
		static u8* recvBuf = NULL;//cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
	  static u8  cmdLen = 0;
		static u16 recvLen = 0;
		u8* pInCoil = (u8*)pBuffer;
		static int ret;
		static int stat =0;	
		switch(stat){
			case 0:
						
//				ASSERT(recvBuf&&pBuffer);
				ReadInCoilCmd(id, addr, num, &cmdBuf, &cmdLen,opt);
			  stat = 1;
				break;
			case 1:
				ret = UniSerialSendCRC2(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,opt);
			
			  if(ret != 0)
					stat = 2;
				break;
			case 2:
				if(1 == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
					BufToInCoil3(recvBuf, recvLen, pInCoil, startaddr, num);
				}
		#if USE_CASCADE_STATIC == 0u
				DELETE(cmdBuf);
		#endif
				stat = 0;
				return ret;
				break;
			default:
				stat = 0;
				break;
		}
		return 0;
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

	RET_STATUS CascadeModbus_ReadInCoil(u8* pBuffer, const u8 id, const u16 addr, const u16 num, const u8 uart) {
		u8* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
	  u8* cmdBuf = NULL;
	  u8  cmdLen = 0;
		u16 recvLen = 0;
		u8* pInCoil = (u8*)pBuffer;
		RET_STATUS ret;
		ASSERT(recvBuf&&pBuffer);
		ReadInCoilCmd(id, addr, num, &cmdBuf, &cmdLen,BUF_NEW);
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, uart,BUF_NEW);
		
		if(OPT_SUCCESS == ret && id == recvBuf[0] && 0x80 > recvBuf[1]) {
			ret = BufToInCoil(recvBuf, recvLen, pInCoil);
		}
		DELETE(cmdBuf);
		return ret;
	}
	//�ӻ�����
		u16 gp_val[6] = {50, 100, 150, 200, 250, 300};
		u8 gp_coil[2] = {0xf0, 0x00};
	void CascadeModbus_AllUpdate(void) {
		u8 id;
		u8* cmdBuf = NULL;
	  u8  cmdLen = 0;
		u8* recvBuf = cascade_recv_buf;//UserMemPtr(CCM_CASCADE_MODBUS);
		u16 recvLen = 0;
		u16 p_val[6];
		u8 p_coil[2];
		ASSERT(recvBuf);
		MemCpy(p_coil,gp_coil,2);
		MemCpy(p_val,gp_val,12);
		
		WriteMultiRegisterCmd(2, 0x10, 6, (u8*)p_val, &cmdBuf, &cmdLen,BUF_NEW);
		UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, CASCADE_USART,BUF_NEW);
		////delay_ms(10);
		WriteMultiCoilCmd(3, 0, 10, (u8*)p_coil, &cmdBuf, &cmdLen,BUF_NEW);
		UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, CASCADE_USART,BUF_NEW);
		////delay_ms(10);
		DELETE(cmdBuf);
		
		id = 2;
		CascadeModbus_ReadReg((u8*)&g_modbusReg.reg[600], id, 0x10, 6, CASCADE_USART);
		//delay_ms(10);
		CascadeModbus_ReadInReg((u8*)&g_modbusInReg.reg[20], id, 0, 6, CASCADE_USART);
		//delay_ms(10);
		id = 3;
		CascadeModbus_ReadCoil((u8*)&g_modbusCoil.coilCh[20], id, 0, 10, CASCADE_USART);
		//delay_ms(10);
		CascadeModbus_ReadInCoil((u8*)&g_modbusInCoil.coilCh[20], id, 0,10, CASCADE_USART);
		//delay_ms(10);
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
	u8 CascadeMapLen = 0;

	
///** @brief  ӳ���ʼ��
//  * @param  void
//  * @retval RET_STATUS
//  * @note   �ο�˵���ĵ�
//  */
//	
//	RET_STATUS CascadeModbus_MapInit(void) {
//		#define MAP_LINE_LEN 6	
//		#define REG_SHIFT  0
//		//CDV_VAR_NUM * 2
//		#define INREG_SHIFT  18                       //����ADDA
//		#define COIL_SHIFT  0
//		//CDV_O_NUM + CDV_EXO_NUM*CDV_FPGA_NUM
//		#define INCOIL_SHIFT  0
//		//CDV_I_NUM + CDV_EXI_NUM*CDV_FPGA_NUM
//    RET_STATUS ret;
//		//struct CASCADE_MAP* map = CascadeMap;
//		/////////////////////////////
//		u32 size = 0;//�ű���С������ת
//		u32 shift = 0;//ƫ����ʼ��ַ���ֽ���
//		u32 lineMapAddr = 0;//SCRIP_MAP;//������ӳ�����ʼ��ַ
//	  u32 lineMapLen = 0;//������ӳ��ĳ���
//		u8 every_map[MAP_LINE_LEN];
//		u8 i;
//		u16 regshift = REG_SHIFT;
//		u16 inregshift = INREG_SHIFT;
//		u16 coilshift = COIL_SHIFT;
//		u16 incoilshift = INCOIL_SHIFT;
//		
//		//if(!g_line.init) 
//		//	return OPT_FAILURE;
//		
//		//SPI_Flash_Read((u8*)&size, lineMapAddr + shift, 4);//��ȡ������ӳ��ű���С
//		lineMapLen = size;
//		shift += 4;
//		size = 0;
//		//SPI_Flash_Read((u8*)&size, lineMapAddr + shift, 1);//��ȡ������ӳ�����
//		CascadeMapLen = size;
//		NEWCH(CascadeMap, CascadeMapLen * sizeof(struct CASCADE_MAP));
//		shift += 1;
//		size = 0;
//		
//		if (lineMapLen < CascadeMapLen * MAP_LINE_LEN + 1)
//		{
//			CascadeMapLen = 0;
//		  DELETE(CascadeMap);
//			return OPT_FAILURE;
//		}
//		
//		
//		
//		for( i = 0; i < CascadeMapLen; i++) {
//		 // SPI_Flash_Read((u8*)&every_map, lineMapAddr + shift, MAP_LINE_LEN);//��ȡһ��ӳ��
//		  CascadeMap[i].type = every_map[0];
//			CascadeMap[i].host = every_map[1];
//			CascadeMap[i].typeno = *(u16*)(every_map + 2);
//			CascadeMap[i].remotenum = *(u16*)(every_map + 4);/*ͨ������*/
//			CascadeMap[i].remoteaddr = 0;
//			shift += MAP_LINE_LEN;
//			switch (CascadeMap[i].type) {
//				case 0://I
//			    CascadeMap[i].localaddr = incoilshift;/*��������ӳ���ַ*/
//				  CascadeMap[i].len = 1;
//					incoilshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(u16*)(every_map + 4);
//				  break;
//				case 1://O
//					CascadeMap[i].localaddr = coilshift;
//				  CascadeMap[i].len = 1;
//					coilshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(u16*)(every_map + 4);
//				  break;
//				case 2://DA
//					CascadeMap[i].localaddr = regshift;
//				  CascadeMap[i].len = 1;
//					regshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(u16*)(every_map + 4);
//				  break;
//				case 3://AD
//					CascadeMap[i].localaddr = inregshift;
//				  CascadeMap[i].len = 1;
//					inregshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(u16*)(every_map + 4);
//				  break;
//				default:
//					break;
//			}
//			
//		}
//		
//		NEWCH(cascade_recv_buf, CASCADE_BUF_LEN);
//		
//#if USE_CASCADE_STATIC == 1u
//		NEWCH(cascade_cmd_buf, CASCADE_BUF_MAX_LEN);
//		NEWCH(cascade_tmp_buf, CASCADE_BUF_MAX_LEN);
//#endif
//		
//		
//#if USE_OVERLAP
//	  NEWCH(g_coilCascade, sizeof(MODBUS_Coil));
//	  NEWCH(g_regCascade, sizeof(MODBUS_Register));
//#endif
//		/////////////////////////////���ӻ��Ƿ����
//		//if(DIP_ON == READ_DIP_SW(2))
//		{
//			if(OPT_FAILURE == CascadeModbus_Map()) {
//		//		OUT_DisPlay(0xFF280B48);
//				//OUT_DisPlay(0xFF511751);
//				while(1);
//			}
//		}
//		/////////////////////////////
//		return OPT_SUCCESS;
//	}
	
	
/**
  * @brief  O�첽ˢ��
  *  
  * @param  coilVal ��Ȧ��ֵ�������Ѿ���modbus���������
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   					
  */

	RET_STATUS CascadeOverlapOWrite(u8 host, u16 remoteaddr, u32 num, u8* coilVal) { 
		RET_STATUS ret =OPT_SUCCESS;
		u8* recvBuf = cascade_recv_buf;
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
		u8  cmdLen = 0;
		//u8 recvBuf[20] = {0};
		u16  recvLen = 0;
		
		WriteMultiCoilCmd(host, remoteaddr, num, coilVal, &cmdBuf, &cmdLen,opt);
    
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, CASCADE_USART,opt);
		
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
	int CascadeOverlapOWrite2(u8 host, u16 remoteaddr, u32 num, u8* coilVal) { 
		static int ret = 0;
		static u8* recvBuf = NULL;//u8* recvBuf = cascade_recv_buf;
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
		static u8  cmdLen = 0;
		//u8 recvBuf[20] = {0};
		static u16  recvLen = 0;
		static int stat = 0;
		switch(stat){
			case 0:
				WriteMultiCoilCmd(host, remoteaddr, num, coilVal, &cmdBuf, &cmdLen,opt);
			  stat =1;
				break;
			case 1:
				ret = UniSerialSendCRC2(cmdBuf, cmdLen, &recvBuf, &recvLen, CASCADE_USART,opt);
				if(0!=ret)
					stat = 2;
			  break;
			case 2:
				if(1 == ret && host == recvBuf[0] && 0x80 > recvBuf[1]) {
					ret = 1;
				} else {
					ret = -1;
				}
		#if USE_CASCADE_STATIC == 0u
				DELETE(cmdBuf);
		#endif
				stat = 0;
				return ret;
				break;
			default:
				stat = 0;
				break;
		}
		return 0;
	}
	
	/**
  * @brief  DA�첽ˢ��
  *  
  * @param  regVal ֵ�������Ѿ���modbus��������ã���Ҫ��modbus�����飬��ı�ֵ
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   					
  */

	RET_STATUS CascadeOverlapDAWrite(u8 host, u16 remoteaddr, u32 num, u8* regVal) { 
		RET_STATUS ret =OPT_SUCCESS;
		u8* recvBuf = cascade_recv_buf;
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
		u8  cmdLen = 0;
		//u8 recvBuf[100] = {0};
		u16  recvLen = 0;
		
		WriteMultiRegisterCmd(host, remoteaddr, num, regVal, &cmdBuf, &cmdLen,opt);
		//WriteMultiCoilCmd(host, remoteaddr, num, coilVal, &cmdBuf, &cmdLen);
    
		ret = UniSerialSendCRC(cmdBuf, cmdLen, &recvBuf, &recvLen, CASCADE_USART,opt);
		
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


	int CascadeOverlapDAWrite2(u8 host, u16 remoteaddr, u32 num, u8* regVal) { 
		static int ret = 0;
		static u8* recvBuf = NULL;//u8* recvBuf = cascade_recv_buf;
#if USE_CASCADE_STATIC == 1u
		u8 *cmdBuf = cascade_cmd_buf;
		BUF_OPT opt = BUF_NONE;
#else
		u8* cmdBuf = NULL;
		BUF_OPT opt = BUF_NEW;
#endif
		static u8  cmdLen = 0;
		//u8 recvBuf[100] = {0};
		static u16  recvLen = 0;
		static int stat = 0;
		switch(stat){
			
			case 0:
				WriteMultiRegisterCmd(host, remoteaddr, num, regVal, &cmdBuf, &cmdLen,opt);
				//WriteMultiCoilCmd(host, remoteaddr, num, coilVal, &cmdBuf, &cmdLen);
			  stat = 1;
				break;
			case 1:
				ret = UniSerialSendCRC2(cmdBuf, cmdLen, &recvBuf, &recvLen, CASCADE_USART,opt);
				if(0!=ret)
					stat = 2;
			  break;
			case 2:
				if(1 == ret && host == recvBuf[0] && 0x80 > recvBuf[1]) {
					ret = 1;
				} else {
					ret = -1;
				}
		#if USE_CASCADE_STATIC == 0u
				DELETE(cmdBuf);
		#endif
				stat =0;
				return ret;
				break;
			default:
				stat =0;
				break;
		}
		return 0;
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
		u8 i;
		RET_STATUS ret = OPT_SUCCESS;
		struct CASCADE_MAP* map = CascadeMap;
#if USE_CASCADE_STATIC == 1u
		u8 *tmp_buf = cascade_tmp_buf;
#endif
		//ASSERT(map);
		//ASSERT(g_coilCascade);//�˺���������ִ�У�����Ҫ
		
	//	if(!g_line.init || !map || !g_coilCascade || !g_regCascade) 
		if(!map) 
			return ret;
		
		for( i = 0; i < CascadeMapLen; i++) {
			switch (map[i].type) {
				case 0://I
					ret = CascadeModbus_ReadInCoil2((u8*)g_modbusInCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				case 1://O
#if USE_OVERLAP
				  ret = CascadeModbus_ReadCoil2((u8*)g_coilCascade->coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(OPT_SUCCESS == ret && CoilCmp((u8*)g_coilCascade->coilCh, map[i].localaddr, (u8*)g_modbusCoil.coilCh, map[i].localaddr, map[i].remotenum)) {
#if USE_CASCADE_STATIC == 0u			
					  u8 *tmp_buf = NULL;
						NEWCH(tmp_buf, map[i].remotenum / 8 + 3);
#endif
					ASSERT(map[i].localaddr > O_NUM);
						CoilToCoil((u8*)(g_modbusCoil.coilCh), map[i].localaddr, tmp_buf, 0, map[i].remotenum);
						ret = CascadeOverlapOWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_buf);
#if USE_CASCADE_STATIC == 0u
					  DELETE(tmp_buf);
#endif
					}
#else
					ret = CascadeModbus_ReadCoil2((u8*)g_modbusCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 2://DA
#if USE_OVERLAP
				  ret = CascadeModbus_ReadReg2((u8*)g_regCascade->reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(OPT_SUCCESS == ret && RegCmp(g_regCascade->reg, map[i].localaddr, g_modbusReg.reg, map[i].localaddr, map[i].remotenum)) {
#if USE_CASCADE_STATIC == 0u	
						u8 *tmp_buf = NULL;
						NEWCH(tmp_buf, 2*map[i].remotenum);
#endif
						MemCpy(tmp_buf, g_modbusReg.reg + map[i].localaddr, 2*map[i].remotenum);
						ret = CascadeOverlapDAWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_buf);
#if USE_CASCADE_STATIC == 0u	
						DELETE(tmp_buf);
#endif
					}
#else
					ret = CascadeModbus_ReadReg2((u8*)g_modbusReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 3://AD
					ret = CascadeModbus_ReadInReg2((u8*)g_modbusInReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				default:
					break;
			}
			
			if(ret == OPT_FAILURE)
				return ret;
			
			//delay_ms(1);
		}
		return ret;
	}
	
/** @brief  ����map���ӻ��Ƿ��й���
  * @param  void
  * @retval RET_STATUS
  * @note   �޹��أ�ϵͳ��������
  */

	RET_STATUS CascadeModbus_MapCheck(void) {
		u8 i;
		RET_STATUS ret = OPT_FAILURE;
		struct CASCADE_MAP* map = CascadeMap;
		
		if(!map) 
			return OPT_FAILURE;
		
		for( i = 0; i < CascadeMapLen; i++) {
			switch (map[i].type) {
				case 0://I
					ret = CascadeModbus_ReadInCoil2((u8*)g_modbusInCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				case 1://O
#if USE_OVERLAP
				  ret = CascadeModbus_ReadCoil2((u8*)g_coilCascade->coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(OPT_SUCCESS == ret && CoilCmp((u8*)g_coilCascade->coilCh, map[i].localaddr, (u8*)g_modbusCoil.coilCh, map[i].localaddr, map[i].remotenum)) {
						u8 *tmp_coil_val = NULL;
						NEWCH(tmp_coil_val, map[i].remotenum / 8 + 3);
						CoilToCoil((u8*)(g_modbusCoil.coilCh), map[i].localaddr, tmp_coil_val, 0, map[i].remotenum);
						ret = CascadeOverlapOWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_coil_val);
						DELETE(tmp_coil_val);
					}
#else
					ret = CascadeModbus_ReadCoil2((u8*)g_modbusCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 2://DA
#if USE_OVERLAP
				  ret = CascadeModbus_ReadReg2((u8*)g_regCascade->reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(OPT_SUCCESS == ret && RegCmp(g_regCascade->reg, map[i].localaddr, g_modbusReg.reg, map[i].localaddr, map[i].remotenum)) {
						//u8 *tmp_coil_val = NULL;
						//NEWCH(tmp_coil_val, map[i].remotenum / 8 + 3);
						//CoilToCoil((u8*)(g_modbusCoil.coilCh), map[i].localaddr, tmp_coil_val, 0, map[i].remotenum);
						ret = CascadeOverlapDAWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, (u8*)(g_regCascade->reg + map[i].localaddr));
						//DELETE(tmp_coil_val);
					}
#else
					ret = CascadeModbus_ReadReg2((u8*)g_modbusReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 3://AD
					ret = CascadeModbus_ReadInReg2((u8*)g_modbusInReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				default:
					break;
			}
			
			if(ret == OPT_FAILURE)
				return ret;
			
			//delay_ms(1);
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
	                    const u8 type,
										  const u16 hosttypeno,
										  const u8 host,
											u16* localaddr,
										  u16* remoteaddr) {
		u8 i;
		struct CASCADE_MAP* map = CascadeMap;
		
		if (!map) 
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
	                    const u8 type,
										  const u16 typeno,
											u16* localaddr,
										  u16* remoteaddr,
											u8* host) {
												
												
		u8 i;
		struct CASCADE_MAP* map = CascadeMap;
		u16 shift = 0;
		//u16 len = 0;
		
		if (!map) 
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

	RET_STATUS TFICmd(u8* buf, u8 len, CMD_ARG *arg) { 
		//OS_ERR err;
		u8 opt, type;
		u16 localaddr, remoteaddr;
		u32 no , num , val;
		//IO_VAL bit;
		RET_STATUS ret =OPT_SUCCESS;
		u8 host = buf[0];

		no  = *(u32*)(buf + 4);
		opt = buf[8];
		type = buf[9];
		num  = *(u32*)(buf + 10);
    ////
		//host no��ӳ��ת��
		ret = CascadeModbus_MapFind(0, *(u32*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0xFF:/*��ѯ*/
#if USE_OVERLAP
			  ret = OPT_SUCCESS;
#else
				ret = CascadeModbus_ReadInCoil2((u8*)g_modbusInCoil.coilCh, localaddr, host, 0, 1, CASCADE_USART);
#endif
			  if(OPT_SUCCESS == ret) {
//					bit = IRead(localaddr);
//					val = (BIT_1 == bit ? 1 : 0);
					val = READ_INCOIL_ADDR(localaddr);
					ResRequest(arg->buf, arg->len, (u8*)(&val), 4, arg,RC_CRC);
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

	RET_STATUS TFOCmd(u8* buf, u8 len, CMD_ARG *arg) { 
   // OS_ERR err;
		u8 opt, type;
		u16 localaddr, remoteaddr;
		u32 no , num , val;
		//IO_VAL bit;
		RET_STATUS ret =OPT_SUCCESS;
		u8 host = buf[0];
		no  = *(u32*)(buf + 4);
		opt = buf[8];
		type = buf[9];
		//num  = *(u32*)(buf + 10);
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
					u8  coil = 0;
#if !USE_OVERLAP
					u8* cmdBuf = NULL;
					u8  cmdLen = 0;
					u8 recvBuf[20] = {0};
					u8  recvLen = 0;
#endif
//					bit = (ArithmeticEx((char*)buf + 10, len - 10, arg)) ? BIT_1 : BIT_0;
//					coil = bit;
//					coil = (ArithmeticEx((char*)buf + 10, len - 10, arg)) ? 1 : 0;

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
			case 0x04:/*��ѯ*/
			case 0xFF:/*��ѯ*/
#if USE_OVERLAP
			  ret = OPT_SUCCESS;
#else
				ret = CascadeModbus_ReadCoil2((u8*)g_modbusCoil.coilCh, localaddr, host, remoteaddr, 1, CASCADE_USART);
#endif
				if(OPT_SUCCESS == ret) {
//					bit = ORead(localaddr); // ���o����addr0��ʼ������������
//					val = (BIT_1 == bit ? 1 : 0);
					val = READ_COIL_ADDR(localaddr);
					ResRequest(arg->buf, arg->len, (u8*)(&val), 4, arg, RC_CRC);
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

	RET_STATUS TFDACmd(u8* buf, u8 len, CMD_ARG *arg) { 
   // OS_ERR err;
		u8 opt, type;
		u16 localaddr, remoteaddr;
		u32 no , num , val;
		IO_VAL bit;
		RET_STATUS ret =OPT_SUCCESS;
		u8 host = buf[0];
		no  = *(u32*)(buf + 4);
		opt = buf[8];
		type = buf[9];
		num  = *(u32*)(buf + 10);
    ////
		//host no��ӳ��ת��
		ret = CascadeModbus_MapFind(2, *(u32*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0x02:/*���*/
				{
					u16  reg = 0;
#if !USE_OVERLAP
					u8* cmdBuf = NULL;
					u8  cmdLen = 0;
					u8  recvBuf[30] = {0};
					u8  recvLen = 0;
#endif
					//reg = ArithmeticEx((char*)buf + 10, len - 10, arg) ;
#if USE_OVERLAP
					g_modbusReg.reg[localaddr] = reg;
#else
					WriteMultiRegisterCmd(host, remoteaddr, 1, (u8*)&reg, &cmdBuf, &cmdLen);
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
			case 0xFF:/*��ѯ*/
#if USE_OVERLAP
			  ret = OPT_SUCCESS;
#else
				ret = CascadeModbus_ReadReg2((u8*)g_modbusReg.reg, localaddr, host, remoteaddr, 1, CASCADE_USART);
#endif	
			if(OPT_SUCCESS == ret) {
					val = g_modbusReg.reg[localaddr];
					ResRequest(arg->buf, arg->len, (u8*)(&val), 4, arg, RC_CRC);
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

	RET_STATUS TFADCmd(u8* buf, u8 len, CMD_ARG *arg) { 
  //  OS_ERR err;
		u8 opt, type;
		u16 localaddr, remoteaddr;
		u32 no , num , val;
		IO_VAL bit;
		RET_STATUS ret =OPT_SUCCESS;
		u8 host = buf[0];
		no  = *(u32*)(buf + 4);
		opt = buf[8];
		type = buf[9];
		num  = *(u32*)(buf + 10);
    ////
		//host no��ӳ��ת��
		ret = CascadeModbus_MapFind(3, *(u32*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0x04:/*��ѯ*/
			case 0xFF:/*��ѯ*/
				ret = CascadeModbus_ReadInReg2((u8*)g_modbusInReg.reg, localaddr, host, remoteaddr, 1, CASCADE_USART);
				if(OPT_SUCCESS == ret) {
					val = g_modbusInReg.reg[localaddr];
					ResRequest(arg->buf, arg->len, (u8*)(&val), 4, arg, RC_CRC);
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
  RET_STATUS TFUsartCmdParse (u8* buf, u16 len, CMD_ARG *arg) {
		RET_STATUS ret = OPT_SUCCESS;
		
	//u32 gstartTime ,gendTime , gtime;
	//	gstartTime = GetCdvTimeTick();
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
		//gendTime = GetCdvTimeTick();
		//gtime = CalcCount(gendTime , gstartTime);
		return ret;
	}

/** @brief  ӳ���ʼ��
  * @param  void
  * @retval RET_STATUS
  * @note   T I buf CRC
  */
	
	RET_STATUS CascadeModbus_MapInit(u8* buf, u16 len) {
		#define MAP_LINE_LEN 6	
		#define REG_SHIFT  0
		//CDV_VAR_NUM * 2
		#define INREG_SHIFT  0                       //����ADDA
		#define COIL_SHIFT  0
		//CDV_O_NUM + CDV_EXO_NUM*CDV_FPGA_NUM
		#define INCOIL_SHIFT  0
		//CDV_I_NUM + CDV_EXI_NUM*CDV_FPGA_NUM
    RET_STATUS ret;
		//struct CASCADE_MAP* map = CascadeMap;
		/////////////////////////////
		u32 size = 0;//�ű���С������ת
		u32 shift = 0;//ƫ����ʼ��ַ���ֽ���
		u32 lineMapAddr = 0;//SCRIP_MAP;//������ӳ�����ʼ��ַ
	  u32 lineMapLen = 0;//������ӳ��ĳ���
		u8 every_map[MAP_LINE_LEN];
		u8 i;
		u16 regshift = REG_SHIFT;
		u16 inregshift = INREG_SHIFT;
		u16 coilshift = COIL_SHIFT;
		u16 incoilshift = INCOIL_SHIFT;
		
		
		// main�У�Ӧ��û�г�ͻ�������޸�
		// if(CascadeMap) 
		//	 return OPT_FAILURE; // �Ѿ���ʼ��������Ҫ����
		
		MemCpy((u8*)&size, buf+lineMapAddr + shift, 4);
		//SPI_Flash_Read((u8*)&size, lineMapAddr + shift, 4);//��ȡ������ӳ��ű���С
		lineMapLen = size;
		shift += 4;
		size = 0;
		MemCpy((u8*)&size, buf+lineMapAddr + shift, 1);
		//SPI_Flash_Read((u8*)&size, lineMapAddr + shift, 1);//��ȡ������ӳ�����
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
			
			MemCpy((u8*)&every_map, buf+lineMapAddr + shift, MAP_LINE_LEN);
		 // SPI_Flash_Read((u8*)&every_map, lineMapAddr + shift, MAP_LINE_LEN);//��ȡһ��ӳ��
		  CascadeMap[i].type = every_map[0];
			CascadeMap[i].host = every_map[1];
			CascadeMap[i].typeno = *(u16*)(every_map + 2);
			CascadeMap[i].remotenum = *(u16*)(every_map + 4);/*ͨ������*/
			CascadeMap[i].remoteaddr = 0;
			shift += MAP_LINE_LEN;
			
			switch (CascadeMap[i].type) {
				case 0://I
			    CascadeMap[i].localaddr = incoilshift;/*��������ӳ���ַ*/
				  CascadeMap[i].len = 1;
					incoilshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(u16*)(every_map + 4);
				  break;
				case 1://O
					CascadeMap[i].localaddr = coilshift;
				  CascadeMap[i].len = 1;
					coilshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(u16*)(every_map + 4);
				  break;
				case 2://DA
					CascadeMap[i].localaddr = regshift;
				  CascadeMap[i].len = 1;
					regshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(u16*)(every_map + 4);
				  break;
				case 3://AD
					CascadeMap[i].localaddr = inregshift;
				  CascadeMap[i].len = 1;
					inregshift += CascadeMap[i].remotenum * CascadeMap[i].len;//*(u16*)(every_map + 4);
				  break;
				default:
					break;
			}
			
		}
		
		if(!cascade_recv_buf)
		NEWCH(cascade_recv_buf, CASCADE_BUF_LEN);
		
#if USE_CASCADE_STATIC == 1u
		if(!cascade_cmd_buf)
		NEWCH(cascade_cmd_buf, CASCADE_BUF_MAX_LEN);
		if(!cascade_tmp_buf)
		NEWCH(cascade_tmp_buf, CASCADE_BUF_MAX_LEN);
#endif
		
		
#if USE_OVERLAP
		if(!g_coilCascade)
	  NEWCH(g_coilCascade, sizeof(MODBUS_Coil));
		if(!g_regCascade)
	  NEWCH(g_regCascade, sizeof(MODBUS_Register));
#endif
		/////////////////////////////���ӻ��Ƿ����
		//if(DIP_ON == READ_DIP_SW(2))
//		{
//			if(OPT_FAILURE == CascadeModbus_Map()) {
//		//		OUT_DisPlay(0xFF280B48);
//				//OUT_DisPlay(0xFF511751);
//				while(1);
//			}
//		}
		/////////////////////////////
		// �ӻ����ݱ��滺��
		regshift -= REG_SHIFT;
		inregshift -= INREG_SHIFT;
		coilshift -= COIL_SHIFT;
		incoilshift -= INCOIL_SHIFT;

			
		shift = 0;
		
		cascade_incoil = cascade_data + shift;
		cascade_incoil_chlen = (incoilshift>>3)+((incoilshift & 0x07) ? 1 : 0);
		shift += cascade_incoil_chlen;
		
		cascade_inreg= cascade_data + shift;
		cascade_inreg_chlen = inregshift << 1 ;
		shift += cascade_inreg_chlen;
		
		cascade_coil= cascade_data + shift;
		cascade_coil_chlen = (coilshift>>3)+((coilshift & 0x07) ? 1 : 0);
		shift += cascade_coil_chlen;
		
		cascade_reg= cascade_data + shift;
		cascade_reg_chlen = regshift << 1 ;
		shift += cascade_reg_chlen;
		
		if (shift > CASCADE_DATA_LEN)
		{
			CascadeMapLen = 0;
		  DELETE(CascadeMap);
			return OPT_FAILURE;
		}
		
		
		return OPT_SUCCESS;
	}

///** @brief  cascade �ṹ���ʼ��
//  * @param  
//  * @retval int -1 ʧ�� �� 0 �ɹ�
//  * @note   0x01	0x10	�������ݳ���	����ID	ԭָ��
//  */
//  int CascadeMap_Init (u8* buf, u16 len, CMD_ARG *arg) {
//		
//		if(len % sizeof(struct CASCADE_MAP) != 0)
//			return -1;
//		
//		CascadeMapLen = len / sizeof(struct CASCADE_MAP);
//		
//		NEWCH(CascadeMap, len/*CascadeMapLen * sizeof(struct CASCADE_MAP)*/);
//		
//		if(CascadeMap)
//		  MemCpy(CascadeMap, buf, len);
//		else
//			return -1;
//		
//		return 0;
//	}
//	

/** @brief  ��ȡ�ӻ��ļĴ���������
  * @param  
  * @retval int -1 δ��ʼ�� 0 ���� 1 ͨѶ������
  * @note   CascadeModbus_Map_Stat,��������
  */

u32 count_ticks = 0,start_ticks = 0;

	int CascadeModbus_Map_Stat(void) {
		static u32 led_ticks = 0;
		//static int stat = 0;
		static u8 i = 0;
		RET_STATUS ret = OPT_SUCCESS;
		struct CASCADE_MAP* map = CascadeMap;
#if USE_CASCADE_STATIC == 1u
		u8 *tmp_buf = cascade_tmp_buf;
#endif
		
		if(!map) 
			return -1;
		
		
		//for( i = 0; i < CascadeMapLen; i++) {
		if(i < CascadeMapLen && CalcCount(sys_ticks, led_ticks)>3) {
			
			if(i == 0) {
				count_ticks = CalcCount(sys_ticks, start_ticks);
				start_ticks = sys_ticks;
				if(count_ticks > 130)
					i = 0;
			}
			
			switch (map[i].type) {
				case 0://I
					ret = CascadeModbus_ReadInCoil2(cascade_incoil/*(u8*)g_modbusInCoil.coilCh*/, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				case 1://O
#if USE_OVERLAP
				  //ret = CascadeModbus_ReadCoil2((u8*)g_coilCascade->coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(1/*OPT_SUCCESS == ret && CoilCmp((u8*)g_coilCascade->coilCh, map[i].localaddr, (u8*)g_modbusCoil.coilCh, map[i].localaddr, map[i].remotenum)*/) {
#if USE_CASCADE_STATIC == 0u
					  u8 *tmp_buf = NULL;
						NEWCH(tmp_buf, map[i].remotenum / 8 + 3);
#endif
					  //ASSERT(map[i].localaddr > O_NUM);
						CoilToCoil(cascade_coil/*(u8*)(g_modbusCoil.coilCh)*/, map[i].localaddr, tmp_buf, 0, map[i].remotenum);
						ret = CascadeOverlapOWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_buf);
					
#if USE_CASCADE_STATIC == 0u
					  DELETE(tmp_buf);
#endif
					}
#else
					ret = CascadeModbus_ReadCoil2((u8*)g_modbusCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 2://DA
#if USE_OVERLAP
				  //ret = CascadeModbus_ReadReg2((u8*)g_regCascade->reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(1/*OPT_SUCCESS == ret && RegCmp(g_regCascade->reg, map[i].localaddr, g_modbusReg.reg, map[i].localaddr, map[i].remotenum)*/) {
#if USE_CASCADE_STATIC == 0u	
						u8 *tmp_buf = NULL;
						NEWCH(tmp_buf, 2*map[i].remotenum);
#endif
						MemCpy(tmp_buf, cascade_reg/*g_modbusReg.reg*/ + map[i].localaddr, 2*map[i].remotenum);
						ret = CascadeOverlapDAWrite(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_buf);
#if USE_CASCADE_STATIC == 0u	
						DELETE(tmp_buf);
#endif
					}
#else
					ret = CascadeModbus_ReadReg2((u8*)g_modbusReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 3://AD
					ret = CascadeModbus_ReadInReg2(cascade_inreg/*(u8*)g_modbusInReg.reg*/, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				default:
					break;
			}
			
//			if(ret == OPT_FAILURE)
//				return ret;
			
			//delay_ms(1);
			
			led_ticks = sys_ticks;
			++i;
		}
		
		if(i >= CascadeMapLen)
			i = 0;
		
		return ret;
	}
	int CascadeModbus_Map_Stat2(void) {
		static u32 led_ticks = 0;
		//static int stat = 0;
		static u8 i = 0;
		int ret = 0;
		struct CASCADE_MAP* map = CascadeMap;
#if USE_CASCADE_STATIC == 1u
		u8 *tmp_buf = cascade_tmp_buf;
#endif
		
		if(!map) 
			return -1;
		
		
		//for( i = 0; i < CascadeMapLen; i++) {
		if(i < CascadeMapLen && CalcCount(sys_ticks, led_ticks)>3) {
			

			
			switch (map[i].type) {
				case 0://I
					ret = CascadeModbus_ReadInCoil3(cascade_incoil/*(u8*)g_modbusInCoil.coilCh*/, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				case 1://O
#if USE_OVERLAP
				  //ret = CascadeModbus_ReadCoil2((u8*)g_coilCascade->coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(1/*OPT_SUCCESS == ret && CoilCmp((u8*)g_coilCascade->coilCh, map[i].localaddr, (u8*)g_modbusCoil.coilCh, map[i].localaddr, map[i].remotenum)*/) {
#if USE_CASCADE_STATIC == 0u
					  u8 *tmp_buf = NULL;
						NEWCH(tmp_buf, map[i].remotenum / 8 + 3);
#endif
					  //ASSERT(map[i].localaddr > O_NUM);
						CoilToCoil(cascade_coil/*(u8*)(g_modbusCoil.coilCh)*/, map[i].localaddr, tmp_buf, 0, map[i].remotenum);
						ret = CascadeOverlapOWrite2(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_buf);
					
#if USE_CASCADE_STATIC == 0u
					  DELETE(tmp_buf);
#endif
					}
#else
					ret = CascadeModbus_ReadCoil2((u8*)g_modbusCoil.coilCh, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 2://DA
#if USE_OVERLAP
				  //ret = CascadeModbus_ReadReg2((u8*)g_regCascade->reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
				  if(1/*OPT_SUCCESS == ret && RegCmp(g_regCascade->reg, map[i].localaddr, g_modbusReg.reg, map[i].localaddr, map[i].remotenum)*/) {
#if USE_CASCADE_STATIC == 0u	
						u8 *tmp_buf = NULL;
						NEWCH(tmp_buf, 2*map[i].remotenum);
#endif
						MemCpy(tmp_buf, cascade_reg/*g_modbusReg.reg*/ + map[i].localaddr, 2*map[i].remotenum);
						ret = CascadeOverlapDAWrite2(map[i].host, map[i].remoteaddr, map[i].remotenum, tmp_buf);
#if USE_CASCADE_STATIC == 0u	
						DELETE(tmp_buf);
#endif
					}
#else
					ret = CascadeModbus_ReadReg2((u8*)g_modbusReg.reg, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
#endif
					break;
				case 3://AD
					ret = CascadeModbus_ReadInReg3(cascade_inreg/*(u8*)g_modbusInReg.reg*/, map[i].localaddr, map[i].host, map[i].remoteaddr, map[i].remotenum, CASCADE_USART);
					break;
				default:
					break;
			}
			
//			if(ret == OPT_FAILURE)
//				return ret;
			
			//delay_ms(1);
			
			if(0!=ret){
				led_ticks = sys_ticks;
							if(i == 0) {
				count_ticks = CalcCount(sys_ticks, start_ticks);
				start_ticks = sys_ticks;
				if(count_ticks > 130)
					i = 0;
			}
			}
			if(1==ret)++i;
		}
		
		if(i >= CascadeMapLen)
			i = 0;
		
		return ret;
	}
	
/** @brief  ��ȡ����������
  * @param  
  * @retval int -1 δ��ʼ�� 0 ���� 1 ͨѶ������
  * @note   CascadeModbus_Map_Stat,��������
  */

	int Cascade_Host_Stat(void) {
		
		return USARTRT(CmdParse , 1);
		
	}
	
	
/** @brief  ӳ���ʼ��
  * @param  rtbuf ָ�룬�ⲿ�����ͷ�
  * @retval 0 ���� -1 ������
  * @note   T W buf CRC
  */
	
	int Cascade_Host_Transfer(u8* buf, u16 len, u8** rtbuf,u16* rtlen) {
		static u16 txlen = 0;
		static u8 txbuf[CASCADE_BUF_LEN];
		u16 crc;
			
		if( !CascadeMap || len != cascade_coil_chlen + cascade_reg_chlen)
			return -1;
		
		txlen = cascade_incoil_chlen + cascade_inreg_chlen;
//		txbuf = NULL;
		// W
		MemCpy(cascade_coil, buf , len);
		// R
		//NEWCH(txbuf, txlen + 4/*T R CRC*/);
		
		txbuf[0] = 'T';
		txbuf[1] = 'R';
		MemCpy(txbuf + 2, cascade_incoil, txlen);
		
		crc=getCRC16(txbuf,txlen + 2);
	  MemCpy(txbuf + txlen + 2, &crc, 2);
		
		*rtbuf = txbuf;
		*rtlen = txlen + 4;
		
		return 0;
	}
	
	