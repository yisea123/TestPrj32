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
  * @brief  �����������Ϣ
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
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
  * @brief  ���빤�˵�ַ
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��˳�����
  */

void AddWorkerAddr(const CDV_INT32U addr){
	ArrayU32Add(&g_line.workerAddr, addr);
}
/**
  * @brief  ���빤�����õ�ַ
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��˳�����
  */

void AddConfigAddr(const CDV_INT32U addr){
	ArrayU32Add(&g_line.configAddr, addr);
}
/**
  * @brief  ��ȡָ�����˵�ַ
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��0��ʼ
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
  * @brief  ��ȡָ���������õ�ַ
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��0��ʼ
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
  * @brief  ��ȡָ����Դ������ַ
  *  
  * @param  void
						ctrl 1:restype
								 2:resNo
								 3:resPar
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��0��ʼ���洢��ʽ����С��
  */
CDV_INT32U GetResAddr(const CDV_INT08U resType){
	CDV_INT08U no, type, par;
	CDV_INT32U size = 0;//�ű���С������ת
	CDV_INT32U shift = 0;//ƫ����ʼ��ַ���ֽ���
	CDV_INT32U resEndAddr = 0;//�����ߵ���Դ������ַ
	
	if(!g_line.init) {
		return 0;
	}
	
	
	SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr, 4);//��ȡ��������Դ�ű���С
	shift = 4;
	resEndAddr = g_line.resAddr + shift + size;
	shift += 1;//����������
	//ƥ��type
	while(g_line.resAddr + shift < resEndAddr) {
		SPI_Flash_Read((CDV_INT08U*)&type, g_line.resAddr + shift, 1);//��ȡ��Դ�������
		shift += 1;
		
		
		SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 4);//��ȡ����ű���С
	  
		shift += 4;
		if(type == resType)
			break;
		
		shift += size;
	}
	
	if(g_line.resAddr + shift >= resEndAddr)
		return 0;//û�ҵ�
	else
		return g_line.resAddr + shift-4;//��������ű���С��ַ
	
//	return 0;
	
}
/**
  * @brief  ��ȡָ����Դ������ַ
  *  
  * @param  void
						ctrl 1:restype
								 2:resNo
								 3:resPar
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��0��ʼ���洢��ʽ����С��
  */
CDV_INT32U GetResAddr0(const CDV_INT08U resType, const CDV_INT08U resNo, const CDV_INT08U resPar, const CDV_INT08U ctrl){
	CDV_INT08U no, type, par;
	CDV_INT32U size = 0;//�ű���С������ת
	CDV_INT32U shift = 0;//ƫ����ʼ��ַ���ֽ���
	CDV_INT32U resEndAddr = 0;//�����ߵ���Դ������ַ
	
	if(!g_line.init) {
		return 0;
	}
	
	if(ctrl <1 || ctrl >3)
		return g_line.resAddr;
	
	
	SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr, 4);//��ȡ��������Դ�ű���С
	shift = 4;
	resEndAddr = g_line.resAddr + shift + size;
	shift += 1;
	//ƥ��type
	while(g_line.resAddr + shift < resEndAddr) {
		SPI_Flash_Read((CDV_INT08U*)&type, g_line.resAddr + shift, 1);//��ȡ��Դ�������
		shift += 1;
		
		
		SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 4);//��ȡ����ű���С
	  
		shift += 4;
		if(type == resType)
			break;
		
		shift += size;
	}
	
	if(g_line.resAddr + shift >= resEndAddr)
		return 0;//û�ҵ�
	
	if(1 == ctrl)
		return g_line.resAddr + shift-4;//��������ű���С��ַ
	
	resEndAddr = g_line.resAddr + shift + size;
	shift += 1;
	
	//ƥ��no
	while(g_line.resAddr + shift < resEndAddr) {
		SPI_Flash_Read((CDV_INT08U*)&no, g_line.resAddr + shift, 1);//��ȡ��Դ����
		shift += 1;
		
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 2);//��ȡ��Դ�ű���С
	  
		shift += 2;
		if(no == resNo)
			break;
		
		shift += size;
	}
	
	if(g_line.resAddr + shift >= resEndAddr)
		return 0;//û�ҵ�
	
	if(2 == ctrl)
		return g_line.resAddr + shift-2;//��Դ�ű���С��ַ
	
	resEndAddr = g_line.resAddr + shift + size;
	
	size = 0;
	SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 1);//��ȡ��Դ���Ƴ���
	shift += 1;
	shift += size;
	
	//ƥ��par
	while(g_line.resAddr + shift < resEndAddr) {
		SPI_Flash_Read((CDV_INT08U*)&par, g_line.resAddr + shift, 1);//��ȡ��������
		shift += 1;
		
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, g_line.resAddr + shift, 2);//��ȡ������С
	  shift += 2;
		if(par == resPar)
			break;
		
		shift += size;
	}
	
	if(g_line.resAddr + shift >= resEndAddr)
		return 0;//û�ҵ�
	
	if(3 == ctrl)
		return g_line.resAddr + shift-2;//���ز�����С��ַ
	
	return 0;
	
}
/**
  * @brief  ��ȡ��������Ϣ
  *  
  * @param  void
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��flash�л�ȡ�������g_line�����洢��ʽ����С��
  */

