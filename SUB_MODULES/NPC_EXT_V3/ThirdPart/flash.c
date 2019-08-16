

#include "flash.h"

CDV_INT08U CHECK_FLASH[5] ={0x01,0x02,0x03,0x04,0x05};
//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25X16
//����Ϊ2M�ֽ�,����32��Block,512��Sector 

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {

  }
}


///*SPI��д
//1.�ȴ�SPI TX��
//2.��������
//3.�ȴ�SPI TX��
//4.�ȴ�SPI RX�ǿ�
//5.��ʱ
//6.��������*/
//CDV_INT08U FPGAoldx_ReadWriteByte(SPI_TypeDef* SPIx, CDV_INT08U TxData)
//{
//	uint16_t ret;
//	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==RESET);
//	SPI_I2S_SendData(SPIx,TxData);
//  while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==RESET);
//	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)==RESET);
//	ret = SPI_I2S_ReceiveData(SPIx);
//	return ret;
//}
//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	FLASH_EN;                            //ʹ������   
	FLASH_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=FLASH_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	FLASH_DS;                            //ȡ��Ƭѡ     
	return byte;   
} 
//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void SPI_FLASH_Write_SR(u8 sr)   
{   
	FLASH_EN;                            //ʹ������   
	FLASH_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	FLASH_ReadWriteByte(sr);               //д��һ���ֽ�  
	FLASH_DS;                            //ȡ��Ƭѡ     	      
}   
//SPI_FLASHдʹ��	
//��WEL��λ   
void SPI_FLASH_Write_Enable(void)   
{
	FLASH_EN;                            //ʹ������   
  FLASH_ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
	FLASH_DS;                            //ȡ��Ƭѡ     	      
} 
//SPI_FLASHд��ֹ	
//��WEL����  
void SPI_FLASH_Write_Disable(void)   
{  
	FLASH_EN;                            //ʹ������   
  FLASH_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��    
	FLASH_DS;                            //ȡ��Ƭѡ     	      
} 			    
//��ȡоƬID W25Q64��ID:0XEF16
u16 SPI_Flash_ReadID(void)
{
	u16 Temp = 0;	  
	FLASH_EN;				    
	FLASH_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	FLASH_ReadWriteByte(0x00); 	    
	FLASH_ReadWriteByte(0x00); 	    
	FLASH_ReadWriteByte(0x00); 	 			   
	Temp|=FLASH_ReadWriteByte(0xFF)<<8;  
	Temp|=FLASH_ReadWriteByte(0xFF);	 
	FLASH_DS;				    
	return Temp;
} 
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
//void FPGA_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
//{ 
// 	u16 i;    		
//	FLASH_EN;                            //ʹ������   
//	FPGAFLASH_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
//	FPGAFLASH_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
//	FPGAFLASH_ReadWriteByte((u8)((ReadAddr)>>8));   
//	FPGAFLASH_ReadWriteByte((u8)ReadAddr);
//	for(i=0;i<NumByteToRead;i++)
//	{
//		pBuffer[i]=FPGAFLASH_ReadWriteByte(0XFF);   //ѭ������  
//	}
//	FLASH_DS;                            //ȡ��Ƭѡ     	      
//}  
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    		
  OS_ERR err;	
  OSMutexPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	SPEED_FLASH;
	FLASH_EN;                            //ʹ������  
  
	FLASH_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
	FLASH_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
	FLASH_ReadWriteByte((u8)((ReadAddr)>>8));   
	FLASH_ReadWriteByte((u8)ReadAddr);
	for(i=0;i<NumByteToRead;i++)
	{
			pBuffer[i]=FLASH_ReadWriteByte(0XFF);   //ѭ������  
	}
	FLASH_DS;                            //ȡ��Ƭѡ   
	SPEED_FPGA;
	OSMutexPost (&MOTO_SEM,OS_OPT_POST_NO_SCHED,&err);
}  
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void Org_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    		
  SPEED_FLASH;
	FLASH_EN;                            //ʹ������   
	FLASH_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
	FLASH_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
	FLASH_ReadWriteByte((u8)((ReadAddr)>>8));   
	FLASH_ReadWriteByte((u8)ReadAddr);
	for(i=0;i<NumByteToRead;i++)
	{
			pBuffer[i]=FLASH_ReadWriteByte(0XFF);   //ѭ������  
	}
	FLASH_DS;                            //ȡ��Ƭѡ
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
	
  SPI_FLASH_Write_Enable();                  //SET WEL 
	FLASH_EN;                            //ʹ������   
  FLASH_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
  FLASH_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
  FLASH_ReadWriteByte((u8)((WriteAddr)>>8));   
  FLASH_ReadWriteByte((u8)WriteAddr);   
  for(i=0;i<NumByteToWrite;i++)FLASH_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	FLASH_DS;                            //ȡ��Ƭѡ 
	SPI_Flash_Wait_Busy();					   //�ȴ�д�����
	
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)  		   
//
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ //u8 SPI_FLASH_BUF[4096];
	u32 secpos;
	u16 secoff;
	u16 secremain;
 	u16 i;
	u8* SPI_FLASH_BUF = UserMemPtr(CCM_FLASH_WRITE_BUF);//�������
	OS_ERR err;
	
	if(g_noWrite)
		return;
	
	OSMutexPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	//PWR_PVDCmd(DISABLE);
	SPEED_FLASH;
	//OSSemPend(&WORKER_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
	secpos=WriteAddr/4096;//������ַ 0~511 for w25x64
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С
	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1)
	{
		Org_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//��Ҫ����
		}
		if(i<secremain)//��Ҫ����
		{
			SPI_Flash_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//д����������

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.		   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0

		  pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		  NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 	 
	//OSSemPost (&WORKER_SEM,OS_OPT_POST_1,&err);
	SPEED_FPGA;
	//PWR_PVDCmd(ENABLE);
	OSMutexPost (&MOTO_SEM,OS_OPT_POST_NO_SCHED,&err);
}
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)  		   
//PVD����
void Org_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
 	u16 i;
	u8* SPI_FLASH_BUF = UserMemPtr(CCM_FLASH_WRITE_BUF);//�������
		
	if(g_noWrite)
		return;
	
	SPEED_FLASH;
	secpos=WriteAddr/4096;//������ַ 0~511 for w25x64
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С
	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1)
	{
		Org_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//��Ҫ����
		}
		if(i<secremain)//��Ҫ����
		{
			SPI_Flash_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//д����������

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.		   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0

		  pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		  NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 	 
	//OSSemPost (&WORKER_SEM,OS_OPT_POST_1,&err);
	SPEED_FPGA;
}
//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//�ȴ�ʱ�䳬��...
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   
  	FLASH_EN;                            //ʹ������   
    FLASH_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	FLASH_DS;                            //ȡ��Ƭѡ     	      
	SPI_Flash_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   
  	FLASH_EN;                            //ʹ������   
    FLASH_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
    FLASH_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    FLASH_ReadWriteByte((u8)((Dst_Addr)>>8));   
    FLASH_ReadWriteByte((u8)Dst_Addr);  
	FLASH_DS;                            //ȡ��Ƭѡ     	      
    SPI_Flash_Wait_Busy();   				   //�ȴ��������
}  
//�ȴ�����
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  	FLASH_EN;                            //ʹ������   
    FLASH_ReadWriteByte(W25X_PowerDown);        //���͵�������  
	  FLASH_DS;                            //ȡ��Ƭѡ     	      
    Delay(6);                               //�ȴ�TPD  
}   
//����
void SPI_Flash_WAKEUP(void)   
{  
  	FLASH_EN;                            //ʹ������   
    FLASH_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	  FLASH_DS;                            //ȡ��Ƭѡ     	      
    Delay(6);                              //�ȴ�TRES1
}   



