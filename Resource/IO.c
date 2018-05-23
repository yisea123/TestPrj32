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
///*内部函数*/
//void IParWrite(CDV_INT32U no , CDV_INT16U flag , CDV_INT08U* buf);
//void IParRead(CDV_INT32U no , CDV_INT16U flag , CDV_INT08U* buf , CDV_INT08U* len);

OS_SEM PLUSE_SEM;
CDV_INT08U g_pluse[COIL_CHN];

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
#if _NPC_VERSION_ == 1u
	CDV1_GpioConfig();
#elif _NPC_VERSION_ == 2u
	NPC2_GpioConfig();
#elif _NPC_VERSION_ == 3u
	NPC2_3_DipConfig();
	NPC2_3_GpioConfig();
#endif
}


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

void NPC2_IOInit(void) {
	/**
	*O初始化
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
  * @brief  I参数设置
  *  
  * @param  no     要设置的I号
  *         flag   设置的参数标记
						buf    参数buf
  *   
  * @retval 无
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
  * @brief  I参数读取
  *  
  * @param  no     要设置的I号
  *         flag   设置的参数标记
						buf    参数buf
  *   
  * @retval 无
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
  * @brief  CDV 读I 30
  *  
  * @param  no     要读的I号
  *   
  * @retval CDV_INT08U：1高电平、0低电平
  *
  * @note   刷新modbus线圈
  */
CDV_INT08U ExIRead(CDV_INT32U no) {
	CDV_INT08U val;
	val = FpgaIRead(no);
	return val;
}
/**
  * @brief  CDV 读O 34
  *  
  * @param  no     要读的O号
  *   
  * @retval CDV_INT08U：1高电平、0低电平
  *
  * @note   刷新modbus线圈
  */
CDV_INT08U ExORead(CDV_INT32U no) {
	CDV_INT08U val;
	val = FpgaORead(no);
  return val;
}
/**
  * @brief  CDV 读所有扩展O
  *  
  * @param  无
  *   
  * @retval CDV_INT08U：1成功、0失败
  *
  * @note   刷新modbus线圈
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

/**
  * @brief  CDV 写所有扩展O
  *  
  * @param  无
  *   
  * @retval CDV_INT08U：1成功、0失败
  *
  * @note   刷新modbus线圈
  */
CDV_INT08U ExOWriteAll(void) 
{
	
#if ENABLE_FPGA_DOWN
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
#endif
}
/**
  * @brief  CDV O输出 34
  *  
  * @param  no     要读的I号
  *         val    CDV_INT08U：1高电平、0低电平
  *   
  * @retval 无
  *
  * @note   刷新modbus线圈
  */
void ExOWrite(CDV_INT08U no , CDV_INT08U val) {
	FpgaOWrite(no , val);
}
/**
  * @brief  CDV 读I(包含EXI
  *  
  * @param  no     要读的I号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   读取modbus线圈，但不刷新20180104
  */
IO_VAL IRead(CDV_INT32U no) {
	CDV_INT08U val;
	
	val = READ_I(no) ? BIT_1 : BIT_0;
		
	return (IO_VAL)val;
}
/**
  * @brief  CDV 读所有普通I
  *  
  * @param  无
  *   
  * @retval CDV_INT08U：1成功、0失败
  *
  * @note   刷新modbus线圈
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
  * @brief  CDV 读所有扩展I
  *  
  * @param  无
  *   
  * @retval CDV_INT08U：1成功、0失败
  *
  * @note   刷新modbus线圈
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
  * @brief  CDV 读I(包含EXI
  *  
  * @param  no     要读的I号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   读取的是npc端口的bit值，bit1对应1，bit0对应0
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
  * @brief  CDV 读I,防抖(包含EXI
  *  
  * @param  no     要读的I号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   刷新modbus线圈
20151225:增加了判断cdv状态，如果不是运行，就退出while，没有判断暂停
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
		
		if (cnt > 0) {//判断前后读的是不是相等的
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
  * @brief  CDV 读O（包含EXO
  *  
  * @param  no     要读的O号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   读取modbus线圈，但不刷新20180104
  */
IO_VAL ORead(CDV_INT32U no) {
	CDV_INT08U val;
	
	val = READ_O(no) ? BIT_1 : BIT_0;
		
	return (IO_VAL)val;
}
/**
  * @brief  CDV 读所有普通O
  *  
  * @param  无
  *   
  * @retval CDV_INT08U：1成功、0失败
  *
  * @note   刷新modbus线圈
  */
void OReadAll(void)
{
	CDV_INT32U no = 0;
	CDV_INT08U val;
	do {
		val = GPIO_ReadInputDataBit(g_cdvO[no].port , g_cdvO[no].pin);
		
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
  * @brief  CDV 写所有普通O
  *  
  * @param  无
  *   
  * @retval 
  *
  * @note   刷新modbus线圈到O口
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
  * @brief  CDV 读O（包含EXO
  *  
  * @param  no     要读的O号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   读取的是npc端口的bit值，bit1对应1，bit0对应0
  */
CDV_INT08U OBitRead(CDV_INT32U no) {
	CDV_INT08U val;
	CDV_INT16U localaddr;
//	CDV_INT16U remoteaddr;
//	CDV_INT08U host;
	IO_VAL bit;
	
	if(no < O_NUM) {
		localaddr = no;
  } else {
		CascadeModbus_MapFindInExp(1, no, &localaddr, NULL, NULL);
	}
	
	bit = ORead(localaddr);
	val = (BIT_1 == bit ? 1 : 0);
	return val;
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
	
	
	if(no >= CDV_O_NUM) {
#if ENABLE_FPGA_DOWN
		if(val) {
			SET_COIL(g_otmp, no - CDV_O_NUM)
		} else {
			RESET_COIL(g_otmp, no - CDV_O_NUM);
		}//ExOWrite(no - CDV_O_NUM, val);
#endif
	} else {
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
		GPIO_WriteBit(g_cdvO[no].port , g_cdvO[no].pin, val);
	}
}
///**
//  * @brief  CDV 读拨码开关
//  *  
//  * @param  no     要读的DIP号
//  *   
//  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
//  *
//  * @note   刷新modbus线圈
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
  * @brief  I脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明
            OPT_SUCCESS命令执行完成
            OPT_FAILURE命令未执行完成，比如等待未等到
  *
  * @note   资源号（00-FF）	00等待	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
  */

RET_STATUS ICmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) { 
	OS_ERR err;
	CDV_INT08U opt, type;
	CDV_INT32U no , num , val;
	IO_VAL bit;
  RET_STATUS ret =OPT_SUCCESS;
	
	CDV_INT08U tmpPar[I_PAR_NUM] = {0} , tmpLen = 0;
//  if(1 == debug) {
//		return ret;
//	}
//	//strLen = rxLen - 3;//中文长度
 // OS_ERR err;

	no  = *(CDV_INT32U*)(rxBuf);//rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	num  = *(CDV_INT32U*)(rxBuf + 6);

	switch(opt) {
		case 0x00:/*等待*/
			  num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
		    bit = (num)?BIT_1:BIT_0;
		    if(bit != IRead(no))
				  ret = OPT_FAILURE;
			break;
		case 0x01:/*抖动等待*/
		    bit = (num)?BIT_1:BIT_0;
		    if(bit != ShakeIRead(no))
				ret = OPT_FAILURE;
			break;
		case 0x02:/*查询*/
			bit = IRead(no);
		  val = (BIT_1 == bit ? 1 : 0);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
		  break;
		case 0x03:/*抖动查询*/
			bit = ShakeIRead(no);
			val = (BIT_1 == bit ? 1 : 0);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
			break;
		case 0x04:/*读取到变量*/
			if(0xFE == type) {
				bit = IRead(no);
			  val = (BIT_1 == bit ? 1 : 0);
				ResVarSet(num, val, arg);
			}
			break;
		case 0x05:/*参数设置*/
			if(0xFF == type) {
			  IParWrite(no , num , rxBuf + 10);
			}
			break;
		case 0x06:/*参数读取*/			
			if(0xFF == type) {
			  IParRead(no , num , tmpPar , &tmpLen);
			  ResRequest(arg->buf, arg->len, tmpPar, tmpLen, arg, RC_CRC);
			}
			break;
		case 0x07:/*跳变等待*/
			OSSemPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
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
			OSSemPost (&PLUSE_SEM,OS_OPT_POST_1,&err);
// 		    bit = IRead(no);
// 		    while(bit == IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
			break;
		case 0x08:/*上跳变等待*/
			OSSemPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
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
			OSSemPost (&PLUSE_SEM,OS_OPT_POST_1,&err);
				
// 		    while(BIT_0 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
// 		    while(BIT_1 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
			break;
		case 0x09:/*下跳变等待*/
			OSSemPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
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
			OSSemPost (&PLUSE_SEM,OS_OPT_POST_1,&err);
// 		    while(BIT_1 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
// 		    while(BIT_0 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
			break;
		case 0x0A:/*去抖读取到变量*/
			if(0xFE == type) {
				bit = ShakeIRead(no);
			  val = (BIT_1 == bit ? 1 : 0);
				//ValSet(num, ShakeIRead(no));
				ResVarSet(num, val, arg);
			}
			break;
		case 0xFF:/*查询*/
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
  * @brief  O脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   资源号（00-FF）	00输出	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
		                                FE	变量代号高位	变量代号低位							

  */

RET_STATUS OCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) { 
//	OS_ERR err;
  //CDV_INT08U no , opt, type, val  /*, strLen*/;
	CDV_INT08U opt, type;
	CDV_INT32U num , no, val;
	IO_VAL bit;
	//OS_ERR err;

//	//strLen = rxLen - 3;//中文长度
//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];	
	no  = *(CDV_INT32U*)(rxBuf);//rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	num  = *(CDV_INT32U*)(rxBuf + 6);

	switch(opt) {
		case 0x00:/*输出*/			
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
			break;
		case 0x01:/*等待*/
			num = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);
		  bit = (num) ? BIT_1 : BIT_0;
		  if(bit != ORead(no))
				return OPT_FAILURE;
		  break;
		case 0x04:/*查询*/
			bit = ORead(no);
		  val = (BIT_1 == bit ? 1 : 0);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg, RC_CRC);
		  break;
		case 0x02:/*读取到变量*/
			if(0xFE == type) {
				bit = ORead(no);
		    val = (BIT_1 == bit ? 1 : 0);
			  //DW_CDV_VAR(rxBuf[3]) = ORead(no);
				//ValSet(num, ORead(no));
				ResVarSet(num, val, arg);
			}
			break;
		case 0xFF:/*查询*/
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
//整个I口状态读取
//num:第几个I口，1-20
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
	u32 s[CDV_I_NUM];
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
//整个O口状态读取
//num:第几个I口，1-22
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
	u32 s[CDV_O_NUM];
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
//扩展IO初始化
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
	if(ExIO_Enable[0]==0x11223344&&ExIO_Enable[1]==0x11223344)//扩展IO
	{
		FPGA_Mode = FPGA_IO_CODE;
		CDV_EXI_NUM = 34;
		CDV_EXO_NUM = 30;
	}
	else if(ExIO_Enable[0]==0x22334455&&ExIO_Enable[1]==0x22334455)	//电机程序返回0x22334455
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
