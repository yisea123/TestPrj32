#include "adc.h"

//ADC_STRUCT adc_par={
//											{0,500,0},//CDV_INT32U Slope[3];//斜率值
//											{0,0,0},//CDV_INT32U Zero[3];//零位值
//											{0},//CDV_INT32U Out_Min[3];//输出最小值
//											{0},//CDV_INT32U Out_Max[3];//输出最大值
//											{0},//CDV_INT32U Slope2[3];//临时斜率值
//											{0},//CDV_INT32U Zero2[3];//临时零位值		
//									};
CDV_INT32S* g_adcval = (CDV_INT32S*)g_modbusInReg.var;

ADC_STRUCT adc_par[Adc_no] = {0};									
CDV_INT16U buff_x[BUFFX_LEN];
CDV_FP32 buff_v[BUFFV_LEN];
u16 adc3_value2[adc3_value_lens][2]={0};

//u16 adc1_value[adc1_value_lens]={0}; //ADC1 DMA采样缓存	
//u16 adc2_value[adc2_value_lens]={0}; //ADC2 DMA采样缓存
//u16 adc3_value[adc3_value_lens]={0}; //ADC3 DMA采样缓存
//u16 adc4_value[adc4_value_lens]={0}; //ADC4 DMA采样缓存
u16 AD_Value[N][M];

#if _NPC_VERSION_ == 2u
u16 sum1[adc1_value_lens] = {0};			  
u16 sum2[adc4_value_lens] = {0};		
#elif _NPC_VERSION_ == 3u
u16 sumAdc[M][N] = {0};		
#endif

//初始化ADC，通道0															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA0 通道0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
 
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	

}	

#if _NPC_VERSION_ == 1u
void ADC1_DMA_Init(void)//ADC2 压力传感器
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA2_Stream0);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;	  //DMA通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//(uint32_t)0x4001204C; 	//外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;	//内存地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		//数据传输方向：外设至内存
//  DMA_InitStructure.DMA_BufferSize = RX_ADC1_DMA;						//传输数据大小，由ADC_ConvertedValue数组决定
//	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[0].buf;	//内存地址
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc2_value;	//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
  DMA_InitStructure.DMA_BufferSize = adc1_value_lens;	
	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设地址寄存器自动增加失能
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//内存地址自增失能
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设的数据大小 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//内存的数据大小
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	//传输模式：循环传输
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;		//优先级为高，总共：超高、高、中、低
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;     //指定使用FIFO模式还是直接模式    
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;	//指定了FIFO的阀值
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;		//内存突发传输每次转移一个数据
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发传输每次转移一个数据
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE); //开DMA2_Stream0

	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA0 通道0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);//源数据变化时开启DMA传输

	ADC_DMACmd(ADC1, ENABLE);//关联ADC1-DMA使能
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	  
	ADC_SoftwareStartConv(ADC1);	//开软件中断
}
#elif _NPC_VERSION_ == 2u
void ADC1_DMA_Init(void)//ADC2 压力传感器
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA2_Stream0);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;	  //DMA通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//(uint32_t)0x4001204C; 	//外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;	//内存地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		//数据传输方向：外设至内存
//  DMA_InitStructure.DMA_BufferSize = RX_ADC1_DMA;						//传输数据大小，由ADC_ConvertedValue数组决定
//	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[1].buf;	//内存地址
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc2_value;	//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
  DMA_InitStructure.DMA_BufferSize = adc2_value_lens;	
	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设地址寄存器自动增加失能
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//内存地址自增失能
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设的数据大小 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//内存的数据大小
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	//传输模式：循环传输
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;		//优先级为高，总共：超高、高、中、低
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;     //指定使用FIFO模式还是直接模式    
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;	//指定了FIFO的阀值
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;		//内存突发传输每次转移一个数据
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发传输每次转移一个数据
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE); //开DMA2_Stream0

	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA0 通道0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);//源数据变化时开启DMA传输

	ADC_DMACmd(ADC1, ENABLE);//关联ADC1-DMA使能
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	  
	ADC_SoftwareStartConv(ADC1);	//开软件中断
}

