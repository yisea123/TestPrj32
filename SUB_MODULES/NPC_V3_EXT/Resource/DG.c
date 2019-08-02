
#include "DG.h"
//#include "rs485.h"
//#include "uart5.h"
//#include "delay.h"
//#include "w25qxx.h"
//u8 ENABLE_SERIAL_CODE[]={0x01,0x05,0x00,0x30,0xff,0x00,0x8c,0x35};//ָʾ����ͨ����Ч
// u8 SVON_CODE[]={0x01,0x05,0x00,0x19,0xff,0x00,0x5d,0xfd};//�ŷ�ON
//const u8 HOLD_CODE[]={0x01,0x05,0x00,0x18,0xff,0x00,0x0c,0x3d};//��ʱֹͣ��hold��
//const u8 REHOLD_CODE[]={0x01,0x05,0x00,0x18,0x00,0x00,0x4d,0xcd};//ȡ����ʱֹͣ��hold��
// u8 SETUP_CODE[]={0x01,0x05,0x00,0x1c,0xff,0x00,0x4D,0xfc};//����
// u8 OVER_SETUP_CODE[]={0x01,0x05,0x00,0x1c,0x00,0x00,0x0c,0x0c};//���㶯������
//const u8 start_directly_CODE[]={0x01,0x10,0x91,0x00,0x00,0x01,0x02,0x01,0x00,0x27,0x09};//����ֱ������
//u8 RESET_CODE[]={0x01,0x05,0x00,0x1b,0xff,0x00,0xfc,0x3d};//����
//u8 OVER_RESET_CODE[]={0x01,0x05,0x00,0x1b,0x00,0x00,0xbd,0xcd};//��������

//const u8 POINT_RUN_CODE[]={0x01,0x05,0x00,0x1a,0xff,0x00,0xad,0xfd};//���� �㶯����
//const u8 OVER_POINT_RUN_CODE[]={0x01,0x05,0x00,0x1A,0x00,0x00,0xEC,0x0D};//�㶯����

//u8 DONE_CODE[]={0x01,0x02,0x00,0x40,0x00,0x10,0x78,0x12};//��ȡ״̬

//extern u8 DG_BUF[16];
//extern u8 DG_BUF_LEN;
u8 DG_Receive_Data[16];
u32 DG_Receive_Len;
//extern u8 Rec_Over;
//extern u8 DG_BUF[16];
extern u8  USART3_REC_OVER;
u32 coords=0;

u8 g_point=0;//�����׵�λ
u8 g_refresh=0;
u8 g_id=0;

u8 wt_flag=0;//���΢�����
DG_PAR directly_run={
                     {{0x01},{0x00,0x01}},//�˶���ʽ(����/���)Ϊ����
                     {{0x01},{0x00 ,0xf4}},//�ٶ�
                     {{0x01},{0x00 ,0x00 ,0x00 ,0x00}},//λ��
                     {{0x01},{0x03 ,0x88}},//���ٶ�
										 {{0x01},{0x03 ,0x88}},//���ٶ�
										 {{0x01},{0x00 ,0x00}},//��ѹ��
										 {{0x01},{0x00 ,0x00}},//��ֵ	 
                     {{0x01},{0x00 ,0x14}},//��ѹ�ٶ�
										 {{0x01},{0x00 ,0x64}},//���λ����
                     {{0x01},{0x00 ,0x00 ,0x00 ,0x00}},//���������1
                     {{0x01},{0x00 ,0x00 ,0x00 ,0x00}},//���������2
                     {{0x01},{0x00 ,0x00 ,0x00 ,0x64}},//��λ�� 	
                     {{0x01},{0x00,0x00}},//��������		
                     0x01,//������ID										 
                    };//ֱ��ָ��λ�á��ٶȵ���Ϣ�˶�
DG_Script DG1;

u8 DIRECTLY_RUN_BUF[41]={0x01,0x10,0x91,0x02,0x00,0x10,0x20};//�洢ֱ���˶��Ĳ���

