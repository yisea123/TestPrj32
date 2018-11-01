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


#if _NPC_VERSION_ == 2u
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

  mymem_init(SRAMCCM); 	//初始化CCM内存池
  USART_Configuration();
#if _NPC_VERSION_ > 2u
	FSMC_SRAM_Init();
#if USE_MEMMNG == 1u
	memmng_init();
#endif
#if USE_COUNTER == 1u
  Counter_init();
#endif
//	fsmc_sram_test();
//#if USE_PVD == 1u
//	//g_dipCtrlWorker = PVD_Restore();
//	PVD_Config();
//#endif


#endif
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
	OSSemPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
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
	OSSemPost (&MEM_SEM,OS_OPT_POST_1,&err);
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
	
	OSSemPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
#if USE_MEMMNG == 1u
  *p = (memmng_realloc(*p ,size));
#else
  *p = (realloc(*p ,size));
#endif
	OSSemPost (&MEM_SEM,OS_OPT_POST_1,&err);
	
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
	OSSemPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
	
	if(*p!=NULL) { 
#if USE_MEMMNG == 1u
    memmng_free(*p);
#else
    free(*p);
#endif
    *p = NULL;
  }
	
	OSSemPost (&MEM_SEM,OS_OPT_POST_1,&err);
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
#if _NPC_VERSION_ > 1u
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
  * @note   主串口如果与触摸屏设备连，则不能开启调试
  */
void com_print(const char* str)
{
		USARTSend((CDV_INT08U*)str, strlen(str), MAIN_COM);
}

