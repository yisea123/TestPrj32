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
	
	#define CASCADE_DATA_LEN 512
	/*
	分配是incoil + inreg + coil + reg
	写需要刷新从cascade_coil地址开始的cascade_coil_chlen + cascade_reg_chlen字节
	读需要发送从cascade_incoil地址开始的cascade_incoil_chlen + cascade_inreg_chlen字节
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
  //级联资源表——版本号
	u8 version[] = {0/*id号*/, 3/*软件大版本*/, 2/*硬件大版本*/, 3/*硬件驱动版本*/, 13/*迭代小版本*/};

	
#if USE_CASCADE_STATIC == 1u
	#define CASCADE_BUF_MAX_LEN 256
	u8* cascade_cmd_buf = NULL; // 用于命令的静态buf，不动态分配
	u8* cascade_tmp_buf = NULL;
#endif
	
  //开启主级联口
	#define	CASCADE_EN 			CASCADE_CS_EN;FPGA1_CS_DS;FPGA2_CS_DS;FLASH_CS_DS;
	//关闭主级联口
	#define	CASCADE_DS 			CASCADE_CS_DS;
	//主级联口发数据
	#define CASCADE_ReadWriteByte(a)				SPIx_ReadWriteByte(SPI2,(a))

	//从机资源表
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
	}REG_MAP;//用于writereg的标记
	
	struct
	{
		u8 coil[COIL_CHN];
		u8 addr[COIL_CHN];
		u8 id[COIL_CHN];//一个tab的id一致，0为本机
		u8 cnt[COIL_CHN];//是否需要写，用critical
		u8 cnted[COIL_CHN];//
	}COIL_MAP;//用于writecoil的标记
	
/** @brief  读到变量 寄存器
  * @param  buf     命令
            len     buf长度
            inReg   寄存器保存地址
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
/** @brief  读到变量 寄存器
  * @param  buf     命令
            len     buf长度
            inReg   寄存器保存地址
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
/** @brief  读到变量 寄存器
  * @param  buf     命令
            len     buf长度
            reg   寄存器保存地址
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
/** @brief  读到变量 寄存器
  * @param  buf     命令
            len     buf长度
            reg   寄存器保存地址
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
/** @brief  读到变量 线圈
  * @param  buf     命令
            len     buf长度
            inCoil   寄存器保存地址
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

/** @brief  读到变量 线圈
  * @param  buf     命令
            len     buf长度
            inCoil  寄存器保存地址
            addr    coil的起始地址0~
  * @retval 
  * @note   不考虑线圈数量
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
/** @brief  读到变量 线圈
  * @param  buf     命令
            len     buf长度
            coil   寄存器保存地址
  * @retval 
  * @note   不考虑线圈数量
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
RET_STATUS CoilToCoil(u8* buf, u8 bufaddr, u8* coil, u16 coiladdr, u16 tonum){
	//u8 fc = buf[1];、
	u8 buf_sta_ch = bufaddr >> 3;
	u8 buf_sf = bufaddr & 0x07;
	u8 coil_sta_ch = (coiladdr >> 3);//读线圈的初始char
	u8 coil_sf = coiladdr & 0x07;
	u16 num = tonum;
	u8 numCh = (num + 7) / 8;//标准长度，sf = 0
	u8 i;
	u8 firstCh, secondCh;
	u8* pCoil = (u8*)(buf + buf_sta_ch);
	RET_STATUS ret = OPT_FAILURE;
	u16 buf_end_num;
	
	ASSERT(buf  && coil);

	__disable_irq();
	
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
		pCoil = (u8*)(buf + buf_sta_ch);
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
	
	__enable_irq();
	//////////////////
	
	ret = OPT_SUCCESS;
	return ret;
}

/** @brief  线圈比较
  * @param  参考CoilToCoil
  * @retval 0 相同，1 不相同
  * @note   
  */