////*******************
////����У����
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
//ָʾ����ͨ����Ч
//ID��������ID�� 1-32
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
//��ʱֹͣ/��������
//ID��������ID�� 1-32
//state��1��ͣ
//       0����
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
//�ŷ�ON/0ff
////ID��������ID�� 1-32
//state:1 �ŷ� ��0 ȡ���ŷ�
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
//ȷ���ŷ�ON
//ID��������ID�� 1-32
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
////�ȴ��ŷ�ON
////����0���ŷ�ONʧ�ܣ�0x02�ŷ����
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
//��׻���
//ID��������ID�� 1-32
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
//���㶯������
//ID��������ID�� 1-32
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
//ȷ�ϻ������
//ID��������ID�� 1-32
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
//�ȴ��������
//����0x04�����㶯������
//****************************************************
//u8 Wait_Setup(u8 data)
//{
//	while(data!=0x04)
//	{
//		data=Verify_Setup(1);
//	}	
//	Over_Setup(1);//���㶯������
//	return data;
//}	
//***************************************************
//����
//ID��������ID�� 1-32
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
//ȷ��������ɣ�ALARM(x4f)���1��
//ID��������ID�� 1-32
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
//��������
//ID��������ID�� 1-32
//***************************************************
void Over_Reset(u8 ID)
{
	//USART3_Send(OVER_RESET_CODE,8);//�������
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
	Enable_serial(ID);//ָʾ������Ч
	delay_ms(500);
	delay_ms(500);
	
	SVON(ID,state);//�ŷ�ON
	delay_ms(500);
	//return Wait_SVON();//�ȴ��ŷ����
}



