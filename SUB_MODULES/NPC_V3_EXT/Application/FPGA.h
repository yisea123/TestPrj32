#ifndef  _FPGA_H
#define  _FPGA_H

#include "cdv_include.h"   
#define CONFIG_FPGA 0    //不从片上FLASH加载FPGA配置文件，从w25q64加载Flash文件

#if ENABLE_FPGA_DOWN
#define CDV_FPGA_NUM 2
#else
#define CDV_FPGA_NUM 0
#endif

//#define CDV_FPGA_NUM2 4//硬件上FPGA块数
extern u32 fpga_con_data_byte; //FPGA配置文件字节数
//****************************************
//CCLK，DIN四块FPGA共用
//FPGA_CCLK=PB3输出
//FPGA_DIN=PB5输出
//FPGA2_PROG_B=PH9输出
//****************************************
#define	FPGA_DIN_ENABLE      GPIO_SetBits(GPIOG,GPIO_Pin_8)//SPI1_MOSI/FPGA_DIN
#define	FPGA_DIN_DISABLE     GPIO_ResetBits(GPIOG,GPIO_Pin_8)

#define	FPGA_CCLK_ENABLE     GPIO_SetBits(GPIOG,GPIO_Pin_6)
#define	FPGA_CCLK_DISABLE    GPIO_ResetBits(GPIOG,GPIO_Pin_6)

#define	FPGA1_PROG_B_ENABLE   GPIO_SetBits(GPIOH,GPIO_Pin_8)
#define	FPGA1_PROG_B_DISABLE  GPIO_ResetBits(GPIOH,GPIO_Pin_8)

#define	FPGA2_PROG_B_ENABLE   GPIO_SetBits(GPIOH,GPIO_Pin_6)
#define	FPGA2_PROG_B_DISABLE  GPIO_ResetBits(GPIOH,GPIO_Pin_6)

#define	FPGA3_PROG_B_ENABLE   //GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define	FPGA3_PROG_B_DISABLE  //GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define	FPGA4_PROG_B_ENABLE   //GPIO_SetBits(GPIOH,GPIO_Pin_7)
#define	FPGA4_PROG_B_DISABLE  //GPIO_ResetBits(GPIOH,GPIO_Pin_7)
//*****************************************
//FPGA复位引脚
#define	FPGA_RESET_ENABLE     GPIO_SetBits(GPIOC,GPIO_Pin_9)//SPI1_MISO
#define	FPGA_RESET_DISABLE    GPIO_ResetBits(GPIOC,GPIO_Pin_9)



void FpgaInit(u8 cfg);
//void Fpga1_Config_Init(void);
//void Fpga2_Config_Init(void);
//void Fpga3_Config_Init(void);
//void Fpga4_Config_Init(void);

void Fpga_Config_Init(void);
	
u8 Fpga1_Read_Done(void);
u8 Fpga1_Read_INT(void);
u8 Fpga2_Read_Done(void);
u8 Fpga2_Read_INT(void);
u8 Fpga3_Read_Done(void);
u8 Fpga3_Read_INT(void);
u8 Fpga4_Read_Done(void);
u8 Fpga4_Read_INT(void);
u8 Fpga1_Config(u8 *Buf,u32 Code_Size);
u8 Fpga_Config(u8 FPGA_Num,u32 FPGA_CODE_ADD);
u8 Fpga_Down(u8 FPGA_Num,u32 FPGA_CODE_ADD);
void delay_ns(u32 cnt);
#endif