#endif
//初始化ADC，通道10															   
void  Adc2_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC2时钟

  //先初始化ADC2通道10 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PC0 通道10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC2复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化
	
 
	ADC_Cmd(ADC2, ENABLE);//开启AD转换器	

}	
void Adc2_Dma_Init(void)//ADC3 变频器
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA_Channel_1);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;	  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC2->DR;//(uint32_t)0x4001214C; 	//外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc2_value;	//内存地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
//  DMA_InitStructure.DMA_BufferSize = adc2_value_lens;	
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[1].buf;	//内存地址
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value;	//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
  DMA_InitStructure.DMA_BufferSize = adc2_value_lens;	
	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream2, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream2, ENABLE); 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC1时钟
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化	
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);//源数据变化时开启DMA传输

	ADC_DMACmd(ADC2, ENABLE);//关联ADC2-DMA使能
	ADC_Cmd(ADC2, ENABLE);//开启AD转换器	  
	ADC_SoftwareStartConv(ADC2);	//开软件中断
}
/*测试双通道adc*/
void Adc2_Dma_Init2(void)//ADC3 变频器
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA_Channel_1);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;	  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC2->DR;//(uint32_t)0x4001214C; 	//外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc2_value;	//内存地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
//  DMA_InitStructure.DMA_BufferSize = adc2_value_lens;	
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value;//&adc3_value;	//内存地址
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value2;//&adc3_value;	//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
  DMA_InitStructure.DMA_BufferSize = adc3_value_lens*2;	
	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream2, &DMA_InitStructure);
 // DMA_Cmd(DMA2_Stream2, ENABLE); //有人说会引起通道错乱，放后

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC1时钟
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 2;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化	
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_56Cycles);
ADC_RegularChannelConfig(ADC2,ADC_Channel_9, 2, ADC_SampleTime_56Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);//源数据变化时开启DMA传输

	ADC_DMACmd(ADC2, ENABLE);//关联ADC2-DMA使能
	ADC_Cmd(ADC2, ENABLE);//开启AD转换器	  
	
//	ADC_ResetCalibration(ADC1);                    //校验复位

//while(ADC_GetResetCalibrationStatus(ADC1));    //等待复位完成

//ADC_StartCalibration(ADC1);                    //开始ADC1校准

//while(ADC_GetCalibrationStatus(ADC1));         //等待校验完成
DMA_Cmd(DMA2_Stream2, ENABLE); 

	ADC_SoftwareStartConv(ADC2);	//开软件中断
}
/*测试四通道adc*/
void Adc2_Dma_Init4(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA_Channel_1);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;	  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC2->DR;//(uint32_t)0x4001214C; 	//外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value;//&adc3_value;	//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
  DMA_InitStructure.DMA_BufferSize = N*4;//adc3_value_lens*2;	
	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream2, &DMA_InitStructure);
 // DMA_Cmd(DMA2_Stream2, ENABLE); //有人说会引起通道错乱，放后

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOAB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC2时钟
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	//ADC2复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 4;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化	
  ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 2, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 3, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 4, ADC_SampleTime_56Cycles);
	
	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);//源数据变化时开启DMA传输

	ADC_DMACmd(ADC2, ENABLE);//关联ADC2-DMA使能
	ADC_Cmd(ADC2, ENABLE);//开启AD转换器	  
	
//	ADC_ResetCalibration(ADC1);                    //校验复位

//  while(ADC_GetResetCalibrationStatus(ADC1));    //等待复位完成

//  ADC_StartCalibration(ADC1);                    //开始ADC1校准

//  while(ADC_GetCalibrationStatus(ADC1));         //等待校验完成
  DMA_Cmd(DMA2_Stream2, ENABLE); 

	ADC_SoftwareStartConv(ADC2);	//开软件中断
	
#if _NPC_VERSION_ == 3u
    adc_par[0].buf = sumAdc[0];	
		adc_par[1].buf = sumAdc[1];	
		adc_par[2].buf = sumAdc[2];	
		adc_par[3].buf = sumAdc[3];	
#endif
}
//初始化ADC3，通道8															   
void  Adc3_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC2时钟

  //先初始化ADC2通道10 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PF10 通道8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC2复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化
	
	ADC_Cmd(ADC3, ENABLE);//开启AD转换器	

}	


