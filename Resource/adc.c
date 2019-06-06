#include "adc.h"

//ADC_STRUCT adc_par={
//											{0,500,0},//CDV_INT32U Slope[3];//б��ֵ
//											{0,0,0},//CDV_INT32U Zero[3];//��λֵ
//											{0},//CDV_INT32U Out_Min[3];//�����Сֵ
//											{0},//CDV_INT32U Out_Max[3];//������ֵ
//											{0},//CDV_INT32U Slope2[3];//��ʱб��ֵ
//											{0},//CDV_INT32U Zero2[3];//��ʱ��λֵ		
//									};
CDV_INT32S* g_adcval = (CDV_INT32S*)g_modbusInReg.var;

ADC_STRUCT adc_par[Adc_no] = {0};									
CDV_INT16U buff_x[BUFFX_LEN];
CDV_FP32 buff_v[BUFFV_LEN];
u16 adc3_value2[adc3_value_lens][2]={0};

//u16 adc1_value[adc1_value_lens]={0}; //ADC1 DMA��������	
//u16 adc2_value[adc2_value_lens]={0}; //ADC2 DMA��������
//u16 adc3_value[adc3_value_lens]={0}; //ADC3 DMA��������
//u16 adc4_value[adc4_value_lens]={0}; //ADC4 DMA��������
u16 AD_Value[N][M];

#if _NPC_VERSION_ == 2u
u16 sum1[adc1_value_lens] = {0};			  
u16 sum2[adc4_value_lens] = {0};		
#elif _NPC_VERSION_ == 3u
u16 sumAdc[M][N] = {0};		
#endif

//��ʼ��ADC��ͨ��0															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

  //�ȳ�ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA0 ͨ��0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
 
	ADC_Cmd(ADC1, ENABLE);//����ADת����	

}	

#if _NPC_VERSION_ == 1u
void ADC1_DMA_Init(void)//ADC2 ѹ��������
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA2_Stream0);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;	  //DMAͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//(uint32_t)0x4001204C; 	//�����ַ
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;	//�ڴ��ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		//���ݴ��䷽���������ڴ�
//  DMA_InitStructure.DMA_BufferSize = RX_ADC1_DMA;						//�������ݴ�С����ADC_ConvertedValue�������
//	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[0].buf;	//�ڴ��ַ
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc2_value;	//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
  DMA_InitStructure.DMA_BufferSize = adc1_value_lens;	
	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�Ĵ����Զ�����ʧ��
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�ڴ��ַ����ʧ��
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//��������ݴ�С 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//�ڴ�����ݴ�С
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	//����ģʽ��ѭ������
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;		//���ȼ�Ϊ�ߣ��ܹ������ߡ��ߡ��С���
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;     //ָ��ʹ��FIFOģʽ����ֱ��ģʽ    
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;	//ָ����FIFO�ķ�ֵ
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;		//�ڴ�ͻ������ÿ��ת��һ������
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ������ÿ��ת��һ������
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE); //��DMA2_Stream0

	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA0 ͨ��0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);//Դ���ݱ仯ʱ����DMA����

	ADC_DMACmd(ADC1, ENABLE);//����ADC1-DMAʹ��
	ADC_Cmd(ADC1, ENABLE);//����ADת����	  
	ADC_SoftwareStartConv(ADC1);	//������ж�
}
#elif _NPC_VERSION_ == 2u
void ADC1_DMA_Init(void)//ADC2 ѹ��������
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA2_Stream0);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;	  //DMAͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//(uint32_t)0x4001204C; 	//�����ַ
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;	//�ڴ��ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		//���ݴ��䷽���������ڴ�
//  DMA_InitStructure.DMA_BufferSize = RX_ADC1_DMA;						//�������ݴ�С����ADC_ConvertedValue�������
//	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[1].buf;	//�ڴ��ַ
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc2_value;	//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
  DMA_InitStructure.DMA_BufferSize = adc2_value_lens;	
	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�Ĵ����Զ�����ʧ��
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�ڴ��ַ����ʧ��
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//��������ݴ�С 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//�ڴ�����ݴ�С
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	//����ģʽ��ѭ������
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;		//���ȼ�Ϊ�ߣ��ܹ������ߡ��ߡ��С���
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;     //ָ��ʹ��FIFOģʽ����ֱ��ģʽ    
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;	//ָ����FIFO�ķ�ֵ
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;		//�ڴ�ͻ������ÿ��ת��һ������
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ������ÿ��ת��һ������
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE); //��DMA2_Stream0

	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA0 ͨ��0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);//Դ���ݱ仯ʱ����DMA����

	ADC_DMACmd(ADC1, ENABLE);//����ADC1-DMAʹ��
	ADC_Cmd(ADC1, ENABLE);//����ADת����	  
	ADC_SoftwareStartConv(ADC1);	//������ж�
}

