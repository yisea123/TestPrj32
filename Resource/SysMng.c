/**
  ******************************************************************************
  * @file    /SysMng.c 
  * @author  MMY
  * @version V0.0.1
  * @date    6-14-2016
  * @brief   a package of SysMng function
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "SysMng.h"


/**
  * @brief  ����APP�·���ϵͳ�������
  *
  * @param  rxBuf     �������ַ���ָ��
  *         rxlen       �������ַ����ĳ���
  *
  * @retval ����ֵ˵��
  *
  * @note   
  */
RET_STATUS SysCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg) { 

	CDV_INT08U opt /*no , , type , strLen*/;
  
	//no   = rxBuf[0];
	opt  = rxBuf[1];
	//type = rxBuf[2];

	switch(opt) {
		case 0x00:/*״̬�л�*/
			
		  ChangeToCdvStat(rxBuf[3]);
			break;
		case 0x01:/*״̬��ѯ*/
		  ResRequest(arg->buf, arg->len, (CDV_INT08U*)&g_cdvStat, 1, arg, RC_CRC);
			break;
		case 0x02:/*����*/
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)NULL, 0, arg, RC_CRC);
			delay_ms(500);
		  ResetCdv();
			break;
		default:
			break;
		
	}
	return OPT_SUCCESS;
}
/**
  * @brief  ����CDV
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
void ResetCdv(void){
	//Log_Write("Reset Cdv" , LOG_EVENT);
	
	CLI();
	NVIC_SystemReset();
}
/**
  * @brief  �ı�CDV״̬
  *  
  * @param  stat     Ŀ��״̬
  *   
  * @retval 
  *
  * @note   
  */
void ChangeToCdvStat(CDV_INT08U stat){
	OS_ERR err;
	char tmp[50]={0};
	
	sprintf(tmp , "Change stat to %d" 
			,stat
			);
	Log_Write(tmp , LOG_EVENT);
	
	switch(stat){ //Ŀ��״̬
	  case CDV_ONLINE://ONLINE
			switch(g_cdvStat) {
				case CDV_START:
				case CDV_STPAUSE:
				case CDV_RUN:
				case CDV_PAUSE:
					g_cdvStat = CDV_STOP;
					break;
				default:
					break;
			}
		case CDV_RUN:
			switch(g_cdvStat) {
				case CDV_ONLINE:
					g_cdvStat = CDV_START;
					break;
				default:
					break;
			}
		default:
			break;
	}
	OSTaskResume((OS_TCB*)&WorkerManageTaskTCB,&err);	
}

