#ifndef  _DAC7512_H
#define  _DAC7512_H

#include "cdv_include.h"   


//#define	DAC7512_DIN_ENABLE     GPIO_SetBits(GPIOA,GPIO_Pin_12)      //DAC7512_DIN1
//#define DAC7512_DIN_DISABLE	   GPIO_ResetBits(GPIOA,GPIO_Pin_12) 

//#define	DAC7512_CCLK_ENABLE       GPIO_SetBits(GPIOA,GPIO_Pin_11)   //DAC7512_CCLK1
//#define	DAC7512_CCLK_DISABLE      GPIO_ResetBits(GPIOA,GPIO_Pin_11)

//#define	DAC7512_SYNC_ENABLE    GPIO_SetBits(GPIOA,GPIO_Pin_8)    //DAC_SYNC1
//#define	DAC7512_SYNC_DISABLE   GPIO_ResetBits(GPIOA,GPIO_Pin_8)

//#define	DAC7512_DIN_ENABLE1     	GPIO_SetBits(GPIOC,GPIO_Pin_11)     //DAC7512_DIN2
//#define DAC7512_DIN_DISABLE1	    GPIO_ResetBits(GPIOC,GPIO_Pin_11) 

//#define	DAC7512_CCLK_ENABLE1      GPIO_SetBits(GPIOC,GPIO_Pin_10) 		//DAC7512_CCLK2
//#define	DAC7512_CCLK_DISABLE1     GPIO_ResetBits(GPIOC,GPIO_Pin_10)

//#define	DAC7512_SYNC_ENABLE1    	GPIO_SetBits(GPIOA,GPIO_Pin_15) 		//DAC_SYNC2
//#define	DAC7512_SYNC_DISABLE1    	GPIO_ResetBits(GPIOA,GPIO_Pin_15)

//#define	DAC7512_DIN_ENABLE2     	GPIO_SetBits(GPIOB,GPIO_Pin_3)      //DAC7512_DIN3
//#define DAC7512_DIN_DISABLE2	    GPIO_ResetBits(GPIOB,GPIO_Pin_3) 

//#define	DAC7512_CCLK_ENABLE2      GPIO_SetBits(GPIOD,GPIO_Pin_2) 		 //DAC7512_CCLK3
//#define	DAC7512_CCLK_DISABLE2     GPIO_ResetBits(GPIOD,GPIO_Pin_2)

//#define	DAC7512_SYNC_ENABLE2    	GPIO_SetBits(GPIOC,GPIO_Pin_12) 		//DAC_SYNC3
//#define	DAC7512_SYNC_DISABLE2    	GPIO_ResetBits(GPIOC,GPIO_Pin_12)

//#define	DAC7512_DIN_ENABLE3     	GPIO_SetBits(GPIOB,GPIO_Pin_6)     //DAC7512_DIN4
//#define DAC7512_DIN_DISABLE3	    GPIO_ResetBits(GPIOB,GPIO_Pin_6) 

//#define	DAC7512_CCLK_ENABLE3      GPIO_SetBits(GPIOB,GPIO_Pin_5) 		//DAC7512_CCLK4
//#define	DAC7512_CCLK_DISABLE3     GPIO_ResetBits(GPIOB,GPIO_Pin_5)

//#define	DAC7512_SYNC_ENABLE3   		GPIO_SetBits(GPIOB,GPIO_Pin_4) 		//DAC_SYNC4
//#define	DAC7512_SYNC_DISABLE3    	GPIO_ResetBits(GPIOB,GPIO_Pin_4)


void DAC7512_Pin_Config(void);
void WriteToDAC1_7512(u16 data);
void WriteToDAC2_7512(u16 data);
void WriteToDAC3_7512(u16 data);
void WriteToDAC4_7512(u16 data);
void delay_ns(u32 cnt);

#endif
