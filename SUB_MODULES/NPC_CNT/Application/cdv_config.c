/**
  ******************************************************************************
  * @file    /user_config.c 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   a package of stm32 device config and operate function，something like ThirdPart
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
	
	#include "cdv_config.h"
	u8 SPItime_Flag = 0; 

	CDV_INT08S g_dipCtrlWorker = -1;
	CDV_INT08S g_noWrite = 0;

/** @brief  PERIPH_DRIVER初始化
  * @param  
  * @retval 
  * @note   
  */
void PeriphDriverInit(void)
{
	u8 i, dat;
	GPIO_Configuration();
	
	CdvInit();  
	
	SPI_Configuration();
	//EXTI_Configuration();
	//USART_Configuration();
	TIM3_Init(84, 10000);//10ms
	LED_Init();                                   //资源初始化
#ifdef __ENABLE_RTC
	My_RTC_Init();
#endif


#if _NPC_VERSION_ == 1u
	ADC1_DMA_Init();
	Adc2_Dma_Init();
	Adc3_Dma_Init();	
#elif _NPC_VERSION_ == 2u
  ADC1_DMA_Init();
	Adc2_Dma_Init2();
	Adc3_Dma_Init();	
#elif _NPC_VERSION_ == 3u
	Adc2_Dma_Init4();
#endif

	Dac1_Init();//DAC通道1输出初始化
	Dac2_Init();//DAC通道2输出初始化
	DAC7512_Pin_Config();
//	GetTable();
	CASCADE_CS_EN;
	for(i = 0 ; i < 1; i++)
	  dat = FLASH_ReadWriteByte(0XFF);
	CASCADE_CS_DS;
	
	//Org_Flash_Read((CDV_INT08U *)&dac_par, DAC_PAR_ADDR, sizeof(dac_par));//从flash读DAC的参数
	//Org_Flash_Read((CDV_INT08U *)&dac_par, DAC_PAR_ADDR, sizeof(dac_par));//从flash读DAC的参数
	//Org_Flash_Read((CDV_INT08U *)&adc_par, ADC_PAR_ADDR, sizeof(adc_par));//从flash读ADC的参数
	
	
#ifdef ENABLE_PID
	Org_Flash_Read((CDV_INT08U *)&Valve_Pid, PID_PAR_ADDR, sizeof(adc_par)); //从flash读PID的参数


	PIDInit_Valve();	
	Valve_PSensor_Coe_Init();
#endif
	
#if ENABLE_FPGA_DOWN
//	SPI1_Init();//FPGA spi配置
  Fpga_Config_Init();
#endif

  USART_Configuration();

#if USE_MEMMNG == 1u
	
	memmng_init();
	
#endif
  mymem_init(SRAMCCM); 	//初始化CCM内存池
#if USE_COUNTER == 1u
  Counter_init();
#endif
//	fsmc_sram_test();
//#if USE_PVD == 1u
//	//g_dipCtrlWorker = PVD_Restore();
//	PVD_Config();
//#endif


}
/** @brief  
  * @param  
  * @retval 
  * @note   在PVD的配合下使用
  */
void ShutDown(void) {
	OS_ERR err;
//	__disable_irq();   // 关闭总中断
	
		RCC_AHB1PeriphResetCmd(0X22E011FF, ENABLE);
		RCC_APB1PeriphResetCmd(0xE6FEC9FF, ENABLE);
		RCC_APB2PeriphResetCmd(0x00377F33, ENABLE);
#if USE_PVD == 1u
	INTX_DISABLE();
	//PVD_Save();
	AllWorkerCtrl(WORKER_STOP);
	// PVD_FlagClear();
	g_noWrite = 1;
	INTX_ENABLE();
	OS_TaskSuspend((OS_TCB*)&UsartRecvTaskTCB,&err);
	//Sys_Enter_Standby();
	while(PVD_GetFlag());
#endif
	ResetCdv();
}
/** @brief  唯一识别码
  * @param  
  * @retval 
  * @note   
  */
