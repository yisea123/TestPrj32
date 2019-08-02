/**
  ******************************************************************************
  * @file    /IO.c 
  * @author  MMY
  * @version V0.0.1
  * @date    1-12-2015
  * @brief   a package of io and dip function
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "IO.h"

CDV_IO g_cdvI[CDV_I_NUM];
CDV_IO g_cdvO[CDV_O_NUM];
CDV_IO g_dipSW[CDV_DIP_NUM];
CDV_INT08U g_dip = 0;

CDV_INT08U* g_i = (CDV_INT08U*)g_modbusInCoil.coilCh;
CDV_INT08U* g_o = (CDV_INT08U*)g_modbusCoil.coilCh;

#if ENABLE_FPGA_DOWN
CDV_INT08U g_otmp[30] = {0};
#endif
///*�ڲ�����*/
//void IParWrite(CDV_INT32U no , CDV_INT16U flag , CDV_INT08U* buf);
//void IParRead(CDV_INT32U no , CDV_INT16U flag , CDV_INT08U* buf , CDV_INT08U* len);

OS_MUTEX PLUSE_SEM;
CDV_INT08U g_pluse[COIL_CHN];

/*GPIO����*/

void CDV1_GpioConfig(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
						|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF
						|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI, ENABLE);//ʹ�����ʱ�� 
#if defined(CDV_V1)
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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10| GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
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
#else
//elif defined(CDV_V2)
	//�������������
    //PA  13/14 swio/swclk
	#ifdef CDV_V2_2 
	  //GPIO_Pin_0ΪADC1�ӿ�
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2 ;
	#else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2 ;
	#endif	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	#ifdef CDV_V2_2 
	  //GPIO_Pin_4 | GPIO_Pin_5ΪDAC�ӿڽӿ�
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12|GPIO_Pin_15;
	#else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12|GPIO_Pin_15;
  #endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB   2Ϊboot1  6��7     10��11 �Ǵ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  #ifdef CDV_V2_2 
	  //GPIO_Pin_3 ΪSPI1 SLCKͨ�Žӿ�
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_15 ;
	#else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_15 ;
  #endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	//PC    10��11 12����           14��15����
	#ifdef CDV_V2_2 
	  //GPIO_Pin_0ΪADC2�ӿ�
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7 |GPIO_Pin_8;
	#else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7 |GPIO_Pin_8;
	#endif
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
	#ifdef CDV_V2_2
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12|GPIO_Pin_14 | GPIO_Pin_15;
	#else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_10 | GPIO_Pin_12 |  GPIO_Pin_14 | GPIO_Pin_15;
	#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	#ifdef CDV_V2_2
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_7| GPIO_Pin_9| GPIO_Pin_11|GPIO_Pin_13 ;
	#else
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_7| GPIO_Pin_9| GPIO_Pin_11|GPIO_Pin_13;
	#endif
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
	//GPIO_Pin_10ΪADC3�ӿ�
	#ifdef CDV_V2_2 
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	#else
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
	#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	//PG    9��14����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	//PH       0��1����   4��5ΪI2C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�������������Ĵ���
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12  | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
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
#endif
/***************************************************������Dividing line*****************************************************************/	

}

void NPC2_GpioConfig(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
						|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF
						|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI, ENABLE);//ʹ�����ʱ�� 
	/**
	 *���������
	 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//PE 7 8 9 10 in
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	//PH 2 3 4 5 in
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	//PA 0 in
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PC 0 2 3 in
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PF 1 2 3 4 5 6 7 8 9 10 in
	GPIO_InitStructure.GPIO_Pin = /*GPIO_Pin_1 | GPIO_Pin_2 |*/ GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	//PI 5 6 dipSW
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	//PH 10 dipSW
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	/**
	 *���������
	 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//PH 13 14 15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15 ;
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	//PA 12
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PD 0 1 4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//PG 15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	//PE 0 2 3 4 5 6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//PI 7 8 9 10 11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	//PB 7 9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PC 13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PB 15 6 RED LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PG 4 RUN LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void NPC2_3_DipConfig(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ�����ʱ�� 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void NPC2_3_GpioConfig(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
						|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF
						|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI, ENABLE);//ʹ�����ʱ�� 
	/**
	 *���������
	 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 
															| GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 
															| GPIO_Pin_2 
															| GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 
															| GPIO_Pin_6 
															| GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 
															| GPIO_Pin_7 
															| GPIO_Pin_8 
															| GPIO_Pin_9 
															| GPIO_Pin_10 
															| GPIO_Pin_11;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 
															| GPIO_Pin_3  
															| GPIO_Pin_4 
															| GPIO_Pin_5;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	/**
	 *���������
	 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 
															| GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//

#if USE_EXTI_POWER_OFF == 0u
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
#endif
	//
	GPIO_InitStructure.GPIO_Pin = 
#if USE_PULSE_DRIVE == 0u
	                            GPIO_Pin_8 | 
#endif
															GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 
															| GPIO_Pin_3 
															| GPIO_Pin_4 
															| GPIO_Pin_5 
															| GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11  
															| GPIO_Pin_12 
															| GPIO_Pin_13;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 
															| GPIO_Pin_14
#if USE_PULSE_DRIVE == 0u															
															| GPIO_Pin_15
#endif
															;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7
															| GPIO_Pin_8
															| GPIO_Pin_9
															| GPIO_Pin_10
															| GPIO_Pin_11;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
}

void GPIO_Configuration(void){
#if _NPC_VERSION_ == 1u
	CDV1_GpioConfig();
#elif _NPC_VERSION_ == 2u
	NPC2_GpioConfig();
#elif _NPC_VERSION_ == 3u
	NPC2_3_DipConfig();
	NPC2_3_GpioConfig();
#endif
}


#if _NPC_VERSION_ == 1u
void CDV1_IOInit(void) {
#if defined(CDV_V1)
	/*IO��ʼ��*/
	g_cdvO[0].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[0].pin = ((uint16_t)0x0020);  /* Pin 5 selected      */
	g_cdvO[1].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvO[1].pin = ((uint16_t)0x0002);  /* Pin 1 selected     */
	g_cdvO[2].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[2].pin = ((uint16_t)0x0010);  /* Pin 4 selected      */
	g_cdvO[3].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvO[3].pin = ((uint16_t)0x0001);  /* Pin 0 selected     */
	g_cdvO[4].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[4].pin = ((uint16_t)0x0008);  /* Pin 3 selected     */
	g_cdvO[5].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[5].pin = ((uint16_t)0x0800);  /* Pin 11 selected    */
	g_cdvO[6].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[6].pin = ((uint16_t)0x0004);  /* Pin 2 selected     */
	g_cdvO[7].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[7].pin = ((uint16_t)0x0400);  /* Pin 10 selected    */
	g_cdvO[8].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[8].pin = ((uint16_t)0x0080);  /* Pin 7 selected     */
	g_cdvO[9].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[9].pin = ((uint16_t)0x0200);  /* Pin 9 selected     */
	g_cdvO[10].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[10].pin = ((uint16_t)0x0040);  /* Pin 6 selected     */
	g_cdvO[11].port = ((GPIO_TypeDef *) GPIOC_BASE);
	g_cdvO[11].pin = ((uint16_t)0x2000);  /* Pin 13 selected    */
	g_cdvO[12].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[12].pin = ((uint16_t)0x0020);  /* Pin 5 selected     */
	g_cdvO[13].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[13].pin = ((uint16_t)0x0100);  /* Pin 8 selected     */
	g_cdvO[14].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[14].pin = ((uint16_t)0x0010);  /* Pin 4 selected     */
	g_cdvO[15].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[15].pin = ((uint16_t)0x0040);  /* Pin 6 selected     */
	
	g_cdvI[0].port = ((GPIO_TypeDef *) GPIOH_BASE);
	g_cdvI[0].pin = ((uint16_t)0x0008);  /* Pin 3 selected    */	
	g_cdvI[1].port = ((GPIO_TypeDef *) GPIOH_BASE);
	g_cdvI[1].pin = ((uint16_t)0x0004);  /* Pin 2 selected    */	
	g_cdvI[2].port = ((GPIO_TypeDef *) GPIOA_BASE);
	g_cdvI[2].pin = ((uint16_t)0x0004);  /* Pin 2 selected    */
	g_cdvI[3].port = ((GPIO_TypeDef *) GPIOA_BASE);
	g_cdvI[3].pin = ((uint16_t)0x0002);  /* Pin 1 selected    */
	g_cdvI[4].port = ((GPIO_TypeDef *) GPIOA_BASE);
	g_cdvI[4].pin = ((uint16_t)0x0001);  /* Pin 0 selected    */
	g_cdvI[5].port = ((GPIO_TypeDef *) GPIOC_BASE);
	g_cdvI[5].pin = ((uint16_t)0x0008);  /* Pin 3 selected    */	
	g_cdvI[6].port = ((GPIO_TypeDef *) GPIOC_BASE);
	g_cdvI[6].pin = ((uint16_t)0x0004);  /* Pin 2 selected    */
	g_cdvI[7].port = ((GPIO_TypeDef *) GPIOC_BASE);
	g_cdvI[7].pin = ((uint16_t)0x0002);  /* Pin 1 selected    */
	g_cdvI[8].port = ((GPIO_TypeDef *) GPIOC_BASE);
	g_cdvI[8].pin = ((uint16_t)0x0001);  /* Pin 0 selected    */
	g_cdvI[9].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[9].pin = ((uint16_t)0x0400);  /* Pin 10 selected    */
	g_cdvI[10].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[10].pin = ((uint16_t)0x0200);  /* Pin 9 selected    */	
	g_cdvI[11].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[11].pin = ((uint16_t)0x0100);  /* Pin 8 selected    */	
	g_cdvI[12].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[12].pin = ((uint16_t)0x0080);  /* Pin 7 selected    */	
	g_cdvI[13].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[13].pin = ((uint16_t)0x0040);  /* Pin 6 selected    */	
	g_cdvI[14].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[14].pin = ((uint16_t)0x0020);  /* Pin 5 selected    */	
	g_cdvI[15].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[15].pin = ((uint16_t)0x0010);  /* Pin 4 selected    */	
	/*���뿪��*/
	g_dipSW[0].port = PORT_G;
	g_dipSW[0].pin  = PIN_6;  /* Pin 6 selected    */	
	g_dipSW[1].port = PORT_G;
	g_dipSW[1].pin  = PIN_7;  /* Pin 7 selected    */	
	g_dipSW[2].port = PORT_G;
	g_dipSW[2].pin  = PIN_8;  /* Pin 8 selected    */	
	g_dipSW[3].port = PORT_C;
	g_dipSW[3].pin  = PIN_6;  /* Pin 6 selected    */	
	g_dipSW[4].port = PORT_C;
	g_dipSW[4].pin  = PIN_7;  /* Pin 7 selected    */	
	g_dipSW[5].port = PORT_C;
	g_dipSW[5].pin  = PIN_8;  /* Pin 8 selected    */	
