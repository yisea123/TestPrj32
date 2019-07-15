/**
  ******************************************************************************
  * @file    /FlashBak.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2018-4-10
  * @brief   a package to solve the problem of lost data in flash module
  * 
@verbatim  
20180410�����ӱ�����flash���ݹ���
20180605: �ĳ�ʹ���ڲ�flash
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "FlashBak.h"
	#include "flash_if.h"
#if USE_FLASH_BAK
//һ�������С
#define FLASHBAK_SECTOR_SIZE      0x20000 //128k
//������ַ��ע����뱻4����
#define FLASHBAK_BAK1_ADDR        ADDR_FLASH_SECTOR_10
#define FLASHBAK_BAK2_ADDR        (FLASHBAK_BAK1_ADDR + FLASHBAK_SECTOR_SIZE)

//���������,������0��0xFFFFFFFF
#define MIN_FLAG         0x00000010
#define MAX_FLAG         0x10000000
#define IS_VALID_FLAG(flag)    ((flag >= MIN_FLAG)&&(flag <= MAX_FLAG))

CDV_INT08S the_newest_bak = -1;

/**
  * @brief  
  * @param  
  * @retval 0 success ; 1 failure
  */
u8 FlashBak_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
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
  * @note WriteAddr NumByteToWrite��Ϊ4��������
  */
u8 FlashBak_Read(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
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
//	
//#define BAK_READ (pval, addr, bytes) SPI_Flash_Read (pval, addr, bytes)
//#define BAK_WRITE(pval, addr, bytes) SPI_Flash_Write(pval, addr, bytes)
#define BAK_READ(pval, addr, bytes)  FlashBak_Read(pval, addr, bytes)
#define BAK_WRITE(pval, addr, bytes) FlashBak_Write(pval, addr, bytes)

//n��0��ʼ
CDV_INT32U GetFlashBakAddr(CDV_INT32U n) {
	return (FLASHBAK_BAK1_ADDR + n*FLASHBAK_SECTOR_SIZE);
}

u32 FlashBak_Unlock(void) {
	u32 err;
	
	if (FLASH_If_GetWriteProtectionStatus() == 0)   
	{
		err = FLASH_If_DisableWriteProtection();
		if (2 == err)
			WhileError();
	}
	else
	{
		err = 0;
	}
	FLASH_If_Init();						//���� 
	FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
	return err;
}
	
void FlashBak_Lock(void) {
	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
}

void FlashBak_Erase(CDV_INT32U n)   
{
	u32 err;
	err = FlashBak_Unlock();
	err = FLASH_If_Erase(GetFlashBakAddr(n));
	FlashBak_Lock();
	
}

//����CDVParamInit
void FlashBak_Restore(void) {
	CDV_INT32S flag[2] = {0};
	CDV_INT08S next_bak;
	
	BAK_READ((CDV_INT08U *)&flag[1], GetFlashBakAddr(1), 4);
	BAK_READ((CDV_INT08U *)&flag[0], GetFlashBakAddr(0), 4);
	
	if(IS_VALID_FLAG(flag[0]) && IS_VALID_FLAG(flag[1])) {
		if(abs(flag[0] - flag[1]) == MAX_FLAG - MIN_FLAG) { // �ж��Ƿ��������߽�ֵ
			if(flag[0] > flag[1]) {
				the_newest_bak = 1;
			} else {
				the_newest_bak = 0;
			}
		}
		else {if(flag[0] > flag[1]) {
				the_newest_bak = 0;
			} else {
				the_newest_bak = 1;
			}
		}
	} else if (IS_VALID_FLAG(flag[0])) {
		the_newest_bak = 0;
	} else if (IS_VALID_FLAG(flag[1])) {
		the_newest_bak = 1;
	} else {
		FlashBak_Erase(0);
		FlashBak_Erase(1);
		return;
	}
	//����Ӧ��
	BAK_READ((CDV_INT08U *)&g_modbusReg, GetFlashBakAddr(the_newest_bak)+4, sizeof(g_modbusReg));
	
	next_bak  = the_newest_bak == 0 ? 1 : 0;
	FlashBak_Erase(next_bak);
}

// ��Ҫ��Ͻ���������
CDV_INT32S FlashBak_BackUp(void) {
	CDV_INT32S flag;
	CDV_INT08S next_bak = the_newest_bak == 0 ? 1 : 0;
	//ԭʼ
	
	BAK_READ((CDV_INT08U *)&flag, GetFlashBakAddr(the_newest_bak), 4);
	flag+=1;
	if(!IS_VALID_FLAG(flag))
		flag = MIN_FLAG;
	
	FlashBak_Unlock();
	
#ifdef  DEBUG_TIME
	time_log(-1);
#endif
	if(0 == BAK_WRITE((CDV_INT08U *)&g_modbusReg, GetFlashBakAddr(next_bak)+4, sizeof(g_modbusReg)))
	  BAK_WRITE((CDV_INT08U *)&flag, GetFlashBakAddr(next_bak), 4);
	
	FlashBak_Lock();
	
	return 0;
}

#endif