////**************************************************
////����������λ���·����ݣ����ݸ�ʽΪ1Byte ID+ 2Byte ��ַ +����(����ǰ)
////addr����ַ
////**************************************************
//void Paramater_Parse(u16 addr)
//{
//  if(Rec_Over==1)
//	{
//		 Rec_Over=0;	
//     directly_run.Controler_Id=DG_BUF[0];	//������ID	
//		 //DIRECTLY_RUN_BUF[0]=directly_run.Controler_Id;
//		 switch(addr)
//		 {
//			 /*******************************�����Ǳ������ֱ������λ�á��ٶ�...����(���ǵ�λ����)************/
//			 /*                             ������д�뵽������RAM�У�������ʧ                                */
//			 case 0x9102: //���ÿ��Ʒ�ʽ
//										directly_run.action_type[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.action_type[directly_run.Controler_Id][1]=DG_BUF[4];
//										// DIRECTLY_RUN_BUF[7]=directly_run.action_type[directly_run.Controler_Id][0];
//										// DIRECTLY_RUN_BUF[8]=directly_run.action_type[directly_run.Controler_Id][1];
//										 			 
//										break;
//			 case 0x9103: //�����ٶ�
//										directly_run.speed[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.speed[directly_run.Controler_Id][1]=DG_BUF[4];  
//			               //DIRECTLY_RUN_BUF[9]=directly_run.speed[directly_run.Controler_Id][0];
//										 //DIRECTLY_RUN_BUF[10]=directly_run.speed[directly_run.Controler_Id][1];
//										 
//										break;					 
//			 
//			 case 0x9104: //����λ��
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
//			 case 0x9106: //���ü��ٶ�
//										directly_run.acc[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.acc[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[15]=directly_run.acc[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[16]=directly_run.acc[directly_run.Controler_Id][1];
//										 
//										break;
//			 case 0x9107://���ü��ٶ�
//									 directly_run.dec[directly_run.Controler_Id][0]=DG_BUF[3];
//									 directly_run.dec[directly_run.Controler_Id][1]=DG_BUF[4];
////			              DIRECTLY_RUN_BUF[17]=directly_run.dec[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[18]=directly_run.dec[directly_run.Controler_Id][1];
//										  		 
//										break; 
//						 
//			 case 0x9108: //������ѹ��
//									  directly_run.tyl[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.tyl[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[19]=directly_run.tyl[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[20]=directly_run.tyl[directly_run.Controler_Id][1];
//										 		 
//										break;           
//				
//			 case 0x9109://������ֵ
//									  directly_run.space[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.space[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[21]=directly_run.space[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[22]=directly_run.space[directly_run.Controler_Id][1];
//										 
//										break;
//			 case 0x910a://���ü���ѹ�ٶ�
//				  
//									  directly_run.tys[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.tys[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[23]=directly_run.tys[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[24]=directly_run.tys[directly_run.Controler_Id][1];
//										  		 
//										break;
//			 case 0x910b: //�����������
//									  directly_run.max_tl[directly_run.Controler_Id][0]=DG_BUF[3];
//										directly_run.max_tl[directly_run.Controler_Id][1]=DG_BUF[4];
////			               DIRECTLY_RUN_BUF[25]=directly_run.max_tl[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[26]=directly_run.max_tl[directly_run.Controler_Id][1];
//										  		 
//										break;
//			 case 0x910c: //�������������1
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
//			 case 0x910e: //�������������2
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
//			 case 0x9110://���ö�λ��
//									 directly_run.dwf[directly_run.Controler_Id][0]=DG_BUF[3];
//									 directly_run.dwf[directly_run.Controler_Id][1]=DG_BUF[4];
//									 directly_run.dwf[directly_run.Controler_Id][2]=DG_BUF[5];
//									 directly_run.dwf[directly_run.Controler_Id][3]=DG_BUF[6];
////			              DIRECTLY_RUN_BUF[35]=directly_run.dwf[directly_run.Controler_Id][0];
////										 DIRECTLY_RUN_BUF[36]=directly_run.dwf[directly_run.Controler_Id][1];
////										 DIRECTLY_RUN_BUF[37]=directly_run.dwf[directly_run.Controler_Id][2];
////										 DIRECTLY_RUN_BUF[38]=directly_run.dwf[directly_run.Controler_Id][3];	
//										break;
//			 case 0x9100://��������0x91000100
//									 directly_run.start_directly[directly_run.Controler_Id][0]=DG_BUF[3];
//									 directly_run.start_directly[directly_run.Controler_Id][1]=DG_BUF[4];
//									 break;
//			 /*****************************�����ǵ㶯����*******************************/
//			 /*                             �������ú�洢���ⲿEEPROM��               */
//			              /********Step_No1-Step_No64*************/
//			default: if((addr>=0x0100)&&(addr<0x04ff))
//			          {
//									 switch(DG_BUF[2])
//									 {
//										 case 0x00://���ÿ��Ʒ�ʽ
//											         Set_Action_Type(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x01://�����ٶ�
//											         Set_Speed(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x02://����λ��
//											         Set_Coord(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4],DG_BUF[5],DG_BUF[6]);
//											         break;
//										 case 0x04://���ü��ٶ�
//											         Set_Acc(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x05://���ü��ٶ�
//											         Set_Dec(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x06://������ѹ����Ĭ��0
//											         Set_Tyl(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x07://������ֵ��Ĭ��0
//											         Set_Space(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);  
//											         break;
//										 case 0x08://������ѹ�ٶȣ�Ĭ��20
//											         Set_Tys(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x09://���ö�λ���������Ĭ��100
//											         Set_Max_Tl(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4]);
//											         break;
//										 case 0x0a://�������������1,Ĭ��0
//											         Set_Out_Space1(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4],DG_BUF[5],DG_BUF[6]);
//											         break;
//										 case 0x0c://�������������2��Ĭ��0
//											         Set_Out_Space2(DG_BUF[0],DG_BUF[1],DG_BUF[3],DG_BUF[4],DG_BUF[5],DG_BUF[6]);
//											         break;
//										 case 0x0e://���ö�λ����Ĭ��100
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
//���� ָ��λ��ֱ������ ���õĲ�����������ȡУ����
//ID��������ID�� 1-32
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
	 
	 data=getCRC16(DIRECTLY_RUN_BUF,39);//��ȡУ����
	 DIRECTLY_RUN_BUF[39]=data & 0x00ff;
	 DIRECTLY_RUN_BUF[40]=(data >> 8) & 0x00ff;
}	
//****************************************************
//������������
//ID��������ID�� 1-32
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
//����ֱ������
//ID��������ID�� 1-32
//****************************************************
void Start_directly_run(u8 ID)
{
	 if((ID>=1)&&(ID<=32))
	 {
			Process_Directly_Run_Code(ID);//����ֱ�����еĲ���
			USART3_Send(DIRECTLY_RUN_BUF,41);//���Ͳ���
			delay_ms(100);//��ʱҪ��
			Send_Directly_Run_Code(ID);
	 }	 
}	
//****************************************************
//ȷ�� ���н���
//f����0x0e���˶����
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
//�ȴ��˶����
//���أ�0x0e���
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
	Verify_Run_Over(ID);//ȷ���Ƿ����
//	delay_ms(100);
//	DG_UART5_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//	while(DG_Receive_Data[4]!=0x0e)//�ȴ�
//	{
////		if((DG_BUF[1]<<8+DG_BUF[2])==0x0018)//��ʱֹͣ
////		{
////			DG_BUF[2]=0;//001801
////		  DG_BUF[1]=0;
////		  Hold(DG_BUF[0],DG_BUF[3]);
////		}	
//        DG_UART5_RX_CNT=0;
//		if(DG_UART5_RX_OVER==1) 
//		{	
//				 
//			   Verify_Run_Over(ID);//ȷ���Ƿ����
//			   delay_ms(100);
//        }			
//		DG_UART5_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//		//UART4_Send(DG_Receive_Data,DG_Receive_Len);
//	 }	
//	return ret;

