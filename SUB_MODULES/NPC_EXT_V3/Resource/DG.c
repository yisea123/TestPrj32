
#include "DG.h"
//#include "rs485.h"
//#include "uart5.h"
//#include "delay.h"
//#include "w25qxx.h"
//u8 ENABLE_SERIAL_CODE[]={0x01,0x05,0x00,0x30,0xff,0x00,0x8c,0x35};//指示串行通信有效
// u8 SVON_CODE[]={0x01,0x05,0x00,0x19,0xff,0x00,0x5d,0xfd};//伺服ON
//const u8 HOLD_CODE[]={0x01,0x05,0x00,0x18,0xff,0x00,0x0c,0x3d};//暂时停止（hold）
//const u8 REHOLD_CODE[]={0x01,0x05,0x00,0x18,0x00,0x00,0x4d,0xcd};//取消暂时停止（hold）
// u8 SETUP_CODE[]={0x01,0x05,0x00,0x1c,0xff,0x00,0x4D,0xfc};//归零
// u8 OVER_SETUP_CODE[]={0x01,0x05,0x00,0x1c,0x00,0x00,0x0c,0x0c};//归零动作结束
//const u8 start_directly_CODE[]={0x01,0x10,0x91,0x00,0x00,0x01,0x02,0x01,0x00,0x27,0x09};//启动直接运行
//u8 RESET_CODE[]={0x01,0x05,0x00,0x1b,0xff,0x00,0xfc,0x3d};//重启
//u8 OVER_RESET_CODE[]={0x01,0x05,0x00,0x1b,0x00,0x00,0xbd,0xcd};//重启结束

//const u8 POINT_RUN_CODE[]={0x01,0x05,0x00,0x1a,0xff,0x00,0xad,0xfd};//启动 点动运行
//const u8 OVER_POINT_RUN_CODE[]={0x01,0x05,0x00,0x1A,0x00,0x00,0xEC,0x0D};//点动结束

//u8 DONE_CODE[]={0x01,0x02,0x00,0x40,0x00,0x10,0x78,0x12};//读取状态

//extern u8 DG_BUF[16];
//extern u8 DG_BUF_LEN;
u8 DG_Receive_Data[16];
u32 DG_Receive_Len;
//extern u8 Rec_Over;
//extern u8 DG_BUF[16];
extern u8  USART3_REC_OVER;
u32 coords=0;

u8 g_point=0;//保存电缸点位
u8 g_refresh=0;
u8 g_id=0;

u8 wt_flag=0;//电缸微调标记
DG_PAR directly_run={
                     {{0x01},{0x00,0x01}},//运动方式(绝对/相对)为绝对
                     {{0x01},{0x00 ,0xf4}},//速度
                     {{0x01},{0x00 ,0x00 ,0x00 ,0x00}},//位置
                     {{0x01},{0x03 ,0x88}},//加速度
										 {{0x01},{0x03 ,0x88}},//减速度
										 {{0x01},{0x00 ,0x00}},//推压力
										 {{0x01},{0x00 ,0x00}},//阈值	 
                     {{0x01},{0x00 ,0x14}},//推压速度
										 {{0x01},{0x00 ,0x64}},//最大定位推力
                     {{0x01},{0x00 ,0x00 ,0x00 ,0x00}},//区域输出端1
                     {{0x01},{0x00 ,0x00 ,0x00 ,0x00}},//区域输出端2
                     {{0x01},{0x00 ,0x00 ,0x00 ,0x64}},//定位幅 	
                     {{0x01},{0x00,0x00}},//启动命令		
                     0x01,//控制器ID										 
                    };//直接指定位置、速度等信息运动
DG_Script DG1;

u8 DIRECTLY_RUN_BUF[41]={0x01,0x10,0x91,0x02,0x00,0x10,0x20};//存储直接运动的参数

////*******************
////产生校验码
////
////*******************												 
//u16 getCRC16(u8*ptr,u16 len) 
//{ 
//    u8 i; 
//    u16 crc = 0xFFFF; 
//    if(len==0){
//        len = 1;
//    } 
//    while(len--){   
//        crc ^= *ptr; 
//        for(i=0; i<8; i++){ 
//            if(crc&1){ 
//                crc >>= 1;  
//                crc ^= 0xA001; 
//        	}  
//        	else{
//                crc >>= 1;
//        	} 
//        }         
//        ptr++; 
//    } 
//    return(crc); 
//} 


//****************************************************
//指示串行通信有效
//ID：控制器ID号 1-32
//****************************************************
void Enable_serial(u8 ID)
{
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x30,0xff,0x00,0x8c,0x35};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}	
}	
//****************************************************
//暂时停止/继续运行
//ID：控制器ID号 1-32
//state：1暂停
//       0继续
//****************************************************
void Hold(u8 ID,u8 state)
{
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x18,0xff,0x00,0x0c,0x3d};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		if(state&0x01)
			send_buf[4]=0xff;
		else
			send_buf[4]=0x00;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}	
}

//****************************************************
//伺服ON/0ff
////ID：控制器ID号 1-32
//state:1 伺服 ，0 取消伺服
//****************************************************
void SVON(u8 ID,u8 state)
{
	//USART3_Send((u8*)SVON_CODE,8);
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x19,0xff,0x00,0x5d,0xfd};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		if(state==1)send_buf[4]=0xff;else send_buf[4]=0x00;
		
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}
}	
//****************************************************
//确认伺服ON
//ID：控制器ID号 1-32
//****************************************************
void Verify_SVON(u8 ID)
{
	//USART3_Send(DONE_CODE,8);
	u16 data;
	u8 send_buf[]={0x01,0x02,0x00,0x40,0x00,0x10,0x78,0x12};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}
}
////****************************************************
////等待伺服ON
////返回0：伺服ON失败，0x02伺服完成
////
////****************************************************
//u8 Wait_SVON(void)
//{
//	u8 i=0;
//	u8 data;
//	data=Verify_SVON(1);
//	while((data!=0x02) && (i<250) )
//	{
//     data=Verify_SVON(1);
//		i++;
//	}	
//	if(i>=250)
//	 return 0;
//	else
//	 return data;	
//}
//****************************************************
//电缸回零
//ID：控制器ID号 1-32
//****************************************************
void Setup(u8 ID)
{
	//USART3_Send(SETUP_CODE,8);
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x1c,0xff,0x00,0x4d,0xfc};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		
		USART3_Send(send_buf,8);
	}
}	
//****************************************************
//回零动作结束
//ID：控制器ID号 1-32
//****************************************************
void Over_Setup(u8 ID)
{
	//USART3_Send(OVER_SETUP_CODE,8);
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x1c,0x00,0x00,0x0c,0x0c};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}
}	

