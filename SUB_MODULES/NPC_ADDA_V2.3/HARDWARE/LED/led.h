#ifndef  _LED_
#define  _LED_
#include "cdv_include.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
#define LED1 PBout(7)		// ����ָʾ��;
#define LED2 PBout(1)	  // ����ָʾ��;	
#define LED3 PBout(0)	// ����ָʾ��;	

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

void LED_Init(void); 	 //��ʼ��
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
