#include "74HC595.h"
#define	IN_SI_H     GPIO_SetBits(GPIOD,GPIO_Pin_12)     //数据线
#define IN_SI_L	    GPIO_ResetBits(GPIOD,GPIO_Pin_12) 

#define	IN_SCK_H    GPIO_SetBits(GPIOD,GPIO_Pin_11) //数据输入时钟线，将当前si移位到寄存器->H->L
#define	IN_SCK_L    GPIO_ResetBits(GPIOD,GPIO_Pin_11)

#define	IN_RCK_H    GPIO_SetBits(GPIOD,GPIO_Pin_10) //输出存储器锁存时钟线,改变输出数据->H->L
#define	IN_RCK_L    GPIO_ResetBits(GPIOD,GPIO_Pin_10)

#define IN_SI_LED_ON   IN_SI_L
#define IN_SI_LED_OFF  IN_SI_H

#define	OUT_SI_H     GPIO_SetBits(GPIOD,GPIO_Pin_13)     //数据线
#define OUT_SI_L	    GPIO_ResetBits(GPIOD,GPIO_Pin_13) 

#define	OUT_SCK_H    GPIO_SetBits(GPIOG,GPIO_Pin_12) //数据输入时钟线，将当前si移位到寄存器->H->L
#define	OUT_SCK_L    GPIO_ResetBits(GPIOG,GPIO_Pin_12)

#define	OUT_RCK_H    GPIO_SetBits(GPIOG,GPIO_Pin_11) //输出存储器锁存时钟线,改变输出数据->H->L
#define	OUT_RCK_L    GPIO_ResetBits(GPIOG,GPIO_Pin_11)

#define OUT_SI_LED_ON   OUT_SI_L
#define OUT_SI_LED_OFF  OUT_SI_H

void PinConfig74HC595(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG, ENABLE);//使能相关时钟
	  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   
		
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	  IN_SCK_L;
	  IN_RCK_L;
}	

void WriteToInLed(u8* data, u8 num)
{
	u8 i,j;
	//u8 tmp;
	
	IN_RCK_L;
	IN_SCK_L;
	DelayUS(100);
	for (i=num;i>0;i--)
		for (j=8;j>0;j--)
		{
			
			//tmp = data[i]>>(8-j);
			if(data[i-1]&(0x01<<(j-1)))
			{
				IN_SI_LED_ON;
			}
			else
			{
				IN_SI_LED_OFF;
			}			
			IN_SCK_H;		
			DelayUS(100);
			IN_SCK_L;
			DelayUS(100);
		}
	
	IN_RCK_H;
	delay_ns(100);
	IN_RCK_L;
}
void WriteToOutLed(u8* data, u8 num)
{
	u8 i,j;
	//u8 tmp;
	
	OUT_RCK_L;
	OUT_SCK_L;
	DelayUS(100);
	for (i=num;i>0;i--)
		for (j=8;j>0;j--)
		{
			
			//tmp = data[i]>>(8-j);
			if(data[i-1]&(0x01<<(j-1)))
			{
				OUT_SI_LED_ON;
			}
			else
			{
				OUT_SI_LED_OFF;
			}			
			OUT_SCK_H;		
			DelayUS(100);
			OUT_SCK_L;
			DelayUS(100);
		}
	
	OUT_RCK_H;
	delay_ns(100);
	OUT_RCK_L;
}