//****************************************************
//确认回零完成
//ID：控制器ID号 1-32
//****************************************************
void Verify_Setup(u8 ID) 
{

	//USART3_Send(DONE_CODE,8);
		u16 data;
	u8 send_buf[]={0x01,0x02,0x00,0x40,0x00,0x10,0x78,0x12};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}
	//return (rec_data[4]&0x04);
}	
//****************************************************
//等待回零完成
//返回0x04：回零动作结束
//****************************************************
//u8 Wait_Setup(u8 data)
//{
//	while(data!=0x04)
//	{
//		data=Verify_Setup(1);
//	}	
//	Over_Setup(1);//回零动作结束
//	return data;
//}	
//***************************************************
//重启
//ID：控制器ID号 1-32
//***************************************************
void Reset(u8 ID)
{
	//USART3_Send(RESET_CODE,8);
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x1b,0xff,0x00,0xfc,0x3d};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}
}	
//***************************************************
//确认重启完成（ALARM(x4f)变成1）
//ID：控制器ID号 1-32
//***************************************************
void Verify_Reset(u8 ID)
{

	//USART3_Send(DONE_CODE,8);
	u16 data;
	u8 send_buf[]={0x01,0x02,0x00,0x40,0x00,0x10,0x78,0x12};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}
	//return (rec_data[4]&0x80);
}	
//***************************************************
//重启结束
//ID：控制器ID号 1-32
//***************************************************
void Over_Reset(u8 ID)
{
	//USART3_Send(OVER_RESET_CODE,8);//重启完成
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x1b,0x00,0x00,0xbd,0xcd};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
		//USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
		//UART4_Send(DG_Receive_Data,DG_Receive_Len);
	}
}	

//***************************************************
void DG_Ready(u8 ID,u8 state)
{
	Enable_serial(ID);//指示串行有效
	delay_ms(500);
	delay_ms(500);
	
	SVON(ID,state);//伺服ON
	delay_ms(500);
	//return Wait_SVON();//等待伺服完成
}



////**************************************************
////参数解析上位机下发数据，数据格式为1Byte ID+ 2Byte 地址 +数据(高在前)
////addr：地址
////**************************************************
//void Paramater_Parse(u16 addr)
//{
//  if(Rec_Over==1)
//	{
//		 Rec_Over=0;	
//     directly_run.Controler_Id=DG_BUF[0];	//控制器ID	
//		 //DIRECTLY_RUN_BUF[0]=directly_run.Controler_Id;
//		 switch(addr)
//		 {
//			 /*******************************以下是保存的是直接设置位置、速度...运行(不是点位设置)************/
//			 /*                             数据最写入到控制器RAM中，掉电消失                                */
//			 case 0x9102: //设置控制方式
//										directly_run.action_type[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.action_type[directly_run.Controler_Id][1]=DG_BUF[4];
//										// DIRECTLY_RUN_BUF[7]=directly_run.action_type[directly_run.Controler_Id][0];
//										// DIRECTLY_RUN_BUF[8]=directly_run.action_type[directly_run.Controler_Id][1];
//										 			 
//										break;
//			 case 0x9103: //设置速度
//										directly_run.speed[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.speed[directly_run.Controler_Id][1]=DG_BUF[4];  
//			               //DIRECTLY_RUN_BUF[9]=directly_run.speed[directly_run.Controler_Id][0];
//										 //DIRECTLY_RUN_BUF[10]=directly_run.speed[directly_run.Controler_Id][1];
//										 
//										break;					 
//			 
//			 case 0x9104: //设置位置
//									  directly_run.coord[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.coord[directly_run.Controler_Id][1]=DG_BUF[4]; 
//										directly_run.coord[directly_run.Controler_Id][2]=DG_BUF[5];
//										directly_run.coord[directly_run.Controler_Id][3]=DG_BUF[6]; 
////			               DIRECTLY_RUN_BUF[11]=directly_run.coord[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[12]=directly_run.coord[directly_run.Controler_Id][1];
////										 DIRECTLY_RUN_BUF[13]=directly_run.coord[directly_run.Controler_Id][2];
////										 DIRECTLY_RUN_BUF[14]=directly_run.coord[directly_run.Controler_Id][3];
//										 
//										break;
//									
//			 case 0x9106: //设置加速度
//										directly_run.acc[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.acc[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[15]=directly_run.acc[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[16]=directly_run.acc[directly_run.Controler_Id][1];
//										 
//										break;
//			 case 0x9107://设置减速度
//									 directly_run.dec[directly_run.Controler_Id][0]=DG_BUF[3];
//									 directly_run.dec[directly_run.Controler_Id][1]=DG_BUF[4];
////			              DIRECTLY_RUN_BUF[17]=directly_run.dec[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[18]=directly_run.dec[directly_run.Controler_Id][1];
//										  		 
//										break; 
//						 
//			 case 0x9108: //设置推压力
//									  directly_run.tyl[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.tyl[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[19]=directly_run.tyl[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[20]=directly_run.tyl[directly_run.Controler_Id][1];
//										 		 
//										break;           
//				
//			 case 0x9109://设置阈值
//									  directly_run.space[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.space[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[21]=directly_run.space[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[22]=directly_run.space[directly_run.Controler_Id][1];
//										 
//										break;
//			 case 0x910a://设置加推压速度
//				  
//									  directly_run.tys[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.tys[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[23]=directly_run.tys[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[24]=directly_run.tys[directly_run.Controler_Id][1];
//										  		 
//										break;
//			 case 0x910b: //设置最大推力
//									  directly_run.max_tl[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.max_tl[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[25]=directly_run.max_tl[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[26]=directly_run.max_tl[directly_run.Controler_Id][1];
//										  		 
//										break;
//			 case 0x910c: //设置区域输出端1
//										directly_run.out_space1[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.out_space1[directly_run.Controler_Id][1]=DG_BUF[4]; 
//										directly_run.out_space1[directly_run.Controler_Id][2]=DG_BUF[5];
//										directly_run.out_space1[directly_run.Controler_Id][3]=DG_BUF[6];
////			               DIRECTLY_RUN_BUF[27]=directly_run.out_space1[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[28]=directly_run.out_space1[directly_run.Controler_Id][1];
////										 DIRECTLY_RUN_BUF[29]=directly_run.out_space1[directly_run.Controler_Id][2];
////										 DIRECTLY_RUN_BUF[30]=directly_run.out_space1[directly_run.Controler_Id][3];
//										 
//										break;
//			 case 0x910e: //设置区域输出端2
//									  directly_run.out_space2[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.out_space2[directly_run.Controler_Id][1]=DG_BUF[4];
//										directly_run.out_space2[directly_run.Controler_Id][2]=DG_BUF[5];
//										directly_run.out_space2[directly_run.Controler_Id][3]=DG_BUF[6];
////			               DIRECTLY_RUN_BUF[31]=directly_run.out_space2[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[32]=directly_run.out_space2[directly_run.Controler_Id][1];
////										 DIRECTLY_RUN_BUF[33]=directly_run.out_space2[directly_run.Controler_Id][2];
////										 DIRECTLY_RUN_BUF[34]=directly_run.out_space2[directly_run.Controler_Id][3];
//										 	 
//										break;
//			 case 0x9110://设置定位幅
//									 directly_run.dwf[directly_run.Controler_Id][0]=DG_BUF[3];
//									 directly_run.dwf[directly_run.Controler_Id][1]=DG_BUF[4];
//									 directly_run.dwf[directly_run.Controler_Id][2]=DG_BUF[5];
//									 directly_run.dwf[directly_run.Controler_Id][3]=DG_BUF[6];
////			              DIRECTLY_RUN_BUF[35]=directly_run.dwf[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[36]=directly_run.dwf[directly_run.Controler_Id][1];
////										 DIRECTLY_RUN_BUF[37]=directly_run.dwf[directly_run.Controler_Id][2];
////										 DIRECTLY_RUN_BUF[38]=directly_run.dwf[directly_run.Controler_Id][3];	
//										break;
//			 case 0x9100://启动命令0x91000100
//									 directly_run.start_directly[directly_run.Controler_Id][0]=DG_BUF[3];
//									 directly_run.start_directly[directly_run.Controler_Id][1]=DG_BUF[4];
//									 break;
//			 /*****************************以下是点动设置*******************************/
//			 /*                             参数设置后存储在外部EEPROM中               */
//			              /********Step_No1-Step_No64*************/
//			default: if((addr>=0x0100)&&(addr<0x04ff))
//			          {
//									 switch(DG_BUF[2])
//									 {
//										 case 0x00://设置控制方式
//											         Set_Action_Type(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x01://设置速度
//											         Set_Speed(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x02://设置位置
//											         Set_Coord(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4],DG_BUF[5],DG_BUF[6]);
//											         break;
//										 case 0x04://设置加速度
//											         Set_Acc(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x05://设置减速度
//											         Set_Dec(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x06://设置推压力，默认0
//											         Set_Tyl(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x07://设置阈值，默认0
//											         Set_Space(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);  
//											         break;
//										 case 0x08://设置推压速度，默认20
//											         Set_Tys(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x09://设置定位最大推力，默认100
//											         Set_Max_Tl(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x0a://设置区域输出端1,默认0
//											         Set_Out_Space1(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4],DG_BUF[5],DG_BUF[6]);
//											         break;
//										 case 0x0c://设置区域输出端2，默认0
//											         Set_Out_Space2(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4],DG_BUF[5],DG_BUF[6]);
//											         break;
//										 case 0x0e://设置定位幅，默认100
//											         Set_Dwf(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4],DG_BUF[5],DG_BUF[6]);
//											         break;
//										 default :break;
//									 }	 
//								}
//				        break;		 		 
//		 }
//     DG_BUF[0]=0;
//     DG_BUF[1]=0;
//     DG_BUF[2]=0;		 
//	 } 		 
//}	
//*****************************************************


