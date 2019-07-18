#include "Moto.h"
//#include "MotoParmTrans.h"
/**
  ******************************************************************************
  * @file    /moto.c 
  * @author  PCF
  * @version V2.5.1
  * @date    29-April-2017
  * @brief   moto drive
	*
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
#if ENABLE_FPGA_DOWN
MOTOR_INFO moto_par;
LINE_INFO line_par;
ARC_INFO arc_par;
MOTOR_STATE moto_sta;
MOTOR_RUNSTA Moto_RunSta[AXLE_NUM]={READY};
CDV_INT32S gmoto_pos[8]={0};
CDV_INT32S gmoto_pos_start[8]={0};
u8 gmoto_run_flag[AXLE_NUM] = {0};//������б�־ 1�������� 0����
u8 gmoto_flag[AXLE_NUM] = {0};
u8 gmoto_rzflag[AXLE_NUM] = {0};//�����ǣ��й����������1
u8 gmoto_suspend_flag[AXLE_NUM] = {0};//�����ͣ��ǣ�����ͣ�������1
u32 gAccAndDec_Cnt[AXLE_NUM] = {0};//S���ٶ�����ʹ�� �Ӽ��ٴ���ͳ�� 5ms����һ�μӼ���
u32 gAcc_Times[AXLE_NUM] = {0};//S���ٶ�����ʹ�� �����ܴ���
u32 gDec_Times[AXLE_NUM] = {0};//S���ٶ�����ʹ�� �����ܴ���
u32 gUniform_Times[AXLE_NUM] = {0};//S���ٶ�����ʹ�� �����ܴ���
u32 gStageStartup_Time[AXLE_NUM] = {0};//���׶ο�ʼʱ��
u32 bufmax_speed;
u8 gmoto_flag2;//����Ӽ��ٲ������������߱��
u8 gmoto_run_mode = TRAP_MODE;//�������ģʽ
u16 wait_err=0;
u32 xc=0;//Բ������x
u32 yc=0;//Բ������y
u32 xee=0;//�յ�����x
u32 yee=0;//�յ�����y
_PT xyc;
_PT xyc_temp[2];
u16 FPGA_STATE=0;
#define line_qua 0x7fffffff

//******��������ȡ����*****
u16 bmq_count_L=0;//������������16
u16 bmq_count_H=0;//������������16
u32 bmq_count=0;//����������32
//*************************
u16 mes_id;
u32 mes_pos;
void y2x(u32 xo,u32 yo,u32 xe,u32 ye,_PT xyc,u8 Moto_NO);//����ת��

//void FPGA_Send_parameter(u8 *buf,u8 no);
//RET_STATUS Line_Interpolation(u8* rxBuf,u8 rxLen, u8 debug);
void MOTO_Enable_Disable(u8 Moto_No,u8 typ);
u8 Moto_Config(u8 Moto_No,u32 Moto_Pflag,u8* rxBuf,u8 type,u8 storage);
RET_STATUS MotorStop(CDV_INT08U Moto_No );//�����ͣ
u8 Read_Moto_Par(u8 Moto_No,u32 pflag);
RET_STATUS Motor_Jog_Run(CDV_INT08U Moto_No ,u8 type,u8 state);//����㶯 
u8 Moto_Solute_Run(u8 Moto_No,u32 steps);
RET_STATUS Motor_Relative_Run(CDV_INT08U Moto_No , CDV_INT32S num,u8 type);
RET_STATUS Motor_Run_Zero(CDV_INT08U Moto_No);//�������
u32 Moto_Acc_Con(u32 user_data,u32 max_speed,u32 start_speed);//���ٶ�ת��
float Moto_Speed_Con(u32 user_data);//�����ٶ�ת��
void MOTO_Enable_Disable(u8 Moto_No,u8 typ);//ʹ�ܡ�ʧ��
u32 Moto_Dec_Con(u32 user_data,u32 max_speed,u32 min_speed);//���ٶ�ת��
u32 Get_Dstep(u32 max_speed,u32 min_speed,u32 dec_speed);//���ٲ���
u8 wait_moto_finish(u8 MOTO_num,u8 FPGA_NUM);
u8 MOTO_ACC_DEC(u8 Moto_No,u32 run_step,MOTOR_INFO *moto_pared);
RET_STATUS Motor_Stop(CDV_INT08U Moto_No);
void Cs_FPGA(u8 no);//ƬѡFPGA
RET_STATUS Arc_Interpolation(u8* rxBuf,u8 rxLen, CMD_ARG *arg);//Բ���岹
u8 Read_Arc_Par(u8 Fpga_No,u16 pflag);//Բ���岹������ȡ
u32 Arc_Runstep(u8 fpga_no ,u32 xo,u32 yo,u32 xe,u32 ye,u8 dir);//Բ���岹��������
u8 Arc_Run(u8* rxBuf);
RET_STATUS Line_Interpolation(u8* rxBuf,u8 rxLen, CMD_ARG *arg);//ֱ�߲岹
u8 Line_Config(u8 Fpga_no,u16 Moto_Pflag,u8* rxBuf,u8 type);//ֱ�߲岹��������
u8 Read_Line_Par(u8 Fpga_No,u16 pflag);//ֱ�߲岹������ȡ
u8 Xc_Yc(float x1,float y1,float x2,float y2,float r, u8 type);//��Բ������
RET_STATUS GratingRuler_CmdParse(u8* rxBuf,u8 rxLen, CMD_ARG *arg);	
u8 Read_GratingRuler_Pos(u8 Grating_No);
void Clear_GratingRuler(u8 Grating_No);
void GratingRuler_Ctrl(u8 Grating_No,u16 flag);
u8 Moto_MultSpeen_Config(u8 Moto_No,u32 Moto_Pflag,u8* rxBuf,u8 type);
RET_STATUS Motor_MultSpeen_Continue(CDV_INT08U Moto_No) ;
u8 Moto_MultSpeen_Config_Continue(u8 Moto_No,u8 Mult_Num_Now,u32 StepsHaveRun);
u32 MotoCountdown(MOTOR_INFO* motor,u8 Moto_No,u32 run_step) ;
u32 MaxSpeed_Real(u32 DecStep,u32 MinSpeed_Con,u32 acc_Con);
void MotoMessageSet(u8 Moto_No,u32 pos,u8 mode,u16 id);
u16 MotoMessageQuery(u8 Moto_No);
void MotoMessageClear(u8 Moto_No);
void MotorFwd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No);
void MotorRev(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No);
void MotorMoveTo(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No);
void U32ToBuf(u8 *dst,u32 data, u8 iBigEndian);
void U16ToBuf(u8 *dst,u32 data, u8 iBigEndian);
void MotorRtnZero(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No);
void ParRangeCtrl(CDV_INT32U* Data, CDV_INT32U Max,CDV_INT32U Min);
CDV_INT32U CalculateSModelLine_Par(CDV_INT32U speed_max, CDV_INT32U speed_min,CDV_INT32U  acc, CDV_FP32 flexible,CDV_INT32U cnt,CDV_INT08U State);
u32 Moto_Speed_Format(u32 user_data);
void MotorMultFwd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No);
void MotorMultRev(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No);
//*****************************************
//�����������
//Moto_pflag:�������
//no:�����0-7
//
//****************************************
u8 Moto_Config(u8 Moto_No,u32 Moto_Pflag,u8* rxBuf,u8 type,u8 storage)
{	
	 u8 i=0;
	 u8 j;
//	 u8 para_pos;
	 u8 buf[6]={0};
	 u32 buf_data;
	 u32 max_speed;
	 u32 min_speed;
	 u32 zero_speed;
	 u32 jog_run_speed;
	 u32 target_speed;
	 u32 acc;
	 u32 dec;
	 u32 m_speed,m_a_d_speed,m_step;
	  u8 m_speed_num;
//	 OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	 Cs_FPGA(Moto_No/2);
	 if(Moto_Pflag & 0x0001)//��������
	 { 
			i=0;
			moto_par.drvtype[Moto_No]=rxBuf[i];
			i=i+1;
	 }		 
	 if(Moto_Pflag & 0x0002)//����ٶ�
	 {
//		  for(j=0;j<4;j++)
//		  {
//		    moto_par.max_speed[Moto_No][3-j]=rxBuf[i+j];
//			}
		   moto_par.max_speed[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			i=i+4;
	 }
	 if(Moto_Pflag & 0x0004)//�����
	 {  
//		  for(j=0;j<4;j++)
//		  {
//		    moto_par.min_speed[Moto_No][3-j]=rxBuf[i+j];
//			}	
		   moto_par.min_speed[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			moto_par.max_speed[Moto_No] = moto_par.max_speed[Moto_No]>=moto_par.min_speed[Moto_No]?moto_par.max_speed[Moto_No]:moto_par.min_speed[Moto_No];
			//min_speed=UartToU32(&moto_par.min_speed[Moto_No][0],4,BIGENDIAN);
			
			buf_data=Moto_Speed_Con(moto_par.min_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x0A;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
			i=i+4;	
	 }
//	 if(Moto_Pflag & 0x0008)//���ٶ�
//	 {  
//		  for(j=0;j<4;j++)
//		  {
//		    moto_par.acc[Moto_No][j]=rxBuf[i+j];
//			}	
//			acc=(moto_par.acc[Moto_No][0]<<24)+(moto_par.acc[Moto_No][1]<<16)+(moto_par.acc[Moto_No][2]<<8)+moto_par.acc[Moto_No][3];
//			buf_data=Moto_Acc_Con(acc,max_speed,min_speed);
//			if((Moto_No&0x01)==0)
//				buf[0]=0x00;
//			else if((Moto_No&0x01)==1)
//				buf[0]=0x02; 
//			buf[1]=0x04;
//			buf[2]=buf_data>>24;
//			buf[3]=buf_data>>16;			
//			buf[4]=buf_data>>8;
//			buf[5]=buf_data;
//			 Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//			 i=i+4;
//	 }
//	 if(Moto_Pflag & 0x0010)//���ٶ�
//	 {
//		  for(j=0;j<4;j++)
//		  {
//		    moto_par.dec[Moto_No][j]=rxBuf[i+j];
//			}	
//			dec=(moto_par.dec[Moto_No][0]<<24)+(moto_par.dec[Moto_No][1]<<16)+(moto_par.dec[Moto_No][2]<<8)+moto_par.dec[Moto_No][3];
//			buf_data=Moto_Dec_Con(dec,max_speed,min_speed);
//			if((Moto_No&0x01)==0)
//				buf[0]=0x00;
//			else if((Moto_No&0x01)==1)
//				buf[0]=0x02; 
//			buf[1]=0x05;
//			buf[2]=buf_data>>24;
//			buf[3]=buf_data>>16;			
//			buf[4]=buf_data>>8;
//			buf[5]=buf_data;
//			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//			i=i+4;
//	 }
	 if(Moto_Pflag & 0x0020)//����λ
	 {
//		  for(j=0;j<4;j++)
//		  {
//		    moto_par.up_limit[Moto_No][3-j]=rxBuf[i+j];			  
//			}	
		   moto_par.up_limit[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
		   if(moto_par.up_limit[Moto_No]>2147483647)
				moto_par.up_limit[Moto_No] = 2147483647;
			i=i+4;
	 }
	 if(Moto_Pflag & 0x0040)//����λ
	 {
//		  for(j=0;j<4;j++)
//		  {
//		    moto_par.down_limit[Moto_No][3-j]=rxBuf[i+j];
//			}	
		   moto_par.down_limit[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			if(moto_par.down_limit[Moto_No]>2147483647)
				moto_par.down_limit[Moto_No] = 2147483647;
			i=i+4;
	 }
	 if(Moto_Pflag & 0x0080)//���㷽��
	 {
		  if(rxBuf[i]==2)//���㷽��ȡ��
			{
				moto_par.zero_dir[Moto_No]= moto_par.zero_dir[Moto_No]? 0 : 1;				
			}	
			else
			{	
				moto_par.zero_dir[Moto_No]=rxBuf[i];
			}	
			i=i+1;
	 }
	 if(Moto_Pflag & 0x0100)//�˶�����
	 {
		  moto_par.pstv_dir[Moto_No]=rxBuf[i];
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x0B;
			buf[2]=0x00;
			buf[3]=0x00;			
			buf[4]=0x00;
			buf[5]=moto_par.pstv_dir[Moto_No];

			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
			i=i+1;
	 }
	 if(Moto_Pflag & 0x0200)//ϸ����
	 {
//		  for(j=0;j<2;j++)
//		  {
//		    moto_par.div_num[Moto_No][1-j]=rxBuf[i+j];
//			}	
		   moto_par.div_num[Moto_No] = UartToU32(rxBuf+i,2,LITTLEEND);
			i=i+2;
	 }
	 if(Moto_Pflag & 0x0400)//�ֶ�Բֱ��
	 {
//		  for(j=0;j<2;j++)
//		  {
//		    moto_par.diameter[Moto_No][1-j]=rxBuf[i+j];
//			}	
		   moto_par.diameter[Moto_No] = UartToU32(rxBuf+i,2,LITTLEEND);
			i=i+2;
	 }
//	 if(Moto_Pflag & 0x0800)//˿�ܵ���
//	 {
//		  for(j=0;j<2;j++)
//		  {
//		    moto_par.lead[Moto_No][j]=rxBuf[i+j];
//			}	
//			i=i+2;
//	 }
	 if(Moto_Pflag & 0x0800)//�����ٶ�
	 {
		  if(type == 0xFD)i++;

		   moto_par.zero_speed[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			ParRangeCtrl(&moto_par.zero_speed[Moto_No],moto_par.max_speed[Moto_No],moto_par.min_speed[Moto_No]);
			moto_par.target_speed[Moto_No] =  moto_par.zero_speed[Moto_No] ;
			buf_data=Moto_Speed_Con(moto_par.zero_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x12;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
			//ͬʱ����Ŀ���ٶ�  ĿǰFPGA�����޹����ٶ�
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x06;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		
			i=i+4;

	 }
	 
	 if(Moto_Pflag & 0x1000)//�㶯�����ٶ�
	 {
		  if(type == 0xFD)i++;

		  moto_par.jog_run_speed[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			ParRangeCtrl(&moto_par.jog_run_speed[Moto_No],moto_par.max_speed[Moto_No],moto_par.min_speed[Moto_No]);
			
			buf_data=Moto_Speed_Con( moto_par.jog_run_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x11;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
			i=i+4;
	 }
//	 if(Moto_Pflag & 0x2000)//CW/CCW��˫����ģʽ�л�
//	 {
//		 moto_par.pul_mode[Moto_No][0]=rxBuf[i];
//			if((Moto_No&0x01)==0)
//				buf[0]=0x00;
//			else if((Moto_No&0x01)==1)
//				buf[0]=0x02; 
//			buf[1]=0x09;
//			buf[2]=0x00;
//			buf[3]=0x00;			
//			buf[4]=0x00;
//			buf[5]=rxBuf[i]&0x02;	  
//		 Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//		 i=i+1;
//	 }
//	 if(Moto_Pflag & 0x4000)//����ٲ�������
//	 {
//		i=0;
//		moto_par.mult_speed_num[Moto_No]=rxBuf[i];
//		moto_par.mult_speed_num[Moto_No] = moto_par.mult_speed_num[Moto_No]>10?10:moto_par.mult_speed_num[Moto_No];
//	   m_speed_num = moto_par.mult_speed_num[Moto_No];	 		 
//		i++;
//		for(j=0;j<m_speed_num;j++)
//		 {
//			moto_par.mult_a_d_speed[Moto_No][j]=UartToU32(&rxBuf[i],4,LITTLEEND);//(rxBuf[i]<<24)+(rxBuf[i+1]<<16)+(rxBuf[i+2]<<8)+rxBuf[i+3];
//			ParRangeCtrl(&moto_par.mult_a_d_speed[Moto_No][j],moto_par.max_acc[Moto_No],moto_par.min_acc[Moto_No]); 
//			moto_par.mult_speed[Moto_No][j]=UartToU32(&rxBuf[i+4],4,LITTLEEND);//(rxBuf[i+4]<<24)+(rxBuf[i+5]<<16)+(rxBuf[i+6]<<8)+rxBuf[i+7];
//		   ParRangeCtrl(&moto_par.mult_speed[Moto_No][j],moto_par.max_speed[Moto_No],moto_par.min_speed[Moto_No]); 
//			moto_par.mult_step[Moto_No][j]=UartToU32(&rxBuf[i+8],4,LITTLEEND);//(rxBuf[i+8]<<24)+(rxBuf[i+9]<<16)+(rxBuf[i+10]<<8)+rxBuf[i+11];
//			
//			i=i+12;
//		 }
//			
//		 Moto_MultSpeen_Config(Moto_No);
//	 }	 
	 
	 
	 if((Moto_Pflag & 0x00010000)&&(Moto_RunSta[Moto_No]==READY))//Ŀ�������ٶ�
	 {
		   if(type == 0xFD)i++;

			moto_par.target_speed[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			ParRangeCtrl(&moto_par.target_speed[Moto_No],moto_par.max_speed[Moto_No],moto_par.min_speed[Moto_No]); 
			buf_data=Moto_Speed_Con(moto_par.target_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x06;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);				
			i=i+4;						
			if(!(Moto_Pflag & 0x0008))//ֻ�����ٶ�ʱ��Ҳ��Ҫ�ԼӼ��ٽ������¼���
			{
				acc=moto_par.acc[Moto_No];
				buf_data=Moto_Acc_Con(acc,moto_par.target_speed[Moto_No],moto_par.min_speed[Moto_No]);
				if((Moto_No&0x01)==0)
					buf[0]=0x00;
				else if((Moto_No&0x01)==1)
					buf[0]=0x02; 
				buf[1]=0x04;
				buf[2]=buf_data>>24;
				buf[3]=buf_data>>16;			
				buf[4]=buf_data>>8;
				buf[5]=buf_data;
				 Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);				
			}
			if(!(Moto_Pflag & 0x0010))
			{
				dec=moto_par.dec[Moto_No];
				buf_data=Moto_Acc_Con(dec,moto_par.target_speed[Moto_No],moto_par.min_speed[Moto_No]);			
				if((Moto_No&0x01)==0)
					buf[0]=0x00;
				else if((Moto_No&0x01)==1)
					buf[0]=0x02; 
				buf[1]=0x05;
				buf[2]=buf_data>>24;
				buf[3]=buf_data>>16;			
				buf[4]=buf_data>>8;
				buf[5]=buf_data;
				Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);			
			}			
	 }	 	 
	 if(Moto_Pflag & 0x00020000)//�����ٶ�
	 {

			moto_par.max_acc[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			i=i+4;
	 }	
	 if(Moto_Pflag & 0x00040000)//��С���ٶ�
	 {

			moto_par.min_acc[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			i=i+4;
	 }		 
	 if(Moto_Pflag & 0x0008)//���ٶ�
	 {  
		  if(type == 0xFD)i++;

		   moto_par.acc[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			ParRangeCtrl(&moto_par.acc[Moto_No],moto_par.max_acc[Moto_No],moto_par.min_acc[Moto_No]); 
			buf_data=Moto_Acc_Con( moto_par.acc[Moto_No],moto_par.target_speed[Moto_No],moto_par.min_speed[Moto_No]);
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x04;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			 Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
			 i=i+4;
	 }
	 if(Moto_Pflag & 0x0010)//���ٶ�
	 {
		  if(type == 0xFD)i++;

		  moto_par.dec[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			ParRangeCtrl(&moto_par.dec[Moto_No],moto_par.max_acc[Moto_No],moto_par.min_acc[Moto_No]); 
			buf_data=Moto_Acc_Con(moto_par.dec[Moto_No],moto_par.target_speed[Moto_No],moto_par.min_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x05;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
			i=i+4;
	 }	 
	 if(Moto_Pflag & 0x00080000)//����ģʽ
	 {
			moto_par.zero_mode[Moto_No] = rxBuf[i];
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x32;
			buf[2]=0x00;
			buf[3]=0x00;			
			buf[4]=0x00;
			buf[5]=rxBuf[i];	  
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);					
			i=i+1;
	 }	  	 
	 if(Moto_Pflag & 0x00100000)//�˶�ģʽ
	 {
			moto_par.run_mode[Moto_No] = rxBuf[i];
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x36;
			buf[2]=0x00;
			buf[3]=0x00;			
			buf[4]=0x00;
			buf[5]=rxBuf[i];	  
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		 
			i=i+1;
	 }	  	 	 
	 if(Moto_Pflag & 0x00200000)//����������
	 {
		 moto_par.encoder_type[Moto_No] = rxBuf[i];
			i=i+1;
	 }	
	 if(Moto_Pflag & 0x00400000)//��϶����
	 {
			moto_par.backlash_comp[Moto_No] = rxBuf[i];
			i=i+1;
	 }	  	
	 if(Moto_Pflag & 0x00800000)//CW/CCW��˫����ģʽ�л�
	 {
		 moto_par.pul_mode[Moto_No]=rxBuf[i];
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x09;
			buf[2]=0x00;
			buf[3]=0x00;			
			buf[4]=0x00;
			buf[5]=((rxBuf[i]<<1)&0x02)|(moto_sta.enable[Moto_No]&0x01);	  
		 Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
		 i=i+1;
	 }	 
	 if(Moto_Pflag & 0x01000000)//�ٶ���������
	 {
		 moto_par.speed_type[Moto_No]=rxBuf[i];
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x37;
			buf[2]=0;
			buf[3]=0;			
			buf[4]=0;
			buf[5]=moto_par.speed_type[Moto_No]&0x01;	  
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
		 i=i+1;
	 }	 	 
		//**���ٲ���***//
	buf_data=Get_Dstep(moto_par.target_speed[Moto_No],moto_par.min_speed[Moto_No],moto_par.dec[Moto_No]);
	if(moto_par.run_mode[Moto_No]==1)//�ջ�
	{
		buf_data = (CDV_INT32U)((CDV_INT64U)buf_data*(CDV_INT64U)moto_par.encoder_pre[Moto_No]/(CDV_INT64U)moto_par.div_num[Moto_No]);
	}	

	if((Moto_No&0x01)==0)
		buf[0]=0x00;
	else if((Moto_No&0x01)==1)
		buf[0]=0x02; 
	buf[1]=0x08;
	buf[2]=buf_data>>24;
	buf[3]=buf_data>>16;			
	buf[4]=buf_data>>8;
	buf[5]=buf_data;	  
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	
	
  if(((type==0xFF) || (type==0xFE))&&storage)
	{	
		SPI_Flash_Write((CDV_INT08U *)&moto_par, MOTOPAR_ADDR, sizeof(moto_par));
	}//	OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
	return 1;
}		
//****************************************************
//��ȡ�������
//
//no:����ţ�0-7��
//****************************************************
u8 Read_Moto_Par(u8 Moto_No,u32 pflag)
{
	u8 i,j,k;
	u32 FPGA_POS;//��ǰλ��
	u32 FPGA_Speed;//��ǰ�ٶ�
	u32 FPGA_RunTime;//������ʱ��
	u32 FPGA_ResTime;//ʣ��ʱ��
	u16 FPGA_POSH;
	u16 FPGA_POSL;
	u32 ela_runtime;
	i=0;
	Cs_FPGA(Moto_No/2);
	 if(pflag & 0x0001)//��������
	 { 
			Para_Buf[i]=moto_par.drvtype[Moto_No];
			i=i+1;			
	 }
	 if(pflag & 0x0002)//����ٶ�
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.max_speed[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.max_speed[Moto_No],BIGENDIAN);			
			i=i+4;
	 }
	 if(pflag & 0x0004)//��С�ٶ�
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.min_speed[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.min_speed[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0008)//���ٶ�
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.acc[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.acc[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0010)//���ٶ�
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.dec[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.dec[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0020)//����λ
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.up_limit[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.up_limit[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0040)//����λ
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.down_limit[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.down_limit[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0080)//���㷽��
	 {
			Para_Buf[i]=moto_par.zero_dir[Moto_No];
			i=i+1;
	 }
	 if(pflag & 0x0100)//���з���
	 {
			Para_Buf[i]=moto_par.pstv_dir[Moto_No];
			i=i+1;
	 }
	 if(pflag & 0x0200)//ϸ����
	 {
//			for(j=0;j<2;j++)
//			{
//				Para_Buf[i+j]=moto_par.div_num[Moto_No][j];
//			}
			U16ToBuf(Para_Buf+i,moto_par.div_num[Moto_No],BIGENDIAN);
			i=i+2;
	 }
	 if(pflag & 0x0400)//�ֶ�Բֱ��
	 {
//			for(j=0;j<2;j++)
//			{
//				Para_Buf[i+j]=moto_par.diameter[Moto_No][j];
//			}
			U16ToBuf(Para_Buf+i,moto_par.diameter[Moto_No],BIGENDIAN);
			i=i+2;
	 }
	 if(pflag & 0x0800)//˿�ܵ���
	 {
//			for(j=0;j<2;j++)
//			{
//				Para_Buf[i+j]=moto_par.lead[Moto_No][j];
//			}
			U16ToBuf(Para_Buf+i,moto_par.lead[Moto_No],BIGENDIAN);
			i=i+2;
	 }
	 if(pflag & 0x1000)//��ǰλ��
	 {
			if(Moto_No%2)
				FPGA_POS=Read_Fpga_Regist(0x8F04,Moto_No>>1);
			else
				FPGA_POS=Read_Fpga_Regist(0x8F03,Moto_No>>1);
//      for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=FPGA_POS>>(24-j*8);
//			}
			U32ToBuf(Para_Buf+i,FPGA_POS,BIGENDIAN);
			i=i+4;
	 }
	 //bit1:x�����
	 //bit2:y�����
	 //bit3:Բ�����
	 //bit4:x������λ
	 //bit5:x������λ
	 //bit6:y������λ
	 //bit7:y������λ
	 //bit8:ֱ�߲岹���
	 //bit9:x����λ
	 //bit10:y����λ
	 if(pflag & 0x2000)//Ӳ����λ�ź�
	 {
		 FPGA_POSL=Read_Fpga_Regist(0x8F00,Moto_No>>1);
		 if(Moto_No%2==0)
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>3);//FPGA_POSL�ĵ���λ
			 
		 }
         else if(Moto_No%2==1)		 
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>5);//FPGA_POSL�ĵ�6λ
			 
		 }	 
		 i=i+1;
	 }	 
	 if(pflag & 0x4000)//Ӳ����λ�ź�
	 {
		 FPGA_POSL=Read_Fpga_Regist(0x8F00,Moto_No>>1);
		 if(Moto_No%2==0)
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>4);//FPGA_POSL�ĵ�5λ
			 
		 }
     else if(Moto_No%2==1)		 
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>6);//FPGA_POSL�ĵ�7λ
			 
		 }
		 i=i+1;
	 }
	 if(pflag & 0x8000)//Ӳ�����ź�
	 {
		 FPGA_POSL=Read_Fpga_Regist(0x8F00,Moto_No>>1);
		 if(Moto_No%2==0)
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>8);//FPGA_POSL�ĵ�9λ
			 
		 }
     else if(Moto_No%2==1)		 
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>9);//FPGA_POSL�ĵ�10λ
			 
		 }
		 i=i+1;
	 }
	 if(pflag & 0x00010000)//Ŀ�������ٶ�
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.target_speed[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.target_speed[Moto_No],BIGENDIAN);
			i=i+4;
	 }	 	 
	 if(pflag & 0x00020000)//�����ٶ�
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.max_acc[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.max_acc[Moto_No],BIGENDIAN);
			i=i+4;
	 }	
	 if(pflag & 0x00040000)//��С���ٶ�
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.min_acc[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.min_acc[Moto_No],BIGENDIAN);
			i=i+4;
	 }		 
	 if(pflag & 0x00080000)//����ģʽ
	 {
//			for(j=0;j<1;j++)
//			{
				Para_Buf[i]=moto_par.zero_mode[Moto_No];
//			}
			
			i=i+1;
	 }	  	 
	 if(pflag & 0x00100000)//�˶�ģʽ
	 {
//			for(j=0;j<1;j++)
//			{
				Para_Buf[i]=moto_par.run_mode[Moto_No];
//			}
			i=i+1;
	 }	  	 	 
	 if(pflag & 0x00200000)//����������
	 {
//			for(j=0;j<1;j++)
//			{
				Para_Buf[i]=moto_par.encoder_type[Moto_No];
//			}
			i=i+1;
	 }	
	 if(pflag & 0x00400000)//��϶����
	 {
//			for(j=0;j<1;j++)
//			{
				Para_Buf[i]=moto_par.backlash_comp[Moto_No];
//			}
			i=i+1;
	 }	  	
	 if(pflag & 0x00800000)//CW/CCW��˫����ģʽ�л�
	 {
		 Para_Buf[i] = moto_par.pul_mode[Moto_No];
		 i=i+1;
	 }	
	 if(pflag & 0x01000000)//��ǰ�ٶ�
	 {
		 FPGA_Speed=Read_Fpga_Regist(0x8F07+(Moto_No%2),Moto_No>>1);
		FPGA_Speed = FPGA_Speed/FPGA_ACC_DIV;
//		FPGA_Speed=(CDV_INT32U)((CDV_INT64U)FPGA_W_FRE_DIV2*(CDV_INT64U)FPGA_ACC_DIV/(CDV_INT64U)FPGA_Speed);
		if(FPGA_Speed<50)FPGA_Speed=0;

		U32ToBuf(Para_Buf+i,FPGA_Speed,BIGENDIAN);		
		i=i+4;		
	 }	  	 
 	  if(pflag & 0x02000000)//����ٲ�����ȡ
	 {
		 moto_par.mult_speed_num[Moto_No] = moto_par.mult_speed_num[Moto_No]>10?10:moto_par.mult_speed_num[Moto_No];
	   Para_Buf[i]= moto_par.mult_speed_num[Moto_No];	 		 
		i++;
		for(j=0;j<moto_par.mult_speed_num[Moto_No];j++)
		 {
			for(k=0;k<4;k++)
			{
				Para_Buf[i+j*12+k]=moto_par.mult_a_d_speed[Moto_No][j]>>(8*(3-k));
				Para_Buf[i+j*12+k+4]=moto_par.mult_speed[Moto_No][j]>>(8*(3-k));
				Para_Buf[i+j*12+k+8]=moto_par.mult_step[Moto_No][j]>>(8*(3-k));
			}					
		 }	
		i=i+12*moto_par.mult_speed_num[Moto_No];		 
	 }	 
	 if(pflag & 0x04000000)//�ٶ��������Ͷ�ȡ
	 {
		 Para_Buf[i] = moto_par.speed_type[Moto_No];
		 i=i+1;
	 }	
	 if(pflag & 0x08000000)//Ԥ������ʱ���ȡ
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=moto_sta.runtime[Moto_No]>>(8*(3-j));
			}		 
		 i=i+4;
	 }	
	 if(pflag & 0x10000000)//������ʱ��
	 {
			if(FpgaMotoWait(Moto_No))
				ela_runtime = 0xffffffff;
			else
				ela_runtime = CalcTimeMS(GetCdvTimeTick(),moto_sta.ela_runtime[Moto_No]);
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=ela_runtime>>(8*(3-j));
//			}	
				U32ToBuf(Para_Buf+i,ela_runtime,BIGENDIAN);
		 i=i+4;
	 }		 
	 if(pflag & 0x20000000)//Ŀ��λ��
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_sta.target_pos[Moto_No]>>(8*(3-j));
//			}		 
			U32ToBuf(Para_Buf+i,moto_sta.target_pos[Moto_No],BIGENDIAN);	
		 i=i+4;
	 }			 
	 if(pflag & 0x40000000)//ʹ��/ʧ��״̬
	 {
	   	Para_Buf[i] = moto_sta.enable[Moto_No];
		   i++;
	 }
	return i;//���س���
}	
//**************************************************************
//���������ʼ��
//Moto_No:�����
//Moto_Pflag:�������
//**************************************************************
void Moto_Init(u8 Moto_No,u32 Moto_Pflag)	
{
//   u8 i;
//	 u8 j;
//	 u8 para_pos;
	 u8 buf[6];
	 u32 buf_data;
	 u32 max_speed;
	 u32 min_speed;
	 u32 zero_speed;
	 u32 target_speed,jog_run_speed;
	 u32 acc;
	 u32 dec;
//	 OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	 
	// U32ToBuf(moto_par.encoder_pre[Moto_No],1000*4,LITTLEEND);//������������Ϊ1000  4��Ƶ
	moto_par.encoder_pre[Moto_No] = 1000*4;
	Clear_GratingRuler(Moto_No);
	 Cs_FPGA(Moto_No/2);
	 if(Moto_Pflag & 0x0002)//����ٶ�
	 {
			//max_speed=(moto_par.max_speed[Moto_No][0]<<24)+(moto_par.max_speed[Moto_No][1]<<16)+(moto_par.max_speed[Moto_No][2]<<8)+moto_par.max_speed[Moto_No][3];
		 moto_par.max_speed[Moto_No] = moto_par.max_speed[Moto_No]>=moto_par.min_speed[Moto_No]?moto_par.max_speed[Moto_No]:moto_par.min_speed[Moto_No];
      buf_data=Moto_Speed_Con(moto_par.max_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x06;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;		
			buf[4]=buf_data>>8;
			buf[5]=buf_data;			
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	 }
	 if(Moto_Pflag & 0x0004)//�����
	 {  
			//min_speed=(moto_par.min_speed[Moto_No][0]<<24)+(moto_par.min_speed[Moto_No][1]<<16)+(moto_par.min_speed[Moto_No][2]<<8)+moto_par.min_speed[Moto_No][3];
			moto_par.min_speed[Moto_No] = moto_par.max_speed[Moto_No]>=moto_par.min_speed[Moto_No]?moto_par.min_speed[Moto_No]:moto_par.max_speed[Moto_No];
			buf_data=Moto_Speed_Con(moto_par.min_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x0A;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;		
			buf[4]=buf_data>>8;
			buf[5]=buf_data;	  		 
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	 }
//	 if(Moto_Pflag & 0x0008)//���ٶ�
//	 {  
//			acc=(moto_par.acc[Moto_No][0]<<24)+(moto_par.acc[Moto_No][1]<<16)+(moto_par.acc[Moto_No][2]<<8)+moto_par.acc[Moto_No][3];
//			buf_data=Moto_Acc_Con(acc,max_speed,min_speed);
//			if((Moto_No&0x01)==0)
//				buf[0]=0x00;
//			else if((Moto_No&0x01)==1)
//				buf[0]=0x02; 
//			buf[1]=0x04;
//			buf[2]=buf_data>>24;
//			buf[3]=buf_data>>16;		
//			buf[4]=buf_data>>8;
//			buf[5]=buf_data;			
//			 Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//	 }
//	 if(Moto_Pflag & 0x0010)//���ٶ�
//	 {
//			dec=(moto_par.dec[Moto_No][0]<<24)+(moto_par.dec[Moto_No][1]<<16)+(moto_par.dec[Moto_No][2]<<8)+moto_par.dec[Moto_No][3];
//      buf_data=Moto_Dec_Con(dec,max_speed,min_speed);

//			if((Moto_No&0x01)==0)
//				buf[0]=0x00;
//			else if((Moto_No&0x01)==1)
//				buf[0]=0x02; 
//			buf[1]=0x05;
//			buf[2]=buf_data>>24;
//			buf[3]=buf_data>>16;		
//			buf[4]=buf_data>>8;
//			buf[5]=buf_data;			
//			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//	 }
	 if(Moto_Pflag & 0x0100)//�˶�����
	 {
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x0B;
			buf[2]=0;
			buf[3]=0;			
			buf[4]=0;
			buf[5]=moto_par.pstv_dir[Moto_No];			
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//			buf_data = Read_Fpga_Regist(0x800B+0x0200*(Moto_No&0x01),Moto_No>>1);
//			if(buf_data!=moto_par.pstv_dir[Moto_No])
//			{
//				Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//			}
			
	 }
	 if(Moto_Pflag & 0x0800)//�����ٶ�
	 {
	
			//zero_speed=(moto_par.zero_speed[Moto_No][0]<<24)+(moto_par.zero_speed[Moto_No][1]<<16)+(moto_par.zero_speed[Moto_No][2]<<8)+moto_par.zero_speed[Moto_No][3];
      buf_data=Moto_Speed_Con(moto_par.zero_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x12;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;		
			buf[4]=buf_data>>8;
			buf[5]=buf_data;			
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	 }
	 if(Moto_Pflag & 0x1000)//�㶯�����ٶ�
	 {
//			jog_run_speed=(moto_par.jog_run_speed[Moto_No][0]<<24)+(moto_par.jog_run_speed[Moto_No][1]<<16)+(moto_par.jog_run_speed[Moto_No][2]<<8)+moto_par.jog_run_speed[Moto_No][3];
			buf_data=Moto_Speed_Con(moto_par.jog_run_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x11;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	 }
	 if(Moto_Pflag & 0x00010000)//Ŀ�������ٶ�
	 {

			//target_speed=(moto_par.target_speed[Moto_No][0]<<24)+(moto_par.target_speed[Moto_No][1]<<16)+(moto_par.target_speed[Moto_No][2]<<8)+moto_par.target_speed[Moto_No][3];
			buf_data=Moto_Speed_Con(moto_par.target_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x06;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);								
	 }	 	 
	 if(Moto_Pflag & 0x00020000)//�����ٶ�
	 {

	 }	
	 if(Moto_Pflag & 0x00040000)//��С���ٶ�
	 {

	 }		 
	 if(Moto_Pflag & 0x0008)//���ٶ�
	 {  
			//acc=(moto_par.acc[Moto_No][0]<<24)+(moto_par.acc[Moto_No][1]<<16)+(moto_par.acc[Moto_No][2]<<8)+moto_par.acc[Moto_No][3];
//			buf_data=Moto_Acc_Con(acc,max_speed,min_speed);
			buf_data=Moto_Acc_Con(moto_par.acc[Moto_No],moto_par.target_speed[Moto_No],moto_par.min_speed[Moto_No]);
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x04;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			 Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);

	 }
	 if(Moto_Pflag & 0x0010)//���ٶ�
	 {

		//	dec=(moto_par.dec[Moto_No][0]<<24)+(moto_par.dec[Moto_No][1]<<16)+(moto_par.dec[Moto_No][2]<<8)+moto_par.dec[Moto_No][3];
//			buf_data=Moto_Dec_Con(dec,max_speed,min_speed);
			buf_data=Moto_Acc_Con(moto_par.dec[Moto_No],moto_par.target_speed[Moto_No],moto_par.min_speed[Moto_No]);			
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x05;
			buf[2]=buf_data>>24;
			buf[3]=buf_data>>16;			
			buf[4]=buf_data>>8;
			buf[5]=buf_data;
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);

	 }	 
	 if(Moto_Pflag & 0x00080000)//����ģʽ
	 {
			 
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x32;
			buf[2]=0x00;
			buf[3]=0x00;			
			buf[4]=0x00;
			buf[5]=moto_par.zero_mode[Moto_No];	  
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		 
	 }	  	 
	 if(Moto_Pflag & 0x00100000)//�˶�ģʽ
	 {
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x36;
			buf[2]=0x00;
			buf[3]=0x00;			
			buf[4]=0x00;
			buf[5]=moto_par.run_mode[Moto_No];	  
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		
	 }	  	 	 
	 if(Moto_Pflag & 0x00200000)//����������
	 {

	 }	
	 if(Moto_Pflag & 0x00400000)//��϶����
	 {

	 }	  	
	 if(Moto_Pflag & 0x00800000)//CW/CCW��˫����ģʽ�л�
	 {
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x09;
			buf[2]=0x00;
			buf[3]=0x00;			
			buf[4]=0x00;
			buf[5]=(moto_par.pul_mode[Moto_No]<<1)&0x02;	  
		 Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	 }	 	
	  if(Moto_Pflag & 0x01000000)//�ٶ���������
	 {
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x37;
			buf[2]=0;
			buf[3]=0;			
			buf[4]=0;
			buf[5]=moto_par.speed_type[Moto_No]&0x01;	  
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	 }	 	 
		//**���ٲ���***//
//	buf_data=Get_Dstep(max_speed,min_speed,dec);
	 buf_data=Get_Dstep(moto_par.target_speed[Moto_No],moto_par.min_speed[Moto_No],moto_par.dec[Moto_No]);
	if((Moto_No&0x01)==0)
		buf[0]=0x00;
	else if((Moto_No&0x01)==1)
		buf[0]=0x02; 
	buf[1]=0x08;
	buf[2]=buf_data>>24;
	buf[3]=buf_data>>16;			
	buf[4]=buf_data>>8;
	buf[5]=buf_data;	  
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	

//	OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
}	
//**************************************************************
//�����������
//Moto_No:����ţ�0-7��
//steps:������
//
//**************************************************************
u8 Moto_Solute_Run(u8 Moto_No,u32 steps)	
{
//	u16 FPGA_POSL;
//	u16 FPGA_POSH;
	u32 FPGA_POS;
	u8 dir;
	u8 buf[6]={0};
	u32 len;
	 Cs_FPGA(Moto_No/2);
	if(Moto_No%2)
		FPGA_POS=Read_Fpga_Regist(0x8F04,Moto_No>>1);
	else
		FPGA_POS=Read_Fpga_Regist(0x8F03,Moto_No>>1);
	gmoto_pos_start[Moto_No]=FPGA_POS;
	dir = (signed)steps > (signed)FPGA_POS? 1 : 0;
//	if(0 != steps)
//	{
		if(dir==1) 
		{//�趨Ҫ�ߵĲ���
			len	= steps - FPGA_POS;
	  }else 
		    {
			    len = FPGA_POS - steps;
		    }
//		if(moto_par.pstv_dir[Moto_No]==1) {//����û��趨�˷�ת����
//				dir = dir? 0 :1;
//			}		
			 dir = dir?moto_par.pstv_dir[Moto_No]:!moto_par.pstv_dir[Moto_No];
		if(0 != len) 
		{
			moto_sta.runtime[Moto_No]=MotoCountdown(&moto_par,Moto_No ,len);	
			MOTO_ACC_DEC(Moto_No,len,& moto_par); 
			/*�����ƶ�*/
			//�ƶ�����
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x03;
			buf[2]=0;
			buf[3]=0;			
			buf[4]=0;
			buf[5]=dir;	 			
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
//			����
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x07;
			buf[2]=len>>24;
			buf[3]=len>>16;			
			buf[4]=len>>8;
			buf[5]=len;	 		
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	

			//������
			//len=len*1024/((moto_par.div_num[Moto_No][0]<<8)+moto_par.div_num[Moto_No][1]);
			len = (CDV_INT32U)((CDV_INT64U)len*(CDV_INT64U)moto_par.encoder_pre[Moto_No]/(CDV_INT64U)moto_par.div_num[Moto_No]);
			len = len==0?1:len;
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x0F;
			buf[2]=len>>24;
			buf[3]=len>>16;			
			buf[4]=len>>8;
			buf[5]=len;	 	
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);

			/*start*/			

			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x00;
			buf[2]=0x00;
			buf[3]=0x00;			
			buf[4]=0x01;
			buf[5]=0x01;	 		
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
		}
//  }else 
//		   {
//		     /*����*/

//				if((Moto_No&0x01)==0)
//					buf[0]=0x00;
//				else if((Moto_No&0x01)==1)
//					buf[0]=0x02; 
//				buf[1]=0x02;
//				buf[2]=0x00;
//				buf[3]=0x00;			
//				buf[4]=0x02;
//				if(moto_par.zero_dir[Moto_No]==0)
//					buf[5] = 0x02;
//				else
//					buf[5] = 0x82;					
//			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//		     gmoto_pos[Moto_No]=0;
//	     }
	return 1;
}
//************************************************
//�����ǰ/����˶�
//Moto_No������ţ�0-7��
//num����ǰ����
//type��1��ǰ��0���
//************************************************
RET_STATUS Motor_Relative_Run(CDV_INT08U Moto_No , CDV_INT32S num,u8 type) 
{	
#ifndef	CDV_V1
	u8 dir=type? 1:0;
//	u8 buf[6]={0};
	u8 buf[6]={1,1,1,1,1,1};	
	u32 len;
//	OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	Cs_FPGA(Moto_No/2);

	len = num;	
	if(len!=0)
	{	
//		if(moto_par.pstv_dir[Moto_No]==1)//����û��趨�˷�ת����
//		{
//			dir = dir? 0 :1;
//		}
		 dir = dir?moto_par.pstv_dir[Moto_No]:!moto_par.pstv_dir[Moto_No];
		if(type==1)
		{
			if((Moto_No&0x01)==0)
			{	

//			  buf[0]=0x00;				
				if(dir !=line_par.x_pos_dir[Moto_No/2][0])
				{	
			    line_par.x_pos_dir[Moto_No/2][0]=dir;
					SPI_Flash_Write((CDV_INT08U *)&line_par, MOTO_LINE_PAR_ADDR, sizeof(line_par));
				}	
			}	
		  else if((Moto_No&0x01)==1)
			{	
//			buf[0]=0x02;
				if(dir !=line_par.y_pos_dir[Moto_No/2][0])
				{	
			    line_par.y_pos_dir[Moto_No/2][0]=dir;
					SPI_Flash_Write((CDV_INT08U *)&line_par, MOTO_LINE_PAR_ADDR, sizeof(line_par));
				}
			}	
			
		}
		/*����ƶ�*/
		MOTO_ACC_DEC(Moto_No,len,& moto_par);

		//�ƶ�����

		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)	
			buf[0]=0x02;			
		buf[1]=0x03;
		buf[2]=0;
		buf[3]=0;
		buf[4]=0;
		buf[5]=dir;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
		
//		����
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)	
			buf[0]=0x02;			
		buf[1]=0x07;
		buf[2]=len>>24;
		buf[3]=len>>16;
		buf[4]=len>>8;
		buf[5]=len;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
		
		//������
		//len=len*1024/((moto_par.div_num[Moto_No][0]<<8)+moto_par.div_num[Moto_No][1]);
		len = (CDV_INT32U)((CDV_INT64U)len*(CDV_INT64U)moto_par.encoder_pre[Moto_No]/(CDV_INT64U)moto_par.div_num[Moto_No]);
		len = len==0?1:len;
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)	
			buf[0]=0x02;			
		buf[1]=0x0F;
		buf[2]=len>>24;
		buf[3]=len>>16;
		buf[4]=len>>8;
		buf[5]=len;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
		/*start*/

		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1]=0x00;
		buf[2]=0x00;
		buf[3]=0x00;			
		buf[4]=0x01;
		buf[5]=0x01;	
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	}	else
	  {
	    MotorStop(Moto_No);
	  }			
