/**
  ******************************************************************************
  * @file    /newScript.c 
  * @author  MMY
  * @version V0.0.1
  * @date    04-27-2016
  * @brief   public define and interface of new uart control of newScript
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "newScript.h"
	
	LINE_ADDR_INFO g_line = {0};
	
	RET_STATUS LineCrcChk(void);
/**
  * @brief  清除生产线信息
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   
  */

void ClearLineInfo(void){
	g_line.init = 0;
	g_line.addr = 0;
	g_line.endAddr = 0;
	g_line.resAddr = 0;
	g_line.AlarmAddr = 0;
	g_line.mapAddr = 0;
	g_line.no = 0;
	ClearArrayU32(&g_line.workerAddr);
	ClearArrayU32(&g_line.configAddr);
}
/**
  * @brief  加入工人地址
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   按顺序加入
  */

void AddWorkerAddr(const CDV_INT32U addr){
	ArrayU32Add(&g_line.workerAddr, addr);
}
/**
  * @brief  加入工人配置地址
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   按顺序加入
  */

void AddConfigAddr(const CDV_INT32U addr){
	ArrayU32Add(&g_line.configAddr, addr);
}
/**
  * @brief  获取指定工人地址
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   从0开始
  */
CDV_INT32U GetWorkerAddr(const CDV_INT08U workerNo){
	CDV_INT08U no = workerNo;
	if(!g_line.init) {
		return 0;
	}
	ASSERT(g_line.init);
	ASSERT(no < g_line.workerAddr.len);
	
	return g_line.workerAddr.p[no];
}
/**
  * @brief  获取指定工人配置地址
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   从0开始
  */
CDV_INT32U GetConfigAddr(const CDV_INT08U workerNo){
	CDV_INT08U no = workerNo;
	if(!g_line.init) {
		return 0;
	}
	if(no >= g_line.configAddr.len)
		no = g_line.configAddr.len - 1;
	return g_line.configAddr.p[no];
}

/**
  * @brief  获取指定资源参数地址
  *  
  * @param  void
						ctrl 1:restype
								 2:resNo
								 3:resPar
  *   
  * @retval 返回值说明
  *
  * @note   从0开始，存储格式必须小端
  */
CDV_INT32U GetResAddr(const CDV_INT08U resType){
	CDV_INT08U no, type, par;
	CDV_INT32U size = 0;//脚本大小，供跳转
	CDV_INT32U shift = 0;//偏移起始地址的字节数
	CDV_INT32U resEndAddr = 0;//生产线的资源结束地址
	
	if(!g_line.init) {
		return 0;
	}
	
	
	SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr, 4);//读取生产线资源脚本大小
	shift = 4;
	resEndAddr = g_line.resAddr + shift + size;
	shift += 1;//跳过种类数
	//匹配type
	while(g_line.resAddr + shift < resEndAddr) {
		SPI_Flash_Read((CDV_INT08U*)&type, g_line.resAddr + shift, 1);//读取资源种类代号
		shift += 1;
		
		
		SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 4);//读取种类脚本大小
	  
		shift += 4;
		if(type == resType)
			break;
		
		shift += size;
	}
	
	if(g_line.resAddr + shift >= resEndAddr)
		return 0;//没找到
	else
		return g_line.resAddr + shift-4;//返回种类脚本大小地址
	
	return 0;
	
}
/**
  * @brief  获取指定资源参数地址
  *  
  * @param  void
						ctrl 1:restype
								 2:resNo
								 3:resPar
  *   
  * @retval 返回值说明
  *
  * @note   从0开始，存储格式必须小端
  */
