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
#if USE_FLASH_BAK
//一块区域大小
#define FLASHBAK_SECTOR_SIZE      4096
//变量地址，注意必须被4096除尽
#define FLASHBAK_VAR_ADDR         VAL_STADDR
#define FLASHBAK_VAR_BAK_ADDR     FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE
//变量数量
#define FLASHBAK_VAR_NUM          CDV_VAR_NUM
//变量所占字节数
#define FLASHBAK_VAR_SIZE         4
//变量区标记,不能是0或0xFFFFFFFF
#define FLASHBAK_VAR_FLAG         0x00001000

CDV_INT32S* flash_var = (CDV_INT32S*)g_modbusReg.reg;
	
//用在CDVParamInit
void FlashBak_VarRestore(void) {
	CDV_INT32S val;
	CDV_INT32U i;
	
	
	SPI_Flash_Read((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	if(FLASHBAK_VAR_FLAG == val){ //标记正常 
		SPI_Flash_Read((CDV_INT08U *)flash_var, FLASHBAK_VAR_ADDR, FLASHBAK_VAR_SIZE * FLASHBAK_VAR_NUM);
		
		return;
}
	
	SPI_Flash_Read((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	if(FLASHBAK_VAR_FLAG != val)//标记不正常
		return;
	
	SPI_Flash_Read((CDV_INT08U *)flash_var, FLASHBAK_VAR_BAK_ADDR, FLASHBAK_VAR_SIZE * FLASHBAK_VAR_NUM);
	SPI_Flash_Write((CDV_INT08U *)flash_var, FLASHBAK_VAR_ADDR, FLASHBAK_VAR_SIZE * FLASHBAK_VAR_NUM);
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
}

//用在ValFlashSet、ValToFlash中
CDV_INT32S FlashBak_VarBackUp(CDV_INT32U no, CDV_INT32S num) {
	CDV_INT32S val;
	CDV_INT32U i;
	//原始
	val = 0;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	SPI_Flash_Write((CDV_INT08U *)&flash_var[no], FLASHBAK_VAR_ADDR + FLASHBAK_VAR_SIZE * no, FLASHBAK_VAR_SIZE * num);
	
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	//备份
	val = 0;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	SPI_Flash_Write((CDV_INT08U *)&flash_var[no], FLASHBAK_VAR_BAK_ADDR + FLASHBAK_VAR_SIZE * no, FLASHBAK_VAR_SIZE * num);
	
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
}

//用在modbus中
CDV_INT32S FlashBak_VarBackUpEx(CDV_INT08U* buf, CDV_INT32U offset, CDV_INT32S num) {
	CDV_INT32S val;
	CDV_INT32U i;
	//原始
	val = 0;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	SPI_Flash_Write(buf, FLASHBAK_VAR_ADDR + offset, num);
	
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	//备份
	val = 0;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	SPI_Flash_Write(buf, FLASHBAK_VAR_BAK_ADDR + offset, num);
	
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
}

#endif
