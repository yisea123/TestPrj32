#include "led.h"

u32 data ;

void LED_Init(void)
{
	
#if _NPC_VERSION_ == 1u
		#if defined(CDV_V1)
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOF时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_11| GPIO_Pin_13;          //调试指示灯对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);              //初始化H
	
		#else
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOF时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;          //调试指示灯对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);              //初始化H
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;    //led2|LED状态指示灯配置
	GPIO_Init(GPIOH, &GPIO_InitStructure);                                             //初始化D
	 
	#endif
	
#elif _NPC_VERSION_ == 2u
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOF时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;          //调试指示灯对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);              //初始化H
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;    //led2|LED状态指示灯配置
	GPIO_Init(GPIOD, &GPIO_InitStructure);                                             //初始化D
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_11|GPIO_Pin_12;                  //RUN|LED状态指示灯配置
	GPIO_Init(GPIOG, &GPIO_InitStructure);                                             //初始化G

#elif _NPC_VERSION_ == 3u
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOF时钟
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;          //调试指示灯对应IO口
  GPIO_Init(GPIOB, &GPIO_InitStructure);              //初始化H
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11;    //led2|LED状态指示灯配置
	GPIO_Init(GPIOH, &GPIO_InitStructure);                                             //初始化D
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;                  //RUN|LED状态指示灯配置
	GPIO_Init(GPIOI, &GPIO_InitStructure);    
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;                  //RUN|LED状态指示灯配置
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
	
#endif

}		
//***************************************************************
void Led_Test(void)
{
	  GPIO_ResetBits(GPIOB,GPIO_Pin_6);  //调试指示灯;
		GPIO_ResetBits(GPIOG,GPIO_Pin_4);  //运行指示灯;
		delay_ms(50);  		   						 
		GPIO_SetBits(GPIOB,GPIO_Pin_6);  	 
		GPIO_SetBits(GPIOG,GPIO_Pin_4);  	 
		delay_ms(50);	 
}


void IN_Hc595SendByte(u32 data)
{
	u32 i;
	
	IN_RCK_DISABLE;
		for(i=0;i<=23;i++)
		{
			if((data>>i)&0x01)
				IN_SDA_ENABLE ;
			else 
				IN_SDA_DISABLE;
			IN_SCK_DISABLE;
			delay_ms(1);
			IN_SCK_ENABLE;
			
		}
		IN_RCK_ENABLE;
}

void OUT_Hc595SendByte(u32 data)
{
  u32 i;
	OUT_RCK_DISABLE;
	for(i=0;i<=23; i++)
		{
		  if((data>>i)&0x01)
				OUT_SDA_ENABLE;
			else 
				OUT_SDA_DISABLE;
			OUT_SCK_DISABLE;
			delay_ms(1);
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


