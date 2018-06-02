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
//void tmr2_callback(void *p_tmr, void *p_arg);	//定时器2回调函数，wifi
//void tmr3_callback(void *p_tmr, void *p_arg);	//定时器2回调函数，串口缓存

CPU_STK START_TASK_STK[START_STK_SIZE];/*任务堆栈*/
void start_task(void *p_arg);/*任务函数*/

CPU_STK USART_TASK_STK[USART_RECV_STK_SIZE];//任务堆栈
void usart_recv_task(void *p_arg);

CPU_STK LED1_TASK_STK[USART_SEND_STK_SIZE];//任务堆栈
void usart_send_task(void *p_arg);//任务函数

CPU_STK FLOAT_TASK_STK[CDV_REFRESH_STK_SIZE];//任务堆栈
void cdv_refresh_task(void *p_arg);//任务函数

CPU_STK WORKER_MANAGE_TASK_STK[WORKER_MANAGE_STK_SIZE];//任务堆栈
void worker_manage_task(void *p_arg);//任务函数
//void worker_task(void *p_arg);//任务函数

CPU_STK PARSE_TASK_STK[PARSE_STK_SIZE];//任务堆栈
void parse_task(void *p_arg);//任务函数

CPU_STK VALVE_TASK_STK[CDV_VALVE_STK_SIZE];//任务堆栈
void cdv_Valve_task(void *p_arg);//任务函数

CPU_STK MOTO_TASK_STK[CDV_MOTO_STK_SIZE];//任务堆栈
void cdv_Moto_task(void *p_arg);//任务函数
///*
// *测试函数
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
	delay_init(168);                               //时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	INTX_DISABLE();                                //关中断,防止滴答定时器对外设初始化的打扰
	PeriphDriverInit();                            //初始化
	
	OSInit(&err);                                  //初始化 UCOSIII
	
	INTX_ENABLE();                                 //开中断	
	
	
#if _NPC_VERSION_ > 1u
#if USE_NPC_NET
//	if(!lwip_comm_init()) 		//lwip初始化
//	{
//		tcp_server_init();
//	}
/* configure Ethernet (GPIOs, clocks, MAC, DMA) */ 
  ETH_BSP_Config();

  /* Initilaize the LwIP stack */
  LwIP_Init();

  /* Initialize webserver demo */
  tcp_server_init();
//	while(tcp_server_init()) 									//初始化tcp_server(创建tcp_server线程)
//	{	
//		delay_ms(500);
//	}
  httpd_init();
	
	udpecho_init();