#if _NPC_VERSION_ == 1u
void Adc3_Dma_Init(void)//ADC1 比例阀
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA2_Stream1);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;	  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC3->DR;//(uint32_t)0x4001224C; 	//外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value;	//内存地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
//  DMA_InitStructure.DMA_BufferSize = adc3_value_lens;	
//	
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[2].buf;	//内存地址
//	 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;	//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		//数据传输方向：外设至内存
  DMA_InitStructure.DMA_BufferSize = adc3_value_lens;						//传输数据大小，由ADC_ConvertedValue数组决定
	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream1, ENABLE); 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC3时钟
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	//ADC3复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PF10 通道8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC3复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);//源数据变化时开启DMA传输

	ADC_DMACmd(ADC3, ENABLE);//关联ADC3-DMA使能
	ADC_Cmd(ADC3, ENABLE);//开启AD转换器	  
	ADC_SoftwareStartConv(ADC3);	//开软件中断
}
#elif _NPC_VERSION_ == 2u
void Adc3_Dma_Init(void)//ADC1 比例阀
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA2_Stream1);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;	  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC3->DR;//(uint32_t)0x4001224C; 	//外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value;	//内存地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
//  DMA_InitStructure.DMA_BufferSize = adc3_value_lens;	
//	
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[0].buf;	//内存地址
//	 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;	//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		//数据传输方向：外设至内存
  DMA_InitStructure.DMA_BufferSize = adc1_value_lens;						//传输数据大小，由ADC_ConvertedValue数组决定
	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream1, ENABLE); 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC3时钟
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	//ADC3复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PF10 通道8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC3复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);//源数据变化时开启DMA传输

	ADC_DMACmd(ADC3, ENABLE);//关联ADC3-DMA使能
	ADC_Cmd(ADC3, ENABLE);//开启AD转换器	  
	ADC_SoftwareStartConv(ADC3);	//开软件中断
}

#endif
//初始化ADC3，通道8															   
//void  Adc3_Init(void)
//{    
//  GPIO_InitTypeDef  GPIO_InitStructure;
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef       ADC_InitStructure;
//	
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOC时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC2时钟

//  //先初始化ADC2通道10 IO口
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PF10 通道8
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  
// 
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC2复位
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 
// 
//	
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
//  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
//	
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
//  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
//  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
//  ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化
//	
//	ADC_Cmd(ADC3, ENABLE);//开启AD转换器	

//}	
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
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
u16 Get_Adc2(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_480Cycles );	//ADC2,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC2);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC2);	//返回最近一次ADC1规则组的转换结果
}
u16 Get_ADCx(ADC_TypeDef* ADCx,u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_480Cycles );	//ADC2,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADCx);		//使能指定的ADC1的软件转换启动功能	
	 
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
		temp_val+=Get_ADCx(ADC3,ch);//Get_Adc(ch);
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
		temp_val+=Get_ADCx(ADC1,ch);//Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 


//******************************************************
//求电压值
//adc_num:adc通道1-4
//返回电压值
//******************************************************
//u16 Get_Adc_Voltge ( u8 adc_num)
//{
//	u16 *buf;
//  u16 lens;	
//	u16 adc_val;
//	u16 i;
//	
//	float vol;
//	if( adc_num == 1 )
//	{
//		buf = ADC_ConvertedValue;
//		lens=adc3_value_lens;
//	}
//  else if( adc_num == 2 )
//	{
//		buf=adc2_value;
//		lens=adc2_value_lens;
//	}	
//	else if( adc_num == 3 )
//	{
//		for(i=0;i<100;i++)
//		{
//			sum1[i] = AD_Value[i][0];
//			buf = sum1;
//		}
//		lens=adc1_value_lens;		
//	}	
//	else if ( adc_num == 4 )
//	{
//	 for(i=0;i<100;i++)
//		{
//			sum2[i] = AD_Value[i][1];
//			buf = sum2;
//		}
//		lens=adc4_value_lens;
//	}
//	
//	else
//  {
//		return 0;
//	}		
//  		
//	//adc_val = Adc_val_filter(buf,lens);
//	adc_val = Grubbs(buf,lens);
////	vol = (float)adc_val/4095*3.3;
//	return  adc_val;
//}

