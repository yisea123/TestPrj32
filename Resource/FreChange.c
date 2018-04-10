#include "FreChange.h"
#include "includes.h" 
CDV_INT08U FreValueFlag[8]={0};  	//ÿ�����ò�����λ��(����CDV����)
CDV_INT08U FlashFlag[8]={0};    //ÿ�����ò�����λ��(����FLASH��ȡ)
int ForwardOrBack;
int WatchFlag;//��ת0����ת1��ֹͣ2
CDV_INT08U *Truf=NULL;
/**
  * @brief  ��Ƶ����������0x10
  *  
  * @param  buf    			�����ű���ַ
  *         FlashAddr  		flash�ű���ŵ�ַ
  *   
  * @retval 
  *
  * @note   
  */
void FrequencySet(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug){
  	//todo���ÿ����λ�����ݲ���������
	CDV_INT16U ValueNum;
	ValueNum = CalculateForAll(buf,3,2);//�м�������Ҫд�룬������Ҫ֪�����ļ���
	SaveCommand(FreValueSum(ValueNum),2,buf,FlashAddr);    
}


/**
  * @brief  ��Ƶ��������ȡ0x11
  *  
  * @param  buf    �����ű���ַ
  *         FlashAddr  flash�ű���ȡ��ַ
  *   
  * @retval 
  *
  * @note   
  */
void FrequencyRead(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug,CDV_INT08U uartNo){
	//todo ���ݵ�λ��ȡ��Ҫ�Ĳ���
	CDV_INT08U Grade,DeviceNo;
	CDV_INT16U ValueNum;
	DeviceNo = buf[0];
	Grade = buf[2];
	//if(Grade==0)return;
	ValueNum = CalculateForAll(buf,3,2);
	FreValueSum(ValueNum);      					//Ϊ�˵õ����Ǽ�������Ҫ��
	ReadCommand(DeviceNo,Grade,ValueNum,2,FlashAddr,0);  //��ȡ ���ǲ�����
	
	ResParaRequest(buf,5,Truf,FreValueSum(ValueNum)*2, uartNo);
	if(Truf!=NULL){
		free(Truf);
		Truf=NULL;
	}
}



/**
  * @brief  ��ת0x12
  *  
  * @param  buf    �����ű���ַ
  *         FlashAddr  flash�ű���ȡ��ַ
  *   
  * @retval 
  *
  * @note   
  */
void FreqcyForward(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug){
	//todo���ݵ�λ��(��ѯ��Ӧ�Ĳ�������)  ��ת
	
// 	 	CDV_INT08U bbuf[240]={0};
// 	CDV_INT08U SlowDown[8]={0x02,0x03,0x00,0x03,0x00,0x01,0x74,0x39};
// 	u8 leeen;
// 	 USART3_Send(SlowDown,8); 
// 	delay_ms(100);
// 	
// 	USART3_Receive_Data(bbuf,&leeen);
	
	
	CDV_INT08U Grade,DeviceNo;
	DeviceNo = buf[0];
	ForwardOrBack=FORWARD;
	switch(buf[2]){
		case 0xFF:
			Grade = CalculateForAll(buf,3,4);
			//if(Grade==0)return;
			if(ReadCommand(DeviceNo,Grade,0,2,FlashAddr,1)!=0){
					setting(DeviceNo,0x06,0x1001,0x02);
					//setting(DeviceNo,0x03,0x1001,0x01);//�����Ƿ�ﵽ�涨Ƶ��
					WatchFlag=0;
			}
			break;
		case 0xFE:
			Grade = VarGet((CalculateForAll(buf,3,2)));//g_modbusReg.TYPE.var[(CalculateForAll(buf,3,2))].valL;
			//if(Grade==0)return;
			if(ReadCommand(DeviceNo,Grade,0,2,FlashAddr,1)!=0){
					setting(DeviceNo,0x06,0x1001,0x02);
			}
			break;
		default:
			break;
	}


}

/**
  * @brief  ��ת0x13
  *  
  * @param  buf    �����ű���ַ
  *         FlashAddr  flash�ű���ȡ��ַ
  *   
  * @retval 
  *
  * @note   
  */
