#ifndef  _DAC7512_H
#define  _DAC7512_H

#include "cdv_include.h"   

#if _NPC_VERSION_ == 2u
#define	DAC7512_DIN_ENABLE     GPIO_SetBits(GPIOE,GPIO_Pin_11)     //SPI1_MOSI
#define DAC7512_DIN_DISABLE	    GPIO_ResetBits(GPIOE,GPIO_Pin_11) 

#define	DAC7512_CCLK_ENABLE       GPIO_SetBits(GPIOE,GPIO_Pin_12) //SPI0_SLCK
#define	DAC7512_CCLK_DISABLE        GPIO_ResetBits(GPIOE,GPIO_Pin_12)

#define	DAC7512_SYNC_ENABLE    GPIO_SetBits(GPIOE,GPIO_Pin_13) //FPGA_PR_B
#define	DAC7512_SYNC_DISABLE    GPIO_ResetBits(GPIOE,GPIO_Pin_13)

#define	DAC7512_DIN_ENABLE1     	GPIO_SetBits(GPIOE,GPIO_Pin_14)     //DAC7512_DIN2
#define DAC7512_DIN_DISABLE1	    GPIO_ResetBits(GPIOE,GPIO_Pin_14) 

#define	DAC7512_CCLK_ENABLE1      GPIO_SetBits(GPIOE,GPIO_Pin_15) 		//DAC7512_CCLK2
#define	DAC7512_CCLK_DISABLE1     GPIO_ResetBits(GPIOE,GPIO_Pin_15)

#define	DAC7512_SYNC_ENABLE1    	GPIO_SetBits(GPIOB,GPIO_Pin_10) 		//DAC_SYNC2
#define	DAC7512_SYNC_DISABLE1    	GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#elif _NPC_VERSION_ == 3u
#define	DAC7512_DIN_ENABLE     GPIO_SetBits(GPIOH,GPIO_Pin_8)     //SPI1_MOSI
#define DAC7512_DIN_DISABLE	    GPIO_ResetBits(GPIOH,GPIO_Pin_8) 

#define	DAC7512_CCLK_ENABLE       GPIO_SetBits(GPIOB,GPIO_Pin_14) //SPI0_SLCK
#define	DAC7512_CCLK_DISABLE        GPIO_ResetBits(GPIOB,GPIO_Pin_14)

#define	DAC7512_SYNC_ENABLE    GPIO_SetBits(GPIOB,GPIO_Pin_15) //FPGA_PR_B
#define	DAC7512_SYNC_DISABLE    GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define	DAC7512_DIN_ENABLE1     	GPIO_SetBits(GPIOB,GPIO_Pin_10)     //DAC7512_DIN2
#define DAC7512_DIN_DISABLE1	    GPIO_ResetBits(GPIOB,GPIO_Pin_10) 

#define	DAC7512_CCLK_ENABLE1      GPIO_SetBits(GPIOH,GPIO_Pin_6) 		//DAC7512_CCLK2
#define	DAC7512_CCLK_DISABLE1     GPIO_ResetBits(GPIOH,GPIO_Pin_6)

#define	DAC7512_SYNC_ENABLE1    	GPIO_SetBits(GPIOH,GPIO_Pin_7) 		//DAC_SYNC2
#define	DAC7512_SYNC_DISABLE1    	GPIO_ResetBits(GPIOH,GPIO_Pin_7)

#endif
void DAC7512_Pin_Config(void);
void WriteToDAC7512(u16 data);
void WriteToDA2C_7512(u16 data);
void delay_ns(u32 cnt);

#endif