#else
//#elif defined(CDV_V2)
	/*IO��ʼ��*/
	g_cdvO[0].port = ((GPIO_TypeDef *) GPIOG_BASE);
	g_cdvO[0].pin = ((uint16_t)0x8000);  /* Pin 15 selected    */
	g_cdvO[1].port = ((GPIO_TypeDef *) GPIOG_BASE);
	g_cdvO[1].pin = ((uint16_t)0x4000);  /* Pin 14 selected    */
	g_cdvO[2].port = ((GPIO_TypeDef *) GPIOG_BASE);
	g_cdvO[2].pin = ((uint16_t)0x2000);  /* Pin 13 selected    */
	g_cdvO[3].port = ((GPIO_TypeDef *) GPIOG_BASE);
	g_cdvO[3].pin = ((uint16_t)0x1000);  /* Pin 12 selected    */
	g_cdvO[4].port = ((GPIO_TypeDef *) GPIOG_BASE);
	g_cdvO[4].pin = ((uint16_t)0x0800);  /* Pin 11 selected    */
	g_cdvO[5].port = ((GPIO_TypeDef *) GPIOG_BASE);
	g_cdvO[5].pin = ((uint16_t)0x0400);  /* Pin 10 selected    */
	g_cdvO[6].port = ((GPIO_TypeDef *) GPIOG_BASE);
	g_cdvO[6].pin = ((uint16_t)0x0200);  /* Pin 9 selected     */
	g_cdvO[7].port = ((GPIO_TypeDef *) GPIOD_BASE);
	g_cdvO[7].pin = ((uint16_t)0x0080);  /* Pin 7 selected     */
	g_cdvO[8].port = ((GPIO_TypeDef *) GPIOB_BASE);
	g_cdvO[8].pin = ((uint16_t)0x0100);  /* Pin 8 selected     */
	g_cdvO[9].port = ((GPIO_TypeDef *) GPIOB_BASE);
	g_cdvO[9].pin = ((uint16_t)0x0200);  /* Pin 9 selected     */
	g_cdvO[10].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[10].pin = ((uint16_t)0x0001);  /* Pin 0 selected    */
	g_cdvO[11].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[11].pin = ((uint16_t)0x0002);  /* Pin 1 selected    */
	g_cdvO[12].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[12].pin = ((uint16_t)0x0020);  /* Pin 5 selected    */
	g_cdvO[13].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[13].pin = ((uint16_t)0x0040);  /* Pin 6 selected     */
	g_cdvO[14].port = ((GPIO_TypeDef *) GPIOI_BASE);
	g_cdvO[14].pin = ((uint16_t)0x0080);  /* Pin 7 selected     */
	g_cdvO[15].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[15].pin = ((uint16_t)0x0004);  /* Pin 2 selected     */
	g_cdvO[16].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[16].pin = ((uint16_t)0x0008);  /* Pin 3 selected     */
	g_cdvO[17].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[17].pin = ((uint16_t)0x0010);  /* Pin 4 selected     */
	g_cdvO[18].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[18].pin = ((uint16_t)0x0020);  /* Pin 5 selected     */
	g_cdvO[19].port = ((GPIO_TypeDef *) GPIOE_BASE);
	g_cdvO[19].pin = ((uint16_t)0x0040);  /* Pin 6 selected     */
	
	g_cdvI[0].port = ((GPIO_TypeDef *) GPIOH_BASE);
	g_cdvI[0].pin = ((uint16_t)0x0008);  /* Pin 3 selected    */	
	g_cdvI[1].port = ((GPIO_TypeDef *) GPIOH_BASE);
	g_cdvI[1].pin = ((uint16_t)0x0004);  /* Pin 2 selected    */	
	g_cdvI[2].port = ((GPIO_TypeDef *) GPIOA_BASE);
	g_cdvI[2].pin = ((uint16_t)0x0004);  /* Pin 2 selected    */
	g_cdvI[3].port = ((GPIO_TypeDef *) GPIOA_BASE);
	g_cdvI[3].pin = ((uint16_t)0x0002);  /* Pin 1 selected    */
	#ifdef CDV_V2_2
		g_cdvI[4].port = ((GPIO_TypeDef *) GPIOD_BASE);
		g_cdvI[4].pin = ((uint16_t)0x0010);  /* Pin 4 selected    */
	#else
    g_cdvI[4].port = ((GPIO_TypeDef *) GPIOA_BASE);
	  g_cdvI[4].pin = ((uint16_t)0x0001);  /* Pin 0 selected    */
  #endif
	
	g_cdvI[5].port = ((GPIO_TypeDef *) GPIOC_BASE);
	g_cdvI[5].pin = ((uint16_t)0x0008);  /* Pin 3 selected    */	
	g_cdvI[6].port = ((GPIO_TypeDef *) GPIOC_BASE);
	g_cdvI[6].pin = ((uint16_t)0x0004);  /* Pin 2 selected    */
	g_cdvI[7].port = ((GPIO_TypeDef *) GPIOC_BASE);
	g_cdvI[7].pin = ((uint16_t)0x0002);  /* Pin 1 selected    */
	#ifdef CDV_V2_2
		g_cdvI[8].port = ((GPIO_TypeDef *) GPIOD_BASE);
		g_cdvI[8].pin = ((uint16_t)0x0008);  /* Pin 3 selected    */
	#else
    g_cdvI[8].port = ((GPIO_TypeDef *) GPIOC_BASE);
	  g_cdvI[8].pin = ((uint16_t)0x0001);  /* Pin 0 selected    */
  #endif
	
	#ifdef CDV_V2_2
		g_cdvI[9].port = ((GPIO_TypeDef *) GPIOD_BASE);
		g_cdvI[9].pin = ((uint16_t)0x0002);  /* Pin 1 selected    */
	#else	
	  g_cdvI[9].port = ((GPIO_TypeDef *) GPIOF_BASE);
	  g_cdvI[9].pin = ((uint16_t)0x0400);  /* Pin 10 selected    */
	#endif
	
	g_cdvI[10].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[10].pin = ((uint16_t)0x0200);  /* Pin 9 selected    */	
	g_cdvI[11].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[11].pin = ((uint16_t)0x0100);  /* Pin 8 selected    */	
	g_cdvI[12].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[12].pin = ((uint16_t)0x0080);  /* Pin 7 selected    */	
	g_cdvI[13].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[13].pin = ((uint16_t)0x0040);  /* Pin 6 selected    */	
	g_cdvI[14].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[14].pin = ((uint16_t)0x0020);  /* Pin 5 selected    */	
	g_cdvI[15].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[15].pin = ((uint16_t)0x0010);  /* Pin 4 selected    */	
	g_cdvI[16].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[16].pin = ((uint16_t)0x0008);  /* Pin 3 selected    */	
	g_cdvI[17].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[17].pin = ((uint16_t)0x0004);  /* Pin 2 selected    */
	g_cdvI[18].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[18].pin = ((uint16_t)0x0002);  /* Pin 1 selected    */
	g_cdvI[19].port = ((GPIO_TypeDef *) GPIOF_BASE);
	g_cdvI[19].pin = ((uint16_t)0x0001);  /* Pin 0 selected    */
	/*���뿪��*/
	g_dipSW[0].port = PORT_G;
	g_dipSW[0].pin  = PIN_4;  /* Pin 4 selected    */	
	g_dipSW[1].port = PORT_G;
	g_dipSW[1].pin  = PIN_5;  /* Pin 5 selected    */	
	g_dipSW[2].port = PORT_G;
	g_dipSW[2].pin  = PIN_6;  /* Pin 6 selected    */	
	g_dipSW[3].port = PORT_G;
	g_dipSW[3].pin  = PIN_7;  /* Pin 7 selected    */	
	g_dipSW[4].port = PORT_C;
	g_dipSW[4].pin  = PIN_7;  /* Pin 7 selected    */	
	g_dipSW[5].port = PORT_C;
	g_dipSW[5].pin  = PIN_8;  /* Pin 8 selected    */		