CDV_INT32U GetOnlyID(void) {
	u32 sn0;
	sn0=*ONLY_ID_ADDR;
	return sn0;
}
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void NewMemory(void **p , size_t size ) {
	OS_ERR err;	
	if(0 == size)
		return;
	OSMutexPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
	if(*p!=NULL) { 
#if USE_MEMMNG == 1u
      memmng_free(*p);
#else
      free(*p);
#endif
      //free(*p);
      *p = NULL;
    }
#if USE_MEMMNG == 1u
    *p = (memmng_malloc(size));
#else
    *p = (malloc(size));
#endif
	OSMutexPost (&MEM_SEM,OS_OPT_POST_NO_SCHED,&err);
    if(NULL == *p) 
    {
      NewError();
    }
	memset(*p, 0, size);
}
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void ReNewMemory(void **p , size_t size ) {
	OS_ERR err;	
	
	if(0 == size)
		return;
	
	OSMutexPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
#if USE_MEMMNG == 1u
  *p = (memmng_realloc(*p ,size));
#else
  *p = (realloc(*p ,size));
#endif
	OSMutexPost (&MEM_SEM,OS_OPT_POST_NO_SCHED,&err);
	
	if(NULL == *p) 
	{
		NewError();
	}
	
}
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void DelMemory(void **p) {
	OS_ERR err;	
	OSMutexPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
	
	if(*p!=NULL) { 
#if USE_MEMMNG == 1u
    memmng_free(*p);
#else
    free(*p);
#endif
    *p = NULL;
  }
	
	OSMutexPost (&MEM_SEM,OS_OPT_POST_NO_SCHED,&err);
}
		
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void* MemCpy(void* dst, const void* src, size_t n)  
{  
	char *tmp;  
	char *s_src;  
	size_t s_n;
	
	ASSERT(dst);
	ASSERT(src);
	ASSERT(((u32)dst+n <= (u32)src) || ((u32)src+n <= (u32)dst));
	
memcpy_again:	
	s_n = n;
  tmp = (char*)dst;  
	s_src = (char*)src;  
	while(s_n--) {  
		*tmp++ = *s_src++;  
	}  
	
//使用外扩sram有风险
#if USE_MEMMNG == 1u
	s_n = n;
  tmp = (char*)dst;  
	s_src = (char*)src;  
	while(s_n--) {  
		//ASSERT(*tmp++ == *s_src++);  
		if(*tmp++ != *s_src++)
			goto memcpy_again;
	}  
#endif
	return dst;  
}  
		
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void* MemCpyUnsafe(void* dst, const void* src, size_t n)  
{  
	char *tmp;  
	char *s_src;  
	size_t s_n;
	
	ASSERT(dst);
	ASSERT(src);
	
	s_n = n;
  tmp = (char*)dst;  
	s_src = (char*)src;  
	while(s_n--) {  
		*tmp++ = *s_src++;  
	}  
	
	return dst;  
}  

/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void* MemMove(void* dst, const void* src, size_t n)  
{  
	char* s_dst;  
	char* s_src;  
	size_t s_n = n;
	ASSERT(dst);
	ASSERT(src);
	s_dst = (char*)dst;  
	s_src = (char*)src;  
	if(s_dst>s_src && (s_src+n>s_dst)) { 
		s_dst = s_dst+n-1;  
		s_src = s_src+n-1;  
		while(n--) {  
			*s_dst-- = *s_src--;  
		}  
	} else {  
		while(n--) {  
			*s_dst++ = *s_src++;  
		}  
	}  
	return dst;  
}  

		
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void NewError(void ) {
	u16 i = 0;
    u8	j = 0;
	u8 sw = 1;
	g_cdvStat = CDV_NEW_ERROR;
	LED3 = LED_ON;
	//Log_Write("NEW ERROR" , LOG_ERROR);
	while(sw)
		if(0 == i++) {
			if(0 ==  j++)
			LED2 = ~LED2;		//呼吸灯;
		}
}
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void WhileError(void ) {
	u16 i = 0;
  u8	j = 0;
	u8 sw = 1;
	g_cdvStat = CDV_WHILE_ERROR;
	LED3 = LED_ON;
	//Log_Write("WHILE ERROR" , LOG_ERROR);
	while(sw)
		if(0 == i++) {
			if(0 ==  j++)
			LED2 = ~LED2;		//呼吸灯;
		}
}
/** @brief  
  * @param  
  * @retval 
  * @note   1US168条指令
  *         102261126
  */