void Adc_Voltge ( void)
{
  //u16 lens;	
	//u16 adc_val;
	u16 i;
	//u8 *buf;
	 if( 1 )
	{
#if _NPC_VERSION_ == 1u

#elif _NPC_VERSION_ == 2u
		for(i=0;i<adc3_value_lens;i++)
		{
			sum1[i] = AD_Value[i][0];			 //adc_par[2].buf
			sum2[i] = AD_Value[i][1];      //adc_par[3].buf
		}	
		memcpy(adc_par[2].buf,sum1,adc3_value_lens*sizeof(u16));	
		memcpy(adc_par[3].buf,sum2,adc3_value_lens*sizeof(u16));		
#elif _NPC_VERSION_ == 3u
		for (i = 0; i < N; i++) {
			sumAdc[0][i] = AD_Value[i][0];
			sumAdc[1][i] = AD_Value[i][1];
			sumAdc[2][i] = AD_Value[i][2];
			sumAdc[3][i] = AD_Value[i][3];
		}	
#endif
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
RET_STATUS AdcCmd(CDV_INT08U* rxBuf, CDV_INT16U rxLen, CMD_ARG *arg)
{
	u8 i=0 ,j=0;
	RET_STATUS  ret =OPT_SUCCESS;
	CDV_INT08U  opt , type; //功能码，功能类型
	CDV_INT32U  no ;     //资源号
	CDV_INT32U  pflag;   //参数标记
	u8 Para_len;         //读取参数长度
  u8 Read_Par_Buf[48];  // 读取参数缓存
	CDV_INT32U  var_num;  //存变量号
	u32 var_val;   //存变量值
	u8 ad_num;    //ad号
	u16 VFD_Speed;
	u16 AD_value;
	float current_val;
	s32 tmp;
	u8 Para_Buf[4];
	
	no  = *(CDV_INT32U*)(rxBuf);     // rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	ad_num = rxBuf[0];
//	pflag = (rxBuf[9]<<24)+(rxBuf[8]<<16)+(rxBuf[7]<<8)+rxBuf[6];
	pflag = *(CDV_INT32U*)&rxBuf[6];
	

	switch(opt) 
	{

#ifdef ENABLE_PID
		//参数设置
		case 0x00:

							if((type==0xFF)||(type==0x2F))
							 {	 
									if(pflag & 0x00000001) //斜率值
									{	
										adc_par[no].Slope=((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
//										if(no==2)
//											R_AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
//										else if(no==0)
//										{
//											AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
//										}
										i=i+4;
									}	
									if(pflag & 0x00000002) //零位值
									{	
										adc_par[no].Zero=((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]); 
//										if(no==2)
//											R_PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
//										else if(no==0)
//										{
//											PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
//										}										
										
										i=i+4;
									}	
									if(pflag & 0x00000004)//输出最小值
									{	
										adc_par[no].Out_Min=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
										i=i+4;
									}	
									if(pflag & 0x00000008)//输出最大值
									{	
										adc_par[no].Out_Max=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
										i=i+4;
									}
									if(pflag & 0x00000010)//临时斜率值
									{	
										adc_par[no].Slope2=((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
										i=i+4;
//										if(no==2)
//											R_AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
//										else if(no==0)
//										{
//											AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
//										}																															
									}	
									if(pflag & 0x00000020) //临时零位值
									{	
										adc_par[no].Zero2=((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
										i=i+4;
//										if(no==2)
//											R_PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
//										else if(no==0)
//										{
//											PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
//										}								
									}	
									if(pflag & 0x00000040) //系数1
									{	
										adc_par[no].Ad_Coeff1=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
                    i=i+4;
									}	
									if(pflag & 0x00000080) //系数2
									{	
										adc_par[no].Ad_Coeff2=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
                    i=i+4;
									}
                  if(pflag & 0x00000100) //分压系数
									{	
										adc_par[no].fen_ya_value=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
                   // i=i+4;
									}										
									Valve_PSensor_Coe_Switchover();
							 }	
							 else if ((type==0xFE)||(type==0x2E)) //按变量设参数
							 {
								  var_num = (rxBuf[13+j]<<24)+(rxBuf[12+j]<<16)+(rxBuf[11+j]<<8)+rxBuf[10+j];  //取变量号
									var_val = VarGet(var_num);      //取变量值
									if(pflag & 0x00000001)			//斜率值
									{	
										adc_par[no].Slope=var_val;
//										if(no==2)
//											R_AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
//										else if(no==0)
//										{
//											AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
//										}
									}	
									if(pflag & 0x00000002) //零位值
									{	
										adc_par[no].Zero=var_val;
//										if(no==2)
//											R_PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
//										else if(no==0)
//										{
//											PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
//										}			
									}	
									if(pflag & 0x00000004) //输出最小值
									{	
										adc_par[no].Out_Min=var_val;
									}	
									if(pflag & 0x00000008) //输出最大值
									{	
										adc_par[no].Out_Max=var_val;
									}
									if(pflag & 0x00000010)//临时斜率值
									{	
										adc_par[no].Slope2=var_val;
//										if(no==2)
//											R_AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
//										else if(no==0)
//										{
//											AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
//										}	
									}	
									if(pflag & 0x00000020)//临时零位值
									{	
										adc_par[no].Zero2=var_val;
//										if(no==2)
//											R_PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
//										else if(no==0)
//										{
//											PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
//										}		
									}	
									if(pflag & 0x00000040)//系数1
									{	
										adc_par[no].Ad_Coeff1=var_val;

									}	
									if(pflag & 0x00000080)//系数2
									{	
										adc_par[no].Ad_Coeff2=var_val;
									}	
									
									if(pflag & 0x00000100)//分压系数
									{	
										adc_par[no].fen_ya_value=var_val;
									}	
									Valve_PSensor_Coe_Switchover();
							 }	 
							 if((type==0xFF) || (type==0xFE)) //不是流程中修改参数，保存参数值到flash
	             {							 
								 SPI_Flash_Write((CDV_INT08U *)&adc_par, ADC_PAR_ADDR, sizeof(adc_par)); //写入flash
							 //SPI_Flash_Write_Check((CDV_INT08U *)&adc_par, ADC_PAR_ADDR, sizeof(adc_par));
							 }
		       break;
		//参数读取
		case 0x01:
							if((type==0xFF)||(type==0x2F))
							{	
								Para_len=Read_Adc_Par(Read_Par_Buf,no,pflag);
								ResRequest(arg->buf, arg->len, Read_Par_Buf,Para_len,arg);
							}	
							else if((type==0xFE)||(type==0x2E))//参数读到变量
							{	
								var_val = 0;
								Para_len=Read_Adc_Par(Read_Par_Buf,no,pflag);
								for(i=0;i<Para_len;i++)
								{
									var_val=var_val+(Read_Par_Buf[i]<<(8*(Para_len-i-1)));//buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
								}
								ValSet(rxBuf[10],var_val);
							}
		          break;
		//读到变量	     
		case 0x02:
			
							 AD_value = Get_Ad_value( no );     //气压值
		           ValSet(rxBuf[6],AD_value);
//			        if(ad_num==1) //获取变频器转速
//							{	
//								//VFD_Speed=Get_VFD_Speed(adc3_value,100); 
//								VFD_Speed=Get_VFD_Speed_CH((u16*)(adc_par[no].buf), 100, 2);
//								ValSet(rxBuf[6],VFD_Speed);
//							}
//              else if(ad_num==0) //获取比例调节阀反馈的压力值
//              {
//								if(Process_Start)
//								{
//									PressureAD_Zero = PressureAD_Zero_2;
//									AD_COEFFICIENT = AD_COEFFICIENT_2;
//								}
//								else
//								{
//									PressureAD_Zero = PressureAD_Zero_1;
//									AD_COEFFICIENT = AD_COEFFICIENT_1;									
//								}
								Valve_PSensor_Coe_Switchover();
//								Present_Pressure_Rel = (Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient;
		
//             if(type==0xfe)	
//						 {
							 // DW_CDV_VAR(rxBuf[3])=(CDV_INT32S)Present_Pressure_Rel;
//								ValSet(rxBuf[6],(CDV_INT32S)Present_Pressure_Rel);
//						 }
//							}	
//							else if(ad_num==2)//获取压力传感器压力值
//							{
//								R_Pressure_Measure();
//								ValSet(rxBuf[6],(CDV_INT32S)R_Pressure);
//							}
              //ResParaRequest(rxBuf,rxLen , Para_Buf,Para_len,1);							
		      break;
#endif
		//等待等于
		case 0x03:
			        if(type==0xFF)//等待等于常量
							{
							
							}
 
              else if(type==0xFE)//等待等于变量
							{	

              }  							
			   
		     break;
		case 0x04	://读取AD电压
			         tmp = Get_Read_AD(no);
		         
//			         if(ad_num==0)//比例调节阀反馈电压
//							 {
//									Present_Pressure_AD = Grubbs(ADC_ConvertedValue,RX_ADC1_DMA);
//								  Present_Pressure_AD =Present_Pressure_AD*fen_ya_val;//如果接了分压电阻，乘上分压系数，没接的时候默认是系数是1
//									tmp = Present_Pressure_AD*3300/4096;
//							 }
//							 else if(ad_num==2)//压力传感器反馈电压
//							 {
//									R_Pressure_AD = Grubbs(adc2_value,adc2_value_lens);
//									tmp = R_Pressure_AD*3300/4096;
//							 }
//							 
//							 else if(ad_num==1)//变频器反馈电压
//							 {
//                   tmp = Grubbs(adc3_value,100)*3300/4096;
//							 }
               if(type==0xFE)//读到变量
							 {	
                 								 
								 ValSet(rxBuf[6],tmp);
							 }
							 
               else//读到开发层
							 {								 
								  Para_Buf[0]=tmp>>24;
								  Para_Buf[1]=tmp>>16;
								  Para_Buf[2]=tmp>>8;
								  Para_Buf[3]=tmp;
								  ResRequest(arg->buf, arg->len, Para_Buf,4,arg, RC_CRC);
							 }							 
		     break;							
		case 0xFF	://查询AD电压
				    tmp = AdcRead(no);//Get_Read_AD(no);
				 
						Para_Buf[3]=tmp>>24;
						Para_Buf[2]=tmp>>16;
						Para_Buf[1]=tmp>>8;
						Para_Buf[0]=tmp;
						ResRequest(arg->buf, arg->len, Para_Buf,4,arg, RC_CRC);					 
		     break;							
		default:
			   break;
		
	}	
	return ret;
}

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

	vol = Grubbs(adc_par[no].buf,N);      //滤波
	vol = (vol -(float)adc_par[no].Zero)/4095*3.3;  
	vol = vol*((float)adc_par[no].Slope/1000*adc_par[no].Ad_Coeff1);
	Ad_value = vol* adc_par[no].Ad_Coeff2;

	return Ad_value; 
}
//0-3对应adc1-4
s32 Get_Read_AD(u8 no)   //电压值
{
	s32 tmp;
  float Pressure_AD;
	Pressure_AD = Grubbs(adc_par[no].buf,N);
	//Pressure_AD = Pressure_AD*adc_par[no].fen_ya_value;
#if _NPC_VERSION_ == 1u
	if(0==no)
		tmp = Pressure_AD*1.4276679187314317096466093600764;
	else if(1==no)
		tmp = Pressure_AD*0.76400118114582622029133657040634;
	else
	  tmp = Pressure_AD*3.2641571391360037968675842429995;              //0~3.3V对应 0~3300
#else
	if(0==no)
		tmp = Pressure_AD*1.494768310911809;
	else
	  tmp = Pressure_AD*3.43878954607978;              //0~3.3V对应 0~3300
#endif
	return tmp;
}
//刷新adc的值到g_adcval
void UpdateAdcVal(void) {
	int i = 0;
	
	for (i = 0; i < AD_NUM; i++) {
		g_adcval[i] = Get_Read_AD(i);
	}
}

CDV_INT32S AdcRead(CDV_INT08U no) {
	CDV_INT16U localaddr;
//	CDV_INT16U remoteaddr;
//	CDV_INT08U host;
	
	if (no < AD_NUM)
		return g_adcval[no];
	
	CascadeModbus_MapFindInExp(3, no, &localaddr, NULL, NULL);
	
	return g_modbusInReg.reg[localaddr];
}