#endif
}
#endif

#if _NPC_VERSION_ == 2u
void NPC2_IOInit(void) {
	/**
	*O��ʼ��
	*/
	g_cdvO[0].port = PORT_H;
	g_cdvO[0].pin  = PIN_13;
	g_cdvO[1].port = PORT_B;//PORT_A;
	g_cdvO[1].pin  = PIN_15;//PIN_12;
	g_cdvO[2].port = PORT_D;
	g_cdvO[2].pin  = PIN_1;
	g_cdvO[3].port = PORT_D;
	g_cdvO[3].pin  = PIN_4;
	g_cdvO[4].port = PORT_H;
	g_cdvO[4].pin  = PIN_15;
	g_cdvO[5].port = PORT_H;
	g_cdvO[5].pin  = PIN_14;
	g_cdvO[6].port = PORT_G;
	g_cdvO[6].pin  = PIN_15;
	g_cdvO[7].port = PORT_D;
	g_cdvO[7].pin  = PIN_0;
	g_cdvO[8].port = PORT_E;
	g_cdvO[8].pin  = PIN_3;
	g_cdvO[9].port = PORT_E;
	g_cdvO[9].pin  = PIN_5;
	g_cdvO[10].port = PORT_E;
	g_cdvO[10].pin  = PIN_4;
	g_cdvO[11].port = PORT_E;
	g_cdvO[11].pin  = PIN_6;
	g_cdvO[12].port = PORT_E;
	g_cdvO[12].pin  = PIN_0;
	g_cdvO[13].port = PORT_I;
	g_cdvO[13].pin  = PIN_7;
	g_cdvO[14].port = PORT_B;
	g_cdvO[14].pin  = PIN_9;
	g_cdvO[15].port = PORT_I;
	g_cdvO[15].pin  = PIN_9;
	g_cdvO[16].port = PORT_E;
	g_cdvO[16].pin  = PIN_2;
	g_cdvO[17].port = PORT_I;
	g_cdvO[17].pin  = PIN_11;
	g_cdvO[18].port = PORT_I;
	g_cdvO[18].pin  = PIN_8;
	g_cdvO[19].port = PORT_C;
	g_cdvO[19].pin  = PIN_13;
	g_cdvO[20].port = PORT_I;
	g_cdvO[20].pin  = PIN_10;
	g_cdvO[21].port = PORT_B;
	g_cdvO[21].pin  = PIN_7;
	
	/**
	*I��ʼ��
	*/
	g_cdvI[0].port = PORT_E;
	g_cdvI[0].pin  = PIN_9;
	g_cdvI[1].port = PORT_E;
	g_cdvI[1].pin  = PIN_8;
	g_cdvI[2].port = PORT_E;
	g_cdvI[2].pin  = PIN_10;
	g_cdvI[3].port = PORT_E;
	g_cdvI[3].pin  = PIN_7;
	g_cdvI[4].port = PORT_H;
	g_cdvI[4].pin  = PIN_3;
	g_cdvI[5].port = PORT_H;
	g_cdvI[5].pin  = PIN_5;
	g_cdvI[6].port = PORT_H;
	g_cdvI[6].pin  = PIN_2;
	g_cdvI[7].port = PORT_H;
	g_cdvI[7].pin  = PIN_4;
	g_cdvI[8].port = PORT_A;
	g_cdvI[8].pin  = PIN_0;
	g_cdvI[9].port = PORT_C;
	g_cdvI[9].pin  = PIN_3;
	g_cdvI[10].port = PORT_C;
	g_cdvI[10].pin  = PIN_2;
	g_cdvI[11].port = PORT_C;
	g_cdvI[11].pin  = PIN_0;
	g_cdvI[12].port = PORT_F;
	g_cdvI[12].pin  = PIN_10;
	g_cdvI[13].port = PORT_F;
	g_cdvI[13].pin  = PIN_9;
	g_cdvI[14].port = PORT_F;
	g_cdvI[14].pin  = PIN_8;
	g_cdvI[15].port = PORT_F;
	g_cdvI[15].pin  = PIN_7;
	g_cdvI[16].port = PORT_F;
	g_cdvI[16].pin  = PIN_6;
	g_cdvI[17].port = PORT_F;
	g_cdvI[17].pin  = PIN_5;
	g_cdvI[18].port = PORT_F;
	g_cdvI[18].pin  = PIN_4;
	g_cdvI[19].port = PORT_F;
	g_cdvI[19].pin  = PIN_3;
//	g_cdvI[20].port = PORT_F;
//	g_cdvI[20].pin  = PIN_2;
//	g_cdvI[21].port = PORT_F;
//	g_cdvI[21].pin  = PIN_1;
	/**
	*���뿪��
	*/
	g_dipSW[0].port = PORT_I;
	g_dipSW[0].pin  = PIN_5;
	g_dipSW[1].port = PORT_I;
	g_dipSW[1].pin  = PIN_6;
	g_dipSW[2].port = PORT_H;
	g_dipSW[2].pin  = PIN_10;
	
}
#endif