void FreqcyReverse(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug){
	//todo ���ݵ�λ��(��ѯ��Ӧ�Ĳ�������)��ת
	CDV_INT08U Grade,DeviceNo;
	DeviceNo = buf[0];
	ForwardOrBack=BACK;
	switch(buf[2]){
		case 0xFF:
			Grade = CalculateForAll(buf,3,4);
			//if(Grade==0)return;
			if(ReadCommand(DeviceNo,Grade,0,2,FlashAddr,1)!=0){
					setting(DeviceNo,0x06,0x1001,0x04);
					WatchFlag=1;
			}
			break;
		case 0xFE:
			Grade = VarGet((CalculateForAll(buf,3,2)));//Grade = g_modbusReg.TYPE.var[(CalculateForAll(buf,3,2))].valL;
			//if(Grade==0)return;
			if(ReadCommand(DeviceNo,Grade,0,2,FlashAddr,1)!=0){
					setting(DeviceNo,0x06,0x1001,0x04);
			}
			break;
		default:
			break;
	}
}


/**
  * @brief  ֹͣ0x14
  *  
  * @param  buf    �����ű���ַ
  *   
  * @retval 
  *
  * @note   
  */
void FreqcyStop(CDV_INT08U *buf,CDV_INT08U debug){
	//todo ֹͣ(����)
	CDV_INT08U DeviceNo;
	DeviceNo = buf[0];
	setting(DeviceNo,0x06,0x1001,0x01);
	WatchFlag=2;
}

/**
  * @brief  �ȴ� 0x15
  *  
  * @buf    
  *   
  * @retval 
  *
  * @note   
  */

RET_STATUS FreqcyWait(CDV_INT08U *buf,CDV_INT08U debug){

	CDV_INT08U recvbuf[12],len,DeviceNo;
	CDV_INT08U addr1,addr2,data1,data2,crc1,crc2,SlowDown[8],YIChang,FuZai,Arrive,Forward,Reverse,temp;
	CDV_INT16U Data,Addr,CRC1;
	DeviceNo = buf[0];
	Addr=0x1001;
	Data = 0x0001;
	addr1=Addr>>8;
	addr2=Addr;
	data1=Data>>8;
	data2=Data;
	SlowDown[0]=DeviceNo;
	SlowDown[1]=0x03;
	SlowDown[2]=addr1;
	SlowDown[3]=addr2;
	SlowDown[4]=data1;
	SlowDown[5]=data2;
	CRC1 = getCRC16(SlowDown,6);		
	crc1=CRC1;
	crc2=CRC1>>8;
	SlowDown[6]=crc1;
	SlowDown[7]=crc2;
	UART4_Send(SlowDown,8);
	delay_ms(20);
	RS485_Receive_Data(recvbuf,&len);
	temp=recvbuf[4];
	YIChang=(temp>>7)<<7;
	FuZai=(temp>>4)<<7;
	Arrive=(temp>>3)<<7;
	Forward=(temp>>1)<<7;
	Reverse=(temp>>2)<<7;
	switch(WatchFlag){
		case 0:
			if(128==YIChang||128==FuZai)	
				return OPT_FAILURE;
			else if(128 == Arrive&&128==Forward){
				return OPT_SUCCESS;
			} 
			else if(temp==0) return OPT_SUCCESS;
			return OPT_FAILURE;
		//	break;
			
		case 1:
			if(128==YIChang||128==FuZai)	
				return OPT_FAILURE;
			else if(128 == Arrive&&128==Reverse){
				return OPT_SUCCESS;
			}
			return OPT_FAILURE;
		//	break;
			
		case 2:
			if(temp==0) return OPT_SUCCESS;
			
			else return OPT_FAILURE;
			//break;	

	}
//	if(128==YIChang||128==FuZai)	
//		return OPT_FAILURE;
//	else if(128 == Arrive){
//	 
//		return OPT_SUCCESS;
//	} 
//	else if(temp==0) return OPT_SUCCESS;
	return OPT_FAILURE;
}

//�ӱ�Ƶ����ȡ����
//void FreqcyRead(CDV_INT08U *buf,CDV_INT16U Addr){
//	CDV_INT08U DeviceNo;
//	DeviceNo = buf[0];
//	setting(DeviceNo,0x03,Addr,0x01);  //ȥ��Ƶ����Addr�ж�ȡһ���Ĵ���������
//	//uart_receive();
//}



/**
  * @brief  �ҳ��м���Ҫ�޸ĵĲ���,����FreValueFlag[],�����λ��
  *  
  * @param  ValueNum   һ��16λ��ÿһλ��һ��1˵����һ������
  *                           
  * @retval ��
  *
  * @note   
  */

