#include "FreChange.h"
#include "includes.h" 
CDV_INT08U FreValueFlag[8]={0};  	//每个设置参数的位置(来自CDV接收)
CDV_INT08U FlashFlag[8]={0};    //每个设置参数的位置(来自FLASH读取)
int ForwardOrBack;
int WatchFlag;//正转0，反转1，停止2
CDV_INT08U *Truf=NULL;
/**
  * @brief  变频器参数设置0x10
  *  
  * @param  buf    			操作脚本地址
  *         FlashAddr  		flash脚本存放地址
  *   
  * @retval 
  *
  * @note   
  */
void FrequencySet(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug){
  	//todo存放每个档位的数据并发送设置
	CDV_INT16U ValueNum;
	ValueNum = CalculateForAll(buf,3,2);//有几个参数要写入，但不需要知道是哪几个
	SaveCommand(FreValueSum(ValueNum),2,buf,FlashAddr);    
}


/**
  * @brief  变频器参数读取0x11
  *  
  * @param  buf    操作脚本地址
  *         FlashAddr  flash脚本读取地址
  *   
  * @retval 
  *
  * @note   
  */
void FrequencyRead(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug,CDV_INT08U uartNo){
	//todo 根据档位读取需要的参数
	CDV_INT08U Grade,DeviceNo;
	CDV_INT16U ValueNum;
	DeviceNo = buf[0];
	Grade = buf[2];
	//if(Grade==0)return;
	ValueNum = CalculateForAll(buf,3,2);
	FreValueSum(ValueNum);      					//为了得到有那几个变量要读
	ReadCommand(DeviceNo,Grade,ValueNum,2,FlashAddr,0);  //读取 但是不发送
	
	ResParaRequest(buf,5,Truf,FreValueSum(ValueNum)*2, uartNo);
	if(Truf!=NULL){
		free(Truf);
		Truf=NULL;
	}
}



/**
  * @brief  正转0x12
  *  
  * @param  buf    操作脚本地址
  *         FlashAddr  flash脚本读取地址
  *   
  * @retval 
  *
  * @note   
  */