//	while(!DG_UART5_RX_OVER){//�ȴ�
//		DG_UART5_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//	}
//	
	delay_ms(100); 
	
	do{//�ȴ�
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
//                                  �㶯
//
//**********************************************************************************

//**********************************
//�������з�ʽ������/��ԣ�
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH:���ݸ�λ
//dataL:���ݵ�λ
//0x0000:����˶���0x0001�������˶�
//**********************************
void Set_Action_Type(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1);
	send_buf[7]=dataH;//Ĭ�ϴ���
	send_buf[8]=dataL;//
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}	            
//**********************************
//�����ٶ�
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH:���ݸ�λ
//dataL:���ݵ�λ
//**********************************
void Set_Speed(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+1;
	send_buf[7]=dataH;//Ĭ��ֵ
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}	 
//**********************************
//����λ��
//����Step_Num:Ҫ���Ƶĵ��
//ID:��׿�����ID�ţ�1-32��
//dataH1:����ֽ�
//dataL2:����ֽ�
//**********************************
void Set_Coord(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2)
{
	u16 data;
	u8 send_buf[13]={0x01,0x10,0x04,0,0x00,0x02,0x04,0,0,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+2;
	send_buf[7]=dataH1;//Ĭ��ֵ
	send_buf[8]=dataH2;
	send_buf[9]=dataL1;//Ĭ��ֵ
	send_buf[10]=dataL2;
	data=getCRC16(send_buf,11);
	send_buf[11]=data & 0x00ff;
  send_buf[12]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,13);
}	 
//**********************************
//���ü��ٶ�
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH:���ݸ�λ
//dataL:���ݵ�λ
//**********************************
void Set_Acc(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;       
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+4;
	send_buf[7]=dataH;//Ĭ��ֵ
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//���ü��ٶ�
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH:���ݸ�λ
//dataL:���ݵ�λ
//**********************************
void Set_Dec(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+5;
	send_buf[7]=dataH;//Ĭ��ֵ
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//������ѹ��
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH:���ݸ�λ
//dataL:���ݵ�λ
//**********************************
void Set_Tyl(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+6;
	send_buf[7]=dataH;//Ĭ��ֵ
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//������ֵ
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH:���ݸ�λ
//dataL:���ݵ�λ
//**********************************
void Set_Space(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+7;
	send_buf[7]=dataH;//Ĭ��ֵ
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//������ѹ�ٶ�
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH:���ݸ�λ
//dataL:���ݵ�λ
//**********************************
void Set_Tys(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+8;
	send_buf[7]=dataH;//Ĭ��ֵ
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//�������λ����
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH:���ݸ�λ
//dataL:���ݵ�λ
//**********************************
void Set_Max_Tl(u8 ID,u8 Step_Num,u8 dataH,u8 dataL)
{
	u16 data;
	u8 send_buf[11]={0x01,0x10,0x04,0,0x00,0x01,0x02,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+9;
	send_buf[7]=dataH;//Ĭ��ֵ
	send_buf[8]=dataL;
	data=getCRC16(send_buf,9);
	send_buf[9]=data & 0x00ff;
  send_buf[10]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,11);
}
//**********************************
//�������������1
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH1:����ֽ�
//dataL2:����ֽ�
//**********************************
void Set_Out_Space1(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2)
{
	u16 data;
	u8 send_buf[13]={0x01,0x10,0x04,0,0x00,0x02,0x04,0,0,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+10;
	send_buf[7]=dataH1;//Ĭ��ֵ
	send_buf[8]=dataH2;
	send_buf[9]=dataL1;//Ĭ��ֵ
	send_buf[10]=dataL2;
	data=getCRC16(send_buf,9);
	send_buf[11]=data & 0x00ff;
  send_buf[12]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,13);
}
//**********************************
//�������������2
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH1:����ֽ�
//dataL2:����ֽ�
//**********************************
void Set_Out_Space2(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2)
{
	u16 data;
	u8 send_buf[13]={0x01,0x10,0x04,0,0x00,0x02,0x04,0,0,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+12;
	send_buf[7]=dataH1;//Ĭ��ֵ
	send_buf[8]=dataH2;
	send_buf[9]=dataL1;//Ĭ��ֵ
	send_buf[10]=dataL2;
	data=getCRC16(send_buf,11);
	send_buf[11]=data & 0x00ff;
  send_buf[12]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,13);
}
//**********************************
//���ö�λ��
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//dataH1:����ֽ�
//dataL2:����ֽ� 
//**********************************
void Set_Dwf(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2)
{
	u16 data;
	u8 send_buf[13]={0x01,0x10,0x04,0,0x00,0x02,0x04,0,0,0,0,0,0};
	send_buf[0]=ID;
	send_buf[3]=0x10*(Step_Num-1)+14;
	send_buf[7]=dataH1;//Ĭ��ֵ
	send_buf[8]=dataH2;
	send_buf[9]=dataL1;//Ĭ��ֵ
	send_buf[10]=dataL2;
	data=getCRC16(send_buf,11);
	send_buf[11]=data & 0x00ff;
  send_buf[12]=(data >> 8) & 0x00ff;
  USART3_Send(send_buf,13);
}
//**********************************
//���ò�����(Ҫ���еĵ�ţ�������֮ǰҪ��ѡ��㣩)
//ID:��׿�����ID�ţ�1-32��
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
//�����㶯���У���������ָ�
//
//ID:��׿�����ID�ţ�1-32��
//**********************************
u8 Activate_Point_Run(u8 ID)
{
	u8 i;
	u8 DG_return_data[16]={0};
	u32 data_len=0; 
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x1a,0xff,0x00,0xad,0xfd};//���� �㶯����
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
//�㶯���н�����DRIVE(Y1A)��0
//
//ID:��׿�����ID�ţ�1-32��
//**********************************
u8 Over_Point_Run(u8 ID)
{
	u8 i;
	u8 DG_return_data[16]={0};
	u32 data_len=0; 
	u16 data;
	u8 send_buf[]={0x01,0x05,0x00,0x1A,0x00,0x00,0xEC,0x0D};//�㶯����
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
//�㶯���к���
//Step_Num:Ҫ���еĵ�
//ID:��׿�����ID�ţ�1-32��
//************************************
u8 Point_run(u8 ID,u8 Step_Num)
{
	u8 state;
	Set_Action_Type(ID,Step_Num,0x00,0x01);//�����˶�
	delay_ms(300);

	state=Select_Step(ID,Step_Num);//ѡ��ڼ�����
	if(state!=2)
	{	
		return state;
	}
    else
	{	
	  state=Activate_Point_Run(ID);//��������ָ���DRIVE��1��
	  if(state!=2)
		{	
			Over_Point_Run(ID);//��DRIVE��0
			return state;
		}
		else
	    {	
			state=Over_Point_Run(ID);//��DRIVE��0
			if(state!=2)
			{	
				Over_Point_Run(ID);//��DRIVE��0
				return state;
			}
			else return 2;
	    }
	}	
		
}


//**********************************************
//������ýű���ȡ�����͵���׿�����
//addr:�ű���ַ
//num:ѡ��Ҫ���ü�����(1-64)
//**********************************************
//u8 DG_Script_Read_And_Config(u32 addr,u8 num)
//{
//	 u8 i;
//	 u8 step_no;
//	 u8 data_buf[4];
//	 u32 script_pos=0; //�ű�λ��
//	 W25QXX_Read(&DG1.DG_No,addr,1);//��ȡ��׺�
//	 W25QXX_Read(&DG1.DG_Name_Len,addr+1,1);//��ȡ������Ƴ���
//	 script_pos=script_pos+addr+2+DG1.DG_Name_Len;
//	 for(i=0;i<num;i++)
//	 {
//		 W25QXX_Read(&step_no,script_pos,1);//��ȡ������
//		 
//		 W25QXX_Read(data_buf,script_pos+1,4);//��ȡλ��
//		 Set_Coord(DG1.DG_No,step_no,data_buf[0],data_buf[1],data_buf[2],data_buf[3]);//����λ��
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+5,2);//��ȡ�ٶ�
//		 Set_Speed(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//�����ٶ�
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+7,2);//��ȡ���ٶ�
//		 Set_Acc(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//���ü��ٶ�
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+9,2);//��ȡ���ٶ�
//		 Set_Dec(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//���ü��ٶ�
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+11,2);//��ȡ��������
//		 Set_Action_Type(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//���ö�������
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+13,2);//��ȡ��ѹ��
//		 Set_Tyl(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//������ѹ��
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+15,2);//��ȡ��ֵ
//		 Set_Space(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//������ֵ
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+17,2);//��ȡ��ѹ�ٶ�
//		 Set_Tys(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//������ѹ�ٶ�
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+19,2);//��ȡ��λ�������
//		 Set_Max_Tl(DG1.DG_No,step_no,data_buf[0],data_buf[1]);//���ö�λ�������
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+23,4);//��ȡ���������1
//		 Set_Out_Space1(DG1.DG_No,step_no,data_buf[0],data_buf[1],data_buf[2],data_buf[3]);//�������������1
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+27,4);//��ȡ���������2
//		 Set_Out_Space2(DG1.DG_No,step_no,data_buf[0],data_buf[1],data_buf[2],data_buf[3]);//�������������2
//		 delay_ms(50);
//		 W25QXX_Read(data_buf,script_pos+31,4);//��ȡ��λ��
//		 Set_Dwf(DG1.DG_No,step_no,data_buf[0],data_buf[1],data_buf[2],data_buf[3]);//���ö�λ��
//		 delay_ms(50);
//		 script_pos=script_pos+33;
//	 }
//	 return 1;
//}
//************************
//���Ժ���
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
//		  case 0x001c://����
//									Setup(DG_BUF[0]);
//									delay_ms(100);
//			            while(DG_Receive_Data[4]!=0x0e)//�ȴ��������
//									{
//										if(DG_UART5_RX_OVER==1)
//										{	
//											if(DG_Receive_Data[4]!=0x0e)//����û���
//											{	
//												delay_ms(100);
//												Verify_Setup(DG_BUF[0]); 
//											}
//										}	
//										else Over_Setup(DG_BUF[0]);//���㶯������
//										USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//										UART4_Send(DG_Receive_Data,DG_Receive_Len);
//									}	
//			            DG_BUF[1]=0;
//									DG_BUF[2]=0;
//			            Over_Setup(DG_BUF[0]);//���㶯������
//									break;
//		  case 0x0019://�ŷ�ON
//									 DG_BUF[2]=0;
//									 DG_BUF[1]=0;
//									 DG_Ready(DG_BUF[0]);//ʹ�ܴ���+�ŷ�ON
//			             while(!(DG_Receive_Data[4]&0x02))//�ȴ��ŷ����
//									 {
//											if(DG_UART5_RX_OVER==1) //���յ�׿��������ص��������
//											{	
//												if(!(DG_Receive_Data[4]&0x02))//�ŷ�û���
//												{	
//												  delay_ms(100);
//													Verify_SVON(DG_BUF[0]);//�ٷ�һ�ζ�ȡ���� 
//												 
//												}	
//											}	
//											USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
//											UART4_Send(DG_Receive_Data,DG_Receive_Len);
//									 }	
//			             break;
//			case 0x0018://��ʱֹͣ
//									 DG_BUF[2]=0;//001801
//									 DG_BUF[1]=0;
//									 Hold(DG_BUF[0],DG_BUF[3]);//		 
//			case 0x001B://����
//									DG_BUF[0]=0;
//									DG_BUF[1]=0;//
//									Reset(DG_BUF[0]);//
//			            delay_ms(100);
////			            while(!(DG_Receive_Data[4]&0x80))//�ȴ��������
////									{
////										if(DG_UART5_RX_OVER==1) 
////										{
////                      if(!(DG_Receive_Data[4]&0x80))											
////											{
////                        delay_ms(100); 												
////												Verify_Reset(DG_BUF[0]);//ȷ�������Ƿ����
////											}
////                    }											
////			              //delay_ms(100);
////										USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
////										UART4_Send(DG_Receive_Data,DG_Receive_Len);
////									}	
//									  Over_Reset(DG_BUF[0]);//������������
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
//    Paramater_Parse(addr);//��������
//		Start_directly_run(DG_BUF[0]);//��������ģʽ����		
//	}		
//**********************************
//��ȡ�����
//����Step_Num:Ҫ���Ƶĵ��1-64
//ID:��׿�����ID�ţ�1-32��
//Para_No:������(1-12)

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
		    send_buf[5]=2;//��ȡ���� Word
			else
				send_buf[5]=1;
    }			
		else if(Para_No<=10)
		{	
			send_buf[3]=0x10*(Step_Num-1)+Para_No;
		  if(Para_No==10)
		    send_buf[5]=2;//��ȡ���� Word
			else
				send_buf[5]=1;
		}	
		else if(Para_No<=12)
		{	
			send_buf[3]=0x10*(Step_Num-1)+Para_No+1;
		  send_buf[5]=2;//��ȡ���� Word
		}
		data=getCRC16(send_buf,6);
		send_buf[6]=data & 0x00ff;
		send_buf[7]=(data >> 8) & 0x00ff;
		USART3_Send(send_buf,8);
	}	
}
//****************************************************
//��ȡ��ǰλ��,�ٶȣ�������Ŀ��λ�ã�����NO��������Ϣ
//
//ID:��׿�����ID�ţ�1-32��
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
		 if(addr & 0x0001)//λ��
		 {
				Read_Point_Para(DG_id,DG_points,3);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0002)//�ٶ�
		 {
				Read_Point_Para(DG_id,DG_points,2);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0004)//���ٶ�
		 {
				Read_Point_Para(DG_id,DG_points,4);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0008)//���ٶ�
		 {
				Read_Point_Para(DG_id,DG_points,5);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0010)//��������
		 {
				Read_Point_Para(DG_id,DG_points,1);
				delay_ms(500);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0020)//��ѹ��
		 {
				Read_Point_Para(DG_id,DG_points,6);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0040)//��ֵ
		 {
				Read_Point_Para(DG_id,DG_points,7);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0080)//��ѹ�ٶ�
		 {
				Read_Point_Para(DG_id,DG_points,8);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0100)//���λ������
		 {
				Read_Point_Para(DG_id,DG_points,9);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0200)//���������1
		 {
				Read_Point_Para(DG_id,DG_points,10);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0400)//���������2
		 {
				Read_Point_Para(DG_id,DG_points,11);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
		 if(addr & 0x0800)//��λ��
		 {
				Read_Point_Para(DG_id,DG_points,12);
				delay_ms(200);
				//while(!DG_UART5_RX_OVER)//�ȴ���׻�����
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
	if(addr & 0x1000)//��������NO(1-64)
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
		 if(addr & 0x2000)//��ǰλ��
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
		 if(addr & 0x4000)//��������Ϣ
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
				for(j=3;j<11;j++)//8������
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
							Para_Buf[i]=0x02;//�����Ҫ�ϵ����ľ�break�����ж�ʣ�µı�����Ϣ
							break;
						}	
				}
				i=i+1;
		 }	 		
	return i;//���س���
}