#if _NPC_VERSION_ == 3u
void NPC2_3_IOInit(void) {
	/**
	*I��ʼ��
	*/
	int i = 0;
	g_cdvI[i].port = PORT_D;
	g_cdvI[i++].pin  = PIN_6;
	g_cdvI[i].port = PORT_D;
	g_cdvI[i++].pin  = PIN_7;
	g_cdvI[i].port = PORT_D;
	g_cdvI[i++].pin  = PIN_3;
	g_cdvI[i].port = PORT_F;
	g_cdvI[i++].pin  = PIN_11;
	g_cdvI[i].port = PORT_H;
	g_cdvI[i++].pin  = PIN_3;
	g_cdvI[i].port = PORT_H;
	g_cdvI[i++].pin  = PIN_5;
	g_cdvI[i].port = PORT_H;
	g_cdvI[i++].pin  = PIN_2;
	
#if USE_COUNTER != 1u
	g_cdvI[i].port = PORT_H;
	g_cdvI[i++].pin  = PIN_4;
	g_cdvI[i].port = PORT_A;
	g_cdvI[i++].pin  = PIN_0;
	g_cdvI[i].port = PORT_C;
	g_cdvI[i++].pin  = PIN_3;
	g_cdvI[i].port = PORT_C;
	g_cdvI[i++].pin  = PIN_2;
#endif
	g_cdvI[i].port = PORT_C;
	g_cdvI[i++].pin  = PIN_0;
	g_cdvI[i].port = PORT_F;
	g_cdvI[i++].pin  = PIN_10;
	g_cdvI[i].port = PORT_F;
	g_cdvI[i++].pin  = PIN_9;
	g_cdvI[i].port = PORT_F;
	g_cdvI[i++].pin  = PIN_8;
	g_cdvI[i].port = PORT_F;
	g_cdvI[i++].pin  = PIN_7;
	g_cdvI[i].port = PORT_F;
	g_cdvI[i++].pin  = PIN_6;
	g_cdvI[i].port = PORT_B;
	g_cdvI[i++].pin  = PIN_4;
	g_cdvI[i].port = PORT_B;
	g_cdvI[i++].pin  = PIN_3;
	g_cdvI[i].port = PORT_G;
	g_cdvI[i++].pin  = PIN_15;
	
	/**
	*O��ʼ��
	*/
	i = 0;
	g_cdvO[i].port = PORT_H;
	g_cdvO[i++].pin  = PIN_13;
	g_cdvO[i].port = PORT_C;
	g_cdvO[i++].pin  = PIN_8;
	g_cdvO[i].port = PORT_G;
	g_cdvO[i++].pin  = PIN_12;
	g_cdvO[i].port = PORT_G;
	g_cdvO[i++].pin  = PIN_13;
	g_cdvO[i].port = PORT_H;
	g_cdvO[i++].pin  = PIN_15;
	g_cdvO[i].port = PORT_H;
	g_cdvO[i++].pin  = PIN_14;
	g_cdvO[i].port = PORT_A;
	g_cdvO[i++].pin  = PIN_8;
	g_cdvO[i].port = PORT_G;
	g_cdvO[i++].pin  = PIN_11;
	g_cdvO[i].port = PORT_E;
	g_cdvO[i++].pin  = PIN_4;
	g_cdvO[i].port = PORT_E;
	g_cdvO[i++].pin  = PIN_5;
	g_cdvO[i].port = PORT_E;
	g_cdvO[i++].pin  = PIN_3;
	g_cdvO[i].port = PORT_E;
	g_cdvO[i++].pin  = PIN_6;
	g_cdvO[i].port = PORT_I;
	g_cdvO[i++].pin  = PIN_7;
	g_cdvO[i].port = PORT_E;
	g_cdvO[i++].pin  = PIN_2;
	g_cdvO[i].port = PORT_B;
	g_cdvO[i++].pin  = PIN_9;
	g_cdvO[i].port = PORT_I;
	g_cdvO[i++].pin  = PIN_9;
	g_cdvO[i].port = PORT_B;
	g_cdvO[i++].pin  = PIN_8;
	g_cdvO[i].port = PORT_I;
	g_cdvO[i++].pin  = PIN_11;
	g_cdvO[i].port = PORT_I;
	g_cdvO[i++].pin  = PIN_8;
	g_cdvO[i].port = PORT_C;
	g_cdvO[i++].pin  = PIN_13;
	g_cdvO[i].port = PORT_I;
	g_cdvO[i++].pin  = PIN_10;
#if USE_EXTI_POWER_OFF == 0u
	g_cdvO[i].port = PORT_B;
	g_cdvO[i++].pin  = PIN_5;
#endif
}

void NPC2_3_SWInit(void) {
	g_dipSW[0].port = PORT_C;
	g_dipSW[0].pin  = PIN_7;
	g_dipSW[1].port = PORT_G;
	g_dipSW[1].pin  = PIN_8;
	g_dipSW[2].port = PORT_C;
	g_dipSW[2].pin  = PIN_6;
	
}
#endif