//	  OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
#endif
	 
	return OPT_SUCCESS;
}	
//************************************************
//�����ǰ/���㶯
//Moto_No������ţ�0-7��
//type��1��ǰ��0���
//state=1������state=0ֹͣ
//************************************************
RET_STATUS Motor_Jog_Run(CDV_INT08U Moto_No ,u8 type,u8 state) 
{	
#ifndef	CDV_V1
	u8 dir=type? 1:0;
	u8 buf[6]={0};
//	OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	Cs_FPGA(Moto_No/2);
//		if(moto_par.pstv_dir[Moto_No]==1)//����û��趨�˷�ת����
//		{
//			dir = dir? 0 :1;
//		}
		 dir = dir?moto_par.pstv_dir[Moto_No]:!moto_par.pstv_dir[Moto_No];
		//�ƶ�����
//		if((Moto_No&0x01)==0)
//			buf[0]=0x04;
//		else if((Moto_No&0x01)==1)
//			buf[0]=0x18; 
//		buf[1]=0;
//		buf[2]=dir;
//		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)	
			buf[0]=0x02;			
		buf[1]=0x03;
		buf[2]=0;
		buf[3]=0;
		buf[4]=0;
		buf[5]=dir;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		
		
		/*start*/

		if((Moto_No&0x01)==0)
		{	
			buf[0]=0x00;
		}	
		else if((Moto_No&0x01)==1)
		{	
			buf[0]=0x02;
		}	
		buf[1] = 0x0E;
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
		if(state==1)
		{	
		   buf[5] = 0x01;

		}	
		else if(state==0)
		{	
			buf[5] = 0x00;			
		}	
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		
		if(state==0||gmoto_flag[Moto_No]==1)//ֹͣ�㶯������һ����֮ͣ��û��������ȡ��ǰλ�ã����µ�ǰλ����Ŀ��λ��  
		{			
			if(Moto_No%2)
				gmoto_pos[Moto_No]=Read_Fpga_Regist(0x8F04,Moto_No>>1);
			else
				gmoto_pos[Moto_No]=Read_Fpga_Regist(0x8F03,Moto_No>>1);	
			moto_sta.target_pos[Moto_No] = gmoto_pos[Moto_No];
		}
		if(state==1)
			moto_sta.ela_runtime[Moto_No] = GetCdvTimeTick();
//	  OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
#endif
	return OPT_SUCCESS;
}
//*****************************************************************
//����
//Moto_No:�����
//
//*****************************************************************
RET_STATUS Motor_Run_Zero(CDV_INT08U Moto_No) 
{
	 u8 buf[6]={0};	
 // u16 FPGA_DATA;
  
//OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	 Cs_FPGA(Moto_No/2);
//	 if((Moto_No&0x01)==0)
//	   buf[0]=0x03;
//   else if((Moto_No&0x01)==1)
//	   buf[0]=0x17;
//   if(moto_par.zero_dir[Moto_No][0]==0)
//  	   buf[2] = 0x02;
//   else
//	   buf[2] = 0x82;	 
//	buf[1] = 02;
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02;
		buf[1]=0x02;		
		buf[2]=0x00;	
		buf[3]=0x00;	
		buf[4] =0x00;
		buf[5] = 0x02;
		if(moto_par.zero_dir[Moto_No]==0)
			buf[5] = 0x02;
		else
			buf[5] = 0x82;	 
			 
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//  
//   FPGA_DATA=Read_Fpga_Regist(buf[0]+0x80,(Moto_No>>1)+1);
//   while(FPGA_DATA!=((buf[1]<<8)+buf[2]))
//   {
//	   err_times++;
//	   Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//	   while(err_times>20);//
//   }
   
//	 OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
	return OPT_SUCCESS; 
}	
//*****************************************
//�������ٲ�������
//Moto_No:�����0-7
//����ֵ���ɹ���1��ʧ�ܣ�0
//****************************************
u8 Moto_MultSpeen_Config(u8 Moto_No,u32 Moto_Pflag,u8* rxBuf,u8 type)
{	
	#if 0
	u8 buf[6],j,i=0;
	u32 buf_data;
	u32 max_speed;
	u32 min_speed;
	u8 m_speed_num;
	m_speed_num = moto_par.mult_speed_num[Moto_No];
	if((Moto_No&0x01)==0)
		buf[0]=0x00;
	else if((Moto_No&0x01)==1)
		buf[0]=0x02; 
	buf[1]=0x14;
	buf[2]=0x00;
	buf[3]=0x00;			
	buf[4]=0x00;
	buf[5]=m_speed_num;		 
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	for(j=0;j<m_speed_num;j++)
	{	
		buf_data=Moto_Speed_Con(moto_par.mult_speed[Moto_No][j]);//�ٶ�ת��
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1]=0x16+j*3;
		buf[2]=buf_data>>24;
		buf[3]=buf_data>>16;			
		buf[4]=buf_data>>8;
		buf[5]=buf_data;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		 
		if(j==0)
		{				
			min_speed=moto_par.min_speed[Moto_No];//(moto_par.min_speed[Moto_No][0]<<24)+(moto_par.min_speed[Moto_No][1]<<16)+(moto_par.min_speed[Moto_No][2]<<8)+moto_par.min_speed[Moto_No][3];
			buf_data=Moto_Acc_Con(moto_par.mult_a_d_speed[Moto_No][0],moto_par.mult_speed[Moto_No][0],min_speed);//��һ�μ��ٶ�ת��
		}
		else
		{
			buf_data=Moto_Acc_Con(moto_par.mult_a_d_speed[Moto_No][j],moto_par.mult_speed[Moto_No][j],moto_par.mult_speed[Moto_No][j-1]);//��j�μӼ��ٶ�ת�� 
		}
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1]=0x15+j*3;
		buf[2]=buf_data>>24;
		buf[3]=buf_data>>16;			
		buf[4]=buf_data>>8;
		buf[5]=buf_data;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		

		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1]=0x17+j*3;
		buf[2]=moto_par.mult_step[Moto_No][j]>>24;
		buf[3]=moto_par.mult_step[Moto_No][j]>>16;			
		buf[4]=moto_par.mult_step[Moto_No][j]>>8;
		buf[5]=moto_par.mult_step[Moto_No][j];
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);			

	}	 
	 
	return 1;
	#else 
	u8 buf[6],j,i=0;
	u32 buf_data;
	u32 max_speed;
	u32 min_speed;
	u8 m_speed_num;
	m_speed_num = moto_par.mult_speed_num[Moto_No];
	if((Moto_No&0x01)==0)
		buf[0]=0x00;
	else if((Moto_No&0x01)==1)
		buf[0]=0x02; 
	buf[1]=0x14;
	buf[2]=0x00;
	buf[3]=0x00;			
	buf[4]=0x00;
	buf[5]=(m_speed_num-1);		 
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	
	moto_par.mult_step[Moto_No][m_speed_num-1] = UartToU32(rxBuf+i,4,LITTLEEND);
	if(m_speed_num>1)
		moto_par.mult_step[Moto_No][m_speed_num-1] +=moto_par.mult_step[Moto_No][m_speed_num-2];
	i+=4;
	if((Moto_No&0x01)==0)
		buf[0]=0x00;
	else if((Moto_No&0x01)==1)
		buf[0]=0x02; 
	buf[1]=0x17+(m_speed_num-1)*3;
	buf[2]=moto_par.mult_step[Moto_No][m_speed_num-1]>>24;
	buf[3]=moto_par.mult_step[Moto_No][m_speed_num-1]>>16;			
	buf[4]=moto_par.mult_step[Moto_No][m_speed_num-1]>>8;
	buf[5]=moto_par.mult_step[Moto_No][m_speed_num-1];
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
	
	if(type == 0xFD)i++;
	moto_par.mult_speed[Moto_No][(m_speed_num-1)] = UartToU32(rxBuf+i,4,LITTLEEND);
	i+=4;
	buf_data=Moto_Speed_Con(moto_par.mult_speed[Moto_No][(m_speed_num-1)]);//�ٶ�ת��
	if((Moto_No&0x01)==0)
		buf[0]=0x00;
	else if((Moto_No&0x01)==1)
		buf[0]=0x02; 
	buf[1]=0x16+(m_speed_num-1)*3;
	buf[2]=buf_data>>24;
	buf[3]=buf_data>>16;			
	buf[4]=buf_data>>8;
	buf[5]=buf_data;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
	
	

	if(Moto_Pflag & 0x0008)//���ٶ�
	{
		if(type == 0xFD)i++;
		moto_par.mult_a_d_speed[Moto_No][(m_speed_num-1)] = UartToU32(rxBuf+i,4,LITTLEEND);
		ParRangeCtrl(&moto_par.mult_a_d_speed[Moto_No][(m_speed_num-1)],moto_par.max_acc[Moto_No],moto_par.min_acc[Moto_No]); 
		moto_par.acc[Moto_No] = moto_par.mult_a_d_speed[Moto_No][(m_speed_num-1)];
		buf_data=Moto_Acc_Con(moto_par.mult_a_d_speed[Moto_No][(m_speed_num-1)],NULL,NULL);
		i=i+4;
	}
	else
		buf_data=Moto_Acc_Con(moto_par.acc[Moto_No],NULL,NULL);

	if((Moto_No&0x01)==0)
	buf[0]=0x00;
	else if((Moto_No&0x01)==1)
	buf[0]=0x02; 
	buf[1]=0x15+(m_speed_num-1)*3;
	buf[2]=buf_data>>24;
	buf[3]=buf_data>>16;			
	buf[4]=buf_data>>8;
	buf[5]=buf_data;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	
	if(Moto_Pflag & 0x0010)//���ٶ�
	{
		moto_par.dec[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
		ParRangeCtrl(&moto_par.dec[Moto_No],moto_par.max_acc[Moto_No],moto_par.min_acc[Moto_No]); 
		buf_data=Moto_Acc_Con(moto_par.dec[Moto_No],NULL,NULL);			
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1]=0x05;
		buf[2]=buf_data>>24;
		buf[3]=buf_data>>16;			
		buf[4]=buf_data>>8;
		buf[5]=buf_data;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	}

   buf_data=Get_Dstep(moto_par.mult_speed[Moto_No][m_speed_num-1],moto_par.min_speed[Moto_No],moto_par.dec[Moto_No]);//���ٲ���
	if(moto_par.run_mode[Moto_No]==1)//�ջ�
	{
		buf_data = (CDV_INT32U)((CDV_INT64U)buf_data*(CDV_INT64U)moto_par.encoder_pre[Moto_No]/(CDV_INT64U)moto_par.div_num[Moto_No]);
	}	
	if((Moto_No&0x01)==0)
		buf[0]=0x00;
	else if((Moto_No&0x01)==1)
		buf[0]=0x02; 
	buf[1]=0x08;
	buf[2]=buf_data>>24;
	buf[3]=buf_data>>16;			
	buf[4]=buf_data>>8;
	buf[5]=buf_data;	  
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	return 1;
	
	#endif
}
//*****************************************
//�������ٲ�������
//no:�����0-7
//����ֵ���ɹ���1��ʧ�ܣ�0
//****************************************
u8 Moto_MultSpeen_Config_Continue(u8 Moto_No,u8 Mult_Num_Now,u32 StepsHaveRun)
{	
	u8 buf[6],j;
	u32 buf_data;
	u32 max_speed;
	u32 min_speed;
	u8 m_speed_num;
	m_speed_num = moto_par.mult_speed_num[Moto_No];
	if((Moto_No&0x01)==0)
		buf[0]=0x00;
	else if((Moto_No&0x01)==1)
		buf[0]=0x02; 
	buf[1]=0x14;
	buf[2]=0x00;
	buf[3]=0x00;			
	buf[4]=0x00;
	buf[5]=m_speed_num-Mult_Num_Now+1;		 
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		
	for(j=Mult_Num_Now-1;j<m_speed_num-Mult_Num_Now+1;j++)
	{	
		buf_data=Moto_Speed_Con(moto_par.mult_speed[Moto_No][j]);//�ٶ�ת��
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1]=0x16+j*3;
		buf[2]=buf_data>>24;
		buf[3]=buf_data>>16;			
		buf[4]=buf_data>>8;
		buf[5]=buf_data;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);			 
		if(j==Mult_Num_Now-1)
		{				
			min_speed=moto_par.min_speed[Moto_No];//(moto_par.min_speed[Moto_No][0]<<24)+(moto_par.min_speed[Moto_No][1]<<16)+(moto_par.min_speed[Moto_No][2]<<8)+moto_par.min_speed[Moto_No][3];
			buf_data=Moto_Acc_Con(moto_par.mult_a_d_speed[Moto_No][0],moto_par.mult_speed[Moto_No][0],min_speed);//��һ�μ��ٶ�ת��
		}
		else
		{
			buf_data=Moto_Acc_Con(moto_par.mult_a_d_speed[Moto_No][j],moto_par.mult_speed[Moto_No][j],moto_par.mult_speed[Moto_No][j-1]);//��j�μӼ��ٶ�ת�� 
		}
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1]=0x15+j*3;
		buf[2]=buf_data>>24;
		buf[3]=buf_data>>16;			
		buf[4]=buf_data>>8;
		buf[5]=buf_data;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		

		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1]=0x17+j*3;
		buf[2]=(moto_par.mult_step[Moto_No][j]-StepsHaveRun)>>24;
		buf[3]=(moto_par.mult_step[Moto_No][j]-StepsHaveRun)>>16;			
		buf[4]=(moto_par.mult_step[Moto_No][j]-StepsHaveRun)>>8;
		buf[5]=(moto_par.mult_step[Moto_No][j]-StepsHaveRun);
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);			

	}	 
	 
	return 1;	
}
//************************************************
//�����ǰ/��������˶�
//Moto_No������ţ�0-7��
//num����ǰ����
//type��1��ǰ��0���
//************************************************
RET_STATUS Motor_MultSpeen_Run(CDV_INT08U Moto_No , CDV_INT32S num,u8 type) 
{	
#ifndef	CDV_V1
	u8 dir=type? 1:0;
	u8 buf[6]={0};
	u32 len;
	Cs_FPGA(Moto_No/2);
	len = num;
	if(len!=0)
	{	
			dir = dir?moto_par.pstv_dir[Moto_No]:!moto_par.pstv_dir[Moto_No];
			MOTO_ACC_DEC(Moto_No,len,& moto_par); 
			/*�����ƶ�*/
			//�ƶ�����
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x03;
			buf[2]=0;
			buf[3]=0;			
			buf[4]=0;
			buf[5]=dir;	  		
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
			//����
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x07;
			buf[2]=len>>24;
			buf[3]=len>>16;			
			buf[4]=len>>8;
			buf[5]=len;	 		
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	

			//������		
			len = (CDV_INT32U)((CDV_INT64U)len*(CDV_INT64U)moto_par.encoder_pre[Moto_No]/(CDV_INT64U)moto_par.div_num[Moto_No]);
			len = len==0?1:len;
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x0F;
			buf[2]=len>>24;
			buf[3]=len>>16;			
			buf[4]=len>>8;
			buf[5]=len;	 	
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);


			/*start*/
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02; 
			buf[1]=0x13;
			buf[2]=0;
			buf[3]=0;			
			buf[4]=0;
			buf[5]=1;	 		
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
		}	
		else
	  {
	    MotorStop(Moto_No);
	  }			