void DelayUS(u32 cnt)
{
	u32 j;
	cnt=cnt*42;
	for(j=0;j<cnt;j++);//大概4条指令
}
///** @brief  
//  * @param  
//  * @retval 
//  * @note   1US168条指令
//  *         102261
//  */
//void DelayMS(u32 cnt)
//{
//	u32 j;
//	cnt=cnt*42000;
//	for(j=0;j<cnt;j++);//大概4条指令
//}
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void CDVParamInit(void) {
	static u8 isInited = 0;
	LineInit();
	if(!isInited) {
		isInited = 1;
#if USE_PVD == 0u && USE_EXTI_POWER_OFF == 0u
	  FlashToVal(0, CDV_VAR_NUM);
#endif
	}
#if USE_CASCADE == 1u
		GetTable();
#endif
}
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void SPI_Flash_Write_Check(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	CDV_INT08U i;
  CDV_INT08U *Readbuf;
//	CDV_INT08U* Readbuf = NULL;
//	NEW08U(Readbuf , 1000);	
	SPI_Flash_Read((CDV_INT08U *)&Readbuf,WriteAddr ,NumByteToWrite);
   for( i=0;i<NumByteToWrite;i++)
	{
	 if (Readbuf[i] == pBuffer[i])
		 ;
	 	
	 else
		 if(SPItime_Flag == 1)
		 {
			SPItime_Flag = 0; 
		  SPI_Flash_Write(pBuffer,WriteAddr,NumByteToWrite);
		 }
	  if( Alarmflag == 1)
		 break;
	 }		
}
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void assert(uint8_t* file, uint8_t* function, uint32_t line)
{ 
  char tmp[100]={0};
	char loop = 1;
	
	__disable_irq();   // 关闭总中断
	
	g_cdvStat = CDV_ASSERT;
	
	sprintf(tmp , "npc assert occur! file:%s\r\nfunction:%s\r\nline:%d\r\n" 
	,file 
	,function
	,line
	);
	
  while (loop)
  {
		USARTSend((CDV_INT08U*)tmp, strlen(tmp), MAIN_COM);
		DelayUS(5000000);
  }
	
	__enable_irq();
}
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void assert_plus(uint8_t* file, uint8_t* function, uint32_t line, char* str, int len, CMD_ARG *arg)
{ 
	#define NUM_ASSERT 200
  char tmp[NUM_ASSERT]={0};
	char loop = 1;
	
	__disable_irq();   // 关闭总中断
	
	g_cdvStat = CDV_ASSERT;
	
	sprintf(tmp , "npc assert occur! file:%s\r\nfunction:%s\r\nline:%d\r\n" 
	,file 
	,function
	,line
	);
	
	if(arg && arg->ptrWorker) {
		sprintf(tmp + strlen(tmp), "worker:%d , line:%d\r\n" 
		,((DEBUG_SCRIPT*)(arg->ptrWorker))->which
		,((DEBUG_SCRIPT*)(arg->ptrWorker))->doNo
		);
	}
	
	
	if(len && str)
	  if(strlen(tmp)+len < NUM_ASSERT) {
		  MemCpy(tmp+strlen(tmp),str,len);
			MemCpy(tmp+strlen(tmp)+len,"\r\n",2);
		}
	
  while (loop)
  {
		USARTSend((CDV_INT08U*)tmp, strlen(tmp), MAIN_COM);
		DelayUS(5000000);
  }
	
	__enable_irq();
}
/** @brief  
  * @param  
  * @retval 
  * @note   主串口如果与触摸屏设备连，则不能开启调试
  */