//*****************************************************
//处理 指定位置直接运行 设置的参数，包括获取校验码
//ID：控制器ID号 1-32
//*****************************************************
void Process_Directly_Run_Code(u8 ID)
{  
	 u16 data;
	 u8 IDn;
	 IDn=1;
	 DIRECTLY_RUN_BUF[0]=ID;
	 DIRECTLY_RUN_BUF[7]=directly_run.action_type[IDn][0];
	 DIRECTLY_RUN_BUF[8]=directly_run.action_type[IDn][1];
	 DIRECTLY_RUN_BUF[9]=directly_run.speed[IDn][0];
	 DIRECTLY_RUN_BUF[10]=directly_run.speed[IDn][1];
	 DIRECTLY_RUN_BUF[11]=directly_run.coord[ID][0];
	 DIRECTLY_RUN_BUF[12]=directly_run.coord[ID][1];
	 DIRECTLY_RUN_BUF[13]=directly_run.coord[ID][2];
	 DIRECTLY_RUN_BUF[14]=directly_run.coord[ID][3];
	 DIRECTLY_RUN_BUF[15]=directly_run.acc[IDn][0];
	 DIRECTLY_RUN_BUF[16]=directly_run.acc[IDn][1];
	 DIRECTLY_RUN_BUF[17]=directly_run.dec[IDn][0];
	 DIRECTLY_RUN_BUF[18]=directly_run.dec[IDn][1];
	 DIRECTLY_RUN_BUF[19]=directly_run.tyl[IDn][0];
	 DIRECTLY_RUN_BUF[20]=directly_run.tyl[IDn][1];
	 DIRECTLY_RUN_BUF[21]=directly_run.space[IDn][0];
	 DIRECTLY_RUN_BUF[22]=directly_run.space[IDn][1];
	 DIRECTLY_RUN_BUF[23]=directly_run.tys[IDn][0];
	 DIRECTLY_RUN_BUF[24]=directly_run.tys[IDn][1];
	 DIRECTLY_RUN_BUF[25]=directly_run.max_tl[IDn][0];
	 DIRECTLY_RUN_BUF[26]=directly_run.max_tl[IDn][1];
	 DIRECTLY_RUN_BUF[27]=directly_run.out_space1[IDn][0];
	 DIRECTLY_RUN_BUF[28]=directly_run.out_space1[IDn][1];
	 DIRECTLY_RUN_BUF[29]=directly_run.out_space1[IDn][2];
	 DIRECTLY_RUN_BUF[30]=directly_run.out_space1[IDn][3];
	 DIRECTLY_RUN_BUF[31]=directly_run.out_space2[IDn][0];
	 DIRECTLY_RUN_BUF[32]=directly_run.out_space2[IDn][1];
	 DIRECTLY_RUN_BUF[33]=directly_run.out_space2[IDn][2];
	 DIRECTLY_RUN_BUF[34]=directly_run.out_space2[IDn][3];
	 DIRECTLY_RUN_BUF[35]=directly_run.dwf[IDn][0];
	 DIRECTLY_RUN_BUF[36]=directly_run.dwf[IDn][1];
	 DIRECTLY_RUN_BUF[37]=directly_run.dwf[IDn][2];
	 DIRECTLY_RUN_BUF[38]=directly_run.dwf[IDn][3];
	 
	 data=getCRC16(DIRECTLY_RUN_BUF,39);//获取校验码
	 DIRECTLY_RUN_BUF[39]=data & 0x00ff;
	 DIRECTLY_RUN_BUF[40]=(data >> 8) & 0x00ff;
}	
//****************************************************
//发送启动命令
//ID：控制器ID号 1-32
//****************************************************
void Send_Directly_Run_Code(u8 ID)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x91,0x00,0x00,0x01,0x02,0x01,0x00,0x27,0x09};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,9);
		send_buf[9]=data & 0x00ff;
		send_buf[10]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,11);
	}
}	
//****************************************************
//启动直接运行
//ID：控制器ID号 1-32
//****************************************************
void Start_directly_run(u8 ID)
{
	 if((ID>=1)&&(ID<=32))
	 {
			Process_Directly_Run_Code(ID);//处理直接运行的参数
			USART3_Send(DIRECTLY_RUN_BUF,41);//发送参数
			delay_ms(100);//延时要加
			Send_Directly_Run_Code(ID);
	 }	 
}	
//****************************************************
//确认 运行结束
//f返回0x0e：运动完成
//****************************************************
u8 Verify_Run_Over(u8 ID)
{
	u16 data;
	u8 send_buf[]={0x01,0x02,0x00,0x40,0x00,0x10,0x78,0x12};
	if((ID>=1)&&(ID<=32))
	{	
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}
	return 1;
}	
//****************************************************
//等待运动完成
//返回：0x0e完成
//
//****************************************************
RET_STATUS Wait_Run_Over(u8 ID)
{	
	u8 cnt = 1;
	RET_STATUS ret =OPT_SUCCESS;
	DG_Receive_Data[4]=0;//jxd2015.08.26
	//Verify_Run_Over(ID);
	//while(!DG_UART5_RX_OVER);
	delay_ms(300); 
	Verify_Run_Over(ID);//确认是否完成
//	delay_ms(100);
//	DG_UART5_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//	while(DG_Receive_Data[4]!=0x0e)//等待
//	{
////		if((DG_BUF[1]<<8+DG_BUF[2])==0x0018)//暂时停止
////		{
////			DG_BUF[2]=0;//001801
////		  DG_BUF[1]=0;
////		  Hold(DG_BUF[0],DG_BUF[3]);
////		}	
//        DG_UART5_RX_CNT=0;
//		if(DG_UART5_RX_OVER==1) 
//		{	
//				 
//			   Verify_Run_Over(ID);//确认是否完成
//			   delay_ms(100);
//        }			
//		DG_UART5_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//		//UART4_Send(DG_Receive_Data,DG_Receive_Len);
//	 }	
//	return ret;

//	while(!DG_UART5_RX_OVER){//等待
//		DG_UART5_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//	}
//	
	delay_ms(100); 
	
	do{//等待
		USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
		cnt++;
	}while(!USART3_REC_OVER && cnt!= 0);
	
	if(DG_Receive_Data[4]!=0x0e) {
		if(DG_Receive_Data[4] & 0x01==0x01)
		  ret =OPT_FAILURE;
		else  ret =OPT_SUCCESS; 
	} else {
		ret =OPT_SUCCESS;
	}
	
	return ret;
}
//**********************************************************************************
//                                  点动
//
//**********************************************************************************