int CoilCmp(u8* buf, u16 bufaddr, u8* coil, u16 coiladdr, u16 tonum){
	u8 coil_sta_ch = (coiladdr >> 3);//读线圈的初始char
	u8 coil_sf = coiladdr & 0x07;
	u8 coil_numCh = (tonum - 8 + coil_sf + 7) / 8 + (coil_sf ? 1 :0);//标准长度，sf = 0
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

/** @brief   寄存器比较
  * @param  参考CoilToCoil
  * @retval 0 相同，1 不相同
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


/** @brief  读取从机的寄存器到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
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
/** @brief  读取从机的寄存器到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
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
/** @brief  读取从机的只读寄存器到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
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
/** @brief  读取从机的只读寄存器到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
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
/** @brief  读取从机的线圈到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
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
/** @brief  读取从机的线圈到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
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
/** @brief  读取从机的只读线圈到本机
  * @param  pBuffer      查询到的值保存的地方
	*         id           主机号
  *         addr         查询起始地址
  *         num          查询的数量
            uart         从机所在串口
  * @retval RET_STATUS
  * @note   CascadeModbus只能用在一个线程中
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
	//从机更新
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
	u8 CascadeMapLen = 0;

	
///** @brief  映射初始化
//  * @param  void
//  * @retval RET_STATUS
//  * @note   参考说明文档
//  */
//	
//	RET_STATUS CascadeModbus_MapInit(void) {
//		#define MAP_LINE_LEN 6	
//		#define REG_SHIFT  0
//		//CDV_VAR_NUM * 2
//		#define INREG_SHIFT  18                       //放在ADDA
//		#define COIL_SHIFT  0
//		//CDV_O_NUM + CDV_EXO_NUM*CDV_FPGA_NUM
//		#define INCOIL_SHIFT  0
//		//CDV_I_NUM + CDV_EXI_NUM*CDV_FPGA_NUM
//    RET_STATUS ret;
//		//struct CASCADE_MAP* map = CascadeMap;
//		/////////////////////////////
//		u32 size = 0;//脚本大小，供跳转
//		u32 shift = 0;//偏移起始地址的字节数
//		u32 lineMapAddr = 0;//SCRIP_MAP;//生产线映射的起始地址
//	  u32 lineMapLen = 0;//生产线映射的长度
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
//		//SPI_Flash_Read((u8*)&size, lineMapAddr + shift, 4);//读取生产线映射脚本大小
//		lineMapLen = size;
//		shift += 4;
//		size = 0;
//		//SPI_Flash_Read((u8*)&size, lineMapAddr + shift, 1);//读取生产线映射个数
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
//		 // SPI_Flash_Read((u8*)&every_map, lineMapAddr + shift, MAP_LINE_LEN);//读取一行映射
//		  CascadeMap[i].type = every_map[0];
//			CascadeMap[i].host = every_map[1];
//			CascadeMap[i].typeno = *(u16*)(every_map + 2);
//			CascadeMap[i].remotenum = *(u16*)(every_map + 4);/*通道数量*/
//			CascadeMap[i].remoteaddr = 0;
//			shift += MAP_LINE_LEN;
//			switch (CascadeMap[i].type) {
//				case 0://I
//			    CascadeMap[i].localaddr = incoilshift;/*在主机的映射基址*/
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
//		/////////////////////////////检测从机是否挂载
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
  * @brief  O异步刷新
  *  
  * @param  coilVal 线圈的值，必须已经如modbus那样整理好
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
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
	/**
  * @brief  DA异步刷新
  *  
  * @param  regVal 值，必须已经如modbus那样整理好，不要用modbus的数组，会改变值
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
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
		u8 i;
		RET_STATUS ret = OPT_SUCCESS;
		struct CASCADE_MAP* map = CascadeMap;
#if USE_CASCADE_STATIC == 1u
		u8 *tmp_buf = cascade_tmp_buf;
#endif
		//ASSERT(map);
		//ASSERT(g_coilCascade);//此函数无条件执行，不需要
		
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
	
/** @brief  利用map检查从机是否有挂载
  * @param  void
  * @retval RET_STATUS
  * @note   无挂载，系统不能启动
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
	
/** @brief  得到一个具体资源的localaddr
  * @param  type          类型
	*         hosttypeno     host所在的序号，比如2号机的da1
  *         host          主机号
  *         retno         返回no
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
		//host no的映射转化
		ret = CascadeModbus_MapFind(0, *(u32*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0xFF:/*查询*/
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
  * @brief  O脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   主机id 01 64 02 资源号（00-FF）	00输出	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
		                                FE	变量代号高位	变量代号低位							
            
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
			case 0x04:/*查询*/
			case 0xFF:/*查询*/
#if USE_OVERLAP
			  ret = OPT_SUCCESS;
#else
				ret = CascadeModbus_ReadCoil2((u8*)g_modbusCoil.coilCh, localaddr, host, remoteaddr, 1, CASCADE_USART);
#endif
				if(OPT_SUCCESS == ret) {
//					bit = ORead(localaddr); // 如果o不是addr0开始的则不能这样用
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
  * @brief  ad脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   主机id 01 64 0C 资源号（00-FF）	00输出	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
		                                FE	变量代号高位	变量代号低位							
            
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
		//host no的映射转化
		ret = CascadeModbus_MapFind(2, *(u32*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0x02:/*输出*/
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
			case 0xFF:/*查询*/
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
  * @brief  ad脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   主机id 01 64 0B 资源号（00-FF）	00输出	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
		                                FE	变量代号高位	变量代号低位							
            
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
		//host no的映射转化
		ret = CascadeModbus_MapFind(3, *(u32*)(buf + 4), host, &localaddr, &remoteaddr);
		if(OPT_FAILURE == ret)
			return ret;
		////
		switch(opt) {
			case 0x04:/*查询*/
			case 0xFF:/*查询*/
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
/** @brief  CascadeTranferCmd之后的解析
  * @param  pBuf   命令
	*         len    命令长度
  * @retval RET_STATUS
  * @note   0x01	0x10	报文数据长度	主机ID	原指令
  */
  RET_STATUS TFUsartCmdParse (u8* buf, u16 len, CMD_ARG *arg) {
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

/** @brief  映射初始化
  * @param  void
  * @retval RET_STATUS
  * @note   T I buf CRC
  */
	
	RET_STATUS CascadeModbus_MapInit(u8* buf, u16 len) {
		#define MAP_LINE_LEN 6	
		#define REG_SHIFT  0
		//CDV_VAR_NUM * 2
		#define INREG_SHIFT  0                       //放在ADDA
		#define COIL_SHIFT  0
		//CDV_O_NUM + CDV_EXO_NUM*CDV_FPGA_NUM
		#define INCOIL_SHIFT  0
		//CDV_I_NUM + CDV_EXI_NUM*CDV_FPGA_NUM
    RET_STATUS ret;
		//struct CASCADE_MAP* map = CascadeMap;
		/////////////////////////////
		u32 size = 0;//脚本大小，供跳转
		u32 shift = 0;//偏移起始地址的字节数
		u32 lineMapAddr = 0;//SCRIP_MAP;//生产线映射的起始地址
	  u32 lineMapLen = 0;//生产线映射的长度
		u8 every_map[MAP_LINE_LEN];
		u8 i;
		u16 regshift = REG_SHIFT;
		u16 inregshift = INREG_SHIFT;
		u16 coilshift = COIL_SHIFT;
		u16 incoilshift = INCOIL_SHIFT;
		
		
		// main中，应该没有冲突，可以修改
		// if(CascadeMap) 
		//	 return OPT_FAILURE; // 已经初始化过，需要重启
		
		MemCpy((u8*)&size, buf+lineMapAddr + shift, 4);
		//SPI_Flash_Read((u8*)&size, lineMapAddr + shift, 4);//读取生产线映射脚本大小
		lineMapLen = size;
		shift += 4;
		size = 0;
		MemCpy((u8*)&size, buf+lineMapAddr + shift, 1);
		//SPI_Flash_Read((u8*)&size, lineMapAddr + shift, 1);//读取生产线映射个数
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
		 // SPI_Flash_Read((u8*)&every_map, lineMapAddr + shift, MAP_LINE_LEN);//读取一行映射
		  CascadeMap[i].type = every_map[0];
			CascadeMap[i].host = every_map[1];
			CascadeMap[i].typeno = *(u16*)(every_map + 2);
			CascadeMap[i].remotenum = *(u16*)(every_map + 4);/*通道数量*/
			CascadeMap[i].remoteaddr = 0;
			shift += MAP_LINE_LEN;
			
			switch (CascadeMap[i].type) {
				case 0://I
			    CascadeMap[i].localaddr = incoilshift;/*在主机的映射基址*/
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
		/////////////////////////////检测从机是否挂载
		//if(DIP_ON == READ_DIP_SW(2))
//		{
//			if(OPT_FAILURE == CascadeModbus_Map()) {
//		//		OUT_DisPlay(0xFF280B48);
//				//OUT_DisPlay(0xFF511751);
//				while(1);
//			}
//		}
		/////////////////////////////
		// 从机数据保存缓存
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

///** @brief  cascade 结构体初始化
//  * @param  
//  * @retval int -1 失败 ， 0 成功
//  * @note   0x01	0x10	报文数据长度	主机ID	原指令
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

/** @brief  读取从机的寄存器到本机
  * @param  
  * @retval int -1 未初始化 0 正常 1 通讯不正常
  * @note   CascadeModbus_Map_Stat,不可重入
  */

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
		if(i < CascadeMapLen && CalcCount(sys_ticks, led_ticks)>1) {
			
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
	
/** @brief  读取主机的命令
  * @param  
  * @retval int -1 未初始化 0 正常 1 通讯不正常
  * @note   CascadeModbus_Map_Stat,不可重入
  */

	int Cascade_Host_Stat(void) {
		
		return USARTRT(CmdParse , 1);
		
	}
	
	
/** @brief  映射初始化
  * @param  rtbuf 指针，外部无需释放
  * @retval 0 正常 -1 不正常
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
	
	