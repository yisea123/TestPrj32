#include "DAC7512.h"
void delay_ns(u32 cnt)
{
	u32 j;
	cnt=cnt/6;
	for(j=0;j<cnt;j++);
}

//**********************************************************
//DAC7512��������
//
//**********************************************************
void DAC7512_Pin_Config(void)
{
//	Fpga1_Config_Init();
#if _NPC_VERSION_ == 1u
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ�����ʱ��

	  #ifdef CDV_V2_2
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
	    
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOG, &GPIO_InitStructure); 
    #else	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3 | GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);	
	  #endif
		
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
#elif _NPC_VERSION_ == 2u
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOH, ENABLE);//ʹ�����ʱ��
	  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   
		//7512_1
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	  //7512_2
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
#elif _NPC_VERSION_ == 3u
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOH, ENABLE);//ʹ�����ʱ��
	  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
		GPIO_Init(GPIOH, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}	

//**********************************************************
//�����ѹVo=3.3*data/4095
//
//**********************************************************
void WriteToDAC7512(u16 data)
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
//ͨ��4�����ѹVo=3.3*data/4095
//
//**********************************************************
void WriteToDA2C_7512(u16 data)
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
