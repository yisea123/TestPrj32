/**
  ******************************************************************************
  * @file    /user_define.c 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   global variable 
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
	
#include "cdv_define.h"


CDV_STATUS g_cdvStat = CDV_FPGA_INIT/*,g_tmpCdvStat = CDV_NULL*/;                      /*CDV״̬���*/
CDV_INT08U g_nowDoRes = 0;   /*������ڽ������Դ��*/
SCRIPT_INFO g_scriptInfo;                               /*CDV�ű�״̬�ṹ��*/
//DEBUG_DATA g_debugData;
/*�����ʱ��*/
CDV_INT32U tm1Re = 0,tm2Re = 0,tm3Re = 0;
OS_TMR 	tmr1;		//��ʱ��1//���ڴ���1��
OS_TMR	tmr2;		//��ʱ��2//����wifi
OS_TMR	tmr3;		//��ʱ��3//���ڴ��ڻ���
/*������ƿ�*/
OS_TCB StartTaskTCB;
OS_TCB UsartRecvTaskTCB;
OS_TCB UsartSendTaskTCB;
OS_TCB CdvRefreshTaskTCB;

//CDV_INT08U g_whichUart; //ָʾ�����߳��е�ǰ�����Ǵ��ĸ����ڷ�������
/**/
OS_SEM TX_SEM, MOTO_SEM,MEM_SEM;
OS_SEM GENERAL_SERIAL_SEM;
OS_SEM TCP_TX_SEM;

	//TEST
double RPressureData[100] = {0};
int RPressureCnt = 0;

void CmdArgDelete(CMD_ARG *arg)
{
	DELETE(arg->reqbuf);
	DELETE(arg->specbuf);
	INIT_CLEAR(*arg);
}
