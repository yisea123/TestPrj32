/*FPGA�����������*/
#include "FPGA.h"
//#include "w25qxx.h" 
//

#if ENABLE_FPGA_DOWN
/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void FPGADelay(__IO uint32_t nCount)
{
  while(nCount--)
  {

  }
}
/**
  * @brief  ����fpga
    
  * @param  cfg     ��4λ����ʾ4fpga�����÷�ʽ���ӵ�λ����λ��1~4������1��Ӧ���ó�motor,0���ó�IO
            
      
  * @retval void
	 
	* @note   
  */

void FpgaInit(u8 cfg)
{
	CDV_INT08U  i;
	if(Fpga_Config(5, FPGA_MOTOR))//������������
		Fpga_Down(5, FPGA_MOTOR);
//	for( i = 1; i<= CDV_FPGA_NUM ; i++) {
//		if(cfg & (0x01 << (i-1)))
//		{
//			Fpga_Config(i, FPGA_MOTOR);//������������
//			Fpga_Down(i, FPGA_MOTOR);
//		} else {
//			Fpga_Config(i, FPGA_IO);//�ڶ���
//			Fpga_Down(i, FPGA_IO);
//		}
//	}
}
//*******************************************************
//��һ��FPGA����ʧ��
//*******************************************************
void FPGA1_COFIG_DISABLE(void)//INT_B
{
	#if CDV_FPGA_NUM > 0
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ�����ʱ��
	  
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	  
	  GPIO_ResetBits(GPIOD,GPIO_Pin_7);//���õ͵�ƽ�ӳ�����
	#endif
}
//*******************************************************
//��һ��FPGA����ʹ��
//*******************************************************
void FPGA1_COFIG_ENABLE(void)
{
	#if CDV_FPGA_NUM > 0
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ�����ʱ��
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	#endif
}
//*******************************************************
//�ڶ���FPGA����ʧ��
//*******************************************************
void FPGA2_COFIG_DISABLE(void)
{
	#if CDV_FPGA_NUM > 1
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ�����ʱ��
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	  GPIO_ResetBits(GPIOG,GPIO_Pin_10);//���õ͵�ƽ�ӳ�����
	  //FPGA2_PROG_B_ENABLE;
	#endif
}
//*******************************************************
//�ڶ���FPGA����ʹ��
//*******************************************************
void FPGA2_COFIG_ENABLE(void)
{
	#if CDV_FPGA_NUM > 1
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ�����ʱ��
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
}
//*******************************************************
//������FPGA����ʧ��
//*******************************************************
void FPGA3_COFIG_DISABLE(void)
{
#if CDV_FPGA_NUM > 2
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ�����ʱ��
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	  GPIO_ResetBits(GPIOD,GPIO_Pin_8);//���õ͵�ƽ�ӳ�����
#endif
}
//*******************************************************
//������FPGA����ʹ��
//*******************************************************
void FPGA3_COFIG_ENABLE(void)
{
#if CDV_FPGA_NUM > 2
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ�����ʱ��
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
}
//*******************************************************
//���Ŀ�FPGA����ʧ��
//*******************************************************
void FPGA4_COFIG_DISABLE(void)
{
#if CDV_FPGA_NUM > 3
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//ʹ�����ʱ��
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��������
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	  GPIO_ResetBits(GPIOH,GPIO_Pin_6);//���õ͵�ƽ�ӳ�����
#endif
}
//*******************************************************
//���Ŀ�FPGA����ʹ��
//*******************************************************
void FPGA4_COFIG_ENABLE(void)
{
#if CDV_FPGA_NUM > 3
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//ʹ�����ʱ��
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		GPIO_Init(GPIOH, &GPIO_InitStructure);
#endif
}
//**********************************************************
//FPGA1������������
//FPGA1_PROG_B=PE8���
//FPGA_CCLK=PB3���
//FPGA_DIN=PB5���
//FPGA1_INT_B=PE7����
//FPGA1_DONE=PH8����
//PB4���ó�FPGA��λ����
//**********************************************************
void Fpga1_Config_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOD, ENABLE);//ʹ�����ʱ��

		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//FPGA_CCLK
		GPIO_Init(GPIOG, &GPIO_InitStructure); 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//FPGA_DIN
		GPIO_Init(GPIOG, &GPIO_InitStructure); 

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//FPGA1_PROG_B
		GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//��λFPGA
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//FPGA1_INT_B
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//DONE
		GPIO_Init(GPIOG, &GPIO_InitStructure);
		
		FPGA1_PROG_B_ENABLE;
		FPGA_RESET_DISABLE;//����λFPGA
}
//**********************************************************
//FPGA2������������
//FPGA2_PROG_B=PH9���
//FPGA_CCLK=PB3���
//FPGA_DIN=PB5���
//FPGA2_INT_B=PB14���루����͵�ƽ�ӳ����أ�
//FPGA2_DONE=PB6����
//PB4���ó�FPGA��λ����
//**********************************************************
void Fpga2_Config_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH , ENABLE);//ʹ�����ʱ��

		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//FPGA_CCLK
		GPIO_Init(GPIOG, &GPIO_InitStructure); 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//FPGA_DIN
		GPIO_Init(GPIOG, &GPIO_InitStructure); 

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PROG_B
		GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//��λFPGA
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��������
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�����ٶ�
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//�������������Ĵ���
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//INT_B
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//DONE
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	
		
		FPGA2_PROG_B_ENABLE;
		FPGA_RESET_DISABLE;//����λFPGA
}