#endif
	return OPT_SUCCESS;
}	
//************************************************
//������˶���������
//Moto_No������ţ�0-7��
//************************************************
RET_STATUS Motor_MultSpeen_Continue(CDV_INT08U Moto_No) 
{		
	u32 pos_now; 
	u8 rdir;
	u32 RemainingSteps;//ʣ�ಽ��
	u32 StepsHaveRun;//�����в���
	u8 Mult_Num_Now;//��ǰ����
	u8 i;
	Read_Moto_Par(Moto_No,0x1000);	
	pos_now=(Para_Buf[0]<<24)|(Para_Buf[1]<<16)|(Para_Buf[2]<<8)|Para_Buf[3];											
	RemainingSteps = abs(gmoto_pos[Moto_No] -pos_now);
	if(RemainingSteps)
	{
		rdir= gmoto_pos[Moto_No]>pos_now?1:0;
		StepsHaveRun = abs(pos_now - gmoto_pos_start[Moto_No]);
//		gmoto_pos_start[Moto_No]
//		moto_par.mult_step[Moto_No][j];
		for(i=0;i<moto_par.mult_speed_num[Moto_No];i++)
		{
			if(StepsHaveRun<moto_par.mult_step[Moto_No][i])
			{
				Mult_Num_Now=i+1;
				break;
			}
		}
		Moto_MultSpeen_Config_Continue(Moto_No,Mult_Num_Now,StepsHaveRun);
		Motor_MultSpeen_Run(Moto_No ,RemainingSteps,rdir);
	}
	return OPT_SUCCESS;
}
//**************************************************
//���ʹ��/ʧ��
//Moto_No:�����
//typ:1ʹ�ܣ�0ʧ��
//**************************************************
void MOTO_Enable_Disable(u8 Moto_No,u8 typ)
{
	u8 buf[6];
//	OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	Cs_FPGA(Moto_No/2);  
	if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1] = 0x09;
		buf[2] = 0x00;			
		buf[3] = 0x00;
		buf[4] = 0x01;
		buf[5] = typ==1?0x01:0x00;		
		moto_sta.enable[Moto_No] = buf[5];
		buf[5] |= moto_par.pul_mode[Moto_No]<<1;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);  
//  OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
}
//************************************************
//�����ͣ
//Moto_No������ţ�0-7��
//************************************************
RET_STATUS Motor_Stop(CDV_INT08U Moto_No) 
{	
#ifndef	CDV_V1
	u8 buf[6]={0};
//	OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	Cs_FPGA(Moto_No/2);

		//��ͣ����
//		if((Moto_No&0x01)==0)
//			buf[0]=0x02;
//		else if((Moto_No&0x01)==1)
//			buf[0]=0x16;
//		buf[1] = 0;
//		buf[2] = 0x03;
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1] = 0x01;
		buf[2] = 0x00;			
		buf[3] = 0x00;
		buf[4] = 0x00;
		buf[5] = 0x03;		
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);		
//	  OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
#endif
	return OPT_SUCCESS;
}
//**************************************************************************
//��������ٶ�ת��
//
//**************************************************************************
float Moto_Speed_Con(u32 user_data)
{
	 float FPGA_Data;
	
	#if 1
	FPGA_Data = Moto_Speed_Format(user_data)*FPGA_ACC_DIV;
	 #else
	  if(user_data<20)
		{
			user_data=20;
    }		
    else if(user_data>=FPGA_W_FRE_DIV2)
    {
       user_data=FPGA_W_FRE_DIV2;
    }			
	 FPGA_Data=FPGA_W_FRE_DIV2/user_data*FPGA_ACC_DIV;	 
	 #endif
	return FPGA_Data;
}
//**************************************************************************
//����ٶȸ�ʽ��
//���ٶȰ��ֱ���ת����FPGA�ܹ������Ƶ��
//**************************************************************************
u32 Moto_Speed_Format(u32 user_data)
{
	u32 FPGA_Data;

	FPGA_Data = FPGA_W_FRE_DIV2/user_data;
	FPGA_Data = FPGA_W_FRE_DIV2/FPGA_Data;
	return FPGA_Data;
}

//***************************************
//���ٶȵ�λת���û����뵥λΪ ����/s^2
//max:����ٶȣ���λ ����/s
//***************************************
u32 Moto_Acc_Con(u32 user_data,u32 max_speed,u32 start_speed)
{
	u32 FPGA_Data;

	if(max_speed <= 20)
	{	
		max_speed = 20;
	}	
	if(start_speed <= 20)
	{	
		start_speed = 20;
	}	
	#if 1

		FPGA_Data = user_data*FPGA_SPEED_CHANGE_CYCLE*FPGA_ACC_DIV;
	#else
		FPGA_Data = (CDV_INT32U)(FPGA_W_FRE_DIV2*FPGA_SPEED_CHANGE_CYCLE*user_data/((CDV_INT64U)start_speed*(CDV_INT64U)max_speed)*FPGA_ACC_DIV);
	#endif
	if(FPGA_Data<1)FPGA_Data=1;
	return FPGA_Data;
}
//***************************************
//���ٶȵ�λת���û����뵥λΪ ����/s^2
//max:����ٶȣ���λ ����/s
//***************************************
//u32 Moto_Dec_Con(u32 user_data,u32 max_speed,u32 min_speed)
//{

//	u32 FPGA_Data;
//	FPGA_Data = Moto_Acc_Con(user_data,max_speed,min_speed);
//	return FPGA_Data;
//}

//**************************************
//������ٲ���
//max:����ٶ�
//min����С�ٶ�
//d_speed:���ٶ�
//���ؼ��ٲ���
//*************************************
u32 Get_Dstep(u32 max_speed,u32 min_speed,u32 dec_speed)
{
	u32 i;
	u32 count;
	float dec_data;
	
	double b;	
	 double c= 0;	//2017-2-8 ��ӷ��޸�
	double a=min_speed;
	dec_data=(float)dec_speed;
	dec_data = dec_data==0?1:dec_data;
	#if 1

		count=(u32)(abs(max_speed-min_speed)/(dec_data*FPGA_SPEED_CHANGE_CYCLE));//�ٶȱ仯����
		if(count<1)count = 1;
		for(i=0;i<count;i++)
		{
			b=FPGA_SPEED_CHANGE_CYCLE*Moto_Speed_Format((u32)a);  
			a=a+dec_data*FPGA_SPEED_CHANGE_CYCLE; 
			c=c+b;
		}	
	#else
		if(max_speed>=min_speed)
		{	
			count=(u16)((FPGA_W_FRE_DIV2/min_speed-FPGA_W_FRE_DIV2/max_speed)/dec_data);//�ٶȱ仯����
		}
	  else
	  {
			count=(u16)((FPGA_W_FRE_DIV2/max_speed-FPGA_W_FRE_DIV2/min_speed)/dec_data);//�ٶȱ仯����
		}		
		for(i=0;i<count;i++)
		{
			b=FPGA_W_FRE_DIV2*FPGA_SPEED_CHANGE_CYCLE/a;  //5ms����һ���ٶȱ仯    b=(10^7/a)*5*10^(-3)
			a=a+dec_data; 
				
			c=c+b;
		}	
	#endif
	return (u32)c;
}

/**
  * @brief  �����ѯ
  *  
  * @param  Moto_No     Ҫ�����ĵ����0-7
	*         opt    0 �ȴ��������
	*                1 
	*                2  
  *   
  * @retval 
  *
  * @note   ��ѯ�����λ���ٶȣ����ٶȵ�
	*         ��modbus�Ĵ����ж�RW_MOTOR��maxSpeed��acc��dec��upLimit��downLimit��minSpeed��������״̬ʱ�趨�ã�
	*                           R_MOTOR_POS (�����Ҫά��ֵ���ɶ�д��ÿ�η����˶�����ɹ�����ĸ�ֵ)
	*         ���������FPGA
  */
RET_STATUS MotorChk(CDV_INT08U Moto_No , CDV_INT08U opt) 
{
//	CDV_INT08U val;
OS_ERR err;

	RET_STATUS ret =OPT_SUCCESS;
#ifndef	CDV_V1
//	if(1 != READ_CDV_DIP(Moto_No>>1))//�ж����õ��ǲ��ǵ��
//		return OPT_SUCCESS;
	
	switch (opt) {
		case 0:
			     /*�ȴ��������*/
		       //__nop();//__NOP;
					OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 5ms
//					mes_id = MotoMessageQuery(Moto_No);
//					if(mes_id)
//					{
//						mes_pos = Read_Fpga_Regist(0x8F03+Moto_No%2,Moto_No>>1);
//						MotoMessageClear(Moto_No);
//					}
			     if(FpgaMotoWait(Moto_No))
				  {
					  Moto_RunSta[Moto_No] = READY;
						wait_err=0;
			     } else {
					        wait_err++;
				            ret = OPT_FAILURE;
							if(wait_err>2000)
							{
/******ͨ�ô��ڲ��ԣ�ע��һ��****/								
//								for(i=0;i<6;i++)
//								{
//									MotorStop(i);
//								}
//								while(1)
//								{
//							       ;
//							    }	
							}	
//							OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 5ms
			            }
			     break;
	  default:
			      break;
	    }
#endif
	return ret;
}
/**
  * @brief  �����ͣ
  *  
  * @param  Moto_No     Ҫ�����ĵ����
  *         num    0    ����
  *                ���� �����˶�Ŀ��λ��(����)
  *   
  * @retval 
  *
  * @note   
  */
RET_STATUS MotorStop(CDV_INT08U Moto_No ) 
{	
#ifndef	CDV_V1
	u8 buf[6]={0};
//OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	Cs_FPGA(Moto_No/2);
//		//������λ
//		if((Moto_No&0x01)==0)
//			buf[0]=0x08;
//		else if((Moto_No&0x01)==1)
//			buf[0]=0x1c;
//		buf[1] = 0;
//		buf[2] =0;
//		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);

//		//������λ
//		if((Moto_No&0x01)==0)
//			buf[0]=0x09;
//		else if((Moto_No&0x01)==1)
//			buf[0]=0x1d;
//		buf[1] = 0;
//		buf[2] = 0;
//		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02;
		buf[1] =0x07;
		buf[2] =0;
		buf[3] =0;
		buf[4] =0;
		buf[5] =0;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
//	 Motor_Relative_Run(Moto_No ,0,0);
//OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
#endif
	return OPT_SUCCESS;
}
//*****************************************
//
//
//
//*****************************************
u8 MOTO_ACC_DEC(u8 Moto_No,u32 run_step,MOTOR_INFO *moto_pared)
{
	 u32 buf_data;
	 u8 buf[6]={0};
	 u32 max_speed;
	 u32 min_speed;
	 u32 acc;
	 u32 dec;
	 u32 DecStep;
	 u32 AccStep;
	 u8 ret = 0;
	 if(moto_par.speed_type[Moto_No] == MULT_SPEEND_MODE)
	 {
			max_speed=moto_par.mult_speed[Moto_No][moto_par.mult_speed_num[Moto_No]-1];
	 }
	 else
	 {
			max_speed=moto_par.target_speed[Moto_No];
	 }
	min_speed=moto_par.min_speed[Moto_No];
   acc=moto_par.acc[Moto_No];
   dec=moto_par.dec[Moto_No];
   if(gmoto_flag2==0)bufmax_speed=max_speed;
   AccStep=Get_Dstep(max_speed,min_speed,acc);//�õ�������Ҫ�Ĳ���
	 if(AccStep<run_step)//������ٲ���С�����г�
	 { 
      DecStep=Get_Dstep(max_speed,min_speed,dec);//�õ�������Ҫ�Ĳ���
		  if((AccStep+DecStep)<run_step)//������ٲ���+���ٲ���С���ܲ���
			{	
				  if(gmoto_flag2==1)//����ٶȡ����ٶȡ����ٶ��޸Ĺ�����Ҫ�Ļ���
				  {
						  //���ٲ���
							DecStep=Get_Dstep(max_speed,min_speed,dec);
							if(moto_par.run_mode[Moto_No]==1)//�ջ�
							{
								DecStep = (CDV_INT32U)((CDV_INT64U)buf_data*(CDV_INT64U)moto_par.encoder_pre[Moto_No]/(CDV_INT64U)moto_par.div_num[Moto_No]);
							}
							if((Moto_No&0x01)==0)
							{	
								buf[0]=0x00;
							}	
							else if((Moto_No&0x01)==1)
							{	
								buf[0]=0x02; 
							}	
							buf[1]=0x08;
							buf[2]=DecStep>>24;
							buf[3]=DecStep>>16;
							buf[4]=DecStep>>8;
							buf[5]=DecStep;								
							Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN); 
							gmoto_flag2=0;							
					}	
				  ret = 0;//���ؼ��ٲ��� 
			}	
			else//������ٲ���+���ٲ��������ܲ���
			{	
				 if(gmoto_flag2==0)bufmax_speed=max_speed;				
				// DecStep=run_step/2; //���ٲ���=1/2���г�
					DecStep=(CDV_INT64U)run_step*acc/(dec+acc);
					//���ͼ��ٲ���
					if(moto_par.run_mode[Moto_No]==1)//�ջ�
					{	
						DecStep = (CDV_INT32U)((CDV_INT64U)DecStep*(CDV_INT64U)moto_par.encoder_pre[Moto_No]/(CDV_INT64U)moto_par.div_num[Moto_No]);	
					}
					if((Moto_No&0x01)==0)
						buf[0]=0x00;
					else if((Moto_No&0x01)==1)
						buf[0]=0x02;
					buf[1]=0x08;
					buf[2]=DecStep>>24;
					buf[3]=DecStep>>16;					
					buf[4]=DecStep>>8;
					buf[5]=DecStep;					
					Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//					max_speed = MaxSpeed_Real(DecStep,Moto_Speed_Con(min_speed),Moto_Acc_Con(dec,1,1));
					gmoto_flag2=1;
					ret = 1;
			}		
   }	
   else//������ٲ����������г�
   {
		  if(gmoto_flag2==0)bufmax_speed=max_speed;
		 
			//DecStep=run_step/2;//���ٲ���=1/2���г�
			DecStep=(CDV_INT64U)run_step*acc/(dec+acc);
			//���ͼ��ٲ���
			if(moto_par.run_mode[Moto_No]==1)//�ջ�
			{
				//DecStep = DecStep*UartToU32(moto_par.encoder_pre[Moto_No],4,LITTLEEND)/UartToU32(moto_par.div_num[Moto_No],2,BIGENDIAN);
				DecStep = (CDV_INT32U)((CDV_INT64U)DecStep*(CDV_INT64U)moto_par.encoder_pre[Moto_No]/(CDV_INT64U)moto_par.div_num[Moto_No]);
			}
			if((Moto_No&0x01)==0)
				buf[0]=0x00;
			else if((Moto_No&0x01)==1)
				buf[0]=0x02;
			buf[1]=0x08;
			buf[2]=DecStep>>24;
			buf[3]=DecStep>>16;					
			buf[4]=DecStep>>8;
			buf[5]=DecStep;				
			Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
//			max_speed = MaxSpeed_Real(DecStep,Moto_Speed_Con(min_speed),Moto_Acc_Con(dec,1,1));
			gmoto_flag2=1;
			ret = 2;
   }		
	moto_par.DecStep[Moto_No] = DecStep;
	return ret;