void com_print(const char* str)
{
		USARTSend((CDV_INT08U*)str, strlen(str), MAIN_COM);
}

//============================================================ time log ============================================================

#ifdef  DEBUG_TIME
#define __TIME_LOG_SHIFT 1004//偶数
#define __TIME_LOG_LEN   200//偶数

CDV_INT32S *time_var = (CDV_INT32S*)(g_modbusReg.reg + __TIME_LOG_SHIFT);

/** @brief  
  * @param  
  * @retval 
  * @note   记录一个值到变量 1000+的，断电保存
  */
void time_log_clear(void)
{
	int i = 0;
	
	time_var[ - 1] = 0;
	
	for (i = 0; i < __TIME_LOG_LEN; i++){
		time_var[i] = 0;
	}
	
}


/** @brief  
  * @param  
  * @retval 
  * @note   记录一个值到变量 1000+的，断电保存，自动记录时间
  */
void time_log(CDV_INT32S info)
{
	CPU_SR_ALLOC();
	CDV_INT32S *ptr = NULL;
	
	CPU_CRITICAL_ENTER();
	
	ptr = time_var + -1;
	
	//CDV_INT32S *ptr = time_var + -1;
	
	if(*ptr < 0 || *ptr >= __TIME_LOG_LEN)
		*ptr = 0;
	
	time_var[(*ptr)++] = ReadClock1s();
	time_var[(*ptr)++] = info;
	
	
	if(*ptr < 0 || *ptr >= __TIME_LOG_LEN)
		*ptr = 0;
	
	OS_CRITICAL_EXIT_NO_SCHED();
}

/** @brief  
  * @param  
  * @retval 
  * @note   记录一个值到变量 1000+的，断电保存，自动记录时间
  */
void time_log_anything(CDV_INT32S info1,CDV_INT32S info2)
{
	CPU_SR_ALLOC();
	CDV_INT32S *ptr = NULL;
	
	CPU_CRITICAL_ENTER();
	
	ptr = time_var + -1;
	
	if(*ptr < 0 || *ptr >= __TIME_LOG_LEN)
		*ptr = 0;
	
	time_var[(*ptr)++] = info1;
	time_var[(*ptr)++] = info2;
	
	
	if(*ptr < 0 || *ptr >= __TIME_LOG_LEN)
		*ptr = 0;
	
	OS_CRITICAL_EXIT_NO_SCHED();
}


/** @brief  
  * @param  
  * @retval 
  * @note   发送
  */
void time_log_send( CMD_ARG *arg)
{
	CDV_INT32S ptr = time_var[ -1];
	
	char *tmp=NULL;//[USART_RX_BUF_LENGTH]={0};
	
	if(ptr < 0 || ptr >= __TIME_LOG_LEN)
		return;
	
	NEWCH(tmp,50 );
	
	while(1) {
		memset(tmp, 0, 50);
		
		if(time_var[ptr] > 0) {
		  sprintf(tmp , "system time %ds : %d00us\r\n" ,time_var[ptr], time_var[ptr+1]);
		} else {
			sprintf(tmp , "info1:%d info2:%d\r\n" ,-1*time_var[ptr], -1*time_var[ptr+1]);
		}
		AddTxNoCrcPlus((CDV_INT08U*)tmp, strlen(tmp), arg);
		
		ptr += 2;
		
		if(ptr < 0 || ptr >= __TIME_LOG_LEN)
			ptr = 0;
		
		if(ptr == time_var[ -1])
			break;
	}
	
	DELETE(tmp);
	
}

	#undef __SHIFT
#endif