CDV_INT32U GetResAddr0(const CDV_INT08U resType, const CDV_INT08U resNo, const CDV_INT08U resPar, const CDV_INT08U ctrl){
	CDV_INT08U no, type, par;
	CDV_INT32U size = 0;//脚本大小，供跳转
	CDV_INT32U shift = 0;//偏移起始地址的字节数
	CDV_INT32U resEndAddr = 0;//生产线的资源结束地址
	
	if(!g_line.init) {
		return 0;
	}
	
	if(ctrl <1 || ctrl >3)
		return g_line.resAddr;
	
	
	SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr, 4);//读取生产线资源脚本大小
	shift = 4;
	resEndAddr = g_line.resAddr + shift + size;
	shift += 1;
	//匹配type
	while(g_line.resAddr + shift < resEndAddr) {
		SPI_Flash_Read((CDV_INT08U*)&type, g_line.resAddr + shift, 1);//读取资源种类代号
		shift += 1;
		
		
		SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 4);//读取种类脚本大小
	  
		shift += 4;
		if(type == resType)
			break;
		
		shift += size;
	}
	
	if(g_line.resAddr + shift >= resEndAddr)
		return 0;//没找到
	
	if(1 == ctrl)
		return g_line.resAddr + shift-4;//返回种类脚本大小地址
	
	resEndAddr = g_line.resAddr + shift + size;
	shift += 1;
	
	//匹配no
	while(g_line.resAddr + shift < resEndAddr) {
		SPI_Flash_Read((CDV_INT08U*)&no, g_line.resAddr + shift, 1);//读取资源代号
		shift += 1;
		
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 2);//读取资源脚本大小
	  
		shift += 2;
		if(no == resNo)
			break;
		
		shift += size;
	}
	
	if(g_line.resAddr + shift >= resEndAddr)
		return 0;//没找到
	
	if(2 == ctrl)
		return g_line.resAddr + shift-2;//资源脚本大小地址
	
	resEndAddr = g_line.resAddr + shift + size;
	
	size = 0;
	SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 1);//读取资源名称长度
	shift += 1;
	shift += size;
	
	//匹配par
	while(g_line.resAddr + shift < resEndAddr) {
		SPI_Flash_Read((CDV_INT08U*)&par, g_line.resAddr + shift, 1);//读取参数代号
		shift += 1;
		
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 2);//读取参数大小
	  shift += 2;
		if(par == resPar)
			break;
		
		shift += size;
	}
	
	if(g_line.resAddr + shift >= resEndAddr)
		return 0;//没找到
	
	if(3 == ctrl)
		return g_line.resAddr + shift-2;//返回参数大小地址
	
	return 0;
	
}
/**
  * @brief  获取生产线信息
  *  
  * @param  void
  *   
  * @retval 返回值说明
  *
  * @note   从flash中获取，并填充g_line，，存储格式必须小端
  */

RET_STATUS GetLineInfo(const CDV_INT08U no) {
	CDV_INT08U i;
	CDV_INT08U type , num;
//	CDV_INT16U len = 0, crc;
	CDV_INT32U size = 0;//脚本大小，供跳转
	CDV_INT32U shift = 0;//偏移起始地址的字节数
	CDV_INT32U lineAddr = 0;//生产线的起始地址
	CDV_INT32U lineEndAddr = 0;//生产线的结束地址
	CDV_INT32U lineResAddr = 0;//生产线的资源地址
	CDV_INT32U lineWorkAddr = 0;//生产线的资源地址
	CDV_INT32U lineAlarmAddr = 0;//生产线的报警地址
	CDV_INT32U lineMapAddr = 0;//生产线的映射地址
	CDV_INT32U lineLen = 0;//生产线的长度
	//APP_SET_NUM = 0; //发送时的工人项计数
	ClearLineInfo();
	
	if(OPT_FAILURE == LineCrcChk())
		return OPT_FAILURE;
	
	//开始从flash中提取信息
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR, 4);//读取所有生产线脚本大小
	lineLen = size;
	shift += 4;
	size = 0;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//读取生产线个数
	shift += 1;
	//定位生产线
	i=0;
	size = 0;
	do{
		shift += size;//定位到下一条生产线
		lineAddr = __LINE_ADDR + shift;//保存当前生产线起始地址
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//读取生产线代号
//		shift += 1;
		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//读取生产线大小
		shift += 4;
	}while(i++ < no);
	size = 0;
