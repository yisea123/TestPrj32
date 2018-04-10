/*FPGA配置下载相关*/
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
  * @brief  配置fpga
    
  * @param  cfg     低4位各表示4fpga的配置方式，从低位到高位按1~4递增，1对应配置成motor,0配置成IO
            
      
  * @retval void
	 
	* @note   
  */

void FpgaInit(u8 cfg)
{
	CDV_INT08U  i;
	if(Fpga_Config(5, FPGA_MOTOR))//下载引脚配置
		Fpga_Down(5, FPGA_MOTOR);
//	for( i = 1; i<= CDV_FPGA_NUM ; i++) {
//		if(cfg & (0x01 << (i-1)))
//		{
//			Fpga_Config(i, FPGA_MOTOR);//下载引脚配置
//			Fpga_Down(i, FPGA_MOTOR);
//		} else {
//			Fpga_Config(i, FPGA_IO);//第二块
//			Fpga_Down(i, FPGA_IO);
//		}
//	}
}
//*******************************************************
//第一块FPGA下载失能
//*******************************************************
void FPGA1_COFIG_DISABLE(void)//INT_B
{
	#if CDV_FPGA_NUM > 0
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能相关时钟
	  
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	  
	  GPIO_ResetBits(GPIOD,GPIO_Pin_7);//设置低电平延迟下载
	#endif
}
//*******************************************************
//第一块FPGA下载使能
//*******************************************************
void FPGA1_COFIG_ENABLE(void)
{
	#if CDV_FPGA_NUM > 0
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能相关时钟
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	#endif
}
//*******************************************************
//第二块FPGA下载失能
//*******************************************************
void FPGA2_COFIG_DISABLE(void)
{
	#if CDV_FPGA_NUM > 1
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能相关时钟
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	  GPIO_ResetBits(GPIOG,GPIO_Pin_10);//设置低电平延迟下载
	  //FPGA2_PROG_B_ENABLE;
	#endif
}
//*******************************************************
//第二块FPGA下载使能
//*******************************************************
void FPGA2_COFIG_ENABLE(void)
{
	#if CDV_FPGA_NUM > 1
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能相关时钟
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
}
//*******************************************************
//第三块FPGA下载失能
//*******************************************************
void FPGA3_COFIG_DISABLE(void)
{
#if CDV_FPGA_NUM > 2
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能相关时钟
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	  GPIO_ResetBits(GPIOD,GPIO_Pin_8);//设置低电平延迟下载
#endif
}
//*******************************************************
//第三块FPGA下载使能
//*******************************************************
void FPGA3_COFIG_ENABLE(void)
{
#if CDV_FPGA_NUM > 2
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能相关时钟
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
}
//*******************************************************
//第四块FPGA下载失能
//*******************************************************
void FPGA4_COFIG_DISABLE(void)
{
#if CDV_FPGA_NUM > 3
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//使能相关时钟
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//设置类型
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	  GPIO_ResetBits(GPIOH,GPIO_Pin_6);//设置低电平延迟下载
#endif
}
//*******************************************************
//第四块FPGA下载使能
//*******************************************************
void FPGA4_COFIG_ENABLE(void)
{
#if CDV_FPGA_NUM > 3
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//使能相关时钟
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		GPIO_Init(GPIOH, &GPIO_InitStructure);
#endif
}
//**********************************************************
//FPGA1下载引脚配置
//FPGA1_PROG_B=PE8输出
//FPGA_CCLK=PB3输出
//FPGA_DIN=PB5输出
//FPGA1_INT_B=PE7输入
//FPGA1_DONE=PH8输入
//PB4配置成FPGA复位引脚
//**********************************************************
void Fpga1_Config_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOD, ENABLE);//使能相关时钟

		
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
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//复位FPGA
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//FPGA1_INT_B
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//DONE
		GPIO_Init(GPIOG, &GPIO_InitStructure);
		
		FPGA1_PROG_B_ENABLE;
		FPGA_RESET_DISABLE;//不复位FPGA
}
//**********************************************************
//FPGA2下载引脚配置
//FPGA2_PROG_B=PH9输出
//FPGA_CCLK=PB3输出
//FPGA_DIN=PB5输出
//FPGA2_INT_B=PB14输入（输出低电平延迟下载）
//FPGA2_DONE=PB6输入
//PB4配置成FPGA复位引脚
//**********************************************************
void Fpga2_Config_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH , ENABLE);//使能相关时钟

		
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
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//复位FPGA
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//设置类型
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置速度
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//设置上拉下拉寄存器
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//INT_B
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//DONE
		GPIO_Init(GPIOG, &GPIO_InitStructure);
	
		
		FPGA2_PROG_B_ENABLE;
		FPGA_RESET_DISABLE;//不复位FPGA
}

