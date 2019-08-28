/**
  ******************************************************************************
  * @file    /Msg.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-2
  * @brief   a package of Msg function
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
	
	#include "Msg.h"



#define CH_CDV_MSG(A)          g_msg[A]
#define SET_MSG(A) do{\
	(CH_CDV_MSG((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);
#define RESET_MSG(A) do{\
	(CH_CDV_MSG((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);
#define READ_MSG(A) (((CH_CDV_MSG((A) >> 3)) & (0x01 <<((A) & 0x07)))? 1 :0) 

CDV_INT08U g_msg[4] = {0};

//extern  CDV_INT08U g_msg[4];
 
//void MsgSend(CDV_INT08U no);
//RET_STATUS MsgWait(CDV_INT08U no);



/**
  * @brief  ��Ϣ����
  *  
  * @param  no     Ҫ��������Ϣ��
  *   
  * @retval 
  *
  * @note   
  */
void MsgSend(CDV_INT32U no) {
	//OS_ERR err;

	SET_MSG(no);              /*������Ϣ*/
}
/**
  * @brief  ��Ϣ�ȴ�
  *  
  * @param  no     Ҫ��������Ϣ��
  *   
  * @retval 
  *
  * @note   
  */
RET_STATUS MsgWait(CDV_INT32U no){
	//OS_ERR err;
  RET_STATUS ret =OPT_SUCCESS;
	//while( 1 != MODBUS_READ_MSG(no) && CDV_STOP != g_cdvStat);/*�ȴ���Ϣ*/
	if( 1 != READ_MSG(no)){
		ret = OPT_FAILURE;
	} else {
	    RESET_MSG(no);            /*�ӵ���Ϣ������*/
	}
	return ret;
}

/**
  * @brief  ��Ϣ����
  * @param  
  * @retval 
  * @note   
  */
void ClearMsg(void){
	int i;
	
	for(i = 0 ; i < CDV_MSG_NUM;i++)
	{
		RESET_MSG(i);
	}
	
	return;
}

/**
  * @brief  ��Ϣ�ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   ��Դ�ţ�00-FF��	00����	ED									�ַ���
	                          01����	ED									
  */

RET_STATUS MsgCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg) {
	OS_ERR err;
	//CPU_SR_ALLOC();//OS_CRITICAL_ENTER()
	CDV_INT08U opt/* , type , strLen*/;
	CDV_INT32U no, val;
	RET_STATUS ret =OPT_SUCCESS;
	//CDV_INT32U num;
	//strLen = rxLen - 3;//���ĳ���
	no   =  *(CDV_INT32U*)(rxBuf);
	opt  = rxBuf[4];
	//type = rxBuf[2];
  OSMutexPend(&MSG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	switch(opt) {
		case 0x00:/*����*/
			MsgSend(no);
			break;
		case 0x01:/*����*/
			//if(1==debug)
					//RESET_MSG(no);
					ret = MsgWait(no);
			break;
		case 0xFF:/*��ѯ*/
			val = READ_MSG(no);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
		  break;
		default:
			break;
		
	}
	OSMutexPost (&MSG_SEM,OS_OPT_POST_NO_SCHED,&err);
//	if(OPT_FAILURE == ret) {
//		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 5ms
//	}
	return ret;
}
