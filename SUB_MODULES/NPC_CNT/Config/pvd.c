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
	#define PVD_FLAG PVD_BASE    //����pvd��ǣ�1��ʾ����pvd������ִ���е��磩��0��ʾδ����pvd������δִ�е��磩
	#define REG_ADDR             PVD_FLAG+4				          //g_modbusReg�Ĵ���ֵ�洢��ַ
  #define INREG_ADDR			     REG_ADDR+REG_N*2				    //g_modbusInRegֻ���Ĵ���ֵ�洢��ַ
  #define COIL_ADDR			       INREG_ADDR+INREG_N*2			  //g_modbusCoil��Ȧֵ�洢��ַ
  #define INCOIL_ADDR			     COIL_ADDR+COIL_CHN				  //g_modbusInCoilֻ����Ȧֵ�洢��ַ
	#define WORKER_ADDR          INCOIL_ADDR+INCOIL_CHN
	
	
	volatile u32 g_pvd_flag = 0;
	
/**
 * @brief ���� PVD.
 * @param None
 * @retval None
 */
void PVD_Config(void) {
 NVIC_InitTypeDef NVIC_InitStructure;
 EXTI_InitTypeDef EXTI_InitStructure;

 /*ʹ�� PWR ʱ�� */
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

 /* ʹ�� PVD �ж� */
 NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);

 /* ���� EXTI16 ��(PVD ���) �����������½����ж�*/
 EXTI_ClearITPendingBit(EXTI_Line16);
 EXTI_InitStructure.EXTI_Line = EXTI_Line16;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);

 //���� PVD ���� 5
 // (PVD ����ѹ����ֵΪ 2.7V�� VDD ��ѹ���� 2.7V ʱ���� PVD �жϣ�
 //�������ݿɲ�ѯ�����ֲ��֪)
 /*���弶������Լ���ʵ��Ӧ��Ҫ������*/
 PWR_PVDLevelConfig(PWR_PVDLevel_5);

 /* ʹ�� PVD ��� */
 PWR_PVDCmd(ENABLE);
 if(FLASH_OB_GetBOR() != OB_BOR_LEVEL1)
 {
   FLASH_OB_Unlock();
   FLASH_OB_BORConfig(OB_BOR_LEVEL1);
   FLASH_OB_Launch();
   FLASH_OB_Lock();
 }
}


/**
  * @brief  
  * @param  
  * @retval 0 success ; 1 failure
  */
u8 PVD_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	ASSERT(IS_FLASH_ADDRESS(WriteAddr) && !(WriteAddr%4) && !(NumByteToWrite%4));
	
	if (FLASH_If_Write16(&WriteAddr, (uint16_t*) pBuffer, (uint16_t) NumByteToWrite/2)  == 0)
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
  * @note WriteAddr NumByteToWrite��Ϊ4��������
  */
u8 PVD_Flash_Read(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	u8* pFrom = (u8*)WriteAddr;
	u8* pTo = pBuffer;
	ASSERT(IS_FLASH_ADDRESS(WriteAddr) && !(WriteAddr%4) && !(NumByteToWrite%4));
	
	//memcpy(pBuffer, (u8*) WriteAddr, NumByteToWrite);
	while( NumByteToWrite-- >0)
	{
			*pTo++ = *pFrom++;            //ֱ�Ӹ�ֵ����
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
		err = FLASH_If_DisableWriteProtection();
		if (2 == err)
			WhileError();
		//err = 1;
	}
	else
	{
		err = 0;
	}
	FLASH_If_Init();									//���� 
	FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
	//err = FLASH_If_Erase(PVD_BASE);//�����ϵ��������������ʡȥ����
	
	if(!err) {
		err = PVD_Flash_Write((CDV_INT08U *)&g_pvd_flag, PVD_FLAG, sizeof(g_pvd_flag));
		err += PVD_Flash_Write((CDV_INT08U *)&g_modbusReg, REG_ADDR, sizeof(g_modbusReg));
//		err += PVD_Flash_Write((CDV_INT08U *)&g_modbusInReg, INREG_ADDR, sizeof(g_modbusInReg));
//		err += PVD_Flash_Write((CDV_INT08U *)&g_modbusCoil, COIL_ADDR, sizeof(g_modbusCoil));
//		err += PVD_Flash_Write((CDV_INT08U *)&g_modbusInCoil, INCOIL_ADDR, sizeof(g_modbusInCoil));
//		err += PVD_Flash_Write((CDV_INT08U *)g_threadInfo, WORKER_ADDR, sizeof(g_threadInfo)*WORKER_MAX_NUM);
	}
	
	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
  
}

void PVD_Erase(void)   
{
	u32 err;
	u32 flag = 0;
	FLASH_If_Init();									//���� 
  FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
	err = FLASH_If_Erase(PVD_BASE);
//	DelayUS(100);
//  err = PVD_Flash_Write((CDV_INT08U *)&flag, PVD_FLAG, sizeof(flag));
//	DelayUS(100);
	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
	
}


void PVD_FlagClear(void)   
{
	g_pvd_flag = 0;
	
}
/**
  * @brief  
  * @param  
  * @retval -1 û�е������� 1�����������
  */
CDV_INT08S PVD_Restore(void)   
{
	u32 err;
	u32 flag;
	CDV_INT08S ret = -1;
	
	err = PVD_Flash_Read((CDV_INT08U *)&flag, PVD_FLAG, sizeof(flag));
	if(1 == flag) {
		err = PVD_Flash_Read((CDV_INT08U *)&g_modbusReg, REG_ADDR, sizeof(g_modbusReg));
//		err = PVD_Flash_Read((CDV_INT08U *)&g_modbusInReg, INREG_ADDR, sizeof(g_modbusInReg));
//		err = PVD_Flash_Read((CDV_INT08U *)&g_modbusCoil, COIL_ADDR, sizeof(g_modbusCoil));
//		err = PVD_Flash_Read((CDV_INT08U *)&g_modbusInCoil, INCOIL_ADDR, sizeof(g_modbusInCoil));
//		err = PVD_Flash_Read((CDV_INT08U *)g_threadInfo, WORKER_ADDR, sizeof(g_threadInfo));
		//�ָ�����
		//OWriteAll();
		ValToFlash(0, CDV_VAR_NUM);
		//RestartWorkers();//��Σ��
		ret = 0;
	} else {
		FlashToVal(0, CDV_VAR_NUM);
	}
	//PVD_Erase();
	return ret;
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

/** @brief  PVD �ж�����
  * @param  
  * @retval 
  * @note   
  */
void PVD_IRQHandler(void) {
  OSIntEnter();
	/* ����ж��ź�*/
	EXTI_ClearITPendingBit(EXTI_Line16);
	/*����Ƿ������ֵ�������� PVD �����ź�*/
	if(PWR_GetFlagStatus (PWR_FLAG_PVDO)==SET)  
  {
    /* ����ƣ�ʵ��Ӧ����Ӧ�������״̬���� */
		//global_start = GetCdvTimeTick();
	  g_noWrite = 1;
		g_pvd_flag = 1;
		PVD_Save();
  }
	else //����Ƿ������ֵ
  {
		g_pvd_flag = 0;
		//ResetCdv();
  }
	OSIntExit();
}
#endif