//**********************************************************
//FPGA3������������
//FPGA3_PROG_B=PB13���
//FPGA3_INT_B=PD8����
//FPGA3_DONE=PI4����
//PB4���ó�FPGA��λ����
//**********************************************************
void Fpga3_Config_Init(void)
{
    
		
//		FPGA3_PROG_B_ENABLE;
//		FPGA_RESET_DISABLE;//����λFPGA
}
//**********************************************************
//FPGA4������������
//FPGA4_PROG_B=PH7���
//FPGA4_INT_B=PH6����
//FPGA4_DONE=PB7����
//PB4���ó�FPGA��λ����
//**********************************************************
void Fpga4_Config_Init(void)
{
    
		
//		FPGA4_PROG_B_ENABLE;
//		FPGA_RESET_DISABLE;//����λFPGA
}


void Fpga_Config_Init(void)
{
	#if ENABLE_FPGA_DOWN
		
		Fpga1_Config_Init();
		Fpga2_Config_Init();
		Fpga3_Config_Init();
		Fpga4_Config_Init();
	#endif
	
}
//**********************************************************
//��ȡ����FPGA1 DONE��ƽ
//**********************************************************
u8 Fpga1_Read_Done(void)
{
	u8 dat;
	dat=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
	return dat;
}	
//**********************************************************
//��ȡ����FPGA1 INT��ƽ
//**********************************************************
u8 Fpga1_Read_INT(void)
{
	u8 dat;
	dat=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7);
	return dat;
}
//**********************************************************
//��ȡ����FPGA2 DONE��ƽ
//**********************************************************
u8 Fpga2_Read_Done(void)
{
	u8 dat;
	dat=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0);
	return dat;
}	
//**********************************************************
//��ȡ����FPGA2 INT��ƽ
//**********************************************************
u8 Fpga2_Read_INT(void)
{
	u8 dat;
	dat=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10);
	return dat;
}
//**********************************************************
//��ȡ����FPGA3 DONE��ƽ
//**********************************************************
u8 Fpga3_Read_Done(void)
{
	return 0;
}	
//**********************************************************
//��ȡ����FPGA3 INT��ƽ
//**********************************************************
u8 Fpga3_Read_INT(void)
{
	return 0;
}
//**********************************************************
//��ȡ����FPGA4 DONE��ƽ
//**********************************************************
u8 Fpga4_Read_Done(void)
{
	return 0;
}	
//**********************************************************
//��ȡ����FPGA4 INT��ƽ
//**********************************************************
u8 Fpga4_Read_INT(void)
{
	return 0;
}
//********************************************************
void delay_ns(u32 cnt)
{
	u32 j;
	cnt=cnt/6;
	for(j=0;j<cnt;j++);
}
//**********************************************************
//FPGA���غ���1�����ô������ARMƬ��Flash
//Buf:�������ݴ洢����
//Code_Size:�����С
//���óɹ�����1������ʧ�ܷ���0
//**********************************************************
//u8 Fpga1_Config(u8 *Buf,u32 Code_Size)
//{
//	 u32 i=0;
//	 u8 j;
//	 u8 Done=0;
//	 u8 dat;//����Buf
//	 u8 num=0;//�ظ����ô���
//	 LED1 = 1;//GPIO_ResetBits(GPIOH,GPIO_Pin_12);
//	
//	 while(!Done)
//	 {
//			 FPGA_CCLK_DISABLE;
//			 FPGA1_PROG_B_ENABLE;
//			 delay_us(300);
//			 FPGA1_PROG_B_DISABLE;
//			 delay_us(1000);
//			 FPGA1_PROG_B_ENABLE;
//			// delay_us(200);
//			 while(!Fpga1_Read_INT()); 
//			 delay_us(5);
//			 //dat=Buf[i];
//			 for(i=0;i<Code_Size;i++)
//			 {
//					dat=Buf[i];
//					for(j=0;j<8;j++)
//					{
//						if((dat & 0x80)==0)
//						{
//							 FPGA_DIN_DISABLE;
//							 GPIO_SetBits(GPIOH,GPIO_Pin_11);
//						}
//						if((dat & 0x80)==0x80)
//						{	
//							FPGA_DIN_ENABLE;
//							GPIO_ResetBits(GPIOH,GPIO_Pin_11);
//						}	
//						FPGA_CCLK_ENABLE;
//						delay_us(3);
//						FPGA_CCLK_DISABLE;
//						delay_us(3);
//						dat=dat << 1;					
//				 }
//				 if(Fpga1_Read_INT()==0)//���ó����������
//					 break;  
//			}			
//			for(i=0;i<100;i++)//����������ɺ���һ������ȴ�ϵͳ����
//			{
//				FPGA_CCLK_ENABLE;
//				delay_us(50);
//				FPGA_CCLK_DISABLE;
//				delay_us(50);
//				if(Fpga1_Read_Done()==1)
//				{
//					Done=1;				
//					break;
//				} 				
//			}
//			for(i=0;i<16;i++)//����������ɺ���һ������ȴ�ϵͳ����
//			{
//				FPGA_CCLK_ENABLE;
//				delay_us(50);
//				FPGA_CCLK_DISABLE;
//				delay_us(50);				
//			}			
//      num++;
//      if(num>5)//����5�ζ�û�ɹ����˳�
//      {
//			   return 0;
//			}				
//	 }	
//   return 1;	 
//}
//**********************************************************
//FPGA���غ���2
//���ⲿFlash��ȡ�����ļ�
//FPGA_Num:�ڼ���FPGA(ȡֵ1-5,5��ʾ4��FPGAͬʱ����)
//
//**********************************************************
u8 Fpga_Config(u8 FPGA_Num,u32 FPGA_CODE_ADD)
{
	 u32 i=0;
//	 u8 j;
//	 u8 Done=0;
//	 u8 dat;//����Buf
//	 u8 num=0;//�ظ����ô���
//	 while(!Done)
//	 {
//			 FPGA_CCLK_DISABLE;
		   switch(FPGA_Num)
		   {
					case 1: 	
						       FPGA2_COFIG_DISABLE();
//					         FPGA3_COFIG_DISABLE();
//					         FPGA4_COFIG_DISABLE();
									 FPGA2_PROG_B_DISABLE;
									 FPGA3_PROG_B_DISABLE;
									 FPGA4_PROG_B_DISABLE;
					         FPGA1_COFIG_ENABLE();
									 FPGA1_PROG_B_ENABLE;
									 delay_ns(300000);
									 FPGA1_PROG_B_DISABLE;
									  delay_ns(1000000);
									 FPGA1_PROG_B_ENABLE;
									 delay_ns(1000);  
								for(i=0;i<100;i++)
								{
									if(Fpga1_Read_INT())
									{
										break;
									}
									delay_ns(5000);
								}
						      break;
					case 2:  
						       FPGA1_COFIG_DISABLE();
//					         FPGA3_COFIG_DISABLE();
//					         FPGA4_COFIG_DISABLE();
					         //
									 FPGA1_PROG_B_DISABLE;
					         FPGA3_PROG_B_DISABLE;
					         FPGA4_PROG_B_DISABLE;
					         //
						       FPGA2_COFIG_ENABLE();
					         FPGA2_PROG_B_ENABLE;
									 delay_ns(300000);
									 FPGA2_PROG_B_DISABLE;
									 delay_ns(1000000);
									 FPGA2_PROG_B_ENABLE;
									 delay_ns(1000);
								if(CDV_FPGA_NUM>1)
								{
									for(i=0;i<100;i++)
									{
										if(Fpga2_Read_INT())
										{
											break;
										}
										delay_ns(5000);
									}
								}
//					         while((!Fpga2_Read_INT())&&(CDV_FPGA_NUM>1));	 
//										delay_ns(5000);
						      break;
					case 3:  
						       FPGA1_COFIG_DISABLE();
					         FPGA2_COFIG_DISABLE();
					         FPGA4_COFIG_DISABLE();
							 
									 FPGA1_PROG_B_DISABLE;
					         FPGA2_PROG_B_DISABLE;
					         FPGA4_PROG_B_DISABLE;
					
					         FPGA3_COFIG_ENABLE();
					         FPGA3_PROG_B_ENABLE;
									 delay_ns(300000);
									 FPGA3_PROG_B_DISABLE;
									 delay_ns(1000000);
									 FPGA3_PROG_B_ENABLE;
									 delay_ns(1000);
					         while((!Fpga3_Read_INT())&&(CDV_FPGA_NUM>2));	 
										delay_ns(5000);
						      break;
					case 4:
						       FPGA1_COFIG_DISABLE();
					         FPGA2_COFIG_DISABLE();
					         FPGA3_COFIG_DISABLE();
							 
									 FPGA1_PROG_B_DISABLE;
					         FPGA2_PROG_B_DISABLE;
									 FPGA3_PROG_B_DISABLE;
					
					         FPGA4_COFIG_ENABLE();
					         FPGA4_PROG_B_ENABLE;
									 delay_ns(300000);
									 FPGA4_PROG_B_DISABLE;
									 delay_us(1000000);
									 FPGA4_PROG_B_ENABLE;
									 delay_ns(1000);
					         while((!Fpga4_Read_INT())&&(CDV_FPGA_NUM>3));	 
										delay_ns(5000);
									break;
					case 5:
						       FPGA1_COFIG_ENABLE();
						       FPGA2_COFIG_ENABLE();
					         FPGA3_COFIG_ENABLE();
						       FPGA4_COFIG_ENABLE();
					         FPGA1_PROG_B_ENABLE;
					         FPGA2_PROG_B_ENABLE;
					         FPGA3_PROG_B_ENABLE;
					         FPGA4_PROG_B_ENABLE;
									 delay_ns(300000);
					         FPGA1_PROG_B_DISABLE;
									 FPGA2_PROG_B_DISABLE;
					         FPGA3_PROG_B_DISABLE;
									 FPGA4_PROG_B_DISABLE;
									 delay_ns(1000000);
					         FPGA1_PROG_B_ENABLE;
									 FPGA2_PROG_B_ENABLE;
					         FPGA3_PROG_B_ENABLE;
									 FPGA4_PROG_B_ENABLE;
									 delay_ns(1000);
//					         while(!Fpga1_Read_INT());	 
//										delay_ns(5000);
								for(i=0;i<100;i++)
								{
									if(Fpga1_Read_INT()&&Fpga2_Read_INT())
									{
										break;
									}
									delay_ns(5000);
								}
						      break;
					default:
						      break;
       }
			 if(i>=100)
				 return 0;
//			 for(i=0;i<340884;i++)
//			 {
//					
//				 //W25QXX_Read(&dat,i+FPGA_CODE_ADD,1);				 
//				  SPI_Flash_Read(&dat, i+FPGA_CODE_ADD, 1);//��ȡ
//					for(j=0;j<8;j++)
//					{
//						if((dat & 0x80)==0)
//						{
//							 FPGA_DIN_DISABLE;
//							 GPIO_SetBits(GPIOH,GPIO_Pin_11);
//						}
//						if((dat & 0x80)==0x80)
//						{	
//							FPGA_DIN_ENABLE;
//							GPIO_ResetBits(GPIOH,GPIO_Pin_11);
//						}	
//						FPGA_CCLK_ENABLE;
//						//delay_us(1);
//						delay_ns(200);
//						FPGA_CCLK_DISABLE;
//						//delay_us(1);
//					  delay_ns(200);
//						dat=dat << 1;					
//				 }
//				 if(FPGA_Num==1)
//				 {	 
//				   if(Fpga1_Read_INT()==0)//���ó����������
//					  break;
//         }
//         else if(FPGA_Num==2)
//         {
//						if(Fpga2_Read_INT()==0)//���ó����������
//					    break;
//				 }
//         else if(FPGA_Num==3)
//         {
//						if(Fpga3_Read_INT()==0)//���ó����������
//					    break;
//				 }	
//         else if(FPGA_Num==4)
//         {
//						if(Fpga4_Read_INT()==0)//���ó����������
//					    break;
//				 }				 
//				 else if(FPGA_Num==5)
//         {
//						;
//				 }
//			}			
//			for(i=0;i<250;i++)//����������ɺ���һ������ȴ�ϵͳ����
//			{
//				FPGA_CCLK_ENABLE;
//				delay_ns(100);
//				FPGA_CCLK_DISABLE;
//				delay_ns(100);
//				if(FPGA_Num==1)
//				{	
//				  if(Fpga1_Read_Done()==1)
//				  {
//						Done=1;				
//						break;
//					}
//        }
//        else if(FPGA_Num==2)
//        {
//					if(Fpga2_Read_Done()==1)
//				  {
//						Done=1;				
//						break;
//					}
//				}
//        else if(FPGA_Num==3)
//        {
//					if(Fpga3_Read_Done()==1)
//				  {
//						Done=1;				
//						break;
//					}
//				}
//        else if(FPGA_Num==4)
//        {
//					if(Fpga4_Read_Done()==1)
//				  {
//						Done=1;				
//						break;
//					}
//				}	
//        else if(FPGA_Num==5)
//        {
//					if(Fpga1_Read_Done()&&Fpga2_Read_Done())
//					{
//						Done=1;				
//						break;
//					}	
//				}				
//			}
//			for(i=0;i<16;i++)//����������ɺ���һ������ȴ�ϵͳ����
//			{
//				FPGA_CCLK_ENABLE;
//				delay_ns(100);
//				FPGA_CCLK_DISABLE;
//				delay_ns(100);				
//			}			
//      num++;
//      if(num>5)//����5�ζ�û�ɹ����˳�
//      {
//			   return 0;
//			}				
//	 }	

delay_ns(1000000000);
   return 1;	 
}
//**********************************************************
//FPGA���غ���3
//���ⲿFlash��ȡ�����ļ�
//FPGA_Num:�ڼ���FPGA(ȡֵ1-5,5��ʾ4��FPGAͬʱ����)
//
//**********************************************************
u8 Fpga_Down(u8 FPGA_Num,u32 FPGA_CODE_ADD)
{
	 u32 i=0;
	 u8 j;
	 u8 Done=0;
	 u8 dat;//����Buf
	 u8 num=0;//�ظ����ô���
	u8 data,ret1,ret2;
//	 for(i = 0 ; i < 100; i++)
//	dat = FLASH_ReadWriteByte(0XFF);
	
	
	 while(!Done)
	 {
			 FPGA_CCLK_DISABLE;
		   
			 for(i=0;i<FPGA_LEN;i++)
			 {
					
				 //W25QXX_Read(&dat,i+FPGA_CODE_ADD,1);				 
				 //FPGA_Flash_Read(&dat, i+FPGA_CODE_ADD, 1);//��ȡ
				  Org_Flash_Read(&dat, i+FPGA_CODE_ADD, 1);//��ȡ
				 data = dat;
					for(j=0;j<8;j++)
					{
						if((dat & 0x80)==0)
						{
							 FPGA_DIN_DISABLE;
							 LED3 = 0;//GPIO_SetBits(GPIOH,GPIO_Pin_11);
						}
						if((dat & 0x80)==0x80)
						{	
							FPGA_DIN_ENABLE;
							LED3 = 1;//GPIO_ResetBits(GPIOH,GPIO_Pin_11);
						}	
						FPGA_CCLK_ENABLE;
						//delay_us(1);
						//delay_ns(200);
						FPGA_CCLK_DISABLE;
						//delay_us(1);
					  //delay_ns(200);
						dat=dat << 1;					
					}
					if(FPGA_Num==1)
					{	 
							if(Fpga1_Read_INT()==0)//���ó����������
							  break;
					}
					else if(FPGA_Num==2)
					{
								if(Fpga2_Read_INT()==0)//���ó����������
								 break;
					 }
					else if(FPGA_Num==3)
					{
								if(Fpga3_Read_INT()==0)//���ó����������
								 break;
					 }	
					else if(FPGA_Num==4)
					{
								if(Fpga4_Read_INT()==0)//���ó����������
								 break;
					 }				 
					 else if(FPGA_Num==5)
					{
						ret1 = Fpga1_Read_INT();
						ret2 = Fpga2_Read_INT();
							if(ret1==0||ret2==0)//���ó����������
								break;
					 }
						 //delay_ns(1000000);
			}			
			for(i=0;i<250;i++)//����������ɺ���һ������ȴ�ϵͳ����
			{
				FPGA_CCLK_ENABLE;
				delay_ns(100);
				FPGA_CCLK_DISABLE;
				delay_ns(100);
				if(FPGA_Num==1)
				{	
				  if(Fpga1_Read_Done()==1)
				  {
						Done=1;				
						break;
					}
				}
			  else if(FPGA_Num==2)
			  {
						if(Fpga2_Read_Done()==1)
					  {
							Done=1;				
							break;
						}
					}
			  else if(FPGA_Num==3)
			  {
						if(Fpga3_Read_Done()==1)
					  {
							Done=1;				
							break;
						}
					}
			  else if(FPGA_Num==4)
			  {
						if(Fpga4_Read_Done()==1)
					  {
							Done=1;				
							break;
						}
					}	
			  else if(FPGA_Num==5)
			  {
						if(Fpga1_Read_Done()&&Fpga2_Read_Done())
						{
							Done=1;				
							break;
						}	
					}				
				}
				for(i=0;i<16;i++)//����������ɺ���һ������ȴ�ϵͳ����
				{
					FPGA_CCLK_ENABLE;
					delay_ns(100);
					FPGA_CCLK_DISABLE;
					delay_ns(100);				
				}			
      num++;
      if(num>1)//����5�ζ�û�ɹ����˳�
      {
			   return 0;
		}				
	 }	
   return 1;	 
}
#endif