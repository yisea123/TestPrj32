
/*FPGA IO AND MOTO FUNCTION*/
#include "FPGA_spi.h"

#define FPGA_ReadWriteByte(A) SPIx_ReadWriteByte(SPI2, (A))


u8 err_times1=0;//���������������¼
u8 err_times2=0;
u16 failed_times=0;
u8 CDV_EXI_NUM = 34;
u8 CDV_EXO_NUM = 30;
u8 FPGA_Mode = FPGA_MOTOR_CODE;//FPGA����ģʽ
//#if ENABLE_FPGA_DOWN
//*******************************************************************
//SPI1��ʼ��
//
//*******************************************************************
//	void SPI1_Init(void)
//{
//	  SPI_InitTypeDef SPI_InitStruct;
//		GPIO_InitTypeDef GPIO_InitStructure;

//		/* ʹ��GPIO ʱ�� */
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOH, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
//		
//		/* ���� SCK, MISO �� MOSI Ϊ���ù��� */
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


//		/* ����Ƭѡ����Ϊ�������ģʽ */
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
//		SF0_CS1_ENABLE;		/* Ƭѡ�øߣ���ѡ�� */
//		SF0_CS2_ENABLE;
//		SF0_CS3_ENABLE;
//		SF0_CS4_ENABLE;
//	}


/** @brief  ��ѯ�ӻ��ظ������ȴ���
  * @param  pBuffer          ��ѯ�����ַ���
	*         NumByteToRead    ��ѯ�ĳ���
  * @retval ����ֵ˵��
  * @note   �����������
  */
void Fpga_Rx(CDV_INT08U* pBuffer, CDV_INT16U NumByteToRead)   
{ 
 	CDV_INT16U i;    		
  OS_ERR err;	
	
	ASSERT(pBuffer);
	
	for(i = 0; i < NumByteToRead; i++)
	{
			pBuffer[i] = FLASH_ReadWriteByte(0X00);   //ѭ������  
	}
}  
/** @brief  ������������
  * @param  pBuffer          ���͵��ַ���
	*         NumByteToWrite   ���͵��ַ�������
  * @retval ����ֵ˵��
  * @note   �����������
  */
