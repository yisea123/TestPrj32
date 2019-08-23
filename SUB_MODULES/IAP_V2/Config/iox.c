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
	
	#include "iox.h"

CDV_IO g_cdvI[CDV_I_NUM];
CDV_IO g_cdvO[CDV_O_NUM];
CDV_IO g_dipSW[CDV_DIP_NUM];
CDV_INT08U g_dip = 0;

CDV_INT08U I[3], O[3];

CDV_INT08U* g_i = I;
CDV_INT08U* g_o = O;

/*GPIO设置*/
void CDV1_GpioConfig(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
						|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF
						|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI, ENABLE);//使能相关时钟 
#if defined(CDV_V1)
	//设置输入输出口
  //PA  13/14 swio/swclk
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB   2为boot1  6、7     10、11 是串口
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
	//PC    10、11 12串口           14、15晶振
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
	//PD   2  5、6为串口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_7| GPIO_Pin_9| GPIO_Pin_11|GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//PE	A1 A2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8 |GPIO_Pin_9|  GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	//PF        无串口 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_3 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	//PG    9、14串口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10| GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	//PH       0、1晶振   4、5为I2C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	//PI   0、1、2、3SPI串口
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOI, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOI, &GPIO_InitStructure);	
#else
//elif defined(CDV_V2)
	//设置输入输出口
    //PA  13/14 swio/swclk
	#ifdef CDV_V2_2 
	  //GPIO_Pin_0为ADC1接口
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2 ;
	#else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2 ;
	#endif	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	#ifdef CDV_V2_2 
	  //GPIO_Pin_4 | GPIO_Pin_5为DAC接口接口
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12|GPIO_Pin_15;
	#else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12|GPIO_Pin_15;
  #endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//PB   2为boot1  6、7     10、11 是串口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  #ifdef CDV_V2_2 
	  //GPIO_Pin_3 为SPI1 SLCK通信接口
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_15 ;
	#else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_15 ;
  #endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	//PC    10、11 12串口           14、15晶振
	#ifdef CDV_V2_2 
	  //GPIO_Pin_0为ADC2接口
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
	
	//PD   2  5、6为串口
	#ifdef CDV_V2_2
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12|GPIO_Pin_14 | GPIO_Pin_15;
	#else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_10 | GPIO_Pin_12 |  GPIO_Pin_14 | GPIO_Pin_15;
	#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	#ifdef CDV_V2_2
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_7| GPIO_Pin_9| GPIO_Pin_11|GPIO_Pin_13 ;
	#else
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_7| GPIO_Pin_9| GPIO_Pin_11|GPIO_Pin_13;
	#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	//PE	A1 A2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8 |GPIO_Pin_9|  GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	//PF        无串口
	//GPIO_Pin_10为ADC3接口
	#ifdef CDV_V2_2 
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	#else
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
	#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	//PG    9、14串口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	//PH       0、1晶振   4、5为I2C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12  | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	//PI   0、1、2、3SPI串口
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOI, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//设置上拉下拉寄存器
	GPIO_Init(GPIOI, &GPIO_InitStructure);	
#endif
/***************************************************不华丽Dividing line*****************************************************************/	

}
void NPC2_GpioConfig(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
						|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF
						|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI, ENABLE);//使能相关时钟 
	/**
	 *设置输入口
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
	 *设置输出口
	 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//PH 13 14 15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15 ;
	GPIO_Init(GPIOH, &GPIO_InitStructure);	
	//PA 12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
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
	//PB 6 RED LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PG 4 RUN LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void NPC2_3_DipConfig(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOG, ENABLE);//使能相关时钟 

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
						|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI, ENABLE);//使能相关时钟 
	/**
	 *设置输入口
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
	 *设置输出口
	 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5
															| GPIO_Pin_8 
															| GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 
															| GPIO_Pin_13;
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
															| GPIO_Pin_15;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7
															| GPIO_Pin_8
															| GPIO_Pin_9
															| GPIO_Pin_10
															| GPIO_Pin_11;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
}

void GPIO_Configuration(void){
//	NPC2_GpioConfig();
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
	/*IO初始化*/
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
	/*拨码开关*/
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
	/*IO初始化*/
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
	/*拨码开关*/
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
void NPC2_IOInit(void) {
	/**
	*O初始化
	*/
	g_cdvO[0].port = PORT_H;
	g_cdvO[0].pin  = PIN_13;
	g_cdvO[1].port = PORT_A;
	g_cdvO[1].pin  = PIN_12;
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
	*I初始化
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
	*拨码开关
	*/
	g_dipSW[0].port = PORT_I;
	g_dipSW[0].pin  = PIN_5;
	g_dipSW[1].port = PORT_I;
	g_dipSW[1].pin  = PIN_6;
	g_dipSW[2].port = PORT_H;
	g_dipSW[2].pin  = PIN_10;
	
}


