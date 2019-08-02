#include "DAC7512.h"


//**********************************************************
//DAC7512引脚配置
//
//**********************************************************
void delay_ns(u32 cnt)
{
	u32 j;
	cnt=cnt/6;
	for(j=0;j<cnt;j++);
}

void DAC7512_Pin_Config(void)
{
	
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE );	  //使能GPIOA GPIOB GPIOC GPIOD通道时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;	 	// 端口配置PA
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	 // 端口配置PB
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	    // 端口配置PC
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	     			 // 端口配置PD
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	    //关闭调试 端口重新映射  使用仿真器调试时，不能用此语  SWJ 完全禁用
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //关闭调试 端口重新映射  使用仿真器调试时，不能用此语  JTAG-DP 禁用 + SW-DP 使能
}	

//**********************************************************
//通道3输出电压Vo=3.3*data/4095
//
//**********************************************************
void WriteToDAC1_7512(u16 data)
{
	u8 i;
	u16 tmp;
	if(data > 4095 )data = 4095;
	DAC7512_SYNC_ENABLE;
	delay_ns(1000);
	DAC7512_SYNC_DISABLE;
	for (i=0;i<16;i++)
	{
		DAC7512_CCLK_ENABLE;		
		delay_ns(1000);
		tmp = data>>(15-i);
		if(tmp&0x01)
		{
			DAC7512_DIN_ENABLE;
        }
        else
        {
			DAC7512_DIN_DISABLE;
		}			
		delay_ns(1000);
		DAC7512_CCLK_DISABLE;
	}
	delay_ns(1000);
	DAC7512_SYNC_DISABLE;
}

//**********************************************************
//通道4输出电压Vo=3.3*data/4095
//
//**********************************************************
void WriteToDAC2_7512(u16 data)
{
	u8 i;
	u16 tmp;
	u16 temp2;
	if(data > 4095 )data = 4095;
	DAC7512_SYNC_ENABLE1;
	delay_ns(1000);
	DAC7512_SYNC_DISABLE1;
	for (i=0;i<16;i++)
	{
		DAC7512_CCLK_ENABLE1;		
		delay_ns(1000);
		tmp = data>>(15-i);
		if(tmp&0x01)
		{
			DAC7512_DIN_ENABLE1;
        }
        else
        {
			DAC7512_DIN_DISABLE1;
		}			
		delay_ns(1000);
		DAC7512_CCLK_DISABLE1;
	}
	delay_ns(1000);
	DAC7512_SYNC_DISABLE1;
}

//**********************************************************
//通道4输出电压Vo=3.3*data/4095
//
//**********************************************************
void WriteToDAC3_7512(u16 data)
{
	u8 i;
	u16 tmp;
	u16 temp2;
	if(data > 4095 )data = 4095;
	DAC7512_SYNC_ENABLE2;
	delay_ns(1000);
	DAC7512_SYNC_DISABLE2;
	for (i=0;i<16;i++)
	{
		DAC7512_CCLK_ENABLE2;		
		delay_ns(1000);
		tmp = data>>(15-i);
		if(tmp&0x01)
		{
			DAC7512_DIN_ENABLE2;
        }
        else
        {
			DAC7512_DIN_DISABLE2;
		}			
		delay_ns(1000);
		DAC7512_CCLK_DISABLE2;
	}
	delay_ns(1000);
	DAC7512_SYNC_DISABLE2;
}

//**********************************************************
//通道4输出电压Vo=3.3*data/4095
//
//**********************************************************
void WriteToDAC4_7512(u16 data)
{
	u8 i;
	u16 tmp;
	u16 temp2;
	if(data > 4095 )data = 4095;
	DAC7512_SYNC_ENABLE3;
	delay_ns(1000);
	DAC7512_SYNC_DISABLE3;
	for (i=0;i<16;i++)
	{
		DAC7512_CCLK_ENABLE3;		
		delay_ns(1000);
		tmp = data>>(15-i);
		if(tmp&0x01)
		{
			DAC7512_DIN_ENABLE3;
        }
        else
        {
			DAC7512_DIN_DISABLE3;
		}			
		delay_ns(1000);
		DAC7512_CCLK_DISABLE3;
	}
	delay_ns(1000);
	DAC7512_SYNC_DISABLE3;
}
