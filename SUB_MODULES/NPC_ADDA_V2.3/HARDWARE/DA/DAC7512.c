#include "DAC7512.h"


//**********************************************************
//DAC7512��������
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE );	  //ʹ��GPIOA GPIOB GPIOC GPIODͨ��ʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;	 	// �˿�����PA
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	 // �˿�����PB
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	    // �˿�����PC
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	     			 // �˿�����PD
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	    //�رյ��� �˿�����ӳ��  ʹ�÷���������ʱ�������ô���  SWJ ��ȫ����
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //�رյ��� �˿�����ӳ��  ʹ�÷���������ʱ�������ô���  JTAG-DP ���� + SW-DP ʹ��
}	

//**********************************************************
//ͨ��3�����ѹVo=3.3*data/4095
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
//ͨ��4�����ѹVo=3.3*data/4095
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
//ͨ��4�����ѹVo=3.3*data/4095
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
//ͨ��4�����ѹVo=3.3*data/4095
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
