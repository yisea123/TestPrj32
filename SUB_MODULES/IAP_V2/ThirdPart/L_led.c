#include "led.h" 
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

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
  /*GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//LED0��LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10���øߣ�����*/
	GPIO_Configuration();

}

void GPIO_Configuration(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
						|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF
						|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI, ENABLE);//ʹ�����ʱ�� 
	//�������������
    //PA  13/14 swio/swclk
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB   2Ϊboot1  6��7     10��11 �Ǵ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//PC    10��11 12����           14��15����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7 |GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	//PD   2  5��6Ϊ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_7| GPIO_Pin_9| GPIO_Pin_11|GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	//PE	A1 A2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8 |GPIO_Pin_9|  GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	//PF        �޴���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_3 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	//PG    9��14����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	//PH       0��1����   4��5ΪI2C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	//PI   0��1��2��3SPI����
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOI, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOI, &GPIO_InitStructure);	
/***************************************************������Dividing line*****************************************************************/	

//	sys.RunPort = ((GPIO_TypeDef *) GPIOD_BASE);
//	sys.RunIO = ((uint16_t)0x0200);  /* Pin 9 selected      */
//	sys.WaringPort = ((GPIO_TypeDef *) GPIOD_BASE);
//	sys.WaringIO = ((uint16_t)0x0800);  /* Pin 11 selected      */
//	sys.ErrorPort = ((GPIO_TypeDef *) GPIOD_BASE);
//	sys.ErrorIO = ((uint16_t)0x2000);  /* Pin 13 selected      */
//	sys.Addr1Port = ((GPIO_TypeDef *) GPIOB_BASE);
//	sys.Addr1IO = ((uint16_t)0x0010); /* Pin 4 selected      */
//	sys.Addr2Port = ((GPIO_TypeDef *) GPIOB_BASE);
//	sys.Addr2IO = ((uint16_t)0x0020); /* Pin 5 selected      */
//	sys.Addr3Port = ((GPIO_TypeDef *) GPIOG_BASE);
//	sys.Addr3IO = ((uint16_t)0x8000); /* Pin 15 selected      */
//	sys.Addr4Port = ((GPIO_TypeDef *) GPIOB_BASE);
//	sys.Addr4IO = ((uint16_t)0x0008); /* Pin 3 selected      */
//	sys.PG1Port = ((GPIO_TypeDef *) GPIOG_BASE);
//	sys.PG1IO = ((uint16_t)0x1000); /* Pin 12 selected      */
//	sys.PG2Port = ((GPIO_TypeDef *) GPIOG_BASE);
//	sys.PG2IO = ((uint16_t)0x2000); /* Pin 13 selected      */
//	sys.A0Port = ((GPIO_TypeDef *) GPIOG_BASE);
//	sys.A0IO = ((uint16_t)0x0002); /* Pin 1 selected      */
//	sys.A1Port = ((GPIO_TypeDef *) GPIOE_BASE);
//	sys.A1IO = ((uint16_t)0x0080); /* Pin 7 selected      */
//	sys.A2Port = ((GPIO_TypeDef *) GPIOE_BASE);
//	sys.A2IO = ((uint16_t)0x0100); /* Pin 8 selected      */
//	sys.S1Port = ((GPIO_TypeDef *) GPIOE_BASE);
//	sys.S1IO = ((uint16_t)0x0200); /* Pin 9 selected      */

}




