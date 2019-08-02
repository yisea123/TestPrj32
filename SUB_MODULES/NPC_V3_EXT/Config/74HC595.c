#include "74HC595.h"


#define	IN_SI_H     GPIO_SetBits(GPIOC,GPIO_Pin_4)     //数据线
#define IN_SI_L	    GPIO_ResetBits(GPIOC,GPIO_Pin_4) 

#define	IN_SCK_H    GPIO_SetBits(GPIOA,GPIO_Pin_7) //数据输入时钟线，将当前si移位到寄存器->H->L
#define	IN_SCK_L    GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define	IN_RCK_H    GPIO_SetBits(GPIOA,GPIO_Pin_6) //输出存储器锁存时钟线,改变输出数据->H->L
#define	IN_RCK_L    GPIO_ResetBits(GPIOA,GPIO_Pin_6)


#define IN_SI_LED_ON   IN_SI_L
#define IN_SI_LED_OFF  IN_SI_H


#define	OUT_SI_H     GPIO_SetBits(GPIOC,GPIO_Pin_6)     //数据线
#define OUT_SI_L	   GPIO_ResetBits(GPIOC,GPIO_Pin_6) 

#define	OUT_SCK_H    GPIO_SetBits(GPIOC,GPIO_Pin_8) //数据输入时钟线，将当前si移位到寄存器->H->L
#define	OUT_SCK_L    GPIO_ResetBits(GPIOC,GPIO_Pin_8)

#define	OUT_RCK_H    GPIO_SetBits(GPIOC,GPIO_Pin_9) //输出存储器锁存时钟线,改变输出数据->H->L
#define	OUT_RCK_L    GPIO_ResetBits(GPIOC,GPIO_Pin_9)


#define OUT_SI_LED_ON   OUT_SI_L
#define OUT_SI_LED_OFF  OUT_SI_H

void Init74HC595(void)
{
	u32 val = 0xFFFFFFFF;
	  IN_SCK_L;
	  IN_RCK_L;
	
	WriteToOutLed((u8*)&val, 4);
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
	DelayUS(1);
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
	DelayUS(1);
	OUT_RCK_L;
}