//void FlashTR(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite, u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
//{
//	
//	
//}

///** @brief  ��ѯ�ӻ��ظ������ȴ���
//  * @param  pBuffer          ��ѯ�����ַ���
//	*         NumByteToRead    ��ѯ�ĳ���
//  * @retval ����ֵ˵��
//  * @note   �����������
//  */
//void Flash_Rx(CDV_INT08U* pBuffer, CDV_INT16U NumByteToRead)   
//{ 
// 	CDV_INT16U i;    		
//  OS_ERR err;	
//	
//	ASSERT(pBuffer);
//	
//	FLASH_EN;                            //ʹ������  
//	
//	for(i = 0; i < NumByteToRead; i++)
//	{
//			pBuffer[i] = FLASH_ReadWriteByte(0XFF);   //ѭ������  
//	}
//	FLASH_DS;                            //ȡ��Ƭѡ   
//}  
///** @brief  ������������
//  * @param  pBuffer          ���͵��ַ���
//	*         NumByteToWrite   ���͵��ַ�������
//  * @retval ����ֵ˵��
//  * @note   �����������
//  */
//void Flash_Tx(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
//{
// 	CDV_INT16U i;    		
//  OS_ERR err;	
//	
//	ASSERT(pBuffer);
//	
//	FLASH_EN;                            //ʹ������  
//	for(i = 0 ; i < NumByteToWrite; i++)
//	{
//			/*temp[i] = */FLASH_ReadWriteByte(pBuffer[i]);   //ѭ������  
//	}
//	FLASH_DS;                            //ȡ��Ƭѡ   
//}