/**
  * @brief  IO��ʼ��
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
void IOInit(void) {
#if _NPC_VERSION_ == 1u
	CDV1_IOInit();
#elif _NPC_VERSION_ == 2u
	NPC2_IOInit();
#elif _NPC_VERSION_ == 3u
	NPC2_3_SWInit();
	NPC2_3_IOInit();
#endif
}
////////////////////////////////////////////////////////////////////



/**
  * @brief  I��������
  *  
  * @param  no     Ҫ���õ�I��
  *         flag   ���õĲ������
						buf    ����buf
  *   
  * @retval ��
  *
  * @note   
  */
void IParWrite(CDV_INT32U no , CDV_INT16U flag , CDV_INT08U* buf) {
	CDV_INT08U i = 0 , len = 0;
	CDV_INT16U sf   = 0x0001;
	CDV_INT32U addr = IPAR_ADDR + no * 2;
	for ( i = 0; i < I_PAR_NUM ; i++) {
		if (flag & (sf<<i)) {
			SPI_Flash_Write(buf+len, addr+i, 1);
			len ++;
		}
	}
}

/**
  * @brief  I������ȡ
  *  
  * @param  no     Ҫ���õ�I��
  *         flag   ���õĲ������
						buf    ����buf
  *   
  * @retval ��
  *
  * @note   
  */
void IParRead(CDV_INT32U no , CDV_INT16U flag , CDV_INT08U* buf , CDV_INT08U* len) {
	CDV_INT08U i = 0;
	CDV_INT16U sf   = 0x0001;
	CDV_INT32U addr = IPAR_ADDR + no * 2;
	*len = 0;
	for ( i = 0; i < I_PAR_NUM ; i++) {
		if (flag & (sf<<i)) {
			SPI_Flash_Read(buf+(*len), addr+i, 1);
			(*len)++;
		}
	}
}


/**
  * @brief  CDV ��I 30
  *  
  * @param  no     Ҫ����I��
  *   
  * @retval CDV_INT08U��1�ߵ�ƽ��0�͵�ƽ
  *
  * @note   ˢ��modbus��Ȧ
  */
CDV_INT08U ExIRead(CDV_INT32U no) {
	CDV_INT08U val;
	val = FpgaIRead(no);
	return val;
}
/**
  * @brief  CDV ��O 34
  *  
  * @param  no     Ҫ����O��
  *   
  * @retval CDV_INT08U��1�ߵ�ƽ��0�͵�ƽ
  *
  * @note   ˢ��modbus��Ȧ
  */
CDV_INT08U ExORead(CDV_INT32U no) {
	CDV_INT08U val;
	val = FpgaORead(no);
  return val;
}
/**
  * @brief  CDV ��������չO
  *  
  * @param  ��
  *   
  * @retval CDV_INT08U��1�ɹ���0ʧ��
  *
  * @note   ˢ��modbus��Ȧ
  */
CDV_INT08U ExOReadAll(void) 
{
	CDV_INT08U i,val,num;	
	CDV_INT08U pOState[FPGA_COMMAND_LEN*2];
	//val = FpgaORead(no);
	FpgaOReadAll(pOState);
	for(i=0;i<CDV_EXO_NUM*CDV_FPGA_NUM;i++)
	{
		if(i<CDV_EXO_NUM)
			num = 10-i/8;
		else
			num = 4-(i-CDV_EXO_NUM)/8;
		if(i>=CDV_EXO_NUM)
			val = (*(pOState+num)>>((i-CDV_EXO_NUM)%8))&0x01;
		else
			val = (*(pOState+num)>>(i%8))&0x01;
		switch((IO_VAL)val) 
		{
			case BIT_1:
				SET_O(CDV_O_NUM+i);
				break;
			case BIT_0:
				RESET_O(CDV_O_NUM+i);
				break;
			default:
				break;
		}
	}
  return 1;
}

#if ENABLE_FPGA_DOWN
/**
  * @brief  CDV д������չO
  *  
  * @param  ��
  *   
  * @retval CDV_INT08U��1�ɹ���0ʧ��
  *
  * @note   ˢ��modbus��Ȧ
  */
CDV_INT08U ExOWriteAll(void) 
{
	
	CDV_INT08U i,val,num;	
	CDV_INT08U pOState[FPGA_COMMAND_LEN*2];
	for(i = 0; i < 5; i++) {
	  pOState[10 - i] = g_otmp[i];
		pOState[4 - i] = g_otmp[i + 5];
	}

	
	FpgaOWriteAll(pOState);
	
	for(i=0;i<CDV_EXO_NUM*2;i++)
	{
		if(i<CDV_EXO_NUM)
			num = 10-i/8;
		else
			num = 4-(i-CDV_EXO_NUM)/8;
		if(i>=CDV_EXO_NUM)
			val = (*(pOState+num)>>((i-CDV_EXO_NUM)%8))&0x01;
		else
			val = (*(pOState+num)>>(i%8))&0x01;
		switch((IO_VAL)val) 
		{
			case BIT_1:
				SET_O(CDV_O_NUM+i);
				break;
			case BIT_0:
				RESET_O(CDV_O_NUM+i);
				break;
			default:
				break;
		}
	}
  return 1;
}
#endif
/**
  * @brief  CDV O��� 34
  *  
  * @param  no     Ҫ����I��
  *         val    CDV_INT08U��1�ߵ�ƽ��0�͵�ƽ
  *   
  * @retval ��
  *
  * @note   ˢ��modbus��Ȧ
  */
void ExOWrite(CDV_INT08U no , CDV_INT08U val) {
	FpgaOWrite(no , val);
}
/**
  * @brief  CDV ��I(����EXI
  *  
  * @param  no     Ҫ����I��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ��ȡmodbus��Ȧ������ˢ��20180104
  */
IO_VAL IRead(CDV_INT32U no) {
	CDV_INT08U val;
	
	val = READ_I(no) ? BIT_1 : BIT_0;
		
	return (IO_VAL)val;
}
/**
  * @brief  CDV ��������ͨI
  *  
  * @param  ��
  *   
  * @retval CDV_INT08U��1�ɹ���0ʧ��
  *
  * @note   ˢ��modbus��Ȧ
  */
void IReadAll(void)
{
	CDV_INT32U no = 0;
	CDV_INT08U val;
	do {
		val = GPIO_ReadInputDataBit(g_cdvI[no].port , g_cdvI[no].pin);
		
		switch((IO_VAL)val) {
			case BIT_1:
				SET_I(no);
				break;
			case BIT_0:
				RESET_I(no);
				break;
			default:
				break;
		}
		
	} while(++no < CDV_I_NUM) ;
}

/**
  * @brief  CDV ��������ͨI,���˲�
  *  
  * @param  readCnt ��ȡ����
  *         delayUs ÿ�ζ�ȡ�ļ��
  *   
  * @retval CDV_INT08U��1�ɹ���0ʧ��
  *
  * @note   ˢ��modbus��Ȧ
  */