void NPC2_3_IOInit(void) {
	/**
	*I初始化
	*/
	g_cdvI[0].port = PORT_D;
	g_cdvI[0].pin  = PIN_6;
	g_cdvI[1].port = PORT_D;
	g_cdvI[1].pin  = PIN_7;
	g_cdvI[2].port = PORT_D;
	g_cdvI[2].pin  = PIN_3;
	g_cdvI[3].port = PORT_F;
	g_cdvI[3].pin  = PIN_11;
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
	g_cdvI[17].port = PORT_B;
	g_cdvI[17].pin  = PIN_4;
	g_cdvI[18].port = PORT_B;
	g_cdvI[18].pin  = PIN_3;
	g_cdvI[19].port = PORT_G;
	g_cdvI[19].pin  = PIN_15;
	
	/**
	*O初始化
	*/
	g_cdvO[0].port = PORT_H;
	g_cdvO[0].pin  = PIN_13;
	g_cdvO[1].port = PORT_C;
	g_cdvO[1].pin  = PIN_8;
	g_cdvO[2].port = PORT_G;
	g_cdvO[2].pin  = PIN_12;
	g_cdvO[3].port = PORT_G;
	g_cdvO[3].pin  = PIN_13;
	g_cdvO[4].port = PORT_H;
	g_cdvO[4].pin  = PIN_15;
	g_cdvO[5].port = PORT_H;
	g_cdvO[5].pin  = PIN_14;
	g_cdvO[6].port = PORT_A;
	g_cdvO[6].pin  = PIN_8;
	g_cdvO[7].port = PORT_G;
	g_cdvO[7].pin  = PIN_11;
	g_cdvO[8].port = PORT_E;
	g_cdvO[8].pin  = PIN_4;
	g_cdvO[9].port = PORT_E;
	g_cdvO[9].pin  = PIN_5;
	g_cdvO[10].port = PORT_E;
	g_cdvO[10].pin  = PIN_3;
	g_cdvO[11].port = PORT_E;
	g_cdvO[11].pin  = PIN_6;
	g_cdvO[12].port = PORT_I;
	g_cdvO[12].pin  = PIN_7;
	g_cdvO[13].port = PORT_E;
	g_cdvO[13].pin  = PIN_2;
	g_cdvO[14].port = PORT_B;
	g_cdvO[14].pin  = PIN_9;
	g_cdvO[15].port = PORT_I;
	g_cdvO[15].pin  = PIN_9;
	g_cdvO[16].port = PORT_B;
	g_cdvO[16].pin  = PIN_8;
	g_cdvO[17].port = PORT_I;
	g_cdvO[17].pin  = PIN_11;
	g_cdvO[18].port = PORT_I;
	g_cdvO[18].pin  = PIN_8;
	g_cdvO[19].port = PORT_C;
	g_cdvO[19].pin  = PIN_13;
	g_cdvO[20].port = PORT_I;
	g_cdvO[20].pin  = PIN_10;
	g_cdvO[21].port = PORT_B;
	g_cdvO[21].pin  = PIN_5;
}

void NPC2_3_SWInit(void) {
	g_dipSW[0].port = PORT_C;
	g_dipSW[0].pin  = PIN_7;
	g_dipSW[1].port = PORT_G;
	g_dipSW[1].pin  = PIN_8;
	g_dipSW[2].port = PORT_C;
	g_dipSW[2].pin  = PIN_6;
	
}




/**
  * @brief  IO初始化
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
  * @brief  CDV 读I(包含EXI
  *  
  * @param  no     要读的I号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   刷新modbus线圈
  */
IO_VAL IRead(CDV_INT32U no) {
	CDV_INT08U val;
	if(no >= CDV_I_NUM) {
	//	val = ExIRead(no - CDV_I_NUM);
	} else {
		val = GPIO_ReadInputDataBit(g_cdvI[no].port , g_cdvI[no].pin);
	}	
	
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
		
	return (IO_VAL)val;
}


/**
  * @brief  CDV 读O（包含EXO
  *  
  * @param  no     要读的O号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   刷新modbus线圈
  */
IO_VAL ORead(CDV_INT32U no) {
	CDV_INT08U val;
	if(no >= CDV_O_NUM) {
		//val = ExORead(no - CDV_O_NUM);
	} else {
		val = GPIO_ReadInputDataBit(g_cdvO[no].port , g_cdvO[no].pin);
	}
	
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
		
	return (IO_VAL)val;
}
/**
  * @brief  CDV O输出(包含EXO
  *  
  * @param  no     要读的I号
  *         val    IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *   
  * @retval 无
  *
  * @note   刷新modbus线圈
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
	
	if(no >= CDV_O_NUM) {
		//ExOWrite(no - CDV_O_NUM, val);
	} else {
		GPIO_WriteBit(g_cdvO[no].port , g_cdvO[no].pin, val);
	}
}
/** @brief  LED初始化
  * @param 
  * @retval 
  * @note   
  */
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
