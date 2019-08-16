/**
  ******************************************************************************
  * @file    /Var.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-2
  * @brief   a package of var function
  * 
@verbatim  
20150106�����Ӳ����ں͵ȴ�������
               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "Var.h"
	
	#define DW_CDV_VAR(A) g_var[A] 
	//CDV_INT32S g_var[CDV_VAR_NUM] = {0};
	CDV_INT32S* g_var = (CDV_INT32S*)g_modbusReg.reg;
	
	//��������
//	void ValAdd(CDV_INT32U no, CDV_INT32S num);
//	void ValSub(CDV_INT32U no, CDV_INT32S num);
//	void ValMul(CDV_INT32U no, CDV_INT32S num);
//	void ValDiv(CDV_INT32U no, CDV_INT32S num);

//	void ValAnd(CDV_INT32U no, CDV_INT32S num);
//	void ValOr (CDV_INT32U no, CDV_INT32S num);
//	void ValNot(CDV_INT32U no, CDV_INT32S num);
//	void ValGre(CDV_INT32U no, CDV_INT32S num);
//	void ValEqu(CDV_INT32U no, CDV_INT32S num);
//	void ValNotEqu(CDV_INT32U no, CDV_INT32S num);
//	void ValLes(CDV_INT32U no, CDV_INT32S num);
//	RET_STATUS ValWaitGre(CDV_INT32U no, CDV_INT32S num);
//	RET_STATUS ValWaitEqu(CDV_INT32U no, CDV_INT32S num);
//	RET_STATUS ValWaitNotEqu(CDV_INT32U no, CDV_INT32S num);
//	RET_STATUS ValWaitLes(CDV_INT32U no, CDV_INT32S num);
//	void ValMod(CDV_INT32U no, CDV_INT32S num);
//	
//	
//	void ValParRead(CDV_INT32U no , CDV_INT32U flag , CDV_INT08U* buf , CDV_INT08U* len);
//	
//	

/**
  * @brief  ������ֵ
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValSet(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) = num;
}
/**
  * @brief  ������ֵ
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
CDV_INT32S VarGet(CDV_INT32U no) {
	ASSERT(no < CDV_VAR_NUM);
	return DW_CDV_VAR(no);
}
/**
  * @brief  ������ֵflash
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValFlashSet(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) = num;
#if USE_PVD == 0u && USE_EXTI_POWER_OFF == 0u
	SPI_Flash_Write((CDV_INT08U *)&num, VAR_ADDR(no), 4);
#endif
}
/**
  * @brief  ���б�����ֵflash
  *  
  * @param  no��ʼ����
						num������������
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValToFlash(CDV_INT32U no, CDV_INT16U num) {
	ASSERT(no + num <= CDV_VAR_NUM);
	
#if USE_PVD == 0u && USE_EXTI_POWER_OFF == 0u
	SPI_Flash_Write((CDV_INT08U *)&DW_CDV_VAR(no), VAR_ADDR(no), (num<<2));
#endif
}
/**
  * @brief  ���б�����ֵflash
  *  
  * @param  no��ʼ����
						num������������
  *   
  * @retval 
  *
  * @note   ���������
  */