void FreqcyForward(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug){
	//todo根据档位号(查询相应的参数设置)  正转
	
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
					//setting(DeviceNo,0x03,0x1001,0x01);//检验是否达到规定频率
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
  * @brief  反转0x13
  *  
  * @param  buf    操作脚本地址
  *         FlashAddr  flash脚本读取地址
  *   
  * @retval 
  *
  * @note   
  */
void FreqcyReverse(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug){
	//todo 根据档位号(查询相应的参数设置)反转
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
  * @brief  停止0x14
  *  
  * @param  buf    操作脚本地址
  *   
  * @retval 
  *
  * @note   
  */
void FreqcyStop(CDV_INT08U *buf,CDV_INT08U debug){
	//todo 停止(保留)
	CDV_INT08U DeviceNo;
	DeviceNo = buf[0];
	setting(DeviceNo,0x06,0x1001,0x01);
	WatchFlag=2;
}

/**
  * @brief  等待 0x15
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

//从变频器读取参数
//void FreqcyRead(CDV_INT08U *buf,CDV_INT16U Addr){
//	CDV_INT08U DeviceNo;
//	DeviceNo = buf[0];
//	setting(DeviceNo,0x03,Addr,0x01);  //去变频器的Addr中读取一个寄存器的内容
//	//uart_receive();
//}



/**
  * @brief  找出有几个要修改的参数,存入FreValueFlag[],针对上位机
  *  
  * @param  ValueNum   一个16位，每一位有一个1说明有一个参数
  *                           
  * @retval 无
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
  * @brief  找出有几个要修改的参数,存入FlashFlag[]，针对flash
  *  
  * @param  ValueNum   一个16位，每一位有一个1说明有一个参数
  *                           
  * @retval 无
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
  * @brief  根据是第几号参数发送相应的指令
  *  
  * @param  ValueNo    几号参数
  *         SetValue   要设置的参数大小                
  * @retval 无
  *
  * @note   无
  */


void SendToSet(CDV_INT08U DeviceNo,CDV_INT08U ValueNo,CDV_INT16U SetValue){

	switch(ValueNo){

		case 0x00:
			//正转频率
			setting(DeviceNo,0x06,0x1009,SetValue);
			break;
		case 0x01:
			//反转频率
			setting(DeviceNo,0x06,0x1009,SetValue);
			break;
		case 0x02:
			//正转上限频率
			setting(DeviceNo,0x06,0x0001,SetValue);
			break;
// 		case 0x03:
// 			//正转下限频率
// 			setting(DeviceNo,0x06,0x0002,SetValue);
// 			break;
		case 0x03:
			//反转上限频率	
			setting(DeviceNo,0x06,0x0001,SetValue);
			break;
// 		case 0x05:
// 			//反转下限频率
// 			setting(DeviceNo,0x06,0x0002,SetValue);
// 			break;
		case 0x04:
			//正转加速时间
			setting(DeviceNo,0x06,0x0007,SetValue); 
			break;
		case 0x06:
			//正转减速时间
			setting(DeviceNo,0x06,0x0008,SetValue);
			break;
		case 0x05:
			//反转加速时间
			setting(DeviceNo,0x06,0x0007,SetValue); 
			break;
		case 0x07:
			//反转减速时间
			setting(DeviceNo,0x06,0x0008,SetValue);
			break;

		default:
			break;


	}

}


/**
  * @brief  保存收到的指令到FLASH
  *  
  * @param  valuenum    参数个数
  *         ValueLen    一个参数的长度     
	*					buf					操作脚本的读取地址
	*         FlashAddr		FLASH的读取地址
	*					
  * @retval 无
  *
  * @note   valuenum是真正算好的参数个数 
  */
void SaveCommand(CDV_INT32U valuenum,CDV_INT08U ValueLen,CDV_INT08U* buf,CDV_INT32U FlashAddr){
	CDV_INT08U *CommandBuf=NULL;
	CDV_INT08U BufLength,Grade;
	BufLength = 3+valuenum*ValueLen;
	Grade = buf[2];
	CommandBuf=(CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(BufLength));
	memcpy(CommandBuf, buf + 2, BufLength); 
	SPI_Flash_Write(CommandBuf,FlashAddr+(Grade)*(3+VALUENUM*ValueLen),BufLength); //把一个档位的信息写到FLASH中
	if(CommandBuf!=NULL){
		free(CommandBuf);
		CommandBuf=NULL;
	}

}



/**
  * @brief  从FLASH里读取相应档位的相应参数
  *  
  * @param  ValueNum    一个16位，每一位有一个1说明有一个参数（没有算好）
  *         ValueLen    一个参数的长度     
	*         FlashAddr		FLASH的读取地址
	*					Flag				标志为1 读取的参数需要发送给变频器；0 则不发送
	*					
  * @retval 无
  *
  * @note   Valuenum 还没有算好
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
					if(tempbuf[0]==Grade){      																		//找到相应档位
							for( i=0;i<Num;i++){								//FLASH中该档位所有参数
									for( j=0;j<NeedNum;j++){    									//找出需要读取的参数
											if(FlashFlag[i]==FreValueFlag[j]){
													Offset=(Grade)*(3+VALUENUM*ValueLen)+3+FlashFlag[i]*ValueLen;
													SPI_Flash_Read(buf,FlashAddr+Offset,ValueLen);	//这个地方需要返回值给CDV开发层
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
						if(tempbuf[0]==Grade){  																				//找到相应档位
								Num=FlashSum(CalculateForAll(tempbuf,1,2));
								for(i=0;i<Num;i++){
										if(ForwardOrBack==FORWARD){
											if((i%2)!=0)	continue;
										}
										else if(ForwardOrBack==BACK){
											if((i%2)==0)	continue;
										}
										Offset=(Grade)*(3+VALUENUM*ValueLen)+3+i*ValueLen;
										SPI_Flash_Read(buf,FlashAddr+Offset,ValueLen);					//这个地方需要返回值给CDV开发层
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
  * @brief  设置需要发送的参数
  *  
  * @param  DeviceNo    设备号
  *         ActionNo    功能号   
	*         Addr				变频器的寄存器地址
	*					Data				.
	*					
  * @retval 无
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




