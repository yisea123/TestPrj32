#ifndef  _LED_
#define  _LED_
#include "cdv_include.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED端口定义
#define LED1 PCout(3)		// 调试指示灯;
#define LED2 PCout(2)	  // 运行指示灯;	
#define LED3 PCout(0)	  // 故障指示灯;	

#define	IN_RCK_ENABLE      		    GPIO_SetBits(GPIOC,GPIO_Pin_13)     //IN_RCK
#define IN_RCK_DISABLE	    			GPIO_ResetBits(GPIOC,GPIO_Pin_13) 

#define	IN_SCK_ENABLE       			GPIO_SetBits(GPIOC,GPIO_Pin_14) 		//IN_SCK
#define	IN_SCK_DISABLE      			GPIO_ResetBits(GPIOC,GPIO_Pin_14)

#define	IN_SDA_ENABLE    					GPIO_SetBits(GPIOC,GPIO_Pin_15) 		//IN_SDA
#define	IN_SDA_DISABLE    				GPIO_ResetBits(GPIOC,GPIO_Pin_15)

#define	OUT_RCK_ENABLE     				GPIO_SetBits(GPIOA,GPIO_Pin_6)     //OUT_RCK
#define OUT_RCK_DISABLE	    			GPIO_ResetBits(GPIOA,GPIO_Pin_6) 

#define	OUT_SCK_ENABLE      			GPIO_SetBits(GPIOA,GPIO_Pin_5) 		//OUT_SCK
#define	OUT_SCK_DISABLE     			GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define	OUT_SDA_ENABLE    				GPIO_SetBits(GPIOA,GPIO_Pin_4) 		//OUT_SDA
#define	OUT_SDA_DISABLE    				GPIO_ResetBits(GPIOA,GPIO_Pin_4)

void LED_Init(void); 	 //初始化
void Led_Test(void);
void Hc595SendByte(u32 data);
void IN_Hc595SendByte(u32 data);
void OUT_Hc595SendByte(u32 data);
void DisPlay(u32 data);
void IN_DisPlay(u32 data);
void OUT_DisPlay(u32 data);

#endif
