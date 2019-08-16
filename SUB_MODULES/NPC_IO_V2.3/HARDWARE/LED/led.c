#include "led.h"

u32 data ;

void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE );	  //使能GPIOA GPIOB GPIOC GPIOD通道时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	 	// 端口配置PA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 	// 端口配置PC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
		
}		
//***************************************************************
void Led_Test(void)
{
	  GPIO_ResetBits(GPIOC,GPIO_Pin_3);  //调试指示灯;
  	GPIO_ResetBits(GPIOC,GPIO_Pin_2);  //运行指示灯;
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		delay_ms(50); 
//	  IN_DisPlay(0xaaaa);
//    OUT_DisPlay(0xaaaa);
//	  delay_ms(10); 
//	  IN_DisPlay(0x5555);
//	  OUT_DisPlay(0x5555);	
		GPIO_SetBits(GPIOC,GPIO_Pin_3);  	 
		GPIO_SetBits(GPIOC,GPIO_Pin_2);  	 
		GPIO_SetBits(GPIOC,GPIO_Pin_0);  	 
		delay_ms(50);	 
}


void IN_Hc595SendByte(u32 data)
{
	u32 i;
	
	IN_RCK_DISABLE;
		for(i=0;i<=15;i++)
		{
			if((data>>i)&0x01)
				IN_SDA_ENABLE ;
			else 
				IN_SDA_DISABLE;
			IN_SCK_DISABLE;
//			delay_ms(1);
			IN_SCK_ENABLE;
			
		}
		IN_RCK_ENABLE;
}

void OUT_Hc595SendByte(u32 data)
{
  u32 i;
	OUT_RCK_DISABLE;
	for(i=0;i<=15; i++)
		{
		  if((data>>i)&0x01)
				OUT_SDA_ENABLE;
			else 
				OUT_SDA_DISABLE;
			OUT_SCK_DISABLE;
//			delay_ms(1);
			OUT_SCK_ENABLE;
		}
	OUT_RCK_DISABLE;
		
}
/*****************************************
输入显示
******************************************/
void IN_DisPlay(u32 data)
{

	IN_RCK_DISABLE;       		//输入状态显示
	IN_Hc595SendByte(data);
	IN_RCK_ENABLE;
	 
}
/******************************************
输出显示
*******************************************/
void OUT_DisPlay(u32 data)
{
	OUT_RCK_DISABLE;					//输出状态显示
	OUT_Hc595SendByte(data);
	OUT_RCK_ENABLE;
	
}