//**********************************************************
//FPGA3下载引脚配置
//FPGA3_PROG_B=PB13输出
//FPGA3_INT_B=PD8输入
//FPGA3_DONE=PI4输入
//PB4配置成FPGA复位引脚
//**********************************************************
void Fpga3_Config_Init(void)
{
    
		
//		FPGA3_PROG_B_ENABLE;
//		FPGA_RESET_DISABLE;//不复位FPGA
}
//**********************************************************
//FPGA4下载引脚配置
//FPGA4_PROG_B=PH7输出
//FPGA4_INT_B=PH6输入
//FPGA4_DONE=PB7输入
//PB4配置成FPGA复位引脚
//**********************************************************
void Fpga4_Config_Init(void)
{
    
		
//		FPGA4_PROG_B_ENABLE;
//		FPGA_RESET_DISABLE;//不复位FPGA
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
//读取引脚FPGA1 DONE电平
//**********************************************************
u8 Fpga1_Read_Done(void)
{
	u8 dat;
	dat=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);
	return dat;
}	
//**********************************************************
//读取引脚FPGA1 INT电平
//**********************************************************
u8 Fpga1_Read_INT(void)
{
	u8 dat;
	dat=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7);
	return dat;
}
//**********************************************************
//读取引脚FPGA2 DONE电平
//**********************************************************
u8 Fpga2_Read_Done(void)
{
	u8 dat;
	dat=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0);
	return dat;
}	
//**********************************************************
//读取引脚FPGA2 INT电平
//**********************************************************
u8 Fpga2_Read_INT(void)
{
	u8 dat;
	dat=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10);
	return dat;
}
//**********************************************************
//读取引脚FPGA3 DONE电平
//**********************************************************
u8 Fpga3_Read_Done(void)
{
	return 0;
}	
//**********************************************************
//读取引脚FPGA3 INT电平
//**********************************************************
u8 Fpga3_Read_INT(void)
{
	return 0;
}
//**********************************************************
//读取引脚FPGA4 DONE电平
//**********************************************************
u8 Fpga4_Read_Done(void)
{
	return 0;
}	
//**********************************************************
//读取引脚FPGA4 INT电平
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
//FPGA下载函数1，配置代码放在ARM片上Flash
//Buf:配置数据存储数组
//Code_Size:数组大小
//配置成功返回1，配置失败返回0
//**********************************************************
//u8 Fpga1_Config(u8 *Buf,u32 Code_Size)
//{
//	 u32 i=0;
//	 u8 j;
//	 u8 Done=0;
//	 u8 dat;//缓存Buf
//	 u8 num=0;//重复配置次数
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
//				 if(Fpga1_Read_INT()==0)//配置出错从新配置
//					 break;  
//			}			
//			for(i=0;i<100;i++)//数据配置完成后发送一定脉冲等待系统启动
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
//			for(i=0;i<16;i++)//数据配置完成后发送一定脉冲等待系统启动
//			{
//				FPGA_CCLK_ENABLE;
//				delay_us(50);
//				FPGA_CCLK_DISABLE;
//				delay_us(50);				
//			}			
//      num++;
//      if(num>5)//配置5次都没成功就退出
//      {
//			   return 0;
//			}				
//	 }	
//   return 1;	 
//}
//**********************************************************
//FPGA下载函数2
//从外部Flash读取配置文件
//FPGA_Num:第几块FPGA(取值1-5,5表示4块FPGA同时下载)
//
//**********************************************************
u8 Fpga_Config(u8 FPGA_Num,u32 FPGA_CODE_ADD)
{
	 u32 i=0;
//	 u8 j;
//	 u8 Done=0;
//	 u8 dat;//缓存Buf
//	 u8 num=0;//重复配置次数
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
//				  SPI_Flash_Read(&dat, i+FPGA_CODE_ADD, 1);//读取
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
//				   if(Fpga1_Read_INT()==0)//配置出错从新配置
//					  break;
//         }
//         else if(FPGA_Num==2)
//         {
//						if(Fpga2_Read_INT()==0)//配置出错从新配置
//					    break;
//				 }
//         else if(FPGA_Num==3)
//         {
//						if(Fpga3_Read_INT()==0)//配置出错从新配置
//					    break;
//				 }	
//         else if(FPGA_Num==4)
//         {
//						if(Fpga4_Read_INT()==0)//配置出错从新配置
//					    break;
//				 }				 
//				 else if(FPGA_Num==5)
//         {
//						;
//				 }
//			}			
//			for(i=0;i<250;i++)//数据配置完成后发送一定脉冲等待系统启动
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
//			for(i=0;i<16;i++)//数据配置完成后发送一定脉冲等待系统启动
//			{
//				FPGA_CCLK_ENABLE;
//				delay_ns(100);
//				FPGA_CCLK_DISABLE;
//				delay_ns(100);				
//			}			
//      num++;
//      if(num>5)//配置5次都没成功就退出
//      {
//			   return 0;
//			}				
//	 }	

delay_ns(1000000000);
   return 1;	 
}
//**********************************************************
//FPGA下载函数3
//从外部Flash读取配置文件
//FPGA_Num:第几块FPGA(取值1-5,5表示4块FPGA同时下载)
//
//**********************************************************
u8 Fpga_Down(u8 FPGA_Num,u32 FPGA_CODE_ADD)
{
	 u32 i=0;
	 u8 j;
	 u8 Done=0;
	 u8 dat;//缓存Buf
	 u8 num=0;//重复配置次数
	u8 data,ret1,ret2;
//	 for(i = 0 ; i < 100; i++)
//	dat = FLASH_ReadWriteByte(0XFF);
	
	
	 while(!Done)
	 {
			 FPGA_CCLK_DISABLE;
		   
			 for(i=0;i<FPGA_LEN;i++)
			 {
					
				 //W25QXX_Read(&dat,i+FPGA_CODE_ADD,1);				 
				 //FPGA_Flash_Read(&dat, i+FPGA_CODE_ADD, 1);//读取
				  Org_Flash_Read(&dat, i+FPGA_CODE_ADD, 1);//读取
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
							if(Fpga1_Read_INT()==0)//配置出错从新配置
							  break;
					}
					else if(FPGA_Num==2)
					{
								if(Fpga2_Read_INT()==0)//配置出错从新配置
								 break;
					 }
					else if(FPGA_Num==3)
					{
								if(Fpga3_Read_INT()==0)//配置出错从新配置
								 break;
					 }	
					else if(FPGA_Num==4)
					{
								if(Fpga4_Read_INT()==0)//配置出错从新配置
								 break;
					 }				 
					 else if(FPGA_Num==5)
					{
						ret1 = Fpga1_Read_INT();
						ret2 = Fpga2_Read_INT();
							if(ret1==0||ret2==0)//配置出错从新配置
								break;
					 }
						 //delay_ns(1000000);
			}			
			for(i=0;i<250;i++)//数据配置完成后发送一定脉冲等待系统启动
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
				for(i=0;i<16;i++)//数据配置完成后发送一定脉冲等待系统启动
				{
					FPGA_CCLK_ENABLE;
					delay_ns(100);
					FPGA_CCLK_DISABLE;
					delay_ns(100);				
				}			
      num++;
      if(num>1)//配置5次都没成功就退出
      {
			   return 0;
		}				
	 }	
   return 1;	 
}
#endif