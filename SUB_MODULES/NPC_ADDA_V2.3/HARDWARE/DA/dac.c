#include "dac.h"
 	
// DAC_STRUCT dac_par[Dac_no];//dac�����ṹ

CDV_INT32S  transducer_speed;//��Ƶ��ת��
CDV_INT32S  transducer_time;//��Ƶ������ʱ��
float transducer_vol;//��Ƶ��ת�ٶ�Ӧ��ѹֵ


CDV_INT32S  transducer1_speed;//��Ƶ1��ת��
CDV_INT32S  transducer1_time;//��Ƶ1������ʱ��
float transducer1_vol;//��Ƶ��1ת�ٶ�Ӧ��ѹֵ

u16 transducer2_speed;//��Ƶ2��ת��
u8 transducer2_time;//��Ƶ2������ʱ��
float transducer2_vol;//��Ƶ��2ת�ٶ�Ӧ��ѹֵ
//DACͨ��1�����ʼ��

void Dac1_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //ʹ��PORTAͨ��ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				  // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 �����
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DAC1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ

}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
//	double temp=vol;
//	temp/=1000;
//	temp=temp*4095/3.3;
//	DAC_SetChannel1Data(DAC_Align_12b_R,temp); //12λ�Ҷ������ݸ�ʽ����DACֵ
	if(vol>4095) 
		vol=4095;
	DAC_SetChannel1Data(DAC_Align_12b_R,vol);	 //12λ�Ҷ������ݸ�ʽ����DACֵ
			
}

//DACͨ��2�����ʼ��
//void Dac2_Init(void)
//{  
//  GPIO_InitTypeDef  GPIO_InitStructure;
//	DAC_InitTypeDef DAC_InitType;
//	
// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��
//	   
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
//// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

////	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
////	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
////	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
////	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
////  DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��2

////	DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DACͨ��2
//  
////  DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
//}

void Dac2_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //ʹ��PORTAͨ��ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);  //PA.5 �����
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC2�������ر� BOFF1=1
  DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��2

	DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DAC2
  
  DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ

}

//����ͨ��2�����ѹ
//vol:0~3300,����0~3.3V
void Dac2_Set_Vol(u16 vol)
{
//	double temp=vol;
//	temp /=1000;
//	temp = temp*4095/3.3;
//	DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
	if(vol > 4095 )
		vol = 4095;
	DAC_SetChannel2Data(DAC_Align_12b_R,vol);//12λ�Ҷ������ݸ�ʽ����DACֵ
	
}

//******************************************************
//�����ѹֵ
//dac_num:dacͨ��1-4
//vol�������ѹ
//******************************************************
void Dac_set_Voltge( CDV_INT08U dac_num, float vol)
{
	double temp=vol;
	u16 *buf;	
  u16 lens;	
	u16 adc_val;
	u16 i;
	if(temp>4095)
		temp = 4095;
	switch (dac_num)
	{
	  case 0x00:
			 Dac1_Set_Vol(temp);
		   MODBUS_REG_ADDR(REG_DA1) = temp;		 
			break;
		case 0x01:
			 Dac2_Set_Vol(temp);
		   MODBUS_REG_ADDR(REG_DA2) = temp;
			break;
		case 0x02:
			 WriteToDAC1_7512(temp);
		   MODBUS_REG_ADDR(REG_DA3) = temp;
			break;
		case 0x03:
			 WriteToDAC2_7512(temp);
			 MODBUS_REG_ADDR(REG_DA4) = temp;
			break;
		case 0x04:
			 WriteToDAC3_7512(temp);
		   MODBUS_REG_ADDR(REG_DA5) = temp;
			break;
		case 0x05:
			 WriteToDAC4_7512(temp);
		   MODBUS_REG_ADDR(REG_DA6) = temp;
			break;
		default:
			break;		
	}

}

//void DA_ReadRegister(void)
//{
//		MODBUS_REG_ADDR(REG_DA1) = Get_Adc_Voltge(1);
//		MODBUS_REG_ADDR(REG_DA2) = Get_Adc_Voltge(2);
//		MODBUS_REG_ADDR(REG_DA3) = Get_Adc_Voltge(3);
//		MODBUS_REG_ADDR(REG_DA4) = Get_Adc_Voltge(4);
//	  MODBUS_REG_ADDR(REG_DA5) = Get_Adc_Voltge(5);
//		MODBUS_REG_ADDR(REG_DA6) = Get_Adc_Voltge(6);
//  }
//*******************************************
//CDV dac ����
//start_val����ʼ��ѹ mv
//end_val��Ŀ���ѹ mv
//run_time :����ʱ��ms
//*******************************************
RET_STATUS CDV_DacRun(u32 start_val,u32 end_val,u32 run_time)
{
	u8 flag;
	u32 i;
	float val;//��ֵ
	u32 dotimes;
	float pval;
	float run_val=0;
	if(end_val>start_val)
	{
		val=end_val-start_val;
		flag=1;
	}	
	else
	{
		val=start_val-end_val;
		flag=0;
	}	
	dotimes=run_time/100;//ÿ100msһ��
	pval=val/dotimes;
	run_val=start_val+pval;
	for(i=0;i<dotimes;i++)
	{
		Dac1_Set_Vol(run_val);
		delay_ms(100);
		if(flag==1)
		{	
			run_val=run_val+pval;
		}
    else
    {
			run_val=run_val+pval;
		} 			
	}
	return OPT_SUCCESS;
}

//*******************************************
//CDV dac ֹͣ
//*******************************************
RET_STATUS CDV_DacStop()
{
	return OPT_SUCCESS;
}

//*******************************************
//CDV dac �ȴ�
//*******************************************
RET_STATUS CDV_DacWait()
{
	return OPT_SUCCESS;
}

//*******************************************
//CDV dac
//
//
//
//

//void  Dac_Set_Vol (DAC_STRUCT *dac_par,u8 no)
//{
//    transducer_vol=(float)transducer_speed/33*1000; //10ת/V��DA����Ŵ�3.3����3.3*10=33����ѹ��λmv
//	  transducer_vol=transducer_vol-(float)dac_par[no].Zero;
//	  transducer_vol=transducer_vol*((float)dac_par[no].Slope/10000); //*��б��
//	
//	  Dac_set_Voltge ( dac_par[no].Dac_num ,transducer_vol);
//}

//u16  Get_Da_value(u8 no)
//{
//  float value = (dac_par[no].vol * 10000 / (float)dac_par[no].Slope + (float)dac_par[no].Zero) * 33 / 1000;
//	return value;
//}
void	DAC_SETOUT(void)
	{
			u8 i;
		 for(i=0;i<6;i++)
			Dac_set_Voltge(i,1000);
	}


