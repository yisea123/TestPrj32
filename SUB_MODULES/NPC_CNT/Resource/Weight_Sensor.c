#include "Weight_Sensor.h"

//**********************************
//设置称重传感器分度值
//ID:称重传感器ID号（1-32）
//dataH:高位
//dataL:地位
//**********************************
void Set_Weight_Sensor_resolution(u8 ID,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x06,0x00,0x0e,0x00,0x05,0,0};
	if((ID>32)||(ID<1))
		send_buf[0]=0x01;
	else
	  send_buf[0]=ID;
	send_buf[4]=dataH;//默认值
	send_buf[5]=dataL;
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,8);
}
//**********************************
//设置称重传感器波特率值
//ID:称重传感器ID号（1-32）
//dataH:高位
//dataL:地位
//**********************************
void Set_Weight_Sensor_Baudrate(u8 ID,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x06,0x00,0x0b,0x00,0x05,0,0};
	if((ID>32)||(ID<1))
		send_buf[0]=0x01;
	else
	  send_buf[0]=ID;
	send_buf[4]=dataH;//默认值
	send_buf[5]=dataL;
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,8);
}
//**********************************
//设置称重传感器滤波长度
//ID:称重传感器ID号（1-32）
//dataH:高位
//dataL:地位
//**********************************
void Set_Weight_Sensor_Smoothinglens(u8 ID,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x06,0x00,0x0c,0x00,0x01,0x9c,0x06};
	if((ID>32)||(ID<1))
		send_buf[0]=0x01;
	else
	  send_buf[0]=ID;
	if(((dataH<<8)+dataL)>30)
	{	
		dataH=0;
	  dataL=0x01;
	}	
	send_buf[4]=dataH;//默认值
	send_buf[5]=dataL;
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,8);
}
//**********************************
//设置称重传感器去皮操作
//ID:称重传感器ID号（1-32）
//dataH:高位
//dataL:地位
//**********************************
void Set_Weight_Sensor_netweight(u8 ID,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[8]={0x01,0x06,0x00,0x11,0x00,0x01,0x18,0x0f};
	if((ID>32)||(ID<1))
		send_buf[0]=0x01;
	else
	  send_buf[0]=ID;
	send_buf[4]=dataH;//默认值
	send_buf[5]=dataL;
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,8);
}
//**********************************
//设置称重传感器速度
//ID:称重传感器ID号（1-32）
//dataH:高位
//dataL:地位
//**********************************
void Set_Weight_Sensor_Speed(u8 ID,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x06,0x00,0x10,0x00,0x00,0x9c,0x00};
	if((ID>32)||(ID<1))
		send_buf[0]=0x01;
	else
	  send_buf[0]=ID;
	if(((dataH<<8)+dataL)>1)
	{	
		dataH=0;
	  dataL=0x00;
	}	
	send_buf[4]=dataH;//默认值
	send_buf[5]=dataL;
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,8);
}
//**********************************
//称重传感器快速标定无负载
//ID:称重传感器ID号（1-32）
//**********************************
void Set_Weight_Sensor_Standard_Zreo(u8 ID)
{
  u16 data;
	u8 send_buf[8]={0x01,0x06,0x00,0x12,0x00,0x01,0xE8,0x0F};
	if((ID>32)||(ID<1))
		send_buf[0]=0x01;
	else
	  send_buf[0]=ID;
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,8);
	delay_ms(200);
}
//**********************************
//称重传感器快速标定有负载
//ID:称重传感器ID号（1-32）
//**********************************
void Set_Weight_Sensor_Standard(u8 ID,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[8]={0x01,0x06,0x00,0x12,0x00,0x02,0xE8,0x0F};
	u8 send_buf2[13]={0x01,0x10,0x00,0x08,0x00,0x02,0x04,0x0B,0xB8,0x00,0x00,0x43,0x1F};
	if((ID>32)||(ID<1))
		send_buf[0]=0x01;
	else
	  send_buf[0]=ID;
	send_buf[5]=0x02;
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,8);
	delay_ms(1000);
	delay_ms(1000);
	send_buf2[0]=ID;
	send_buf2[7]=dataH;
	send_buf2[8]=dataL;
	data=getCRC16(send_buf2,11);
	send_buf2[11]=data & 0x00ff;
  send_buf2[12]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf2,13);
	delay_ms(200);
//	send_buf[3]=0x09;
//	send_buf[4]=dataH;
//	send_buf[5]=dataL;
//	data=getCRC16(send_buf,6);
//	send_buf[6]=data & 0x00ff;
//  send_buf[7]=(data >> 8) & 0x00ff;
//  USART3_Send(send_buf,8);
//	delay_ms(200);
}
//**********************************
//
//ID:
//Para_No:参数号