CDV_INT32S FreValueSum(CDV_INT16U ValueNum)
{
	CDV_INT32S AxisNum=0,i=0;
	CDV_INT16U TempNum=0;
	for(i=0;i<16;i++){
		TempNum=(ValueNum>>(15-i))<<15;
		if(32768==TempNum){
			FreValueFlag[AxisNum]=15-i;
			AxisNum++;
		}
	}
	return AxisNum;	
}
/**
  * @brief  �ҳ��м���Ҫ�޸ĵĲ���,����FlashFlag[]�����flash
  *  
  * @param  ValueNum   һ��16λ��ÿһλ��һ��1˵����һ������
  *                           
  * @retval ��
  *
  * @note   
  */

CDV_INT32S FlashSum(CDV_INT16U ValueNum)
{
	CDV_INT32S AxisNum=0,i=0;
	CDV_INT16U TempNum=0;
	for(i=0;i<16;i++){
		TempNum=(ValueNum>>(15-i))<<15;
		if(32768==TempNum){
			FlashFlag[AxisNum]=15-i;
			AxisNum++;
		}
	}
	return AxisNum;	
}


/**
  * @brief  �����ǵڼ��Ų���������Ӧ��ָ��
  *  
  * @param  ValueNo    ���Ų���
  *         SetValue   Ҫ���õĲ�����С                
  * @retval ��
  *
  * @note   ��
  */


void SendToSet(CDV_INT08U DeviceNo,CDV_INT08U ValueNo,CDV_INT16U SetValue){

	switch(ValueNo){

		case 0x00:
			//��תƵ��
			setting(DeviceNo,0x06,0x1009,SetValue);
			break;
		case 0x01:
			//��תƵ��
			setting(DeviceNo,0x06,0x1009,SetValue);
			break;
		case 0x02:
			//��ת����Ƶ��
			setting(DeviceNo,0x06,0x0001,SetValue);
			break;
// 		case 0x03:
// 			//��ת����Ƶ��
// 			setting(DeviceNo,0x06,0x0002,SetValue);
// 			break;
		case 0x03:
			//��ת����Ƶ��	
			setting(DeviceNo,0x06,0x0001,SetValue);
			break;
// 		case 0x05:
// 			//��ת����Ƶ��
// 			setting(DeviceNo,0x06,0x0002,SetValue);
// 			break;
		case 0x04:
			//��ת����ʱ��
			setting(DeviceNo,0x06,0x0007,SetValue); 
			break;
		case 0x06:
			//��ת����ʱ��
			setting(DeviceNo,0x06,0x0008,SetValue);
			break;
		case 0x05:
			//��ת����ʱ��
			setting(DeviceNo,0x06,0x0007,SetValue); 
			break;
		case 0x07:
			//��ת����ʱ��
			setting(DeviceNo,0x06,0x0008,SetValue);
			break;

		default:
			break;


	}

}


/**
  * @brief  �����յ���ָ�FLASH
  *  
  * @param  valuenum    ��������
  *         ValueLen    һ�������ĳ���     
	*					buf					�����ű��Ķ�ȡ��ַ
	*         FlashAddr		FLASH�Ķ�ȡ��ַ
	*					
  * @retval ��
  *
  * @note   valuenum��������õĲ������� 
  */
void SaveCommand(CDV_INT32U valuenum,CDV_INT08U ValueLen,CDV_INT08U* buf,CDV_INT32U FlashAddr){
	CDV_INT08U *CommandBuf=NULL;
	CDV_INT08U BufLength,Grade;
	BufLength = 3+valuenum*ValueLen;
	Grade = buf[2];
	CommandBuf=(CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(BufLength));
	memcpy(CommandBuf, buf + 2, BufLength); 
	SPI_Flash_Write(CommandBuf,FlashAddr+(Grade)*(3+VALUENUM*ValueLen),BufLength); //��һ����λ����Ϣд��FLASH��
	if(CommandBuf!=NULL){
		free(CommandBuf);
		CommandBuf=NULL;
	}

}



/**
  * @brief  ��FLASH���ȡ��Ӧ��λ����Ӧ����
  *  
  * @param  ValueNum    һ��16λ��ÿһλ��һ��1˵����һ��������û����ã�
  *         ValueLen    һ�������ĳ���     
	*         FlashAddr		FLASH�Ķ�ȡ��ַ
	*					Flag				��־Ϊ1 ��ȡ�Ĳ�����Ҫ���͸���Ƶ����0 �򲻷���
	*					
  * @retval ��
  *
  * @note   Valuenum ��û�����
  */
