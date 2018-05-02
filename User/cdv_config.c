/**
  ******************************************************************************
  * @file    /user_config.c 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   a package of stm32 device config and operate function��something like ThirdPart
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

/** @brief  PERIPH_DRIVER��ʼ��
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
	EXTI_Configuration();
	//USART_Configuration();
	TIM3_Init(84, 10000);//10ms
	LED_Init();                                   //��Դ��ʼ��
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

	Dac1_Init();//DACͨ��1�����ʼ��
	Dac2_Init();//DACͨ��2�����ʼ��
	DAC7512_Pin_Config();
//	GetTable();
	CASCADE_CS_EN;
	for(i = 0 ; i < 1; i++)
	  dat = FLASH_ReadWriteByte(0XFF);
	CASCADE_CS_DS;
	
	//Org_Flash_Read((CDV_INT08U *)&dac_par, DAC_PAR_ADDR, sizeof(dac_par));//��flash��DAC�Ĳ���
	Org_Flash_Read((CDV_INT08U *)&dac_par, DAC_PAR_ADDR, sizeof(dac_par));//��flash��DAC�Ĳ���
	Org_Flash_Read((CDV_INT08U *)&adc_par, ADC_PAR_ADDR, sizeof(adc_par));//��flash��ADC�Ĳ���
	
	
#ifdef ENABLE_PID
	Org_Flash_Read((CDV_INT08U *)&Valve_Pid, PID_PAR_ADDR, sizeof(adc_par)); //��flash��PID�Ĳ���


	PIDInit_Valve();	
	Valve_PSensor_Coe_Init();
#endif
	
#if ENABLE_FPGA_DOWN
//	SPI1_Init();//FPGA spi����
  Fpga_Config_Init();
#endif

  mymem_init(SRAMCCM); 	//��ʼ��CCM�ڴ��
  USART_Configuration();
#if _NPC_VERSION_ > 2u
	FSMC_SRAM_Init();
#if USE_MEMMNG == 1u
	memmng_init();
#endif
//	fsmc_sram_test();
#if USE_PVD == 1u
	//g_dipCtrlWorker = PVD_Restore();
	PVD_Config();
#endif
#endif
}
/** @brief  
  * @param  
  * @retval 
  * @note   ��PVD�������ʹ��
  */
void ShutDown(void) {
	OS_ERR err;
//	__disable_irq();   // �ر����ж�
	
#if USE_PVD == 1u
	INTX_DISABLE();
	PVD_Save();
	AllWorkerCtrl(WORKER_STOP);
	// PVD_FlagClear();
	g_noWrite = 1;
	INTX_ENABLE();
	OS_TaskSuspend((OS_TCB*)&UsartRecvTaskTCB,&err);
	while(PVD_GetFlag());
#endif
	ResetCdv();
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
	OSSemPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
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
	
	OSSemPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
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
	OSSemPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	
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
	
//ʹ������sram�з���
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
	Log_Write("NEW ERROR" , LOG_ERROR);
	while(sw)
		if(0 == i++) {
			if(0 ==  j++)
			LED2 = ~LED2;		//������;
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
	Log_Write("WHILE ERROR" , LOG_ERROR);
	while(sw)
		if(0 == i++) {
			if(0 ==  j++)
			LED2 = ~LED2;		//������;
		}
}
/** @brief  
  * @param  
  * @retval 
  * @note   1US168��ָ��
  *         102261126
  */
void DelayUS(u32 cnt)
{
	u32 j;
	cnt=cnt*42;
	for(j=0;j<cnt;j++);//���4��ָ��
}
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
#if USE_FLASH_BAK
		FlashBak_VarRestore();
#elseif USE_PVD == 0u
	  FlashToVal(0, CDV_VAR_NUM);
#endif
#if _NPC_VERSION_ > 1u
		GetTable();
#endif
	}
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
	
	__disable_irq();   // �ر����ж�
	
	g_cdvStat = CDV_ASSERT;
	
	sprintf(tmp , "assert occur! file:%s\r\nfunction:%s\r\nline:%d\r\n" 
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