/**
  * @brief  ��ײ�������
  *         
  *  
  * @param  rxBuf     ���ڽ��������յ��Ĵ�������
  *         para      ��ȡ���Ĳ���
  *         paraLen   ��ȡ���Ĳ�������
  *   
  * @retval ����ֵ˵��
  *
  * @note   ������CDV�Ĵ��ڷ���������غ���
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
	u8 DG_id;//��׿�����ID
	u8 DG_function;//��׹��ܣ����ò�����д����������������
  u8 DG_points;//��׵��
  u32 rcord=0;
	u16 DG_pflag;//��ײ������
//	u32 DG_pos;//����˶�λ��
//	u32 DG_para;//��ײ���
	u8 para_pos;//��ײ���λ��
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
//	if(DG_function>=31)//ͨ�ô���
//	{
//		General_Serial(rxBuf,rxLen); 
//	}	
//	else
//	{	
		switch(DG_function)
		{
			case 0x00: //��ײ�������
						 if(DG_pflag & 0x0001)//λ��
								 {
									  i=0;
							  Set_Coord(DG_id,DG_points,rxBuf[para_pos],rxBuf[para_pos+1],rxBuf[para_pos+2],rxBuf[para_pos+3]);
									  i=i+4;
									  delay_ms(100);
								 }
								 if(DG_pflag & 0x0002)//�ٶ�
								 {
		 
							  Set_Speed(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0004)//���ٶ�
								 {
							  Set_Acc(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0008)//���ٶ�
								 {
							  Set_Dec(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0010)//��������
								 {
							  Set_Action_Type(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0020)//��ѹ��
								 {
							  Set_Tyl(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0040)//��ֵ
								 {
							  Set_Space(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0080)//��ѹ�ٶ�
								 {
							  Set_Tys(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0100)//�������
								 {
							  Set_Max_Tl(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1]);
									  i=i+2;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0200)//���������1
								 {
							  Set_Out_Space1(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1],rxBuf[para_pos+i+2],rxBuf[para_pos+i+3]);
									  i=i+4;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0400)//���������2
								 {
							  Set_Out_Space2(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1],rxBuf[para_pos+i+2],rxBuf[para_pos+i+3]);
									  i=i+4;
									 delay_ms(100);
								 }
								 if(DG_pflag & 0x0800)//��λ��
								 {
							  Set_Dwf(DG_id,DG_points,rxBuf[para_pos+i],rxBuf[para_pos+i+1],rxBuf[para_pos+i+2],rxBuf[para_pos+i+3]);
									  i=i+4;
									 delay_ms(100);
								 }
								 //
								 //
								 //
						break;
			case 0x01://��ײ�����ȡ
						Para_len=Read_Current_Inf(DG_id,DG_points,DG_pflag);
					  // UART4_Send(Para_Buf,Para_len);
					 // DG_ParaRequest(rxBuf,Para_Buf,Para_len);
						  ResParaRequest(rxBuf,rxLen , Para_Buf,Para_len, uartNo);
						break;
			case 0x02://����ƶ�
					  wt_flag=0;
					  coords=0;
	//			      if(1!=debug) {
						if(rxBuf[6]==0)//����
						{
									coords=0;
									Setup(DG_id);
										delay_ms(200);
	//			            while(DG_Receive_Data[4]!=0x0e)//�ȴ��������
	//									{
	//										if(DG_UART5_RX_OVER==1)
	//										{	
	//											if(DG_Receive_Data[4]!=0x0e)//����û���
	//											{	
	//												delay_ms(100);
	//												Verify_Setup(DG_id); 
	//											}
	//										}	
	//										else Over_Setup(DG_id);//���㶯������
	//										USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
	////										UART4_Send(DG_Receive_Data,DG_Receive_Len);
	//									}	
								   Over_Setup(DG_id);//���㶯������
									  
										//break;
								}	
								else//�㶯
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
					 // OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 10ms(DG_DELAY);
						break;
			case 0x03://��׵ȴ�
	//							if(debug) {
	//								;
	//							} else {
									ret =Wait_Run_Over(DG_id);
	//							}
					  //  OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 10ms(DG_DELAY);
						break;
			case 0x04://�ŷ�����
							DG_Ready(DG_id,rxBuf[6]&0x01);//ʹ�ܴ���+�ŷ�ON
	//						 while(!(DG_Receive_Data[4]&0x02))//�ȴ��ŷ����
	//						 {
	//								if(DG_UART5_RX_OVER==1) //���յ�׿��������ص��������
	//								{	
	//									if(!(DG_Receive_Data[4]&0x02))//�ŷ�û���
	//									{	
	//										delay_ms(100);
	//										Verify_SVON(DG_id);//�ٷ�һ�ζ�ȡ���� 
	//									 
	//									}	
	//								}	
	//								USART3_Receive_Data(DG_Receive_Data,&DG_Receive_Len);
	////								UART4_Send(DG_Receive_Data,DG_Receive_Len);
	//						 }	
	//OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 10ms(DG_DELAY);
							 break;
		 case 0x05://����	
				 wt_flag=0;
				 Reset(DG_id);//
					   delay_ms(100);
							 Over_Reset(DG_id);//������������
							 coords=0;
							// OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 10ms(DG_DELAY);
					  break;	
	  case 0x06://΢��+
				   
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
					//OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 10ms(DG_DELAY);
				 break; 	
	   case 0x07://΢��-

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
			 // OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 10ms(DG_DELAY);
				 break;	 
			
	  case 0x08://��ͣ	
				 Hold(DG_id,rxBuf[6]&0x01);
				// OSTimeDlyHMSM(0,0,0,DG_DELAY,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 10ms(DG_DELAY);
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
				if(DG_function>=31)//ͨ�ô���
	{
		General_Serial(rxBuf,rxLen); 
	}	
	break;
		/**********************ѹ��������***************************/	
		case 0x20://���ش�������������
					Set_Weight_Sensor_Par(DG_id,DG_pflag,para_pos,rxBuf ) ; 
				 break;
		case 0x21://���ش�����������ȡ
				   
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
  * @brief  CDV ���ˢ�¡�
  *  
  * @param  ��
  *   
  * @retval ��
  *
  * @note   
  */
void DGRefresh(void) {
	if(g_refresh) {
		if(g_point == 0)//����
		{
				Setup(g_id);
				delay_ms(100);
				Over_Setup(g_id);//���㶯������	
		}
		else//�㶯
		{
			Point_run(g_id,g_point);
		}
		g_refresh = 0;
	}
}

