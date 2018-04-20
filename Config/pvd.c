/**
  ******************************************************************************
  * @file    /pvd.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2018-2-1
  * @brief   a package of pvd
  * 
@verbatim  


@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */

	#include "pvd.h"
	#include "flash_if.h"
#if USE_PVD == 1u
	#define PVD_BASE ADDR_FLASH_SECTOR_11
	#define PVD_FLAG PVD_BASE    //保存pvd标记，1表示发生pvd（流程执行中掉电），0表示未发生pvd（流程未执行掉电）
	#define REG_ADDR             PVD_FLAG+4				          //g_modbusReg寄存器值存储地址
  #define INREG_ADDR			     REG_ADDR+REG_N*2				    //g_modbusInReg只读寄存器值存储地址
  #define COIL_ADDR			       INREG_ADDR+INREG_N*2			  //g_modbusCoil线圈值存储地址
  #define INCOIL_ADDR			     COIL_ADDR+COIL_CHN				  //g_modbusInCoil只读线圈值存储地址
	#define WORKER_ADDR          INCOIL_ADDR+INCOIL_CHN
	
	
	u32 g_pvd_flag = 0;
	
/**
 * @brief 配置 PVD.
 * @param None
 * @retval None
 */
void PVD_Config(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;
 EXTI_InitTypeDef EXTI_InitStructure;

 /*使能 PWR 时钟 */
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

 /* 使能 PVD 中断 */
 NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);

 /* 配置 EXTI16 线(PVD 输出) 来产生上升下降沿中断*/
 EXTI_ClearITPendingBit(EXTI_Line16);
 EXTI_InitStructure.EXTI_Line = EXTI_Line16;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);

 //配置 PVD 级别 5
 // (PVD 检测电压的阈值为 2.8V， VDD 电压低于 2.8V 时产生 PVD 中断，
 //具体数据可查询数据手册获知)
 /*具体级别根据自己的实际应用要求配置*/
 PWR_PVDLevelConfig(PWR_PVDLevel_5);

 /* 使能 PVD 输出 */
 PWR_PVDCmd(ENABLE);
}


/**
  * @brief  
  * @param  
  * @retval 0 success ; 1 failure
  */
u8 PVD_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	ASSERT(IS_FLASH_ADDRESS(WriteAddr) && !(WriteAddr%4) && !(NumByteToWrite%4));
	
	if (FLASH_If_Write(&WriteAddr, (uint32_t*) pBuffer, (uint16_t) NumByteToWrite/4)  == 0)
	{
		
		return 0;
	}
	else /* An error occurred while writing to Flash memory */
	{
		
		return 1;
	}
}
/**
  * @brief  
  * @param  
  * @retval
  * @note WriteAddr NumByteToWrite需为4的整数倍
  */
u8 PVD_Flash_Read(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	u8* pFrom = (u8*)WriteAddr;
	u8* pTo = pBuffer;
	ASSERT(IS_FLASH_ADDRESS(WriteAddr) && !(WriteAddr%4) && !(NumByteToWrite%4));
	
	//memcpy(pBuffer, (u8*) WriteAddr, NumByteToWrite);
	while( NumByteToWrite-- >0)
	{
			*pTo++ = *pFrom++;            //直接赋值即可
//			 
//			 pTo++;
//			 pFrom++;
	}
	
	return 0;
}

void PVD_Save(void)   
{
	
	u32 err;
	if (FLASH_If_GetWriteProtectionStatus() == 0)   
	{
		err = 1;
	}
	else
	{
		err = 0;
	}
	FLASH_If_Init();									//解锁 
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
	err = FLASH_If_Erase(PVD_BASE);
	//DelayUS(100);
	if(!err) {
		err = PVD_Flash_Write((CDV_INT08U *)&g_pvd_flag, PVD_FLAG, sizeof(g_pvd_flag));
		err += PVD_Flash_Write((CDV_INT08U *)&g_modbusReg, REG_ADDR, sizeof(g_modbusReg));
		err += PVD_Flash_Write((CDV_INT08U *)&g_modbusInReg, INREG_ADDR, sizeof(g_modbusInReg));
		err += PVD_Flash_Write((CDV_INT08U *)&g_modbusCoil, COIL_ADDR, sizeof(g_modbusCoil));
		err += PVD_Flash_Write((CDV_INT08U *)&g_modbusInCoil, INCOIL_ADDR, sizeof(g_modbusInCoil));
		err += PVD_Flash_Write((CDV_INT08U *)g_threadInfo, WORKER_ADDR, sizeof(g_threadInfo)*WORKER_MAX_NUM);
	}
	//DelayUS(100);
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁
	
}

void PVD_Erase(void)   
{
	u32 err;
	u32 flag = 0;
	FLASH_If_Init();									//解锁 
  FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
	err = FLASH_If_Erase(PVD_BASE);
//	DelayUS(100);
//  err = PVD_Flash_Write((CDV_INT08U *)&flag, PVD_FLAG, sizeof(flag));
//	DelayUS(100);
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁
	
}


void PVD_FlagClear(void)   
{
	g_pvd_flag = 0;
	
}
/**
  * @brief  
  * @param  
  * @retval -1 没有掉电重启 1掉电重启完成
  */
CDV_INT08S PVD_Restore(void)   
{
	u32 err;
	u32 flag;
	
	err = PVD_Flash_Read((CDV_INT08U *)&flag, PVD_FLAG, sizeof(flag));
	if(1 == flag) {
		err = PVD_Flash_Read((CDV_INT08U *)&g_modbusReg, REG_ADDR, sizeof(g_modbusReg));
		err = PVD_Flash_Read((CDV_INT08U *)&g_modbusInReg, INREG_ADDR, sizeof(g_modbusInReg));
		err = PVD_Flash_Read((CDV_INT08U *)&g_modbusCoil, COIL_ADDR, sizeof(g_modbusCoil));
		err = PVD_Flash_Read((CDV_INT08U *)&g_modbusInCoil, INCOIL_ADDR, sizeof(g_modbusInCoil));
		err = PVD_Flash_Read((CDV_INT08U *)g_threadInfo, WORKER_ADDR, sizeof(g_threadInfo));
		PVD_Erase();
		
		//恢复动作
		OWriteAll();
		//RestartWorkers();//很危险
		return 0;//1;
	}
	return -1;
}

/**
  * @brief  
  * @param  
  * @retval
  */
u32 PVD_GetFlag(void)   
{
	return g_pvd_flag;
}

/** @brief  PVD 中断请求
  * @param  
  * @retval 
  * @note   
  */
void PVD_IRQHandler(void) {
  OSIntEnter();
	/*检测是否产生了 PVD 警告信号*/
	if(PWR_GetFlagStatus (PWR_FLAG_PVDO)==SET)  
  {
    /* 亮红灯，实际应用中应进入紧急状态处理 */
		/*这里无法操作内部flash？ */
//	  LED1 = LED_OFF;
//		LED2 = LED_OFF;
//		LED3 = LED_OFF;
		g_pvd_flag = 1;
  }
	/* 清除中断信号*/
	EXTI_ClearITPendingBit(EXTI_Line16);
	OSIntExit();
}
#endif
