
/*FPGA IO AND MOTO FUNCTION*/
#include "FPGA_spi.h"

#define FPGA_ReadWriteByte(A) SPIx_ReadWriteByte(SPI2, (A))


u8 err_times1=0;//电机归零出错次数记录
u8 err_times2=0;
u16 failed_times=0;
u8 CDV_EXI_NUM = 34;
u8 CDV_EXO_NUM = 30;
u8 FPGA_Mode = FPGA_MOTOR_CODE;//FPGA程序模式
//#if ENABLE_FPGA_DOWN
//*******************************************************************
//SPI1初始化
//
//*******************************************************************
//	void SPI1_Init(void)
//{
//	  SPI_InitTypeDef SPI_InitStruct;
//		GPIO_InitTypeDef GPIO_InitStructure;

//		/* 使能GPIO 时钟 */
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOH, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
//		
//		/* 配置 SCK, MISO 、 MOSI 为复用功能 */
//	  #ifdef CDV_V2_2
//	      GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
//	      GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
//			  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

//				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
//				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//				GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
//				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//				GPIO_Init(GPIOB, &GPIO_InitStructure);
//	  #else
//	     GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
//			 GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
//			 GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

//			 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//			 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//			 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//			 GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
//			 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//			 GPIO_Init(GPIOA, &GPIO_InitStructure);
//	  #endif
//		
//		
//		SPI_I2S_DeInit(SPI1);
//		SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
//		SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 
//		SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
//		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
//		SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
//		SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;
//		SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
//		SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
//		SPI_InitStruct.SPI_CRCPolynomial = 7;
//		SPI_Init(SPI1, &SPI_InitStruct);

//	  SPI_Cmd(SPI1, ENABLE);


//		/* 配置片选口线为推挽输出模式 */
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//		GPIO_Init(GPIOE, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//		GPIO_Init(GPIOH, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//		
//		SF0_CS1_ENABLE;		/* 片选置高，不选中 */
//		SF0_CS2_ENABLE;
//		SF0_CS3_ENABLE;
//		SF0_CS4_ENABLE;
//	}


/** @brief  查询从机回复（不等待）
  * @param  pBuffer          查询到的字符串
	*         NumByteToRead    查询的长度
  * @retval 返回值说明
  * @note   请勿随意调用
  */
void Fpga_Rx(CDV_INT08U* pBuffer, CDV_INT16U NumByteToRead)   
{ 
 	CDV_INT16U i;    		
  OS_ERR err;	
	
	ASSERT(pBuffer);
	
	for(i = 0; i < NumByteToRead; i++)
	{
			pBuffer[i] = FLASH_ReadWriteByte(0X00);   //循环读数  
	}
}  
/** @brief  主机发送命令
  * @param  pBuffer          发送的字符串
	*         NumByteToWrite   发送的字符串长度
  * @retval 返回值说明
  * @note   请勿随意调用
  */
