#include "user.h"
//////////////////////////////////////////////////////////////////////////////////	 
//��ʢ����
//�޸�����:2014/1/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ���˺�ʢ�������޹�˾
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
extern u8  Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 

//��ʼ�����������.��ʹ��ʱ��		    
// IO��ʼ��
/*************************************�����ķָ��߻���*************************************************/
//void USER_Init(void)
//{
////	u8 i;
//	GPIO_InitTypeDef  GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
//							RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|
//							RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|
//							RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PA~PG�˿�ʱ��
//	//������Ҫ�趨�����������ģʽ
//	//PA
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_15; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;     //��©���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	//PB
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������?
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);							
//	//PC
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; 
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
////	GPIO_Init(GPIOC, &GPIO_InitStructure);			
//  //PD
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOD, &GPIO_InitStructure);			
//	//PE
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOE, &GPIO_InitStructure);	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;   
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //���ó���������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOE, &GPIO_InitStructure);		
//	//PF
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;   
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //���ó���������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOF, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOF, &GPIO_InitStructure);					     //�����趨������ʼ��GPIOF
//	//PG
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
//	GPIO_Init(GPIOG, &GPIO_InitStructure);			

////	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	//�رյ��� �˿�����ӳ��  ʹ�÷���������ʱ�������ô���

////������Ҫȫ�����ߵ�ƽ
//  GPIO_SetBits(GPIOA,GPIO_Pin_All);
//	GPIO_SetBits(GPIOB,GPIO_Pin_All);
//	GPIO_SetBits(GPIOC,GPIO_Pin_All);
//	GPIO_SetBits(GPIOD,GPIO_Pin_All);
//	GPIO_SetBits(GPIOE,GPIO_Pin_All);
//	GPIO_SetBits(GPIOF,GPIO_Pin_All);
//	GPIO_SetBits(GPIOG,GPIO_Pin_All);  

