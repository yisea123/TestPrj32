#include "led.h"

u32 data ;

void LED_Init(void)
{
	
#if _NPC_VERSION_ == 1u
		#if defined(CDV_V1)
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOFʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_11| GPIO_Pin_13;          //����ָʾ�ƶ�ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
  GPIO_Init(GPIOD, &GPIO_InitStructure);              //��ʼ��H
	
		#else
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOFʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;          //����ָʾ�ƶ�ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
  GPIO_Init(GPIOB, &GPIO_InitStructure);              //��ʼ��H
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;    //led2|LED״ָ̬ʾ������
	GPIO_Init(GPIOH, &GPIO_InitStructure);                                             //��ʼ��D
	 
	#endif
	
#elif _NPC_VERSION_ == 2u
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOFʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;          //����ָʾ�ƶ�ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
  GPIO_Init(GPIOB, &GPIO_InitStructure);              //��ʼ��H
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;    //led2|LED״ָ̬ʾ������
	GPIO_Init(GPIOD, &GPIO_InitStructure);                                             //��ʼ��D
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_11|GPIO_Pin_12;                  //RUN|LED״ָ̬ʾ������
	GPIO_Init(GPIOG, &GPIO_InitStructure);                                             //��ʼ��G

#elif _NPC_VERSION_ == 3u
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOFʱ��
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;          //����ָʾ�ƶ�ӦIO��
  GPIO_Init(GPIOB, &GPIO_InitStructure);              //��ʼ��H
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11;    //led2|LED״ָ̬ʾ������
	GPIO_Init(GPIOH, &GPIO_InitStructure);                                             //��ʼ��D
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;                  //RUN|LED״ָ̬ʾ������
	GPIO_Init(GPIOI, &GPIO_InitStructure);    
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;                  //RUN|LED״ָ̬ʾ������
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
	
#endif

}		
//***************************************************************
void Led_Test(void)
{
	  GPIO_ResetBits(GPIOB,GPIO_Pin_6);  //����ָʾ��;
		GPIO_ResetBits(GPIOG,GPIO_Pin_4);  //����ָʾ��;
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
������ʾ
******************************************/
void IN_DisPlay(u32 data)
{

	IN_RCK_DISABLE;       		//����״̬��ʾ
	IN_Hc595SendByte(data);
	IN_RCK_ENABLE;
	 
}
/******************************************
�����ʾ
*******************************************/
void OUT_DisPlay(u32 data)
{
	OUT_RCK_DISABLE;					//���״̬��ʾ
	OUT_Hc595SendByte(data);
	OUT_RCK_ENABLE;
	
}


