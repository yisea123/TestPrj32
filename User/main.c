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
//void tmr2_callback(void *p_tmr, void *p_arg);	//��ʱ��2�ص�������wifi
//void tmr3_callback(void *p_tmr, void *p_arg);	//��ʱ��2�ص����������ڻ���

CPU_STK START_TASK_STK[START_STK_SIZE];/*�����ջ*/
void start_task(void *p_arg);/*������*/

CPU_STK USART_TASK_STK[USART_RECV_STK_SIZE];//�����ջ
void usart_recv_task(void *p_arg);

CPU_STK LED1_TASK_STK[USART_SEND_STK_SIZE];//�����ջ
void usart_send_task(void *p_arg);//������

CPU_STK FLOAT_TASK_STK[CDV_REFRESH_STK_SIZE];//�����ջ
void cdv_refresh_task(void *p_arg);//������

CPU_STK WORKER_MANAGE_TASK_STK[WORKER_MANAGE_STK_SIZE];//�����ջ
void worker_manage_task(void *p_arg);//������
//void worker_task(void *p_arg);//������

CPU_STK PARSE_TASK_STK[PARSE_STK_SIZE];//�����ջ
void parse_task(void *p_arg);//������

CPU_STK VALVE_TASK_STK[CDV_VALVE_STK_SIZE];//�����ջ
void cdv_Valve_task(void *p_arg);//������

CPU_STK MOTO_TASK_STK[CDV_MOTO_STK_SIZE];//�����ջ
void cdv_Moto_task(void *p_arg);//������
///*
// *���Ժ���
// */
//void test(void){
//	CDV_INT08U* buf = NULL;
//	CDV_INT16U len;
//	CDV_INT32U addr = SCRIP_RES(0);
//	CDV_INT08U i = 0;
//	CDV_INT08U AxleNum;
//	if (!addr)
//		return;
//	
//  SPI_Flash_Read((CDV_INT08U *)&len, addr, 4);
//	NEW08U(buf, len);
//	#if 0
//	SPI_Flash_Read((CDV_INT08U *)buf, addr + 4, len);
//	AxleNum = buf[0];
//	for(i=0;i<AxleNum;i++)
//	{
//		int len = 0;
//		int nbyte = 1;
//		moto_par.drvtype[i] = UartToU32(buf+i+1,nbyte,LITTLEEND);
//		len+=nbyte;
//		nbyte = 4;
//		moto_par.max_speed[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;
//		moto_par.min_speed[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;
//		moto_par.up_limit[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;
//		moto_par.down_limit[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;
//		nbyte = 1;
//		moto_par.zero_dir[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;
//		moto_par.pstv_dir[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;	
//		nbyte = 2;
//		moto_par.div_num[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;			
//		nbyte = 1;
//		moto_par.pul_mode[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;	
//		nbyte = 4;
//		moto_par.max_acc[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;		
//		moto_par.min_acc[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;		
//		nbyte = 1;
//		moto_par.zero_mode[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;	
//		moto_par.run_mode[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;	
//		moto_par.encoder_type[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
//		len+=nbyte;		
//		moto_par.backlash_comp[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);				
//	}
//	#else
//	SPI_Flash_Read((CDV_INT08U *)&moto_par, addr + 5, len);
//	//AxleNum = buf[0];
//	#endif
//	DELETE(buf);
//}


//u8 serial_state;
//u8 serial_flag;

u8 debug = 0, debug1 = 0, debug2 = 0;
		