void Fpga_Tx(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{
 	CDV_INT16U i;    		
  OS_ERR err;	
	
	ASSERT(pBuffer);
	
	for(i = 0 ; i < NumByteToWrite; i++)
	{
			/*temp[i] = */FLASH_ReadWriteByte(pBuffer[i]);   //循环读数  
	}
}

/*
no:0-3
对应
fpga:0-3
*/

void FpgaCS(CDV_INT08U no)
{
	SPI2_Configuration(SPI_BaudRatePrescaler_8);
	FPGA_DS;
	switch(no)
	{
		case 0:
			FPGA_EN(1);
			break;
		case 1:
			FPGA_EN(2);
			break;
		case 2:
			
		case 3:
			
		default:
			break;
	}
}
/** @brief  主机发送接收接口，用于加入信号量
  * @param  pTxBuf          发送的字符串 NULL 不发送
	*         txByte          发送的字符串长度
  *         pRxBuf          查询成功保存从机回复的字符串
	*         rxLen           查询长度
  *         no              第几片FPGA，从0开始
  * @retval 返回值说明
  * @note   外部主级联封装发送命令必须使用该接口，或者与级联总线的其他设备资源易产生冲突
  */
void FpgaTR(CDV_INT08U* pTxBuf, CDV_INT16U txByte, CDV_INT08U* pRxBuf, CDV_INT16U rxByte, CDV_INT08U no)   
{
  OS_ERR err;
	OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	FpgaCS(no);
	
	if(pTxBuf && txByte) {
	  Fpga_Tx(pTxBuf, txByte);
	}
	
	if(pRxBuf && rxByte) {
	  Fpga_Rx(pRxBuf, rxByte);
	}
	FPGA_DS;
	OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
}

/*************************************************************/
void FpgaRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
{

}

void FpgaWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{

}


#define SW 3
#if (1 == SW)
//**************************************
//发送参数
//**************************************
void Send_parameter(u8 *buf)
{
	u8 i;
	
	
	for(i=0;i<3;i++)//发送读3字节
	{  
		FPGA_ReadWriteByte(buf[i]);
	}
}
	
//*************************************
//
//
//*************************************
void FPGA_Send_parameter(u8 *buf,u8 no)
{
   u16 FPGA_DATA;
   Send_parameter(buf);
	
   FPGA_DATA=Read_Fpga_Regist(buf[0]+0x80,(no>>1)+1);
   
   while(FPGA_DATA!=((buf[1]<<8)+buf[2]))
   {
	   err_times1++;
	   err_times2++;
	   Send_parameter(buf);
	   FPGA_DATA=Read_Fpga_Regist(buf[0]+0x80,(no>>1)+1);
	   if(err_times1>20)
	   { 	
           failed_times++;		   
		   break;//
	   } 	   
   }
   err_times1=0;
}


/*电机命令*/
//u8 cmdMotorStart[8]    = {0x01 , 0x15 , 0 , 0};
//u8 cmdMotorReturn[8]   = {0x03 , 0x17 , 0 , 0};
//u8 cmdMotorDir[8]      = {0x04 , 0x18 , 0 , 0};
//u8 cmdMotorAcc[8]      = {0x05 , 0x19 , 0 , 0};
//u8 cmdMotorDec[8]      = {0x06 , 0x1a , 0 , 0};
//u8 cmdMotorMaxSpeed[8] = {0x07 , 0x1b , 0 , 0};
//u8 cmdMotorStepH[8]    = {0x08 , 0x1c , 0 , 0};
//u8 cmdMotorStepL[8]    = {0x09 , 0x1d , 0 , 0};
/*圆弧运动命令*/
//u8 cmdArcStart[4]    = {0x3d , 0 , 0 , 0};
//u8 cmdArcDir[4]      = {0x3f , 0 , 0 , 0};
//u8 cmdArcRadiusH[4]  = {0x40 , 0 , 0 , 0};
//u8 cmdArcRadiusL[4]  = {0x41 , 0 , 0 , 0};
//u8 cmdArcStartXH[4]  = {0x42 , 0 , 0 , 0};
//u8 cmdArcStartXL[4]  = {0x43 , 0 , 0 , 0};
//u8 cmdArcStartYH[4]  = {0x44 , 0 , 0 , 0};
//u8 cmdArcStartYL[4]  = {0x45 , 0 , 0 , 0};
//u8 cmdArcEndXH[4]    = {0x46 , 0 , 0 , 0};
//u8 cmdArcEndXL[4]    = {0x47 , 0 , 0 , 0};
//u8 cmdArcEndYH[4]    = {0x48 , 0 , 0 , 0};
//u8 cmdArcEndYL[4]    = {0x49 , 0 , 0 , 0};
//u8 cmdArcAcc[4]      = {0x4a , 0 , 0 , 0};
//u8 cmdArcDec[4]      = {0x4b , 0 , 0 , 0};
//u8 cmdArcMaxSpeed[4] = {0x4c , 0 , 0 , 0};


/**
  * @brief  等待某轴电机到位
    
  * @param  no     要控制的电机号0-7
      
      
  * @retval u8 1 完成，0未完成
	 
  * @note   
  */

u8 FpgaMotoWait(u8 no){
	u8 ret=0;
	u8 moto = no & 0x01;
	u8 fpga = (no >>1)+1;
	u16 res = Read_Fpga_Regist(0xd5,fpga);
	//delay_ms(5);
	 if(moto==0)
	 {
		ret=(0x01 & res);
	 } 
	 else if(moto==1)
	 {

		ret=(0x02 & res);
	 }	
	
	 return ret;
	
}
/**************************************************扩展IO******************************************************/
//**************************************
//jxd
//FPGAIO读取
//num:I号（0-27）
//
//**************************************
u8 Read_FPGA_I(u8 num)
{
	u8 i;
	u8 buf[3]={0,0,0};
	u8 state[3]={0,0};
	
	buf[0]=0xa3+num;//起始地址0xa3
	for(i=0;i<3;i++)//发送读3字节
	{  
		FPGA_ReadWriteByte(buf[0]);
	}
	//LED2=1;
	for(i=0;i<3;i++)//发送读3字节
	{  
		state[i]=FPGA_ReadWriteByte(0x00);
	}
	buf[0]=0;
	
	return state[1];//第二字节有效
	
}
//***************************************
/**
  * @brief  读取i
    
  * @param  no     要控制的i号
      
  * @retval u8 1高电平，0低电平
	 
	* @note   
  */

u8 FpgaIRead( u8 no )
{
	//OS_ERR err;
	u8 buf=0;
	u8 tmpNo = no % CDV_EXI_NUM;
	u8 fpgaNo = no / CDV_EXI_NUM;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	switch(fpgaNo)
	{
		case 0:
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS1_DISENABLE;
			SF0_CS1_ENABLE;
		  break;
		case 1:
			SF0_CS1_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS2_ENABLE;
		  break;
		case 2:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS3_ENABLE;
		  break;
		case 3:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS4_ENABLE;
		  break;
		default:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
		  break;
	} 
//	SF0_CS1_DISENABLE;//这里把第二块FPGA作为扩展IO
//	SF0_CS2_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS3_DISENABLE;//所以只使能了第二块FPGA
//	SF0_CS2_ENABLE;
	//LED2=0;
	if(/*(no>=0)&&*/(tmpNo<CDV_EXI_NUM))//扩展I口有30个
	{	
	  buf=Read_FPGA_I(tmpNo);
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return (buf & 0x01);//最低位(bit0)为有效位
	}	
	else
	{
//    OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);		
	  return 2;
	}
}
//**************************************
//jxd
//FPGAO读取
//num:O口（0-33）
//返回读取到的有效字节(第二字节)
//**************************************
u8 Read_FPGA_O(u8 num)
{
	u8 i;
	u8 buf[3];
	u8 state[2];
	
	buf[0]=0x81+num;//O口首地址0x81
	
	
	for(i=0;i<3;i++)//发送读3字节
	{  
		FPGA_ReadWriteByte(buf[i]);
	}
	for(i=0;i<2;i++)//发送读2字节
	{  
		state[i]=FPGA_ReadWriteByte(0x00);
	}
	
	return state[1];//第二字节的Bit(0)为有效位
}
//*************************************
/**
  * @brief  读取O
    
  * @param  no     要控制的O号
      
  * @retval u8 1高电平，0低电平
	 
	* @note   
  */

u8 FpgaORead( u8 no )
{
//	OS_ERR err;
    u8 buf[3] = {0};
    u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
			  
	switch(fpgaNo)
	{
		case 0:
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS1_DISENABLE;
			SF0_CS1_ENABLE;
		  break;
		case 1:
			SF0_CS1_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS2_ENABLE;
		  break;
		case 2:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS3_ENABLE;
		  break;
		case 3:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS4_ENABLE;
		  break;
		default:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
		  break;
	}   
//	SF0_CS1_DISENABLE;//这里把第二块FPGA作为扩展IO
//	SF0_CS3_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS2_DISENABLE;//所以只使能了第二块FPGA
//	SF0_CS2_ENABLE;//所以只使能了第二块FPGA
	if(/*(no>=0)&&*/(tmpNo<CDV_EXO_NUM))//34个O口
	{	
		buf[0]=Read_FPGA_O(tmpNo);
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return (buf[0]&0x01);
	}	
	else {
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return 4;
	}
}
//**************************************
//jxd
//FPGA写O口
//num:Io口号1-32
//state：高/低
//**************************************
void PUT_FPGA_O(u8 num,u8 state)
{
	u8 i;
	u8 buf[3]={0};
	
	if(/*(num>=0)&&*/(num<=33))
	{	
//		if(num==0){
//			if(state==1)
//			LED2=1;
//		else LED2=0;
//		}
		buf[0]=0x01+num;
		buf[1]=0x00;
	  buf[2]=0x01 & state;
    for(i=0;i<3;i++)//发送读3字节
	  {  
		  FPGA_ReadWriteByte(buf[i]);
	  }
	}	
	
}
//***************************************

/**
  * @brief  写O
    
  * @param  no     要控制的O号
      
  * @retval u8 1高电平，0低电平
	 
	* @note   
  */

void FpgaOWrite( u8 no , u8 val)
{
//	OS_ERR err;
	u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;
	//u8 buf[3] = {0};
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	
	switch(fpgaNo)
	{
		case 0:
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS1_DISENABLE;
			SF0_CS1_ENABLE;
		  break;
		case 1:
			SF0_CS1_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS2_ENABLE;
		  break;
		case 2:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS3_ENABLE;
		  break;
		case 3:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS4_ENABLE;
		  break;
		default:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
		  break;
	}   
	
//	SF0_CS1_DISENABLE;//这里把第二块FPGA作为扩展IO
//	SF0_CS3_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS2_DISENABLE;//所以只使能了第二块FPGA
//	SF0_CS2_ENABLE;
	PUT_FPGA_O(tmpNo,val);
//	OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
}


//***************************************************************
//读取FPGA寄存器
//add:寄存器地址
//返回读取到的两个字节数据
//***************************************************************
u16 Read_Fpga_Regist(u8 add,u8 FPGA_NUM)
{
	  u8 i;
	  u8 SPI1_Buffer_Rx[3]={0};
	  u16 rec_data=0x0000;
//	  OS_ERR err;
//	  OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	  switch(FPGA_NUM)
		{
			case 1:
							SF0_CS2_DISENABLE;
							SF0_CS3_DISENABLE;
							SF0_CS4_DISENABLE;
							SF0_CS1_DISENABLE;
							SF0_CS1_ENABLE;
							break;
			case 2:
							SF0_CS1_DISENABLE;
							SF0_CS3_DISENABLE;
							SF0_CS4_DISENABLE;
							SF0_CS2_DISENABLE;
			SF0_CS1_ENABLE;
							SF0_CS2_ENABLE;
			
//				SF0_CS1_DISENABLE;
//							SF0_CS3_DISENABLE;
//							SF0_CS4_DISENABLE;
//							SF0_CS2_DISENABLE;
						 break;
			case 3:
							SF0_CS1_DISENABLE;
							SF0_CS2_DISENABLE;
							SF0_CS4_DISENABLE;
							SF0_CS3_DISENABLE;
							SF0_CS3_ENABLE;
						 break;
			case 4:
							SF0_CS1_DISENABLE;
							SF0_CS2_DISENABLE;
							SF0_CS3_DISENABLE;
							SF0_CS4_DISENABLE;
							SF0_CS4_ENABLE;
						 break;
			default:
							 SF0_CS1_DISENABLE;
							 SF0_CS2_DISENABLE;
							 SF0_CS3_DISENABLE;
							 SF0_CS4_DISENABLE;
						 break;
		}   
	
		//发送读3字节地址
		FPGA_ReadWriteByte(add);
		FPGA_ReadWriteByte(0x00);
		FPGA_ReadWriteByte(0x00);

		for(i=0;i<3;i++)//发送3字节长度
		{  
				SPI1_Buffer_Rx[i]=FPGA_ReadWriteByte(0x00);	  
		}
		rec_data=(SPI1_Buffer_Rx[0]<<8)+SPI1_Buffer_Rx[1];
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return rec_data;
}
#endif
#if (2 == SW)
//**************************************
//发送参数
//**************************************
void Send_parameter(u8 *buf)
{
	u8 i;
	for(i=0;i<6;i++)//发送读6字节
	{  
		FPGA_ReadWriteByte(buf[i]);
	}
}
//*************************************
//
//
//*************************************
void FPGA_Send_parameter(u8 *buf,u8 no)
{
   u32 FPGA_DATA;
   Send_parameter(buf);
	
	FPGA_DATA=Read_Fpga_Regist((buf[0]<<8)+0x8000+buf[1],(no>>1)+1);
//   while(FPGA_DATA!=((buf[1]<<8)+buf[2]))
	while(FPGA_DATA!=((buf[2]<<24)+(buf[3]<<16)+(buf[4]<<8)+buf[5]))
   {
	   err_times1++;
	   err_times2++;
	   Send_parameter(buf);
//	   FPGA_DATA=Read_Fpga_Regist(buf[0]+0x80,(no>>1)+1);
		FPGA_DATA=Read_Fpga_Regist((buf[0]<<8)+0x8000+buf[1],(no>>1)+1);
	   if(err_times1>20)
	   { 	
           failed_times++;		   
		   break;//
	   } 	   
   }
   err_times1=0;
}


/*电机命令*/
//u8 cmdMotorStart[8]    = {0x01 , 0x15 , 0 , 0};
//u8 cmdMotorReturn[8]   = {0x03 , 0x17 , 0 , 0};
//u8 cmdMotorDir[8]      = {0x04 , 0x18 , 0 , 0};
//u8 cmdMotorAcc[8]      = {0x05 , 0x19 , 0 , 0};
//u8 cmdMotorDec[8]      = {0x06 , 0x1a , 0 , 0};
//u8 cmdMotorMaxSpeed[8] = {0x07 , 0x1b , 0 , 0};
//u8 cmdMotorStepH[8]    = {0x08 , 0x1c , 0 , 0};
//u8 cmdMotorStepL[8]    = {0x09 , 0x1d , 0 , 0};
/*圆弧运动命令*/
//u8 cmdArcStart[4]    = {0x3d , 0 , 0 , 0};
//u8 cmdArcDir[4]      = {0x3f , 0 , 0 , 0};
//u8 cmdArcRadiusH[4]  = {0x40 , 0 , 0 , 0};
//u8 cmdArcRadiusL[4]  = {0x41 , 0 , 0 , 0};
//u8 cmdArcStartXH[4]  = {0x42 , 0 , 0 , 0};
//u8 cmdArcStartXL[4]  = {0x43 , 0 , 0 , 0};
//u8 cmdArcStartYH[4]  = {0x44 , 0 , 0 , 0};
//u8 cmdArcStartYL[4]  = {0x45 , 0 , 0 , 0};
//u8 cmdArcEndXH[4]    = {0x46 , 0 , 0 , 0};
//u8 cmdArcEndXL[4]    = {0x47 , 0 , 0 , 0};
//u8 cmdArcEndYH[4]    = {0x48 , 0 , 0 , 0};
//u8 cmdArcEndYL[4]    = {0x49 , 0 , 0 , 0};
//u8 cmdArcAcc[4]      = {0x4a , 0 , 0 , 0};
//u8 cmdArcDec[4]      = {0x4b , 0 , 0 , 0};
//u8 cmdArcMaxSpeed[4] = {0x4c , 0 , 0 , 0};


/**
  * @brief  等待某轴电机到位
    
  * @param  no     要控制的电机号0-7
      
      
  * @retval u8 1 完成，0未完成
	 
  * @note   
  */

u8 FpgaMotoWait(u8 no){
	u8 ret=0;
	u8 moto = no & 0x01;
	u8 fpga = (no >>1)+1;
	//u16 res = Read_Fpga_Regist(0xd5,fpga);
	u32 res = Read_Fpga_Regist(0x8F00,fpga);
	//delay_ms(5);
	 if(moto==0)
	 {
		ret=(0x01 & res);
	 } 
	 else if(moto==1)
	 {

		ret=(0x02 & res);
	 }	
	
	 return ret;
	
}
/**************************************************扩展IO******************************************************/
//**************************************
//jxd
//FPGAIO读取
//num:I号（0-27）
//
//**************************************
u8 Read_FPGA_I(u8 num)
{
	u8 i;
	u8 buf[3]={0,0,0};
	u8 state[3]={0,0};
	buf[0]=0xa3+num;//起始地址0xa3
	for(i=0;i<3;i++)//发送读3字节
	{  
		FPGA_ReadWriteByte(buf[0]);
	}
	//LED2=1;
	for(i=0;i<3;i++)//发送读3字节
	{  
		state[i]=FPGA_ReadWriteByte(0x00);
	}
	buf[0]=0;
	return state[1];//第二字节有效
}
//***************************************
/**
  * @brief  读取i
    
  * @param  no     要控制的i号
      
  * @retval u8 1高电平，0低电平
	 
	* @note   
  */

u8 FpgaIRead( u8 no )
{
//	OS_ERR err;
	u8 buf=0;
	u8 tmpNo = no % CDV_EXI_NUM;
	u8 fpgaNo = no / CDV_EXI_NUM;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	switch(fpgaNo)
	{
		case 0:
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS1_DISENABLE;
			SF0_CS1_ENABLE;
		  break;
		case 1:
			SF0_CS1_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS2_ENABLE;
		  break;
		case 2:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS3_ENABLE;
		  break;
		case 3:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS4_ENABLE;
		  break;
		default:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
		  break;
	} 
//	SF0_CS1_DISENABLE;//这里把第二块FPGA作为扩展IO
//	SF0_CS2_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS3_DISENABLE;//所以只使能了第二块FPGA
//	SF0_CS2_ENABLE;
	//LED2=0;
	if(/*(no>=0)&&*/(tmpNo<CDV_EXI_NUM))//扩展I口有30个
	{	
	  buf=Read_FPGA_I(tmpNo);
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return (buf & 0x01);//最低位(bit0)为有效位
	}	
	else
	{
//    OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);		
	  return 2;
	}
}
//**************************************
//jxd
//FPGAO读取
//num:O口（0-33）
//返回读取到的有效字节(第二字节)
//**************************************
u8 Read_FPGA_O(u8 num)
{
	u8 i;
	u8 buf[3];
	u8 state[2];
	buf[0]=0x81+num;//O口首地址0x81
	
	for(i=0;i<3;i++)//发送读3字节
	{  
		FPGA_ReadWriteByte(buf[i]);
	}
	for(i=0;i<2;i++)//发送读2字节
	{  
		state[i]=FPGA_ReadWriteByte(0x00);
	}
	return state[1];//第二字节的Bit(0)为有效位
}
//*************************************
/**
  * @brief  读取O
    
  * @param  no     要控制的O号
      
  * @retval u8 1高电平，0低电平
	 
	* @note   
  */

u8 FpgaORead( u8 no )
{
//	OS_ERR err;
    u8 buf[3] = {0};
    u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
			  
	switch(fpgaNo)
	{
		case 0:
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS1_DISENABLE;
			SF0_CS1_ENABLE;
		  break;
		case 1:
			SF0_CS1_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS2_ENABLE;
		  break;
		case 2:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS3_ENABLE;
		  break;
		case 3:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS4_ENABLE;
		  break;
		default:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
		  break;
	}   
//	SF0_CS1_DISENABLE;//这里把第二块FPGA作为扩展IO
//	SF0_CS3_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS2_DISENABLE;//所以只使能了第二块FPGA
//	SF0_CS2_ENABLE;//所以只使能了第二块FPGA
	if(/*(no>=0)&&*/(tmpNo<CDV_EXO_NUM))//34个O口
	{	
		buf[0]=Read_FPGA_O(tmpNo);
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return (buf[0]&0x01);
	}	
	else {
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return 4;
	}
}
//**************************************
//jxd
//FPGA写O口
//num:Io口号1-32
//state：高/低
//**************************************
void PUT_FPGA_O(u8 num,u8 state)
{
	u8 i;
	u8 buf[3]={0};
	if(/*(num>=0)&&*/(num<=33))
	{	
//		if(num==0){
//			if(state==1)
//			LED2=1;
//		else LED2=0;
//		}
		buf[0]=0x01+num;
		buf[1]=0x00;
	  buf[2]=0x01 & state;
    for(i=0;i<3;i++)//发送读3字节
	  {  
		  FPGA_ReadWriteByte(buf[i]);
	  }
	}	
}
//***************************************

/**
  * @brief  写O
    
  * @param  no     要控制的O号
      
  * @retval u8 1高电平，0低电平
	 
	* @note   
  */

void FpgaOWrite( u8 no , u8 val)
{
//	OS_ERR err;
	u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;
	//u8 buf[3] = {0};
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	
	switch(fpgaNo)
	{
		case 0:
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS1_DISENABLE;
			SF0_CS1_ENABLE;
		  break;
		case 1:
			SF0_CS1_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS2_ENABLE;
		  break;
		case 2:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS3_ENABLE;
		  break;
		case 3:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
			SF0_CS4_ENABLE;
		  break;
		default:
			SF0_CS1_DISENABLE;
			SF0_CS2_DISENABLE;
			SF0_CS3_DISENABLE;
			SF0_CS4_DISENABLE;
		  break;
	}   
	
//	SF0_CS1_DISENABLE;//这里把第二块FPGA作为扩展IO
//	SF0_CS3_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS2_DISENABLE;//所以只使能了第二块FPGA
//	SF0_CS2_ENABLE;
	PUT_FPGA_O(tmpNo,val);
//	OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
}


//***************************************************************
//读取FPGA寄存器
//add:寄存器地址
//返回读取到的两个字节数据
//***************************************************************
u32 Read_Fpga_Regist(u16 add,u8 FPGA_NUM)
{
	  u8 i;
	  u8 SPI1_Buffer_Rx[6];
	  u32 rec_data=0;
	  u32 buf=0;
//	  OS_ERR err;
//	  OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	  switch(FPGA_NUM)
		{
			case 1:
							SF0_CS2_DISENABLE;
							SF0_CS3_DISENABLE;
							SF0_CS4_DISENABLE;
							SF0_CS1_DISENABLE;
							SF0_CS1_ENABLE;
							break;
			case 2:
							SF0_CS1_DISENABLE;
							SF0_CS3_DISENABLE;
							SF0_CS4_DISENABLE;
							SF0_CS2_DISENABLE;
							SF0_CS2_ENABLE;
						 break;
			case 3:
							SF0_CS1_DISENABLE;
							SF0_CS2_DISENABLE;
							SF0_CS4_DISENABLE;
							SF0_CS3_DISENABLE;
							SF0_CS3_ENABLE;
						 break;
			case 4:
							SF0_CS1_DISENABLE;
							SF0_CS2_DISENABLE;
							SF0_CS3_DISENABLE;
							SF0_CS4_DISENABLE;
							SF0_CS4_ENABLE;
						 break;
			default:
							 SF0_CS1_DISENABLE;
							 SF0_CS2_DISENABLE;
							 SF0_CS3_DISENABLE;
							 SF0_CS4_DISENABLE;
						 break;
		}   
	
		//发送读6字节地址
		FPGA_ReadWriteByte(add>>8);
		FPGA_ReadWriteByte(add);
//		FPGA_ReadWriteByte(0x00);
//		FPGA_ReadWriteByte(0x00);
//		FPGA_ReadWriteByte(0x01);
//		FPGA_ReadWriteByte(0x00);
		FPGA_ReadWriteByte(buf>>24);
		FPGA_ReadWriteByte(buf>>16);
		FPGA_ReadWriteByte(buf>>8);
		FPGA_ReadWriteByte(buf);		
		for(i=0;i<6;i++)//发送3字节长度
		{  
				SPI1_Buffer_Rx[i]=FPGA_ReadWriteByte(0x00);	  
		}
		rec_data=(SPI1_Buffer_Rx[1]<<24)+(SPI1_Buffer_Rx[2]<<16)+(SPI1_Buffer_Rx[3]<<8)+SPI1_Buffer_Rx[4];
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return rec_data;
}
#endif

#if (3 == SW)
//**************************************
//发送参数
//nofpga号 从0开始
//moto专用
//**************************************
void Send_parameter(u8 no, u8 *buf, u8 len)
{
	FpgaTR(buf, len, NULL, 0, no);
}
//*************************************
//no电机号
//moto专用
//*************************************
void FPGA_Send_parameter(u8 *buf,u8 no)
{
   u32 FPGA_DATA;
   Send_parameter(no>>1, buf, 6);
	
	FPGA_DATA=Read_Fpga_Regist((buf[0]<<8)+0x8000+buf[1],no>>1);
//   while(FPGA_DATA!=((buf[1]<<8)+buf[2]))
	while(FPGA_DATA!=((buf[2]<<24)+(buf[3]<<16)+(buf[4]<<8)+buf[5]))
   {
	   err_times1++;
	   err_times2++;
	   Send_parameter((no>>1)+1, buf, 6);
//	   FPGA_DATA=Read_Fpga_Regist(buf[0]+0x80,(no>>1)+1);
		FPGA_DATA=Read_Fpga_Regist((buf[0]<<8)+0x8000+buf[1],no>>1);
	   if(err_times1>20)
	   { 	
           failed_times++;		   
		   break;//
	   } 	   
   }
   err_times1=0;
}


/**
  * @brief  等待某轴电机到位
    
  * @param  no     要控制的电机号0-7
      
      
  * @retval u8 1 完成，0未完成
	 
  * @note   
  */

u8 FpgaMotoWait(u8 no){
	u8 ret=0;
	u8 moto = no & 0x01;
	u8 fpga = no >>1;
	//u16 res = Read_Fpga_Regist(0xd5,fpga);
	u32 res = Read_Fpga_Regist(0x8F00,fpga);
	//delay_ms(5);
	 if(moto==0)
	 {
		ret=(0x01 & res);
	 } 
	 else if(moto==1)
	 {

		ret=(0x02 & res);
	 }	
	
	 return ret;
	
}
/**************************************************扩展IO******************************************************/

/**
  * @brief  读取i
    
  * @param  no     要控制的i号
      
  * @retval u8 1高电平，0低电平
	 
	* @note   
  */

u8 FpgaIRead( u8 no )
{
	u8 buf[FPGA_COMMAND_LEN]={0};
	u8 tmpNo = no % CDV_EXI_NUM;
	u8 fpgaNo = no / CDV_EXI_NUM;

	if(/*(no>=0)&&*/(tmpNo<CDV_EXI_NUM))//扩展I口有30个
	{	
		u8 txBuf[FPGA_COMMAND_LEN] = {0};
		int i=0;
		if(FPGA_Mode == FPGA_MOTOR_CODE)
			txBuf[0] = 0x89;
		else
			txBuf[0] = 0x8F;			
		txBuf[1] = tmpNo;		
		for(;i<3;i++)
		{
			FpgaTR(txBuf, FPGA_COMMAND_LEN, buf, FPGA_COMMAND_LEN, fpgaNo);
			if(buf[4]==0xff&&buf[3]==0xff&&buf[2]==0x00&&buf[1]==0x00)
				return 0;
			else if(buf[1]==0xff&&buf[2]==0xff&&buf[3]==0x00&&buf[4]==0x00)
				return 1;
		}
		return 2;
	}	
	else
	{
	  return 2;
	}
}

void FpgaIReadAll(CDV_INT08U* pRxBuf)
{
	u8 buf[FPGA_COMMAND_LEN*2] = {0};
	u8 txBuf[FPGA_COMMAND_LEN] = {0};
	if(FPGA_Mode == FPGA_MOTOR_CODE)
	{
		txBuf[0] = 0x89;
		txBuf[1] = 0xFF;
	}
	else if(FPGA_Mode == FPGA_IO_CODE)
	{
		txBuf[0] = 0x8F;
		txBuf[1] = 0xDD;
	}
	else
	{
		memset(pRxBuf,1,sizeof(u8)*FPGA_COMMAND_LEN*2); 
	}
	FpgaTR(txBuf, FPGA_COMMAND_LEN, buf+FPGA_COMMAND_LEN, FPGA_COMMAND_LEN, 0);
	FpgaTR(txBuf, FPGA_COMMAND_LEN, buf, FPGA_COMMAND_LEN, 1);
	MemCpy(pRxBuf,buf,sizeof(u8)*FPGA_COMMAND_LEN*2);
}
/**
  * @brief  读取O
    
  * @param  no     要控制的O号
      
  * @retval u8 1高电平，0低电平
	 
	* @note   
  */

u8 FpgaORead( u8 no )
{
//	OS_ERR err;
  u8 buf[FPGA_COMMAND_LEN] = {0};
  u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;

	if(/*(no>=0)&&*/(tmpNo<CDV_EXO_NUM))//34个O口
	{		
		u8 txBuf[FPGA_COMMAND_LEN] = {0};
		int i =0;
		if(FPGA_Mode == FPGA_MOTOR_CODE)
			txBuf[0] = 0x88;
		else
			txBuf[0] = 0x80;
		txBuf[1] = tmpNo;
		for(;i<3;i++)
		{
			FpgaTR(txBuf, FPGA_COMMAND_LEN, buf, FPGA_COMMAND_LEN, fpgaNo);
			if(buf[4]==0xff&&buf[3]==0xff&&buf[2]==0x00&&buf[1]==0x00)
				return 0;
			else if(buf[1]==0xff&&buf[2]==0xff&&buf[3]==0x00&&buf[4]==0x00)
				return 1;
		}
		//FpgaTR(txBuf, FPGA_COMMAND_LEN, buf, FPGA_COMMAND_LEN, fpgaNo);
				
			return 4;
	}	
	else
	{
		return 4;
	}
}

void FpgaOReadAll(CDV_INT08U* pRxBuf)
{
	u8 buf[FPGA_COMMAND_LEN*2] = {0};
	u8 txBuf[FPGA_COMMAND_LEN] = {0};
	if(FPGA_Mode == FPGA_MOTOR_CODE)
	{
		txBuf[0] = 0x88;
		txBuf[1] = 0xFF;
	}
	else if(FPGA_Mode == FPGA_IO_CODE)
	{
		txBuf[0] = 0x80;
		txBuf[1] = 0xDD;
	}
	else
	{
		memset(pRxBuf,1,sizeof(u8)*FPGA_COMMAND_LEN*2); 
	}
	FpgaTR(txBuf, FPGA_COMMAND_LEN, buf+FPGA_COMMAND_LEN, FPGA_COMMAND_LEN, 0);
	FpgaTR(txBuf, FPGA_COMMAND_LEN, buf, FPGA_COMMAND_LEN, 1);
	MemCpy(pRxBuf,buf,sizeof(u8)*FPGA_COMMAND_LEN*2);
}

//所有fpga o的写
void FpgaOWriteAll(CDV_INT08U* pRxBuf)
{
	u8 buf[FPGA_COMMAND_LEN*2] = {0};
	u8 txBuf[FPGA_COMMAND_LEN] = {0};
	if(FPGA_Mode == FPGA_MOTOR_CODE)
	{
		txBuf[0] = 0x08;
		txBuf[1] = 0xFF;
	}
	else if(FPGA_Mode == FPGA_IO_CODE)
	{
		txBuf[0] = 0x00;
		txBuf[1] = 0xFF;
	}
	else
	{
		memset(pRxBuf,1,sizeof(u8)*FPGA_COMMAND_LEN*2); 
	}
	FpgaTR(txBuf, FPGA_COMMAND_LEN, buf+FPGA_COMMAND_LEN, FPGA_COMMAND_LEN, 0);
	FpgaTR(txBuf, FPGA_COMMAND_LEN, buf, FPGA_COMMAND_LEN, 1);
	//memcpy(pRxBuf,buf,sizeof(u8)*FPGA_COMMAND_LEN*2);
}

/**
  * @brief  写O
    
  * @param  no     要控制的O号
      
  * @retval u8 1高电平，0低电平
	 
	* @note   
  */

void FpgaOWrite( u8 no , u8 val)
{
	u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;

	if(/*(num>=0)&&*/(tmpNo<=CDV_EXO_NUM))
	{	
		u8 txBuf[FPGA_COMMAND_LEN] = {0};
		if(FPGA_Mode == FPGA_MOTOR_CODE)
		{
			txBuf[0] = 0x08;
			txBuf[1] = tmpNo;
		}
		else
		{
			txBuf[0] = 0x00;
			txBuf[1] = tmpNo;	
		}
//		txBuf[2] = 0x00;
//		txBuf[3] = 0x00;
//		txBuf[4] = 0x00;
//		txBuf[5] = 0x01 & val;
		if(val == 1)
		{
			txBuf[2] = 0xff;
			txBuf[3] = 0xff;
			txBuf[4] = 0x00;
			txBuf[5] = 0x00;
		}
		else if(val == 0)
		{
			txBuf[2] = 0x00;
			txBuf[3] = 0x00;
			txBuf[4] = 0xff;
			txBuf[5] = 0xff;		
		}
		FpgaTR(txBuf, FPGA_COMMAND_LEN, NULL, 0, fpgaNo);
	}
}


//***************************************************************
//读取FPGA寄存器
//add:寄存器地址
//返回读取到的两个字节数据
//***************************************************************
u32 Read_Fpga_Regist(u16 add,u8 FPGA_NUM)
{
	
	u8 SPI1_Buffer_Rx[6];
 	u32 rec_data=0;
	u32 buf=0;
	
	u8 txBuf[6] = {0};
	txBuf[0] = add>>8;
	txBuf[1] = add;
	txBuf[2] = buf>>24;
	txBuf[3] = buf>>16;
	txBuf[4] = buf>>8;
	txBuf[5] = buf;
	
	FpgaTR(txBuf, 6, SPI1_Buffer_Rx, 6, FPGA_NUM);

	rec_data=(SPI1_Buffer_Rx[1]<<24)+(SPI1_Buffer_Rx[2]<<16)+(SPI1_Buffer_Rx[3]<<8)+SPI1_Buffer_Rx[4];

	return rec_data;
}
#endif

//#endif