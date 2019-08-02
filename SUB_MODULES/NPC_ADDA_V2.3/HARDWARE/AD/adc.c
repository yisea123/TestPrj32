#include "adc.h"


ADC_STRUCT adc_par[Adc_no];									
CDV_INT16U buff_x[BUFFX_LEN];
CDV_FP32 buff_v[BUFFV_LEN];
u16 adc3_value2[adc3_value_lens][2]={0};

u16 adc1_value[adc1_value_lens]={0}; //ADC1 DMA采样缓存	
u16 adc2_value[adc2_value_lens]={0}; //ADC2 DMA采样缓存
u16 adc3_value[adc3_value_lens]={0}; //ADC3 DMA采样缓存
u16 adc4_value[adc4_value_lens]={0}; //ADC4 DMA采样缓存
u16 AD1_Value[N][M];
u16 AD2_Value[N][M];
u16 AD3_Value[N][M1];
u16 sum1[adc1_value_lens] = {0};			  
u16 sum2[adc4_value_lens] = {0};
u16 sum3[adc1_value_lens] = {0};			  
u16 sum4[adc4_value_lens] = {0};
u16 sum5[adc1_value_lens] = {0};			  
u16 sum6[adc4_value_lens] = {0};

//初始化ADC1，通道0 1															   
void  Adc1_DMA_Init(void)
{    
  ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef DMA_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA0 PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		      //模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//顺序进行规则转换的ADC通道的数目
	ADC_RegularChannelConfig( ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig( ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5);
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
  
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStruct.DMA_PeripheralBaseAddr =  (u32)&ADC1->DR;
	DMA_InitStruct.DMA_MemoryBaseAddr = ((uint32_t)&AD1_Value);
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = adc1_value_lens*2;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init( DMA1_Channel1,&DMA_InitStruct);

	DMA_Cmd(DMA1_Channel1, ENABLE);

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);   //采样开始指令，之后ADC就在不停采样
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	ADC_DMACmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}	


//初始化ADC2，通道0 1	ADC2不具有DMA功能
/*****************************************

void  Adc2_DMA_Init(void)
{    
  ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef DMA_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC2, ENABLE );	  //使能ADC1通道时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PC2 PC3 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		      //模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC2);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//顺序进行规则转换的ADC通道的数目
	ADC_RegularChannelConfig( ADC1, ADC_Channel_12, 1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig( ADC1, ADC_Channel_13, 2, ADC_SampleTime_71Cycles5);
	ADC_Init(ADC2, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
  
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStruct.DMA_PeripheralBaseAddr =  (u32)&ADC2->DR;
	DMA_InitStruct.DMA_MemoryBaseAddr = ((uint32_t)&AD2_Value);
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = adc2_value_lens*2;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init( DMA1_Channel2,&DMA_InitStruct);

	DMA_Cmd(DMA1_Channel2, ENABLE);


	ADC_SoftwareStartConvCmd(ADC2, ENABLE);   //采样开始指令，之后ADC就在不停采样
	ADC_Cmd(ADC2, ENABLE);	//使能指定的ADC1
	ADC_DMACmd(ADC2, ENABLE);
	ADC_ResetCalibration(ADC2);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC2));	//等待复位校准结束
	
	ADC_StartCalibration(ADC2);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC2));	 //等待校准结束
 
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);		//使能指定的ADC1的软件转换启动功能
}	
****************************/
//初始化ADC2，通道0 1	
void  Adc3_DMA_Init(void)
{    
  ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef DMA_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC3	, ENABLE );	  //使能ADC1通道时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	// PC0 PC1 PC2 PC3 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		      //模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC3);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 4;	//顺序进行规则转换的ADC通道的数目
	ADC_RegularChannelConfig( ADC3, ADC_Channel_10, 1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig( ADC3, ADC_Channel_11, 2, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig( ADC3, ADC_Channel_12, 3, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig( ADC3, ADC_Channel_13, 4, ADC_SampleTime_71Cycles5);
	ADC_Init(ADC3, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
  
	DMA_DeInit(DMA2_Channel5);
	DMA_InitStruct.DMA_PeripheralBaseAddr =  (u32)&ADC3->DR;
	DMA_InitStruct.DMA_MemoryBaseAddr = ((uint32_t)&AD3_Value);
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = adc3_value_lens*4;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init( DMA2_Channel5,&DMA_InitStruct);

	DMA_Cmd(DMA2_Channel5, ENABLE);


	ADC_SoftwareStartConvCmd(ADC3, ENABLE);   //采样开始指令，之后ADC就在不停采样
	ADC_Cmd(ADC3, ENABLE);	//使能指定的ADC1
	ADC_DMACmd(ADC3, ENABLE);
	ADC_ResetCalibration(ADC3);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC3));	//等待复位校准结束
	
	ADC_StartCalibration(ADC3);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC3));	 //等待校准结束
 
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);		//使能指定的ADC1的软件转换启动功能
}	