#endif
//��ʼ��ADC��ͨ��10															   
void  Adc2_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //ʹ��ADC2ʱ��

  //�ȳ�ʼ��ADC2ͨ��10 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PC0 ͨ��10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC2��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC2, &ADC_InitStructure);//ADC��ʼ��
	
 
	ADC_Cmd(ADC2, ENABLE);//����ADת����	

}	
void Adc2_Dma_Init(void)//ADC3 ��Ƶ��
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA_Channel_1);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;	  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC2->DR;//(uint32_t)0x4001214C; 	//�����ַ
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc2_value;	//�ڴ��ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
//  DMA_InitStructure.DMA_BufferSize = adc2_value_lens;	
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[1].buf;	//�ڴ��ַ
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value;	//�ڴ��ַ
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

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //ʹ��ADC1ʱ��
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC2, &ADC_InitStructure);//ADC��ʼ��	
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);//Դ���ݱ仯ʱ����DMA����

	ADC_DMACmd(ADC2, ENABLE);//����ADC2-DMAʹ��
	ADC_Cmd(ADC2, ENABLE);//����ADת����	  
	ADC_SoftwareStartConv(ADC2);	//������ж�
}
/*����˫ͨ��adc*/
void Adc2_Dma_Init2(void)//ADC3 ��Ƶ��
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA_Channel_1);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;	  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC2->DR;//(uint32_t)0x4001214C; 	//�����ַ
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc2_value;	//�ڴ��ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
//  DMA_InitStructure.DMA_BufferSize = adc2_value_lens;	
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value;//&adc3_value;	//�ڴ��ַ
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value2;//&adc3_value;	//�ڴ��ַ
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
 // DMA_Cmd(DMA2_Stream2, ENABLE); //����˵������ͨ�����ң��ź�

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //ʹ��ADC1ʱ��
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 2;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC2, &ADC_InitStructure);//ADC��ʼ��	
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_56Cycles);
ADC_RegularChannelConfig(ADC2,ADC_Channel_9, 2, ADC_SampleTime_56Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);//Դ���ݱ仯ʱ����DMA����

	ADC_DMACmd(ADC2, ENABLE);//����ADC2-DMAʹ��
	ADC_Cmd(ADC2, ENABLE);//����ADת����	  
	
//	ADC_ResetCalibration(ADC1);                    //У�鸴λ

//while(ADC_GetResetCalibrationStatus(ADC1));    //�ȴ���λ���

//ADC_StartCalibration(ADC1);                    //��ʼADC1У׼

//while(ADC_GetCalibrationStatus(ADC1));         //�ȴ�У�����
DMA_Cmd(DMA2_Stream2, ENABLE); 

	ADC_SoftwareStartConv(ADC2);	//������ж�
}
/*������ͨ��adc*/
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
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC2->DR;//(uint32_t)0x4001214C; 	//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&AD_Value;//&adc3_value;	//�ڴ��ַ
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
 // DMA_Cmd(DMA2_Stream2, ENABLE); //����˵������ͨ�����ң��ź�

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOABʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //ʹ��ADC2ʱ��
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	//ADC2��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 4;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC2, &ADC_InitStructure);//ADC��ʼ��	
  ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 2, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 3, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 4, ADC_SampleTime_56Cycles);
	
	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);//Դ���ݱ仯ʱ����DMA����

	ADC_DMACmd(ADC2, ENABLE);//����ADC2-DMAʹ��
	ADC_Cmd(ADC2, ENABLE);//����ADת����	  
	