void FlashToVal(CDV_INT32U no, CDV_INT16U num) {
	ASSERT(no + num <= CDV_VAR_NUM);
	SPI_Flash_Read((CDV_INT08U *)&DW_CDV_VAR(no), VAR_ADDR(no), (num<<2));
}
/**
  * @brief  ������flash
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
CDV_INT32S ValFlashGet(CDV_INT32U no) {
	CDV_INT32S num;
	ASSERT(no < CDV_VAR_NUM);
	SPI_Flash_Read((CDV_INT08U *)&num, VAR_ADDR(no), 4);
	DW_CDV_VAR(no) = num;
	return num;
}
/**
  * @brief  ������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValAdd(CDV_INT32U no, CDV_INT32S num) {
	//CDV_INT32S val = R_VAR(no) + num;
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) += num;
	//WRITE_VAR( no , val);
}
/**
  * @brief  ������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValSub(CDV_INT32U no, CDV_INT32S num) {
	//CDV_INT32S val = R_VAR(no) - num;
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) -= num;
	//WRITE_VAR( no , val);
}
/**
  * @brief  ������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValMul(CDV_INT32U no, CDV_INT32S num) {
//	CDV_INT32S val = R_VAR(no) * num;
//	WRITE_VAR( no , val);
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) *= num;
}
/**
  * @brief  ������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValDiv(CDV_INT32U no, CDV_INT32S num) {
//	CDV_INT32S val;
	ASSERT(no < CDV_VAR_NUM);
//	ASSERT(0 != num);
	if(!num)
		return;
//	val = R_VAR(no) / num;
//	WRITE_VAR( no , val);
	DW_CDV_VAR(no) /= num;
}
/**
  * @brief  ������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValAnd(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) = DW_CDV_VAR(no) && num;
}
/**
  * @brief  ������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValOr(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) || num;
}
/**
  * @brief  ������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValNot(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = !DW_CDV_VAR(no);
}
/**
  * @brief  ��������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValGre(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) > num;
}
/**
  * @brief  ��������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValEqu(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) == num;
}
/**
  * @brief  ����������
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValNotEqu(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) != num;
}

/**
  * @brief  ����С��
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValLes(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) < num;
}

/**
  * @brief  �����ȴ�����
  *  
  * @param  no     Ҫ��������Ϣ��
  *   
  * @retval OPT_SUCCESS�ȵ���
						OPT_FAILURE��û�ȵ�
  *
  * @note   ���������
  */
RET_STATUS ValWaitGre(CDV_INT32U no, CDV_INT32S num){
  RET_STATUS ret = OPT_FAILURE;
	ASSERT(no < CDV_VAR_NUM);
	
	if(DW_CDV_VAR(no) > num){
		ret = OPT_SUCCESS;
	}
	
	return ret;
}
/**
  * @brief  �����ȴ�����
  *  
  * @param  no     Ҫ��������Ϣ��
  *   
  * @retval 
  *
  * @note   ���������
  */
RET_STATUS ValWaitEqu(CDV_INT32U no, CDV_INT32S num){
  RET_STATUS ret = OPT_FAILURE;
	ASSERT(no < CDV_VAR_NUM);
	
	if(DW_CDV_VAR(no) == num){
		ret = OPT_SUCCESS;
	}
	return ret;
}
/**
  * @brief  �����ȴ�������
  *  
  * @param  no     Ҫ��������Ϣ��
  *   
  * @retval 
  *
  * @note   ���������
  */
RET_STATUS ValWaitNotEqu(CDV_INT32U no, CDV_INT32S num){
  RET_STATUS ret = OPT_FAILURE;
	ASSERT(no < CDV_VAR_NUM);
	
	if(DW_CDV_VAR(no) != num){
		ret = OPT_SUCCESS;
	}
	return ret;
}
/**
  * @brief  �����ȴ�С��
  *  
  * @param  no     Ҫ��������Ϣ��
  *   
  * @retval 
  *
  * @note   ���������
  */
RET_STATUS ValWaitLes(CDV_INT32U no, CDV_INT32S num){
  RET_STATUS ret = OPT_FAILURE;
	ASSERT(no < CDV_VAR_NUM);
	
	if(DW_CDV_VAR(no) < num){
		ret = OPT_SUCCESS;
	}
	return ret;
}
/**
  * @brief  ����
  *  
  * @param  no     Ҫ�����ı�����
  *   
  * @retval 
  *
  * @note   ���������
  */
void ValMod(CDV_INT32U no, CDV_INT32S num){
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) %= num;
}

/**
  * @brief  Val������ȡ
  *  
  * @param  no     Ҫ���õ�I��
  *         flag   ���õĲ������
						buf    ����buf
  *   
  * @retval ��
  *
  * @note   
  */
void ValParRead(CDV_INT32U no , CDV_INT32U flag , CDV_INT08U* buf , CDV_INT08U* len) {
	*len = 0;
	ASSERT(no < CDV_VAR_NUM);
	
	//if (flag & 0x0001) 
	{
		buf[(*len) ++] = (CDV_INT08U)(DW_CDV_VAR(no));
		//(*len) ++;
		buf[(*len) ++] = (CDV_INT08U)(DW_CDV_VAR(no) >> 8);
		buf[(*len) ++] = (CDV_INT08U)(DW_CDV_VAR(no) >> 16);
		buf[(*len) ++] = (CDV_INT08U)(DW_CDV_VAR(no) >> 24);
	}
}