//void Adc3_Dma_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef       ADC_InitStructure;
//	DMA_InitTypeDef       DMA_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
////  DMA_DeInit(DMA2_Stream1);
//  /* DMA2 Stream0 channe0 configuration **************************************/
//  DMA_InitStructure.DMA_Channel = DMA_Channel_2;	  
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x4001224C; 	//外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value;	//内存地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
//  DMA_InitStructure.DMA_BufferSize = adc3_value_lens;	
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
//  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
//  DMA_Cmd(DMA2_Stream1, ENABLE); 

//    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC3时钟
//	
//	
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	//ADC3复位
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PF10 通道8
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  
//	
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC3复位
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 
// 
//	
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
//  ADC_CommonInit(&ADC_CommonInitStructure);//初始化	

//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
//	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//连续转换
//	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
//	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
//	ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化	
//	ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 1, ADC_SampleTime_480Cycles);

//	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);//源数据变化时开启DMA传输

//	ADC_DMACmd(ADC3, ENABLE);//关联ADC3-DMA使能
//	ADC_Cmd(ADC3, ENABLE);//开启AD转换器	  
//	ADC_SoftwareStartConv(ADC3);	//开软件中断
//}
//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_Adc1(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_71Cycles5 );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
u16 Get_Adc2(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_71Cycles5 );	//ADC2,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC2);	//返回最近一次ADC1规则组的转换结果
}
u16 Get_ADCx(ADC_TypeDef* ADCx,u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_71Cycles5 );	//ADC2,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADCx);	//返回最近一次ADC1规则组的转换结果
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_ADCx(ADC3,ch);  //Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
u16 Get_Adc2_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_ADCx(ADC1,ch); //Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 


//******************************************************
//求电压值
//adc_num:adc通道1-6
//返回电压值
//******************************************************
u16 Get_Adc_Voltge (u8 adc_num)
{
	u16 *buf;
  u16 lens;	
	u16 adc_val;
	u16 i;
	
	float vol;
	if( adc_num == 1)
	{  
		for(i=0;i<10;i++){
			sum1[i] = AD1_Value[i][1];
			buf = sum1;
		}
		lens=adc1_value_lens;
	}
  else if( adc_num == 2 )
	{
		for(i=0;i<10;i++){
			sum2[i] = AD1_Value[i][0];
			buf = sum2;	
		}
		lens=adc2_value_lens;
	}	
	else if( adc_num == 5 )
	{
		for(i=0;i<10;i++)
		{
			sum3[i] = AD3_Value[i][1];
			buf = sum3;
		}
		lens=adc3_value_lens;		
	}	
	else if ( adc_num == 6 )
	{
	 for(i=0;i<10;i++)
		{
			sum4[i] = AD3_Value[i][0];
			buf = sum4;
		}
		lens=adc4_value_lens;
	}
	else if (adc_num ==3)
	{
	 for(i=0;i<10;i++)
		{
		  sum5[i] = AD3_Value[i][3];
			buf = sum5;
		}
		lens = adc5_value_lens;
	}
	else if(adc_num ==4)
	{
		for(i=0;i<10;i++)
		{	
			sum6[i] = AD3_Value[i][2];
			buf = sum6;
		}
		lens = adc6_value_lens;
	}
	else
  {
		return 0;
	}		
  		
	adc_val = Adc_val_filter(buf,lens);
//	adc_val = Grubbs(buf,lens);
	vol = (float)adc_val/4095*3.3;
	return  adc_val;
}
void AD_ReadInRegister(void)
{
		MODBUS_INREG_ADDR(INREG_AD1)  = Get_Adc_Voltge(1);
		MODBUS_INREG_ADDR(INREG_AD2)  = Get_Adc_Voltge(2);
		MODBUS_INREG_ADDR(INREG_AD3)  = Get_Adc_Voltge(3);
		MODBUS_INREG_ADDR(INREG_AD4)  = Get_Adc_Voltge(4);
	  MODBUS_INREG_ADDR(INREG_AD5)  = Get_Adc_Voltge(5);
		MODBUS_INREG_ADDR(INREG_AD6)  = Get_Adc_Voltge(6);
  }

