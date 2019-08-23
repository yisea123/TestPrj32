/**
  ******************************************************************************
  * @file    /main.c 
  * @author  MMY
  * @version V2.0.0
  * @date    27-August-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "cdv_include.h"
void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�������CDV����

CPU_STK START_TASK_STK[START_STK_SIZE];/*�����ջ*/
void start_task(void *p_arg);/*������*/

CPU_STK USART_TASK_STK[USART_RECV_STK_SIZE];//�����ջ
void usart_recv_task(void *p_arg);

CPU_STK LED1_TASK_STK[USART_SEND_STK_SIZE];//�����ջ
void usart_send_task(void *p_arg);//������

CPU_STK FLOAT_TASK_STK[CDV_REFRESH_STK_SIZE];//�����ջ
void cdv_refresh_task(void *p_arg);//������



int main(void){
	CDV_INT08C fields[5] = "";
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init(168);                               //ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������

		
	//IAP_LoadApp();
	
	INTX_DISABLE();                                //���ж�,��ֹ�δ�ʱ���������ʼ���Ĵ���
	PeriphDriverInit();                            //��ʼ��
		
//	if(DIP_ON == READ_DIP_SW(2)) //����
//	{		
//		IAP_LoadApp();
//	}
		
	OSInit(&err);                                  //��ʼ�� UCOSIII
	INTX_ENABLE();                                 //���ж�	
	
	// OTA���
	SPI_Flash_Read((CDV_INT08U*)fields, OTA_ADDR, 3);
	SPI_Flash_Read((CDV_INT08U*)(&g_ota_ipaddr.addr), OTA_IP, 4);
	
	if(0 != strncmp(fields,"OTA",3))
	  StartMenu();
	
//	memset((void*)fields, 0, 3);
//	SPI_Flash_Write((CDV_INT08U*)fields, OTA_ADDR, 3);
	
#if USE_NPC_NET
  /* configure Ethernet (GPIOs, clocks, MAC, DMA) */ 
  ETH_BSP_Config();

  /* Initilaize the LwIP stack */
  LwIP_Init();

  /* Initialize webserver demo */
  tcp_server_init();
  //httpd_init();
	IAP_httpd_init();
#endif

	OS_CRITICAL_ENTER();                           //�����ٽ���
	/*������ʼ����*/
	OSTaskCreate((OS_TCB* )&StartTaskTCB,          //������ƿ�
		(CPU_CHAR* )"start task",                    //��������
		(OS_TASK_PTR )start_task,                    //������
		(void* )0,                                   //���ݸ��������Ĳ���
		(OS_PRIO )START_TASK_PRIO,                   //�������ȼ�
		(CPU_STK* )&START_TASK_STK[0],               //�����ջ����ַ
		(CPU_STK_SIZE )START_STK_SIZE/10,            //�����ջ�����λ
		(CPU_STK_SIZE )START_STK_SIZE,               //�����ջ��С
		(OS_MSG_QTY )0,                              //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ����Ϣ
		(OS_TICK )0,                                 //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
		(void* )0,                                   //�û�����Ĵ洢��
		(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
		(OS_ERR* )&err);                             //��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();                            //�˳��ٽ���
	OSStart(&err);                                 //���� UCOSIII
	while(1);
}
//��ʼ������
void start_task(void *p_arg){
	CDV_INT08U i;
	//CDV_INT32U size;
	OS_ERR err;
	CPU_SR_ALLOC();//OS_CRITICAL_ENTER()
	p_arg = p_arg;
	CPU_Init();
	
	#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err); //ͳ������
	#endif
	#ifdef CPU_CFG_INT_DIS_MEAS_EN //���ʹ���˲����жϹر�ʱ��
	CPU_IntDisMeasMaxCurReset();
	#endif
	#if OS_CFG_SCHED_ROUND_ROBIN_EN //��ʹ��ʱ��Ƭ��ת��ʱ��
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);//ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ 1 ��ϵͳʱ�ӽ��ģ��� 1*1=1ms
	#endif
	//�����ź���							
	OSSemCreate ((OS_SEM* )&TX_SEM, //ָ���ź���
								(CPU_CHAR* )"TX_SEM", //�ź�������
								(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);

	OSSemCreate ((OS_SEM* )&MOTO_SEM, //ָ���ź���
								(CPU_CHAR* )"MOTO_SEM", //�ź�������
								(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);
	OSSemCreate ((OS_SEM* )&MEM_SEM, //ָ���ź���
								(CPU_CHAR* )"MEM_SEM", //�ź�������
								(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);
								
	OSSemCreate ((OS_SEM* )&GENERAL_SERIAL_SEM, //ָ���ź���
								(CPU_CHAR* )"GENERAL_SERIAL_SEM", //�ź�������
								(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);			

	OSSemCreate ((OS_SEM* )&TCP_TX_SEM, //ָ���ź���
								(CPU_CHAR* )"TCP_TX_SEM", //�ź�������
								(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);		


	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
                (CPU_CHAR	*)"tmr1",		//��ʱ������
                (OS_TICK	 )0,			//0*1=0ms
                (OS_TICK	 )50,          //n*10ms=20ms 10ms=1/OS_CFG_TMR_TASK_RATE_HZ 
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����						
	//������ʱ��2
								
	OS_CRITICAL_ENTER(); //�����ٽ���
	//���� ���ڽ�������
	OSTaskCreate((OS_TCB* )&UsartRecvTaskTCB,
		(CPU_CHAR* )"usart recv task",
		(OS_TASK_PTR )usart_recv_task,
		(void* )0,
		(OS_PRIO )USART_RECV_TASK_PRIO,
		(CPU_STK* )&USART_TASK_STK[0],
		(CPU_STK_SIZE )USART_RECV_STK_SIZE/10,
		(CPU_STK_SIZE )USART_RECV_STK_SIZE,
		(OS_MSG_QTY )0,
		(OS_TICK )0,
		(void* )0,
		(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
		(OS_ERR* )&err);
	//���� ���ڷ�������
	OSTaskCreate((OS_TCB* )&UsartSendTaskTCB,
		(CPU_CHAR* )"usart send task",
		(OS_TASK_PTR )usart_send_task,
		(void* )0,
		(OS_PRIO )USART_SEND_TASK_PRIO,
		(CPU_STK* )&LED1_TASK_STK[0],
		(CPU_STK_SIZE )USART_SEND_STK_SIZE/10,
		(CPU_STK_SIZE )USART_SEND_STK_SIZE,
		(OS_MSG_QTY )0,
		(OS_TICK )0,
		(void* )0,
		(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
		(OS_ERR* )&err);
	//����CDVˢ������
	OSTaskCreate((OS_TCB* )&CdvRefreshTaskTCB,
		(CPU_CHAR* )"cdv refresh task",
		(OS_TASK_PTR )cdv_refresh_task,
		(void* )0,
		(OS_PRIO )CDV_REFRESH_TASK_PRIO,
		(CPU_STK* )&FLOAT_TASK_STK[0],
		(CPU_STK_SIZE )CDV_REFRESH_STK_SIZE/10,
		(CPU_STK_SIZE )CDV_REFRESH_STK_SIZE,
		(OS_MSG_QTY )0,
		(OS_TICK )0,
		(void* )0,
		(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
		(OS_ERR* )&err);
		
		//Log_Write("START END" , LOG_EVENT);
		g_cdvStat = CDV_ONLINE;
		LED3 = LED_OFF;
		LED2 = LED_OFF;
	 // OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err); //����ʼ����
	  OS_CRITICAL_EXIT(); //�����ٽ���		
		
		OSTmrStart(&tmr1,&err);
		
		OSTaskDel((OS_TCB*)0,&err); //ɾ�� start_task ��������
}



/*usart ����������*/
void usart_recv_task(void *p_arg){
	OS_ERR err;
	RET_STATUS ret = OPT_SUCCESS;
	CDV_INT32U tmp1 = 0, tmp2 = 0;
	u32 clk;
	p_arg = p_arg; 
	while(1)	{
		if(USART_RX_LEN)//�жϵ�ǰ�����Ƿ�Ϊ0
		{
			if(tmp1 != tm1Re)
			{
				tmp1 = tm1Re;
				clk = 0;
			}
			else
			{
				if(CalcCount(ReadClock1ms(), tmp1) > 2)//����2ms����Ϊһ���������
					if(++clk > 1)
					  USART_RX_QUEUE_SELF_ADD;
			}
		}
		
		//��������г�����
		if(USART_RX_HAD) {
			if(OPT_SUCCESS == RecvParse(g_uartRx.QUEUE.rxBuf[g_uartRx.doPos],g_uartRx.QUEUE.rxLen[g_uartRx.doPos], MAIN_COM))	{
				 USART_RX_QUEUE_DO_NEXT;                                    /*ת�Ƶ����ն��е���һ��*/
			}
		} else if (HAVE_ONLINE_CMD) {
			RecvParse(g_olCache.buf,g_olCache.len, g_olCache.uart);
			ClearOnlineCmdCache();
		}
		
		
		if (USART_TX_HAD)//���������Ҫ���ͣ����������߳�
  		OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
		
		TaskSched();
	}
}

/*usart����������*/
void usart_send_task(void *p_arg){
	OS_ERR err;
	p_arg = p_arg;
	while(1){
		if (USART_TX_HAD) {
			USARTSend(USART_TX_DO_ADDR, USART_TX_DO_LEN, MAIN_COM);					
			//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ
			USART_TX_QUEUE_DO_NEXT;
		}
		else {
			OS_TaskSuspend((OS_TCB*)&UsartSendTaskTCB,&err);               
		}
		
		TaskSched();
	}
}

int nFlag = 0;
void cdv_refresh_task(void *p_arg){
	u32 data1,data2;
  OS_ERR err;
	char dip = -1;
	CDV_INT08U i;
	
	while(1)	{
		LED1=~LED1;		//������
		delay_ms(500);
		
		if(DIP_ON == READ_DIP_SW(2) && dip != 1) //����
		{
					
			dip = 1;
		}
		else if(DIP_OFF == READ_DIP_SW(2) && dip != 0)//�˳�
		{
			dip = 0;
		}
	}
}

//��ʱ��1�Ļص�����
//��������״̬�µĴ�����
void tmr1_callback(void *p_tmr, void *p_arg) {
		
//#if _NPC_VERSION_ > 1u
#if USE_NPC_NET
  Eth_Link_query();
//#else
//	OS_TaskSuspend((OS_TCB*)&WorkerManageTaskTCB,&err);
#endif
//#endif
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