//// 	GPIO_ResetBits(GPIOA,GPIO_Pin_All);
//// 	GPIO_ResetBits(GPIOB,GPIO_Pin_All);
//// 	GPIO_ResetBits(GPIOC,GPIO_Pin_All);
//// 	GPIO_ResetBits(GPIOD,GPIO_Pin_All);
//// 	GPIO_ResetBits(GPIOE,GPIO_Pin_All);
//// 	GPIO_ResetBits(GPIOF,GPIO_Pin_All);
//// 	GPIO_ResetBits(GPIOG,GPIO_Pin_All); 	
///*************************************�����ķָ���**���Ŷ���*************************************************/
//	//�����
//	Led[0].LedPort[0]=(GPIO_TypeDef *) GPIOD_BASE;
//	Led[0].LedIO[0]=(u16)0x0200;//9
//	Led[0].LedPort[1]=(GPIO_TypeDef *) GPIOD_BASE;
//	Led[0].LedIO[1]=(u16)0x0400;//10
//	Led[0].LedPort[2]=(GPIO_TypeDef *) GPIOD_BASE;
//	Led[0].LedIO[2]=(u16)0x0800;//11
//	Led[0].LedPort[3]=(GPIO_TypeDef *) GPIOD_BASE;
//	Led[0].LedIO[3]=(u16)0x1000;//12
//	Led[0].LedPort[4]=(GPIO_TypeDef *) GPIOD_BASE;
//	Led[0].LedIO[4]=(u16)0x2000;//13
//	Led[0].LedPort[5]=(GPIO_TypeDef *) GPIOD_BASE;
//	Led[0].LedIO[5]=(u16)0x4000;//14
//	Led[0].LedPort[6]=(GPIO_TypeDef *) GPIOD_BASE;
//	Led[0].LedIO[6]=(u16)0x8000;//15
//	Led[0].LedPort[7]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[0].LedIO[7]=(u16)0x0004;//2
//	
//	Led[0].LedDuanPort[0]=(GPIO_TypeDef *) GPIOD_BASE;
//	Led[0].LedDuanIO[0]=(u16)0x0100;//8
//	Led[0].LedBitPort[0]=(GPIO_TypeDef *) GPIOB_BASE;
//	Led[0].LedBitIO[0]=(u16)0x8000;//15
//	
//	Led[1].LedPort[0]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[1].LedIO[0]=(u16)0x0001;//0
//	Led[1].LedPort[1]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[1].LedIO[1]=(u16)0x0002;//1
//	Led[1].LedPort[2]=(GPIO_TypeDef *) GPIOE_BASE;
//	Led[1].LedIO[2]=(u16)0x0080;//7
//	Led[1].LedPort[3]=(GPIO_TypeDef *) GPIOE_BASE;
//	Led[1].LedIO[3]=(u16)0x0100;//8
//	Led[1].LedPort[4]=(GPIO_TypeDef *) GPIOE_BASE;
//	Led[1].LedIO[4]=(u16)0x0200;//9
//	Led[1].LedPort[5]=(GPIO_TypeDef *) GPIOE_BASE;
//	Led[1].LedIO[5]=(u16)0x0400;//10
//	Led[1].LedPort[6]=(GPIO_TypeDef *) GPIOE_BASE;
//	Led[1].LedIO[6]=(u16)0x0800;//11
//	Led[1].LedPort[7]=(GPIO_TypeDef *) GPIOE_BASE;
//	Led[1].LedIO[7]=(u16)0x1000;//12
//	
//	Led[1].LedDuanPort[0]=(GPIO_TypeDef *) GPIOE_BASE;
//	Led[1].LedDuanIO[0]=(u16)0x2000;//13
//	Led[1].LedBitPort[0]=(GPIO_TypeDef *) GPIOE_BASE;
//	Led[1].LedBitIO[0]=(u16)0x4000;//14
//	
//	Led[2].LedPort[0]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[2].LedIO[0]=(u16)0x0200;//9
//	Led[2].LedPort[1]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[2].LedIO[1]=(u16)0x0400;//10
//	Led[2].LedPort[2]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[2].LedIO[2]=(u16)0x0800;//11
//	Led[2].LedPort[3]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[2].LedIO[3]=(u16)0x1000;//12
//	Led[2].LedPort[4]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[2].LedIO[4]=(u16)0x2000;//13
//	Led[2].LedPort[5]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[2].LedIO[5]=(u16)0x4000;//14
//	Led[2].LedPort[6]=(GPIO_TypeDef *) GPIOG_BASE;
//	Led[2].LedIO[6]=(u16)0x8000;//15
//	Led[2].LedPort[7]=(GPIO_TypeDef *) GPIOB_BASE;
//	Led[2].LedIO[7]=(u16)0x0008;//3
//	
//	Led[2].LedDuanPort[0]=(GPIO_TypeDef *) GPIOB_BASE;
//	Led[2].LedDuanIO[0]=(u16)0x0020;//5
//	Led[2].LedBitPort[0]=(GPIO_TypeDef *) GPIOB_BASE;
//	Led[2].LedBitIO[0]=(u16)0x0010;//4
//	
//	Led[3].LedPort[0]=(GPIO_TypeDef *) GPIOF_BASE;
//	Led[3].LedIO[0]=(u16)0x0040;//6
//	Led[3].LedPort[1]=(GPIO_TypeDef *) GPIOF_BASE;
//	Led[3].LedIO[1]=(u16)0x0080;//7
//	Led[3].LedPort[2]=(GPIO_TypeDef *) GPIOF_BASE;
//	Led[3].LedIO[2]=(u16)0x0100;//8
//	Led[3].LedPort[3]=(GPIO_TypeDef *) GPIOF_BASE;
//	Led[3].LedIO[3]=(u16)0x0200;//9
//	Led[3].LedPort[4]=(GPIO_TypeDef *) GPIOF_BASE;
//	Led[3].LedIO[4]=(u16)0x0400;//10
//	Led[3].LedPort[5]=(GPIO_TypeDef *) GPIOC_BASE;
//	Led[3].LedIO[5]=(u16)0x0001;//0
//	Led[3].LedPort[6]=(GPIO_TypeDef *) GPIOC_BASE;
//	Led[3].LedIO[6]=(u16)0x0002;//1
//	Led[3].LedPort[7]=(GPIO_TypeDef *) GPIOC_BASE;
//	Led[3].LedIO[7]=(u16)0x0004;//2
//	
//	Led[3].LedDuanPort[0]=(GPIO_TypeDef *) GPIOF_BASE;
//	Led[3].LedDuanIO[0]=(u16)0x0020;//5
//	Led[3].LedBitPort[0]=(GPIO_TypeDef *) GPIOF_BASE;
//	Led[3].LedBitIO[0]=(u16)0x0010;//4

