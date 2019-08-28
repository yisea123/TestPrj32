
#ifndef  _FPGA_SPI_H
#define  _FPGA_SPI_H

#include "cdv_include.h"   

/*��չ��Դ*/
//#define CDV_EXI_NUM 34
//#define CDV_EXO_NUM 30
#define FPGA_NONE 2
#define FPGA_MOTOR_CODE 1
#define FPGA_IO_CODE 0
#define	SF0_CS1_ENABLE 			  CASCADE_CS_DS;FLASH_CS_DS;FPGA1_CS_EN;  //ѡ��FPGA1����
#define	SF0_CS1_DISENABLE 		FPGA1_CS_DS    //�ر�FPGA1����

#define	SF0_CS2_ENABLE  			CASCADE_CS_DS;FLASH_CS_DS;FPGA2_CS_EN;  //ѡ��FPGA2����
#define	SF0_CS2_DISENABLE			FPGA2_CS_DS    //�ر�FPGA2����

#define	SF0_CS3_ENABLE  			//GPIO_ResetBits(GPIOB,GPIO_Pin_15)  //ѡ��FPGA3����
#define	SF0_CS3_DISENABLE			//GPIO_SetBits(GPIOB,GPIO_Pin_15)    //�ر�FPGA3����	

#define	SF0_CS4_ENABLE  			//GPIO_ResetBits(GPIOE,GPIO_Pin_9)  //ѡ��FPGA4����
#define	SF0_CS4_DISENABLE 		//GPIO_SetBits(GPIOE,GPIO_Pin_9)    //�ر�FPGA4����

#define FPGA_EN(n)            CASCADE_CS_DS;FLASH_CS_DS;FPGA##n##_CS_EN;  //ѡ��FPGA1����

#define FPGA_DS               FPGA1_CS_DS;FPGA2_CS_DS;


extern u8 err_times1;
extern u8 CDV_EXI_NUM;
extern u8 CDV_EXO_NUM;
extern u8 FPGA_Mode;
//u8 FPGAx_ReadWriteByte(SPI_TypeDef* SPIx, u8 TxData);
void Get_paramate(void);
void SPI1_Init(void);
u8 FpgaMotoWait(u8 no);
u8 FpgaIRead( u8 no );
u8 FpgaORead( u8 no );
void FpgaOWrite( u8 no , u8 val);
//u16 Read_Fpga_Regist(u8 add,u8 FPGA_NUM);
u32 Read_Fpga_Regist(u16 add,u8 FPGA_NUM);

void Send_parameter(u8 no, u8 *buf, u8 len);
void FpgaOReadAll(CDV_INT08U* pRxBuf);
void FpgaIReadAll(CDV_INT08U* pRxBuf);
void FpgaOWriteAll(CDV_INT08U* pRxBuf);
#endif
