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
#define LED1 PBout(7)		// 调试指示灯;
#define LED2 PBout(1)	  // 运行指示灯;	
#define LED3 PBout(0)	// 故障指示灯;	

#define	IN_RCK_ENABLE      		    GPIO_SetBits(GPIOA,GPIO_Pin_6)     //IN_RCK  PA6
#define IN_RCK_DISABLE	    			GPIO_ResetBits(GPIOA,GPIO_Pin_6) 

#define	IN_SCK_ENABLE       			GPIO_SetBits(GPIOA,GPIO_Pin_7) 		//IN_SCK   PA7
#define	IN_SCK_DISABLE      			GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define	IN_SDA_ENABLE    					GPIO_SetBits(GPIOC,GPIO_Pin_4) 		//IN_SDA   PC4
#define	IN_SDA_DISABLE    				GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define	OUT_RCK_ENABLE     				GPIO_SetBits(GPIOC,GPIO_Pin_9)     //OUT_RCK  PC9
#define OUT_RCK_DISABLE	    			GPIO_ResetBits(GPIOC,GPIO_Pin_9) 

#define	OUT_SCK_ENABLE      			GPIO_SetBits(GPIOC,GPIO_Pin_8) 		//OUT_SCK  PC8
#define	OUT_SCK_DISABLE     			GPIO_ResetBits(GPIOC,GPIO_Pin_8)

#define	OUT_SDA_ENABLE    				GPIO_SetBits(GPIOC,GPIO_Pin_6) 		//OUT_SDA  PC6
#define	OUT_SDA_DISABLE    				GPIO_ResetBits(GPIOC,GPIO_Pin_6)

#define SW1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define SW2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)
#define SW3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define READ_ID   ((SW1<<2)|(SW2<<1)|SW3 )

void LED_Init(void); 	 //初始化
void Led_Test(void);
void Hc595SendByte(u32 data);
void IN_Hc595SendByte(u32 data);
void OUT_Hc595SendByte(u32 data);
void DisPlay(u32 data);
void IN_DisPlay(u32 data);
void OUT_DisPlay(u32 data);
u16 Read_Input_All(void);
u16 Read_Output_ALL(void);
#endif