///** @brief  �������ͽ��սӿڣ����ڼ����ź���
//  * @param  pTxBuf          ���͵��ַ��� NULL ������
//	*         txByte          ���͵��ַ�������
//  *         pRxBuf         ��ѯ�ɹ�����ӻ��ظ����ַ���
//	*         rxLen          ��ѯ����
//  * @retval ����ֵ˵��
//  * @note   �ⲿ��������װ�����������ʹ�øýӿڣ������뼶�����ߵ������豸��Դ�ײ�����ͻ
//  */
//void FlashTR(CDV_INT08U* pTxBuf, CDV_INT16U txByte, CDV_INT08U* pRxBuf, CDV_INT16U rxLen)   
//{
//  OS_ERR err;	

//	FLASH_EN;
//	
//	if(pTxBuf && txByte) {
//	  Flash_Tx(pTxBuf, txByte);
//	}
//	
//	if(pRxBuf && rxLen) {
//	  Flash_Rx(pRxBuf, rxLen);
//	}
//	
//	FLASH_DS;

//}

///*************************************************************/
//void FlashRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
//{ 
// 	u16 i;    		
//  
//	CDV_INT08U txBuf[4] = {W25X_ReadData, (u8)((ReadAddr)>>16), (u8)((ReadAddr)>>8), (u8)ReadAddr};
//  
//	FlashTR(txBuf, 4, pBuffer, NumByteToRead);
////	FLASH_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
////	FLASH_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
////	FLASH_ReadWriteByte((u8)((ReadAddr)>>8));   
////	FLASH_ReadWriteByte((u8)ReadAddr);
////	for(i=0;i<NumByteToRead;i++)
////	{
////			pBuffer[i]=FLASH_ReadWriteByte(0XFF);   //ѭ������  
////	}
//	
//}  

//void FlashWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
//{
//	u32 secpos;
//	u16 secoff;
//	u16 secremain;
// 	u16 i;
//	
//	secpos=WriteAddr/4096;//������ַ 0~511 for w25x64
//	secoff=WriteAddr%4096;//�������ڵ�ƫ��
//	secremain=4096-secoff;//����ʣ��ռ��С
//	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
//	while(1)
//	{
//		
//		FlashRead(SPI_FLASH_BUF,secpos*4096,4096);//������������������
//		for(i=0;i<secremain;i++)//У������
//		{
//			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//��Ҫ����
//		}
//		if(i<secremain)//��Ҫ����
//		{
//			SPI_Flash_Erase_Sector(secpos);//�����������
//			for(i=0;i<secremain;i++)	   //����
//			{
//				SPI_FLASH_BUF[i+secoff]=pBuffer[i];
//			}
//			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//д����������

//		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.		   
//		if(NumByteToWrite==secremain)break;//д�������
//		else//д��δ����
//		{
//			secpos++;//������ַ��1
//			secoff=0;//ƫ��λ��Ϊ0

//		  pBuffer+=secremain;  //ָ��ƫ��
//			WriteAddr+=secremain;//д��ַƫ��	   
//		  NumByteToWrite-=secremain;				//�ֽ����ݼ�
//			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
//			else secremain=NumByteToWrite;			//��һ����������д����
//		}	 
//	};	 	 
//	
//}
int Flash_Check(void)
{
	int i;
 CDV_INT08U Readbuf[5]={0};
 SPI_Flash_Write((CDV_INT08U *)&CHECK_FLASH,CHECK_FLASH_ADDR , sizeof(CHECK_FLASH));
 delay_ms(1);
 SPI_Flash_Read((CDV_INT08U *)&Readbuf,CHECK_FLASH_ADDR ,sizeof(CHECK_FLASH));
  for( i=0;i<5;i++)
	{
	 if (CHECK_FLASH[i] == Readbuf[i])
		 Alarmflag = 0;
	 else
		 Alarmflag = 1;
	 if(Alarmflag == 1)
		 break;
	 }		
	return Alarmflag;
}