//**********************************
//设置运行方式（绝对/相对）
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH:数据高位
//dataL:数据地位
//0x0000:相对运动，0x0001：绝对运动
//**********************************
void Set_Action_Type(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1);
	send_buf[7]=dataH;//默认串行
	send_buf[8]=dataL;//
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}	            
//**********************************
//设置速度
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH:数据高位
//dataL:数据地位
//**********************************
void Set_Speed(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+1;
	send_buf[7]=dataH;//默认值
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}	 
//**********************************
//设置位置
//参数Step_Num:要控制的点号
//ID:电缸控制器ID号（1-32）
//dataH1:最高字节
//dataL2:最低字节
//**********************************
void Set_Coord(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2)
{
	u16 data;
	u8 send_buf[13]={0x01,0x10,0x04,0,0x00,0x02,0x04,0,0,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+2;
	send_buf[7]=dataH1;//默认值
	send_buf[8]=dataH2;
	send_buf[9]=dataL1;//默认值
	send_buf[10]=dataL2;
	data=getCRC16(send_buf,11);
	send_buf[11]=data & 0x00ff;
  send_buf[12]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,13);
}	 
//**********************************
//设置加速度
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH:数据高位
//dataL:数据地位
//**********************************
void Set_Acc(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;       
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+4;
	send_buf[7]=dataH;//默认值
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//设置减速度
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH:数据高位
//dataL:数据地位
//**********************************
void Set_Dec(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+5;
	send_buf[7]=dataH;//默认值
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//设置推压力
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH:数据高位
//dataL:数据地位
//**********************************
void Set_Tyl(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+6;
	send_buf[7]=dataH;//默认值
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//设置阈值
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH:数据高位
//dataL:数据地位
//**********************************
void Set_Space(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+7;
	send_buf[7]=dataH;//默认值
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//设置推压速度
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH:数据高位
//dataL:数据地位
//**********************************
void Set_Tys(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+8;
	send_buf[7]=dataH;//默认值
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//设置最大定位推力
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH:数据高位
//dataL:数据地位
//**********************************
void Set_Max_Tl(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+9;
	send_buf[7]=dataH;//默认值
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//设置区域输出端1
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH1:最高字节
//dataL2:最低字节
//**********************************
void Set_Out_Space1(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2)
{
	u16 data;
	u8 send_buf[13]={0x01,0x10,0x04,0,0x00,0x02,0x04,0,0,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+10;
	send_buf[7]=dataH1;//默认值
	send_buf[8]=dataH2;
	send_buf[9]=dataL1;//默认值
	send_buf[10]=dataL2;
	data=getCRC16(send_buf,9);
	send_buf[11]=data & 0x00ff;
  send_buf[12]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,13);
}
//**********************************
//设置区域输出端2
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH1:最高字节
//dataL2:最低字节
//**********************************
void Set_Out_Space2(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2)
{
	u16 data;
	u8 send_buf[13]={0x01,0x10,0x04,0,0x00,0x02,0x04,0,0,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+12;
	send_buf[7]=dataH1;//默认值
	send_buf[8]=dataH2;
	send_buf[9]=dataL1;//默认值
	send_buf[10]=dataL2;
	data=getCRC16(send_buf,11);
	send_buf[11]=data & 0x00ff;
  send_buf[12]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,13);
}
//**********************************
//设置定位幅
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//dataH1:最高字节
//dataL2:最低字节 
//**********************************
void Set_Dwf(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2)
{
	u16 data;
	u8 send_buf[13]={0x01,0x10,0x04,0,0x00,0x02,0x04,0,0,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+14;
	send_buf[7]=dataH1;//默认值
	send_buf[8]=dataH2;
	send_buf[9]=dataL1;//默认值
	send_buf[10]=dataL2;
	data=getCRC16(send_buf,11);
	send_buf[11]=data & 0x00ff;
  send_buf[12]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,13);
}
//**********************************
//设置步数据(要运行的点号，（启动之前要先选择点）)
//ID:电缸控制器ID号（1-32）
//Step_Num:1-64
//**********************************
u8 Select_Step(u8 ID,u8 Step_Num)
{
	u8 i;
	u8 DG_return_data[16]={0};
	u32 data_len=0;
	u16 data;
	u8 send_buf[10]={0x01,0x0f,0x00,0x10,0x00,0x08,0x01,0,0,0};
	USART3_Receive_Data(DG_return_data,&data_len);
	send_buf[0]=ID;
	send_buf[7]=Step_Num-1;
	data=getCRC16(send_buf,8);
	send_buf[8]=data & 0x00ff;
  send_buf[9]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,10);
	delay_ms(300);
	USART3_Receive_Data(DG_return_data,&data_len);
	for(i=0;i<6;i++)
	{
		if(DG_return_data[i]!=send_buf[i])
			return 3;
	}
	return 2;
}	
//**********************************
//启动点动运行（发送启动指令）
//
//ID:电缸控制器ID号（1-32）
//**********************************
u8 Activate_Point_Run(u8 ID)
{
	u8 i;
	u8 DG_return_data[16]={0};
	u32 data_len=0; 
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x1a,0xff,0x00,0xad,0xfd};//启动 点动运行
	if((ID>=1)&&(ID<=32))
	{	
		USART3_Receive_Data(DG_return_data,&data_len);
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
		delay_ms(300);
	  USART3_Receive_Data(DG_return_data,&data_len);
		for(i=0;i<8;i++)
		{
			if(DG_return_data[i]!=send_buf[i])
				return 4;
		}
		return 2;
  }
	return 6;
}	
//**********************************
//点动运行结束将DRIVE(Y1A)置0
//
//ID:电缸控制器ID号（1-32）
//**********************************
u8 Over_Point_Run(u8 ID)
{
	u8 i;
	u8 DG_return_data[16]={0};
	u32 data_len=0; 
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x1A,0x00,0x00,0xEC,0x0D};//点动结束
	if((ID>=1)&&(ID<=32))
	{	
		USART3_Receive_Data(DG_return_data,&data_len);
		send_buf[0]=ID;
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
		delay_ms(300);
	  USART3_Receive_Data(DG_return_data,&data_len);
		for(i=0;i<8;i++)
		{
			if(DG_return_data[i]!=send_buf[i])
				return 5;
		}
		return 2;
	}
	return 6;
}


//************************************
//点动运行函数
//Step_Num:要运行的点
//ID:电缸控制器ID号（1-32）
//************************************
u8 Point_run(u8 ID,u8 Step_Num)
{
	u8 state;
	Set_Action_Type(ID,Step_Num,0x00,0x01);//绝对运动
	delay_ms(300);

	state=Select_Step(ID,Step_Num);//选择第几个点
	if(state!=2)
	{	
		return state;
	}
    else
	{	
	  state=Activate_Point_Run(ID);//发送启动指令（把DRIVE置1）
	  if(state!=2)
		{	
			Over_Point_Run(ID);//把DRIVE置0
			return state;
		}
		else
	    {	
			state=Over_Point_Run(ID);//把DRIVE置0
			if(state!=2)
			{	
				Over_Point_Run(ID);//把DRIVE置0
				return state;
			}
			else return 2;
	    }
	}	
		
}


//**********************************************
//电缸配置脚本读取并发送到电缸控制器
//addr:脚本地址
//num:选择要配置几个点(1-64)
//**********************************************
//u8 DG_Script_Read_And_Config(u32 addr,u8 num)
//{
//	 u8 i;
//	 u8 step_no;
//	 u8 data_buf[4];
//	 u32 script_pos=0; //脚本位置
//	 W25QXX_Read(&DG1.DG_No,addr,1);//读取电缸号
//	 W25QXX_Read(&DG1.DG_Name_Len,addr+1,1);//读取电缸名称长度
//	 script_pos=script_pos+addr+2+DG1.DG_Name_Len;
//	 for(i=0;i<num;i++)
//	 {
//		 W25QXX_Read(&step_no,script_pos,1);//读取步数号
//		 
//		 W25QXX_Read(data_buf,script_pos+1,4);//读取位置
//		 Set_Coord(DG1.DG_No,step_no,data_buf[0],data_buf[1],data_buf[2],data_buf[3]);//设置位置
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+5,2);//读取速度
//		 Set_Speed(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//设置速度
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+7,2);//读取加速度
//		 Set_Acc(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//设置加速度
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+9,2);//读取减速度
//		 Set_Dec(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//设置减速度
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+11,2);//读取动作方法
//		 Set_Action_Type(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//设置动作方法
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+13,2);//读取推压力
//		 Set_Tyl(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//设置推压力
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+15,2);//读取阈值
//		 Set_Space(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//设置阈值
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+17,2);//读取推压速度
//		 Set_Tys(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//设置推压速度
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+19,2);//读取定位最大推力
//		 Set_Max_Tl(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//设置定位最大推力
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+23,4);//读取区域输出端1
//		 Set_Out_Space1(DG1.DG_No,step_no,data_buf[0],data_buf[1],data_buf[2],data_buf[3]);//设置区域输出端1
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+27,4);//读取区域输出端2
//		 Set_Out_Space2(DG1.DG_No,step_no,data_buf[0],data_buf[1],data_buf[2],data_buf[3]);//设置区域输出端2
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+31,4);//读取定位幅
//		 Set_Dwf(DG1.DG_No,step_no,data_buf[0],data_buf[1],data_buf[2],data_buf[3]);//设置定位幅
//		 delay_ms(50);
//		 script_pos=script_pos+33;
//	 }
//	 return 1;
//}
//************************
//测试函数
//
//************************
//void DG_TEST(void)
//{	
//	  u16 addr;
//	  RS485_Receive_Data(DG_BUF,&DG_BUF_LEN);
//		USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//	  UART4_Send(DG_Receive_Data,DG_Receive_Len);
//		addr=(DG_BUF[1]<<8) + DG_BUF[2];
//    switch(addr)
//		{	
//		  case 0x001c://回零
//									Setup(DG_BUF[0]);
//									delay_ms(100);
//			            while(DG_Receive_Data[4]!=0x0e)//等待回零完成
//									{
//										if(DG_UART5_RX_OVER==1)
//										{	
//											if(DG_Receive_Data[4]!=0x0e)//回零没完成
//											{	
//												delay_ms(100);
//												Verify_Setup(DG_BUF[0]); 
//											}
//										}	
//										else Over_Setup(DG_BUF[0]);//回零动作结束
//										USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//										UART4_Send(DG_Receive_Data,DG_Receive_Len);
//									}	
//			            DG_BUF[1]=0;
//									DG_BUF[2]=0;
//			            Over_Setup(DG_BUF[0]);//回零动作结束
//									break;
//		  case 0x0019://伺服ON
//									 DG_BUF[2]=0;
//									 DG_BUF[1]=0;
//									 DG_Ready(DG_BUF[0]);//使能串行+伺服ON
//			             while(!(DG_Receive_Data[4]&0x02))//等待伺服完成
//									 {
//											if(DG_UART5_RX_OVER==1) //接收电缸控制器返回的数据完成
//											{	
//												if(!(DG_Receive_Data[4]&0x02))//伺服没完成
//												{	
//												  delay_ms(100);
//													Verify_SVON(DG_BUF[0]);//再发一次读取数据 
//												 
//												}	
//											}	
//											USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//											UART4_Send(DG_Receive_Data,DG_Receive_Len);
//									 }	
//			             break;
//			case 0x0018://暂时停止
//									 DG_BUF[2]=0;//001801
//									 DG_BUF[1]=0;
//									 Hold(DG_BUF[0],DG_BUF[3]);//		 
//			case 0x001B://重启
//									DG_BUF[0]=0;
//									DG_BUF[1]=0;//
//									Reset(DG_BUF[0]);//
//			            delay_ms(100);
////			            while(!(DG_Receive_Data[4]&0x80))//等待重启完成
////									{
////										if(DG_UART5_RX_OVER==1) 
////										{
////                      if(!(DG_Receive_Data[4]&0x80))											
////											{
////                        delay_ms(100); 												
////												Verify_Reset(DG_BUF[0]);//确认重启是否完成
////											}
////                    }											
////			              //delay_ms(100);
////										USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
////										UART4_Send(DG_Receive_Data,DG_Receive_Len);
////									}	
//									  Over_Reset(DG_BUF[0]);//重启动作结束
//									  //delay_ms(100);
//			            break;
//			default : 
//				        if((DG_BUF[0]==0xff)&&(DG_BUF[1]<=32)&&(DG_BUF[1]>=1)&&(DG_BUF[2]<=64)&&(DG_BUF[2]>=1))
//								{
//									Point_run(DG_BUF[1],DG_BUF[2]);
//									DG_BUF[2]=0;
//									DG_BUF[1]=0;
//		            }
//								break;
//		}			
//    Paramater_Parse(addr);//参数解析
//		Start_directly_run(DG_BUF[0]);//启动简易模式运行		
//	}		
//**********************************
//读取点参数
//参数Step_Num:要控制的点号1-64
//ID:电缸控制器ID号（1-32）
//Para_No:参数号(1-12)

