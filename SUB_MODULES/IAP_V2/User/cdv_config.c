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

/*PERIPH_DRIVER初始化*/

//Ⅰ、Ⅱ、Ⅲ、Ⅳ（IIII）、Ⅴ、Ⅵ、Ⅶ、Ⅷ、Ⅸ、Ⅹ
u8 RomanNumerals[9][3] = {0x08, 0x08, 0x08, //Ⅰ
              0x28, 0x28, 0x28,  //Ⅱ
              0x2A, 0x2A, 0x2A,  //Ⅲ
              0x2A, 0x2A, 0x12,  //Ⅳ
              0x28, 0x28, 0x10,  //Ⅴ
              0x2A, 0x2A, 0x24,  //Ⅵ
              0xAA, 0xAA, 0xA4,  //Ⅶ
              0xEA, 0xEA, 0xE4,  //Ⅷ
              0x2A, 0x12, 0x2A};  //Ⅸ
void PeriphDriverInit(void)
{
	u8 i, dat;
	Dac1_Init();
	Dac2_Init();
	GPIO_Configuration();
	IOInit();
  for(i = 0 ; i < CDV_O_NUM/* + CDV_EXO_NUM*2 */; i ++) {
		OWrite(i , BIT_0);
	}
  LED_Init();
	SPI_Configuration();
	//EXTI_Configuration();
	USART_Configuration();
	TIM3_Init(84, 10000);//10ms

	
		
	{
		u8 in[3] = {0x00, 0X00, 0X00};
		u8 out[3] = {0xC9, 0XDD, 0X55};//低位->高位 对应 左到右
		PinConfig74HC595();
    WriteToInLed(in, 3);
		WriteToOutLed(out, 3);
	}
	
	CASCADE_CS_EN;
	for(i = 0 ; i < 1; i++)
	  dat = FLASH_ReadWriteByte(0XFF);
	CASCADE_CS_DS;

  mymem_init(SRAMCCM); 	//初始化CCM内存池
}



void NewMemory(void **p , size_t size ) {
	OS_ERR err;	
	if(0 == size)
		return;
	OSSemPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
	if(*p!=NULL) { 
      free(*p);
      *p = NULL;
    }
	
    *p = (malloc(size));
	OSSemPost (&MEM_SEM,OS_OPT_POST_1,&err);
    if(NULL == *p) 
    {
      NewError();
    }
	
}

void DelMemory(void **p) {
	OS_ERR err;	
	OSSemPend(&MEM_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
	if(*p!=NULL) { 
      free(*p);
      *p = NULL;
    }
	OSSemPost (&MEM_SEM,OS_OPT_POST_1,&err);
}

void NewError(void ) {
	u16 i = 0;
    u8	j = 0;
	g_cdvStat = CDV_NEW_ERROR;
	LED3 = LED_ON;
	Log_Write("NEW ERROR" , LOG_ERROR);
	while(1)
		if(0 == i++) {
			if(0 ==  j++)
			LED2 = ~LED2;		//呼吸灯;
		}
}

void WhileError(void ) {
	u16 i = 0;
  u8	j = 0;
	g_cdvStat = CDV_WHILE_ERROR;
	LED3 = LED_ON;
	Log_Write("WHILE ERROR" , LOG_ERROR);
	while(1)
		if(0 == i++) {
			if(0 ==  j++)
			LED2 = ~LED2;		//呼吸灯;
		}
}


void DelayUS(u32 cnt)
{
	u32 j;
	cnt=cnt*42;
	for(j=0;j<cnt;j++);
}

void CDVParamInit(void) {
//	static u8 isInited = 0;
//	LineInit();
//	if(!isInited) {
//		isInited = 1;
//	  FlashToVal(0, CDV_VAR_NUM);
//		
//	}
}


	
void assert(uint8_t* file, uint8_t* function, uint32_t line)
{ 
  char tmp[100]={0};
	char loop = 1;
	
	__disable_irq();   // 关闭总中断
	
	g_cdvStat = CDV_ASSERT;
	
	sprintf(tmp , "iap assert occur! file:%s\r\nfunction:%s\r\nline:%d\r\n" 
	,file 
	,function
	,line
	);
	
  while (loop)
  {
		USARTSend((CDV_INT08U*)tmp, strlen(tmp), MAIN_COM);
		DelayUS(5000000);
  }
	
	//__enable_irq();
}

/**
  * @brief  重启CDV
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
void ResetCdv(void){
	Log_Write("Reset Cdv" , LOG_EVENT);
	
	CLI();
	NVIC_SystemReset();
}
