/**
  ******************************************************************************
  * @file    /FlashBak.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2018-4-10
  * @brief   a package to solve the problem of lost data in flash module
  * 
@verbatim  
20180410：增加变量的flash备份管理
20180605: 改成使用内部flash
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
//一块区域大小
#define FLASHBAK_SECTOR_SIZE      0x20000 //128k
//变量地址，注意必须被4除尽
#define FLASHBAK_BAK1_ADDR        ADDR_FLASH_SECTOR_10
#define FLASHBAK_BAK2_ADDR        (FLASHBAK_BAK1_ADDR + FLASHBAK_SECTOR_SIZE)

//变量区标记,不能是0或0xFFFFFFFF
#define MIN_FLAG         0x00000010
#define MAX_FLAG         0x10000000
#define IS_VALID_FLAG(flag)    (flag >= MIN_FLAG)&&(flag <= MAX_FLAG)

CDV_INT08S the_newest_bak = -1;
//	
//#define BAK_READ (pval, addr, bytes) SPI_Flash_Read (pval, addr, bytes)
//#define BAK_WRITE(pval, addr, bytes) SPI_Flash_Write(pval, addr, bytes)
#define BAK_READ(pval, addr, bytes)  PVD_Flash_Read(pval, addr, bytes)
#define BAK_WRITE(pval, addr, bytes) PVD_Flash_Write(pval, addr, bytes)

//n从0开始
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
	FLASH_If_Init();						//解锁 
	FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
	return err;
}
	
void FlashBak_Lock(void) {
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁
}

void FlashBak_Erase(CDV_INT32U n)   
{
	u32 err;
	err = FlashBak_Unlock();
	err = FLASH_If_Erase(GetFlashBakAddr(n));
	FlashBak_Lock();
	
}

//用在CDVParamInit
void FlashBak_Restore(void) {
	CDV_INT32S flag[2] = {0};
	CDV_INT08S next_bak;
	
	BAK_READ((CDV_INT08U *)&flag[0], FLASHBAK_BAK1_ADDR + FLASHBAK_SECTOR_SIZE - 4, 4);
	BAK_READ((CDV_INT08U *)&flag[1], FLASHBAK_BAK2_ADDR + FLASHBAK_SECTOR_SIZE - 4, 4);
	
	if(IS_VALID_FLAG(flag[0]) && IS_VALID_FLAG(flag[1])) {
		if(abs(flag[0] - flag[1]) == MAX_FLAG - MIN_FLAG) { // 判断是否是两个边界值
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
		return;
	}
	//具体应用
	BAK_READ((CDV_INT08U *)&g_modbusReg, GetFlashBakAddr(the_newest_bak), sizeof(g_modbusReg));
	
	next_bak  = the_newest_bak == 0 ? 1 : 0;
	FlashBak_Erase(next_bak);
}

// 需要配合解锁，擦除
CDV_INT32S FlashBak_BackUp(void) {
	CDV_INT32S flag;
	CDV_INT08S next_bak = the_newest_bak == 0 ? 1 : 0;
	//原始
	
	BAK_READ((CDV_INT08U *)&flag, GetFlashBakAddr(the_newest_bak) + FLASHBAK_SECTOR_SIZE - 4, 4);
	flag+=1;
	if(IS_VALID_FLAG(flag))
		flag = MIN_FLAG;
	
	FlashBak_Unlock();
	BAK_WRITE((CDV_INT08U *)&g_modbusReg, GetFlashBakAddr(next_bak), sizeof(g_modbusReg));
	BAK_WRITE((CDV_INT08U *)&flag, GetFlashBakAddr(next_bak) + FLASHBAK_SECTOR_SIZE - 4, 4);
	FlashBak_Lock();
}

#endif
