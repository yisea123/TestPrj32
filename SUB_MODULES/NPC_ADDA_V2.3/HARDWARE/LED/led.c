#include "led.h"

u32 data ;

void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE );	  //使能GPIOA GPIOB GPIOC GPIOD通道时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 	// 端口配置PB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7;	 	// 端口配置PB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	 	// 端口配置PB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9;	 	// 端口配置PB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	 	// 端口配置PC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}		
//***************************************************************
void Led_Test(void)
{
	  GPIO_ResetBits(GPIOB,GPIO_Pin_7);  //调试指示灯;
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //运行指示灯;
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //故障指示灯;
		delay_ms(50);  		   						 
		GPIO_SetBits(GPIOB,GPIO_Pin_7);  	 
		GPIO_SetBits(GPIOB,GPIO_Pin_1);  	 
		GPIO_SetBits(GPIOB,GPIO_Pin_0);  	 
		delay_ms(50);	 
}


void IN_Hc595SendByte(u32 data)
{
	u32 i;
	
	IN_RCK_DISABLE;
		for(i=0;i<=15;i++)
		{
			if((data>>i)&0x01)
				IN_SDA_ENABLE;
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

//********************************************************
//整个I口状态读取
//num:第几个I口，1-20
//********************************************************
//u16 Read_Input_All()
//{
//	u16  s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14;
//	u16 state=0xffff;

//	s1=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9); 
//	s2=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8); 
//	s3=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7); 
//	s4=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6); 
//	s5=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5); 
//	s6=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4); 
//	s7=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3); 
//	s8=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2); 
//	s9=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12); 
//	s10=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11); 
//	s11=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10); 
//	s12=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15); 
//	s13=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12); 
//	s14=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11); 
//	
//state = (s1<<15)|(s2<<14)|(s3<<13)|(s4<<12)|(s5<<11)|(s6<<10)|(s7<<9)
//				|(s8<<8)|(s9<<7)|(s10<<6)|(s11<<5)|(s12<<4)|(s13<<3)
//				|(s14<<2);
//state =  state|0x0001;	
//return state;
//}

//********************************************************
//整个O口状态读取
//num:第几个I口，1-22
//********************************************************
//u16 Read_Output_ALL()
//{
//	u16  s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15;
//	u16 state = 0xffff;
//	 
//	s1=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_4);
//	s2=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5);
//	s3=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0);
//	s4=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_1);
//	s5=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_10);
//	s6=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_11);
//	s7=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12);
//	s8=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13);
//	s9=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14);
//	s10=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15);
//	s11=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_6);
//	s12=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_7);
//	s13=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_8);
//	s14=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_9);
//	s15=GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8);

//	
//	state = (s1<<15)|(s2<<14)|(s3<<13)|(s4<<12)|(s5<<11)|(s6<<10)|(s7<<9)
//				|(s8<<8)|(s9<<7)|(s10<<6)|(s11<<5)|(s12<<4)|(s13<<3)
//				|(s14<<2)|(s15<<1);
////	state = state|Alarm_flag(Alarmflag);
//	return state;
//}