void Fpga_Tx(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{
 	CDV_INT16U i;    		
  OS_ERR err;	
	
	ASSERT(pBuffer);
	
	for(i = 0 ; i < NumByteToWrite; i++)
	{
			/*temp[i] = */FLASH_ReadWriteByte(pBuffer[i]);   //ѭ������  
	}
}

/*
no:0-3
��Ӧ
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
/** @brief  �������ͽ��սӿڣ����ڼ����ź���
  * @param  pTxBuf          ���͵��ַ��� NULL ������
	*         txByte          ���͵��ַ�������
  *         pRxBuf          ��ѯ�ɹ�����ӻ��ظ����ַ���
	*         rxLen           ��ѯ����
  *         no              �ڼ�ƬFPGA����0��ʼ
  * @retval ����ֵ˵��
  * @note   �ⲿ��������װ�����������ʹ�øýӿڣ������뼶�����ߵ������豸��Դ�ײ�����ͻ
  */
void FpgaTR(CDV_INT08U* pTxBuf, CDV_INT16U txByte, CDV_INT08U* pRxBuf, CDV_INT16U rxByte, CDV_INT08U no)   
{
  OS_ERR err;
	OSMutexPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	FpgaCS(no);
	
	if(pTxBuf && txByte) {
	  Fpga_Tx(pTxBuf, txByte);
	}
	
	if(pRxBuf && rxByte) {
	  Fpga_Rx(pRxBuf, rxByte);
	}
	FPGA_DS;
	OSMutexPost (&MOTO_SEM,OS_OPT_POST_NO_SCHED,&err);
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
//���Ͳ���
//**************************************
void Send_parameter(u8 *buf)
{
	u8 i;
	
	
	for(i=0;i<3;i++)//���Ͷ�3�ֽ�
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


/*�������*/
//u8 cmdMotorStart[8]    = {0x01 , 0x15 , 0 , 0};
//u8 cmdMotorReturn[8]   = {0x03 , 0x17 , 0 , 0};
//u8 cmdMotorDir[8]      = {0x04 , 0x18 , 0 , 0};
//u8 cmdMotorAcc[8]      = {0x05 , 0x19 , 0 , 0};
//u8 cmdMotorDec[8]      = {0x06 , 0x1a , 0 , 0};
//u8 cmdMotorMaxSpeed[8] = {0x07 , 0x1b , 0 , 0};
//u8 cmdMotorStepH[8]    = {0x08 , 0x1c , 0 , 0};
//u8 cmdMotorStepL[8]    = {0x09 , 0x1d , 0 , 0};
/*Բ���˶�����*/
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
  * @brief  �ȴ�ĳ������λ
    
  * @param  no     Ҫ���Ƶĵ����0-7
      
      
  * @retval u8 1 ��ɣ�0δ���
	 
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
/**************************************************��չIO******************************************************/
//**************************************
//jxd
//FPGAIO��ȡ
//num:I�ţ�0-27��
//
//**************************************
u8 Read_FPGA_I(u8 num)
{
	u8 i;
	u8 buf[3]={0,0,0};
	u8 state[3]={0,0};
	
	buf[0]=0xa3+num;//��ʼ��ַ0xa3
	for(i=0;i<3;i++)//���Ͷ�3�ֽ�
	{  
		FPGA_ReadWriteByte(buf[0]);
	}
	//LED2=1;
	for(i=0;i<3;i++)//���Ͷ�3�ֽ�
	{  
		state[i]=FPGA_ReadWriteByte(0x00);
	}
	buf[0]=0;
	
	return state[1];//�ڶ��ֽ���Ч
	
}
//***************************************
/**
  * @brief  ��ȡi
    
  * @param  no     Ҫ���Ƶ�i��
      
  * @retval u8 1�ߵ�ƽ��0�͵�ƽ
	 
	* @note   
  */

u8 FpgaIRead( u8 no )
{
	//OS_ERR err;
	u8 buf=0;
	u8 tmpNo = no % CDV_EXI_NUM;
	u8 fpgaNo = no / CDV_EXI_NUM;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
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
//	SF0_CS1_DISENABLE;//����ѵڶ���FPGA��Ϊ��չIO
//	SF0_CS2_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS3_DISENABLE;//����ֻʹ���˵ڶ���FPGA
//	SF0_CS2_ENABLE;
	//LED2=0;
	if(/*(no>=0)&&*/(tmpNo<CDV_EXI_NUM))//��չI����30��
	{	
	  buf=Read_FPGA_I(tmpNo);
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return (buf & 0x01);//���λ(bit0)Ϊ��Чλ
	}	
	else
	{
//    OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);		
	  return 2;
	}
}
//**************************************
//jxd
//FPGAO��ȡ
//num:O�ڣ�0-33��
//���ض�ȡ������Ч�ֽ�(�ڶ��ֽ�)
//**************************************
u8 Read_FPGA_O(u8 num)
{
	u8 i;
	u8 buf[3];
	u8 state[2];
	
	buf[0]=0x81+num;//O���׵�ַ0x81
	
	
	for(i=0;i<3;i++)//���Ͷ�3�ֽ�
	{  
		FPGA_ReadWriteByte(buf[i]);
	}
	for(i=0;i<2;i++)//���Ͷ�2�ֽ�
	{  
		state[i]=FPGA_ReadWriteByte(0x00);
	}
	
	return state[1];//�ڶ��ֽڵ�Bit(0)Ϊ��Чλ
}
//*************************************
/**
  * @brief  ��ȡO
    
  * @param  no     Ҫ���Ƶ�O��
      
  * @retval u8 1�ߵ�ƽ��0�͵�ƽ
	 
	* @note   
  */

u8 FpgaORead( u8 no )
{
//	OS_ERR err;
    u8 buf[3] = {0};
    u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
			  
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
//	SF0_CS1_DISENABLE;//����ѵڶ���FPGA��Ϊ��չIO
//	SF0_CS3_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS2_DISENABLE;//����ֻʹ���˵ڶ���FPGA
//	SF0_CS2_ENABLE;//����ֻʹ���˵ڶ���FPGA
	if(/*(no>=0)&&*/(tmpNo<CDV_EXO_NUM))//34��O��
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
//FPGAдO��
//num:Io�ں�1-32
//state����/��
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
    for(i=0;i<3;i++)//���Ͷ�3�ֽ�
	  {  
		  FPGA_ReadWriteByte(buf[i]);
	  }
	}	
	
}
//***************************************

/**
  * @brief  дO
    
  * @param  no     Ҫ���Ƶ�O��
      
  * @retval u8 1�ߵ�ƽ��0�͵�ƽ
	 
	* @note   
  */

void FpgaOWrite( u8 no , u8 val)
{
//	OS_ERR err;
	u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;
	//u8 buf[3] = {0};
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	
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
	
//	SF0_CS1_DISENABLE;//����ѵڶ���FPGA��Ϊ��չIO
//	SF0_CS3_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS2_DISENABLE;//����ֻʹ���˵ڶ���FPGA
//	SF0_CS2_ENABLE;
	PUT_FPGA_O(tmpNo,val);
//	OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
}


//***************************************************************
//��ȡFPGA�Ĵ���
//add:�Ĵ�����ַ
//���ض�ȡ���������ֽ�����
//***************************************************************
u16 Read_Fpga_Regist(u8 add,u8 FPGA_NUM)
{
	  u8 i;
	  u8 SPI1_Buffer_Rx[3]={0};
	  u16 rec_data=0x0000;
//	  OS_ERR err;
//	  OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
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
	
		//���Ͷ�3�ֽڵ�ַ
		FPGA_ReadWriteByte(add);
		FPGA_ReadWriteByte(0x00);
		FPGA_ReadWriteByte(0x00);

		for(i=0;i<3;i++)//����3�ֽڳ���
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
//���Ͳ���
//**************************************
void Send_parameter(u8 *buf)
{
	u8 i;
	for(i=0;i<6;i++)//���Ͷ�6�ֽ�
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


/*�������*/
//u8 cmdMotorStart[8]    = {0x01 , 0x15 , 0 , 0};
//u8 cmdMotorReturn[8]   = {0x03 , 0x17 , 0 , 0};
//u8 cmdMotorDir[8]      = {0x04 , 0x18 , 0 , 0};
//u8 cmdMotorAcc[8]      = {0x05 , 0x19 , 0 , 0};
//u8 cmdMotorDec[8]      = {0x06 , 0x1a , 0 , 0};
//u8 cmdMotorMaxSpeed[8] = {0x07 , 0x1b , 0 , 0};
//u8 cmdMotorStepH[8]    = {0x08 , 0x1c , 0 , 0};
//u8 cmdMotorStepL[8]    = {0x09 , 0x1d , 0 , 0};
/*Բ���˶�����*/
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
  * @brief  �ȴ�ĳ������λ
    
  * @param  no     Ҫ���Ƶĵ����0-7
      
      
  * @retval u8 1 ��ɣ�0δ���
	 
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
/**************************************************��չIO******************************************************/
//**************************************
//jxd
//FPGAIO��ȡ
//num:I�ţ�0-27��
//
//**************************************
u8 Read_FPGA_I(u8 num)
{
	u8 i;
	u8 buf[3]={0,0,0};
	u8 state[3]={0,0};
	buf[0]=0xa3+num;//��ʼ��ַ0xa3
	for(i=0;i<3;i++)//���Ͷ�3�ֽ�
	{  
		FPGA_ReadWriteByte(buf[0]);
	}
	//LED2=1;
	for(i=0;i<3;i++)//���Ͷ�3�ֽ�
	{  
		state[i]=FPGA_ReadWriteByte(0x00);
	}
	buf[0]=0;
	return state[1];//�ڶ��ֽ���Ч
}
//***************************************
/**
  * @brief  ��ȡi
    
  * @param  no     Ҫ���Ƶ�i��
      
  * @retval u8 1�ߵ�ƽ��0�͵�ƽ
	 
	* @note   
  */

u8 FpgaIRead( u8 no )
{
//	OS_ERR err;
	u8 buf=0;
	u8 tmpNo = no % CDV_EXI_NUM;
	u8 fpgaNo = no / CDV_EXI_NUM;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
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
//	SF0_CS1_DISENABLE;//����ѵڶ���FPGA��Ϊ��չIO
//	SF0_CS2_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS3_DISENABLE;//����ֻʹ���˵ڶ���FPGA
//	SF0_CS2_ENABLE;
	//LED2=0;
	if(/*(no>=0)&&*/(tmpNo<CDV_EXI_NUM))//��չI����30��
	{	
	  buf=Read_FPGA_I(tmpNo);
//		OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
		return (buf & 0x01);//���λ(bit0)Ϊ��Чλ
	}	
	else
	{
//    OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);		
	  return 2;
	}
}
//**************************************
//jxd
//FPGAO��ȡ
//num:O�ڣ�0-33��
//���ض�ȡ������Ч�ֽ�(�ڶ��ֽ�)
//**************************************
u8 Read_FPGA_O(u8 num)
{
	u8 i;
	u8 buf[3];
	u8 state[2];
	buf[0]=0x81+num;//O���׵�ַ0x81
	
	for(i=0;i<3;i++)//���Ͷ�3�ֽ�
	{  
		FPGA_ReadWriteByte(buf[i]);
	}
	for(i=0;i<2;i++)//���Ͷ�2�ֽ�
	{  
		state[i]=FPGA_ReadWriteByte(0x00);
	}
	return state[1];//�ڶ��ֽڵ�Bit(0)Ϊ��Чλ
}
//*************************************
/**
  * @brief  ��ȡO
    
  * @param  no     Ҫ���Ƶ�O��
      
  * @retval u8 1�ߵ�ƽ��0�͵�ƽ
	 
	* @note   
  */

u8 FpgaORead( u8 no )
{
//	OS_ERR err;
    u8 buf[3] = {0};
    u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
			  
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
//	SF0_CS1_DISENABLE;//����ѵڶ���FPGA��Ϊ��չIO
//	SF0_CS3_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS2_DISENABLE;//����ֻʹ���˵ڶ���FPGA
//	SF0_CS2_ENABLE;//����ֻʹ���˵ڶ���FPGA
	if(/*(no>=0)&&*/(tmpNo<CDV_EXO_NUM))//34��O��
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
//FPGAдO��
//num:Io�ں�1-32
//state����/��
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
    for(i=0;i<3;i++)//���Ͷ�3�ֽ�
	  {  
		  FPGA_ReadWriteByte(buf[i]);
	  }
	}	
}
//***************************************

/**
  * @brief  дO
    
  * @param  no     Ҫ���Ƶ�O��
      
  * @retval u8 1�ߵ�ƽ��0�͵�ƽ
	 
	* @note   
  */

void FpgaOWrite( u8 no , u8 val)
{
//	OS_ERR err;
	u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;
	//u8 buf[3] = {0};
//	OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	
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
	
//	SF0_CS1_DISENABLE;//����ѵڶ���FPGA��Ϊ��չIO
//	SF0_CS3_DISENABLE;
//	SF0_CS4_DISENABLE;
//	SF0_CS2_DISENABLE;//����ֻʹ���˵ڶ���FPGA
//	SF0_CS2_ENABLE;
	PUT_FPGA_O(tmpNo,val);
//	OSSemPost (&SPI_SEM,OS_OPT_POST_1,&err);
}


//***************************************************************
//��ȡFPGA�Ĵ���
//add:�Ĵ�����ַ
//���ض�ȡ���������ֽ�����
//***************************************************************
u32 Read_Fpga_Regist(u16 add,u8 FPGA_NUM)
{
	  u8 i;
	  u8 SPI1_Buffer_Rx[6];
	  u32 rec_data=0;
	  u32 buf=0;
//	  OS_ERR err;
//	  OSSemPend(&SPI_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
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
	
		//���Ͷ�6�ֽڵ�ַ
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
		for(i=0;i<6;i++)//����3�ֽڳ���
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
//���Ͳ���
//nofpga�� ��0��ʼ
//motoר��
//**************************************
void Send_parameter(u8 no, u8 *buf, u8 len)
{
	FpgaTR(buf, len, NULL, 0, no);
}
//*************************************
//no�����
//motoר��
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
  * @brief  �ȴ�ĳ������λ
    
  * @param  no     Ҫ���Ƶĵ����0-7
      
      
  * @retval u8 1 ��ɣ�0δ���
	 
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
/**************************************************��չIO******************************************************/

/**
  * @brief  ��ȡi
    
  * @param  no     Ҫ���Ƶ�i��
      
  * @retval u8 1�ߵ�ƽ��0�͵�ƽ
	 
	* @note   
  */

u8 FpgaIRead( u8 no )
{
	u8 buf[FPGA_COMMAND_LEN]={0};
	u8 tmpNo = no % CDV_EXI_NUM;
	u8 fpgaNo = no / CDV_EXI_NUM;

	if(/*(no>=0)&&*/(tmpNo<CDV_EXI_NUM))//��չI����30��
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
  * @brief  ��ȡO
    
  * @param  no     Ҫ���Ƶ�O��
      
  * @retval u8 1�ߵ�ƽ��0�͵�ƽ
	 
	* @note   
  */

u8 FpgaORead( u8 no )
{
//	OS_ERR err;
  u8 buf[FPGA_COMMAND_LEN] = {0};
  u8 tmpNo = no % CDV_EXO_NUM;
	u8 fpgaNo = no / CDV_EXO_NUM;

	if(/*(no>=0)&&*/(tmpNo<CDV_EXO_NUM))//34��O��
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

//����fpga o��д
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
  * @brief  дO
    
  * @param  no     Ҫ���Ƶ�O��
      
  * @retval u8 1�ߵ�ƽ��0�͵�ƽ
	 
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
//��ȡFPGA�Ĵ���
//add:�Ĵ�����ַ
//���ض�ȡ���������ֽ�����
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