//	if(Moto_RunSta[Moto_No]==READY)
//	{
//		Moto_RunSta[Moto_No] = ACC;
//		gDec_Times[Moto_No] = (max_speed-min_speed)/dec;
//		gAcc_Times[Moto_No] = (max_speed-min_speed)/acc;		
//	}
}	
//******************************************************
//�ȴ�moto�˶����
//MOTO_num:�ڼ��ŵ��1:X ��  2��Y��   3��Բ��
//FPGA_num:�ڼ���FPGA��1-4��
//******************************************************
u8 wait_moto_finish(u8 MOTO_num,u8 FPGA_NUM)
{
	 u8 moto_state=0;
	 if(MOTO_num==1)
	 {
			 while(1)
			 {
//					moto_state=(0x01 & Read_Fpga_Regist(0xd5,FPGA_NUM));
					moto_state=(0x01 & Read_Fpga_Regist(0x8F00,FPGA_NUM));
					if(moto_state)
						break;
			 }	
	 } 
	 else if(MOTO_num==2)
	 {
			while(1)
			 {
					moto_state=(0x02 & Read_Fpga_Regist(0x8F00,FPGA_NUM));
					if(moto_state)
						break;
			 }
	 }	
   else if(MOTO_num==3)
	 {
			while(1)
			 {
					moto_state=(0x04 & Read_Fpga_Regist(0x8F00,FPGA_NUM));
					if(moto_state)
						break;
			 }
	 }
	 return moto_state;
}	
/**
  * @brief  ȫ���������
  *  
  * @param  ctl    ���ƴ��ţ�0����ͣ��1������
  *   
  * @retval ����ֵ˵��
  *
  * @note   ��������
  */

void AllMotorCTL(CDV_INT08U ctl) {
	CDV_INT08U i = 0;
	static CDV_INT08U all_motor_ctl = 0;
	OS_ERR err;
	  
	  
//	if (0 == ctl)
//  	all_motor_ctl = 0;
	OSMutexPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	for(i = 0; i < 8 ; i++) {
		if(1 != READ_CDV_DIP(i>>1))//�ж����õ��ǲ��ǵ��
		  continue;
		
		if(OPT_SUCCESS == MotorChk(i , 0)) {
			if (1 == ctl && 1 == BIT_READ(all_motor_ctl , i)) {
				//all_motor_ctl = 0;
			  BIT_RESET(all_motor_ctl , i);
			  Moto_Solute_Run(i,gmoto_pos[i]);//����
			}
		} else {
			if (0 == ctl && 0 == BIT_READ(all_motor_ctl , i)) {
				//all_motor_ctl = 1;
			  BIT_SET(all_motor_ctl , i);
			  Motor_Stop(i);//��ͣ
			}
		}
	}
	OSMutexPost (&MOTO_SEM,OS_OPT_POST_NO_SCHED,&err);
	if (1 == ctl)
  	all_motor_ctl = 0;
}

/**
  * @brief  ����ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��
            OPT_SUCCESS����ִ�����
            OPT_FAILURE����δִ����ɣ�����ȴ�δ�ȵ�
  *
  * @note   ��Դ�ţ�00-FF��	00�����˶�	    FF	uint32��ֵ��λ	uint32��ֵ	 uint32��ֵ	uint32��ֵ��λ			�ַ���
																				    FE	�������Ÿ�λ	  �������ŵ�λ							
														01�ȴ��˶����	ED									                                            �ַ���
  */

RET_STATUS MotorCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) 
{ 
	u8 i;
	//u16 FPGA_POSH;
	//u16 FPGA_POSL;
	u32 buf=0;
	u8 buff[3];
	CDV_INT32S up_limit,down_limit;
	CDV_INT08U Moto_No , type; //strLen;
	CDV_INT32U opt;
	CDV_INT32S num;
	u8 variate_num;//������
	u8 rdir=0;
	u32 pflag;
	u8 Para_len;

//	FPGA_MOTOR_INFO fpgaPar;
	RET_STATUS ret =OPT_SUCCESS;
	OS_ERR err;
	if(!CDV_FPGA_NUM)
		return ret;

	Moto_No  = UartToU32(&rxBuf[0],4,LITTLEEND);//rxBuf[0];
	opt  = rxBuf[4];//UartToU32(&rxBuf[1],4,LITTLEEND);
	type = rxBuf[5];
	up_limit =moto_par.up_limit[Moto_No]; //UartToU32(&moto_par.up_limit[Moto_No][0],4,BIGENDIAN);
   down_limit = -(CDV_INT64S)moto_par.down_limit[Moto_No];//UartToU32(&moto_par.down_limit[Moto_No][0],4,BIGENDIAN);
	if( (opt>=0x10)&&(opt<0x14) )//ֱ�߲岹
	{
		ret=Line_Interpolation(rxBuf,rxLen,arg);
	}	
	else if( (opt>=0x20)&&(opt<0x25) )//Բ���岹
	{
		ret=Arc_Interpolation(rxBuf,rxLen,arg);
	}	
	else if( (opt>=0x30)&&(opt<=0x35) )//��դ�߻������	
	{
		ret=GratingRuler_CmdParse(rxBuf,rxLen,arg);
	}
	else//����
	{	
		rxBuf = rxBuf+3;//������Դ����1byte��Ϊ4byte��Ϊ�˷����²��ִ��벻���޸� ��rxBufָ�������λ
		pflag = UartToU32(&rxBuf[3],4,LITTLEEND);
		switch(opt) {
			case 0x00://�Ƶ�
								if(Moto_RunSta[Moto_No]==READY)
									MotorMoveTo(rxBuf+2,rxLen,Moto_No);
								break;
			case 0x01://�ȴ��������	
								//delay_ms(100);
								ret = MotorChk(Moto_No , 0);
								break;
			case 0x02://����
								if(Moto_RunSta[Moto_No]==READY)
									MotorRtnZero(rxBuf+2,rxLen,Moto_No);
								break;
			case 0x03://��ǰ	
								if(Moto_RunSta[Moto_No]==READY)
									MotorFwd(rxBuf+2,rxLen,Moto_No);
								break;
			case 0x04://���
									if(Moto_RunSta[Moto_No]==READY)
									MotorRev(rxBuf+2,rxLen,Moto_No);					
							break;
			case 0x05://��ͣ/����
								if((rxBuf[3] & 0x01)==1)//��ͣ
								{
									ret=Motor_Stop(Moto_No);
									gmoto_flag[Moto_No]=1;
									gmoto_suspend_flag[Moto_No]=1;
									Moto_RunSta[Moto_No] = READY;
									gAccAndDec_Cnt[Moto_No] = 0;
								}	
								else if(gmoto_rzflag[Moto_No]==1)//��������������ͣ�ͼ�������
								{
									if(gmoto_suspend_flag[Moto_No]==1) 
									{	
										ret = Motor_Run_Zero(Moto_No);
										gmoto_pos[Moto_No]=0;
									}	
								}						
								else//��������
								{
									 if(gmoto_flag[Moto_No]==1)
									 {	   
										 if(moto_par.speed_type[Moto_No] == TRAP_MODE)
										 {
											 
											Moto_Solute_Run(Moto_No,gmoto_pos[Moto_No]);	
										 }
										 else
										 {
											Motor_MultSpeen_Continue(Moto_No);
										 }											 
										 gmoto_flag[Moto_No]=0;
									 }
								}								
								break;
			case 0x06://ʧ��/ʹ��
								MOTO_Enable_Disable(Moto_No,rxBuf[3]);
								break;
			case 0x07://�������� 
							 switch(type) 
							 {
								case 0xFF: 
								case 0x2F:
//										 if(pflag>0xFFFF)
											Moto_Config(Moto_No,pflag,rxBuf+7,type,1);
//										 else
//											Moto_Config(Moto_No,pflag,rxBuf+5,type);
										 break;
								case 0xFE:
								case 0x2E:
										 variate_num=rxBuf[5];
										 num = VarGet(variate_num);
										 *(rxBuf+5)=(num&0xff000000)>>24;
										 *(rxBuf+6)=(num&0x00ff0000)>>16;
										 *(rxBuf+7)=(num&0x0000ff00)>>8;
										 *(rxBuf+8)=(num&0x000000ff);
										 Moto_Config(Moto_No,pflag,rxBuf+7,type,1);
										 break;
//								case 0xFD:
//									
//										 break;
							 }   
							 break;
			case 0x08://������ȡ
							 switch(type) 
							 {
									case 0xFF: 
									case 0x2F:	  
														Para_len=Read_Moto_Par(Moto_No,pflag);
														ResRequest(arg->buf, arg->len, Para_Buf,Para_len,arg);
														break;
									case 0xFE:
									case 0x2E:
													Para_len=Read_Moto_Par(Moto_No,pflag);
													for(i=0;i<Para_len;i++)
													{
														buf=buf+(Para_Buf[i]<<(8*(Para_len-i-1)));//buf��4Byte��һ��ֻ��һ��������ÿ����������ȡ4���ֽ���ֵ
													}
													ValSet(rxBuf[5],buf);

													break; 
							 }								
							 break;
			case 0x09://�㶯
//							if((rxBuf[3]&0x01)==1)//�㶯��ǰ
//							 {	 									
									ret=Motor_Jog_Run(Moto_No ,rxBuf[3]&0x01,rxBuf[3]>>1);
									
//							 }	 
//							 else if((rxBuf[3]&0x01)==0)//�㶯���
//							 {	 
//									ret=Motor_Jog_Run(Moto_No ,0,rxBuf[3]>>1);
//									moto_sta.ela_runtime[Moto_No] = GetCdvTimeTick();
//								}								 
					     break;			
			case 0x0A://�������ǰ	
							if(Moto_RunSta[Moto_No]==READY)
								 MotorMultFwd(rxBuf+2,rxLen,Moto_No);							 
					     break;	
			case 0x0B://��������	
							if(Moto_RunSta[Moto_No]==READY)
								 MotorMultRev(rxBuf+2,rxLen,Moto_No);														 
					     break;									
		  default:
							break;
			
		}
	}	
	//OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
	if(OPT_FAILURE == ret) {
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 5ms
	}
	return ret;
}

//*****************************************************
//ֱ�߲岹
//
//
//*****************************************************
RET_STATUS Line_Interpolation(u8* rxBuf,u8 rxLen, CMD_ARG *arg)	
{
	u8 i=0;
	
	u32 val_buf=0;
	u8 buf[6]={0};
  u8 Para_len=0;
	RET_STATUS ret =OPT_SUCCESS;
	u8 Fpga_No;//������
	u8 Moto_Function;//������ܣ����ò�����д����������������
	u32 Moto_Pflag;//����������
	u8 type;
	
	//OS_ERR err;
	//OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	
	Fpga_No=UartToU32(&rxBuf[0],4,LITTLEEND)-8;//rxBuf[0]-8;
	Moto_Function=rxBuf[4];//rxBuf[1];
	type=rxBuf[5];//rxBuf[2];
	Moto_Pflag=UartToU32(&rxBuf[6],4,LITTLEEND);//(rxBuf[3]<<8)+rxBuf[4];
	rxBuf = rxBuf+3;
	Cs_FPGA(Fpga_No);
	switch(Moto_Function)
	{
		case 0x10: //�����������
	            Line_Config(Fpga_No ,Moto_Pflag ,rxBuf+5,0);
		          break;
		case 0x11://���������ȡ
			        switch(type) 
						  {
								case 0xFF: 
								case 0x2F:	  
													Para_len=Read_Line_Par(Fpga_No,Moto_Pflag);
													ResRequest(arg->buf, arg->len, Para_Buf,Para_len,arg);
													break;
								case 0xFE:
								case 0x2E:
												Para_len=Read_Line_Par(Fpga_No,Moto_Pflag);
												for(i=0;i<Para_len;i++)
												{
													val_buf=val_buf+Para_Buf[i]<<(8*(Para_len-i-1));//buf��4Byte��һ��ֻ��һ��������ÿ����������ȡ4���ֽ���ֵ
												}
												ValSet(rxBuf[5],val_buf);

												break; 
						  }								
			        break;
		case 0x12://����ƶ�
				
						//�յ�����
						buf[0]=0x04;
						buf[1]=0x01;						
						buf[2]=rxBuf[3];
						buf[3]=rxBuf[4];	
						buf[4]=rxBuf[5];
						buf[5]=rxBuf[6];
						Send_parameter(Fpga_No+1,buf,FPGA_COMMAND_LEN);//Xe		
						buf[0]=0x04;
						buf[1]=0x02;						
						buf[2]=rxBuf[7];
						buf[3]=rxBuf[8];	
						buf[4]=rxBuf[9];
						buf[5]=rxBuf[10];
						Send_parameter(Fpga_No+1,buf,FPGA_COMMAND_LEN);//Ye
						//��������
						buf[0]=0x04;
						buf[1]=0x00;
						buf[2]=0x00;
						buf[0]=0x00;
						buf[1]=0x00;
						buf[2]=0x51;						
						Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);						
			        break;
		case 0x13://����ȴ�
//			        FPGA_STATE=Read_Fpga_Regist(0xd5,Fpga_No+1);
					  FPGA_STATE=Read_Fpga_Regist(0x8F00,Fpga_No);
			        if(0x00000080 & FPGA_STATE)
							{	
		             ret=OPT_SUCCESS;
							}	
              else
							{	
                 ret=OPT_FAILURE;								
							}	
			        break;
		default:
			        break;
	}	
  //OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
	return ret;
}
//*********************************
//ֱ�߲岹��������
//no:����ţ�0~3
//Moto_Pflag:�������λ
//*********************************
u8 Line_Config(u8 Fpga_no,u16 Moto_Pflag,u8* rxBuf,u8 type)
{	
	 u8 i=0;
	 u8 j;
	 u8 buf[6];
	 u8 para_pos=0;
//	 u32 buf_data;
	 u32 Speed_Data;
	 u32 Max_Speed=0;
	 u32 Min_Speed=0;
	 u32 Dec_Speed=0;
	 u32 Acc_Speed=0;
   Cs_FPGA(Fpga_no);
	 
	 if(Moto_Pflag & 0x0002)//����ٶ�
	 { 
			i=0;
			for(j=0;j<4;j++)
			{
				line_par.max_speed[Fpga_no][j]=rxBuf[para_pos+i+j];
			}
			Max_Speed=(rxBuf[para_pos]<<24)+(rxBuf[para_pos+1]<<16)+(rxBuf[para_pos+2]<<8)+rxBuf[para_pos+3];
			Speed_Data=Moto_Speed_Con(Max_Speed); 
//			buf[0]=0x35;
//			buf[1]=Speed_Data>>8;
//			buf[2]=Speed_Data;
			buf[0]=0x04;
			buf[1]=0x05;
			buf[2]=0;			
			buf[3]=0;
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_no+1,buf,FPGA_COMMAND_LEN);
			i=i+4;
	 }
	 if(Moto_Pflag & 0x0004)//����ٶ�
	 {
			for(j=0;j<4;j++)
			{
				line_par.min_speed[Fpga_no][j]=rxBuf[para_pos+i+j];
			}
			Min_Speed=(rxBuf[para_pos+i]<<24)+(rxBuf[para_pos+i+1]<<16)+(rxBuf[para_pos+i+2]<<8)+rxBuf[para_pos+i+3];
			Speed_Data=Moto_Speed_Con(Min_Speed);
//			buf[0]=0x38;
//			buf[1]=Speed_Data>>8;
//			buf[2]=Speed_Data;
			buf[0]=0x04;
			buf[1]=0x06;
			buf[2]=0;			
			buf[3]=0;
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;			
			Send_parameter(Fpga_no+1,buf,FPGA_COMMAND_LEN);
			i=i+4;
	 }
	 if(Moto_Pflag & 0x0008)//���ٶ�
	 {
			for(j=0;j<4;j++)
			{
				line_par.acc[Fpga_no][j]=rxBuf[para_pos+i+j];
			} 
			 Acc_Speed=(rxBuf[para_pos+i]<<24)+(rxBuf[para_pos+i+1]<<16)+(rxBuf[para_pos+i+2]<<8)+rxBuf[para_pos+i+3];
			 Speed_Data=Moto_Acc_Con(Acc_Speed,Max_Speed,Min_Speed);
//			 buf[0]=0x33;
//			 buf[1]=Speed_Data>>8;
//			 buf[2]=Speed_Data;
			 buf[0]=0x04;
			 buf[1]=0x03;
			 buf[2]=0;			
			 buf[3]=0;
			 buf[4]=Speed_Data>>8;
		 	 buf[5]=Speed_Data;				
			 Send_parameter(Fpga_no+1,buf,FPGA_COMMAND_LEN);
			 i=i+4;
	 }
	 if(Moto_Pflag & 0x0010)//���ٶ�
	 {  
			for(j=0;j<4;j++)
			{
				line_par.dec[Fpga_no][j]=rxBuf[para_pos+i+j];
			}
			Dec_Speed=(rxBuf[para_pos+i]<<24)+(rxBuf[para_pos+i+1]<<16)+(rxBuf[para_pos+i+2]<<8)+rxBuf[para_pos+i+3];
			Speed_Data=Moto_Acc_Con(Dec_Speed,Max_Speed,Min_Speed);
//			buf[0]=0x34;
//			buf[1]=Speed_Data>>8;
//			buf[2]=Speed_Data;
			buf[0]=0x04;
			buf[1]=0x04;
			buf[2]=0;			
			buf[3]=0;
			buf[4]=Speed_Data>>8;
		 	buf[5]=Speed_Data;				
			Send_parameter(Fpga_no+1,buf,FPGA_COMMAND_LEN);
			i=i+4;
	 }
	 
	 if(Moto_Pflag & 0x0400)//ֱ�߲岹���� �Ѿ���ʹ��
	 {
		  if(rxBuf[para_pos+i]==0)
			{
				arc_par.rel_dir[Fpga_no][0]=1;
				arc_par.rel_dir[Fpga_no][1]=1;
			}	
			else if(rxBuf[para_pos+i]==1)
			{
				arc_par.rel_dir[Fpga_no][0]=0;
				arc_par.rel_dir[Fpga_no][1]=1;
			}
			else if(rxBuf[para_pos+i]==2)
			{
				arc_par.rel_dir[Fpga_no][0]=0;
				arc_par.rel_dir[Fpga_no][1]=0;
			}
			else if(rxBuf[para_pos+i]==3)
			{
				arc_par.rel_dir[Fpga_no][0]=1;
				arc_par.rel_dir[Fpga_no][1]=0;
			}
		 
			line_par.quadrant[Fpga_no][0]=rxBuf[para_pos+i];
//			buf[0]=0x37;
//			buf[1]=0;
//			buf[2]=rxBuf[para_pos+i];
//			Send_parameter(Fpga_no+1,buf,FPGA_COMMAND_LEN);
			i=i+1;
	 }
		//**���ٲ���***//
	Speed_Data=Get_Dstep(Max_Speed,Min_Speed,Dec_Speed);
//	buf[0]=0x36; 
//	buf[1] = Speed_Data>>8;
//	buf[2] = Speed_Data;
	buf[0]=0x04;
	buf[1]=0x07;
	buf[2]=Speed_Data>>24;			
	buf[3]=Speed_Data>>16;
	buf[4]=Speed_Data>>8;
	buf[5]=Speed_Data;		 
	Send_parameter(Fpga_no+1,buf,FPGA_COMMAND_LEN);
	 
	SPI_Flash_Write((CDV_INT08U *)&line_par, MOTO_LINE_PAR_ADDR, sizeof(line_par)); 
	SPI_Flash_Write((CDV_INT08U *)& arc_par, MOTO_ARC_PAR_ADDR, sizeof(arc_par)); 
	 return 1;
 }	 