int main(void){
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init(168);                               //ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	INTX_DISABLE();                                //���ж�,��ֹ�δ�ʱ���������ʼ���Ĵ���
	PeriphDriverInit();                            //��ʼ��
	
	OSInit(&err);                                  //��ʼ�� UCOSIII
	
	INTX_ENABLE();                                 //���ж�	
	
	
#if USE_NPC_NET == 1u
	NPC_PRINT("net init start\r\n");
//	if(!lwip_comm_init()) 		//lwip��ʼ��
//	{
//		tcp_server_init();
//	}
/* configure Ethernet (GPIOs, clocks, MAC, DMA) */ 
  ETH_BSP_Config();
  NPC_PRINT("���� bsp end\r\n");
  /* Initilaize the LwIP stack */
  LwIP_Init();
  NPC_PRINT("���� lwip end\r\n");
  /* Initialize webserver demo */
  tcp_server_init();
//	while(tcp_server_init()) 									//��ʼ��tcp_server(����tcp_server�߳�)
//	{	
//		delay_ms(500);
//	}
  httpd_init();
	
	udpecho_init();
	NPC_PRINT("net init end\r\n");
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
	
	NPC_PRINT("start task start\r\n");
	
	p_arg = p_arg;
	CPU_Init();
	
	#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err); //ͳ������
	#endif
	#ifdef CPU_CFG_INT_DIS_MEAS_EN //���ʹ���˲����жϹر�ʱ��
	CPU_IntDisMeasMaxCurReset();
	#endif
	#if OS_CFG_SCHED_ROUND_ROBIN_EN //��ʹ��ʱ��Ƭ��ת��ʱ��
	OSSchedRoundRobinCfg(DEF_ENABLED, 5, &err);//ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ 1 ��ϵͳʱ�ӽ���
	#endif
	//�����ź���							
	OSMutexCreate ((OS_MUTEX* )&TX_SEM, //ָ���ź���
								(CPU_CHAR* )"TX_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);
													
//	OSSemCreate ((OS_SEM* )&VAL_SEM, //ָ���ź���
//								(CPU_CHAR* )"VAL_SEM", //�ź�������
//								(OS_SEM_CTR )1, //�ź���ֵΪ 1
//								(OS_ERR* )&err);
//	OSSemCreate ((OS_SEM* )&SPI_SEM, //ָ���ź���
//								(CPU_CHAR* )"SPI_SEM", //�ź�������
//								(OS_SEM_CTR )1, //�ź���ֵΪ 1
//								(OS_ERR* )&err);
	OSMutexCreate ((OS_MUTEX* )&WORKER_SEM, //ָ���ź���
								(CPU_CHAR* )"WORKER_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);
	OSMutexCreate ((OS_MUTEX* )&MOTO_SEM, //ָ���ź���
								(CPU_CHAR* )"MOTO_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);
	OSMutexCreate ((OS_MUTEX* )&MEM_SEM, //ָ���ź���
								(CPU_CHAR* )"MEM_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);
	OSMutexCreate ((OS_MUTEX* )&MSG_SEM, //ָ���ź���
								(CPU_CHAR* )"MSG_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);
  OSMutexCreate ((OS_MUTEX* )&LIST_SEM, //ָ���ź���
								(CPU_CHAR* )"LIST_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);
//	OSSemCreate ((OS_SEM* )&WIFI_SEM, //ָ���ź���
//								(CPU_CHAR* )"WIFI_SEM", //�ź�������
//								(OS_SEM_CTR )1, //�ź���ֵΪ 1
//								(OS_ERR* )&err);	
//	OSSemCreate ((OS_SEM* )&PROMPT_SEM, //ָ���ź���
//								(CPU_CHAR* )"PROMPT_SEM", //�ź�������
//								(OS_SEM_CTR )1, //�ź���ֵΪ 1
//								(OS_ERR* )&err);							
	
//	OSSemCreate ((OS_SEM* )&CACHE_SEM, //ָ���ź���
//								(CPU_CHAR* )"CACHE_SEM", //�ź�������
//								(OS_SEM_CTR )1, //�ź���ֵΪ 1
//								(OS_ERR* )&err);		
								
	OSMutexCreate ((OS_MUTEX* )&COIL_SEM, //ָ���ź���
								(CPU_CHAR* )"COIL_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);			
	
	for (i = 0; i < 6; i++) {
		char buf[10];
		sprintf(buf, "COM_SEM%d", i);
	  OSMutexCreate ((OS_MUTEX* )&COM_SEM[i], //ָ���ź���
									(CPU_CHAR* )buf, //�ź�������
									//(OS_SEM_CTR )1, //�ź���ֵΪ 1
									(OS_ERR* )&err);		
	}

	OSMutexCreate ((OS_MUTEX* )&TCP_TX_SEM, //ָ���ź���
								(CPU_CHAR* )"TCP_TX_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);		
//�����ź���							
	OSMutexCreate ((OS_MUTEX* )&PLUSE_SEM, //ָ���ź���
								(CPU_CHAR* )"PLUSE_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);

#if USE_WORKER_DEBUG == 1u
	OSMutexCreate ((OS_MUTEX* )&DEBUG_SEM, //ָ���ź���
								(CPU_CHAR* )"DEBUG_SEM", //�ź�������
								//(OS_SEM_CTR )1, //�ź���ֵΪ 1
								(OS_ERR* )&err);
#endif
	/*FPGA����������ؿ���*/
//	SPI_Flash_Read((CDV_INT08U *)&SRP_NUM_RUN,SCRIP_NUM_RUN,2);
//	SPI_Flash_Read((CDV_INT08U *)&SRP_NUM_RUN,SCRIP_NUM_RUN,2);
//	SPI_Flash_Read((CDV_INT08U *)&SRP_NUM_DEBUG,SCRIP_NUM_DEBUG,2);
	//Log_Write("START DOWNLOAD FPGA" , LOG_EVENT);
	NPC_PRINT("���� sem end\r\n");
	
#if ENABLE_FPGA_DOWN == 1u
	FpgaRecvCtl();
#endif
	CDVParamInit();
	NPC_PRINT("���� CDVParamInit end\r\n");
			
#if USE_PVD == 1u
	g_dipCtrlWorker = PVD_Restore();
	PVD_Config();
	PVD_Erase();
#endif
#if USE_EXTI_POWER_OFF == 1u
  FlashBak_Restore();
  EXTIPowerOff_Configuration();
#endif

	NPC_PRINT("���� Restore end\r\n");
#if ENABLE_FPGA_DOWN
//	for(i = CDV_O_NUM ; i < CDV_O_NUM + CDV_EXO_NUM*2 ; i ++) {
//		OWrite(i , BIT_0);
//		//IRead(i);
//	}
	ExIOInit();	
	/*��ȡflash����*/

	if(FPGA_Mode == FPGA_MOTOR_CODE)
	{
		MotoResParRead();
		for(i = 0 ; i < CDV_FPGA_NUM*2;i++)
		{
			Moto_Init(i,0xffffffff);//��ʼ���������
		}	
	}
	//for(i = 0 ; i < CDV_FPGA_NUM;i++)
//	{
//		Line_Arc_Config(1);//���������ʼ��
//	}	
	
#endif
	//MODBUS_SET_UPDATE;

	
//	//Fpga_Config(1, FPGA_MOTOR);
//	OS_CRITICAL_ENTER(); //�����ٽ���
//	//Fpga_Down(1, FPGA_MOTOR);
//	OS_CRITICAL_EXIT(); //�����ٽ���		
	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
                (CPU_CHAR	*)"tmr1",		//��ʱ������
                (OS_TICK	 )0,			//0*1=0ms
                (OS_TICK	 )5,          //n*10ms=20ms 10ms=1/OS_CFG_TMR_TASK_RATE_HZ 
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����						
	//������ʱ��2
//	OSTmrCreate((OS_TMR		*)&tmr2,		
//                (CPU_CHAR	*)"tmr2",		
//                (OS_TICK	 )0,			//N*10=3000ms	
//                (OS_TICK	 )2,   					
//                (OS_OPT		 )OS_OPT_TMR_PERIODIC,//OS_OPT_TMR_ONE_SHOT, 	//���ζ�ʱ��
//                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//��ʱ��2�ص�����
//                (void	    *)0,			
//                (OS_ERR	    *)&err);	
//								//������ʱ��2
//	OSTmrCreate((OS_TMR		*)&tmr3,		
//                (CPU_CHAR	*)"tmr3",		
//                (OS_TICK	 )0,			//N*10=3000ms	
//                (OS_TICK	 )2,   					
//                (OS_OPT		 )OS_OPT_TMR_PERIODIC,//OS_OPT_TMR_ONE_SHOT, 	//���ζ�ʱ��
//                (OS_TMR_CALLBACK_PTR)tmr3_callback,	//��ʱ��3�ص�����
//                (void	    *)0,			
//                (OS_ERR	    *)&err);	
							
//		if(DIP_ON == READ_DIP_SW(2)) //����ģʽ
//		{
//			//if(CascadeGetNativeNo() == 0xFF)
//  			CascadeCombine(0x00);
//	
//		}
//		else
//		{
//			CascadeLocal(0x00);
//		}
//			
			
		if(debug)//�˳�
		{
			WorkerControl(1, WORKER_LOOP);
			if(debug)//�˳�
			{
				WorkerControl(0, WORKER_LOOP);
			}
			return;
		}

	//OS_CRITICAL_ENTER(); //�����ٽ���
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
		//�����������
	OSTaskCreate((OS_TCB* )&TaskParseTCB,
		(CPU_CHAR* )"task parse",
		(OS_TASK_PTR )parse_task,
		(void* )0,
		(OS_PRIO )PARSE_TASK_PRIO,
		(CPU_STK* )&PARSE_TASK_STK[0],
		(CPU_STK_SIZE )PARSE_STK_SIZE/10,
		(CPU_STK_SIZE )PARSE_STK_SIZE,
		(OS_MSG_QTY )0,
		(OS_TICK )0,
		(void* )0,
		(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
		(OS_ERR* )&err);
		if(debug)//�˳�
		{
			WorkerControl(1, WORKER_LOOP);
			if(debug)//�˳�
			{
				WorkerControl(0, WORKER_LOOP);
			}
			return;
		}
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
	//������������������
//	OSTaskCreate((OS_TCB* )&CdvValveTaskTCB,
//		(CPU_CHAR* )"cdv_Valve_task",
//		(OS_TASK_PTR )cdv_Valve_task,
//		(void* )0,
//		(OS_PRIO )CDV_VALVE_TASK_PRIO,
//		(CPU_STK* )&VALVE_TASK_STK[0],
//		(CPU_STK_SIZE )CDV_VALVE_STK_SIZE/10,
//		(CPU_STK_SIZE )CDV_VALVE_STK_SIZE,
//		(OS_MSG_QTY )0,
//		(OS_TICK )0,
//		(void* )0,
//		(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//		(OS_ERR* )&err);		
		
	//�������˹�����������
	OSTaskCreate((OS_TCB* )&WorkerManageTaskTCB,
		(CPU_CHAR* )"worker manage task",
		(OS_TASK_PTR )worker_manage_task,
		(void* )0,
		(OS_PRIO )WORKER_MANAGE_TASK_PRIO,
		(CPU_STK* )&WORKER_MANAGE_TASK_STK[0],
		(CPU_STK_SIZE )WORKER_MANAGE_STK_SIZE/10,
		(CPU_STK_SIZE )WORKER_MANAGE_STK_SIZE,
		(OS_MSG_QTY )0,
		(OS_TICK )0,
		(void* )0,
		(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
		(OS_ERR* )&err);
		
	//���������������
#if ENABLE_FPGA_DOWN
	OSTaskCreate((OS_TCB* )&CdvMotoTaskTCB,
		(CPU_CHAR* )"cdv_Moto_task",
		(OS_TASK_PTR )cdv_Moto_task,
		(void* )0,
		(OS_PRIO )CDV_MOTO_TASK_PRIO,
		(CPU_STK* )&MOTO_TASK_STK[0],
		(CPU_STK_SIZE )CDV_MOTO_STK_SIZE/10,
		(CPU_STK_SIZE )CDV_MOTO_STK_SIZE,
		(OS_MSG_QTY )0,
		(OS_TICK )0,
		(void* )0,
		(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
		(OS_ERR* )&err);				
#endif
	//�������˹�����������
//		WORKER_RUN_NUM = WORKER_NUM;
//		for(i = 0; i< WORKER_NUM ; i++) {
//			OSTaskCreate((OS_TCB* )&WorkerTaskTCB[i],
//				(CPU_CHAR* )workerName[i],
//				(OS_TASK_PTR )worker_task,
//				(void* )&workerId[i],
//				(OS_PRIO )WORKER_TASK_PRIO,
//				(CPU_STK* )&WORKER_TASK_STK[i][0],
//				(CPU_STK_SIZE )WORKER_STK_SIZE/10,
//				(CPU_STK_SIZE )WORKER_STK_SIZE,
//				(OS_MSG_QTY )0,
//				(OS_TICK )0,
//				(void* )0,
//				(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//				(OS_ERR* )&err);
//			
//		}
//		Log_Write("START END" , LOG_EVENT);
		g_cdvStat = CDV_ONLINE;
		LED3 = LED_OFF;
		LED2 = LED_OFF;

	 // OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err); //����ʼ����
	  //OS_CRITICAL_EXIT(); //�����ٽ���		
		OSTmrStart(&tmr1,&err);

    NPC_PRINT("start task end\r\n");
		
		OSTaskDel((OS_TCB*)0,&err); //ɾ�� start_task ��������
}



/*usart ����������*/
void usart_recv_task(void *p_arg){
	OS_ERR err;
	RET_STATUS ret = OPT_SUCCESS;
	CDV_INT32U tmp1 = 0, tmp2 = 0;
	p_arg = p_arg; 
	while(1)	{
		u32 clk, clk2;
		if(USART_RX_LEN)//�жϵ�ǰ�����Ƿ�Ϊ0
		{
			if(tmp1 != tm1Re)
			{
				tmp1 = tm1Re;
				clk = 0;
			}
			else
			{
				if(CalcTimeMS(GetCdvTimeTick(), tmp1) > 2)//����2ms����Ϊһ���������
					if(++clk > 1)
					  USART_RX_QUEUE_SELF_ADD;
			}
		}
#if USE_CASCADE == 1u
		//��������Ľ���
		if(GetUsartRxLen())//�жϵ�ǰ�����Ƿ�Ϊ0
		{
			if(tmp2 != tm2Re)
			{
				tmp2 = tm2Re;
				clk2 = 0;
			}
			else
			{
				if(CalcCount(ReadClock1ms(), tmp2) > 2)//����2ms����Ϊһ���������
					if(++clk2 > 1)
					{//���ս�����
						CDV_INT16U crc;
						crc = getCRC16(g_pUsart3RxBuf, g_Usart3RxLen-2);
						if(((crc&0xFF) == g_pUsart3RxBuf[g_Usart3RxLen - 2]) && ((crc>>8)&0xFF) == g_pUsart3RxBuf[g_Usart3RxLen - 1] )
						{
							if(PortCmdCache(g_pUsart3RxBuf, g_Usart3RxLen-2, 3)) 
							{
								
							}
						}
						SetUsartRxLen(0);
					}
			}
		}
#endif
		
		//��������г�����
		if(USART_RX_HAD) 
		{
				if(OPT_SUCCESS == RecvParse(g_uartRx.QUEUE.rxBuf[g_uartRx.doPos],g_uartRx.QUEUE.rxLen[g_uartRx.doPos], MAIN_COM, NULL))
				{
				  USART_RX_QUEUE_DO_NEXT;                                    /*ת�Ƶ����ն��е���һ��*/
				}
		}
		else 
		{
			//OS_TaskSuspend((OS_TCB*)&UsartRecvTaskTCB,&err);
		}
		
		if(g_olCache.mark || g_portCmdCache.mark)//����н��������������߳�
		{
			OSTaskResume((OS_TCB*)&TaskParseTCB,&err);		
		}
		
		if (USART_TX_HAD)//���������Ҫ���ͣ����������߳�
  		OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
		
		TaskSched();
	}
}

void parse_task(void *p_arg)
{
	OS_ERR err;
	while(1) 
	{
		if(HAVE_ONLINE_CMD) 
		{
			if(
#if USE_16BIT_CMD == 1u
  #if USE_CASCADE == 1u
				(KFC_IC == g_olCache.buf[0] && 0x10 == g_olCache.buf[1] && CascadeGetNativeNo() == g_olCache.buf[4] || ((KFC_IC != g_olCache.buf[0] || 0x10 != g_olCache.buf[1]) && CascadeGetNativeNo() == g_olCache.buf[3] ))||
  #endif
				(KFC_IC == g_olCache.buf[0] && (0x10 > g_olCache.buf[1]||0x30 == g_olCache.buf[1])) ||
				(KFC_IC == g_olCache.buf[0] && 0x10 == g_olCache.buf[1] && 0x41 == g_olCache.buf[6]/*g_olCache.buf[5]*/)||
			   KFC_IC != g_olCache.buf[0] ) 
#else
  #if USE_CASCADE == 1u
				CascadeGetNativeNo() == g_olCache.buf[3] || 
  #endif
				(KFC_IC == g_olCache.buf[0] && (0x10 > g_olCache.buf[1]||0x30 == g_olCache.buf[1])) ||
				(KFC_IC == g_olCache.buf[0] && 0x10 == g_olCache.buf[1] && 0x41 == g_olCache.buf[5])||
			   KFC_IC != g_olCache.buf[0] ) 
#endif
			{
				CMD_ARG arg;
				CmdArgInit(&arg);
				arg.uart = g_olCache.uart;
				arg.arg = g_olCache.arg;
				arg.len = g_olCache.len;
				arg.buf =	g_olCache.buf;
#if USE_CASCADE == 1u
				arg.hostid = CascadeGetNativeNo();
#endif
				
				CmdParse(g_olCache.buf, g_olCache.len, &arg);
				ClearOnlineCmdCache();
				/*������Դ�ظ�����*/
				DoResRequest(&arg);
				CmdArgDelete(&arg);
				
			}
#if USE_CASCADE_MAP == 1u
			else//������ӻ�
			{
				CMD_ARG arg;
				CmdArgInit(&arg);
				arg.uart = g_olCache.uart;
				arg.arg = g_olCache.arg;
				arg.len = g_olCache.len;
				arg.buf =	g_olCache.buf;
				arg.hostid = CascadeGetNativeNo();
				
				TFUsartCmdParse(g_olCache.buf, g_olCache.len, &arg);
				
				/*������Դ�ظ�����*/
				DoResRequest(&arg);
				CmdArgDelete(&arg);
				
//				CDV_INT08U* recvBuf = NULL;
//			  CDV_INT08U  recvLen = 0;
//				RET_STATUS ret = OPT_FAILURE;
//				//g_olCache.buf[3] = 2;
//				
////			  if(OPT_SUCCESS == (ret = CascadeTranferCmd(&recvBuf, &recvLen, g_olCache.buf, g_olCache.len)))
////				{
////				  ret = TFCmdParse(recvBuf, recvLen);
////				}
//				
//				
//				
//				if(OPT_SUCCESS == (ret = CascadeUsartTranferCmd(&recvBuf, &recvLen, g_olCache.buf, g_olCache.len)))
//				{
//				  //ret = TFUsartCmdParse(recvBuf, recvLen);
//					if(recvLen - g_olCache.len == 7)//����б������������������
//					{
//						CDV_INT08U index = g_olCache.len;
//						if(0x05 == recvBuf[index])
//						{
//							ValSet(*(CDV_INT16U*)(recvBuf + index + 1) , *(CDV_INT32S*)(recvBuf + index + 3));
//						}
//					}
//				}
//				
//				if(OPT_SUCCESS == ret)
//				{
//				  AddTx(recvBuf, recvLen, g_olCache.uart);
//				}
//				else
//				{
//					g_olCache.buf[1] += 0x80;
//					AddTx(g_olCache.buf, g_olCache.len, g_olCache.uart);
//				}
//				
//				DELETE(recvBuf);
        ClearOnlineCmdCache();
			}
#endif
			
			

		} 
#if USE_CASCADE == 1u
		else if (g_portCmdCache.mark)//�����Ľ���
		{
				CMD_ARG arg;
				INIT_CLEAR(arg);
				arg.uart = g_portCmdCache.uart;
				arg.len = g_portCmdCache.len;
				arg.buf =	g_portCmdCache.buf;
				arg.hostid = CascadeGetNativeNo();
			
			  if(CascadeGetNativeNo() == g_portCmdCache.buf[3] || 
				(KFC_IC == g_portCmdCache.buf[0] && 0x10 > g_portCmdCache.buf[1]) ||
			   KFC_IC != g_portCmdCache.buf[0] )
				{
				  CmdParse(g_portCmdCache.buf, g_portCmdCache.len, &arg);
				}
				/*������Դ�ظ�����*/
				if(OPT_FAILURE == DoSpecRequest(&arg))
				  DoResRequest(&arg);
				
				CmdArgDelete(&arg);
				ClearPortCmdCache();
		}	
#endif
		else 
		{
			LED3 = LED_OFF;
			OS_TaskSuspend((OS_TCB*)&TaskParseTCB,&err);               /*��������*/
			LED3 = LED_ON;
		}
		
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
//��������������
#ifdef ENABLE_PID
void cdv_Valve_task(void *p_arg){
u16 i=0;

#if 1
	while(1){	
		for ( i = 0; i < PID_NUM; i++)
		{
		  RunProcess(i);
		}
		
		#if 0
		if((i++)>30)
	  {
				// PH_ReadValue(0x01);
				 
//				 WorkerThreadRun(0,0);
				 
//					sprintf(tmp , "%s%dKp %s%d%Kp\r\n" 
//		//		,(CDV_INT08U*)cdv_error[g_cdvStat] 
//						,"�����������ѹ:" 
//			      ,(u32)Present_Pressure_Rel
//			      ,"��������ѹ��" 
//						,(u32)R_Pressure
//						);
//				UART4_Send(tmp, strlen(tmp));	
			i=0;
			break;
		}
		#endif 
	}
	#endif
}
#endif

	u32 ftime_cascade_map, time_cascade_map;
	
void cdv_refresh_task(void *p_arg){
	
#if USE_CASCADE_MAP == 1u
//  OS_ERR err;
//	CDV_INT08U i;
  u32 cnt=0;

	while(1)	{
#ifdef  DEBUG_TIME
  ftime_cascade_map = GetCdvTimeTick();
#endif
		////CascadeModbus_AllUpdate();
		if(OPT_FAILURE ==CascadeModbus_Map()) {
			if(cnt++ > 20) {
				OUT_DisPlay(0xFF280B48);
			  ManagerControl(WORKER_STOP);
			}
		}else{
			cnt = 0;
		}
		
#ifdef  DEBUG_TIME
		//stime = GetCdvTimeTick();
		
		time_cascade_map = CalcCount(GET_TICK, ftime_cascade_map);
		
		if(time_cascade_map > 1000) {
			time_log(time_cascade_map);
			
//			char tmp[50]={0};

//			sprintf(tmp , "cascade map timeout:%d" 
//					,time_cascade_map
//					);
//			
//			Log_Write(tmp , LOG_EVENT);
		
		}
#endif
//		if (500 < CalcTimeMS(stime,ftime ))
//		{
//			ftime = stime;

//		}
	}
#endif
}

	u32 ftime_local_map, time_local_map;
/*��������ˢ������*/
void worker_manage_task(void *p_arg){
	
	OS_ERR err;
  //ˢ��

	while (1) {
#ifdef  DEBUG_TIME
  ftime_local_map = GetCdvTimeTick();
#endif
		int i=0;
		
		IReadAllFilter(5, 10); //IReadAll();

		//OReadAll();
#if ENABLE_FPGA_DOWN
		ExIReadAll();
		ExOReadAll();
		ExOWriteAll();
#endif
		//delay_ms(10);
		
		memmng_test();
		LIST_Test();
		
	  if(debug)//�˳�
	  {
			CDV_INT08U buf[] = {0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12};
			
      CoilToCoil(buf, 6, (CDV_INT08U*)g_modbusCoil.coilCh, 3, 20);
			CoilToCoil(buf, 3, (CDV_INT08U*)g_modbusCoil.coilCh, 6, 20);
			CoilToCoil(buf, 5, (CDV_INT08U*)g_modbusCoil.coilCh, 3, 2);
			CoilToCoil(buf, 2, (CDV_INT08U*)g_modbusCoil.coilCh, 7, 2);
//		CDV_INT32S flag = 100;
//		CDV_INT32U addr = 0x080E0000 + 0x20000 -4;
		//WorkerControl(debug1, debug2);
//		udpecho_find(MAIN_COM);
//	//	INTX_DISABLE();
//	FlashBak_Erase(1);
//		
//		FlashBak_Unlock();
//	FlashBak_Write((CDV_INT08U *)&flag, addr, sizeof(flag));
//	
//	FlashBak_Lock();
////	INTX_ENABLE();
		  debug = 0;
	  }
		
#if USE_PVD == 1u
		if(PVD_GetFlag()) {
			ShutDown();
		}
#endif
		
//#if USE_EXTI_POWER_OFF == 1u
//		if(-1 == EXTIPowerOff_GetFlag()) {
//			INTX_DISABLE();
//		  FlashBak_BackUp();
//			INTX_ENABLE();
//		} else if(1 == EXTIPowerOff_GetFlag()) {
//			ResetCdv();
//		}
//		
//#endif
		
    TaskSched();
		
#ifdef  DEBUG_TIME
		//stime = GetCdvTimeTick();
		
		time_local_map = CalcCount(GET_TICK, ftime_local_map);
		
		if(time_local_map > 1000) {
			time_log(time_local_map);
			
//			char tmp[50]={0};

//			sprintf(tmp , "cascade map timeout:%d" 
//					,time_cascade_map
//					);
//			
//			Log_Write(tmp , LOG_EVENT);
		
		}
#endif
	}
}

#if ENABLE_FPGA_DOWN
//�����������
void cdv_Moto_task(void *p_arg){

	CDV_INT08U i;	
	OS_ERR err;

	while(1)
	{
		CDV_INT08U Cnt = 0;
		for(i=0;i<AXLE_NUM;i++)
		{
			RealTimeSpeedSet(i);
			if(Moto_RunSta[i]==READY)
				Cnt++;
		}
		if(Cnt ==AXLE_NUM)
			OS_TaskSuspend((OS_TCB*)&CdvMotoTaskTCB,&err);
		delay_ms(5);
	}	
}

#endif
/*������ǹ���id����0-7*/

//void worker_task(void *p_arg){
//	OS_ERR err;
//	//p_arg = p_arg;
//	CDV_INT08U *pArg = (CDV_INT08U*)p_arg;
//	while(1)	{
//		switch(g_cdvStat){
//	//		case CDV_PAUSE:
//			case CDV_RUN:
//			  RunScript(*pArg+2 ,1, g_offlineUart);
//				break;
//			default:
//				OSSemPend(&VAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
//				WORKER_RUN_NUM--;
//			  OSSemPost (&VAL_SEM,OS_OPT_POST_1,&err);
//				OS_TaskSuspend((OS_TCB*)&WorkerTaskTCB[*pArg],&err);               /*��������*/
//			  OSSemPend(&VAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
//			  WORKER_RUN_NUM++;
//			  OSSemPost (&VAL_SEM,OS_OPT_POST_1,&err);
//				break;
//		}
//	}
//}




//��ʱ��1�Ļص�����
//��������״̬�µĴ�����
void tmr1_callback(void *p_tmr, void *p_arg) {
//  OS_ERR err;
	u32 data1,data2;
  static u32 cnt = 0;
//	if(++tm1Re < 2)
//		return;
////	serial_state=USART3_Receive_Data2(general_serial_buf,&general_serial_count);//�жϴ��ڽ������
////	if(serial_state==1)//�������
////	{
////			general_serial_return(general_serial_buf, general_serial_count);
////		  serial_flag=1;
////      general_serial_count2=general_serial_count;
////		  OSTaskResume((OS_TCB*)&UsartRecvTaskTCB,&err);
////	}
//	
//	USART_RX_QUEUE_SELF_ADD;
//	if(USART_RX_HAD) {
//		OSTaskResume((OS_TCB*)&UsartRecvTaskTCB,&err);
//	}
	if(cnt++>10) {
	  LED1=~LED1;		//������
		cnt = 0;
	}

	//delay_ms(500);
	data1 = Read_Input_All();
	data2 = Read_Output_ALL();
	IN_DisPlay(data1);          //����״̬��ʾ
	OUT_DisPlay(data2);         //���״̬��ʾ
	Adc_Voltge ();
	// Flash_Check();
	UpdateDacVal();
	UpdateAdcVal();
	
	if(DIP_ON == READ_DIP_SW(2) && g_dipCtrlWorker != 1) //����
	{
	//if(!Cascade_HaveSlaveTable()) //����ģʽ
	//{
	//	CascadeCombine(0x00);
	//}
	//	Test();
		ManagerControl(WORKER_ONCE);
		//WorkerControl(0, WORKER_LOOP);//����
		//WorkerControl(1, WORKER_ONCE);//�๤
		g_dipCtrlWorker = 1;
	}
	else if(DIP_OFF == READ_DIP_SW(2) && g_dipCtrlWorker != 0)//�˳�
	{
		//ManagerControl(WORKER_STOP);
		//AllWorkerCtrl(WORKER_STOP);
		#if USE_WORKER_DEBUG == 1u
		AllDebugStateSet(DEBUG_DISABLE);//�ȹص�����Ȼ�˲���
		#endif
		
		AllWorkerCtrl(WORKER_STOP);
		g_dipCtrlWorker = 0;
	}
	

//		if(time_cascade_map > 1000)
//			Log_Write("cascade map timeout" , LOG_EVENT);
//		

#if USE_NPC_NET == 1u
  Eth_Link_query();
//#else
//	OS_TaskSuspend((OS_TCB*)&WorkerManageTaskTCB,&err);
#endif

}
////��ʱ��1�Ļص�����
////��������״̬�µĴ�����
//void tmr2_callback(void *p_tmr, void *p_arg){
//  OS_ERR err;
//	if(++tm2Re < 2)
//		return;
//	WIFI_RX_QUEUE_SELF_ADD;
//	if(WIFI_RX_HAD) {
//		OSTaskResume((OS_TCB*)&UsartRecvTaskTCB,&err);
//	}
//}
////��ʱ��1�Ļص�����
////��������״̬�µĴ�����
//void tmr3_callback(void *p_tmr, void *p_arg){
//  OS_ERR err;
//	if(++tm3Re < 1)
//		return;
//	//serial_state=USART3_Receive_Data2(general_serial_buf,&general_serial_count);//�жϴ��ڽ������
////	if(serial_state==1)//�������
////	{
////			general_serial_return(general_serial_buf, general_serial_count);
////		  serial_flag=1;
////      general_serial_count2=general_serial_count;
////		  OSTaskResume((OS_TCB*)&UsartRecvTaskTCB,&err);
////	}
//}


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