//	ADC_ResetCalibration(ADC1);                    //У�鸴λ

//  while(ADC_GetResetCalibrationStatus(ADC1));    //�ȴ���λ���

//  ADC_StartCalibration(ADC1);                    //��ʼADC1У׼

//  while(ADC_GetCalibrationStatus(ADC1));         //�ȴ�У�����
  DMA_Cmd(DMA2_Stream2, ENABLE); 

	ADC_SoftwareStartConv(ADC2);	//������ж�
	
#if _NPC_VERSION_ == 3u
    adc_par[0].buf = sumAdc[0];	
		adc_par[1].buf = sumAdc[1];	
		adc_par[2].buf = sumAdc[2];	
		adc_par[3].buf = sumAdc[3];	
#endif
}
//��ʼ��ADC3��ͨ��8															   
void  Adc3_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //ʹ��ADC2ʱ��

  //�ȳ�ʼ��ADC2ͨ��10 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PF10 ͨ��8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC2��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC3, &ADC_InitStructure);//ADC��ʼ��
	
	ADC_Cmd(ADC3, ENABLE);//����ADת����	

}	


#if _NPC_VERSION_ == 1u
void Adc3_Dma_Init(void)//ADC1 ������
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA2_Stream1);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;	  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC3->DR;//(uint32_t)0x4001224C; 	//�����ַ
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value;	//�ڴ��ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
//  DMA_InitStructure.DMA_BufferSize = adc3_value_lens;	
//	
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[2].buf;	//�ڴ��ַ
//	 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;	//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		//���ݴ��䷽���������ڴ�
  DMA_InitStructure.DMA_BufferSize = adc3_value_lens;						//�������ݴ�С����ADC_ConvertedValue�������
	
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

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //ʹ��ADC3ʱ��
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	//ADC3��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PF10 ͨ��8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC3��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC��ʼ��	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);//Դ���ݱ仯ʱ����DMA����

	ADC_DMACmd(ADC3, ENABLE);//����ADC3-DMAʹ��
	ADC_Cmd(ADC3, ENABLE);//����ADת����	  
	ADC_SoftwareStartConv(ADC3);	//������ж�
}
#elif _NPC_VERSION_ == 2u
void Adc3_Dma_Init(void)//ADC1 ������
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//  DMA_DeInit(DMA2_Stream1);
  /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;	  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC3->DR;//(uint32_t)0x4001224C; 	//�����ַ
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value;	//�ڴ��ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		
//  DMA_InitStructure.DMA_BufferSize = adc3_value_lens;	
//	
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_par[0].buf;	//�ڴ��ַ
//	 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_ConvertedValue;	//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;		//���ݴ��䷽���������ڴ�
  DMA_InitStructure.DMA_BufferSize = adc1_value_lens;						//�������ݴ�С����ADC_ConvertedValue�������
	
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

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //ʹ��ADC3ʱ��
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	//ADC3��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PF10 ͨ��8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC3��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��	

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC��ʼ��	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);//Դ���ݱ仯ʱ����DMA����

	ADC_DMACmd(ADC3, ENABLE);//����ADC3-DMAʹ��
	ADC_Cmd(ADC3, ENABLE);//����ADת����	  
	ADC_SoftwareStartConv(ADC3);	//������ж�
}

#endif
//��ʼ��ADC3��ͨ��8															   
//void  Adc3_Init(void)
//{    
//  GPIO_InitTypeDef  GPIO_InitStructure;
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef       ADC_InitStructure;
//	
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOCʱ��
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //ʹ��ADC2ʱ��

//  //�ȳ�ʼ��ADC2ͨ��10 IO��
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PF10 ͨ��8
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  
// 
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC2��λ
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 
// 
//	
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
//  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
//	
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
//  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
//  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
//  ADC_Init(ADC3, &ADC_InitStructure);//ADC��ʼ��
//	
//	ADC_Cmd(ADC3, ENABLE);//����ADת����	

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
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x4001224C; 	//�����ַ
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc3_value;	//�ڴ��ַ
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