void Adc_Voltge ( void)
{
  u16 lens;	
	u16 adc_val;
	u16 i;
	u8 *buf;
	 if( 1 )
	{
		for(i=0;i<adc3_value_lens;i++)
		{
			sum1[i] = AD1_Value[i][0];			 //adc_par[2].buf
			sum2[i] = AD1_Value[i][1];       //adc_par[3].buf
		}	
memcpy(adc_par[2].buf,sum1,adc3_value_lens*sizeof(u16));	
memcpy(adc_par[3].buf,sum2,adc3_value_lens*sizeof(u16));	
	}	

}

//******************************************************
//求平均值滤波
//buf：adcDMA缓存
//lens：DMA缓存长度
//******************************************************
u16 Adc_val_filter(u16 *buf,u16 lens)
{
	u16 i;
	double sum=0;
	u16 val;
	for(i=0;i<lens;i++)
	{
		sum=sum+buf[i];
	}
	val=sum/lens;
	return val;
}
//防脉冲干扰平均值滤波

u16 Get_Average(u16 pData[],u16 n)
{
	u16 x[60];	
	u16 Ave;
	u32 sum = 0;
	u16 i,count = 0;
	u16 max = 0;
	u16 min = 65535;
	memcpy(x,pData,sizeof(u16)*n);
	for(i=0;i<n;i++)
	{
		sum += x[i];	
		if(x[i]>max)max=x[i];
		if(x[i]<min&&(x[i]!=0))min=x[i];
		if(x[i]==0)
			count++;
	}
	Ave=(sum-max-min)/(n-count-2);
	return Ave;
}

u16 Grubbs(u16 x[],u16 n)
{	
	u8 leap=1;
	u16 i,max=0,min=0;
	float temp=0,sd,aver;
	u16 len = n;
	//float v[1000];		
	double ld=0;
	
	memcpy(buff_x,x,sizeof(u16)*n);	
	
	while(leap)
	{		
		aver=AVRG(buff_x,n);
		//aver=AVRG(buff,n);		
		for(i=0;i<n;i++)
		{
			buff_v[i]=buff_x[i]-aver;
			ld+=buff_v[i]*buff_v[i];
			if(buff_x[min]>buff_x[i])min=i;
			if(buff_x[max]<buff_x[i])max=i;
		}
		sd=sqrt((ld)/(n-1));
		if(abs(buff_v[max])<abs(buff_v[min]))max=min;
		if(abs(buff_v[max])>abs(3*sd))
		{
			for(i=max;i<n-1;i++)
				x[i]=x[i+1];	//除去最大误差值，其右侧数据左移
			leap=1;
			n=n-1;
			if(n<=len/3)break;
		}
		else
		{
			leap=0;
		}
	}
	temp=AVRG(x,n);
	//temp=AVRG2(x,n);	
	
	memset(buff_x,0,sizeof(float)*BUFFX_LEN);		
	memset(buff_v,0,sizeof(float)*BUFFV_LEN);	
	
	return temp;
}
float AVRG(u16 x[],u16 n)
{
	float aver=0;
//	u16 x[3000];	
//	memcpy(x,pData,n*sizeof(u16));
	aver=SUM(x,n)/n;
	return aver;
}
u32 SUM(u16 x[],u16 n)
{
	u32 s=0;
	u16 i;
//	u16 x[3000];	
//	memcpy(x,pData,n*sizeof(u16));
	for(i=0;i<n;i++)
		s+=x[i];
	return s;
}


//u16 Get_VFD_Speed_CH(u16 *buf,u16 lens,u8 channel)
//{
//	u16 i;
//	u16 vfd_speed;
//	double sum=0;
//	double vol;
//	for(i=0;i<lens;i++)
//	{
//		sum=sum+buf[i*channel];
//	}
//	vol=sum/lens/4095*3.3;
//	vol=vol-(float)adc_par.Zero[1];
//  vol=vol*((float)adc_par.Slope[1]/1000);//*斜率
//	//vfd_speed=vol*4*5;
//	vfd_speed=vol*10*4;//变频器电压1v=10转
//	//vfd_speed=vfd_speed*(float)adc_par.Slope[1]/1000-(float)adc_par.Zero[1]/1000;//*斜率-零位值
//	return vfd_speed;
//}