void IReadAllFilter(int readCnt, int delayUs)
{
	CDV_INT08S val[CDV_I_NUM] = {0};
	CDV_INT08S no;
	CDV_INT32S cnt;
	
	for(cnt = 0; cnt < readCnt; cnt++) {
		
		for(no = 0; no < CDV_I_NUM;no++) {
			val[no] += GPIO_ReadInputDataBit(g_cdvI[no].port , g_cdvI[no].pin);
			
			if (cnt == readCnt - 1) {
				if (val[no] >= readCnt - 1) {
					val[no] = 1;
				} else if (val[no] <= 1) {
					val[no] = 0;
				} else {
					val[no] = -1;
				}
			}
			
		}
		
		if(cnt < readCnt - 1)
		  DelayUS(delayUs);
	}
	
	for(no = 0; no < CDV_I_NUM;no++) {
		
		if(val[no] != -1) {
		
			switch((IO_VAL)val[no]) {
				case BIT_1:
					SET_I(no);
					break;
				case BIT_0:
					RESET_I(no);
					break;
				default:
					break;
			}
		}
		
	}
}

/**
  * @brief  CDV ��������չI
  *  
  * @param  ��
  *   
  * @retval CDV_INT08U��1�ɹ���0ʧ��
  *
  * @note   ˢ��modbus��Ȧ
  */
CDV_INT08U ExIReadAll(void)
{
	CDV_INT08U i,val,num;	
	CDV_INT08U pOState[FPGA_COMMAND_LEN*2];
	//val = FpgaORead(no);
	FpgaIReadAll(pOState);
	for(i=0;i<CDV_EXI_NUM*2;i++)
	{
		if(i<CDV_EXI_NUM)
			num = 10-i/8;
		else
			num = 4-(i-CDV_EXI_NUM)/8;
		if(i>=CDV_EXI_NUM)
			val = (*(pOState+num)>>((i-CDV_EXI_NUM)%8))&0x01;
		else
			val = (*(pOState+num)>>(i%8))&0x01;
		switch((IO_VAL)val) 
		{
			case BIT_1:
				SET_I(CDV_I_NUM+i);
				break;
			case BIT_0:
				RESET_I(CDV_I_NUM+i);
				break;
			default:
				break;
		}
	}
  return 1;
}
/**
  * @brief  CDV ��I(����EXI
  *  
  * @param  no     Ҫ����I��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ��ȡ����npc�˿ڵ�bitֵ��bit1��Ӧ1��bit0��Ӧ0
  */
CDV_INT08U IBitRead(CDV_INT32U no) {
	CDV_INT08U val;
	CDV_INT16U localaddr;
//	CDV_INT16U remoteaddr;
//	CDV_INT08U host;
	IO_VAL bit;
	
	if(no < I_NUM) {
		localaddr = no;
  } else {
		CascadeModbus_MapFindInExp(0, no, &localaddr, NULL, NULL);
	}
	
	bit = IRead(localaddr);
	val = (BIT_1 == bit ? 1 : 0);
	return val;
}


/**
  * @brief  CDV ��I,����(����EXI
  *  
  * @param  no     Ҫ����I��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ˢ��modbus��Ȧ
20151225:�������ж�cdv״̬������������У����˳�while��û���ж���ͣ
  */
IO_VAL ShakeIRead(CDV_INT32U no) {
	CDV_INT08U cnt = 0,freq = 0, sleep = 0 ,len;
	IO_VAL bit , bitLast;
	IParRead(no , 0x0001 , &freq , &len);
	if(1!=len)
		WhileError();
	IParRead(no , 0x0002 , &sleep , &len);
	if(1!=len)
		WhileError();

	do {
//		if(no >= CDV_IO_NUM) {
//		  bit = ExIRead(no - CDV_IO_NUM);
//		} else {
//			bit = GPIO_ReadInputDataBit(g_cdvI[no].port , g_cdvI[no].pin) ? BIT_1 : BIT_0;
//		}	
		bit = IRead(no);
		
		if (cnt > 0) {//�ж�ǰ������ǲ�����ȵ�
			if (bit != bitLast) {
				cnt = 0;
			}
		}
		
		bitLast = bit;
		TimeDelay(0 , sleep);
	} while (++cnt < freq && ( CDV_STOP != g_cdvStat));
	
	return bit;
}

/**
  * @brief  CDV ��O������EXO
  *  
  * @param  no     Ҫ����O��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ��ȡmodbus��Ȧ������ˢ��20180104
  */
IO_VAL ORead(CDV_INT32U no) {
	CDV_INT08U val;
	
	val = READ_O(no) ? BIT_1 : BIT_0;
		
	return (IO_VAL)val;
}
/**
  * @brief  CDV ��������ͨO
  *  
  * @param  ��
  *   
  * @retval CDV_INT08U��1�ɹ���0ʧ��
  *
  * @note   ˢ��modbus��Ȧ
  */
void OReadAll(void)
{
	CDV_INT32U no = 0;
	CDV_INT08U val;
	do {
		val = GPIO_ReadOutputDataBit(g_cdvO[no].port , g_cdvO[no].pin);
		
		switch((IO_VAL)val) {
			case BIT_1:
				SET_O(no);
				break;
			case BIT_0:
				RESET_O(no);
				break;
			default:
				break;
		}
		
	} while(++no < CDV_O_NUM) ;
}
/**
  * @brief  CDV д������ͨO
  *  
  * @param  ��
  *   
  * @retval 
  *
  * @note   ˢ��modbus��Ȧ��O��
  */
void OWriteAll(void)
{
	CDV_INT32U no = 0;
	IO_VAL io_val;
	do {
		io_val = READ_O(no) ? BIT_1 : BIT_0;
		//OWrite(no , io_val);
		GPIO_WriteBit(g_cdvO[no].port , g_cdvO[no].pin, (CDV_INT08U)io_val);
	} while(++no < CDV_O_NUM) ;
}
/**
  * @brief  CDV ��O������EXO
  *  
  * @param  no     Ҫ����O��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ��ȡ����npc�˿ڵ�bitֵ��bit1��Ӧ1��bit0��Ӧ0
  */
CDV_INT08U OBitRead(CDV_INT32U no) {
	CDV_INT08U val;
	CDV_INT16U localaddr = no;
//	CDV_INT16U remoteaddr;
//	CDV_INT08U host;
	IO_VAL bit;
	
	if(no < O_NUM) {
		//localaddr = no;
  } else {
		CascadeModbus_MapFindInExp(1, no, &localaddr, NULL, NULL);
	}
	
	bit = ORead(localaddr);
	val = (BIT_1 == bit ? 1 : 0);
	return val;
}
/**
  * @brief  CDV O���(����EXO
  *  
  * @param  no     Ҫ����I��
  *         val    IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *   
  * @retval ��
  *
  * @note   ˢ��modbus��Ȧ
  */