//**********************************
void Read_Point_Para(u8 ID,u8 Step_Num,u8 Para_No)
{
	u16 data;       
	u8 send_buf[8]={0x01,0x03,0x04,0,0x00,0x01,0,0};
	if((ID>=1)&&(ID<=32))
	{
		send_buf[0]=ID;
		if(Para_No<=3)
		{	
		  send_buf[3]=0x10*(Step_Num-1)+Para_No-1;
			if(Para_No==3)
		    send_buf[5]=2;//读取长度 Word
			else
				send_buf[5]=1;
    }			
		else if(Para_No<=10)
		{	
			send_buf[3]=0x10*(Step_Num-1)+Para_No;
		  if(Para_No==10)
		    send_buf[5]=2;//读取长度 Word
			else
				send_buf[5]=1;
		}	
		else if(Para_No<=12)
		{	
			send_buf[3]=0x10*(Step_Num-1)+Para_No+1;
		  send_buf[5]=2;//读取长度 Word
		}
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}	
}
//****************************************************
//读取当前位置,速度，推力，目标位置，运行NO，报警信息
//
//ID:电缸控制器ID号（1-32）
//****************************************************
u8 Read_Current_Inf(u8 DG_id,u8 DG_points,u16 addr)
{
	u8 i,j;
	u16 data;
	u8 SEND_BUF[8]={0x01,0x03,0x90,0,0x00,0x01,0,0};
 	u8 REC_BUF[16]={0};
	u32 REC_BUF_LEN;
	SEND_BUF[0]=DG_id;
	i=0;
	//DG_UART5_RX_CNT=0;
	if((DG_id<=32)&&(DG_id>=1)&&(DG_points>=1)&&(DG_points<=64))
	{	
		 if(addr & 0x0001)//位置
		 {
				Read_Point_Para(DG_id,DG_points,3);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}	
				for(j=0;j<4;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+4;			
		 }
		 if(addr & 0x0002)//速度
		 {
				Read_Point_Para(DG_id,DG_points,2);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}	
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0004)//加速度
		 {
				Read_Point_Para(DG_id,DG_points,4);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0008)//减速度
		 {
				Read_Point_Para(DG_id,DG_points,5);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0010)//动作方法
		 {
				Read_Point_Para(DG_id,DG_points,1);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0020)//推压力
		 {
				Read_Point_Para(DG_id,DG_points,6);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0040)//阈值
		 {
				Read_Point_Para(DG_id,DG_points,7);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0080)//退压速度
		 {
				Read_Point_Para(DG_id,DG_points,8);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0100)//最大定位你推力
		 {
				Read_Point_Para(DG_id,DG_points,9);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<2;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x0200)//区域输出端1
		 {
				Read_Point_Para(DG_id,DG_points,10);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<4;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+4;
		 }
		 if(addr & 0x0400)//区域输出端2
		 {
				Read_Point_Para(DG_id,DG_points,11);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<4;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+4;
		 }
		 if(addr & 0x0800)//定位幅
		 {
				Read_Point_Para(DG_id,DG_points,12);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//等待电缸回馈完
				//{	 
					USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				//}
				for(j=0;j<4;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+4;
		 }
		 
	 }
	if(addr & 0x1000)//运行数据NO(1-64)
		 {
			  
				SEND_BUF[3]=0x06;
				SEND_BUF[6]=0x49;
			  SEND_BUF[5]=0x01;
			  SEND_BUF[7]=0x0b;
			  data=getCRC16(SEND_BUF,6);
		    SEND_BUF[6]=data & 0x00ff;
		    SEND_BUF[7]=(data >> 8) & 0x00ff;
			  USART3_Send(SEND_BUF,8);	
			  delay_ms(200);	 
				USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				for(j=0;j<2;j++)
				{
					if(j==1)
					  Para_Buf[i+j]=REC_BUF[j+3]+1;
					else
						Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+2;
		 }
		 if(addr & 0x2000)//当前位置
		 {
			 SEND_BUF[3]=0x00;
				SEND_BUF[6]=0xe9;
			  SEND_BUF[5]=0x02;
			  SEND_BUF[7]=0x0b;
			  data=getCRC16(SEND_BUF,6);
		    SEND_BUF[6]=data & 0x00ff;
		    SEND_BUF[7]=(data >> 8) & 0x00ff;
			  USART3_Send(SEND_BUF,8);	
			  delay_ms(200);	 
				USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				for(j=0;j<4;j++)
				{
					Para_Buf[i+j]=REC_BUF[j+3];
					REC_BUF[j+3]=0;
				}
				i=i+4;
		 }
		 if(addr & 0x4000)//读报警信息
		 {
			  
				SEND_BUF[3]=0x08;
				SEND_BUF[6]=0xE8;
			  SEND_BUF[5]=0x04;
			  SEND_BUF[7]=0xcb;
			  data=getCRC16(SEND_BUF,6);
		    SEND_BUF[6]=data & 0x00ff;
		    SEND_BUF[7]=(data >> 8) & 0x00ff;
			  USART3_Send(SEND_BUF,8);	
			  delay_ms(500);	 
				USART3_Receive_Data(REC_BUF,&REC_BUF_LEN);
				for(j=3;j<11;j++)//8个报警
				{
					  if(REC_BUF[j]==0x00)
						{
						  if(Para_Buf[i]>0x00)
							{
								continue;
							}						
						}	
						else if(REC_BUF[j]<150)
						{		
							Para_Buf[i]=0x01;
						}	
						else if(REC_BUF[j]<199)
						{	
							Para_Buf[i]=0x02;//如果需要断电解决的就break，不判断剩下的报警信息
							break;
						}	
				}
				i=i+1;
		 }	 		
	return i;//返回长度
}

/**
  * @brief  电缸参数反馈
  *         
  *  
  * @param  rxBuf     串口解析函数收到的串口命令
  *         para      读取到的参数
  *         paraLen   读取到的参数长度
  *   
  * @retval 返回值说明
  *
  * @note   调用了CDV的串口发送命令加载函数
  */
void DG_ParaRequest(CDV_INT08U* rxBuf, CDV_INT08U* para, CDV_INT08U paraLen, CDV_INT08U uartNo)	{
	CDV_INT08U txLen = paraLen + 8;
	
	CDV_INT08U *txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(txLen));
	memcpy(txBuf , rxBuf - 3 , 8);
	memcpy(txBuf + 8 , para , paraLen);
	AddTx(txBuf , txLen , uartNo);
	free(txBuf);
	txBuf = NULL;
	
	resReqed = 1;
}
#define DG_DELAY 400
RET_STATUS DG_RUN(u8* rxBuf,u8 rxLen/*, u8 debug*/,CDV_INT08U uartNo)	
{
	u8 DG_id;//电缸控制器ID
	u8 DG_function;//电缸功能（设置参数，写参数，启动。。）
  u8 DG_points;//电缸点号
  u32 rcord=0;
	u16 DG_pflag;//电缸参数标记
//	u32 DG_pos;//电缸运动位置
//	u32 DG_para;//电缸参数
	u8 para_pos;//电缸参数位置
	u8 Para_len=0;
	CDV_INT32S retVal = 0;
//	OS_ERR err;
	u8 i=0;
	RET_STATUS ret =OPT_SUCCESS;
    para_pos=5;
	DG_id=rxBuf[0];
	DG_function=rxBuf[1];
	DG_points=rxBuf[2];
	DG_pflag= (rxBuf[3]<<8)+rxBuf[4];
//	if(DG_function>=31)//通用串口
//	{
//		General_Serial(rxBuf,rxLen); 
//	}	
//	else
//	{	
		switch(DG_function)
		{
			case 0x00: //电缸参数设置
						 if(DG_pflag & 0x0001)//位置
								 {
									  i=0;
							  Set_Coord(DG_id,DG_points,rxBuf[para_pos],rxBuf[para_pos+1],rxBuf[para_pos+2],rxBuf[para_pos+3]);
									  i=i+4;
									  delay_ms(100);
								 }
								 if(DG_pflag & 0x0002)//速度
								 {
		 
							  Set_Speed(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0004)//加速度
								 {
							  Set_Acc(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0008)//减速度
								 {
							  Set_Dec(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0010)//动作方法
								 {
							  Set_Action_Type(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0020)//推压力
								 {
							  Set_Tyl(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0040)//阈值
								 {
							  Set_Space(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0080)//退压速度
								 {
							  Set_Tys(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0100)//最大推力
								 {
							  Set_Max_Tl(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0200)//区域输出端1
								 {
							  Set_Out_Space1(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1],rxBuf[para_pos+i+2],rxBuf[para_pos+i+3]);
									  i=i+4;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0400)//区域输出端2
								 {
							  Set_Out_Space2(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1],rxBuf[para_pos+i+2],rxBuf[para_pos+i+3]);
									  i=i+4;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0800)//定位幅
								 {
							  Set_Dwf(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1],rxBuf[para_pos+i+2],rxBuf[para_pos+i+3]);
									  i=i+4;
									 delay_ms(100);
								 }
								 //
								 //
								 //
						break;
			case 0x01://电缸参数读取
						Para_len=Read_Current_Inf(DG_id,DG_points,DG_pflag);
					  // UART4_Send(Para_Buf,Para_len);
					 // DG_ParaRequest(rxBuf,Para_Buf,Para_len);
						  ResParaRequest(rxBuf,rxLen , Para_Buf,Para_len, uartNo);
						break;
			case 0x02://电缸移动
					  wt_flag=0;
					  coords=0;
	//			      if(1!=debug) {
						if(rxBuf[6]==0)//回零
						{
									coords=0;
									Setup(DG_id);
										delay_ms(200);
	//			            while(DG_Receive_Data[4]!=0x0e)//等待回零完成
	//									{
	//										if(DG_UART5_RX_OVER==1)
	//										{	
	//											if(DG_Receive_Data[4]!=0x0e)//回零没完成
	//											{	
	//												delay_ms(100);
	//												Verify_Setup(DG_id); 
	//											}
	//										}	
	//										else Over_Setup(DG_id);//回零动作结束
	//										USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
	////										UART4_Send(DG_Receive_Data,DG_Receive_Len);
	//									}	
								   Over_Setup(DG_id);//回零动作结束
									  
										//break;
								}	
								else//点动
								{
									 
								  ret=(RET_STATUS)Point_run(DG_id,rxBuf[6]);
								  if(ret!=2)
								  {
										ret=(RET_STATUS)Point_run(DG_id,rxBuf[6]);
								  }	  
								  
								}
	//						}else{
	//							g_point = rxBuf[6];
	//						  g_refresh = 1;
	//						  g_id = DG_id;
	//						}
					 // OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 10ms(DG_DELAY);
						break;
			case 0x03://电缸等待
	//							if(debug) {
	//								;
	//							} else {
									ret =Wait_Run_Over(DG_id);
	//							}
					  //  OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 10ms(DG_DELAY);
						break;
			case 0x04://伺服启动
							DG_Ready(DG_id,rxBuf[6]&0x01);//使能串行+伺服ON
	//						 while(!(DG_Receive_Data[4]&0x02))//等待伺服完成
	//						 {
	//								if(DG_UART5_RX_OVER==1) //接收电缸控制器返回的数据完成
	//								{	
	//									if(!(DG_Receive_Data[4]&0x02))//伺服没完成
	//									{	
	//										delay_ms(100);
	//										Verify_SVON(DG_id);//再发一次读取数据 
	//									 
	//									}	
	//								}	
	//								USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
	////								UART4_Send(DG_Receive_Data,DG_Receive_Len);
	//						 }	
	//OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 10ms(DG_DELAY);
							 break;
		 case 0x05://重启	
				 wt_flag=0;
				 Reset(DG_id);//
					   delay_ms(100);
							 Over_Reset(DG_id);//重启动作结束
							 coords=0;
							// OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 10ms(DG_DELAY);
					  break;	
	  case 0x06://微调+
				   
				   // coords = 0;
						   if(wt_flag==0)
							 {	
									 Para_len=Read_Current_Inf(DG_id,0,0x2000);
									 //Para_len=Read_Current_Inf(DG_id,0,0x2000);
									 coords = (Para_Buf[0]<<24)+(Para_Buf[1]<<16)+(Para_Buf[2]<<8)+(Para_Buf[3]);				    
							  }
							  wt_flag=1;	
				  delay_ms(100); 				
						  coords=coords+((rxBuf[5]<<8)+rxBuf[6]);
				 // coords=(rxBuf[5]<<8)+rxBuf[6]; 
				  directly_run.coord[DG_id][0]=coords>>24;
					directly_run.coord[DG_id][1]=coords>>16;
					directly_run.coord[DG_id][2]=coords>>8;
					directly_run.coord[DG_id][3]=coords;
					Start_directly_run(DG_id);
					//OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 10ms(DG_DELAY);
				 break; 	
	   case 0x07://微调-

					  if(wt_flag==0)
					  {	
						 Para_len=Read_Current_Inf(DG_id,0,0x2000); 
						//Para_len=Read_Current_Inf(DG_id,0,0x2000);
						 coords = (Para_Buf[0]<<24)|(Para_Buf[1]<<16)|(Para_Buf[2]<<8)|(Para_Buf[3]);				
					}
					  wt_flag=1;	
			  delay_ms(100);				 
					  if(coords>((rxBuf[5]<<8)+rxBuf[6]))	
					  {	          				   
						  coords=coords-((rxBuf[5]<<8)+rxBuf[6]);
					  }
			  else  coords=0; 				 
						directly_run.coord[DG_id][0]=coords>>24;
						directly_run.coord[DG_id][1]=coords>>16;
						directly_run.coord[DG_id][2]=coords>>8;
						directly_run.coord[DG_id][3]=coords;
						Start_directly_run(DG_id);		
			 // OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 10ms(DG_DELAY);
				 break;	 
			
	  case 0x08://暂停	
				 Hold(DG_id,rxBuf[6]&0x01);
				// OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 10ms(DG_DELAY);
					  break;
	  case 0x09://
				  rcord=((rxBuf[5]<<8)+rxBuf[6]);
				  directly_run.coord[DG_id][0]=rcord>>24;
						directly_run.coord[DG_id][1]=rcord>>16;
						directly_run.coord[DG_id][2]=rcord>>8;
						directly_run.coord[DG_id][3]=rcord;
						Start_directly_run(DG_id);
				  
	  case 0x10:
					FrequencySet(rxBuf,SCRIP_FRE,0);
					break;
	  case 0x11:
					FrequencyRead(rxBuf,SCRIP_FRE,0, uartNo);
					break;
	  case 0x12:
					FreqcyForward(rxBuf,SCRIP_FRE,0);
					break;
	  case 0x013:
					FreqcyReverse(rxBuf,SCRIP_FRE,0);
					break;
	  case 0x14:
					FreqcyStop(rxBuf,0);
					break;
	  case 0x15:
					ret = FreqcyWait(rxBuf,0);
						break;	 
		case 0x1F:
				if(DG_function>=31)//通用串口
	{
		General_Serial(rxBuf,rxLen); 
	}	
	break;
		/**********************压力传感器***************************/	
		case 0x20://称重传感器参数设置
					Set_Weight_Sensor_Par(DG_id,DG_pflag,para_pos,rxBuf ) ; 
				 break;
		case 0x21://称重传感器参数读取
				   
				 Para_len=Read_Weight_Sensor_Inf(DG_id,DG_pflag);
				   DG_ParaRequest(rxBuf,Para_Buf,Para_len, uartNo);
				 Para_len=0;
				 break;
		case 0x22:
			//DG_pflag = 1;
				   Para_len=Read_Weight_Sensor_Inf(DG_id,1);
				   ValSet(rxBuf[3],(Para_Buf[0]<<24)+(Para_Buf[1]<<16)+(Para_Buf[2]<<8)+ Para_Buf[3]);
				 break;	
		case  0x40:
				
					 
		      retVal=PH_ReadValue(1);
					if(retVal>0)
					{
						Para_Buf[0] = (retVal>>24)&0x000000ff;
						Para_Buf[1] = (retVal>>16)&0x000000ff;
						Para_Buf[2] = (retVal>>8)&0x000000ff;
						Para_Buf[3] = (retVal)&0x000000ff;
						Para_len=4;
						ResParaRequest(rxBuf, rxLen, Para_Buf, Para_len, uartNo);
					}
					else
					{
						ret =OPT_FAILURE;
					}
					break;
    case  0x41:
				
					 
		      retVal=PH_ReadValue(1);
					if(retVal>0)
					{
						//DW_CDV_VAR(rxBuf[3]) = retVal;
						ValSet(rxBuf[3], retVal);
					}
					else
					{
						ret =OPT_FAILURE;
					}
					break;
		}	
	
//	DG_UART5_RX_CNT=0;
	return ret;
}	

/**
  * @brief  CDV 电缸刷新。
  *  
  * @param  无
  *   
  * @retval 无
  *
  * @note   
  */
void DGRefresh(void) {
	if(g_refresh) {
		if(g_point == 0)//回零
		{
				Setup(g_id);
				delay_ms(100);
				Over_Setup(g_id);//回零动作结束	
		}
		else//点动
		{
			Point_run(g_id,g_point);
		}
		g_refresh = 0;
	}
}