//*******************************************
//CDV adc
//
//
//
//
//*******************************************
//RET_STATUS AdcCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg)
//{
//	u8 i=0 ,j=0;
//	RET_STATUS  ret =OPT_SUCCESS;
//	CDV_INT08U  opt , type; //功能码，功能类型
//	CDV_INT32U  no ;     //资源号
//	CDV_INT32U  pflag;   //参数标记
//	u8 Para_len;         //读取参数长度
//  u8 Read_Par_Buf[48];  // 读取参数缓存
//	CDV_INT32U  var_num;  //存变量号
//	u32 var_val;   //存变量值
//	u8 ad_num;    //ad号
//	u16 VFD_Speed;
//	u16 AD_value;
//	float current_val;
//	s32 tmp;
//	u8 Para_Buf[4];
//	
//	no  = *(CDV_INT32U*)(rxBuf);     // rxBuf[0];
//	opt = rxBuf[4];
//	type = rxBuf[5];
//	ad_num = rxBuf[0];
////	pflag = (rxBuf[9]<<24)+(rxBuf[8]<<16)+(rxBuf[7]<<8)+rxBuf[6];
//	pflag = *(CDV_INT32U*)&rxBuf[6];
//	

//	switch(opt) 
//	{
//		//参数设置
//		case 0x00:

//							if((type==0xFF)||(type==0x2F))
//							 {	 
//									if(pflag & 0x00000001) //斜率值
//									{	
//										adc_par[no].Slope=((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
////										if(no==2)
////											R_AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
////										else if(no==0)
////										{
////											AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
////										}
//										i=i+4;
//									}	
//									if(pflag & 0x00000002) //零位值
//									{	
//										adc_par[no].Zero=((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]); 
////										if(no==2)
////											R_PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
////										else if(no==0)
////										{
////											PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
////										}										
//										
//										i=i+4;
//									}	
//									if(pflag & 0x00000004)//输出最小值
//									{	
//										adc_par[no].Out_Min=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
//										i=i+4;
//									}	
//									if(pflag & 0x00000008)//输出最大值
//									{	
//										adc_par[no].Out_Max=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
//										i=i+4;
//									}
//									if(pflag & 0x00000010)//临时斜率值
//									{	
//										adc_par[no].Slope2=((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
//										i=i+4;
////										if(no==2)
////											R_AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
////										else if(no==0)
////										{
////											AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
////										}																															
//									}	
//									if(pflag & 0x00000020) //临时零位值
//									{	
//										adc_par[no].Zero2=((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
//										i=i+4;
////										if(no==2)
////											R_PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
////										else if(no==0)
////										{
////											PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
////										}								
//									}	
//									if(pflag & 0x00000040) //系数1
//									{	
//										adc_par[no].Ad_Coeff1=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
//                    i=i+4;
//									}	
//									if(pflag & 0x00000080) //系数2
//									{	
//										adc_par[no].Ad_Coeff2=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
//                    i=i+4;
//									}
//                  if(pflag & 0x00000100) //分压系数
//									{	
//										adc_par[no].fen_ya_value=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
//                   // i=i+4;
//									}										
//									Valve_PSensor_Coe_Switchover();
//							 }	
//							 else if ((type==0xFE)||(type==0x2E)) //按变量设参数
//							 {
//								  var_num = (rxBuf[13+j]<<24)+(rxBuf[12+j]<<16)+(rxBuf[11+j]<<8)+rxBuf[10+j];  //取变量号
//									var_val = VarGet(var_num);      //取变量值
//									if(pflag & 0x00000001)			//斜率值
//									{	
//										adc_par[no].Slope=var_val;
////										if(no==2)
////											R_AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
////										else if(no==0)
////										{
////											AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
////										}
//									}	
//									if(pflag & 0x00000002) //零位值
//									{	
//										adc_par[no].Zero=var_val;
////										if(no==2)
////											R_PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
////										else if(no==0)
////										{
////											PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
////										}			
//									}	
//									if(pflag & 0x00000004) //输出最小值
//									{	
//										adc_par[no].Out_Min=var_val;
//									}	
//									if(pflag & 0x00000008) //输出最大值
//									{	
//										adc_par[no].Out_Max=var_val;
//									}
//									if(pflag & 0x00000010)//临时斜率值
//									{	
//										adc_par[no].Slope2=var_val;
////										if(no==2)
////											R_AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
////										else if(no==0)
////										{
////											AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
////										}	
//									}	
//									if(pflag & 0x00000020)//临时零位值
//									{	
//										adc_par[no].Zero2=var_val;
////										if(no==2)
////											R_PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
////										else if(no==0)
////										{
////											PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
////										}		
//									}	
//									if(pflag & 0x00000040)//系数1
//									{	
//										adc_par[no].Ad_Coeff1=var_val;