void OWrite(CDV_INT32U no , IO_VAL ioVal) {
	
	CDV_INT08U val = ioVal;

	switch(ioVal){
		case BIT_1:
			SET_O(no);
			break;
		case BIT_0:
			RESET_O(no);
			break;
		default:
			break;
	}
#if !USE_OVERLAP
	if(no < CDV_O_NUM) {
		GPIO_WriteBit(g_cdvO[no].port , g_cdvO[no].pin, val);
	}
#endif
}
//����ĺ���������CDV_O_NUM���ϵ�O�޷�Ӧ����Ϊ���ǵ���ĥ����Ҫ�õ�O����Ϊ���ʹ�ã����������������100���O�����Կ��Ա����������������Ҫ�ſ�����������
//void OWrite(CDV_INT32U no , IO_VAL ioVal) {
//	
//	CDV_INT08U val = ioVal;
//	
//	
//	if(no >= CDV_O_NUM) {
//#if ENABLE_FPGA_DOWN
//		if(val) {
//			SET_COIL(g_otmp, no - CDV_O_NUM)
//		} else {
//			RESET_COIL(g_otmp, no - CDV_O_NUM);
//		}//ExOWrite(no - CDV_O_NUM, val);
//#endif
//	} else {
//		switch(ioVal){
//			case BIT_1:
//				SET_O(no);
//				break;
//			case BIT_0:
//				RESET_O(no);
//				break;
//			default:
//				break;
//		}
//		GPIO_WriteBit(g_cdvO[no].port , g_cdvO[no].pin, val);
//	}
//}
///**
//  * @brief  CDV �����뿪��
//  *  
//  * @param  no     Ҫ����DIP��
//  *   
//  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
//  *
//  * @note   ˢ��modbus��Ȧ
//  */
//IO_VAL DIPRead(CDV_INT08U no) {
//	CDV_INT08U val;
//	val = GPIO_ReadInputDataBit(g_dipSW[no].port , g_dipSW[no].pin);
//	if(val) {
//	  //	  MODBUS_SET_DIP(no);
//		return BIT_1;
//	}	else {
//	  //	  MODBUS_RESET_DIP(no);
//		return BIT_0;
//	}		
//}


////////////////////////////////////////////////////////////////////

/**
  * @brief  I�ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��
            OPT_SUCCESS����ִ�����
            OPT_FAILURE����δִ����ɣ�����ȴ�δ�ȵ�
  *
  * @note   ��Դ�ţ�00-FF��	00�ȴ�	ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	�ַ���
  */

RET_STATUS ICmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg) { 
	OS_ERR err;
	CDV_INT08U opt, type;
	CDV_INT32U no , num , val;
	IO_VAL bit;
  RET_STATUS ret =OPT_SUCCESS;
	
	CDV_INT08U tmpPar[I_PAR_NUM] = {0} , tmpLen = 0;
//  if(1 == debug) {
//		return ret;
//	}
//	//strLen = rxLen - 3;//���ĳ���
 // OS_ERR err;

	no  = *(CDV_INT32U*)(rxBuf);//rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	num  = *(CDV_INT32U*)(rxBuf + 6);

	switch(opt) {
		case 0x00:/*�ȴ�*/
			  num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
		    bit = (num)?BIT_1:BIT_0;
		    if(bit != IRead(no))
				  ret = OPT_FAILURE;
			break;
		case 0x01:/*�����ȴ�*/
		    bit = (num)?BIT_1:BIT_0;
		    if(bit != ShakeIRead(no))
				ret = OPT_FAILURE;
			break;
		case 0x02:/*��ѯ*/
			bit = IRead(no);
		  val = (BIT_1 == bit ? 1 : 0);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
		  break;
		case 0x03:/*������ѯ*/
			bit = ShakeIRead(no);
			val = (BIT_1 == bit ? 1 : 0);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
			break;
		case 0x04:/*��ȡ������*/
			if(0xFE == type) {
				bit = IRead(no);
			  val = (BIT_1 == bit ? 1 : 0);
				ResVarSet(num, val, arg);
			}
			break;
		case 0x05:/*��������*/
			if(0xFF == type) {
			  IParWrite(no , num , rxBuf + 10);
			}
			break;
		case 0x06:/*������ȡ*/			
			if(0xFF == type) {
			  IParRead(no , num , tmpPar , &tmpLen);
			  ResRequest(arg->buf, arg->len, tmpPar, tmpLen, arg, RC_CRC);
			}
			break;
		case 0x07:/*����ȴ�*/
			OSMutexPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
			ret = OPT_FAILURE;
			bit = READ_PLUSE_I(no);
			if(bit != IRead(no))
			{
				//delay_ms(5);
				if(bit != IRead(no))
				{
					if(BIT_0 == bit)
					{
						SET_PLUSE_I(no);
					}
					else
					{
						RESET_PLUSE_I(no);
					}
					ret = OPT_SUCCESS;
				}
			}
			OSMutexPost (&PLUSE_SEM,OS_OPT_POST_NO_SCHED,&err);
// 		    bit = IRead(no);
// 		    while(bit == IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
			break;
		case 0x08:/*������ȴ�*/
			OSMutexPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
			ret = OPT_FAILURE;
			bit = READ_PLUSE_I(no);
			if(bit != IRead(no))
			{
				//delay_ms(5);
				if(bit != IRead(no))
				{
					if(BIT_0 == bit)
					{
						ret = OPT_SUCCESS;
						SET_PLUSE_I(no);
					}
					else
					{
						RESET_PLUSE_I(no);
					}
				}
			}
			OSMutexPost (&PLUSE_SEM,OS_OPT_POST_NO_SCHED,&err);
				
// 		    while(BIT_0 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
// 		    while(BIT_1 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
			break;
		case 0x09:/*������ȴ�*/
			OSMutexPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
			ret = OPT_FAILURE;
			bit = READ_PLUSE_I(no);
			if(bit != IRead(no))
			{
				//delay_ms(5);
				if(bit != IRead(no))
				{
					if(BIT_0 == bit)
					{
						SET_PLUSE_I(no);
					}
					else
					{
						ret = OPT_SUCCESS;
						RESET_PLUSE_I(no);
					}
				}
			}
			OSMutexPost (&PLUSE_SEM,OS_OPT_POST_NO_SCHED,&err);
// 		    while(BIT_1 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
// 		    while(BIT_0 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
			break;
		case 0x0A:/*ȥ����ȡ������*/
			if(0xFE == type) {
				bit = ShakeIRead(no);
			  val = (BIT_1 == bit ? 1 : 0);
				//ValSet(num, ShakeIRead(no));
				ResVarSet(num, val, arg);
			}
			break;
		case 0xFF:/*��ѯ*/
//			bit = IRead(no);
//		  val = (BIT_1 == bit ? 1 : 0);
		  val = IBitRead(no);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
		  break;
		default:
			break;
	}

	return ret;
}

/**
  * @brief  O�ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   ��Դ�ţ�00-FF��	00���	ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	�ַ���
		                                FE	�������Ÿ�λ	�������ŵ�λ							

  */

RET_STATUS OCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg) { 
//	OS_ERR err;
  //CDV_INT08U no , opt, type, val  /*, strLen*/;
	CDV_INT08U opt, type;
	CDV_INT32U num , no, val;
	IO_VAL bit;
	//OS_ERR err;

//	//strLen = rxLen - 3;//���ĳ���
//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];	
	no  = *(CDV_INT32U*)(rxBuf);//rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	num  = *(CDV_INT32U*)(rxBuf + 6);

	switch(opt) {
		case 0x00:/*���*/			
//			switch(type) {				
//				case 0xFE:
//					bit = (VarGet(num))?BIT_1:BIT_0;
//					OWrite(no,bit);
//					break;
//				default:
//					bit = (num)?BIT_1:BIT_0;
//					OWrite(no, bit);
//					break;
//			}
			bit = (ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg)) ? BIT_1 : BIT_0;
			OWrite(no, bit);
		
		// TEST ��¼O2������λ�����
		if((no == 0 || no == 1) && bit == BIT_0)
		  time_log_anything(
				-1*((DEBUG_SCRIPT*)(arg->ptrWorker))->which,
		    -1*((DEBUG_SCRIPT*)(arg->ptrWorker))->doNo
		    );
		
			break;
		case 0x01:/*�ȴ�*/
			num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
		  bit = (num) ? BIT_1 : BIT_0;
		  if(bit != ORead(no))
				return OPT_FAILURE;
		  break;
		case 0x04:/*��ѯ*/
			bit = ORead(no);
		  val = (BIT_1 == bit ? 1 : 0);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
		  break;
		case 0x02:/*��ȡ������*/
			if(0xFE == type) {
				bit = ORead(no);
		    val = (BIT_1 == bit ? 1 : 0);
			  //DW_CDV_VAR(rxBuf[3]) = ORead(no);
				//ValSet(num, ORead(no));
				ResVarSet(num, val, arg);
			}
			break;
		case 0xFF:/*��ѯ*/
//			bit = ORead(no);
//		  val = (BIT_1 == bit ? 1 : 0);
		  val = OBitRead(no);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
		  break;
		default:
			break;
		
	}

	return OPT_SUCCESS;
}