//    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //ʹ��ADC3ʱ��
//	
//	
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	//ADC3��λ
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PF10 ͨ��8
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  
//	
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC3��λ
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 
// 
//	
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
//  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��	

//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
//	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE	;//����ת��
//	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
//	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
//	ADC_Init(ADC3, &ADC_InitStructure);//ADC��ʼ��	
//	ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 1, ADC_SampleTime_480Cycles);

//	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);//Դ���ݱ仯ʱ����DMA����

//	ADC_DMACmd(ADC3, ENABLE);//����ADC3-DMAʹ��
//	ADC_Cmd(ADC3, ENABLE);//����ADת����	  
//	ADC_SoftwareStartConv(ADC3);	//������ж�
//}
//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
u16 Get_Adc2(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_480Cycles );	//ADC2,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC2);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC2);	//�������һ��ADC1�������ת�����
}
u16 Get_ADCx(ADC_TypeDef* ADCx,u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_480Cycles );	//ADC2,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADCx);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADCx);	//�������һ��ADC1�������ת�����
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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
//���ѹֵ
//adc_num:adcͨ��1-4
//���ص�ѹֵ
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
//��ƽ��ֵ�˲�
//buf��adcDMA����
//lens��DMA���泤��
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
//���������ƽ��ֵ�˲�

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
				x[i]=x[i+1];	//��ȥ������ֵ�����Ҳ���������
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
//  vol=vol*((float)adc_par.Slope[1]/1000);//*б��
//	//vfd_speed=vol*4*5;
//	vfd_speed=vol*10*4;//��Ƶ����ѹ1v=10ת
//	//vfd_speed=vfd_speed*(float)adc_par.Slope[1]/1000-(float)adc_par.Zero[1]/1000;//*б��-��λֵ
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
	CDV_INT08U  opt , type; //�����룬��������
	CDV_INT32U  no ;     //��Դ��
	CDV_INT32U  pflag;   //�������
	u8 Para_len;         //��ȡ��������
  u8 Read_Par_Buf[48];  // ��ȡ��������
	CDV_INT32U  var_num;  //�������
	u32 var_val;   //�����ֵ
	u8 ad_num;    //ad��
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
		//��������
		case 0x00:

							if((type==0xFF)||(type==0x2F))
							 {	 
									if(pflag & 0x00000001) //б��ֵ
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
									if(pflag & 0x00000002) //��λֵ
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
									if(pflag & 0x00000004)//�����Сֵ
									{	
										adc_par[no].Out_Min=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
										i=i+4;
									}	
									if(pflag & 0x00000008)//������ֵ
									{	
										adc_par[no].Out_Max=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
										i=i+4;
									}
									if(pflag & 0x00000010)//��ʱб��ֵ
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
									if(pflag & 0x00000020) //��ʱ��λֵ
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
									if(pflag & 0x00000040) //ϵ��1
									{	
										adc_par[no].Ad_Coeff1=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
                    i=i+4;
									}	
									if(pflag & 0x00000080) //ϵ��2
									{	
										adc_par[no].Ad_Coeff2=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
                    i=i+4;
									}
                  if(pflag & 0x00000100) //��ѹϵ��
									{	
										adc_par[no].fen_ya_value=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
                   // i=i+4;
									}										
									Valve_PSensor_Coe_Switchover();
							 }	
							 else if ((type==0xFE)||(type==0x2E)) //�����������
							 {
								  var_num = (rxBuf[13+j]<<24)+(rxBuf[12+j]<<16)+(rxBuf[11+j]<<8)+rxBuf[10+j];  //ȡ������
									var_val = VarGet(var_num);      //ȡ����ֵ
									if(pflag & 0x00000001)			//б��ֵ
									{	
										adc_par[no].Slope=var_val;
//										if(no==2)
//											R_AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
//										else if(no==0)
//										{
//											AD_COEFFICIENT_1 = (CDV_FP64)adc_par.Slope[no]/10000;
//										}
									}	
									if(pflag & 0x00000002) //��λֵ
									{	
										adc_par[no].Zero=var_val;
//										if(no==2)
//											R_PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
//										else if(no==0)
//										{
//											PressureAD_Zero_1 = adc_par.Zero[no]*4096/3300;
//										}			
									}	
									if(pflag & 0x00000004) //�����Сֵ
									{	
										adc_par[no].Out_Min=var_val;
									}	
									if(pflag & 0x00000008) //������ֵ
									{	
										adc_par[no].Out_Max=var_val;
									}
									if(pflag & 0x00000010)//��ʱб��ֵ
									{	
										adc_par[no].Slope2=var_val;
//										if(no==2)
//											R_AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
//										else if(no==0)
//										{
//											AD_COEFFICIENT_2 = (CDV_FP64)adc_par.Slope2[no]/10000;	
//										}	
									}	
									if(pflag & 0x00000020)//��ʱ��λֵ
									{	
										adc_par[no].Zero2=var_val;
//										if(no==2)
//											R_PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
//										else if(no==0)
//										{
//											PressureAD_Zero_2 = adc_par.Zero2[no]*4096/3300;		
//										}		
									}	
									if(pflag & 0x00000040)//ϵ��1
									{	
										adc_par[no].Ad_Coeff1=var_val;

									}	
									if(pflag & 0x00000080)//ϵ��2
									{	
										adc_par[no].Ad_Coeff2=var_val;
									}	
									
									if(pflag & 0x00000100)//��ѹϵ��
									{	
										adc_par[no].fen_ya_value=var_val;
									}	
									Valve_PSensor_Coe_Switchover();
							 }	 
							 if((type==0xFF) || (type==0xFE)) //�����������޸Ĳ������������ֵ��flash
	             {							 
								 SPI_Flash_Write((CDV_INT08U *)&adc_par, ADC_PAR_ADDR, sizeof(adc_par)); //д��flash
							 //SPI_Flash_Write_Check((CDV_INT08U *)&adc_par, ADC_PAR_ADDR, sizeof(adc_par));
							 }
		       break;
		//������ȡ
		case 0x01:
							if((type==0xFF)||(type==0x2F))
							{	
								Para_len=Read_Adc_Par(Read_Par_Buf,no,pflag);
								ResRequest(arg->buf, arg->len, Read_Par_Buf,Para_len,arg);
							}	
							else if((type==0xFE)||(type==0x2E))//������������
							{	
								var_val = 0;
								Para_len=Read_Adc_Par(Read_Par_Buf,no,pflag);
								for(i=0;i<Para_len;i++)
								{
									var_val=var_val+(Read_Par_Buf[i]<<(8*(Para_len-i-1)));//buf��4Byte��һ��ֻ��һ��������ÿ����������ȡ4���ֽ���ֵ
								}
								ValSet(rxBuf[10],var_val);
							}
		          break;
		//��������	     
		case 0x02:
			
							 AD_value = Get_Ad_value( no );     //��ѹֵ
		           ValSet(rxBuf[6],AD_value);