//									}	
//									if(pflag & 0x00000080)//系数2
//									{	
//										adc_par[no].Ad_Coeff2=var_val;
//									}	
//									
//									if(pflag & 0x00000100)//分压系数
//									{	
//										adc_par[no].fen_ya_value=var_val;
//									}	
//									Valve_PSensor_Coe_Switchover();
//							 }	 
//							 if((type==0xFF) || (type==0xFE)) //不是流程中修改参数，保存参数值到flash
//	             {							 
//								 SPI_Flash_Write((CDV_INT08U *)&adc_par, ADC_PAR_ADDR, sizeof(adc_par)); //写入flash
//							 //SPI_Flash_Write_Check((CDV_INT08U *)&adc_par, ADC_PAR_ADDR, sizeof(adc_par));
//							 }
//		       break;
//		//参数读取
//		case 0x01:
//							if((type==0xFF)||(type==0x2F))
//							{	
//								Para_len=Read_Adc_Par(Read_Par_Buf,no,pflag);
//								ResRequest(arg->buf, arg->len, Read_Par_Buf,Para_len,arg);
//							}	
//							else if((type==0xFE)||(type==0x2E))//参数读到变量
//							{	
//								var_val = 0;
//								Para_len=Read_Adc_Par(Read_Par_Buf,no,pflag);
//								for(i=0;i<Para_len;i++)
//								{
//									var_val=var_val+(Read_Par_Buf[i]<<(8*(Para_len-i-1)));//buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
//								}
//								ValSet(rxBuf[10],var_val);
//							}
//		          break;
//		//读到变量	     
//		case 0x02:
//			
//							 AD_value = Get_Ad_value( no );     //气压值
//		           ValSet(rxBuf[6],AD_value);
////			        if(ad_num==1) //获取变频器转速
////							{	
////								//VFD_Speed=Get_VFD_Speed(adc3_value,100); 
////								VFD_Speed=Get_VFD_Speed_CH((u16*)(adc_par[no].buf), 100, 2);
////								ValSet(rxBuf[6],VFD_Speed);
////							}
////              else if(ad_num==0) //获取比例调节阀反馈的压力值
////              {
////								if(Process_Start)
////								{
////									PressureAD_Zero = PressureAD_Zero_2;
////									AD_COEFFICIENT = AD_COEFFICIENT_2;
////								}
////								else
////								{
////									PressureAD_Zero = PressureAD_Zero_1;
////									AD_COEFFICIENT = AD_COEFFICIENT_1;									
////								}
//								Valve_PSensor_Coe_Switchover();
////								Present_Pressure_Rel = (Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient;
//		
////             if(type==0xfe)	
////						 {
//							 // DW_CDV_VAR(rxBuf[3])=(CDV_INT32S)Present_Pressure_Rel;
////								ValSet(rxBuf[6],(CDV_INT32S)Present_Pressure_Rel);
////						 }
////							}	
////							else if(ad_num==2)//获取压力传感器压力值
////							{
////								R_Pressure_Measure();
////								ValSet(rxBuf[6],(CDV_INT32S)R_Pressure);
////							}
//              //ResParaRequest(rxBuf,rxLen , Para_Buf,Para_len,1);							
//		      break;
//		//等待等于
//		case 0x03:
//			        if(type==0xFF)//等待等于常量
//							{
//							
//							}
// 
//              else if(type==0xFE)//等待等于变量
//							{	