//********************************************************
//����I��״̬��ȡ
//num:�ڼ���I�ڣ�1-20
//********************************************************
u32 Read_Input_All()
{
	#if 0
	u32  s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s20;
	u32 state=0xffffff;

	s1=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9); 
	s2=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8); 
	s3=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10); 
	s4=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7); 
	s5=GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_3); 
	s6=GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_5); 
	s7=GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_2); 
	s8=GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_4); 
	s9=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0); 
	s10=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3); 
	s11=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2); 
	s12=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0); 
	s13=GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10); 
	s14=GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9); 
	s15=GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8); 
	s16=GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7); 
	s17=GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6); 
	s18=GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5); 
	s19=GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4); 
	s20=GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3); 
	state = (s1<<23)|(s2<<22)|(s3<<21)|(s4<<20)|(s5<<19)|(s6<<18)|(s7<<17)
				|(s8<<16)|(s9<<15)|(s10<<14)|(s11<<13)|(s12<<12)|(s13<<11)
				|(s14<<10)|(s15<<9)|(s16<<8)|(s17<<7)|(s18<<6)
				|(s19<<5)|(s20<<4);	
	state =  state|0x00000f;	
	#else
	u32 s[20/*CDV_I_NUM*/];
	u32 state = 0xffffff;
	int i;
	for(i=0;i<CDV_I_NUM;i++)
	{
		s[i] = GPIO_ReadInputDataBit(g_cdvI[i].port,g_cdvI[i].pin);
	}
	state = (s[0]<<23)|(s[1]<<22)|(s[2]<<21)|(s[3]<<20)|(s[4]<<19)|(s[5]<<18)|(s[6]<<17)
			|(s[7]<<16)|(s[8]<<15)|(s[9]<<14)|(s[10]<<13)|(s[11]<<12)|(s[12]<<11)
			|(s[13]<<10)|(s[14]<<9)|(s[15]<<8)|(s[16]<<7)|(s[17]<<6)
			|(s[18]<<5)|(s[19]<<4);
	state = state|0x00000f;	
	#endif 

return state;
}

//********************************************************
//����O��״̬��ȡ
//num:�ڼ���I�ڣ�1-22
//********************************************************
u32 Read_Output_ALL()
{
	#if 0
	u32  s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s20,s21,s22;
	u32 state = 0xffffff;

//	s1=GPIO_ReadOutputDataBit(GPIOH,GPIO_Pin_13);
//	s2=GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_12);
//	s3=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_1);
//	s4=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_4);
//	s5=GPIO_ReadOutputDataBit(GPIOH,GPIO_Pin_15);
//	s6=GPIO_ReadOutputDataBit(GPIOH,GPIO_Pin_14);
//	s7=GPIO_ReadOutputDataBit(GPIOG,GPIO_Pin_15);
//	s8=GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_0);
//	s9=GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_3);
//	s10=GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_5);
//	s11=GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_4);
//	s12=GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_6);
//	s13=GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_0);
//	s14=GPIO_ReadOutputDataBit(GPIOI,GPIO_Pin_7);
//	s15=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9);
//	s16=GPIO_ReadOutputDataBit(GPIOI,GPIO_Pin_9);
//	s17=GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_2);
//	s18=GPIO_ReadOutputDataBit(GPIOI,GPIO_Pin_11);
//	s19=GPIO_ReadOutputDataBit(GPIOI,GPIO_Pin_8);
//	s20=GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13);
//	s21=GPIO_ReadOutputDataBit(GPIOI,GPIO_Pin_10);
//	s22=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7);
	state = (s1<<23)|(s2<<22)|(s3<<21)|(s4<<20)|(s5<<19)|(s6<<18)|(s7<<17)
				|(s8<<16)|(s9<<15)|(s10<<14)|(s11<<13)|(s12<<12)|(s13<<11)
				|(s14<<10)|(s15<<9)|(s16<<8)|(s17<<7)|(s18<<6)
				|(s19<<5)|(s20<<4)|(s21<<3)|(s22<<2);
	state = state|Alarm_flag(Alarmflag);
	#else
	u32 s[22/*CDV_O_NUM*/];
	u32 state = 0xffffff;
	int i;
	for(i=0;i<CDV_O_NUM;i++)
	{
		s[i] = GPIO_ReadOutputDataBit(g_cdvO[i].port,g_cdvO[i].pin);
	}
	state = (s[0]<<23)|(s[1]<<22)|(s[2]<<21)|(s[3]<<20)|(s[4]<<19)|(s[5]<<18)|(s[6]<<17)
			|(s[7]<<16)|(s[8]<<15)|(s[9]<<14)|(s[10]<<13)|(s[11]<<12)|(s[12]<<11)
			|(s[13]<<10)|(s[14]<<9)|(s[15]<<8)|(s[16]<<7)|(s[17]<<6)
			|(s[18]<<5)|(s[19]<<4)|(s[20]<<3)|(s[21]<<2);
	state = state|Alarm_flag(Alarmflag);
	#endif 
	return state;
}
//********************************************************
//��չIO��ʼ��
//********************************************************
void ExIOInit(void)
{
#if ENABLE_FPGA_DOWN
	CDV_INT08U i;
	CDV_INT32U ExIO_Enable[CDV_FPGA_NUM];
	for(i=0;i<CDV_FPGA_NUM;i++)
	{
		ExIO_Enable[i] = Read_Fpga_Regist(0x8FFF,i);
	}
	if(ExIO_Enable[0]==0x11223344&&ExIO_Enable[1]==0x11223344)//��չIO
	{
		FPGA_Mode = FPGA_IO_CODE;
		CDV_EXI_NUM = 34;
		CDV_EXO_NUM = 30;
	}
	else if(ExIO_Enable[0]==0x22334455&&ExIO_Enable[1]==0x22334455)	//������򷵻�0x22334455
	{		
		FPGA_Mode = FPGA_MOTOR_CODE;
		CDV_EXI_NUM = 14;
		CDV_EXO_NUM = 18;
	}
	else
	{
		FPGA_Mode = FPGA_NONE;
		CDV_EXI_NUM = 0;
		CDV_EXO_NUM = 0;
	}		
	for(i = CDV_O_NUM ; i < CDV_O_NUM + CDV_EXO_NUM*CDV_FPGA_NUM ; i ++) 
	{
		OWrite(i , BIT_0);
		//IRead(i);
	}
	ExOWriteAll();
#endif
}