//	//����
//	SWReset[0].SWPort = (GPIO_TypeDef *) GPIOF_BASE;
//	SWReset[0].SWIO =(u16)0x0001;//0
//	SWReset[1].SWPort = (GPIO_TypeDef *) GPIOE_BASE;
//	SWReset[1].SWIO =(u16)0x0008;//3
//	SWReset[2].SWPort = (GPIO_TypeDef *) GPIOF_BASE;
//	SWReset[2].SWIO =(u16)0x0004;//2
//	SWReset[3].SWPort = (GPIO_TypeDef *) GPIOE_BASE;
//	SWReset[3].SWIO =(u16)0x0020;//5
////	
////	
////���Թ�λ
//	gw[0].PulsePort=(GPIO_TypeDef *) GPIOF_BASE;
//	gw[0].PulseIO=(u16)0x0002;//1

//	gw[1].PulsePort=(GPIO_TypeDef *) GPIOE_BASE;
//	gw[1].PulseIO=(u16)0x0010;//4
//	
//	gw[2].PulsePort=(GPIO_TypeDef *) GPIOF_BASE;
//	gw[2].PulseIO=(u16)0x0008;//3
//	
//	gw[3].PulsePort=(GPIO_TypeDef *) GPIOE_BASE;
//	gw[3].PulseIO=(u16)0x0020;//5
////	
////	//����ָʾ��
//	wsyj.RLedPort=(GPIO_TypeDef *) GPIOA_BASE;
//	wsyj.RLedIO=(u16)0x8000;//15
//	wsyj.GLedPort=(GPIO_TypeDef *) GPIOA_BASE;
//	wsyj.GLedIO=(u16)0x1000;//12
//	
///*************************************�����ķָ���**����*************************************************/

//	
//}

void LED_Display( LEDPORT pled, u8 LedOut)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(LedOut & (1<<i))
		{
			LEDH(pled.LedPort[i],pled.LedIO[i]);
		}
		else
		{
			LEDL(pled.LedPort[i],pled.LedIO[i]);
		}
	}
		
}

