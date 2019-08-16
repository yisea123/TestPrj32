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
//#define LED0 PFout(9)		// DS0
//#define LED1 PFout(10)	// DS1	
#if _NPC_VERSION_ == 1u
	#define	IN_RCK_ENABLE      		    
	#define IN_RCK_DISABLE	    			

	#define	IN_SCK_ENABLE       			
	#define	IN_SCK_DISABLE      			

	#define	IN_SDA_ENABLE    					
	#define	IN_SDA_DISABLE    				

	#define	OUT_RCK_ENABLE     				
	#define OUT_RCK_DISABLE	    			

	#define	OUT_SCK_ENABLE      			
	#define	OUT_SCK_DISABLE     			

	#define	OUT_SDA_ENABLE    				
	#define	OUT_SDA_DISABLE    				

#elif _NPC_VERSION_ == 2u
	#define	IN_RCK_ENABLE      		    GPIO_SetBits(GPIOD,GPIO_Pin_10)     //IN_RCK
	#define IN_RCK_DISABLE	    			GPIO_ResetBits(GPIOD,GPIO_Pin_10) 

	#define	IN_SCK_ENABLE       			GPIO_SetBits(GPIOD,GPIO_Pin_11) 		//IN_SCK
	#define	IN_SCK_DISABLE      			GPIO_ResetBits(GPIOD,GPIO_Pin_11)

	#define	IN_SDA_ENABLE    					GPIO_SetBits(GPIOD,GPIO_Pin_12) 		//IN_SDA
	#define	IN_SDA_DISABLE    				GPIO_ResetBits(GPIOD,GPIO_Pin_12)

	#define	OUT_RCK_ENABLE     				GPIO_SetBits(GPIOG,GPIO_Pin_11)     //OUT_RCK
	#define OUT_RCK_DISABLE	    			GPIO_ResetBits(GPIOG,GPIO_Pin_11) 

	#define	OUT_SCK_ENABLE      			GPIO_SetBits(GPIOG,GPIO_Pin_12) 		//OUT_SCK
	#define	OUT_SCK_DISABLE     			GPIO_ResetBits(GPIOG,GPIO_Pin_12)

	#define	OUT_SDA_ENABLE    				GPIO_SetBits(GPIOD,GPIO_Pin_13) 		//OUT_SDA
	#define	OUT_SDA_DISABLE    				GPIO_ResetBits(GPIOD,GPIO_Pin_13)

#elif _NPC_VERSION_ == 3u
	#define	IN_RCK_ENABLE      		    GPIO_SetBits(GPIOH,GPIO_Pin_10)     //IN_RCK
	#define IN_RCK_DISABLE	    			GPIO_ResetBits(GPIOH,GPIO_Pin_10) 

	#define	IN_SCK_ENABLE       			GPIO_SetBits(GPIOH,GPIO_Pin_11) 		//IN_SCK
	#define	IN_SCK_DISABLE      			GPIO_ResetBits(GPIOH,GPIO_Pin_11)

	#define	IN_SDA_ENABLE    					GPIO_SetBits(GPIOH,GPIO_Pin_9) 		//IN_SDA
	#define	IN_SDA_DISABLE    				GPIO_ResetBits(GPIOH,GPIO_Pin_9)

	#define	OUT_RCK_ENABLE     				GPIO_SetBits(GPIOI,GPIO_Pin_4)     //OUT_RCK
	#define OUT_RCK_DISABLE	    			GPIO_ResetBits(GPIOI,GPIO_Pin_4) 

	#define	OUT_SCK_ENABLE      			GPIO_SetBits(GPIOI,GPIO_Pin_5) 		//OUT_SCK
	#define	OUT_SCK_DISABLE     			GPIO_ResetBits(GPIOI,GPIO_Pin_5)

	#define	OUT_SDA_ENABLE    				GPIO_SetBits(GPIOI,GPIO_Pin_6) 		//OUT_SDA
	#define	OUT_SDA_DISABLE    				GPIO_ResetBits(GPIOI,GPIO_Pin_6)
#endif

void LED_Init(void); 	 //��ʼ��
void Led_Test(void);
void Hc595SendByte(u32 data);
void IN_Hc595SendByte(u32 data);
void OUT_Hc595SendByte(u32 data);
void DisPlay(u32 data);
void IN_DisPlay(u32 data);
void OUT_DisPlay(u32 data);

#endif