//              }  							
//			   
//		     break;
//		case 0x04	://读取AD电压
//			         tmp = Get_Read_AD(no);
//		         
////			         if(ad_num==0)//比例调节阀反馈电压
////							 {
////									Present_Pressure_AD = Grubbs(ADC_ConvertedValue,RX_ADC1_DMA);
////								  Present_Pressure_AD =Present_Pressure_AD*fen_ya_val;//如果接了分压电阻，乘上分压系数，没接的时候默认是系数是1
////									tmp = Present_Pressure_AD*3300/4096;
////							 }
////							 else if(ad_num==2)//压力传感器反馈电压
////							 {
////									R_Pressure_AD = Grubbs(adc2_value,adc2_value_lens);
////									tmp = R_Pressure_AD*3300/4096;
////							 }
////							 
////							 else if(ad_num==1)//变频器反馈电压
////							 {
////                   tmp = Grubbs(adc3_value,100)*3300/4096;
////							 }
//               if(type==0xFE)//读到变量
//							 {	
//                 								 
//								 ValSet(rxBuf[6],tmp);
//							 }
//							 
//               else//读到开发层
//							 {								 
//								  Para_Buf[0]=tmp>>24;
//								  Para_Buf[1]=tmp>>16;
//								  Para_Buf[2]=tmp>>8;
//								  Para_Buf[3]=tmp;
//								  ResRequest(arg->buf, arg->len, Para_Buf,4,arg);
//							 }							 
//		     break;							
//		default:
//			    break;
//		
//	}	
//	return ret;
//}

//****************************************************
//读取参数
//no:设备号
//pflag:参数标记
//buf:参数缓存
//****************************************************
u8 Read_Adc_Par(u8 *buf,u8 no,u32 pflag)
{
	u8 i,j;
	u32 FPGA_POS;
	u16 FPGA_POSH;
	u16 FPGA_POSL;
	
	u8 buf_no;
	
	i=0;
	if(no==0)
	{
		buf_no=0;
	}
	else if(no==1)
	{
		buf_no=2;
	}
	else if(no==2)
	{
		buf_no=1;
	}
  	
	 if(pflag & 0x00000001)//斜率值
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Slope>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000002)//零位值
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Zero>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000004)//输出最小值
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Out_Min>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x000000008)//输出最大值
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Out_Max>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000010)//临时斜率值
	 {	
		  for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Slope2>>(24-8*j);
			}
			i=i+4;									
	 }	
	 if(pflag & 0x00000020) //临时零位值
	 {	
		 for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Zero2>>(24-8*j);
			}
			i=i+4;						
	 }
	 if(pflag & 0x00000040) //系数1
	 {	
		 for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Ad_Coeff1>>(24-8*j);
			}
			i=i+4;						
	 }
	 if(pflag & 0x00000080) //系数2
	 {	
		 for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Ad_Coeff2>>(24-8*j);
			}
			i=i+4;						
	 }
	 
	if(pflag & 0x00000100)//分压系数
	{	
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].fen_ya_value>>(24-8*j);
			}
			i=i+4;
	}	
	return i; //返回长度
	 
}	
//******************************************************
//变频器转速
//
//
//******************************************************

//u16 Get_VFD_Speed(u16 *buf,u16 lens)
//{
//	u16 i;
//	u16 vfd_speed;
//	double sum=0;
//	double vol;
//	for(i=0;i<lens;i++)
//	{
//		sum=sum+buf[i];
//	}
//	vol=sum/lens/4095*3.3;
//	vol=vol-(float)adc_par.Zero[1];
//  vol=vol*((float)adc_par.Slope[1]/1000);//*斜率
//	//vfd_speed=vol*4*5;
//	vfd_speed=vol*10*4;//变频器电压1v=10转
//	//vfd_speed=vfd_speed*(float)adc_par.Slope[1]/1000-(float)adc_par.Zero[1]/1000;//*斜率-零位值
//	return vfd_speed;
//}

u16 Get_Ad_value(u8 no)   //气压值
{
  u16 i;
	u16 Ad_value;
	double sum=0;
	double vol;

	vol = Grubbs(adc_par[no].buf,100);      //滤波
	vol = (vol -(float)adc_par[no].Zero)/4095*3.3;  
	vol = vol*((float)adc_par[no].Slope/1000*adc_par[no].Ad_Coeff1);
	Ad_value = vol* adc_par[no].Ad_Coeff2;

	return Ad_value; 
}

s32 Get_Read_AD(u8 no)   //电压值
{
	s32 tmp;
  float Pressure_AD;
	Pressure_AD = Grubbs(adc_par[no].buf,100);
	Pressure_AD = Pressure_AD*adc_par[no].fen_ya_value;
	tmp = Pressure_AD*3300/4096;              //0~3.3V对应 0~3300
	return tmp;
}