#endif
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
	OSSchedRoundRobinCfg(DEF_ENABLED, 5, &err);//使能时间片轮转调度功能,时间片长度为 1 个系统时钟节拍
	#endif
	//创建信号量							
	OSSemCreate ((OS_SEM* )&TX_SEM, //指向信号量
								(CPU_CHAR* )"TX_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);
													
//	OSSemCreate ((OS_SEM* )&VAL_SEM, //指向信号量
//								(CPU_CHAR* )"VAL_SEM", //信号量名字
//								(OS_SEM_CTR )1, //信号量值为 1
//								(OS_ERR* )&err);
//	OSSemCreate ((OS_SEM* )&SPI_SEM, //指向信号量
//								(CPU_CHAR* )"SPI_SEM", //信号量名字
//								(OS_SEM_CTR )1, //信号量值为 1
//								(OS_ERR* )&err);
	OSSemCreate ((OS_SEM* )&WORKER_SEM, //指向信号量
								(CPU_CHAR* )"WORKER_SEM", //信号量名字
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
	OSSemCreate ((OS_SEM* )&MSG_SEM, //指向信号量
								(CPU_CHAR* )"MSG_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);
//	OSSemCreate ((OS_SEM* )&WIFI_SEM, //指向信号量
//								(CPU_CHAR* )"WIFI_SEM", //信号量名字
//								(OS_SEM_CTR )1, //信号量值为 1
//								(OS_ERR* )&err);	
//	OSSemCreate ((OS_SEM* )&PROMPT_SEM, //指向信号量
//								(CPU_CHAR* )"PROMPT_SEM", //信号量名字
//								(OS_SEM_CTR )1, //信号量值为 1
//								(OS_ERR* )&err);							
	
//	OSSemCreate ((OS_SEM* )&CACHE_SEM, //指向信号量
//								(CPU_CHAR* )"CACHE_SEM", //信号量名字
//								(OS_SEM_CTR )1, //信号量值为 1
//								(OS_ERR* )&err);		
								
	OSSemCreate ((OS_SEM* )&GENERAL_SERIAL_SEM, //指向信号量
								(CPU_CHAR* )"GENERAL_SERIAL_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);			
	
//	for (i = 0; i < 6; i++) {
//		char buf[10];
//		sprintf(buf, "COM_SEM%d", i);
//	  OSSemCreate ((OS_SEM* )&COM_SEM[i], //指向信号量
//									(CPU_CHAR* )buf, //信号量名字
//									(OS_SEM_CTR )1, //信号量值为 1
//									(OS_ERR* )&err);		
//	}

	OSSemCreate ((OS_SEM* )&TCP_TX_SEM, //指向信号量
								(CPU_CHAR* )"TCP_TX_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);		
//创建信号量							
	OSSemCreate ((OS_SEM* )&PLUSE_SEM, //指向信号量
								(CPU_CHAR* )"PLUSE_SEM", //信号量名字
								(OS_SEM_CTR )1, //信号量值为 1
								(OS_ERR* )&err);
	/*FPGA程序接收下载控制*/
//	SPI_Flash_Read((CDV_INT08U *)&SRP_NUM_RUN,SCRIP_NUM_RUN,2);
//	SPI_Flash_Read((CDV_INT08U *)&SRP_NUM_RUN,SCRIP_NUM_RUN,2);
//	SPI_Flash_Read((CDV_INT08U *)&SRP_NUM_DEBUG,SCRIP_NUM_DEBUG,2);
	//Log_Write("START DOWNLOAD FPGA" , LOG_EVENT);
	
#if _NPC_VERSION_ == 2u
	FpgaRecvCtl();
#endif
	CDVParamInit();
			
#if USE_PVD == 1u
	g_dipCtrlWorker = PVD_Restore();
	PVD_Config();
	PVD_Erase();
#endif

#if ENABLE_FPGA_DOWN
//	for(i = CDV_O_NUM ; i < CDV_O_NUM + CDV_EXO_NUM*2 ; i ++) {
//		OWrite(i , BIT_0);
//		//IRead(i);
//	}
	ExIOInit();	
	/*读取flash参数*/

	if(FPGA_Mode == FPGA_MOTOR_CODE)
	{
		MotoResParRead();
		for(i = 0 ; i < CDV_FPGA_NUM*2;i++)
		{
			Moto_Init(i,0xffffffff);//初始化电机参数
		}	
	}
	//for(i = 0 ; i < CDV_FPGA_NUM;i++)
//	{
//		Line_Arc_Config(1);//轴组参数初始化
//	}	
	
#endif
	//MODBUS_SET_UPDATE;

	
//	//Fpga_Config(1, FPGA_MOTOR);
//	OS_CRITICAL_ENTER(); //进入临界区
//	//Fpga_Down(1, FPGA_MOTOR);
//	OS_CRITICAL_EXIT(); //进入临界区		
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
//	OSTmrCreate((OS_TMR		*)&tmr2,		
//                (CPU_CHAR	*)"tmr2",		
//                (OS_TICK	 )0,			//N*10=3000ms	
//                (OS_TICK	 )2,   					
//                (OS_OPT		 )OS_OPT_TMR_PERIODIC,//OS_OPT_TMR_ONE_SHOT, 	//单次定时器
//                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//定时器2回调函数
//                (void	    *)0,			
//                (OS_ERR	    *)&err);	
//								//创建定时器2
//	OSTmrCreate((OS_TMR		*)&tmr3,		
//                (CPU_CHAR	*)"tmr3",		
//                (OS_TICK	 )0,			//N*10=3000ms	
//                (OS_TICK	 )2,   					
//                (OS_OPT		 )OS_OPT_TMR_PERIODIC,//OS_OPT_TMR_ONE_SHOT, 	//单次定时器
//                (OS_TMR_CALLBACK_PTR)tmr3_callback,	//定时器3回调函数
//                (void	    *)0,			
//                (OS_ERR	    *)&err);	
							
//		if(DIP_ON == READ_DIP_SW(2)) //主机模式
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
			
		if(debug)//退出
		{
			WorkerControl(1, 2);
			if(debug)//退出
			{
				WorkerControl(0, 2);
			}
			return;
		}

	//OS_CRITICAL_ENTER(); //进入临界区
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
		//创建任务解析
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
		if(debug)//退出
		{
			WorkerControl(1, 2);
			if(debug)//退出
			{
				WorkerControl(0, 2);
			}
			return;
		}
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
	//创建比例阀控制任务
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
		
	//创建工人管理启动任务
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
		
	//创建电机控制任务
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
	//创建工人管理启动任务
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

	 // OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err); //挂起开始任务
	  //OS_CRITICAL_EXIT(); //进入临界区		
		OSTmrStart(&tmr1,&err);

		OSTaskDel((OS_TCB*)0,&err); //删除 start_task 任务自身
}



/*usart 接收任务函数*/
void usart_recv_task(void *p_arg){
	OS_ERR err;
	RET_STATUS ret = OPT_SUCCESS;
	CDV_INT32U tmp1 = 0, tmp2 = 0;
	p_arg = p_arg; 
	while(1)	{
		u32 clk, clk2;
		if(USART_RX_LEN)//判断当前缓存是否为0
		{
			if(tmp1 != tm1Re)
			{
				tmp1 = tm1Re;
				clk = 0;
			}
			else
			{
				if(CalcTimeMS(GetCdvTimeTick(), tmp1) > 2)//超过2ms，认为一条命令结束
					if(++clk > 1)
					  USART_RX_QUEUE_SELF_ADD;
			}
		}
#if _NPC_VERSION_ > 1u
		//级联命令的接收
		if(GetUsartRxLen())//判断当前缓存是否为0
		{
			if(tmp2 != tm2Re)
			{
				tmp2 = tm2Re;
				clk2 = 0;
			}
			else
			{
				if(CalcCount(ReadClock1ms(), tmp2) > 2)//超过2ms，认为一条命令结束
					if(++clk2 > 1)
					{//接收结束，
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
		
		//有命令，进行初解析
		if(USART_RX_HAD) 
		{
				if(OPT_SUCCESS == RecvParse(g_uartRx.QUEUE.rxBuf[g_uartRx.doPos],g_uartRx.QUEUE.rxLen[g_uartRx.doPos], MAIN_COM))
				{
				  USART_RX_QUEUE_DO_NEXT;                                    /*转移到接收队列的下一条*/
				}
		}
		else 
		{
			//OS_TaskSuspend((OS_TCB*)&UsartRecvTaskTCB,&err);
		}
		
		if(g_olCache.mark || g_portCmdCache.mark)//如果有解析，启动解析线程
		{
			OSTaskResume((OS_TCB*)&TaskParseTCB,&err);		
		}
		
		if (USART_TX_HAD)//如果有命令要发送，启动发送线程
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
#if _NPC_VERSION_ > 1u
				CascadeGetNativeNo() == g_olCache.buf[3] || 
#endif
				(KFC_IC == g_olCache.buf[0] && (0x10 > g_olCache.buf[1]||0x30 == g_olCache.buf[1])) ||
				(KFC_IC == g_olCache.buf[0] && 0x10 == g_olCache.buf[1] && 0x41 == g_olCache.buf[5])||
			   KFC_IC != g_olCache.buf[0] ) 
			{
				CMD_ARG arg;
				CmdArgInit(&arg);
				arg.uart = g_olCache.uart;
				arg.len = g_olCache.len;
				arg.buf =	g_olCache.buf;
#if _NPC_VERSION_ > 1u
				arg.hostid = CascadeGetNativeNo();
#endif
				
				CmdParse(g_olCache.buf, g_olCache.len, &arg);
				ClearOnlineCmdCache();
				/*处理资源回复请求*/
				DoResRequest(&arg);
				CmdArgDelete(&arg);
				
			}
#if _NPC_VERSION_ > 1u
			else//命令给从机
			{
				CMD_ARG arg;
				CmdArgInit(&arg);
				arg.uart = g_olCache.uart;
				arg.len = g_olCache.len;
				arg.buf =	g_olCache.buf;
				arg.hostid = CascadeGetNativeNo();
				
				TFUsartCmdParse(g_olCache.buf, g_olCache.len, &arg);
				
				/*处理资源回复请求*/
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
//					if(recvLen - g_olCache.len == 7)//如果有变量操作，则进行隐射
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
#if _NPC_VERSION_ > 1u
		else if (g_portCmdCache.mark)//级联的解析
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
				/*处理资源回复请求*/
				if(OPT_FAILURE == DoSpecRequest(&arg))
				  DoResRequest(&arg);
				
				CmdArgDelete(&arg);
				ClearPortCmdCache();
		}	
#endif
		else 
		{
			LED3 = LED_OFF;
			OS_TaskSuspend((OS_TCB*)&TaskParseTCB,&err);               /*挂起任务*/
			LED3 = LED_ON;
		}
		
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
//比例阀调节任务
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
//						,"比例阀输出气压:" 
//			      ,(u32)Present_Pressure_Rel
//			      ,"传感器气压：" 
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

void cdv_refresh_task(void *p_arg){
	
  OS_ERR err;
	WORKER_STATUS stat;
	CDV_INT08U i;
	u32 ftime, stime;
  ftime = GetCdvTimeTick();
	while(1)	{
		////CascadeModbus_AllUpdate();
		CascadeModbus_Map();
		stime = GetCdvTimeTick();
		if (500 < CalcTimeMS(stime,ftime ))
		{
			ftime = stime;
//		  LED1=~LED1;		//呼吸灯
//		
//			//delay_ms(500);
//			data1 = Read_Input_All();
//			data2 = Read_Output_ALL();
//			IN_DisPlay(data1);          //输入状态显示
//			OUT_DisPlay(data2);         //输出状态显示
//			Adc_Voltge ();
//		  // Flash_Check();
//			UpdateDacVal();
//			UpdateAdcVal();
//			
//			if(DIP_ON == READ_DIP_SW(2) && g_dipCtrlWorker != 1) //启动
//			{
//			//if(!Cascade_HaveSlaveTable()) //主机模式
//			//{
//			//	CascadeCombine(0x00);
//			//}
//			//	Test();
//				WorkerControl(0, WORKER_LOOP);//隐藏
//				WorkerControl(1, WORKER_ONCE);//监工
//				g_dipCtrlWorker = 1;
//			}
//			else if(DIP_OFF == READ_DIP_SW(2) && g_dipCtrlWorker != 0)//退出
//			{
//				AllWorkerCtrl(WORKER_STOP);
//				g_dipCtrlWorker = 0;
//			}
//			
//			
//			if(debug)//退出
//			{
//				WorkerControl(debug1, debug2);
//			}
		}
//#if USE_PVD == 1u
//		if(PVD_GetFlag()) {
//			ShutDown();
//		}
//#endif

//#if _NPC_VERSION_ > 1u
//#if USE_NPC_NET
//  Eth_Link_query();
////#else
////	OS_TaskSuspend((OS_TCB*)&WorkerManageTaskTCB,&err);
//#endif
//#endif
	}
}

/*本机外设刷新任务*/
void worker_manage_task(void *p_arg){
	
	OS_ERR err;
  //刷新

	while (1) {
			
		int i=0;
		
		IReadAll();

		OReadAll();
#if ENABLE_FPGA_DOWN
		ExIReadAll();
		ExOReadAll();
		ExOWriteAll();
#endif
		//delay_ms(10);
		
		memmng_test();
		
#if USE_PVD == 1u
		if(PVD_GetFlag()) {
			ShutDown();
		}
#endif
    TaskSched();
	}
}

#if ENABLE_FPGA_DOWN
//电机控制任务
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
/*传入的是工人id，从0-7*/

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
//				OSSemPend(&VAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
//				WORKER_RUN_NUM--;
//			  OSSemPost (&VAL_SEM,OS_OPT_POST_1,&err);
//				OS_TaskSuspend((OS_TCB*)&WorkerTaskTCB[*pArg],&err);               /*挂起任务*/
//			  OSSemPend(&VAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
//			  WORKER_RUN_NUM++;
//			  OSSemPost (&VAL_SEM,OS_OPT_POST_1,&err);
//				break;
//		}
//	}
//}




//定时器1的回调函数
//用于联机状态下的串口收
void tmr1_callback(void *p_tmr, void *p_arg) {
//  OS_ERR err;
	u32 data1,data2;
//	if(++tm1Re < 2)
//		return;
////	serial_state=USART3_Receive_Data2(general_serial_buf,&general_serial_count);//判断串口接收完成
////	if(serial_state==1)//接收完成
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
	LED1=~LED1;		//呼吸灯

	//delay_ms(500);
	data1 = Read_Input_All();
	data2 = Read_Output_ALL();
	IN_DisPlay(data1);          //输入状态显示
	OUT_DisPlay(data2);         //输出状态显示
	Adc_Voltge ();
	// Flash_Check();
	UpdateDacVal();
	UpdateAdcVal();
	
	if(DIP_ON == READ_DIP_SW(2) && g_dipCtrlWorker != 1) //启动
	{
	//if(!Cascade_HaveSlaveTable()) //主机模式
	//{
	//	CascadeCombine(0x00);
	//}
	//	Test();
		WorkerControl(0, WORKER_LOOP);//隐藏
		WorkerControl(1, WORKER_ONCE);//监工
		g_dipCtrlWorker = 1;
	}
	else if(DIP_OFF == READ_DIP_SW(2) && g_dipCtrlWorker != 0)//退出
	{
		AllWorkerCtrl(WORKER_STOP);
		g_dipCtrlWorker = 0;
	}
	
	
	if(debug)//退出
	{
		WorkerControl(debug1, debug2);
	}
		
#if _NPC_VERSION_ > 1u
#if USE_NPC_NET
  Eth_Link_query();
//#else
//	OS_TaskSuspend((OS_TCB*)&WorkerManageTaskTCB,&err);
#endif
#endif
}
////定时器1的回调函数
////用于联机状态下的串口收
//void tmr2_callback(void *p_tmr, void *p_arg){
//  OS_ERR err;
//	if(++tm2Re < 2)
//		return;
//	WIFI_RX_QUEUE_SELF_ADD;
//	if(WIFI_RX_HAD) {
//		OSTaskResume((OS_TCB*)&UsartRecvTaskTCB,&err);
//	}
//}
////定时器1的回调函数
////用于联机状态下的串口收
//void tmr3_callback(void *p_tmr, void *p_arg){
//  OS_ERR err;
//	if(++tm3Re < 1)
//		return;
//	//serial_state=USART3_Receive_Data2(general_serial_buf,&general_serial_count);//判断串口接收完成
////	if(serial_state==1)//接收完成
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
