
#ifndef _FLASH_H
#define _FLASH_H
#include <cdv_include.h>
//选中FLASH
#define	FLASH_EN 			CASCADE_CS_DS;FPGA1_CS_DS;FPGA2_CS_DS;FLASH_CS_EN;
//关闭FLASH
#define	FLASH_DS 			FLASH_CS_DS;
#define FLASH_ReadWriteByte(a)				SPIx_ReadWriteByte(SPI2,(a))
//#define FPGAFLASH_ReadWriteByte(a)				FPGAoldx_ReadWriteByte(SPI2,(a))
////////////////////////////////////////////////////////////////////////////
//W25Q64读写
#define W25Q64 	0XEF16
//指令表
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


//u16  SPI_Flash_ReadID(void);  	    //读取FLASH ID
//u8	 SPI_Flash_ReadSR(void);        //读取状态寄存器 
//void SPI_FLASH_Write_SR(u8 sr);  	//写状态寄存器
//void SPI_FLASH_Write_Enable(void);  //写使能 
//void SPI_FLASH_Write_Disable(void);	//写保护
//void FPGA_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead) ;
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void Org_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   ;
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void Org_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
//void SPI_Flash_Erase_Chip(void);    	  //整片擦除
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
//void SPI_Flash_PowerDown(void);           //进入掉电模式
//void SPI_Flash_WAKEUP(void);			  //唤醒
extern  CDV_INT08U CHECK_FLASH[5];
int Flash_Check(void);
#endif
