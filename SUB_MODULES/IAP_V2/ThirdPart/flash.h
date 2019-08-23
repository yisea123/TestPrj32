
#ifndef _FLASH_H
#define _FLASH_H
#include <cdv_include.h>
//ѡ��FLASH
#define	FLASH_EN 			CASCADE_CS_DS;FPGA1_CS_DS;FPGA2_CS_DS;FLASH_CS_EN;
//�ر�FLASH
#define	FLASH_DS 			FLASH_CS_DS;
#define FLASH_ReadWriteByte(a)				SPIx_ReadWriteByte(SPI2,(a))
//#define FPGAFLASH_ReadWriteByte(a)				FPGAoldx_ReadWriteByte(SPI2,(a))
////////////////////////////////////////////////////////////////////////////
//W25Q64��д
#define W25Q64 	0XEF16
//ָ���
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg	0x05 

#define W25X_WriteStatusReg	0x01 
#define W25X_ReadData			  0x03 
#define W25X_FastReadData		0x0B 

#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			    0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


//u16  SPI_Flash_ReadID(void);  	    //��ȡFLASH ID
//u8	 SPI_Flash_ReadSR(void);        //��ȡ״̬�Ĵ��� 
//void SPI_FLASH_Write_SR(u8 sr);  	//д״̬�Ĵ���
//void SPI_FLASH_Write_Enable(void);  //дʹ�� 
//void SPI_FLASH_Write_Disable(void);	//д����
//void FPGA_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead) ;
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void Org_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   ;
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д��flash
void Org_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
//void SPI_Flash_Erase_Chip(void);    	  //��Ƭ����
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//��������
void SPI_Flash_Wait_Busy(void);           //�ȴ�����
//void SPI_Flash_PowerDown(void);           //�������ģʽ
//void SPI_Flash_WAKEUP(void);			  //����
#endif