CDV_INT32S ReadCommand(CDV_INT08U DeviceNo,CDV_INT08U Grade,CDV_INT16U ValueNum,CDV_INT08U ValueLen,CDV_INT32U FlashAddr,CDV_INT08U Flag){
	CDV_INT08U tempbuf[9]={0};
	CDV_INT08U buf[4]={0};
	CDV_INT32U i,j,Num,NeedNum,Offset;
	Truf = (CDV_INT08U *)malloc(sizeof(CDV_INT08U)*ValueNum*ValueLen);
	switch(Flag){
		case 0x00:
				while(1){	
					Offset=(Grade)*(3+VALUENUM*ValueLen);
					SPI_Flash_Read(tempbuf,FlashAddr+Offset,9);
					Num = FlashSum(CalculateForAll(tempbuf,1,2));
					NeedNum = FreValueSum(ValueNum);
					if(tempbuf[0]==Grade){      																		//�ҵ���Ӧ��λ
							for( i=0;i<Num;i++){								//FLASH�иõ�λ���в���
									for( j=0;j<NeedNum;j++){    									//�ҳ���Ҫ��ȡ�Ĳ���
											if(FlashFlag[i]==FreValueFlag[j]){
													Offset=(Grade)*(3+VALUENUM*ValueLen)+3+FlashFlag[i]*ValueLen;
													SPI_Flash_Read(buf,FlashAddr+Offset,ValueLen);	//����ط���Ҫ����ֵ��CDV������
													memcpy(Truf+(NeedNum-1-j)*ValueLen,buf,ValueLen);
											}
									}
							}
							break;
					}
					else return 0;
			}
				
				break;
			
		case 0x01:
				while(1){
						Offset=(Grade)*(3+VALUENUM*ValueLen);
						SPI_Flash_Read(tempbuf,FlashAddr+Offset,9);
						if(tempbuf[0]==Grade){  																				//�ҵ���Ӧ��λ
								Num=FlashSum(CalculateForAll(tempbuf,1,2));
								for(i=0;i<Num;i++){
										if(ForwardOrBack==FORWARD){
											if((i%2)!=0)	continue;
										}
										else if(ForwardOrBack==BACK){
											if((i%2)==0)	continue;
										}
										Offset=(Grade)*(3+VALUENUM*ValueLen)+3+i*ValueLen;
										SPI_Flash_Read(buf,FlashAddr+Offset,ValueLen);					//����ط���Ҫ����ֵ��CDV������
										SendToSet(DeviceNo,FlashFlag[Num-1-i],CalculateForAll(buf,0,ValueLen));
								}
								break;
						}
						else return 0;
				}			
				break;
	}
	return 1;
}

/**
  * @brief  ������Ҫ���͵Ĳ���
  *  
  * @param  DeviceNo    �豸��
  *         ActionNo    ���ܺ�   
	*         Addr				��Ƶ���ļĴ�����ַ
	*					Data				.
	*					
  * @retval ��
  *
  * @note   
  */

void setting(CDV_INT08U DeviceNo,CDV_INT08U ActionNo,CDV_INT16U Addr,CDV_INT16U Data){	//CDV_INT16U CRC=0;
	CDV_INT16U CRC1;
	CDV_INT08U addr1,addr2,data1,data2,crc1,crc2,SlowDown[8],bbbbuf[20],lennn;
	addr1=Addr>>8;
	addr2=Addr;
	data1=Data>>8;
	data2=Data;
	SlowDown[0]=DeviceNo;
	SlowDown[1]=ActionNo;
	SlowDown[2]=addr1;
	SlowDown[3]=addr2;
	SlowDown[4]=data1;
	SlowDown[5]=data2;
	CRC1 = getCRC16(SlowDown,6);		
	crc1=CRC1;
	crc2=CRC1>>8;
	SlowDown[6]=crc1;
	SlowDown[7]=crc2;
	//usart_send(SlowDown,8);
	//USART3_Send(SlowDown,8);
	UART4_Send(SlowDown,8);
	delay_ms(20);
	//USART3_Receive_Data(bbbbuf,&lennn);
	RS485_Receive_Data(bbbbuf,&lennn);
}