RET_STATUS GetLineInfo(const CDV_INT08U no) {
	CDV_INT08U i;
	CDV_INT08U type , num;
//	CDV_INT16U len = 0, crc;
	CDV_INT32U size = 0;//�ű���С������ת
	CDV_INT32U shift = 0;//ƫ����ʼ��ַ���ֽ���
	CDV_INT32U lineAddr = 0;//�����ߵ���ʼ��ַ
	CDV_INT32U lineEndAddr = 0;//�����ߵĽ�����ַ
	CDV_INT32U lineResAddr = 0;//�����ߵ���Դ��ַ
	CDV_INT32U lineWorkAddr = 0;//�����ߵ���Դ��ַ
	CDV_INT32U lineAlarmAddr = 0;//�����ߵı�����ַ
	CDV_INT32U lineMapAddr = 0;//�����ߵ�ӳ���ַ
	CDV_INT32U lineLen = 0;//�����ߵĳ���
	//APP_SET_NUM = 0; //����ʱ�Ĺ��������
	ClearLineInfo();
	
	if(OPT_FAILURE == LineCrcChk())
		return OPT_FAILURE;
	
	//��ʼ��flash����ȡ��Ϣ
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR, 4);//��ȡ���������߽ű���С
	lineLen = size;
	shift += 4;
	size = 0;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//��ȡ�����߸���
	shift += 1;
	//��λ������
	i=0;
	size = 0;
	do{
		shift += size;//��λ����һ��������
		lineAddr = __LINE_ADDR + shift;//���浱ǰ��������ʼ��ַ
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//��ȡ�����ߴ���
//		shift += 1;
		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ�����ߴ�С
		shift += 4;
	}while(i++ < no);
	size = 0;
//	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//��ȡ���������Ƴ���
//	shift += 1;
//	shift += size;
	lineResAddr = __LINE_ADDR + shift;//�õ���������Դ��ַ
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ��������Դ�ű���С
	shift += 4;
	shift += size;
#if USE_CASCADE_MAP == 1u
	lineMapAddr = __LINE_ADDR + shift;//�õ�������ӳ���ַ
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ������ӳ��ű���С
	shift += 4;
	shift += size;
#endif
	lineAlarmAddr = __LINE_ADDR + shift;//�õ������߱�����ַ
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ�����߱����ű���С
	shift += 4;
	shift += size;
	lineWorkAddr = __LINE_ADDR + shift;
	//��ʼ���˽ű���Ϣ��ȡ
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ�����߹��˽ű���С
	shift += 4;
	lineEndAddr = __LINE_ADDR + shift + size;//�õ������ߵĽ�����ַ
	SPI_Flash_Read((CDV_INT08U*)&num, __LINE_ADDR + shift, 1);//��ȡ��������
	shift += 1;
	//SRP_NUM_RUN = num;
	//������Ϣ��ȡ
	while(__LINE_ADDR + shift < lineEndAddr  && shift < lineLen) {
		SPI_Flash_Read((CDV_INT08U*)&type, __LINE_ADDR + shift, 1);//��ȡ�����������
	  shift += 1;
		AddConfigAddr(__LINE_ADDR + shift);//��ӹ���������ʼ��ַ��ǰ���ֽ��Ǵ�С
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 2);//��ȡ�������ô�С
	  shift += 2;
		shift += size;
		AddWorkerAddr(__LINE_ADDR + shift);//��ӹ�����ʼ��ַ��ǰ���ֽ��Ǵ�С
		size = 0;
		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 2);//��ȡ���˽ű���С
	  shift += 2;
		shift += size;
	}
	
	//�жϹ������Ƿ�һ�£�һ�±��棬��һ�����
	
	if(g_line.workerAddr.len == num && g_line.configAddr.len == num) {
		g_line.addr = lineAddr;
		g_line.endAddr = lineEndAddr;//����Ϊ������1Ӧ�ò�ű���С(4B)�洢����ʼ��ַ
		g_line.resAddr = lineResAddr;
		g_line.workAddr = lineWorkAddr;
		g_line.AlarmAddr = lineAlarmAddr;
#if USE_CASCADE_MAP == 1u
		g_line.mapAddr = lineMapAddr;
#endif
		g_line.no = no;
		g_line.init = 1;
		return OPT_SUCCESS;
	}
	
	ClearLineInfo();
	return OPT_FAILURE;
	
}
/**
  * @brief  ���սű�crc����
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
	
	SPI_Flash_Read((CDV_INT08U*)&len, __LINE_ADDR, 4);//��ȡ���������߽ű���С
	
	
	if(len > LINE_MAX_LEN - 6 || len <= 2)
		return OPT_FAILURE;
	
	len += 6;//����crc�����������ߴ�С
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
  * @brief  ����flash����
  *  
  * @param  uartNo     ���ں�
  *   
  * @retval ����ֵ˵��
  *
  * @note   
  */
  
