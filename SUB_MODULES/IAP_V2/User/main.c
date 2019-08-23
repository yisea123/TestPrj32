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
void tmr1_callback(void *p_tmr, void *p_arg); 	//定时器1回调函数，CDV串口

CPU_STK START_TASK_STK[START_STK_SIZE];/*任务堆栈*/
void start_task(void *p_arg);/*任务函数*/

CPU_STK USART_TASK_STK[USART_RECV_STK_SIZE];//任务堆栈
void usart_recv_task(void *p_arg);

CPU_STK LED1_TASK_STK[USART_SEND_STK_SIZE];//任务堆栈
void usart_send_task(void *p_arg);//任务函数

CPU_STK FLOAT_TASK_STK[CDV_REFRESH_STK_SIZE];//任务堆栈
void cdv_refresh_task(void *p_arg);//任务函数



int main(void){
	CDV_INT08C fields[5] = "";
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init(168);                               //时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置

		
	//IAP_LoadApp();
	
	INTX_DISABLE();                                //关中断,防止滴答定时器对外设初始化的打扰
	PeriphDriverInit();                            //初始化
		
//	if(DIP_ON == READ_DIP_SW(2)) //启动
//	{		
//		IAP_LoadApp();
//	}
		
	OSInit(&err);                                  //初始化 UCOSIII
	INTX_ENABLE();                                 //开中断	
	
	// OTA检测
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

	OS_CRITICAL_ENTER();                           //进入临界区
	/*创建开始任务*/
	OSTaskCreate((OS_TCB* )&StartTaskTCB,          //任务控制块
		(CPU_CHAR* )"start task",                    //任务名字
		(OS_TASK_PTR )start_task,                    //任务函数
		(void* )0,                                   //传递给任务函数的参数
		(OS_PRIO )START_TASK_PRIO,                   //任务优先级
		(CPU_STK* )&START_TASK_STK[0],               //任务堆栈基地址
		(CPU_STK_SIZE )START_STK_SIZE/10,            //任务堆栈深度限位
		(CPU_STK_SIZE )START_STK_SIZE,               //任务堆栈大小
		(OS_MSG_QTY )0,                              //任务内部消息队列能够接收的最大消息数目,为 0 时禁止收消息
		(OS_TICK )0,                                 //当使能时间片轮转时的时间片长度，为 0 时为默认长度。
		(void* )0,                                   //用户补充的存储区
		(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
		(OS_ERR* )&err);                             //存放该函数错误时的返回值
	OS_CRITICAL_EXIT();                            //退出临界区
	OSStart(&err);                                 //开启 UCOSIII
	while(1);
}
//开始任务函数
void start_task(void *p_arg){
	CDV_INT08U i;
	//CDV_INT32U size;
	OS_ERR err;
	CPU_SR_ALLOC();//OS_CRITICAL_ENTER()
	p_arg = p_arg;
	CPU_Init();
	
	#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err); //统计任务
	#endif
	#ifdef CPU_CFG_INT_DIS_MEAS_EN //如果使能了测量中断关闭时间
	CPU_IntDisMeasMaxCurReset();
	#endif
	#if OS_CFG_SCHED_ROUND_ROBIN_EN //当使用时间片轮转的时候
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);//使能时间片轮转调度功能,时间片长度为 1 个系统时钟节拍，既 1*1=1ms
	#endif
	//创建信号量							
	OSSemCreate ((OS_SEM* )&TX_SEM, //指向信号量
								(CPU_CHAR* )"TX_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);

	OSSemCreate ((OS_SEM* )&MOTO_SEM, //指向信号量
								(CPU_CHAR* )"MOTO_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);
	OSSemCreate ((OS_SEM* )&MEM_SEM, //指向信号量
								(CPU_CHAR* )"MEM_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);
								
	OSSemCreate ((OS_SEM* )&GENERAL_SERIAL_SEM, //指向信号量
								(CPU_CHAR* )"GENERAL_SERIAL_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);			

	OSSemCreate ((OS_SEM* )&TCP_TX_SEM, //指向信号量
								(CPU_CHAR* )"TCP_TX_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);		


	//创建定时器1
	OSTmrCreate((OS_TMR		*)&tmr1,		//定时器1
                (CPU_CHAR	*)"tmr1",		//定时器名字
                (OS_TICK	 )0,			//0*1=0ms
                (OS_TICK	 )50,          //n*10ms=20ms 10ms=1/OS_CFG_TMR_TASK_RATE_HZ 
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码						
	//创建定时器2
								
	OS_CRITICAL_ENTER(); //进入临界区
	//创建 串口接收任务
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
	//创建 串口发送任务
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
	//创建CDV刷新任务
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
	 // OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err); //挂起开始任务
	  OS_CRITICAL_EXIT(); //进入临界区		
		
		OSTmrStart(&tmr1,&err);
		
		OSTaskDel((OS_TCB*)0,&err); //删除 start_task 任务自身
}



/*usart 接收任务函数*/
void usart_recv_task(void *p_arg){
	OS_ERR err;
	RET_STATUS ret = OPT_SUCCESS;
	CDV_INT32U tmp1 = 0, tmp2 = 0;
	u32 clk;
	p_arg = p_arg; 
	while(1)	{
		if(USART_RX_LEN)//判断当前缓存是否为0
		{
			if(tmp1 != tm1Re)
			{
				tmp1 = tm1Re;
				clk = 0;
			}
			else
			{
				if(CalcCount(ReadClock1ms(), tmp1) > 2)//超过2ms，认为一条命令结束
					if(++clk > 1)
					  USART_RX_QUEUE_SELF_ADD;
			}
		}
		
		//有命令，进行初解析
		if(USART_RX_HAD) {
			if(OPT_SUCCESS == RecvParse(g_uartRx.QUEUE.rxBuf[g_uartRx.doPos],g_uartRx.QUEUE.rxLen[g_uartRx.doPos], MAIN_COM))	{
				 USART_RX_QUEUE_DO_NEXT;                                    /*转移到接收队列的下一条*/
			}
		} else if (HAVE_ONLINE_CMD) {
			RecvParse(g_olCache.buf,g_olCache.len, g_olCache.uart);
			ClearOnlineCmdCache();
		}
		
		
		if (USART_TX_HAD)//如果有命令要发送，启动发送线程
  		OSTaskResume((OS_TCB*)&UsartSendTaskTCB,&err);
		
		TaskSched();
	}
}

/*usart发送任务函数*/
void usart_send_task(void *p_arg){
	OS_ERR err;
	p_arg = p_arg;
	while(1){
		if (USART_TX_HAD) {
			USARTSend(USART_TX_DO_ADDR, USART_TX_DO_LEN, MAIN_COM);					
			//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err); //延时
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
		LED1=~LED1;		//呼吸灯
		delay_ms(500);
		
		if(DIP_ON == READ_DIP_SW(2) && dip != 1) //启动
		{
					
			dip = 1;
		}
		else if(DIP_OFF == READ_DIP_SW(2) && dip != 0)//退出
		{
			dip = 0;
		}
	}
}

//定时器1的回调函数
//用于联机状态下的串口收
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