/**
  * @brief  �����ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   ��Դ�ţ�00-FF��	00��������		FF	uint32��ֵ��λ	uint32��ֵ	uint32��ֵ	uint32��ֵ��λ					�ַ���
																					ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	
														01�������޸�	FF	uint32��ֵ��λ	uint32��ֵ	uint32��ֵ	uint32��ֵ��λ					
																					ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	
														02��ֵ				FF	uint32��ֵ��λ	uint32��ֵ	uint32��ֵ	uint32��ֵ��λ					
																					ö�ٺţ�00-03									
														03�Լ�				FF	uint32��ֵ��λ	uint32��ֵ	uint32��ֵ	uint32��ֵ��λ					
														04�Լ�				FF	uint32��ֵ��λ	uint32��ֵ	uint32��ֵ	uint32��ֵ��λ					
            ʹ�õ�ȫ�ֱ���R_APP_SET_NUM�������������ű����еģ��ʲ��ᷢ����ͻ
  */

RET_STATUS ValCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg) { 
	CDV_INT08U opt , type;
	CDV_INT32U no;
	CDV_INT32S num;
	RET_STATUS ret = OPT_SUCCESS;
	no  = *(CDV_INT32U*)(rxBuf);
	opt  = rxBuf[4];
	type = rxBuf[5];
	
	if(no >= CDV_VAR_NUM)
		return OPT_FAILURE;
	
	if (0xff == type) {//��ֵ��
		num = *(CDV_INT32S*)(rxBuf + 6);
	} else if (0xfe == type){//������
		if(*(CDV_INT32U*)(rxBuf + 6) >= CDV_VAR_NUM)
		  return OPT_FAILURE;
		num = DW_CDV_VAR(*(CDV_INT32U*)(rxBuf + 6));
	} else {
		num = type;
	}
  
	switch(opt) {
		case 0x02:/*��ֵ*/
			num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
			ValSet(no , num);
			break;
		case 0x03:/*��*/
			ValAdd(no , num);
			break;
		case 0x04:/*��*/
			ValSub(no , num);
			break;
		case 0x05:/*��*/
			ValMul(no , num);
			break;
		case 0x06:/*��*/
			ValDiv(no , num);
			break;
		case 0x07:/*��*/
			ValAnd(no , num);
			break;
		case 0x08:/*��*/
			ValOr(no , num);
			break;
		case 0x09:/*��*/
			ValNot(no , num);
			break;
		case 0x0A:/*����*/
			ValGre(no , num);
			break;
		case 0x0B:/*����*/
			ValEqu(no , num);
			break;
		case 0x0C:/*С��*/
			ValLes(no , num);
			break;
		case 0x0D:/*�ȴ�����*/
			ret = ValWaitGre(no , num);
			break;
		case 0x0E:/*�ȴ�����*/
			ret = ValWaitEqu(no , num);
			break;
		case 0x0F:/*�ȴ�С��*/
			ret = ValWaitLes(no , num);
			break;
		case 0x10:/*��ȡ*/
			//if(0xFF == type) 
			{
				
	      CDV_INT08U tmpPar[VAL_PAR_NUM] = {0} , tmpLen = 0;
			  ValParRead(no , /*rxBuf[3]<<8 | rxBuf[4]*/num , tmpPar , &tmpLen);
				ResRequest(arg->buf, arg->len, tmpPar, tmpLen, arg, RC_CRC);//ResParaRequest(rxBuf, rxLen , tmpPar, tmpLen,arg->uart);
			}
			break;
		case 0x11:/*������*/
			ValNotEqu(no , num);
			break;
		case 0x12:/*�ȴ�������*/
			ret = ValWaitNotEqu(no , num);
			break;
		case 0x13:/*����*/
			ValMod(no , num);
			break;
		case 0x14:/*����ֵ*/
			num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
			ValFlashSet(no , num);
			break;
		case 0x20:/*��������*/
			num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
			ValSet(no , num);
			break;
	  case 0xFF:/*��ѯ*/
			{
				CDV_INT08U tmpPar[VAL_PAR_NUM] = {0} , tmpLen = 0;
				ValParRead(no , /*rxBuf[3]<<8 | rxBuf[4]*/num , tmpPar , &tmpLen);
				ResRequest(arg->buf, arg->len, tmpPar, tmpLen, arg, RC_CRC);//ResParaRequest(rxBuf, rxLen , tmpPar, tmpLen,arg->uart);
			}
			break;
		default:
			break;
		
	}

	return ret;
}