//****************************************************
//ֱ�߲岹������ȡ
//pflag:�������
//Fpga_No:���飨0-3��
//****************************************************
u8 Read_Line_Par(u8 Fpga_No,u16 pflag)
{
	u8 i=0;
	u8 j=0;
	Cs_FPGA(Fpga_No);
	 if(pflag & 0x0002)//�ٶ�
	 { 
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=line_par.max_speed[Fpga_No][j];
			}
			i=i+4;			
	 }
	 if(pflag & 0x0004)//����ٶ�
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=line_par.min_speed[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0008)//���ٶ�
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=line_par.acc[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0010)//���ٶ�
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=line_par.dec[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0400)//����
	 {
				Para_Buf[i+j]=line_par.quadrant[Fpga_No][j];
			  i=i+1;
	 }
	return i;//���س���
}	
//*****************************************************
//Բ���岹
//
//
//*****************************************************
RET_STATUS Arc_Interpolation(u8* rxBuf,u8 rxLen, CMD_ARG *arg)
{
	u8 i=0;
	u8 j;
	u32 val_buf=0;
  u8 Para_len=0;
	u8 buf[6]={0};
	u32 Speed_Data;
	u32 Max_Speed=0;
	u32 Min_Speed=0;
	u32 Dec_Speed=0;
	u32 Acc_Speed=0;
//	u32 xo;
//	u32 yo;
//	u32 xe;
//	u32 ye;
	
	RET_STATUS ret =OPT_SUCCESS;
	u8 Fpga_No;//������
	u8 Moto_Function;//������ܣ����ò�����д����������������
	u32 Moto_Pflag;//����������
	u8 para_pos=5;//�������λ��
	u8 type;
	Fpga_No=UartToU32(&rxBuf[0],4,LITTLEEND)-8;//rxBuf[0]-8;
	Moto_Function=rxBuf[4];//rxBuf[1];
	type=rxBuf[5];//rxBuf[2];
	Moto_Pflag=UartToU32(&rxBuf[6],4,LITTLEEND);//(rxBuf[3]<<8)+rxBuf[4];
	rxBuf = rxBuf+3;
	Cs_FPGA(Fpga_No);
	switch(Moto_Function)
	{
		case 0x20: //���в�������
	
							 if(Moto_Pflag & 0x0002)//����ٶ�
							 { 
	                i=0;
								  for(j=0;j<4;j++)
									{
										arc_par.max_speed[Fpga_No][j]=rxBuf[para_pos+i+j];
									}
									 Max_Speed=(rxBuf[para_pos]<<24)+(rxBuf[para_pos+1]<<16)+(rxBuf[para_pos+2]<<8)+rxBuf[para_pos+3];
								  //Max_Speed=(rxBuf[para_pos]<<8)+rxBuf[para_pos+1];
								  Speed_Data=Moto_Speed_Con(Max_Speed); 
								  buf[0]=0x06;
								  buf[1]=0x08;
								  buf[2]=0;									
								  buf[3]=0;
								  buf[4]=Speed_Data>>8;
								  buf[5]=Speed_Data;
								  Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
								  i=i+4;
							 }
							 if(Moto_Pflag & 0x0004)//����ٶ�
							 {
								  for(j=0;j<4;j++)
									{
										arc_par.min_speed[Fpga_No][j]=rxBuf[para_pos+i+j];
									}
									Min_Speed=(rxBuf[para_pos+i]<<24)+(rxBuf[para_pos+i+1]<<16)+(rxBuf[para_pos+i+2]<<8)+rxBuf[para_pos+i+3];
									Speed_Data=Moto_Speed_Con(Min_Speed);
								  buf[0]=0x06;
								  buf[1]=0x09;
								  buf[2]=0;									
								  buf[3]=0;
								  buf[4]=Speed_Data>>8;
								  buf[5]=Speed_Data;
								  Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
								  i=i+4;
							 }
							 if(Moto_Pflag & 0x0008)//���ٶ�
							 {
								  for(j=0;j<4;j++)
									{
										arc_par.acc[Fpga_No][j]=rxBuf[para_pos+i+j];
									} 
									Acc_Speed=(rxBuf[para_pos+i]<<24)+(rxBuf[para_pos+i+1]<<16)+(rxBuf[para_pos+i+2]<<8)+rxBuf[para_pos+i+3];
								  Speed_Data=Moto_Acc_Con(Acc_Speed,Max_Speed,Min_Speed);
								  
								  buf[0]=0x06;
								  buf[1]=0x06;
								  buf[2]=0;									
								  buf[3]=0;
								  buf[4]=Speed_Data>>8;
								  buf[5]=Speed_Data;
								   Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
								   i=i+4;
							 }
							 if(Moto_Pflag & 0x0010)//���ٶ�
							 {  
								  for(j=0;j<4;j++)
									{
										arc_par.dec[Fpga_No][j]=rxBuf[para_pos+i+j];
									}
								  Dec_Speed=(rxBuf[para_pos+i]<<24)+(rxBuf[para_pos+i+1]<<16)+(rxBuf[para_pos+i+2]<<8)+rxBuf[para_pos+i+3];
									Speed_Data=Moto_Acc_Con(Dec_Speed,Max_Speed,Min_Speed);
								  buf[0]=0x06;
								  buf[1]=0x07;
								  buf[2]=0;									
								  buf[3]=0;
								  buf[4]=Speed_Data>>8;
								  buf[5]=Speed_Data;
								  Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
								  i=i+4;
							 }
							  //**���ٲ���***//
							Speed_Data=Get_Dstep(Max_Speed,Min_Speed,Dec_Speed);
							  buf[0]=0x06;
							  buf[1]=0x0B;
							  buf[2]=Speed_Data>>24;									
							  buf[3]=Speed_Data>>16;
							  buf[4]=Speed_Data>>8;
							  buf[5]=Speed_Data;
							Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
							 
							SPI_Flash_Write((CDV_INT08U *)& arc_par, MOTO_ARC_PAR_ADDR, sizeof(arc_par)); 
			        break;
		case 0x21://���������ȡ
			        switch(type) 
						  {
								case 0xFF: 
								case 0x2F:	  
													Para_len=Read_Arc_Par(Fpga_No,Moto_Pflag);
													ResRequest(arg->buf, arg->len , Para_Buf,Para_len,arg);
													break;
								case 0xFE:
								case 0x2E:
												Para_len=Read_Arc_Par(Fpga_No,Moto_Pflag);
												for(i=0;i<Para_len;i++)
												{
													val_buf=val_buf+Para_Buf[i]<<(8*(Para_len-i-1));//buf��4Byte��һ��ֻ��һ��������ÿ����������ȡ4���ֽ���ֵ
												}
												ValSet(rxBuf[5],val_buf);

												break; 
						  }								
			        break;
		case 0x22://����
			        Arc_Run(rxBuf);
							break;
		case 0x23://Բ���ȴ�
			        FPGA_STATE=Read_Fpga_Regist(0x8F00,Fpga_No);
			        if(0x0004 & FPGA_STATE)
		             ret=OPT_SUCCESS;
              else
                 ret=OPT_FAILURE;								
			        break;
		default:
			        break;
	}	
	//OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
	return ret;
}
//****************************************************
//Բ������
//pflag:�������
//no:���飨0-3��
//****************************************************
u8 Arc_Run(u8* rxBuf)
{
	u8 i=0;
	u8 j=0;
	u32 run_step_temp[2];
	u8 buf[6]={0};
	u16 FPGA_POSH=0;
	u16 FPGA_POSL=0;
	u32 run_step;
	int xo=0;
	int yo=0;
  u32 xe=0;
	u32 ye=0;
	u8 dir;
	u8 cw;
	s32 tx;
	s32 ty;
	u8 angle;
	u32 xstep=0;
	u32 ystep=0;
//	u32 xc=0;
//	u32 yc=0;
	u32 rr=0;
	u8 Fpga_No;//������
	u16 Moto_Pflag;//����������
	u8 para_pos=5;//�������λ��
	Fpga_No=rxBuf[0]-8;
	Moto_Pflag=(rxBuf[3]<<8)+rxBuf[4];
	
	if((Moto_Pflag & 0x0001)==0)//����յ�����û����
	{
		return 0;
	}	
	else
	{	
	  xe=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		if((xe>>31)==1)
		{
			xe=0x80000000|(-xe);
		}	
		xee=xe;
		i=i+4;
		//X���յ�����
//		buf[0]=0x46;
//		buf[1]=xe>>24;
//		buf[2]=xe>>16;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
//		buf[0]=0x47;//
//		buf[1]=xe>>8;
//		buf[2]=xe;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		buf[0]=0x06;
		buf[1]=0x04;		
		buf[2]=xe>>24;
		buf[3]=xe>>16;
		buf[4]=xe>>8;
		buf[5]=xe;
		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);		
		ye=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		i=i+4;
		if((ye>>31)==1)
		{
			ye=0x80000000|(-ye);
		}	
		yee=ye;
		//Y���յ�����
//		buf[0]=0x48;//
//		buf[1]=ye>>24;
//		buf[2]=ye>>16;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
//		buf[0]=0x49;//
//		buf[1]=ye>>8;
//		buf[2]=ye;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);		
		buf[0]=0x06;
		buf[1]=0x05;	
		buf[2]=ye>>24;
		buf[3]=ye>>16;
		buf[4]=ye>>8;
		buf[5]=ye;
		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
	}	
	if(Moto_Pflag & 0x0002)//�������Բ������
	{	
		xc=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		if((xc>>31)==1)
		{
			xc=0x80000000|(-xc);
		}
//		buf[0]=0x42;//Բ��x��
//		buf[1]=xc>>24;
//		buf[2]=xc>>16;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
//		buf[0]=0x43;//Բ��x��
//		buf[1]=xc>>8;
//		buf[2]=xc;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		buf[0]=0x06;//Բ��x
		buf[1]=0x02;		
		buf[2]=xc>>24;
		buf[3]=xc>>16;
		buf[4]=xc>>8;
		buf[5]=xc;
		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		i=i+4;
		
    yc=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		if((yc>>31)==1)
		{
			yc=0x80000000|(-yc);
		}
//		buf[0]=0x44;//Բ��y��
//		buf[1]=yc>>24;
//		buf[2]=yc>>16;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);	

//		buf[0]=0x45;//Բ��y��
//		buf[1]=yc>>8;
//		buf[2]=yc;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		buf[0]=0x06;//Բ��Y
		buf[1]=0x03;		
		buf[2]=yc>>24;
		buf[3]=yc>>16;
		buf[4]=yc>>8;
		buf[5]=yc;
		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);	
		i=i+4;
	}	
	if((Moto_Pflag & 0x0004))//��������
	{
		dir=rxBuf[para_pos+i];
		i=i+1;
	}	
	if((Moto_Pflag & 0x0008))//����뾶����
	{
		rr=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		i=i+4;
	}	
	if((Moto_Pflag & 0x0010))//����С�ڰ�Բ
	{
		cw=rxBuf[para_pos+i];
		i=i+1;
	}
	if((Moto_Pflag & 0x0020))//������
	{
		tx=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		i=i+4;
	  ty=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		i=i+4;
	}
	if((Moto_Pflag & 0x0040))//���н�
	{
		angle=rxBuf[para_pos+i];
		i=i+1;
	}
//	FPGA_POSH=Read_Fpga_Regist(0xd6,Fpga_No+1);
//  FPGA_POSL=Read_Fpga_Regist(0xd7,Fpga_No+1);
//  xo =(FPGA_POSH<<16)+FPGA_POSL;//x��ǰλ��

//  FPGA_POSH=Read_Fpga_Regist(0xd8,Fpga_No+1);
//  FPGA_POSL=Read_Fpga_Regist(0xd9,Fpga_No+1);
//  yo=(FPGA_POSH<<16)+FPGA_POSL;//y��ǰλ��
	xo = Read_Fpga_Regist(0x8F03,Fpga_No);//x��ǰλ��
	yo = Read_Fpga_Regist(0x8F04,Fpga_No);//x��ǰλ��
	if((Moto_Pflag & 0x0002)==0)//���Բ������û����
	{
			if((Moto_Pflag & 0x0008)==0)//����뾶����Ҳû����
			{
					if((Moto_Pflag & 0x0020)==0)//���������Ҳû����
					{
							 if((Moto_Pflag & 0x0040)==0)//���Բ�Ľ�Ҳû����
							 {
								 return 0;
							 }
							 else//������Բ�ĽǶ�������
							 { 				
								 //���xc��yc
								 xyc=CircleCentreFromTangent((f64) xo, (f64) yo, (f64) xe, (f64) ye, (f64)angle,cw);
								 y2x(xo,yo,xe,ye,xyc,Fpga_No);
							} 
								 								

					}
					else//��������������le
					{
							 if((tx>xo))
							 {
								 if(ty>yo)
								 {	 
										dir=(1^arc_par.rel_dir[Fpga_No/2][0])&arc_par.rel_dir[Fpga_No/2][1];//��ʱ��
								 }
								 else
								 {
										dir=(0^arc_par.rel_dir[Fpga_No/2][0])&arc_par.rel_dir[Fpga_No/2][1];//˳ʱ��
								 }								 
							 }
							 else if(ty>yo)
							 {
									dir=(0^arc_par.rel_dir[Fpga_No/2][0])&arc_par.rel_dir[Fpga_No/2][1];//˳ʱ��
							 }
							 else
							 {
									dir=(1^arc_par.rel_dir[Fpga_No/2][0])&arc_par.rel_dir[Fpga_No/2][1];//��ʱ��
							 }							 
            // }							 
						 xyc=CircleCentreFromThirdPoint((f64) xo, (f64) yo, (f64) xe, (f64) ye, (f64) tx, (f64) ty); 
					   y2x(xo,yo,xe,ye,xyc,Fpga_No);					
					}	
			}
			else//����뾶��������le 
			{	
           if(rr==1)
           { 						
							return 1;				
					 }
           else
           {  	
              for(j=0;j<2;j++)	
              {						 
								xyc_temp[j]=CircleCentreFromRadius((f64)xo, (f64)yo, (f64)xe, (f64)ye, (f64)rr,j);
								y2x(xo,yo,xe,ye,xyc_temp[j],Fpga_No);	
                if(dir==0)//����˳ʱ��
								{	
									if((xo==xe)&&(xe==ye))
									{
										xstep=4*rr;
										ystep=4*rr;
										run_step_temp[j]=xstep+ystep; 
									}	
									else if((xe==0)&&(ye==0))
									{	
										run_step_temp[j]=sqrt((xc &line_qua) *(xc &line_qua)+(yc &line_qua)*(yc &line_qua))*8;
									}
									else
									{
											run_step_temp[j]=Arc_Runstep(Fpga_No,xc,yc,xee,yee,dir);
									}					
									//����
									buf[0]=0x06;
									buf[1]=0x01;
									buf[2]=0x00;									
									buf[3]=0x00;
									buf[4]=0x00;
									buf[5]=0x01;
									Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
									i=i+1;
								}	
								else //��ʱ��
								{
										if((xo==xe)&&(xe==ye))
									{
										xstep=4*rr;
										ystep=4*rr;
										run_step_temp[j]=xstep+ystep;
									}	
										else if((xe==0)&&(ye==0))
										{	
											run_step_temp[j]=sqrt((xc &line_qua) *(xc &line_qua)+(yc &line_qua)*(yc &line_qua))*8;
										}
										else
										{
												run_step_temp[j]=sqrt((xc &line_qua) *(xc &line_qua)+(yc &line_qua)*(yc &line_qua))*8-Arc_Runstep(Fpga_No,xc,yc,xee,yee,dir);
												//run_step_temp[j]=Arc_Runstep(Fpga_No,xc,yc,xee,yee,dir);

										}	
										//����
										buf[0]=0x06;
										buf[1]=0x01;
										buf[2]=0x00;									
										buf[3]=0x00;
										buf[4]=0x00;
										buf[5]=0x00;
										Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
										i=i+1;
								}	 								
							}	
							if(cw==0)
							{
								run_step=run_step_temp[0]<run_step_temp[1] ? run_step_temp[0]:run_step_temp[1];
								xyc=run_step_temp[0]<run_step_temp[1] ? xyc_temp[1]:xyc_temp[0];
									
							}	
							else
							{
								run_step=run_step_temp[0]<run_step_temp[1] ? run_step_temp[1]:run_step_temp[0];
								xyc=run_step_temp[0]<run_step_temp[1] ? xyc_temp[0]:xyc_temp[1];
								//y2x(xo,yo,xe,ye,Fpga_No);
							}	
							y2x(xo,yo,xe,ye,xyc,Fpga_No);
					 }	 
				}		
	}
	else//������Բ������
	{
		if((xc==0)&&(yc==0))//Բ�����겻��
		{
			return 1;
		}	
			
	}	
  if((Moto_Pflag & 0x0008)==0)//����뾶����Ҳû����
	{	
			if(dir==0)//����˳ʱ��
			{	
				if((xo==xe)&&(xe==ye))
				{
					xstep=4*rr;
					ystep=4*rr;
					run_step=xstep+ystep; 
				}	
				else if((xe==0)&&(ye==0))
				{	
					run_step=sqrt((xc &line_qua) *(xc &line_qua)+(yc &line_qua)*(yc &line_qua))*8;
				}
				else
				{
						run_step=Arc_Runstep(Fpga_No,xc,yc,xee,yee,dir);
				}					
				//����
				buf[0]=0x06;
				buf[1]=0x01;
				buf[2]=0x00;									
				buf[3]=0x00;
				buf[4]=0x00;
				buf[5]=0x01;
				Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
				i=i+1;
			}	
			else //��ʱ��
			{
					if((xo==xe)&&(xe==ye))
				{
					xstep=4*rr;
					ystep=4*rr;
					run_step=xstep+ystep;
				}	
					else if((xe==0)&&(ye==0))
					{	
						run_step=sqrt((xc &line_qua) *(xc &line_qua)+(yc &line_qua)*(yc &line_qua))*8;
					}
					else
					{
							run_step=sqrt((xc &line_qua) *(xc &line_qua)+(yc &line_qua)*(yc &line_qua))*8-Arc_Runstep(Fpga_No,xc,yc,xee,yee,dir);
					}	
					//����
					buf[0]=0x06;
					buf[1]=0x01;
					buf[2]=0x00;									
					buf[3]=0x00;
					buf[4]=0x00;
					buf[5]=0x00;
					Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
					i=i+1;
			}	
	 }			
	 //���в���
	 buf[0]=0x06;
	 buf[1]=0x0A;
	 buf[2]=run_step>>24;
	 buf[3]=run_step>>16;
	 buf[4]=run_step>>8;
	 buf[5]=run_step;
	 Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
	
//	 buf[0]=0x4e;
//	 buf[1]=run_step>>8;
//	 buf[2]=run_step;
//	 Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
	 //��������
	 buf[0]=0x06;
	 buf[1]=0x00;
	 buf[2]=0x00;	 
	 buf[3]=0x00;
	 buf[4]=0x00;
	 buf[5]=0x51;
	 Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
	
	 return 1;
}
//****************************************************
//Բ���岹������ȡ
//pflag:�������
//Fpga_No:���飨0-3��
//****************************************************
u8 Read_Arc_Par(u8 Fpga_No,u16 pflag)
{
	u8 i=0;
	u8 j=0;
	 if(pflag & 0x0002)//�ٶ�
	 { 
		  for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=arc_par.max_speed[Fpga_No][j];
			}	
			i=i+4;			
	 }
	 if(pflag & 0x0004)//����ٶ�
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=arc_par.min_speed[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0008)//���ٶ�
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=arc_par.acc[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0010)//���ٶ�
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=arc_par.dec[Fpga_No][j];
			}
			i=i+4;
	 }
	return i;//���س���
}	
/*************************************************************
*����Բ��Ҫ�ߵĲ���
*xo��yo��Բ������
*xee��yee���յ�����
**************************************************************/
u32 Arc_Runstep(u8 Fpga_No ,u32 xo,u32 yo,u32 xe,u32 ye,u8 dir)
 {
  u32 run_step=0;
	u32 arc_r;//�뾶
  u8 start_qua=0;
	u8 end_qua=0;
	u32 xstep=0;
	u32 ystep=0;
	u32 r_xo=(xo>>31)? line_qua & xo :0x80000000|xo;
	u32 r_yo=(yo>>31)? line_qua & yo :0x80000000|yo;
	
//	 u8 buf[3]={0};
	u32 r_xe=0;
	u32 r_ye=0;
	arc_r=sqrt(xo*xo+yo*yo);
	if((xo>>31)==1)
	{
		if((xe>>31)==0)
		{
			r_xe=(line_qua & xe)+(line_qua & xo);
		}
		else
		{
			if((line_qua & xe)>(line_qua & xo))
			{
				r_xe=0x80000000|((line_qua & xe)-(line_qua & xo));
			}
      else
      {
				r_xe=0x80000000|((line_qua & xo)-(line_qua & xe));
			}				
		}	
	}
	else
	{	
		if((xe>>31)==1)
			{
				r_xe=0x80000000 |((line_qua & xe)+(line_qua & xo));
			}
			else
			{
				if(xe>=xo)
				{
					r_xe=xe-xo;
				}
				else
				{
					r_xe=0x80000000|((line_qua & xo)-(line_qua & xe)); 
				}				
			}
	}	
	
  if((yo>>31)==1)
	{
		if((ye>>31)==0)
		{
			r_ye=(line_qua & ye)+(line_qua & yo);
		}
		else
		{
			if((line_qua & ye)>(line_qua & yo))
			{
				r_ye=0x80000000|((line_qua & ye)-(line_qua & yo));
			}
      else
      {
				r_ye=0x80000000|((line_qua & yo)-(line_qua & ye));
			}				
		}	
	}
	else
	{	
		if((ye>>31)==1)
			{
				r_ye=0x80000000 |((line_qua & ye)+(line_qua & yo));
			}
			else
			{
				if(ye>=yo)
				{
					r_ye=ye-yo;
				}
				else
				{
					r_ye=0x80000000|((line_qua & yo)-(line_qua & ye));
				}				
			}
	}				
	
	if((xe==0)&&(ye==0))
	{
    //����һȦ
			run_step=arc_r*8;
		  xstep=arc_r*4;
		  ystep=arc_r*4;
	}	
  else
	{	
		//������������ж� 
		if((r_xo & line_qua)==0)//Y����
		{
			if((r_yo>>31)==0)
			{
				start_qua=2;
			}	
			else
			{
				start_qua=4;
			}	
		}
    else if((r_yo & line_qua)==0)//X����
		{
			if((r_xo>>31)==0)
			{
				start_qua=1;
			}	
			else
			{
				start_qua=3;
			}
		}	
		else //����X��Ҳ����Y����
		{
			if((r_xo>>31)==1)
      {				
				if((r_yo>>31)>0)
				{
					start_qua=2;
				}
				else
				{
					start_qua=3;
				}			
		  }
		  else
		  {
			   if((r_yo>>31)>0)
			   {
				    start_qua=1;
			   }
			   else
			   {
				   start_qua=4;
			   }	
		  }		
	  }	
		
		//�յ����������ж� 
    if((r_xe & line_qua)==0)//Y����
		{
			if((r_ye>>31)==0)
			{
				end_qua=2;
			}	
			else
			{
				end_qua=4;
			}	
		}
    else if((r_ye & line_qua)==0)//X����
		{
			if((r_xe>>31)==0)
			{
				end_qua=1;
			}	
			else
			{
				end_qua=3;
			}
		}	
		else //����X��Ҳ����Y����
		{	 
			if((r_xe>>31)==0)
			{
				if((r_ye>>31)==0)
				{
					end_qua=1;
				}	
				else
				{
					end_qua=4;
				}	
			}	
			else 
			{
				if((r_ye>>31)==0)
				{	
					end_qua=2;
				}
				else
				{
					end_qua=3;
				}	
			}	
	 }		
		switch(start_qua)
		{
			case 1:
						 switch(end_qua)
							{
								case 1:
									     xstep=(r_xe & line_qua)-(r_xo & line_qua);
											 ystep=(r_ye & line_qua)-(r_yo & line_qua);
									     if(dir==1)
											 {	 
													xstep=arc_r*4-xstep;
												  ystep=arc_r*4-ystep;
											 }
                       											 
													
											 //run_step=(r_ye & line_qua)-(r_yo & line_qua)+(r_xe & line_qua)-(r_xo & line_qua);//4
								       run_step=xstep+ystep;
								       if((r_xe & line_qua)>(r_xo & line_qua))
											 {
											    run_step=arc_r*8-run_step;
												  xstep=arc_r*4-xstep;
												  ystep=arc_r*4-ystep;
                       }
											 break;
								case 2:
									     xstep=(r_xe & line_qua)-(r_xo & line_qua);
											 ystep=(r_ye & line_qua)-(r_yo & line_qua);
									     if(dir==1)
											 {	 
													xstep=arc_r*4-xstep;
												  ystep=arc_r*4-ystep;
											 }
									     xstep=(r_xe & line_qua)+(r_xo & line_qua);
								       ystep=arc_r*2-(r_ye & line_qua)-(r_yo & line_qua);
											 //run_step=(r_ye & line_qua)-(r_yo & line_qua)+(r_xe & line_qua)-(r_xo & line_qua);//4
								       run_step=xstep+ystep;
									     //run_step=arc_r*2-(r_ye & line_qua)-(r_yo & line_qua)+(r_xe & line_qua)+(r_xo & line_qua);//1
											 break;
								case 3:
									      xstep=(r_xe & line_qua)-(r_xo & line_qua);
											 ystep=(r_ye & line_qua)-(r_yo & line_qua);
									     if(dir==1)
											 {	 
													xstep=arc_r*4-xstep;
												  ystep=arc_r*4-ystep;
											 }
									      xstep=arc_r*2-(r_xe & line_qua)+(r_xo & line_qua);
                        ystep=arc_r*2-(r_yo & line_qua)+(r_ye & line_qua);
                        run_step=xstep+ystep;								
									      //run_step=arc_r*4-(r_yo & line_qua)+(r_ye & line_qua)-(r_xe & line_qua)+(r_xo & line_qua);//2
											 break;
								case 4:
									      xstep=(r_xe & line_qua)-(r_xo & line_qua);
											 ystep=(r_ye & line_qua)-(r_yo & line_qua);
									     if(dir==1)
											 {	 
													xstep=arc_r*4-xstep;
												  ystep=arc_r*4-ystep;
											 }
									      xstep=arc_r*3+(r_xe & line_qua)+(r_xo & line_qua);
                        ystep=arc_r*3-(r_yo & line_qua)-(r_ye & line_qua);
                        run_step=xstep+ystep;	
									    // run_step=arc_r*6-(r_ye & line_qua)-(r_yo & line_qua)+(r_xe & line_qua)+(r_xo & line_qua);//3
											 break;
								default:
											 break;
							}	
						 break;
			case 2:
				     switch(end_qua)
							{
								case 1:
									     xstep=arc_r*3-(r_xe & line_qua)-(r_xo & line_qua);
                        ystep=arc_r*3+(r_yo & line_qua)+(r_ye & line_qua);
                        run_step=xstep+ystep; 
									     //run_step=arc_r*6+(r_ye & line_qua)+(r_yo & line_qua)-(r_xe & line_qua)-(r_xo & line_qua);//7
											 break;
								case 2:
									      xstep=(r_xe & line_qua)-(r_xo & line_qua);
                        ystep=(r_ye & line_qua)-(r_ye & line_qua);
                        run_step=xstep+ystep;
									     //run_step=(r_ye & line_qua)-(r_yo & line_qua)+(r_xe & line_qua)-(r_xo & line_qua);//8
								       if((r_xe & line_qua)<(r_xo & line_qua))
											 {
											    run_step=arc_r*8-run_step;
												  xstep=arc_r*4-xstep;
												  ystep=arc_r*4-ystep;
                       }												 
											 break;
								case 3:
									     xstep=arc_r*2-(r_xe & line_qua)-(r_xo & line_qua);
                        ystep=(r_ye & line_qua)+(r_yo & line_qua);
                        run_step=xstep+ystep;
									     //run_step=arc_r*2+(r_ye & line_qua)+(r_yo & line_qua)-(r_xe & line_qua)-(r_xo & line_qua);//5
											 break;
								case 4:
									     xstep=arc_r*2-(r_xo & line_qua)+(r_xe & line_qua);
                       ystep=arc_r*2-(r_ye & line_qua)+(r_yo & line_qua);
                       run_step=xstep+ystep;
									    // run_step=arc_r*4-(r_ye & line_qua)+(r_yo & line_qua)+(r_xe & line_qua)-(r_xo & line_qua);//6
											 break;
								default:
											 break;
							}	
						 break;
			case 3:
				      switch(end_qua)
							{
								case 1:
									     xstep=arc_r*2-(r_xe & line_qua)+(r_xo & line_qua);
                       ystep=arc_r*2-(r_yo & line_qua)+(r_ye & line_qua);
                       run_step=xstep+ystep;
											//run_step=arc_r*4-(r_yo & line_qua)+(r_ye & line_qua)-(r_xe & line_qua)+(r_xo & line_qua);	//2	
 								      break;
								case 2:
									     xstep=arc_r*2-(r_xo & line_qua)+(r_xe & line_qua);
                       ystep=arc_r*4-(r_ye & line_qua)-(r_yo & line_qua);
                       run_step=xstep+ystep; 
									     //run_step=arc_r*6-(r_ye & line_qua)-(r_yo & line_qua)+(r_xe & line_qua)+(r_xo & line_qua);
											 break;
								case 3:
									     xstep=(r_xo & line_qua)-(r_xe & line_qua);
                       ystep=(r_ye & line_qua)-(r_yo & line_qua);
                       run_step=xstep+ystep; 
									     //run_step=(r_ye & line_qua)-(r_yo & line_qua)+(r_xo & line_qua)-(r_xe & line_qua);//4
									     if((r_xe & line_qua)>(r_xo & line_qua))
											 {	 
									        run_step=arc_r*8-run_step;//4
												  xstep=arc_r*4-xstep;
												  ystep=arc_r*4-ystep;
											 }											 
											 break;
								case 4:
									     xstep=(r_xo & line_qua)+(r_xe & line_qua);
                       ystep=arc_r*2-(r_ye & line_qua)-(r_yo & line_qua);
                       run_step=xstep+ystep; 
									     //run_step=arc_r*2-(r_ye & line_qua)-(r_yo & line_qua)+(r_xe & line_qua)+(r_xo & line_qua);//1
											 break;
								default:
											 break;
							}	
						 break;
			case 4:
				     switch(end_qua)
							{
								case 1:
									     xstep=arc_r*2-(r_xo & line_qua)-(r_xe & line_qua);
                       ystep=(r_ye & line_qua)+(r_yo & line_qua);
                       run_step=xstep+ystep;
											 //run_step=arc_r*2+(r_ye & line_qua)+(r_yo & line_qua)-(r_xe & line_qua)-(r_xo & line_qua);//5
											 break;
								case 2:
									     xstep=arc_r*2+(r_xe & line_qua)-(r_xo & line_qua);
                       ystep=arc_r*2+(r_ye & line_qua)-(r_yo & line_qua);
                       run_step=xstep+ystep; 
									     //run_step=arc_r*4-(r_ye & line_qua)+(r_yo & line_qua)+(r_xe & line_qua)-(r_xo & line_qua);//6
											 break;
								case 3:
									     xstep=arc_r*6-(r_xo & line_qua)-(r_xe & line_qua);
                       ystep=(r_ye & line_qua)+(r_yo & line_qua);
                       run_step=xstep+ystep; 
									     //run_step=arc_r*6+(r_ye & line_qua)+(r_yo & line_qua)-(r_xe & line_qua)-(r_xo & line_qua);//7
											 break;
								case 4:
									     xstep=(r_xe & line_qua)-(r_xo & line_qua);
                       ystep=(r_ye & line_qua)-(r_yo & line_qua);
                       run_step=xstep+ystep; 
									    // run_step=(r_ye & line_qua)-(r_yo & line_qua)+(r_xe & line_qua)-(r_xo & line_qua);//8
								       if((r_xe & line_qua)<(r_xo & line_qua))
											 {
											    run_step=arc_r*8-run_step;
												  xstep=arc_r*4-xstep;
												  ystep=arc_r*4-ystep;
                       }
											 break;
								default:
											 break;
							}	
						 break;
			default:
						 break;
		}	
		
	}	
//  buf[0]=0x46;//
//		buf[1]=xstep>>24;
//		buf[2]=xstep>>16;
//		Send_parameter(no+1,buf,FPGA_COMMAND_LEN);
//		buf[0]=0x47;//
//		buf[1]=xstep>>8;
//		buf[2]=xstep;
//		Send_parameter(no+1,buf,FPGA_COMMAND_LEN);

//		buf[0]=0x48;//
//		buf[1]=ystep>>24;
//		buf[2]=ystep>>16;
//		Send_parameter(no+1,buf,FPGA_COMMAND_LEN);
//		buf[0]=0x49;//
//		buf[1]=ystep>>8;
//		buf[2]=ystep;
//		Send_parameter(no+1,buf,FPGA_COMMAND_LEN);	
	return run_step;
}
//*********************************
//���������ʼ��
//Fpga_No:����ţ�0~3
//
//*********************************
u8 Line_Arc_Config(u8 Fpga_No)
{	
	 u8 i=0;
//	 u8 j;
	 u8 buf[6];
//	 u32 buf_data;
	 u32 Speed_Data;
	 u32 Max_Speed=0;
	 u32 Min_Speed=0;
	 u32 Dec_Speed=0;
	 u32 Acc_Speed=0;
   Cs_FPGA(Fpga_No);
	  //ֱ�� 
	   //����ٶ�
			Max_Speed=(line_par.max_speed[Fpga_No][0]<<24)+(line_par.max_speed[Fpga_No][1]<<16)+(line_par.max_speed[Fpga_No][2]<<8)+line_par.max_speed[Fpga_No][3];
      Speed_Data=Moto_Speed_Con(Max_Speed); 
			buf[0]=0x04;
			buf[1]=0x05;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		 //�����
			Min_Speed=(line_par.min_speed[Fpga_No][0]<<24)+(line_par.min_speed[Fpga_No][1]<<16)+(line_par.min_speed[Fpga_No][2]<<8)+line_par.min_speed[Fpga_No][3];
			Speed_Data=Moto_Speed_Con(Min_Speed);
			buf[0]=0x04;
			buf[1]=0x06;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			i=i+4;
	  //���ٶ�  
			Acc_Speed=(line_par.acc[Fpga_No][0]<<24)+(line_par.acc[Fpga_No][1]<<16)+(line_par.acc[Fpga_No][2]<<8)+line_par.acc[Fpga_No][3];
			Speed_Data=Moto_Acc_Con(Acc_Speed,Max_Speed,Min_Speed);
			buf[0]=0x04;
			buf[1]=0x03;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			 Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			 i=i+2;
	  //���ٶ�
			Dec_Speed=(line_par.dec[Fpga_No][0]<<24)+(line_par.dec[Fpga_No][1]<<16)+(line_par.dec[Fpga_No][2]<<8)+line_par.dec[Fpga_No][3];
      Speed_Data=Moto_Acc_Con(Dec_Speed,Max_Speed,Min_Speed);
			buf[0]=0x04;
			buf[1]=0x04;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			i=i+2;
		//���ٲ���
			Speed_Data=Get_Dstep(Max_Speed,Min_Speed,Dec_Speed);
			buf[0]=0x04;
			buf[1]=0x07;
			buf[2]=Speed_Data>>24;
			buf[3]=Speed_Data>>16;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);	
	  //ֱ�߲岹���� ͣ��
//			buf[0]=0x37;
//			buf[1]=0;
//			buf[2]=line_par.quadrant[Fpga_No][0];
//			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			i=i+1;
	 //X��������
//			buf[0]=0x0d;
//			buf[1]=0;
//			buf[2]=line_par.x_pos_dir[Fpga_No][0];
			buf[0]=0x00;
			buf[1]=0x0B;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=0;
			buf[5]=line_par.x_pos_dir[Fpga_No][0];			
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			i=i+1;
   //Y��������
//	    buf[0]=0x21;
//			buf[1]=0;
//			buf[2]=line_par.y_pos_dir[Fpga_No][0];
			buf[0]=0x02;
			buf[1]=0x0B;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=0;
			buf[5]=line_par.y_pos_dir[Fpga_No][0];
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			i=i+1;
  //Բ��
	   //����ٶ�
			Max_Speed=(arc_par.max_speed[Fpga_No][0]<<24)+(arc_par.max_speed[Fpga_No][1]<<16)+(arc_par.max_speed[Fpga_No][2]<<8)+arc_par.max_speed[Fpga_No][3];
      Speed_Data=Moto_Speed_Con(Max_Speed); 
			buf[0]=0x06;
			buf[1]=0x08;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		 //�����
			Min_Speed=(arc_par.min_speed[Fpga_No][0]<<24)+(arc_par.min_speed[Fpga_No][1]<<16)+(arc_par.min_speed[Fpga_No][2]<<8)+arc_par.min_speed[Fpga_No][3];
			Speed_Data=Moto_Speed_Con(Min_Speed);
			buf[0]=0x06;
			buf[1]=0x09;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			i=i+4;
	  //���ٶ�  
			 Acc_Speed=(arc_par.acc[Fpga_No][0]<<24)+(arc_par.acc[Fpga_No][1]<<16)+(arc_par.acc[Fpga_No][2]<<8)+arc_par.acc[Fpga_No][3];
			 Speed_Data=Moto_Acc_Con(Acc_Speed,Max_Speed,Min_Speed);
			buf[0]=0x06;
			buf[1]=0x06;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			 Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			 i=i+2;
	  //���ٶ�
			Dec_Speed=(arc_par.dec[Fpga_No][0]<<24)+(arc_par.dec[Fpga_No][1]<<16)+(arc_par.dec[Fpga_No][2]<<8)+arc_par.dec[Fpga_No][3];
      Speed_Data=Moto_Acc_Con(Dec_Speed,Max_Speed,Min_Speed);
			buf[0]=0x06;
			buf[1]=0x07;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			i=i+2;
	 //���ٲ���
			Speed_Data=Get_Dstep(Max_Speed,Min_Speed,Dec_Speed);
			buf[0]=0x06;
			buf[1]=0x0B;
			buf[2]=Speed_Data>>24;
			buf[3]=Speed_Data>>16;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);		
	 
	 return 1;
 }	
//***********************************
//��Բ������
//
//*********************************** 
u8 Xc_Yc(float x1,float y1,float x2,float y2,float r, u8 type)
{
	float k;
	float xa;
	float ya;
	float A;
	float B;
	float C;
	float Z;
	float s;
	float XC1;
	float XC2;
	float YC1;
	float YC2;

	if(y1-y2==0)//���������ͬһ��ˮƽ����
	{
		if(x1>x2)
		{
			if(x1-x2==r+r)
			{
				xc=(x1+x2)/2;
				yc=y1;
			}	
			else if(x1-x2>r+r)
			{
				return 0;
			}			
		}	
	}	
	k=-(x1-x2)/(y1-y2);
	xa=(x1+x2)/2;
	ya=(y1+y2)/2;
	s=r*r*4-(x1-x2)*(x1-x2)-(y1-y2)*(y1-y2);

	//Ȼ�����������飺
	//yc=ya+k*(xc-xa);
	//(yc-ya)^2+(xc-xa)^2=s/4;  
	
  //(k*(xc-xa))^2+(xc-xa)^2=s/4;
	//(k^2*(xc-xa)^2)+(xc-xa)^2=s/4;
	//(k^2*(xc^2+xa^2-2xc*xa))+(xc-xa)^2=s/4;
	//(k^2*xc^2+k^2*xa^2-k^2*2xc*xa))+(xc-xa)^2=s/4;
  //k^2*xc^2 +k^2*xa^2 -k^2*2xc*xa +xc^2 +xa^2 -2xc*xa=s/4;
	
	    
	//�������
	A=(k*k)+1;
	B=-(2*k*k*xa+2*xa);
	C=k*k*xa*xa+xa*xa-s/4;
  Z=B*B-4*A*C;
	
  if(Z>=0.0)
	{	
		XC1=(-B+sqrt(Z))/(2*A);
		YC1=ya+k*(XC1-xa);
		
		XC2=(-B-sqrt(Z))/(2*A);
		YC2=ya+k*(XC2-xa);	
		
		if((XC1*XC1+YC1*YC1)>(XC2*XC2+YC2*YC2))//Ĭ��ѡ���������ԭ�����Բ��
		{
			if(type>=1)//Ĭ��ѡ���������ԭ�����Բ��
			{
				xc=XC1;
				yc=YC1;
			}	
			else//Ĭ��ѡ���������ԭ��Զ��Բ��
			{
				xc=XC2;
				yc=YC2;
			}	
		}	
		else
		{
			if(type>=1)//Ĭ��ѡ���������ԭ�����Բ��
			{
				xc=XC2;
				yc=YC2;
			}	
			else//Ĭ��ѡ���������ԭ��Զ��Բ��
			{
				xc=XC1;
				yc=YC1;
			}
		}	
	}
	else
	{	
		return 0;//ʧ��
	}	
	return 1;//�ɹ� 
}
//************************************************
//����ת��
//
//************************************************
void y2x(u32 xo,u32 yo,u32 xe,u32 ye,_PT xyc,u8 Moto_NO)
{ 
	 u8 buf[6]={0};
	 if(arc_par.rel_dir[Moto_NO/2][0]==0)//x�෴
	 {	 
			xee=(xe>=xo)? 0x80000000|xe-xo :(xo-xe);
	 }	 
	 else
	 {	 
		  xee=(xe>=xo)? xe-xo :0x80000000|(xo-xe);
	 }	
	 
	 if(arc_par.rel_dir[Moto_NO/2][1]==1)//yͬ��
	 {	 
		    yee=(ye>=yo)? ye-yo :0x80000000|(yo-ye);		
				
	 }	 
	 else
	 {	 
			 yee=(ye>=yo)? 0x80000000|(ye-yo) :(yo-ye);
        
	 }	 
	 //���xc��yc
	 if(xyc.x<0)
	 {
		 xyc.x=-xyc.x;
		 if(arc_par.rel_dir[Moto_NO/2][0]==0)
			 xc=xo+xyc.x;
		 else	
			 xc=0x80000000|(xo+xyc.x);	 
	 }
	 else
	 {	
		 if(arc_par.rel_dir[Moto_NO/2][0]==0)
				xc=(xyc.x>=xo)? 0x80000000|(xyc.x-xo) :(xo-xyc.x);
		 else							 
				xc=(xyc.x>=xo)? xyc.x-xo :0x80000000|(xo-xyc.x);
	 }	
		
	if(xyc.y<0)
	{
		xyc.y=-xyc.y;
		if(arc_par.rel_dir[Moto_NO/2][1]==0)
		{
			yc=0x80000000|(yo+xyc.y);
		}
		else
		{	
			yc=yo+xyc.y; 
		}	
	}
	else
	{	
		if(arc_par.rel_dir[Moto_NO/2][1]==0)
			 
			 yc=(xyc.y>=yo)? 0x80000000|(xyc.y-yo) :(yo-xyc.y);
		else
			 yc=(xyc.y>=yo)? xyc.y-yo :0x80000000|(yo-xyc.y);
			
	}		

//	buf[0]=0x42;//Բ��x��
//	buf[1]=xc>>24;
//	buf[2]=xc>>16;
//	Send_parameter(no+1,buf,FPGA_COMMAND_LEN);

//	buf[0]=0x43;//Բ��x��
//	buf[1]=xc>>8;
//	buf[2]=xc;
//	Send_parameter(no+1,buf,FPGA_COMMAND_LEN);
//				
//	buf[0]=0x44;//Բ��y��
//	buf[1]=yc>>24;
//	buf[2]=yc>>16;
//	Send_parameter(no+1,buf,FPGA_COMMAND_LEN);	

//	buf[0]=0x45;//Բ��y��
//	buf[1]=yc>>8;
//	buf[2]=yc;
//	Send_parameter(no+1,buf,FPGA_COMMAND_LEN);
	buf[0]=0x06;//Բ��x
	buf[1]=0x02;		
	buf[2]=xc>>24;
	buf[3]=xc>>16;
	buf[4]=xc>>8;
	buf[5]=xc;
	Send_parameter(Moto_NO+1,buf,FPGA_COMMAND_LEN);
	buf[0]=0x06;//Բ��y
	buf[1]=0x03;		
	buf[2]=yc>>24;
	buf[3]=yc>>16;
	buf[4]=yc>>8;
	buf[5]=yc;
	Send_parameter(Moto_NO+1,buf,FPGA_COMMAND_LEN);
}	


/**
  * @brief  ��դ���������
  *  
  * @param  rxBuf     ��������  
  *  			rxLen     ���ݳ���
  *   
  * @retval ����ֵ˵��
            OPT_SUCCESS����ִ�����
            OPT_FAILURE����δִ����ɣ�����ȴ�δ�ȵ�
  *
  * @note   							                                            �ַ���
  */

RET_STATUS GratingRuler_CmdParse(u8* rxBuf,u8 rxLen, CMD_ARG *arg)	
{
	u8 i;
	u8 Para_len;
	u32 buf=0;
	u32 Grating_No;//��դ�ߺ�
	u8 Grating_Function;//��դ�߹��ܣ����ò�����д����������������
	u8 type;
	u32 pflag;
	Grating_No = UartToU32(&rxBuf[0],4,LITTLEEND);//rxBuf[0];
	Grating_Function = rxBuf[4];
	type = rxBuf[5];
	pflag = UartToU32(&rxBuf[6],4,LITTLEEND);//(rxBuf[3]<<8)+rxBuf[4];
	rxBuf = rxBuf+3;
	switch(Grating_Function)
	{
		case 0x30:	//��դ�߲�������
				switch(type) 
				{
					case 0xFF:  

							break;
					case 0xFE://ʹ�ñ���ֵ
							break;
					default:
						   break;
				}
			break;
		case 0x31:	//��դ�߲�����ȡ
				switch(type) 
				{
					case 0xFF: 
					case 0x2F:	  
							Para_len=Read_GratingRuler_Pos(Grating_No);
							ResRequest(arg->buf, arg->len , Para_Buf,Para_len,arg);
							break;
					case 0xFE:
					case 0x2E:
							Para_len=Read_GratingRuler_Pos(Grating_No);
							for(i=0;i<Para_len;i++)
							{
								buf=buf+(Para_Buf[i]<<(8*(Para_len-i-1)));//buf��4Byte��һ��ֻ��һ��������ÿ����������ȡ4���ֽ���ֵ
							}
							ValSet(rxBuf[5],buf);
							break; 					
					default:
							 break;
				}
			break;	
		case 0x32:	//��դ��ʹ��
				if(type == 0xFF)
				{
					GratingRuler_Ctrl(Grating_No,pflag&0x0001);
				}
				
			break;				
		case 0x33:	//��դ�߼�������
				if(type == 0xFF)
				{
					Clear_GratingRuler(Grating_No);
				}
			break;	
		case 0x34:	//��դ�߼�����ȡ
				switch(type) 
				{
					case 0xFF: 
					case 0x2F:	  
							Para_len=Read_GratingRuler_Pos(Grating_No);
							ResRequest(arg->buf, arg->len , Para_Buf,Para_len,arg);
							break;
					case 0xFE:
					case 0x2E:
							Para_len=Read_GratingRuler_Pos(Grating_No);
							for(i=0;i<Para_len;i++)
							{
								buf=buf+(Para_Buf[i]<<(8*(Para_len-i-1)));//buf��4Byte��һ��ֻ��һ��������ÿ����������ȡ4���ֽ���ֵ
							}
							ValSet(rxBuf[5],buf);
							break; 					
					default:
							 break;
				}
			break;
		case 0x35:	//��դ��λ�ö�ȡ
				switch(type) 
				{
					case 0xFF: 
					case 0x2F:	  
							Para_len=Read_GratingRuler_Pos(Grating_No);
							ResRequest(arg->buf, arg->len , Para_Buf,Para_len,arg);
							break;
					case 0xFE:
					case 0x2E:
							Para_len=Read_GratingRuler_Pos(Grating_No);
							for(i=0;i<Para_len;i++)
							{
								buf=buf+(Para_Buf[i]<<(8*(Para_len-i-1)));//buf��4Byte��һ��ֻ��һ��������ÿ����������ȡ4���ֽ���ֵ
							}
							ValSet(rxBuf[5],buf);
							break; 					
					default:
							 break;
				}
			break;		
	}
	return OPT_SUCCESS;
}
//****************************************************
//��ȡ��դ��λ��
//
//Grating_No:��դ�ߺţ�0-7��
//****************************************************
u8 Read_GratingRuler_Pos(u8 Grating_No)
{
	 s32 Grating_POS;
	 u16 i=0;
	 Cs_FPGA(Grating_No/2);
	 if(Grating_No%2)	//Y�� ż����
	 {
		 bmq_count = Read_Fpga_Regist(0x8F02,Grating_No>>1);  
	 }
	 else			//X�� ������
	 {
		 bmq_count = Read_Fpga_Regist(0x8F01,Grating_No>>1);  

	 }	
//	Grating_POS =(bmq_count_H <<16)+bmq_count_L;
	Grating_POS = bmq_count;
	for(i=0;i<4;i++)
	{
		Para_Buf[i]=Grating_POS>>(24-i*8);
	}
	return i;
}
//****************************************************
//�����դ�߼���
//
//Grating_No:��դ�ߺţ�0-7��
//****************************************************
void Clear_GratingRuler(u8 Grating_No)
{
	u8 buf[6] = {0};
	Cs_FPGA(Grating_No/2);
	if(Grating_No%2)//Y�� ż����
	{
		buf[0]=0X02;
	}
	else//X�� ������
	{
		buf[0]=0x00;
	}	
	buf[1] = 0x0C;
	buf[2] = 0;
	buf[3] = 0;	
	buf[4] = 0;
	buf[5] = 1;

	Send_parameter(Grating_No>>1,buf,FPGA_COMMAND_LEN);
	buf[5] = 0;
	delay_us(5);
	Send_parameter(Grating_No>>1,buf,FPGA_COMMAND_LEN);//ȥ�������־
}
//****************************************************
//ʹ��/ʧ�ܹ�դ��
//
//Grating_No:��դ�ߺţ�0-7��
//flag: 1ʹ�� 0ʧ��
//****************************************************
void GratingRuler_Ctrl(u8 Grating_No,u16 flag)
{
	u8 buf[6] = {0};
	Cs_FPGA(Grating_No/2);
	if(Grating_No%2)//Y�� ż����
	{
		buf[0]=0X02;
	}
	else//X�� ������
	{
		buf[0]=0x00;
	}	
	buf[1] = 0x0D;
	buf[2] = 0;
	buf[3] = 0;	
	buf[4] = 0;
	if(flag)//ʹ��
		buf[5] = 1;
	else
		buf[5] = 0;
	Send_parameter(Grating_No>>1,buf,FPGA_COMMAND_LEN);
}


void Cs_FPGA(u8 no)
{
//	SPI2_Configuration(SPI_BaudRatePrescaler_64);
//   switch(no)
//	 {
//		case 0:
//			SF0_CS2_DISENABLE;
//			SF0_CS3_DISENABLE;
//			SF0_CS4_DISENABLE;
//			SF0_CS1_DISENABLE;
//			SF0_CS1_ENABLE;
//		  break;
//		case 1:
//			SF0_CS1_DISENABLE;
//			SF0_CS3_DISENABLE;
//			SF0_CS4_DISENABLE;
//			SF0_CS2_DISENABLE;
//		SF0_CS1_ENABLE;
//			SF0_CS2_ENABLE;
//		
////		SF0_CS1_DISENABLE;
////			SF0_CS3_DISENABLE;
////			SF0_CS4_DISENABLE;
////			SF0_CS2_DISENABLE;
//		  break;
//		case 2:
//			SF0_CS1_DISENABLE;
//			SF0_CS2_DISENABLE;
//			SF0_CS4_DISENABLE;
//			SF0_CS3_DISENABLE;
//			SF0_CS3_ENABLE;
//		  break;
//		case 3:
//			SF0_CS1_DISENABLE;
//			SF0_CS2_DISENABLE;
//			SF0_CS3_DISENABLE;
//			SF0_CS4_DISENABLE;
//			SF0_CS4_ENABLE;
//		  break;
//		default:
//			SF0_CS1_DISENABLE;
//			SF0_CS2_DISENABLE;
//			SF0_CS3_DISENABLE;
//			SF0_CS4_DISENABLE;
//		  break;
//	 }
 }