//	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//读取生产线名称长度
//	shift += 1;
//	shift += size;
	lineResAddr = __LINE_ADDR + shift;//得到生产线资源地址
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//读取生产线资源脚本大小
	shift += 4;
	shift += size;
	lineMapAddr = __LINE_ADDR + shift;//得到生产线映射地址
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//读取生产线映射脚本大小
	shift += 4;
	shift += size;
	lineAlarmAddr = __LINE_ADDR + shift;//得到生产线报警地址
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//读取生产线报警脚本大小
	shift += 4;
	shift += size;
	lineWorkAddr = __LINE_ADDR + shift;
	//开始工人脚本信息读取
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//读取生产线工人脚本大小
	shift += 4;
	lineEndAddr = __LINE_ADDR + shift + size;//得到生产线的结束地址
	SPI_Flash_Read((CDV_INT08U*)&num, __LINE_ADDR + shift, 1);//读取工人种数
	shift += 1;
	//SRP_NUM_RUN = num;
	//工人信息读取
	while(__LINE_ADDR + shift < lineEndAddr  && shift < lineLen) {
		SPI_Flash_Read((CDV_INT08U*)&type, __LINE_ADDR + shift, 1);//读取工人种类代号
	  shift += 1;
		AddConfigAddr(__LINE_ADDR + shift);//添加工人配置起始地址，前两字节是大小
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 2);//读取工人配置大小
	  shift += 2;
		shift += size;
		AddWorkerAddr(__LINE_ADDR + shift);//添加工人起始地址，前两字节是大小
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 2);//读取工人脚本大小
	  shift += 2;
		shift += size;
	}
	
	//判断工人数是否一致，一致保存，不一致清空
	
	if(g_line.workerAddr.len == num && g_line.configAddr.len == num) {
		g_line.addr = lineAddr;
		g_line.endAddr = lineEndAddr;//可作为生产线1应用层脚本大小(4B)存储的起始地址
		g_line.resAddr = lineResAddr;
		g_line.workAddr = lineWorkAddr;
		g_line.AlarmAddr = lineAlarmAddr;
		g_line.mapAddr = lineMapAddr;
		g_line.no = no;
		g_line.init = 1;
		return OPT_SUCCESS;
	}
	
	ClearLineInfo();
	return OPT_FAILURE;
	
}
/**
  * @brief  接收脚本crc检验
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   
  */
RET_STATUS LineCrcChk(void) {
	CDV_INT08U ch;
	CDV_INT16U crc = 0xFFFF , tmpCrc;
	CDV_INT32U i ;
	CDV_INT32U addr = __LINE_ADDR;
	CDV_INT32U len;
	
	SPI_Flash_Read((CDV_INT08U*)&len, __LINE_ADDR, 4);//读取所有生产线脚本大小
	
	
	if(len > LINE_MAX_LEN - 6 || len <= 2)
		return OPT_FAILURE;
	
	len += 6;//包括crc的完整生产线大小
	for(i = 0; i< len - 2; i++) {
		SPI_Flash_Read(&ch, i + addr, 1);
		crc = oneCRC16(ch, crc); 
	}
	
	SPI_Flash_Read(&ch, i + addr, 1);
	tmpCrc = ch;
	SPI_Flash_Read(&ch, i+1 + addr, 1);
	tmpCrc |= ch<<8;
	
	if(crc == tmpCrc) {
		return OPT_SUCCESS;
	} else {
		return OPT_FAILURE;
	}
}


/**
  * @brief  发送flash内容
  *  
  * @param  uartNo     串口号
  *   
  * @retval 返回值说明
  *
  * @note   
  */
  
#define FLASH_SEND_LEN 0x400
void Flash_Send(const CDV_INT32U addr , const CDV_INT32U len, CDV_INT08U uartNo){
	CDV_INT32U sendPt = 0;
	CDV_INT08U* sendBuf = UserMemPtr(CCM_SEND_BUF);//[FLASH_SEND_LEN];
	
	while(sendPt < len){
		if(sendPt + FLASH_SEND_LEN > len) {
			SPI_Flash_Read(sendBuf, addr + sendPt, len - sendPt);
			AddTxNoCrc(sendBuf ,len - sendPt ,uartNo );
		}
		else{
			SPI_Flash_Read(sendBuf, addr + sendPt, FLASH_SEND_LEN);
			AddTxNoCrc(sendBuf ,FLASH_SEND_LEN ,uartNo );
		}
		sendPt += FLASH_SEND_LEN;
	}
	
}