#define FLASH_SEND_LEN 0x400
void Flash_Send(const CDV_INT32U addr , const CDV_INT32U len, CMD_ARG *arg){
	CDV_INT32U sendPt = 0;
	CDV_INT08U* sendBuf = UserMemPtr(CCM_SEND_BUF);//[FLASH_SEND_LEN];
	
	while(sendPt < len){
		if(sendPt + FLASH_SEND_LEN > len) {
			SPI_Flash_Read(sendBuf, addr + sendPt, len - sendPt);
			AddTxNoCrcPlus(sendBuf ,len - sendPt ,arg );
		}
		else{
			SPI_Flash_Read(sendBuf, addr + sendPt, FLASH_SEND_LEN);
			AddTxNoCrcPlus(sendBuf ,FLASH_SEND_LEN ,arg );
		}
		sendPt += FLASH_SEND_LEN;
	}
	
}


/**
  * @brief  ��ȡ���̽ű��汾
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */

RET_STATUS GetLineVersion(CDV_INT08U** buf, CDV_INT32U *len) {
	CDV_INT08U i;
	//CDV_INT08U type , num;
	CDV_INT32U size = 0;//�ű���С������ת
	CDV_INT32U shift = 0;//ƫ����ʼ��ַ���ֽ���

	//CDV_INT32U lineLen = 0;//�����ߵĳ���
	
	if(OPT_FAILURE == LineCrcChk() || *buf != NULL)
		return OPT_FAILURE;
	
	//��ʼ��flash����ȡ��Ϣ
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR, 4);//��ȡ���������߽ű���С
	//lineLen = size;
	shift += 4;
	size = 0;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//��ȡ�����߸���
	shift += 1;
	//��λ������
	i=0;
	size = 0;
	do{
		shift += size;//��λ����һ��������
		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ�����ߴ�С
		shift += 4;
	}while(i++ < 0);
	size = 0;
//	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//��ȡ���������Ƴ���
//	shift += 1;
//	shift += size;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ��������Դ�ű���С
	shift += 4;
	shift += size;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ������ӳ��ű���С
	shift += 4;
	shift += size;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ�����߱����ű���С
	shift += 4;
	shift += size;	
	
	//TODO��ģ��
	NEWCH(* buf, *len = size);
	SPI_Flash_Read(* buf, __LINE_ADDR + shift, size);//��ȡ���̽ű��汾
		
	return OPT_SUCCESS;
}

/**
  * @brief  ��ȡӦ�ò�汾
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */

RET_STATUS GetYYCVersion(CDV_INT08U** buf, CDV_INT32U *len) {
	CDV_INT08U i;
	//CDV_INT08U type , num;
	CDV_INT32U size = 0;//�ű���С������ת
	CDV_INT32U shift = 0;//ƫ����ʼ��ַ���ֽ���

	//CDV_INT32U lineLen = 0;//�����ߵĳ���
	
	if(OPT_FAILURE == LineCrcChk() || *buf != NULL)
		return OPT_FAILURE;
	
	//��ʼ��flash����ȡ��Ϣ
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR, 4);//��ȡ���������߽ű���С
	//lineLen = size;
	shift += 4;
	size = 0;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//��ȡ�����߸���
	shift += 1;
	//��λ������
	i=0;
	size = 0;
	do{
		shift += size;//��λ����һ��������
		SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ�����ߴ�С
		shift += 4;
	}while(i++ < 0);
	size = 0;
//	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 1);//��ȡ���������Ƴ���
//	shift += 1;
//	shift += size;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ��������Դ�ű���С
	shift += 4;
	shift += size;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ������ӳ��ű���С
	shift += 4;
	shift += size;
	SPI_Flash_Read((CDV_INT08U*)&size, __LINE_ADDR + shift, 4);//��ȡ�����߱����ű���С
	shift += 4;
	shift += size;	
	
	//TODO��ģ��
	NEWCH(* buf, *len = size);
	SPI_Flash_Read(* buf, __LINE_ADDR + shift, size);//��ȡ���̽ű��汾
		
	return OPT_SUCCESS;
}