void LED_show0(u32 Count)
{
	u8 i,j;
	u8 LLEDOUT[8];	
	
   LLEDOUT[7]=Disp_Tab[Count%100000000/10000000];  //ǧ��
	 LLEDOUT[6]=Disp_Tab[Count%10000000/1000000];    //����
	 LLEDOUT[5]=Disp_Tab[Count%1000000/100000];	    //ʮ��
	 LLEDOUT[4]=Disp_Tab[Count%100000/10000];	      //��
	
	 LLEDOUT[3]=Disp_Tab[Count%10000/1000]; //ǧλ
	 LLEDOUT[2]=Disp_Tab[Count%1000/100];   //��λ
	 LLEDOUT[1]=Disp_Tab[Count%100/10];	//ʮλ
	 LLEDOUT[0]=Disp_Tab[Count%10];	 //��λ

	
		
  for(display=0;display<8;display++)

	{	
		switch(display)
		{
			case 0:
			  LEDL(Led[0].LedPort[0],Led[0].LedIO[0]);
			for(i=1;i<8;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			  LEDH(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			  LEDL(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
				break;
			case 1:
			  LEDH(Led[0].LedPort[0],Led[0].LedIO[0]);
			  LEDL(Led[0].LedPort[1],Led[0].LedIO[1]);
			for(i=2;i<8;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			  LEDH(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			  LEDL(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
				 break;
			case 2:
			  LEDH(Led[0].LedPort[0],Led[0].LedIO[0]);
			  LEDH(Led[0].LedPort[1],Led[0].LedIO[1]);
			  LEDL(Led[0].LedPort[2],Led[0].LedIO[2]);
			for(i=3;i<8;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			  LEDH(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			  LEDL(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
				 break;
			case 3:
				for(i=0;i<3;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			  LEDL(Led[0].LedPort[3],Led[0].LedIO[3]);
			for(i=4;i<8;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			  LEDH(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
		    LEDL(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
				 break;
			case 4:
				for(i=0;i<4;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			  LEDL(Led[0].LedPort[4],Led[0].LedIO[4]);
				for(i=5;i<8;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }	 
			  LEDH(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			  LEDL(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			  break;
			case 5:
				for(i=0;i<5;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			  LEDL(Led[0].LedPort[5],Led[0].LedIO[5]);
			  LEDH(Led[0].LedPort[6],Led[0].LedIO[6]);
			  LEDH(Led[0].LedPort[7],Led[0].LedIO[7]);
			
			  LEDH(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			  LEDL(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			
			  break;
			case 6:
				for(i=0;i<6;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			  LEDL(Led[0].LedPort[6],Led[0].LedIO[6]);
			  LEDH(Led[0].LedPort[7],Led[0].LedIO[7]);
			
			  LEDH(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			  LEDL(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			   break;
			case 7:
				for(i=0;i<7;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			  LEDL(Led[0].LedPort[7],Led[0].LedIO[7]);
			
			  LEDH(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			  LEDL(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			   break;
			default:
				for(i=0;i<8;i++)
			  {
				LEDH(Led[0].LedPort[i],Led[0].LedIO[i]);
           }
			
			  LEDH(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
			  LEDL(Led[0].LedBitPort[0],Led[0].LedBitIO[0]);
				break;
		}
		
		LED_Display( Led[0],  LLEDOUT[display]); //����ֵ��Led[]��������
		LEDH(Led[0].LedDuanPort[0],Led[0].LedDuanIO[0]);
		LEDL(Led[0].LedDuanPort[0],Led[0].LedDuanIO[0]);
		
   for(j=0;j<8;j++)
		 {
			 LEDL(Led[0].LedPort[j],Led[0].LedIO[j]);
        }
		LEDH(Led[0].LedDuanPort[0],Led[0].LedDuanIO[0]);
		LEDL(Led[0].LedDuanPort[0],Led[0].LedDuanIO[0]);
		//display++;
	}
	  u8Time++;
		
		}
		
void LED_show1(u32 Count1)
{
	u8 i,j;
	u8 LLEDOUT[8];

   LLEDOUT[7]=Disp_Tab[Count1%100000000/10000000];  //ǧ��
	 LLEDOUT[6]=Disp_Tab[Count1%10000000/1000000];    //����
	 LLEDOUT[5]=Disp_Tab[Count1%1000000/100000];	    //ʮ��
	 LLEDOUT[4]=Disp_Tab[Count1%100000/10000];	      //��
	
	 LLEDOUT[3]=Disp_Tab[Count1%10000/1000]; //ǧλ
	 LLEDOUT[2]=Disp_Tab[Count1%1000/100];   //��λ
	 LLEDOUT[1]=Disp_Tab[Count1%100/10];	//ʮλ
	 LLEDOUT[0]=Disp_Tab[Count1%10];	 //��λ
	
  for(display=0;display<8;display++)

 	{	 
		switch(display)
		{
			case 0:
			  LEDL(Led[1].LedPort[0],Led[1].LedIO[0]);
			for(i=1;i<8;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }
			  LEDH(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			  LEDL(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
				break;
			case 1:
			  LEDH(Led[1].LedPort[0],Led[1].LedIO[0]);
			  LEDL(Led[1].LedPort[1],Led[1].LedIO[1]);
			  for(i=2;i<8;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }
			  LEDH(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			  LEDL(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
				 break;
			case 2:
			  LEDH(Led[1].LedPort[0],Led[1].LedIO[0]);
			  LEDH(Led[1].LedPort[1],Led[1].LedIO[1]);
			  LEDL(Led[1].LedPort[2],Led[1].LedIO[2]);
			for(i=3;i<8;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }
			  LEDH(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			  LEDL(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
				 break;
			case 3:
				for(i=0;i<3;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }
			  LEDL(Led[1].LedPort[3],Led[1].LedIO[3]);
			for(i=4;i<8;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }	
			  LEDH(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
		    LEDL(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
				 break;
			case 4:
				for(i=0;i<4;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }
			  LEDL(Led[1].LedPort[4],Led[1].LedIO[4]);
				for(i=5;i<8;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }	
			  LEDH(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			  LEDL(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			  break;
			case 5:
				for(i=0;i<5;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }
			  LEDL(Led[1].LedPort[5],Led[1].LedIO[5]);
			  LEDH(Led[1].LedPort[6],Led[1].LedIO[6]);
			  LEDH(Led[1].LedPort[7],Led[1].LedIO[7]);
			
			  LEDH(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			  LEDL(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			
			  break;
			case 6:
				for(i=0;i<6;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }

			  LEDL(Led[1].LedPort[6],Led[1].LedIO[6]);
			  LEDH(Led[1].LedPort[7],Led[1].LedIO[7]);
			
			  LEDH(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			  LEDL(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			   break;
			case 7:
				for(i=0;i<7;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }
			  LEDL(Led[1].LedPort[7],Led[1].LedIO[7]);
			
			  LEDH(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			  LEDL(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			   break;
			default:
				for(i=0;i<8;i++)
			  {
				LEDH(Led[1].LedPort[i],Led[1].LedIO[i]);
           }
			
			  LEDH(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
			  LEDL(Led[1].LedBitPort[0],Led[1].LedBitIO[0]);
				break;
		}
		 LED_Display( Led[1],  LLEDOUT[display]); //����ֵ��Led[]��������
		LEDH(Led[1].LedDuanPort[0],Led[1].LedDuanIO[0]);
		LEDL(Led[1].LedDuanPort[0],Led[1].LedDuanIO[0]);
		for(j=0;j<8;j++)
		{
			LEDL(Led[1].LedPort[j],Led[1].LedIO[j]);
     }

		LEDH(Led[1].LedDuanPort[0],Led[1].LedDuanIO[0]);
		LEDL(Led[1].LedDuanPort[0],Led[1].LedDuanIO[0]);
		//display++;
	}
	  u8Time++;
		
		}
	
		
void LED_show2(u32 Count2)
{
	u8 i,j;
	u8 LLEDOUT[8];

   LLEDOUT[7]=Disp_Tab[Count2%100000000/10000000];  //ǧ��
	 LLEDOUT[6]=Disp_Tab[Count2%10000000/1000000];    //����
	 LLEDOUT[5]=Disp_Tab[Count2%1000000/100000];	    //ʮ��
	 LLEDOUT[4]=Disp_Tab[Count2%100000/10000];	      //��
	
	 LLEDOUT[3]=Disp_Tab[Count2%10000/1000]; //ǧλ
	 LLEDOUT[2]=Disp_Tab[Count2%1000/100];   //��λ
	 LLEDOUT[1]=Disp_Tab[Count2%100/10];	//ʮλ
	 LLEDOUT[0]=Disp_Tab[Count2%10];	 //��λ
	
  for(display=0;display<8;display++)

	{	
		switch(display)
		{
			case 0:
			  LEDL(Led[2].LedPort[0],Led[2].LedIO[0]);
			for(i=1;i<8;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDH(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  LEDL(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
				break;
			case 1:
			  LEDH(Led[2].LedPort[0],Led[2].LedIO[0]);
			  LEDL(Led[2].LedPort[1],Led[2].LedIO[1]);
			for(i=2;i<8;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDH(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  LEDL(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
				 break;
			case 2:
			  LEDH(Led[2].LedPort[0],Led[2].LedIO[0]);
			  LEDH(Led[2].LedPort[1],Led[2].LedIO[1]);
			  LEDL(Led[2].LedPort[2],Led[2].LedIO[2]);
			for(i=3;i<8;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDH(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  LEDL(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
				 break;
			case 3:
				for(i=0;i<3;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDL(Led[2].LedPort[3],Led[2].LedIO[3]);
			for(i=4;i<8;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDH(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  LEDL(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
				 break;
			case 4:
				for(i=0;i<4;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDL(Led[2].LedPort[4],Led[2].LedIO[4]);
			for(i=5;i<8;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDH(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  LEDL(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  break;
			case 5:
				for(i=0;i<5;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDL(Led[2].LedPort[5],Led[2].LedIO[5]);
			  LEDH(Led[2].LedPort[6],Led[2].LedIO[6]);
			  LEDH(Led[2].LedPort[7],Led[2].LedIO[7]);
			  LEDH(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  LEDL(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			
			  break;
			case 6:
				for(i=0;i<6;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDL(Led[2].LedPort[6],Led[2].LedIO[6]);
			  LEDH(Led[2].LedPort[7],Led[2].LedIO[7]);
			  LEDH(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  LEDL(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			   break;
			case 7:
				for(i=0;i<7;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDL(Led[2].LedPort[7],Led[2].LedIO[7]);
			  LEDH(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  LEDL(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			   break;
			default:
				for(i=0;i<8;i++)
			  {
				LEDH(Led[2].LedPort[i],Led[2].LedIO[i]);
           }
			  LEDH(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
			  LEDL(Led[2].LedBitPort[0],Led[2].LedBitIO[0]);
				break;
		}
		 LED_Display( Led[2],  LLEDOUT[display]); //����ֵ��Led[]��������

		LEDH(Led[2].LedDuanPort[0],Led[2].LedDuanIO[0]);
		LEDL(Led[2].LedDuanPort[0],Led[2].LedDuanIO[0]);
		
	for(j=0;j<8;j++)
		{
			LEDL(Led[2].LedPort[j],Led[2].LedIO[j]);
     }
		LEDH(Led[2].LedDuanPort[0],Led[2].LedDuanIO[0]);
		LEDL(Led[2].LedDuanPort[0],Led[2].LedDuanIO[0]);
		//display++;
	  }
	  u8Time++;
 }	
void LED_show3(u32 Count3)
{
	u8 i,j;
	u8 LLEDOUT[8];

   LLEDOUT[7]=Disp_Tab[Count3%100000000/10000000];  //ǧ��
	 LLEDOUT[6]=Disp_Tab[Count3%10000000/1000000];    //����
	 LLEDOUT[5]=Disp_Tab[Count3%1000000/100000];	    //ʮ��
	 LLEDOUT[4]=Disp_Tab[Count3%100000/10000];	      //��
	
	 LLEDOUT[3]=Disp_Tab[Count3%10000/1000]; //ǧλ
	 LLEDOUT[2]=Disp_Tab[Count3%1000/100];   //��λ
	 LLEDOUT[1]=Disp_Tab[Count3%100/10];	//ʮλ
	 LLEDOUT[0]=Disp_Tab[Count3%10];	 //��λ
	
  for(display=0;display<8;display++)

	{	
		switch(display)
		{
			case 0:
			  LEDL(Led[3].LedPort[0],Led[3].LedIO[0]);
			for(i=1;i<8;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDH(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  LEDL(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
				break;
			case 1:
			  LEDH(Led[3].LedPort[0],Led[3].LedIO[0]);
			  LEDL(Led[3].LedPort[1],Led[3].LedIO[1]);
			for(i=2;i<8;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDH(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  LEDL(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
				 break;
			case 2:
				for(i=0;i<2;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDL(Led[3].LedPort[2],Led[3].LedIO[2]);
				for(i=3;i<8;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDH(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  LEDL(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
				 break;
			case 3:
				for(i=0;i<3;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDL(Led[3].LedPort[3],Led[3].LedIO[3]);
				for(i=4;i<8;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDH(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  LEDL(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
				 break;
			case 4:
				for(i=0;i<4;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDL(Led[3].LedPort[4],Led[3].LedIO[4]);
			  for(i=5;i<8;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDH(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  LEDL(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  break;
			case 5:
				for(i=0;i<5;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDL(Led[3].LedPort[5],Led[3].LedIO[5]);
			  LEDH(Led[3].LedPort[6],Led[3].LedIO[6]);
			  LEDH(Led[3].LedPort[7],Led[3].LedIO[7]);
			  LEDH(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  LEDL(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			
			  break;
			case 6:
				for(i=0;i<6;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDL(Led[3].LedPort[6],Led[3].LedIO[6]);
			  LEDH(Led[3].LedPort[7],Led[3].LedIO[7]);
			  LEDH(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  LEDL(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			   break;
			case 7:
				for(i=0;i<7;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDL(Led[3].LedPort[7],Led[3].LedIO[7]);
			  LEDH(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  LEDL(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			   break;
			default:
				for(i=0;i<8;i++)
			  {
				LEDH(Led[3].LedPort[i],Led[3].LedIO[i]);
           }
			  LEDH(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
			  LEDL(Led[3].LedBitPort[0],Led[3].LedBitIO[0]);
				break;
		}
		 LED_Display( Led[3],  LLEDOUT[display]); //����ֵ��Led[]��������
		LEDH(Led[3].LedDuanPort[0],Led[3].LedDuanIO[0]);
		LEDL(Led[3].LedDuanPort[0],Led[3].LedDuanIO[0]);
	for(j=0;j<8;j++)
		{
			LEDL(Led[3].LedPort[j],Led[3].LedIO[j]);
     }
		LEDH(Led[3].LedDuanPort[0],Led[3].LedDuanIO[0]);
		LEDL(Led[3].LedDuanPort[0],Led[3].LedDuanIO[0]);
		//display++;
	}
	  u8Time++;
		
		}
	
	


	