//**********************************
void Read_Weight_Sensor_Para(u8 ID,u8 Para_No)
{
	u16 data;       
	u8 send_buf[8]={0x01,0x03,0x00,0x00,0x00,0x02,0xc4,0x0b};
	if((ID>=1)&&(ID<=32))
	{
		send_buf[0]=ID;
		if(Para_No>1)
		   send_buf[5]=1;//读取长度 Word
		else
				send_buf[5]=2;		
		if(Para_No==1)//重量值寄存器
		{
			 send_buf[2]=0x00;
			 send_buf[3]=0x00;
		}
		if(Para_No==2)//分度值寄存器地址
		{
			 send_buf[2]=0x00;
			 send_buf[3]=0x0e;
		}
		if(Para_No==3)//波特率寄存器地址
		{
			 send_buf[2]=0x00;
			 send_buf[3]=0x0b;
		}
		if(Para_No==4)//速度寄存器地址
		{
			 send_buf[2]=0x00;
			 send_buf[3]=0x10;
		}
		if(Para_No==5)//滤波长度寄存器地址
		{
			 send_buf[2]=0x00;
			 send_buf[3]=0x0c;
		}
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}	
}
//****************************************************
//读取称重值和参数
//
//ID:ID号（1-32）
//addr:参数标记位
//****************************************************
u8 Read_Weight_Sensor_Inf(u8 DG_id,u16 addr)
{
	u8 i,j;
 	u8 REC_BUF[16]={0};
	u32 REC_BUF_LEN=0;
	i=0;
	if((DG_id<=32)&&(DG_id>=1))
	{	
		 if(addr & 0x0001)//当前重量
		 {
				Read_Weight_Sensor_Para(DG_id,1);
				delay_ms(500);
				USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
			  Para_Buf[0]=REC_BUF[5];
			  Para_Buf[1]=REC_BUF[6];
			  Para_Buf[2]=REC_BUF[3];
			  Para_Buf[3]=REC_BUF[4];
				i=i+4;			
		 }
		 if(addr & 0x0002)//分度值
		 {
				Read_Weight_Sensor_Para(DG_id,2);
				delay_ms(500);
				USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 
		 if(addr & 0x0004)//波特率
		 {
				Read_Weight_Sensor_Para(DG_id,3);
				delay_ms(500);
				USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0008)//采样速度
		 {
				Read_Weight_Sensor_Para(DG_id,4);
				delay_ms(500);
				USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0010)//滤波长度
		 {
				Read_Weight_Sensor_Para(DG_id,5);
				delay_ms(500);
				USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
	}	 
  REC_BUF_LEN=0;		 
	return i;//返回长度
}
//********************************************************
//
//称重传感器参数设置
//DG_id:设备id
//DG_pflag:参数位标记
//para_pos:参数起始字节
//rxBuf:指令队列(包含设备号、参数标记位、参数内容等)
//********************************************************
u8 Set_Weight_Sensor_Par(u8 DG_id,u8 DG_pflag,u8 para_pos,u8* rxBuf )
{
	 u8 i=0;
	   if(DG_pflag & 0x0001)//标定0
		 {
				i=0;
				Set_Weight_Sensor_Standard_Zreo(DG_id);
				i=i+2;
				delay_ms(100);
		 }
		 if(DG_pflag & 0x0002)//标定加负载
		 {
				Set_Weight_Sensor_Standard(DG_id,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
				i=i+2;
				delay_ms(100);
		 }
		 if(DG_pflag & 0x0004)//分度值
		 {

				Set_Weight_Sensor_resolution(DG_id,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
				i=i+2;
				delay_ms(100);
		 }
		 if(DG_pflag & 0x0008)//波特率
		 {

				Set_Weight_Sensor_Baudrate(DG_id,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
				i=i+2;
				delay_ms(100);
		 }
		 if(DG_pflag & 0x0010)//采样速度
		 {
				Set_Weight_Sensor_Speed(DG_id,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
				i=i+2;
			 delay_ms(100);
		 }
		 if(DG_pflag & 0x0020)//滤波长度
		 {
				Set_Weight_Sensor_Smoothinglens(DG_id,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
				i=i+2;
				delay_ms(100);
		 }
		 if(DG_pflag & 0x0040)//去皮操作
		 {
				Set_Weight_Sensor_netweight(DG_id,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
				i=i+2;
				delay_ms(100);
		 }
		 return 2;
}							 


