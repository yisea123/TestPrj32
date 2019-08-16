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
#define LED1 PCout(3)		// ����ָʾ��;
#define LED2 PCout(2)	  // ����ָʾ��;	
#define LED3 PCout(0)	  // ����ָʾ��;	

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

void LED_Init(void); 	 //��ʼ��
void Led_Test(void);
void Hc595SendByte(u32 data);
void IN_Hc595SendByte(u32 data);
void OUT_Hc595SendByte(u32 data);
void DisPlay(u32 data);
void IN_DisPlay(u32 data);
void OUT_DisPlay(u32 data);

#endif
