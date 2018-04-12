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
//һ�������С
#define FLASHBAK_SECTOR_SIZE      4096
//������ַ��ע����뱻4096����
#define FLASHBAK_VAR_ADDR         VAL_STADDR
#define FLASHBAK_VAR_BAK_ADDR     FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE
//��������
#define FLASHBAK_VAR_NUM          CDV_VAR_NUM
//������ռ�ֽ���
#define FLASHBAK_VAR_SIZE         4
//���������,������0��0xFFFFFFFF
#define FLASHBAK_VAR_FLAG         0x00001000

CDV_INT32S* flash_var = (CDV_INT32S*)g_modbusReg.reg;
	
//����CDVParamInit
void FlashBak_VarRestore(void) {
	CDV_INT32S val;
	CDV_INT32U i;
	
	
	SPI_Flash_Read((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	if(FLASHBAK_VAR_FLAG == val){ //������� 
		SPI_Flash_Read((CDV_INT08U *)flash_var, FLASHBAK_VAR_ADDR, FLASHBAK_VAR_SIZE * FLASHBAK_VAR_NUM);
		
		return;
}
	
	SPI_Flash_Read((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	if(FLASHBAK_VAR_FLAG != val)//��ǲ�����
		return;
	
	SPI_Flash_Read((CDV_INT08U *)flash_var, FLASHBAK_VAR_BAK_ADDR, FLASHBAK_VAR_SIZE * FLASHBAK_VAR_NUM);
	SPI_Flash_Write((CDV_INT08U *)flash_var, FLASHBAK_VAR_ADDR, FLASHBAK_VAR_SIZE * FLASHBAK_VAR_NUM);
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
}

//����ValFlashSet��ValToFlash��
CDV_INT32S FlashBak_VarBackUp(CDV_INT32U no, CDV_INT32S num) {
	CDV_INT32S val;
	CDV_INT32U i;
	//ԭʼ
	val = 0;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	SPI_Flash_Write((CDV_INT08U *)&flash_var[no], FLASHBAK_VAR_ADDR + FLASHBAK_VAR_SIZE * no, FLASHBAK_VAR_SIZE * num);
	
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	//����
	val = 0;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	SPI_Flash_Write((CDV_INT08U *)&flash_var[no], FLASHBAK_VAR_BAK_ADDR + FLASHBAK_VAR_SIZE * no, FLASHBAK_VAR_SIZE * num);
	
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
}

//����modbus��
CDV_INT32S FlashBak_VarBackUpEx(CDV_INT08U* buf, CDV_INT32U offset, CDV_INT32S num) {
	CDV_INT32S val;
	CDV_INT32U i;
	//ԭʼ
	val = 0;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	SPI_Flash_Write(buf, FLASHBAK_VAR_ADDR + offset, num);
	
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	//����
	val = 0;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
	SPI_Flash_Write(buf, FLASHBAK_VAR_BAK_ADDR + offset, num);
	
	val = FLASHBAK_VAR_FLAG;
	SPI_Flash_Write((CDV_INT08U *)&val, FLASHBAK_VAR_BAK_ADDR + FLASHBAK_SECTOR_SIZE - FLASHBAK_VAR_SIZE, FLASHBAK_VAR_SIZE);
	
}

#endif