/**
  * @brief  ���������ʼ��
  * @param  
  * @retval 
  * @note   
  */
void MotoParaInit(void) 
{
	Org_Flash_Read((CDV_INT08U *)&moto_par, MOTOPAR_ADDR, sizeof(moto_par));
	Org_Flash_Read((CDV_INT08U *)&line_par, MOTO_LINE_PAR_ADDR, sizeof(line_par));
	Org_Flash_Read((CDV_INT08U *)&arc_par, MOTO_ARC_PAR_ADDR, sizeof(arc_par));
}

/**
  * @brief  ����Ԥ��������ʱ��(��λ����)
  * @param  
  * @retval 
  * @note   
  */
u32 MotoCountdown(MOTOR_INFO* motor,u8 Moto_No,u32 run_step) 
{
//	motor->acc[Moto_No]
	u32 runtime,acctime,dectime,uniformtime = 0;
	u32 TargetSpeed,MinSpeed,MaxSpeed,dec,acc;
	u32 TargetSpeed_Con,MinSpeed_Con,MaxSpeed_Con,dec_Con,acc_Con;
	u32 AccStep,DecStep;
	MOTOR_STATE state;	
	TargetSpeed=motor->target_speed[Moto_No];
	MinSpeed=motor->min_speed[Moto_No];
	dec=motor->dec[Moto_No];
	acc=motor->acc[Moto_No];
	DecStep = Get_Dstep(TargetSpeed,MinSpeed,dec);
	AccStep = Get_Dstep(TargetSpeed,MinSpeed,acc);
	#if 1
		if(moto_par.speed_type[Moto_No]==TRAP_MODE||moto_par.speed_type[Moto_No]==S_MODE)
		{
 			if(DecStep+AccStep<=run_step)//FPGA_SPEED_CHANGE_CYCLE
			{
				acctime = (CDV_INT64U)(TargetSpeed-MinSpeed)*1000/acc;//����ʱ��
				dectime = (CDV_INT64U)(TargetSpeed-MinSpeed)*1000/dec;//����ʱ��
				MaxSpeed = TargetSpeed;
				uniformtime = (CDV_INT32U)((CDV_INT64U)(run_step-DecStep-AccStep)*1000/(CDV_INT64U)TargetSpeed);	
				runtime = acctime+dectime+uniformtime;
			}
//			else if(AccStep>run_step)//���ٲ��������ܲ���  ���ٶȸĳɼ��ٶ�
//			{
//				//DecStep = run_step/2;		
//				DecStep=run_step*acc/(dec+acc);
//				dec_Con = (u32)Moto_Acc_Con(dec,TargetSpeed,MinSpeed);	
//				MinSpeed_Con = (u32)Moto_Speed_Con(MinSpeed);
//				MaxSpeed = MaxSpeed_Real(DecStep,MinSpeed_Con,dec_Con);				
//				dectime = (MaxSpeed-MinSpeed)*1000/dec;//����ʱ��
//				runtime = dectime*(dec+acc)/acc;		
//			}
			else	
			{
				//DecStep = run_step/2;	
				DecStep=(CDV_INT64U)run_step*acc/(dec+acc);
				dec_Con = (u32)Moto_Acc_Con(dec,TargetSpeed,MinSpeed);	
				MinSpeed_Con = (u32)Moto_Speed_Con(MinSpeed);				
				MaxSpeed = MaxSpeed_Real(DecStep,MinSpeed_Con,dec_Con);
				if(MaxSpeed>TargetSpeed)MaxSpeed = TargetSpeed;
				dectime = (CDV_INT64U)(MaxSpeed-MinSpeed)*1000/dec;//����ʱ��	
				runtime = (CDV_INT64U)dectime*(dec+acc)/acc;			
			}
		}
	if(Moto_RunSta[Moto_No]==READY&&moto_par.speed_type[Moto_No]==S_MODE)
	{
		OS_ERR err;
		motor->target_speed[Moto_No] = MaxSpeed;
		Moto_RunSta[Moto_No] = ACC;
		gDec_Times[Moto_No] = (CDV_INT32U)((CDV_FP32)(MaxSpeed-MinSpeed)/((CDV_FP32)dec*FPGA_SPEED_CHANGE_CYCLE));
		gAcc_Times[Moto_No] = (CDV_INT32U)(CDV_FP32)(MaxSpeed-MinSpeed)/((CDV_FP32)acc*FPGA_SPEED_CHANGE_CYCLE);		
		gUniform_Times[Moto_No] = uniformtime/(FPGA_SPEED_CHANGE_CYCLE*1000);		
		OSTaskResume((OS_TCB*)&CdvMotoTaskTCB,&err);
	}
	#else
	TargetSpeed_Con =(u32)Moto_Speed_Con(TargetSpeed);
	MinSpeed_Con = (u32)Moto_Speed_Con(MinSpeed);
	acc_Con =(u32)Moto_Acc_Con(acc,TargetSpeed,MinSpeed);
	dec_Con = (u32)Moto_Acc_Con(dec,TargetSpeed,MinSpeed);
	if(DecStep+AccStep<=run_step)//FPGA_SPEED_CHANGE_CYCLE
	{
		acctime = (CDV_INT32U)((CDV_INT32U)((MinSpeed_Con-TargetSpeed_Con)/(float)acc_Con+0.5)*FPGA_SPEED_CHANGE_CYCLE*1000);//����ʱ��
		dectime = (CDV_INT32U)((CDV_INT32U)((MinSpeed_Con-TargetSpeed_Con)/(float)dec_Con+0.5)*FPGA_SPEED_CHANGE_CYCLE*1000);//����ʱ��
		TargetSpeed=(CDV_INT32U)((CDV_INT64U)FPGA_W_FRE_DIV2*(CDV_INT64U)FPGA_ACC_DIV/(CDV_INT64U)TargetSpeed);
		TargetSpeed=(CDV_INT32U)((CDV_INT64U)FPGA_W_FRE_DIV2*(CDV_INT64U)FPGA_ACC_DIV/(CDV_INT64U)TargetSpeed);
		uniformtime = (CDV_INT32U)((CDV_INT64U)(run_step-DecStep-AccStep)*1000/(CDV_INT64U)TargetSpeed);	
		runtime = acctime+dectime+uniformtime;
	}
	else if(AccStep>run_step)//���ٲ��������ܲ���  ���ٶȸĳɼ��ٶ�
	{
		DecStep = run_step/2;		
		MaxSpeed = MaxSpeed_Real(DecStep,MinSpeed_Con,acc_Con);
		MaxSpeed_Con = (u32)Moto_Speed_Con(MaxSpeed);
		acctime = (CDV_INT32U)((CDV_INT32U)((MinSpeed_Con-MaxSpeed_Con)/(float)acc_Con+0.5)*FPGA_SPEED_CHANGE_CYCLE*1000);//����ʱ��
		runtime = acctime*2;		
	}
	else	//���ٲ���С���ܲ��� ���ٶȸĳɼ��ٶ�
	{
		DecStep = run_step/2;		
		MaxSpeed = MaxSpeed_Real(DecStep,MinSpeed_Con,dec_Con);
		MaxSpeed_Con = (u32)Moto_Speed_Con(MaxSpeed);
		dectime = (CDV_INT32U)((CDV_INT32U)((MinSpeed_Con-MaxSpeed_Con)/(float)dec_Con+0.5)*FPGA_SPEED_CHANGE_CYCLE*1000);//����ʱ��	
		runtime = dectime*2;			
	}
	#endif
	moto_sta.ela_runtime[Moto_No] = GetCdvTimeTick();
	gStageStartup_Time[Moto_No] = GetCdvTimeTick();
	return runtime;
}
/**
  * @brief  �ٶȴﲻ������ٶ�ʱ����ʵ��������ٶȼ���
  * @param  DecStep�����ٲ���
				MinSpeed_Con������ٶ�(ת����)
				acc_Con�����ٶ�(ת����)
  * @retval 
  * @note   
  */
u32 MaxSpeed_Real(u32 DecStep,u32 MinSpeed_Con,u32 acc_Con)
{
	u32 MaxSpeedReal;
	u32 speed_imm;//��ʱ�ٶ�
	double step_imm=0;//��ʱ����
	u32 i=0;
	#if 1
	
		while(step_imm<DecStep)
		{
			speed_imm = (MinSpeed_Con+acc_Con*i)/FPGA_ACC_DIV;
			step_imm += Moto_Speed_Format(speed_imm)*FPGA_SPEED_CHANGE_CYCLE;
			i++;
		}
	
	

	#else
		while(step_imm<DecStep)
		{
			speed_imm = FPGA_W_FRE_DIV2/((float)MinSpeed_Con/FPGA_ACC_DIV-(float)acc_Con/FPGA_ACC_DIV*i);
			step_imm += speed_imm*FPGA_SPEED_CHANGE_CYCLE;
			i++;
		}
	#endif
	//MaxSpeedReal = speed_imm-acc_Con/FPGA_ACC_DIV;
	MaxSpeedReal = speed_imm;
	return MaxSpeedReal;
}
/**
  * @brief  �����Ϣ����
  * @param  Moto_No�������
				pos������λ��
				mode������ģʽ
				id����ϢID
  * @retval 
  * @note   
  */
void MotoMessageSet(u8 Moto_No,u32 pos,u8 mode,u16 id)
{
	u8 buf[6] = {0};
	Cs_FPGA(Moto_No/2);
	if(Moto_No%2)//Y�� ż����
	{
		buf[0]=0X02;
	}
	else//X�� ������
	{
		buf[0]=0x00;
	}	
	//����λ��
	buf[1] = 0x33;
	buf[2] = pos>>24;
	buf[3] = pos>>16;	
	buf[4] = pos>>8;
	buf[5] = pos;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	
	//����ģʽ
	buf[1] = 0x34;
	buf[2] = 0;
	buf[3] = 0;	
	buf[4] = 0;
	buf[5] = mode;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	
	//��ϢID
	buf[1] = 0x35;
	buf[2] = 0;
	buf[3] = 0;	
	buf[4] = id>>8;
	buf[5] = id;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
	
}
/**
  * @brief  �����Ϣ��ѯ
  * @param  no�������
  * @retval ��������ϢID��
  * @note   
  */
u16 MotoMessageQuery(u8 Moto_No)
{
	u16 mes_id;	
	mes_id=(u16)Read_Fpga_Regist(0x8F09,Moto_No >>1);
	return mes_id;
}
/**
  * @brief  �����Ϣ���
  * @param  no�������
  * @retval ��������ϢID��
  * @note   
  */
void MotoMessageClear(u8 Moto_No)
{
	u8 buf[6] = {0};
	Cs_FPGA(Moto_No/2);
	if(Moto_No%2)//Y�� ż����
	{
		buf[0]=0X02;
	}
	else//X�� ������
	{
		buf[0]=0x00;
	}		
	//��ϢID
	buf[1] = 0x35;
	buf[2] = 0;
	buf[3] = 0;	
	buf[4] = 0;
	buf[5] = 0;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
}
/**
  * @brief  ������ֵsrc��nByte�ֽڣ�ת����u32
  * @param  src������ֵ
				nByte��ת���ֽ��� С�ڵ���4
				iBigEndian��1�����ģʽ��0��С��ģʽ
  * @retval 
  * @note   
  */
// 
u32 UartToU32(u8 * src,u8 nByte, u8 iBigEndian)
{
	u32 uldst=0;
	u16 i = 0;
	if(nByte>4)nByte=4;
	if (1 == iBigEndian)// ���ģʽ
	{
		for (i =0; i < nByte; i++)
		{
			uldst |= src[i]<<((nByte-i-1)*8);			
		}
	}
	else if (0 == iBigEndian)// С��ģʽ
	{
		for (i =0; i < nByte; i++)
		{
			uldst |= src[i]<<(i*8);			
		}
	}
	
	return uldst;
}
/**
  * @brief  �����ת����ǰ��
  * @param  *rxBuf�����ڻ���ָ��
				rxLen�����泤��
				Moto_No�������
  * @retval 
  * @note   
  */
// 
void MotorFwd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	u8 rdir;
	u32 variate_num;//������
	s32 up_limit,down_limit;
	CDV_INT64S num;	
	OS_ERR err;
	u8 type,sub_type[32];
	u32 pflag;
	u8 i;
	RET_STATUS ret =OPT_SUCCESS;
	type= rxBuf[0];
	up_limit =moto_par.up_limit[Moto_No]; //UartToU32(&moto_par.up_limit[Moto_No][0],4,BIGENDIAN);
   down_limit = -(CDV_INT64S)moto_par.down_limit[Moto_No];//UartToU32(&moto_par.down_limit[Moto_No][0],4,BIGENDIAN);
	gmoto_rzflag[Moto_No]=0;
	if(moto_par.speed_type[Moto_No] != TRAP_MODE)
	{
		moto_par.speed_type[Moto_No] = TRAP_MODE;
		Moto_Config(Moto_No,0x01000000,&moto_par.speed_type[Moto_No],0xFF,1);
	}
	if(gmoto_flag[Moto_No]==1)//�����һ����֮ͣ��û��������Ҫ���¶�ȡFPGA��ǰ������
	{
		gmoto_flag[Moto_No]=0;
		Para_len=Read_Moto_Par(Moto_No,0x1000);	
		gmoto_pos[Moto_No]=UartToU32(&Para_Buf[0],4,BIGENDIAN);
	}
//	if(moto_par.speed_type[Moto_No]==MULT_SPEEND_MODE)
//	{	
//		switch(type)
//		{
//			 case 0xFF:
//								num = (CDV_INT32S)UartToU32(&rxBuf[1],4,LITTLEEND);
//								if(num+gmoto_pos[Moto_No]<=up_limit)
//								{	
//									moto_sta.target_pos[Moto_No] = num+gmoto_pos[Moto_No];
//									ret=Motor_MultSpeen_Run(Moto_No ,num,1);	
//									rdir=0;
//									gmoto_pos_start[Moto_No]=gmoto_pos[Moto_No];
//									gmoto_pos[Moto_No]=gmoto_pos[Moto_No]+num;
//									gmoto_flag[Moto_No]=0;
//								}	
//								break;
//			 case 0xfe://�������ƶ�
//								variate_num=rxBuf[1];
//								num = VarGet(variate_num);
//								if(num+gmoto_pos[Moto_No]<=up_limit)
//								{	
//									moto_sta.target_pos[Moto_No] = num+gmoto_pos[Moto_No];
//									ret=Motor_MultSpeen_Run(Moto_No ,num,1);	
//									rdir=0;
//									gmoto_pos_start[Moto_No]=gmoto_pos[Moto_No];
//									gmoto_pos[Moto_No]=gmoto_pos[Moto_No]+num;
//									gmoto_flag[Moto_No]=0;
//								}	
//								break;			
//			default :
//							break;
//		}	
//	}
//	else
//	{				
//		gmoto_run_mode = moto_par.speed_type[Moto_No];
		switch(type) 
		{
			case 0xFF: 
								if(rxLen>10)//��Ҫͬʱ������������
								{
									pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
									num = (CDV_INT32S)UartToU32(&rxBuf[5],4,LITTLEEND);
									Moto_Config(Moto_No,pflag,rxBuf+9,type,0);								
								}
								else
								{
									num = (CDV_INT32S)UartToU32(&rxBuf[1],4,LITTLEEND);

								}
								break;
			case 0xfe://�������ƶ�
								if(rxLen>10)//��Ҫͬʱ������������
								{
									pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
									variate_num=UartToU32(&rxBuf[5],4,LITTLEEND);
									num = VarGet(variate_num);
									for(i=1;i*4<rxLen-10;i++)
									{					
										variate_num=UartToU32(&rxBuf[i*4+5],4,LITTLEEND);
										U32ToBuf(rxBuf+i*4+5,VarGet(variate_num),LITTLEEND);	
									}									
									Moto_Config(Moto_No,pflag,rxBuf+9,type,0);																	
								}
								else
								{				
									variate_num=UartToU32(&rxBuf[1],4,LITTLEEND);
									num = VarGet(variate_num);	
								}	
								break;
			case 0xFD://�������
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
								for(i=0;i*5<rxLen-10;i++)
								{
									sub_type[i] = rxBuf[i*5+5];
									if(i==0)
									{
										if(sub_type[i]==0xFF)//����
										{
											num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										}
										else if(sub_type[i]==0xFE)//����
										{
											variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
											num = VarGet(variate_num);
										}
									}
									else
									{
										if(sub_type[i]==0xFE)//����
										{
											variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
											U32ToBuf(rxBuf+i*5+6,VarGet(variate_num),LITTLEEND);
										}									
									
									}
								}
								Moto_Config(Moto_No,pflag,rxBuf+10,type,0);	
								break;
			default:
							break;
		}
		if(num+gmoto_pos[Moto_No]<=up_limit)
		{	
			moto_sta.target_pos[Moto_No] = num+gmoto_pos[Moto_No];
		  moto_sta.runtime[Moto_No]=MotoCountdown(&moto_par,Moto_No ,num);	
		  rdir=0;
		  gmoto_pos_start[Moto_No]=gmoto_pos[Moto_No];
		  gmoto_pos[Moto_No]=gmoto_pos[Moto_No]+num;
			ret=Motor_Relative_Run(Moto_No ,num,1);
		  //gmoto_pos[Moto_No]=moto_sta.target_pos[Moto_No];
		  gmoto_flag[Moto_No]=0;
		}	
//	}
}
/**
  * @brief  ����������ת����ǰ��
  * @param  *rxBuf�����ڻ���ָ��
				rxLen�����泤��
				Moto_No�������
  * @retval 
  * @note   
  */
// 
void MotorMultFwd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	u8 rdir;
	u32 variate_num;//������
	s32 up_limit,down_limit;
	CDV_INT64S num;
	static CDV_INT64S step_sum[AXLE_NUM] = {0};	
	OS_ERR err;
	u8 type,sub_type[32];
	u32 pflag;
	u8 i;
	RET_STATUS ret =OPT_SUCCESS;
	type= rxBuf[0];
	up_limit =moto_par.up_limit[Moto_No]; 
   down_limit = -(CDV_INT64S)moto_par.down_limit[Moto_No];
	gmoto_rzflag[Moto_No]=0;
	if(gmoto_flag[Moto_No]==1)//�����һ����֮ͣ��û��������Ҫ���¶�ȡFPGA��ǰ������
	{
		gmoto_flag[Moto_No]=0;
		Para_len=Read_Moto_Par(Moto_No,0x1000);	
		gmoto_pos[Moto_No]=UartToU32(&Para_Buf[0],4,BIGENDIAN);
	}
	if(FpgaMotoWait(Moto_No))//���н���
	{
		if(moto_par.speed_type[Moto_No] != MULT_SPEEND_MODE)
		{
			moto_par.speed_type[Moto_No] = MULT_SPEEND_MODE;
			Moto_Config(Moto_No,0x01000000,&moto_par.speed_type[Moto_No],0xFF,1);
		}
		moto_par.mult_speed_num[Moto_No] = 1;
		step_sum[Moto_No] = 0;
	}
	else if(moto_par.speed_type[Moto_No] == MULT_SPEEND_MODE)
	{
		moto_par.mult_speed_num[Moto_No]++;		
	}
	else
	{
		return;
	}
	switch(type) 
	{
		case 0xFF: 
							if(rxLen>10)//��Ҫͬʱ������������
							{
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
								num = (CDV_INT32S)UartToU32(&rxBuf[5],4,LITTLEEND);								
								Moto_MultSpeen_Config(Moto_No,pflag,rxBuf+5,type);
							}
							else
							{
								return;
							}
							break;
		case 0xfe://�������ƶ�
							if(rxLen>10)//��Ҫͬʱ������������
							{
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
								variate_num=UartToU32(&rxBuf[5],4,LITTLEEND);
								num = VarGet(variate_num);
								for(i=1;i*4<rxLen-10;i++)
								{					
									variate_num=UartToU32(&rxBuf[i*4+5],4,LITTLEEND);
									U32ToBuf(rxBuf+i*4+5,VarGet(variate_num),LITTLEEND);	
								}				
								Moto_MultSpeen_Config(Moto_No,pflag,rxBuf+5,type);
							}
							else
							{				
								variate_num=UartToU32(&rxBuf[1],4,LITTLEEND);
								num = VarGet(variate_num);	
							}	
							break;
		case 0xFD://�������
							pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
							for(i=0;i*5<rxLen-10;i++)
							{
								sub_type[i] = rxBuf[i*5+5];
								if(i==0)
								{
									if(sub_type[i]==0xFF)//����
									{
										num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
									}
									else if(sub_type[i]==0xFE)//����
									{
										variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										num = VarGet(variate_num);
									}
								}
								else
								{
									if(sub_type[i]==0xFE)//����
									{
										variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										U32ToBuf(rxBuf+i*5+6,VarGet(variate_num),LITTLEEND);
									}									
								
								}
							}
							Moto_MultSpeen_Config(Moto_No,pflag,rxBuf+6,type);	
							break;
		default:
						break;
	}
	if(num+gmoto_pos[Moto_No]<=up_limit)
	{	
		moto_sta.target_pos[Moto_No] = num+gmoto_pos[Moto_No];
		gmoto_pos_start[Moto_No]=gmoto_pos[Moto_No];
		gmoto_pos[Moto_No]=moto_sta.target_pos[Moto_No];
		gmoto_flag[Moto_No]=0;
		step_sum[Moto_No] += num;
		ret=Motor_MultSpeen_Run(Moto_No ,step_sum[Moto_No],1);	
		rdir=0;			
	}	

}
/**
  * @brief  ��u32����ת��u8������������
  * @param  *dst��Ŀ������/ָ��
				data��u32����ԭ����
				iBigEndian���洢��ʽ 1��� 0С��
  * @retval 
  * @note   
  */
//
void U32ToBuf(u8 *dst,u32 data, u8 iBigEndian)
{
	if(iBigEndian == BIGENDIAN)	//���
	{	
		*(dst+3) = (data&0x000000FF);
		*(dst+2) = (data&0x0000FF00)>>8;
		*(dst+1) = (data&0x00FF0000)>>16;
		*dst= (data&0xFF000000)>>24;		
	}
	else//С��
	{
		*dst = (data&0x000000FF);
		*(dst+1) = (data&0x0000FF00)>>8;
		*(dst+2) = (data&0x00FF0000)>>16;
		*(dst+3) = (data&0xFF000000)>>24;	

	}
}
/**
  * @brief  ��u16����ת��u8������������
  * @param  *dst��Ŀ������/ָ��
				data��u32����ԭ����
				iBigEndian���洢��ʽ 1��� 0С��
  * @retval 
  * @note   
  */
//
void U16ToBuf(u8 *dst,u32 data, u8 iBigEndian)
{
	if(iBigEndian == BIGENDIAN)	//���
	{	
		*(dst+1) = data&0x00FF;
		*dst= (data&0xFF00)>>8;		
	}
	else//С��
	{
		*dst = data&0x00FF;
		*(dst+1) = (data&0xFF00)>>8;

	}
}
/**
  * @brief  �����ת�����
  * @param  *rxBuf�����ڻ���ָ��
				rxLen�����泤��
				Moto_No�������
  * @retval 
  * @note   
  */