//			        if(ad_num==1) //��ȡ��Ƶ��ת��
//							{	
//								//VFD_Speed=Get_VFD_Speed(adc3_value,100); 
//								VFD_Speed=Get_VFD_Speed_CH((u16*)(adc_par[no].buf), 100, 2);
//								ValSet(rxBuf[6],VFD_Speed);
//							}
//              else if(ad_num==0) //��ȡ�������ڷ�������ѹ��ֵ
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
//							else if(ad_num==2)//��ȡѹ��������ѹ��ֵ
//							{
//								R_Pressure_Measure();
//								ValSet(rxBuf[6],(CDV_INT32S)R_Pressure);
//							}
              //ResParaRequest(rxBuf,rxLen , Para_Buf,Para_len,1);							
		      break;
#endif
		//�ȴ�����
		case 0x03:
			        if(type==0xFF)//�ȴ����ڳ���
							{
							
							}
 
              else if(type==0xFE)//�ȴ����ڱ���
							{	

              }  							
			   
		     break;
		case 0x04	://��ȡAD��ѹ
			         tmp = Get_Read_AD(no);
		         
//			         if(ad_num==0)//�������ڷ�������ѹ
//							 {
//									Present_Pressure_AD = Grubbs(ADC_ConvertedValue,RX_ADC1_DMA);
//								  Present_Pressure_AD =Present_Pressure_AD*fen_ya_val;//������˷�ѹ���裬���Ϸ�ѹϵ����û�ӵ�ʱ��Ĭ����ϵ����1
//									tmp = Present_Pressure_AD*3300/4096;
//							 }
//							 else if(ad_num==2)//ѹ��������������ѹ
//							 {
//									R_Pressure_AD = Grubbs(adc2_value,adc2_value_lens);
//									tmp = R_Pressure_AD*3300/4096;
//							 }
//							 
//							 else if(ad_num==1)//��Ƶ��������ѹ
//							 {
//                   tmp = Grubbs(adc3_value,100)*3300/4096;
//							 }
               if(type==0xFE)//��������
							 {	
                 								 
								 ValSet(rxBuf[6],tmp);
							 }
							 
               else//����������
							 {								 
								  Para_Buf[0]=tmp>>24;
								  Para_Buf[1]=tmp>>16;
								  Para_Buf[2]=tmp>>8;
								  Para_Buf[3]=tmp;
								  ResRequest(arg->buf, arg->len, Para_Buf,4,arg, RC_CRC);
							 }							 
		     break;							
		case 0xFF	://��ѯAD��ѹ
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
//��ȡ����
//no:�豸��
//pflag:�������
//buf:��������
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
  	
	 if(pflag & 0x00000001)//б��ֵ
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Slope>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000002)//��λֵ
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Zero>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000004)//�����Сֵ
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Out_Min>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x000000008)//������ֵ
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Out_Max>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000010)//��ʱб��ֵ
	 {	
		  for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Slope2>>(24-8*j);
			}
			i=i+4;									
	 }	
	 if(pflag & 0x00000020) //��ʱ��λֵ
	 {	
		 for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Zero2>>(24-8*j);
			}
			i=i+4;						
	 }
	 if(pflag & 0x00000040) //ϵ��1
	 {	
		 for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Ad_Coeff1>>(24-8*j);
			}
			i=i+4;						
	 }
	 if(pflag & 0x00000080) //ϵ��2
	 {	
		 for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].Ad_Coeff2>>(24-8*j);
			}
			i=i+4;						
	 }
	 
	if(pflag & 0x00000100)//��ѹϵ��
	{	
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=adc_par[no].fen_ya_value>>(24-8*j);
			}
			i=i+4;
	}	
	return i; //���س���
	 
}	
//******************************************************
//��Ƶ��ת��
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
//  vol=vol*((float)adc_par.Slope[1]/1000);//*б��
//	//vfd_speed=vol*4*5;
//	vfd_speed=vol*10*4;//��Ƶ����ѹ1v=10ת
//	//vfd_speed=vfd_speed*(float)adc_par.Slope[1]/1000-(float)adc_par.Zero[1]/1000;//*б��-��λֵ
//	return vfd_speed;
//}

u16 Get_Ad_value(u8 no)   //��ѹֵ
{
  u16 i;
	u16 Ad_value;
	double sum=0;
	double vol;

	vol = Grubbs(adc_par[no].buf,N);      //�˲�
	vol = (vol -(float)adc_par[no].Zero)/4095*3.3;  
	vol = vol*((float)adc_par[no].Slope/1000*adc_par[no].Ad_Coeff1);
	Ad_value = vol* adc_par[no].Ad_Coeff2;

	return Ad_value; 
}
//0-3��Ӧadc1-4
s32 Get_Read_AD(u8 no)   //��ѹֵ
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
	  tmp = Pressure_AD*3.2641571391360037968675842429995;              //0~3.3V��Ӧ 0~3300
#else
	if(0==no)
		tmp = Pressure_AD*1.494768310911809;
	else
	  tmp = Pressure_AD*3.43878954607978;              //0~3.3V��Ӧ 0~3300
#endif
	return tmp;
}
//ˢ��adc��ֵ��g_adcval
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