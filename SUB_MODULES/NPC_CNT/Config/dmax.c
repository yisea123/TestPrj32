/**
  ******************************************************************************
  * @file    /dmax.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-4-1
  * @brief   a package of dmax
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */

	#include "dmax.h"

////DMAx�ĸ�ͨ������
////����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
////�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
////DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
////chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
////par:�����ַ
////mar:�洢����ַ
////ndtr:���ݴ�����  
//void DMA_Config(DMA_Channel_TypeDef *DMA_Channelx,u32 chx,u32 par,u32 mar,u16 ndtr)
//{ 
// 
//	DMA_InitTypeDef  DMA_InitStructure;
//	
//	if((u32)DMA_Channelx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
//	{
//	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
//		
//	}else 
//	{
//	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
//	}
//  DMA_DeInit(DMA_Channelx);
//	
//	while (DMA_GetCmdStatus(DMA_Channelx) != DISABLE){}//�ȴ�DMA������ 
//	
//  /* ���� DMA Stream */
//  //DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
//  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
//  DMA_InitStructure.DMA_MemoryBaseAddr = mar;//DMA �洢��0��ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
//  DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
//  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
//  DMA_Init(DMA_Channelx, &DMA_InitStructure);//��ʼ��DMA Stream
//	

//} 

#if defined (STM32F10X_HD)
//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Channel0~7/DMA2_Channel0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
void DMA_ConfigDir(DMA_Channel_TypeDef *DMA_Channelx,u32 par,u32 mar,u16 ndtr,u32 dir)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Channelx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Channelx);
	
	//while (DMA_GetCmdStatus(DMA_Channelx) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  //DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
  //DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = dir;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�е����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Channelx, &DMA_InitStructure);//��ʼ��DMA Stream
	

} 
//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void DMA_Enable(DMA_Channel_TypeDef *DMA_Channelx,u32 mar,u16 ndtr)
{
 
	DMA_Cmd(DMA_Channelx, DISABLE);                      //�ر�DMA���� 
	
	DMA_Channelx->CNDTR = ndtr;                          // �����ڴ泤��

  // DMA_Channelx->CPAR = 

  DMA_Channelx->CMAR = mar;                            // �����ڴ��ַ
	
	//while(DMA_GetCmdStatus(DMA_Channelx) != DISABLE);
//	if(DMA_GetCmdStatus(DMA_Streamx) != DISABLE){return 0;}	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(DMA_Channelx,ndtr);          //���ݴ�����  
 
	DMA_Cmd(DMA_Channelx, ENABLE);                      //����DMA���� 
	
	//return 1;
}
#elif defined (STM32F40_41xxx)
// ��dma�м�������ndtr - 1��
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u32 mar/*�ڴ��ַ*/,u16 ndtr/*�ڴ泤��*/)
{
	DMA_Cmd(DMA_Streamx, DISABLE);  // ���֮ǰ��enable�������dma tc�¼�                   //�ر�DMA���� 
	while(DMA_GetCmdStatus(DMA_Streamx) != DISABLE){};
//	if(DMA_GetCmdStatus(DMA_Streamx) != DISABLE){return ;}	//ȷ��DMA���Ա�����  
  DMA_MemoryTargetConfig(DMA_Streamx,mar,DMA_Memory_0);
	
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
	
}

void DMA_ConfigDir(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par/*�����ַ*/,u32 mar/*�ڴ��ַ*/,u16 ndtr/*�ڴ��ֽڳ��ȣ����ó��ֽ�*/,u32 dir)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = dir;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
	
	
}
void DMA_ConfigDir16(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par/*�����ַ*/,u32 mar/*�ڴ��ַ*/,u16 ndtr/*�ڴ��ֽڳ��ȣ����ó��ֽ�*/,u32 dir,u32 mode)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = dir;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݳ���:16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�洢�����ݳ���:16λ
  DMA_InitStructure.DMA_Mode = mode;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream
	
}
#endif