// 
void MotorRev(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	//u8 rdir;
	u32 variate_num;//������
	CDV_INT64S up_limit,down_limit;
	CDV_INT64S num;	
	OS_ERR err;
	u8 type,sub_type[32];
	u32 pflag;
	u8 i;
	RET_STATUS ret =OPT_SUCCESS;
	type= rxBuf[0];
	up_limit =moto_par.up_limit[Moto_No]; //UartToU32(&moto_par.up_limit[Moto_No][0],4,BIGENDIAN);
   down_limit = -(CDV_INT64S)moto_par.down_limit[Moto_No];//UartToU32(&moto_par.down_limit[Moto_No][0],4,BIGENDIAN);
	if(moto_par.speed_type[Moto_No]==MULT_SPEEND_MODE)
	{
		gmoto_rzflag[Moto_No]=0;	
//		gmoto_run_mode = MULT_SPEEND_MODE;
		if(gmoto_flag[Moto_No]==1)//�����һ����֮ͣ��û��������Ҫ���¶�ȡFPGA��ǰ������
		{
			gmoto_flag[Moto_No]=0;
			Para_len=Read_Moto_Par(Moto_No,0x1000);	
			gmoto_pos[Moto_No]=UartToU32(&Para_Buf[0],4,BIGENDIAN);
		}			
		switch(type)
		{
			 case 0xFF:
								num = (CDV_INT32S)UartToU32(&rxBuf[1],4,LITTLEEND);
								if(gmoto_pos[Moto_No]-num>=down_limit)
								{	
									moto_sta.target_pos[Moto_No] = gmoto_pos[Moto_No]-num;
									ret=Motor_MultSpeen_Run(Moto_No ,num,1);	
							//		rdir=0;
									gmoto_pos_start[Moto_No]=gmoto_pos[Moto_No];
									gmoto_pos[Moto_No]=gmoto_pos[Moto_No]+num;
									gmoto_flag[Moto_No]=0;
								}	
								break;
			 case 0xfe://�������ƶ�
								variate_num=rxBuf[1];
								num = VarGet(variate_num);
								if(gmoto_pos[Moto_No]-num>=down_limit)
								{	
									moto_sta.target_pos[Moto_No] = gmoto_pos[Moto_No]-num;
									ret=Motor_MultSpeen_Run(Moto_No ,num,1);	
//									rdir=0;
									gmoto_pos_start[Moto_No]=gmoto_pos[Moto_No];
									gmoto_pos[Moto_No]=gmoto_pos[Moto_No]+num;
									gmoto_flag[Moto_No]=0;
								}	
								break;			
			default :
							break;
		}	
	}
	else
	{				
//		gmoto_run_mode = TRAP_MODE;
		gmoto_rzflag[Moto_No]=0;
		if(gmoto_flag[Moto_No]==1)//�����һ����֮ͣ��û��������Ҫ���¶�ȡFPGA��ǰ������
		{
			gmoto_flag[Moto_No]=0;
			Para_len=Read_Moto_Par(Moto_No,0x1000);	
			gmoto_pos[Moto_No]=UartToU32(&Para_Buf[0],4,BIGENDIAN);
		}
		switch(type) 
		{
			case 0xFF: 
								if(rxLen>10)//��Ҫͬʱ������������
								{
									pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
									num = (CDV_INT32S)UartToU32(&rxBuf[5],4,LITTLEEND);
									Moto_Config(Moto_No,pflag,rxBuf+9,type,0);								
								}
								else
								{
									num = (CDV_INT32S)UartToU32(&rxBuf[1],4,LITTLEEND);

								}

								break;
			case 0xfe://�������ƶ�
								if(rxLen>10)//��Ҫͬʱ������������
								{
									pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
									variate_num=UartToU32(&rxBuf[5],4,LITTLEEND);
									num = VarGet(variate_num);
									for(i=1;i*4<rxLen-10;i++)
									{					
										variate_num=UartToU32(&rxBuf[i*4+5],4,LITTLEEND);
										U32ToBuf(rxBuf+i*4+5,VarGet(variate_num),LITTLEEND);	
									}		
									Moto_Config(Moto_No,pflag,rxBuf+9,type,0);																	
								}
								else
								{				
									variate_num=UartToU32(&rxBuf[1],4,LITTLEEND);
									num = VarGet(variate_num);	
								}
								break;
			case 0xFD://�������
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
								for(i=0;i*5<rxLen-10;i++)
								{
									sub_type[i] = rxBuf[i*5+5];
									if(i==0)
									{
										if(sub_type[i]==0xFF)//����
										{
											num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										}
										else if(sub_type[i]==0xFE)//����
										{
											variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
											num = VarGet(variate_num);
										}
									}
									else
									{
										if(sub_type[i]==0xFE)//����
										{
											variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
											U32ToBuf(rxBuf+i*5+6,VarGet(variate_num),LITTLEEND);
										}									
									
									}
								}
								Moto_Config(Moto_No,pflag,rxBuf+10,type,0);	
//								if(gmoto_pos[Moto_No]-num>=down_limit)
//								{	
//									moto_sta.target_pos[Moto_No] = num+gmoto_pos[Moto_No];
//								  moto_sta.runtime[Moto_No]=MotoCountdown(&moto_par,Moto_No ,num);																	
//								  ret=Motor_Relative_Run(Moto_No ,num,1);
//								  rdir=0;
//								  gmoto_pos[Moto_No]=gmoto_pos[Moto_No]+num;
//								  gmoto_flag[Moto_No]=0;
//								}	
								break;
			default:
							break;
		}	
		if(gmoto_pos[Moto_No]-num>=down_limit)
		{	
				moto_sta.target_pos[Moto_No] = gmoto_pos[Moto_No]-num;
		  moto_sta.runtime[Moto_No]=MotoCountdown(&moto_par,Moto_No ,num);
		  //rdir=0;
		  gmoto_pos_start[Moto_No]=gmoto_pos[Moto_No];
		  gmoto_pos[Moto_No]=moto_sta.target_pos[Moto_No];
		  ret=Motor_Relative_Run(Moto_No ,num,0);
		  gmoto_flag[Moto_No]=0;
		}	
	}
}
/**
  * @brief  �������ٷ�ת�����
  * @param  *rxBuf�����ڻ���ָ��
				rxLen�����泤��
				Moto_No�������
  * @retval 
  * @note   
  */
// 
void MotorMultRev(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	//u8 rdir;
	u32 variate_num;//������
	s32 up_limit,down_limit;
	CDV_INT64S num;
	static CDV_INT64S step_sum[AXLE_NUM] = {0};	
	OS_ERR err;
	u8 type,sub_type[32];
	u32 pflag;
	u8 i;
	RET_STATUS ret =OPT_SUCCESS;
	type= rxBuf[0];
	up_limit =moto_par.up_limit[Moto_No]; 
   down_limit = -(CDV_INT64S)moto_par.down_limit[Moto_No];
	gmoto_rzflag[Moto_No]=0;
	if(gmoto_flag[Moto_No]==1)//�����һ����֮ͣ��û��������Ҫ���¶�ȡFPGA��ǰ������
	{
		gmoto_flag[Moto_No]=0;
		Para_len=Read_Moto_Par(Moto_No,0x1000);	
		gmoto_pos[Moto_No]=UartToU32(&Para_Buf[0],4,BIGENDIAN);
	}
	if(FpgaMotoWait(Moto_No))//���н���
	{
		if(moto_par.speed_type[Moto_No] != MULT_SPEEND_MODE)
		{
			moto_par.speed_type[Moto_No] = MULT_SPEEND_MODE;
			Moto_Config(Moto_No,0x01000000,&moto_par.speed_type[Moto_No],0xFF,1);
		}
		moto_par.mult_speed_num[Moto_No] = 1;
		step_sum[Moto_No] = 0;
	}
	else if(moto_par.speed_type[Moto_No] == MULT_SPEEND_MODE)
	{
		moto_par.mult_speed_num[Moto_No]++;		
	}
	else
	{
		return;
	}
	switch(type) 
	{
		case 0xFF: 
							if(rxLen>10)//��Ҫͬʱ������������
							{
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
								num = (CDV_INT32S)UartToU32(&rxBuf[5],4,LITTLEEND);								
								Moto_MultSpeen_Config(Moto_No,pflag,rxBuf+5,type);
							}
							else
							{
								return;
							}
							break;
		case 0xfe://�������ƶ�
							if(rxLen>10)//��Ҫͬʱ������������
							{
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
								variate_num=UartToU32(&rxBuf[5],4,LITTLEEND);
								num = VarGet(variate_num);
								for(i=1;i*4<rxLen-10;i++)
								{					
									variate_num=UartToU32(&rxBuf[i*4+5],4,LITTLEEND);
									U32ToBuf(rxBuf+i*4+5,VarGet(variate_num),LITTLEEND);	
								}				
								Moto_MultSpeen_Config(Moto_No,pflag,rxBuf+5,type);
							}
							else
							{				
								variate_num=UartToU32(&rxBuf[1],4,LITTLEEND);
								num = VarGet(variate_num);	
							}	
							break;
		case 0xFD://�������
							pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
							for(i=0;i*5<rxLen-10;i++)
							{
								sub_type[i] = rxBuf[i*5+5];
								if(i==0)
								{
									if(sub_type[i]==0xFF)//����
									{
										num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
									}
									else if(sub_type[i]==0xFE)//����
									{
										variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										num = VarGet(variate_num);
									}
								}
								else
								{
									if(sub_type[i]==0xFE)//����
									{
										variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										U32ToBuf(rxBuf+i*5+6,VarGet(variate_num),LITTLEEND);
									}									
								
								}
							}
							Moto_MultSpeen_Config(Moto_No,pflag,rxBuf+6,type);	
							break;
		default:
						break;
	}
	if(gmoto_pos[Moto_No]-num>=down_limit)
	{	
		moto_sta.target_pos[Moto_No] = gmoto_pos[Moto_No]-num;
		gmoto_pos_start[Moto_No]=gmoto_pos[Moto_No];
		gmoto_pos[Moto_No]=moto_sta.target_pos[Moto_No];
		gmoto_flag[Moto_No]=0;
		step_sum[Moto_No] += num;
		ret=Motor_MultSpeen_Run(Moto_No ,step_sum[Moto_No],0);	
		//rdir=0;			
	}	
}
/**
  * @brief  ����Ƶ�
  * @param  *rxBuf�����ڻ���ָ��
				rxLen�����泤��
				Moto_No�������
  * @retval 
  * @note   
  */
// 
void MotorMoveTo(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	u8 rdir;
	u32 variate_num;//������
	s32 up_limit,down_limit;
	CDV_INT64S num;	
	OS_ERR err;
	u8 type,sub_type[32];
	u32 pflag;
	u8 i;
	RET_STATUS ret =OPT_SUCCESS;
	type= rxBuf[0];
	up_limit =moto_par.up_limit[Moto_No]; //UartToU32(&moto_par.up_limit[Moto_No][0],4,BIGENDIAN);
   down_limit = -(CDV_INT64S)moto_par.down_limit[Moto_No];//UartToU32(&moto_par.down_limit[Moto_No][0],4,BIGENDIAN);
	gmoto_rzflag[Moto_No]=0;
	if(gmoto_flag[Moto_No]==1)
	{
		gmoto_flag[Moto_No]=0;
	}	
	switch(type) 
	{
		case 0xFF:  
							//num = (CDV_INT32S)UartToU32(&rxBuf[1],4,LITTLEEND);
							if(rxLen>10)//��Ҫͬʱ������������
							{
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
								num = (CDV_INT32S)UartToU32(&rxBuf[5],4,LITTLEEND);
								Moto_Config(Moto_No,pflag,rxBuf+9,type,0);								
							}
							else
							{
								num = (CDV_INT32S)UartToU32(&rxBuf[1],4,LITTLEEND);

							}
							break;
		case 0xFE://ʹ�ñ���ֵ
//							variate_num=rxBuf[1];
//							num = VarGet(variate_num);
							if(rxLen>10)//��Ҫͬʱ������������
							{
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
								variate_num=UartToU32(&rxBuf[5],4,LITTLEEND);
								num = VarGet(variate_num);
								for(i=1;i*4<rxLen-10;i++)
								{					
									variate_num=UartToU32(&rxBuf[i*4+5],4,LITTLEEND);
									U32ToBuf(rxBuf+i*4+5,VarGet(variate_num),LITTLEEND);	
								}		
								Moto_Config(Moto_No,pflag,rxBuf+9,type,0);																	
							}
							else
							{				
								variate_num=UartToU32(&rxBuf[1],4,LITTLEEND);
								num = VarGet(variate_num);	
							}							
							break;
			case 0xFD://�������
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
								for(i=0;i*5<rxLen-10;i++)
								{
									sub_type[i] = rxBuf[i*5+5];
									if(i==0)
									{
										if(sub_type[i]==0xFF)//����
										{
											num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										}
										else if(sub_type[i]==0xFE)//����
										{
											variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
											num = VarGet(variate_num);
										}
									}
									else
									{
										if(sub_type[i]==0xFE)//����
										{
											variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
											U32ToBuf(rxBuf+i*5+6,VarGet(variate_num),LITTLEEND);
										}									
									
									}
								}
								Moto_Config(Moto_No,pflag,rxBuf+10,type,0);	
								break;
		default:
				 break;
	}	
	if((num<=up_limit)&&(num>=down_limit)&&Moto_RunSta[Moto_No]==READY)
	{	
		moto_sta.target_pos[Moto_No] = num;
		gmoto_pos[Moto_No]=num;
//		MotoMessageSet(Moto_No,10000,0,11);
		Moto_Solute_Run(Moto_No,num);
		
	}	
}
/**
  * @brief  �������
  * @param  *rxBuf�����ڻ���ָ��
				rxLen�����泤��
				Moto_No�������
  * @retval 
  * @note   
  */
// 
void MotorRtnZero(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	u8 rdir;
	u32 variate_num;//������
	s32 up_limit,down_limit;
	s32 num;	
	OS_ERR err;
	u8 type,sub_type[32];
	u32 pflag;
	u8 i;
	RET_STATUS ret =OPT_SUCCESS;
	type= rxBuf[0];
//	if(moto_par.speed_type[Moto_No]==MULT_SPEEND_MODE)
//	{	
////		gmoto_run_mode = MULT_SPEEND_MODE;
//	}
//	else
//	{				
//		gmoto_run_mode = TRAP_MODE;
	
		switch(type) 
		{
			case 0xFF: 
								if(rxLen>10)//��Ҫͬʱ������������
								{
									pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
									
									Moto_Config(Moto_No,pflag,rxBuf+5,type,0);								
								}
								break;
			case 0xfe://�������ƶ�
								if(rxLen>10)//��Ҫͬʱ������������
								{
									pflag = UartToU32(&rxBuf[1],4,LITTLEEND);
									for(i=0;i*4<rxLen-10;i++)
									{					
										variate_num=UartToU32(&rxBuf[i*4+5],4,LITTLEEND);
										U32ToBuf(rxBuf+i*4+5,VarGet(variate_num),LITTLEEND);	
									}		
									Moto_Config(Moto_No,pflag,rxBuf+5,type,0);																	
								}
								break;
			case 0xFD://�������
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
								for(i=0;i*5<rxLen-10;i++)
								{
									sub_type[i] = rxBuf[i*5+5];	
									if(sub_type[i]==0xFE)//����
									{
										variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										U32ToBuf(rxBuf+i*5+6,VarGet(variate_num),LITTLEEND);
									}		
								}
								Moto_Config(Moto_No,pflag,rxBuf+5,type,0);	
								break;
			default:
							break;
		}
		moto_sta.ela_runtime[Moto_No] = GetCdvTimeTick();
		ret = Motor_Run_Zero(Moto_No);
		gmoto_pos[Moto_No]=0;
		gmoto_rzflag[Moto_No]=1;
		gmoto_flag[Moto_No]=0;
		gmoto_suspend_flag[Moto_No]=0;	
//					
//	}
}
/**
  * @brief  ������Χ����
  * @param  *Data�����ݵ�ַ
				Max�����ֵ
				Min����Сֵ
  * @retval 
  * @note   
  */
// 
void ParRangeCtrl(CDV_INT32U* Data, CDV_INT32U Max,CDV_INT32U Min) 
{
	if(*Data>Max)
		*Data = Max;
	else if(*Data<Min)
		*Data = Min;
}

/**
  * @brief  �����Դ������ȡ
  * @param  
  * @retval 
  * @note   
  */
// 
void MotoResParRead(void)
{
	CDV_INT08U* buf = NULL;
	CDV_INT16U len;
	CDV_INT32U addr = SCRIP_RES(0);
	CDV_INT08U i = 0;
	CDV_INT08U AxleNum;
	
	SPI_Flash_Read((CDV_INT08U *)&moto_par, MOTOPAR_ADDR, sizeof(moto_par));
	
	SPI_Flash_Read((CDV_INT08U *)&line_par, MOTO_LINE_PAR_ADDR, sizeof(line_par));
	SPI_Flash_Read((CDV_INT08U *)&arc_par, MOTO_ARC_PAR_ADDR, sizeof(arc_par));
	
	if (!addr)
		return;
	
  SPI_Flash_Read((CDV_INT08U *)&len, addr, 4);
	NEW08U(buf, len);
	#if 0
	SPI_Flash_Read((CDV_INT08U *)buf, addr + 4, len);
	AxleNum = buf[0];
	for(i=0;i<AxleNum;i++)
	{
		int len = 0;
		int nbyte = 1;
		moto_par.drvtype[i] = UartToU32(buf+i+1,nbyte,LITTLEEND);
		len+=nbyte;
		nbyte = 4;
		moto_par.max_speed[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;
		moto_par.min_speed[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;
		moto_par.up_limit[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;
		moto_par.down_limit[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;
		nbyte = 1;
		moto_par.zero_dir[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;
		moto_par.pstv_dir[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;	
		nbyte = 2;
		moto_par.div_num[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;			
		nbyte = 1;
		moto_par.pul_mode[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;	
		nbyte = 4;
		moto_par.max_acc[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;		
		moto_par.min_acc[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;		
		nbyte = 1;
		moto_par.zero_mode[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;	
		moto_par.run_mode[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;	
		moto_par.encoder_type[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);
		len+=nbyte;		
		moto_par.backlash_comp[i] = UartToU32(buf+nbyte*i+len*AxleNum+1,nbyte,LITTLEEND);				
	}
	#else
	SPI_Flash_Read((CDV_INT08U *)&moto_par, addr + 5, len);
	//AxleNum = buf[0];
	#endif
	DELETE(buf);
}



/**
  * @brief  ����S���ٶ�����
  * @param  
  * @retval speed:ʵʱ�ٶ�
  * @note   
  */

CDV_INT32U CalculateSModelLine(CDV_INT08U Moto_No)
{
	CDV_FP32 deno ;
	CDV_FP32 melo ;
	CDV_INT32U speed_max = moto_par.target_speed[Moto_No];
	CDV_INT32U speed_min = moto_par.min_speed[Moto_No];
	CDV_INT32U delt = speed_max-speed_min;
	CDV_INT32U acc = moto_par.acc[Moto_No];
	CDV_INT32U cnt = gAccAndDec_Cnt[Moto_No];//cnt���Ӽ��ٹ����еĵڼ���ʱ��㣬һ��ʱ���Ϊ5ms
	CDV_INT32U speed;
	CDV_FP32 len; 
	if(Moto_RunSta[Moto_No]==ACC)	
		len = (CDV_FP32)gAcc_Times[Moto_No];	//5ms�ı�һ���ٶ�
	else if(Moto_RunSta[Moto_No]==DEC)	
		len = (CDV_FP32)gDec_Times[Moto_No];	//5ms�ı�һ���ٶ�
//	else if(Moto_RunSta[Moto_No]==UNIFORM)	
//		return speed_max*FPGA_ACC_DIV;
	if(gAccAndDec_Cnt[Moto_No]>=(CDV_INT32U)len)
	{
		if(Moto_RunSta[Moto_No]==ACC)			
			return speed_max*FPGA_ACC_DIV;
		else if(Moto_RunSta[Moto_No]==DEC)	
			return speed_min*FPGA_ACC_DIV;	
	}
//	melo = moto_par.SModel_flexible[Moto_No] * (cnt-len/2) / (len/2);//SModel_flexible����S�������䣨Խ�����ѹ�������������м䣨x����0����Χ�����ٶ�Խ��ԽСԽ�ӽ��ȼ��١������S���ߵ�ȡֵΪ4-6
//	deno = 1.0 / (1 + expf(-melo));   
//	if(Moto_RunSta[Moto_No]==ACC)	//����
//		speed = (delt * deno + speed_min)*FPGA_ACC_DIV;
//	else if(Moto_RunSta[Moto_No]==DEC)		//����
//		speed = (speed_max - delt * deno)*FPGA_ACC_DIV;
	if(Moto_RunSta[Moto_No]==ACC)	//����
		melo = moto_par.SModel_flexible[Moto_No] * (cnt-len/2) / (len/2);//SModel_flexible����S�������䣨Խ�����ѹ�������������м䣨x����0����Χ�����ٶ�Խ��ԽСԽ�ӽ��ȼ��١������S���ߵ�ȡֵΪ4-6
	else if(Moto_RunSta[Moto_No]==DEC)		//����
		melo = moto_par.SModel_flexible[Moto_No] * (len/2-cnt) / (len/2);
	deno = 1.0 / (1 + expf(-melo));   	
	speed = (delt * deno + speed_min)*FPGA_ACC_DIV;
	return speed;
}

 
CDV_INT32U CalculateSModelLine_Par(CDV_INT32U speed_max, CDV_INT32U speed_min,CDV_INT32U  acc, CDV_FP32 flexible,CDV_INT32U cnt,CDV_INT08U State)
{
	CDV_FP32 deno ;
	CDV_FP32 melo ;
	CDV_INT32U delt = speed_max-speed_min;
	CDV_FP32 len = delt/(FPGA_SPEED_CHANGE_CYCLE*acc);	//5ms�ı�һ���ٶ�
	CDV_INT32U speed;
	if(cnt>=(CDV_INT32U)len)
	{
		if(State)			
			return speed_max*FPGA_ACC_DIV;
		else
			return speed_min*FPGA_ACC_DIV;	
	}
	melo = flexible * (cnt-len/2) / (len/2);
	deno = 1.0 / (1 + expf(-melo));   
	if(State)//����
		speed = (delt * deno + speed_min)*FPGA_ACC_DIV;
	else	//����
		speed = (speed_max - delt * deno)*FPGA_ACC_DIV;;
	return speed;
}

/**
  * @brief  ʵʱ�ٶ�����
* @param  Moto_No:�����
  * @retval 
  * @note   
  */
// 
RET_STATUS RealTimeSpeedSet(CDV_INT08U Moto_No)
{
	RET_STATUS ret =OPT_FAILURE;
	OS_ERR err;
	CDV_INT08U buf[6]; 
	CDV_INT32U cnt = 0;
	CDV_INT32U speed ;
	CDV_INT32U ela_runtime;//������ʱ��
	CDV_INT32U pos;
	CDV_INT32U running_step;//�����в���
	CDV_INT32U run_step;//��Ҫ���в���
	if(moto_par.speed_type[Moto_No] ==S_MODE)
	{
		if(Moto_RunSta[Moto_No]!=READY)
		{
			ela_runtime = CalcTimeMS(GetCdvTimeTick(),gStageStartup_Time[Moto_No]);
			gAccAndDec_Cnt[Moto_No] = ela_runtime/(FPGA_SPEED_CHANGE_CYCLE*1000);
//			pos = Read_Fpga_Regist(0x8F03+Moto_No%2,Moto_No>>1);
//			running_step = abs(pos-gmoto_pos_start[Moto_No]);
//			run_step = abs(gmoto_pos[Moto_No]-gmoto_pos_start[Moto_No]);
		}
		if(Moto_RunSta[Moto_No]==READY)
			return OPT_SUCCESS;
		else if(Moto_RunSta[Moto_No]==ACC)
		{
			if(gAccAndDec_Cnt[Moto_No]>=gAcc_Times[Moto_No])//||running_step>=(run_step-moto_par.DecStep[Moto_No]))
			{								
				gStageStartup_Time[Moto_No] = GetCdvTimeTick();	
				if(gUniform_Times[Moto_No]==0)
				{
					Moto_RunSta[Moto_No]=DEC;
				}
				else
				{
					Moto_RunSta[Moto_No]=UNIFORM;
					speed = Moto_Speed_Con(moto_par.target_speed[Moto_No]);	
					if((Moto_No&0x01)==0)
						buf[0]=0x00;
					else if((Moto_No&0x01)==1)
						buf[0]=0x02; 
					buf[1]=0x06;
					buf[2]=speed>>24;
					buf[3]=speed>>16;			
					buf[4]=speed>>8;
					buf[5]=speed;
					Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
				}
				return OPT_FAILURE;
			}
		}
		else if(Moto_RunSta[Moto_No]==UNIFORM)
		{
			if(gAccAndDec_Cnt[Moto_No]>=gUniform_Times[Moto_No])
			{
				Moto_RunSta[Moto_No]=DEC;
				gStageStartup_Time[Moto_No] = GetCdvTimeTick();
			}
			return OPT_FAILURE;
		}
		else if(Moto_RunSta[Moto_No]==DEC)
		{
			if(gAccAndDec_Cnt[Moto_No]>=gDec_Times[Moto_No])
			{
				Moto_RunSta[Moto_No]=READY;
				speed = Moto_Speed_Con(moto_par.min_speed[Moto_No]);	
				if((Moto_No&0x01)==0)
					buf[0]=0x00;
				else if((Moto_No&0x01)==1)
					buf[0]=0x02; 
				buf[1]=0x06;
				buf[2]=speed>>24;
				buf[3]=speed>>16;			
				buf[4]=speed>>8;
				buf[5]=speed;
				Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
				return OPT_FAILURE;
			}
		}
		speed = CalculateSModelLine(Moto_No);	
		if((Moto_No&0x01)==0)
			buf[0]=0x00;
		else if((Moto_No&0x01)==1)
			buf[0]=0x02; 
		buf[1]=0x06;
		buf[2]=speed>>24;
		buf[3]=speed>>16;			
		buf[4]=speed>>8;
		buf[5]=speed;
		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	}
	return ret;
}

#endif