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
u8 gmoto_run_flag[AXLE_NUM] = {0};//电机运行标志 1正在运行 0就绪
u8 gmoto_flag[AXLE_NUM] = {0};
u8 gmoto_rzflag[AXLE_NUM] = {0};//归零标记，有归零命令就置1
u8 gmoto_suspend_flag[AXLE_NUM] = {0};//电机暂停标记，有暂停命令就置1
u32 gAccAndDec_Cnt[AXLE_NUM] = {0};//S型速度曲线使用 加减速次数统计 5ms进行一次加减速
u32 gAcc_Times[AXLE_NUM] = {0};//S型速度曲线使用 加速总次数
u32 gDec_Times[AXLE_NUM] = {0};//S型速度曲线使用 减速总次数
u32 gUniform_Times[AXLE_NUM] = {0};//S型速度曲线使用 匀速总次数
u32 gStageStartup_Time[AXLE_NUM] = {0};//各阶段开始时刻
u32 bufmax_speed;
u8 gmoto_flag2;//电机加减速不满足梯形曲线标记
u8 gmoto_run_mode = TRAP_MODE;//电机运行模式
u16 wait_err=0;
u32 xc=0;//圆心坐标x
u32 yc=0;//圆心坐标y
u32 xee=0;//终点坐标x
u32 yee=0;//终点坐标y
_PT xyc;
_PT xyc_temp[2];
u16 FPGA_STATE=0;
#define line_qua 0x7fffffff

//******编码器读取测试*****
u16 bmq_count_L=0;//编码器计数低16
u16 bmq_count_H=0;//编码器计数高16
u32 bmq_count=0;//编码器计数32
//*************************
u16 mes_id;
u32 mes_pos;
void y2x(u32 xo,u32 yo,u32 xe,u32 ye,_PT xyc,u8 Moto_NO);//坐标转换

//void FPGA_Send_parameter(u8 *buf,u8 no);
//RET_STATUS Line_Interpolation(u8* rxBuf,u8 rxLen, u8 debug);
void MOTO_Enable_Disable(u8 Moto_No,u8 typ);
u8 Moto_Config(u8 Moto_No,u32 Moto_Pflag,u8* rxBuf,u8 type,u8 storage);
RET_STATUS MotorStop(CDV_INT08U Moto_No );//电机急停
u8 Read_Moto_Par(u8 Moto_No,u32 pflag);
RET_STATUS Motor_Jog_Run(CDV_INT08U Moto_No ,u8 type,u8 state);//电机点动 
u8 Moto_Solute_Run(u8 Moto_No,u32 steps);
RET_STATUS Motor_Relative_Run(CDV_INT08U Moto_No , CDV_INT32S num,u8 type);
RET_STATUS Motor_Run_Zero(CDV_INT08U Moto_No);//电机归零
u32 Moto_Acc_Con(u32 user_data,u32 max_speed,u32 start_speed);//加速度转换
float Moto_Speed_Con(u32 user_data);//运行速度转换
void MOTO_Enable_Disable(u8 Moto_No,u8 typ);//使能、失能
u32 Moto_Dec_Con(u32 user_data,u32 max_speed,u32 min_speed);//减速度转换
u32 Get_Dstep(u32 max_speed,u32 min_speed,u32 dec_speed);//减速步数
u8 wait_moto_finish(u8 MOTO_num,u8 FPGA_NUM);
u8 MOTO_ACC_DEC(u8 Moto_No,u32 run_step,MOTOR_INFO *moto_pared);
RET_STATUS Motor_Stop(CDV_INT08U Moto_No);
void Cs_FPGA(u8 no);//片选FPGA
RET_STATUS Arc_Interpolation(u8* rxBuf,u8 rxLen, CMD_ARG *arg);//圆弧插补
u8 Read_Arc_Par(u8 Fpga_No,u16 pflag);//圆弧插补参数读取
u32 Arc_Runstep(u8 fpga_no ,u32 xo,u32 yo,u32 xe,u32 ye,u8 dir);//圆弧插补步数计算
u8 Arc_Run(u8* rxBuf);
RET_STATUS Line_Interpolation(u8* rxBuf,u8 rxLen, CMD_ARG *arg);//直线插补
u8 Line_Config(u8 Fpga_no,u16 Moto_Pflag,u8* rxBuf,u8 type);//直线插补参数设置
u8 Read_Line_Par(u8 Fpga_No,u16 pflag);//直线插补参数读取
u8 Xc_Yc(float x1,float y1,float x2,float y2,float r, u8 type);//求圆心坐标
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
//主轴参数设置
//Moto_pflag:参数标记
//no:电机号0-7
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
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	 Cs_FPGA(Moto_No/2);
	 if(Moto_Pflag & 0x0001)//驱动类型
	 { 
			i=0;
			moto_par.drvtype[Moto_No]=rxBuf[i];
			i=i+1;
	 }		 
	 if(Moto_Pflag & 0x0002)//最高速度
	 {
//		  for(j=0;j<4;j++)
//		  {
//		    moto_par.max_speed[Moto_No][3-j]=rxBuf[i+j];
//			}
		   moto_par.max_speed[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			i=i+4;
	 }
	 if(Moto_Pflag & 0x0004)//最低速
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
//	 if(Moto_Pflag & 0x0008)//加速度
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
//	 if(Moto_Pflag & 0x0010)//减速度
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
	 if(Moto_Pflag & 0x0020)//上限位
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
	 if(Moto_Pflag & 0x0040)//下限位
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
	 if(Moto_Pflag & 0x0080)//回零方向
	 {
		  if(rxBuf[i]==2)//回零方向取反
			{
				moto_par.zero_dir[Moto_No]= moto_par.zero_dir[Moto_No]? 0 : 1;				
			}	
			else
			{	
				moto_par.zero_dir[Moto_No]=rxBuf[i];
			}	
			i=i+1;
	 }
	 if(Moto_Pflag & 0x0100)//运动方向
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
	 if(Moto_Pflag & 0x0200)//细分数
	 {
//		  for(j=0;j<2;j++)
//		  {
//		    moto_par.div_num[Moto_No][1-j]=rxBuf[i+j];
//			}	
		   moto_par.div_num[Moto_No] = UartToU32(rxBuf+i,2,LITTLEEND);
			i=i+2;
	 }
	 if(Moto_Pflag & 0x0400)//分度圆直径
	 {
//		  for(j=0;j<2;j++)
//		  {
//		    moto_par.diameter[Moto_No][1-j]=rxBuf[i+j];
//			}	
		   moto_par.diameter[Moto_No] = UartToU32(rxBuf+i,2,LITTLEEND);
			i=i+2;
	 }
//	 if(Moto_Pflag & 0x0800)//丝杠导程
//	 {
//		  for(j=0;j<2;j++)
//		  {
//		    moto_par.lead[Moto_No][j]=rxBuf[i+j];
//			}	
//			i=i+2;
//	 }
	 if(Moto_Pflag & 0x0800)//归零速度
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
			//同时设置目标速度  目前FPGA部分无归零速度
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
	 
	 if(Moto_Pflag & 0x1000)//点动运行速度
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
//	 if(Moto_Pflag & 0x2000)//CW/CCW单双脉冲模式切换
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
//	 if(Moto_Pflag & 0x4000)//多段速参数设置
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
	 
	 
	 if((Moto_Pflag & 0x00010000)&&(Moto_RunSta[Moto_No]==READY))//目标运行速度
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
			if(!(Moto_Pflag & 0x0008))//只更改速度时，也需要对加减速进行重新计算
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
	 if(Moto_Pflag & 0x00020000)//最大加速度
	 {

			moto_par.max_acc[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			i=i+4;
	 }	
	 if(Moto_Pflag & 0x00040000)//最小加速度
	 {

			moto_par.min_acc[Moto_No] = UartToU32(rxBuf+i,4,LITTLEEND);
			i=i+4;
	 }		 
	 if(Moto_Pflag & 0x0008)//加速度
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
	 if(Moto_Pflag & 0x0010)//减速度
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
	 if(Moto_Pflag & 0x00080000)//回零模式
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
	 if(Moto_Pflag & 0x00100000)//运动模式
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
	 if(Moto_Pflag & 0x00200000)//编码器类型
	 {
		 moto_par.encoder_type[Moto_No] = rxBuf[i];
			i=i+1;
	 }	
	 if(Moto_Pflag & 0x00400000)//背隙补偿
	 {
			moto_par.backlash_comp[Moto_No] = rxBuf[i];
			i=i+1;
	 }	  	
	 if(Moto_Pflag & 0x00800000)//CW/CCW单双脉冲模式切换
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
	 if(Moto_Pflag & 0x01000000)//速度曲线类型
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
		//**减速步数***//
	buf_data=Get_Dstep(moto_par.target_speed[Moto_No],moto_par.min_speed[Moto_No],moto_par.dec[Moto_No]);
	if(moto_par.run_mode[Moto_No]==1)//闭环
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
//读取电机参数
//
//no:电机号（0-7）
//****************************************************
u8 Read_Moto_Par(u8 Moto_No,u32 pflag)
{
	u8 i,j,k;
	u32 FPGA_POS;//当前位置
	u32 FPGA_Speed;//当前速度
	u32 FPGA_RunTime;//已运行时间
	u32 FPGA_ResTime;//剩余时间
	u16 FPGA_POSH;
	u16 FPGA_POSL;
	u32 ela_runtime;
	i=0;
	Cs_FPGA(Moto_No/2);
	 if(pflag & 0x0001)//驱动类型
	 { 
			Para_Buf[i]=moto_par.drvtype[Moto_No];
			i=i+1;			
	 }
	 if(pflag & 0x0002)//最大速度
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.max_speed[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.max_speed[Moto_No],BIGENDIAN);			
			i=i+4;
	 }
	 if(pflag & 0x0004)//最小速度
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.min_speed[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.min_speed[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0008)//加速度
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.acc[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.acc[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0010)//减速度
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.dec[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.dec[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0020)//上限位
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.up_limit[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.up_limit[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0040)//下限位
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.down_limit[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.down_limit[Moto_No],BIGENDIAN);
			i=i+4;
	 }
	 if(pflag & 0x0080)//回零方向
	 {
			Para_Buf[i]=moto_par.zero_dir[Moto_No];
			i=i+1;
	 }
	 if(pflag & 0x0100)//运行方向
	 {
			Para_Buf[i]=moto_par.pstv_dir[Moto_No];
			i=i+1;
	 }
	 if(pflag & 0x0200)//细分数
	 {
//			for(j=0;j<2;j++)
//			{
//				Para_Buf[i+j]=moto_par.div_num[Moto_No][j];
//			}
			U16ToBuf(Para_Buf+i,moto_par.div_num[Moto_No],BIGENDIAN);
			i=i+2;
	 }
	 if(pflag & 0x0400)//分度圆直径
	 {
//			for(j=0;j<2;j++)
//			{
//				Para_Buf[i+j]=moto_par.diameter[Moto_No][j];
//			}
			U16ToBuf(Para_Buf+i,moto_par.diameter[Moto_No],BIGENDIAN);
			i=i+2;
	 }
	 if(pflag & 0x0800)//丝杠导程
	 {
//			for(j=0;j<2;j++)
//			{
//				Para_Buf[i+j]=moto_par.lead[Moto_No][j];
//			}
			U16ToBuf(Para_Buf+i,moto_par.lead[Moto_No],BIGENDIAN);
			i=i+2;
	 }
	 if(pflag & 0x1000)//当前位置
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
	 //bit1:x轴完成
	 //bit2:y轴完成
	 //bit3:圆弧完成
	 //bit4:x轴上限位
	 //bit5:x轴下限位
	 //bit6:y轴上限位
	 //bit7:y轴下限位
	 //bit8:直线插补完成
	 //bit9:x轴零位
	 //bit10:y轴零位
	 if(pflag & 0x2000)//硬上限位信号
	 {
		 FPGA_POSL=Read_Fpga_Regist(0x8F00,Moto_No>>1);
		 if(Moto_No%2==0)
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>3);//FPGA_POSL的第四位
			 
		 }
         else if(Moto_No%2==1)		 
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>5);//FPGA_POSL的第6位
			 
		 }	 
		 i=i+1;
	 }	 
	 if(pflag & 0x4000)//硬下限位信号
	 {
		 FPGA_POSL=Read_Fpga_Regist(0x8F00,Moto_No>>1);
		 if(Moto_No%2==0)
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>4);//FPGA_POSL的第5位
			 
		 }
     else if(Moto_No%2==1)		 
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>6);//FPGA_POSL的第7位
			 
		 }
		 i=i+1;
	 }
	 if(pflag & 0x8000)//硬回零信号
	 {
		 FPGA_POSL=Read_Fpga_Regist(0x8F00,Moto_No>>1);
		 if(Moto_No%2==0)
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>8);//FPGA_POSL的第9位
			 
		 }
     else if(Moto_No%2==1)		 
		 {	 
				Para_Buf[i]=0x00001&(FPGA_POSL>>9);//FPGA_POSL的第10位
			 
		 }
		 i=i+1;
	 }
	 if(pflag & 0x00010000)//目标运行速度
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.target_speed[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.target_speed[Moto_No],BIGENDIAN);
			i=i+4;
	 }	 	 
	 if(pflag & 0x00020000)//最大加速度
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.max_acc[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.max_acc[Moto_No],BIGENDIAN);
			i=i+4;
	 }	
	 if(pflag & 0x00040000)//最小加速度
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_par.min_acc[Moto_No][j];
//			}
			U32ToBuf(Para_Buf+i,moto_par.min_acc[Moto_No],BIGENDIAN);
			i=i+4;
	 }		 
	 if(pflag & 0x00080000)//回零模式
	 {
//			for(j=0;j<1;j++)
//			{
				Para_Buf[i]=moto_par.zero_mode[Moto_No];
//			}
			
			i=i+1;
	 }	  	 
	 if(pflag & 0x00100000)//运动模式
	 {
//			for(j=0;j<1;j++)
//			{
				Para_Buf[i]=moto_par.run_mode[Moto_No];
//			}
			i=i+1;
	 }	  	 	 
	 if(pflag & 0x00200000)//编码器类型
	 {
//			for(j=0;j<1;j++)
//			{
				Para_Buf[i]=moto_par.encoder_type[Moto_No];
//			}
			i=i+1;
	 }	
	 if(pflag & 0x00400000)//背隙补偿
	 {
//			for(j=0;j<1;j++)
//			{
				Para_Buf[i]=moto_par.backlash_comp[Moto_No];
//			}
			i=i+1;
	 }	  	
	 if(pflag & 0x00800000)//CW/CCW单双脉冲模式切换
	 {
		 Para_Buf[i] = moto_par.pul_mode[Moto_No];
		 i=i+1;
	 }	
	 if(pflag & 0x01000000)//当前速度
	 {
		 FPGA_Speed=Read_Fpga_Regist(0x8F07+(Moto_No%2),Moto_No>>1);
		FPGA_Speed = FPGA_Speed/FPGA_ACC_DIV;
//		FPGA_Speed=(CDV_INT32U)((CDV_INT64U)FPGA_W_FRE_DIV2*(CDV_INT64U)FPGA_ACC_DIV/(CDV_INT64U)FPGA_Speed);
		if(FPGA_Speed<50)FPGA_Speed=0;

		U32ToBuf(Para_Buf+i,FPGA_Speed,BIGENDIAN);		
		i=i+4;		
	 }	  	 
 	  if(pflag & 0x02000000)//多段速参数读取
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
	 if(pflag & 0x04000000)//速度曲线类型读取
	 {
		 Para_Buf[i] = moto_par.speed_type[Moto_No];
		 i=i+1;
	 }	
	 if(pflag & 0x08000000)//预计运行时间读取
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=moto_sta.runtime[Moto_No]>>(8*(3-j));
			}		 
		 i=i+4;
	 }	
	 if(pflag & 0x10000000)//已运行时间
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
	 if(pflag & 0x20000000)//目标位置
	 {
//			for(j=0;j<4;j++)
//			{
//				Para_Buf[i+j]=moto_sta.target_pos[Moto_No]>>(8*(3-j));
//			}		 
			U32ToBuf(Para_Buf+i,moto_sta.target_pos[Moto_No],BIGENDIAN);	
		 i=i+4;
	 }			 
	 if(pflag & 0x40000000)//使能/失能状态
	 {
	   	Para_Buf[i] = moto_sta.enable[Moto_No];
		   i++;
	 }
	return i;//返回长度
}	
//**************************************************************
//电机参数初始化
//Moto_No:电机号
//Moto_Pflag:参数标记
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
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	 
	// U32ToBuf(moto_par.encoder_pre[Moto_No],1000*4,LITTLEEND);//编码器精度设为1000  4倍频
	moto_par.encoder_pre[Moto_No] = 1000*4;
	Clear_GratingRuler(Moto_No);
	 Cs_FPGA(Moto_No/2);
	 if(Moto_Pflag & 0x0002)//最高速度
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
	 if(Moto_Pflag & 0x0004)//最低速
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
//	 if(Moto_Pflag & 0x0008)//加速度
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
//	 if(Moto_Pflag & 0x0010)//减速度
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
	 if(Moto_Pflag & 0x0100)//运动方向
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
	 if(Moto_Pflag & 0x0800)//归零速度
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
	 if(Moto_Pflag & 0x1000)//点动运行速度
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
	 if(Moto_Pflag & 0x00010000)//目标运行速度
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
	 if(Moto_Pflag & 0x00020000)//最大加速度
	 {

	 }	
	 if(Moto_Pflag & 0x00040000)//最小加速度
	 {

	 }		 
	 if(Moto_Pflag & 0x0008)//加速度
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
	 if(Moto_Pflag & 0x0010)//减速度
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
	 if(Moto_Pflag & 0x00080000)//回零模式
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
	 if(Moto_Pflag & 0x00100000)//运动模式
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
	 if(Moto_Pflag & 0x00200000)//编码器类型
	 {

	 }	
	 if(Moto_Pflag & 0x00400000)//背隙补偿
	 {

	 }	  	
	 if(Moto_Pflag & 0x00800000)//CW/CCW单双脉冲模式切换
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
	  if(Moto_Pflag & 0x01000000)//速度曲线类型
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
		//**减速步数***//
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
//电机绝对运行
//Moto_No:电机号（0-7）
//steps:脉冲数
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
		{//设定要走的步数
			len	= steps - FPGA_POS;
	  }else 
		    {
			    len = FPGA_POS - steps;
		    }
//		if(moto_par.pstv_dir[Moto_No]==1) {//如果用户设定了翻转方向
//				dir = dir? 0 :1;
//			}		
			 dir = dir?moto_par.pstv_dir[Moto_No]:!moto_par.pstv_dir[Moto_No];
		if(0 != len) 
		{
			moto_sta.runtime[Moto_No]=MotoCountdown(&moto_par,Moto_No ,len);	
			MOTO_ACC_DEC(Moto_No,len,& moto_par); 
			/*绝对移动*/
			//移动方向
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
//			步长
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
	

			//编码器
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
//		     /*归零*/

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
//电机向前/向后运动
//Moto_No：电机号（0-7）
//num：向前步数
//type：1向前，0向后
//************************************************
RET_STATUS Motor_Relative_Run(CDV_INT08U Moto_No , CDV_INT32S num,u8 type) 
{	
#ifndef	CDV_V1
	u8 dir=type? 1:0;
//	u8 buf[6]={0};
	u8 buf[6]={1,1,1,1,1,1};	
	u32 len;
//	OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	Cs_FPGA(Moto_No/2);

	len = num;	
	if(len!=0)
	{	
//		if(moto_par.pstv_dir[Moto_No]==1)//如果用户设定了翻转方向
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
		/*相对移动*/
		MOTO_ACC_DEC(Moto_No,len,& moto_par);

		//移动方向

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
		
//		步长
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
		
		//编码器
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
//电机向前/向后点动
//Moto_No：电机号（0-7）
//type：1向前，0向后
//state=1启动，state=0停止
//************************************************
RET_STATUS Motor_Jog_Run(CDV_INT08U Moto_No ,u8 type,u8 state) 
{	
#ifndef	CDV_V1
	u8 dir=type? 1:0;
	u8 buf[6]={0};
//	OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	Cs_FPGA(Moto_No/2);
//		if(moto_par.pstv_dir[Moto_No]==1)//如果用户设定了翻转方向
//		{
//			dir = dir? 0 :1;
//		}
		 dir = dir?moto_par.pstv_dir[Moto_No]:!moto_par.pstv_dir[Moto_No];
		//移动方向
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
		if(state==0||gmoto_flag[Moto_No]==1)//停止点动或者上一次暂停之后没继续，获取当前位置，更新当前位置至目标位置  
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
//归零
//Moto_No:电机号
//
//*****************************************************************
RET_STATUS Motor_Run_Zero(CDV_INT08U Moto_No) 
{
	 u8 buf[6]={0};	
 // u16 FPGA_DATA;
  
//OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
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
//单轴多段速参数设置
//Moto_No:电机号0-7
//返回值：成功，1；失败，0
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
		buf_data=Moto_Speed_Con(moto_par.mult_speed[Moto_No][j]);//速度转化
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
			buf_data=Moto_Acc_Con(moto_par.mult_a_d_speed[Moto_No][0],moto_par.mult_speed[Moto_No][0],min_speed);//第一段加速度转化
		}
		else
		{
			buf_data=Moto_Acc_Con(moto_par.mult_a_d_speed[Moto_No][j],moto_par.mult_speed[Moto_No][j],moto_par.mult_speed[Moto_No][j-1]);//第j段加减速度转化 
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
	buf_data=Moto_Speed_Con(moto_par.mult_speed[Moto_No][(m_speed_num-1)]);//速度转化
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
	
	

	if(Moto_Pflag & 0x0008)//加速度
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
	
	if(Moto_Pflag & 0x0010)//减速度
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

   buf_data=Get_Dstep(moto_par.mult_speed[Moto_No][m_speed_num-1],moto_par.min_speed[Moto_No],moto_par.dec[Moto_No]);//减速步数
	if(moto_par.run_mode[Moto_No]==1)//闭环
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
//单轴多段速参数设置
//no:电机号0-7
//返回值：成功，1；失败，0
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
		buf_data=Moto_Speed_Con(moto_par.mult_speed[Moto_No][j]);//速度转化
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
			buf_data=Moto_Acc_Con(moto_par.mult_a_d_speed[Moto_No][0],moto_par.mult_speed[Moto_No][0],min_speed);//第一段加速度转化
		}
		else
		{
			buf_data=Moto_Acc_Con(moto_par.mult_a_d_speed[Moto_No][j],moto_par.mult_speed[Moto_No][j],moto_par.mult_speed[Moto_No][j-1]);//第j段加减速度转化 
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
//电机向前/向后多段速运动
//Moto_No：电机号（0-7）
//num：向前步数
//type：1向前，0向后
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
			/*绝对移动*/
			//移动方向
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
			//步长
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
	

			//编码器		
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
//多段速运动继续运行
//Moto_No：电机号（0-7）
//************************************************
RET_STATUS Motor_MultSpeen_Continue(CDV_INT08U Moto_No) 
{		
	u32 pos_now; 
	u8 rdir;
	u32 RemainingSteps;//剩余步数
	u32 StepsHaveRun;//已运行步数
	u8 Mult_Num_Now;//当前段数
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
//电机使能/失能
//Moto_No:电机号
//typ:1使能，0失能
//**************************************************
void MOTO_Enable_Disable(u8 Moto_No,u8 typ)
{
	u8 buf[6];
//	OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
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
//电机暂停
//Moto_No：电机号（0-7）
//************************************************
RET_STATUS Motor_Stop(CDV_INT08U Moto_No) 
{	
#ifndef	CDV_V1
	u8 buf[6]={0};
//	OS_ERR err;
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	Cs_FPGA(Moto_No/2);

		//暂停命令
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
//电机运行速度转换
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
//电机速度格式化
//将速度按分辨率转化成FPGA能够输出的频率
//**************************************************************************
u32 Moto_Speed_Format(u32 user_data)
{
	u32 FPGA_Data;

	FPGA_Data = FPGA_W_FRE_DIV2/user_data;
	FPGA_Data = FPGA_W_FRE_DIV2/FPGA_Data;
	return FPGA_Data;
}

//***************************************
//加速度单位转换用户输入单位为 脉冲/s^2
//max:最大速度，单位 脉冲/s
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
//减速度单位转换用户输入单位为 脉冲/s^2
//max:最大速度，单位 脉冲/s
//***************************************
//u32 Moto_Dec_Con(u32 user_data,u32 max_speed,u32 min_speed)
//{

//	u32 FPGA_Data;
//	FPGA_Data = Moto_Acc_Con(user_data,max_speed,min_speed);
//	return FPGA_Data;
//}

//**************************************
//计算减速步数
//max:最大速度
//min：最小速度
//d_speed:减速度
//返回减速步数
//*************************************
u32 Get_Dstep(u32 max_speed,u32 min_speed,u32 dec_speed)
{
	u32 i;
	u32 count;
	float dec_data;
	
	double b;	
	 double c= 0;	//2017-2-8 彭从锋修改
	double a=min_speed;
	dec_data=(float)dec_speed;
	dec_data = dec_data==0?1:dec_data;
	#if 1

		count=(u32)(abs(max_speed-min_speed)/(dec_data*FPGA_SPEED_CHANGE_CYCLE));//速度变化次数
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
			count=(u16)((FPGA_W_FRE_DIV2/min_speed-FPGA_W_FRE_DIV2/max_speed)/dec_data);//速度变化次数
		}
	  else
	  {
			count=(u16)((FPGA_W_FRE_DIV2/max_speed-FPGA_W_FRE_DIV2/min_speed)/dec_data);//速度变化次数
		}		
		for(i=0;i<count;i++)
		{
			b=FPGA_W_FRE_DIV2*FPGA_SPEED_CHANGE_CYCLE/a;  //5ms进行一次速度变化    b=(10^7/a)*5*10^(-3)
			a=a+dec_data; 
				
			c=c+b;
		}	
	#endif
	return (u32)c;
}

/**
  * @brief  电机查询
  *  
  * @param  Moto_No     要操作的电机号0-7
	*         opt    0 等待运行完成
	*                1 
	*                2  
  *   
  * @retval 
  *
  * @note   查询电机到位，速度，加速度等
	*         从modbus寄存器中读RW_MOTOR。maxSpeed，acc，dec，upLimit，downLimit，minSpeed（由联机状态时设定好）
	*                           R_MOTOR_POS (变成主要维护值，可读写，每次发送运动命令成功后更改该值)
	*         发送命令给FPGA
  */
RET_STATUS MotorChk(CDV_INT08U Moto_No , CDV_INT08U opt) 
{
//	CDV_INT08U val;
OS_ERR err;

	RET_STATUS ret =OPT_SUCCESS;
#ifndef	CDV_V1
//	if(1 != READ_CDV_DIP(Moto_No>>1))//判断配置的是不是电机
//		return OPT_SUCCESS;
	
	switch (opt) {
		case 0:
			     /*等待运行完成*/
		       //__nop();//__NOP;
					OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 5ms
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
/******通用串口测试，注释一下****/								
//								for(i=0;i<6;i++)
//								{
//									MotorStop(i);
//								}
//								while(1)
//								{
//							       ;
//							    }	
							}	
//							OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 5ms
			            }
			     break;
	  default:
			      break;
	    }
#endif
	return ret;
}
/**
  * @brief  电机急停
  *  
  * @param  Moto_No     要操作的电机号
  *         num    0    归零
  *                非零 绝对运动目标位置(脉冲)
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
//	  OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	Cs_FPGA(Moto_No/2);
//		//步长高位
//		if((Moto_No&0x01)==0)
//			buf[0]=0x08;
//		else if((Moto_No&0x01)==1)
//			buf[0]=0x1c;
//		buf[1] = 0;
//		buf[2] =0;
//		Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);

//		//步长低位
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
   AccStep=Get_Dstep(max_speed,min_speed,acc);//得到加速需要的步数
	 if(AccStep<run_step)//如果加速步数小于总行程
	 { 
      DecStep=Get_Dstep(max_speed,min_speed,dec);//得到减速需要的步数
		  if((AccStep+DecStep)<run_step)//如果加速步数+减速步数小于总步数
			{	
				  if(gmoto_flag2==1)//如果速度、加速度、减速度修改过，就要改回来
				  {
						  //减速步数
							DecStep=Get_Dstep(max_speed,min_speed,dec);
							if(moto_par.run_mode[Moto_No]==1)//闭环
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
				  ret = 0;//返回减速步数 
			}	
			else//如果加速步数+减速步数大于总步数
			{	
				 if(gmoto_flag2==0)bufmax_speed=max_speed;				
				// DecStep=run_step/2; //减速步数=1/2总行程
					DecStep=(CDV_INT64U)run_step*acc/(dec+acc);
					//发送减速步数
					if(moto_par.run_mode[Moto_No]==1)//闭环
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
   else//如果加速步数大于总行程
   {
		  if(gmoto_flag2==0)bufmax_speed=max_speed;
		 
			//DecStep=run_step/2;//减速步数=1/2总行程
			DecStep=(CDV_INT64U)run_step*acc/(dec+acc);
			//发送减速步数
			if(moto_par.run_mode[Moto_No]==1)//闭环
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
//等待moto运动完成
//MOTO_num:第几号电机1:X 轴  2：Y轴   3：圆弧
//FPGA_num:第几号FPGA（1-4）
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
  * @brief  全部电机控制
  *  
  * @param  ctl    控制代号，0：暂停，1：继续
  *   
  * @retval 返回值说明
  *
  * @note   不可重入
  */

void AllMotorCTL(CDV_INT08U ctl) {
	CDV_INT08U i = 0;
	static CDV_INT08U all_motor_ctl = 0;
	OS_ERR err;
	  
	  
//	if (0 == ctl)
//  	all_motor_ctl = 0;
	OSMutexPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	for(i = 0; i < 8 ; i++) {
		if(1 != READ_CDV_DIP(i>>1))//判断配置的是不是电机
		  continue;
		
		if(OPT_SUCCESS == MotorChk(i , 0)) {
			if (1 == ctl && 1 == BIT_READ(all_motor_ctl , i)) {
				//all_motor_ctl = 0;
			  BIT_RESET(all_motor_ctl , i);
			  Moto_Solute_Run(i,gmoto_pos[i]);//继续
			}
		} else {
			if (0 == ctl && 0 == BIT_READ(all_motor_ctl , i)) {
				//all_motor_ctl = 1;
			  BIT_SET(all_motor_ctl , i);
			  Motor_Stop(i);//暂停
			}
		}
	}
	OSMutexPost (&MOTO_SEM,OS_OPT_POST_NO_SCHED,&err);
	if (1 == ctl)
  	all_motor_ctl = 0;
}

/**
  * @brief  电机脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明
            OPT_SUCCESS命令执行完成
            OPT_FAILURE命令未执行完成，比如等待未等到
  *
  * @note   资源号（00-FF）	00绝对运动	    FF	uint32数值低位	uint32数值	 uint32数值	uint32数值高位			字符串
																				    FE	变量代号高位	  变量代号低位							
														01等待运动完成	ED									                                            字符串
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
	u8 variate_num;//变量号
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
	if( (opt>=0x10)&&(opt<0x14) )//直线插补
	{
		ret=Line_Interpolation(rxBuf,rxLen,arg);
	}	
	else if( (opt>=0x20)&&(opt<0x25) )//圆弧插补
	{
		ret=Arc_Interpolation(rxBuf,rxLen,arg);
	}	
	else if( (opt>=0x30)&&(opt<=0x35) )//光栅尺或编码器	
	{
		ret=GratingRuler_CmdParse(rxBuf,rxLen,arg);
	}
	else//单轴
	{	
		rxBuf = rxBuf+3;//由于资源号由1byte改为4byte，为了方便下部分代码不做修改 对rxBuf指针进行移位
		pflag = UartToU32(&rxBuf[3],4,LITTLEEND);
		switch(opt) {
			case 0x00://移到
								if(Moto_RunSta[Moto_No]==READY)
									MotorMoveTo(rxBuf+2,rxLen,Moto_No);
								break;
			case 0x01://等待运行完成	
								//delay_ms(100);
								ret = MotorChk(Moto_No , 0);
								break;
			case 0x02://归零
								if(Moto_RunSta[Moto_No]==READY)
									MotorRtnZero(rxBuf+2,rxLen,Moto_No);
								break;
			case 0x03://向前	
								if(Moto_RunSta[Moto_No]==READY)
									MotorFwd(rxBuf+2,rxLen,Moto_No);
								break;
			case 0x04://向后
									if(Moto_RunSta[Moto_No]==READY)
									MotorRev(rxBuf+2,rxLen,Moto_No);					
							break;
			case 0x05://暂停/继续
								if((rxBuf[3] & 0x01)==1)//暂停
								{
									ret=Motor_Stop(Moto_No);
									gmoto_flag[Moto_No]=1;
									gmoto_suspend_flag[Moto_No]=1;
									Moto_RunSta[Moto_No] = READY;
									gAccAndDec_Cnt[Moto_No] = 0;
								}	
								else if(gmoto_rzflag[Moto_No]==1)//如果归零过程中暂停就继续归零
								{
									if(gmoto_suspend_flag[Moto_No]==1) 
									{	
										ret = Motor_Run_Zero(Moto_No);
										gmoto_pos[Moto_No]=0;
									}	
								}						
								else//继续运行
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
			case 0x06://失能/使能
								MOTO_Enable_Disable(Moto_No,rxBuf[3]);
								break;
			case 0x07://参数设置 
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
			case 0x08://参数读取
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
														buf=buf+(Para_Buf[i]<<(8*(Para_len-i-1)));//buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
													}
													ValSet(rxBuf[5],buf);

													break; 
							 }								
							 break;
			case 0x09://点动
//							if((rxBuf[3]&0x01)==1)//点动向前
//							 {	 									
									ret=Motor_Jog_Run(Moto_No ,rxBuf[3]&0x01,rxBuf[3]>>1);
									
//							 }	 
//							 else if((rxBuf[3]&0x01)==0)//点动向后
//							 {	 
//									ret=Motor_Jog_Run(Moto_No ,0,rxBuf[3]>>1);
//									moto_sta.ela_runtime[Moto_No] = GetCdvTimeTick();
//								}								 
					     break;			
			case 0x0A://多段速向前	
							if(Moto_RunSta[Moto_No]==READY)
								 MotorMultFwd(rxBuf+2,rxLen,Moto_No);							 
					     break;	
			case 0x0B://多段速向后	
							if(Moto_RunSta[Moto_No]==READY)
								 MotorMultRev(rxBuf+2,rxLen,Moto_No);														 
					     break;									
		  default:
							break;
			
		}
	}	
	//OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
	if(OPT_FAILURE == ret) {
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 5ms
	}
	return ret;
}

//*****************************************************
//直线插补
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
	u8 Fpga_No;//电机组号
	u8 Moto_Function;//电机功能（设置参数，写参数，启动。。）
	u32 Moto_Pflag;//电机参数标记
	u8 type;
	
	//OS_ERR err;
	//OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	
	Fpga_No=UartToU32(&rxBuf[0],4,LITTLEEND)-8;//rxBuf[0]-8;
	Moto_Function=rxBuf[4];//rxBuf[1];
	type=rxBuf[5];//rxBuf[2];
	Moto_Pflag=UartToU32(&rxBuf[6],4,LITTLEEND);//(rxBuf[3]<<8)+rxBuf[4];
	rxBuf = rxBuf+3;
	Cs_FPGA(Fpga_No);
	switch(Moto_Function)
	{
		case 0x10: //电机参数设置
	            Line_Config(Fpga_No ,Moto_Pflag ,rxBuf+5,0);
		          break;
		case 0x11://电机参数读取
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
													val_buf=val_buf+Para_Buf[i]<<(8*(Para_len-i-1));//buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
												}
												ValSet(rxBuf[5],val_buf);

												break; 
						  }								
			        break;
		case 0x12://电机移动
				
						//终点坐标
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
						//启动命令
						buf[0]=0x04;
						buf[1]=0x00;
						buf[2]=0x00;
						buf[0]=0x00;
						buf[1]=0x00;
						buf[2]=0x51;						
						Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);						
			        break;
		case 0x13://电机等待
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
//直线插补参数设置
//no:轴组号：0~3
//Moto_Pflag:参数标记位
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
	 
	 if(Moto_Pflag & 0x0002)//最大速度
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
	 if(Moto_Pflag & 0x0004)//最低速度
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
	 if(Moto_Pflag & 0x0008)//加速度
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
	 if(Moto_Pflag & 0x0010)//减速度
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
	 
	 if(Moto_Pflag & 0x0400)//直线插补象限 已经不使用
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
		//**减速步数***//
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
//直线插补参数读取
//pflag:参数标记
//Fpga_No:轴组（0-3）
//****************************************************
u8 Read_Line_Par(u8 Fpga_No,u16 pflag)
{
	u8 i=0;
	u8 j=0;
	Cs_FPGA(Fpga_No);
	 if(pflag & 0x0002)//速度
	 { 
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=line_par.max_speed[Fpga_No][j];
			}
			i=i+4;			
	 }
	 if(pflag & 0x0004)//最低速度
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=line_par.min_speed[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0008)//加速度
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=line_par.acc[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0010)//减速度
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=line_par.dec[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0400)//象限
	 {
				Para_Buf[i+j]=line_par.quadrant[Fpga_No][j];
			  i=i+1;
	 }
	return i;//返回长度
}	
//*****************************************************
//圆弧插补
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
	u8 Fpga_No;//电机组号
	u8 Moto_Function;//电机功能（设置参数，写参数，启动。。）
	u32 Moto_Pflag;//电机参数标记
	u8 para_pos=5;//电机参数位置
	u8 type;
	Fpga_No=UartToU32(&rxBuf[0],4,LITTLEEND)-8;//rxBuf[0]-8;
	Moto_Function=rxBuf[4];//rxBuf[1];
	type=rxBuf[5];//rxBuf[2];
	Moto_Pflag=UartToU32(&rxBuf[6],4,LITTLEEND);//(rxBuf[3]<<8)+rxBuf[4];
	rxBuf = rxBuf+3;
	Cs_FPGA(Fpga_No);
	switch(Moto_Function)
	{
		case 0x20: //运行参数设置
	
							 if(Moto_Pflag & 0x0002)//最大速度
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
							 if(Moto_Pflag & 0x0004)//最低速度
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
							 if(Moto_Pflag & 0x0008)//加速度
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
							 if(Moto_Pflag & 0x0010)//减速度
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
							  //**减速步数***//
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
		case 0x21://电机参数读取
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
													val_buf=val_buf+Para_Buf[i]<<(8*(Para_len-i-1));//buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
												}
												ValSet(rxBuf[5],val_buf);

												break; 
						  }								
			        break;
		case 0x22://运行
			        Arc_Run(rxBuf);
							break;
		case 0x23://圆弧等待
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
//圆弧运行
//pflag:参数标记
//no:轴组（0-3）
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
	u8 Fpga_No;//电机组号
	u16 Moto_Pflag;//电机参数标记
	u8 para_pos=5;//电机参数位置
	Fpga_No=rxBuf[0]-8;
	Moto_Pflag=(rxBuf[3]<<8)+rxBuf[4];
	
	if((Moto_Pflag & 0x0001)==0)//如果终点坐标没设置
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
		//X轴终点坐标
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
		//Y轴终点坐标
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
	if(Moto_Pflag & 0x0002)//如果设置圆心坐标
	{	
		xc=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		if((xc>>31)==1)
		{
			xc=0x80000000|(-xc);
		}
//		buf[0]=0x42;//圆心x高
//		buf[1]=xc>>24;
//		buf[2]=xc>>16;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
//		buf[0]=0x43;//圆心x低
//		buf[1]=xc>>8;
//		buf[2]=xc;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		buf[0]=0x06;//圆心x
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
//		buf[0]=0x44;//圆心y高
//		buf[1]=yc>>24;
//		buf[2]=yc>>16;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);	

//		buf[0]=0x45;//圆心y低
//		buf[1]=yc>>8;
//		buf[2]=yc;
//		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		buf[0]=0x06;//圆心Y
		buf[1]=0x03;		
		buf[2]=yc>>24;
		buf[3]=yc>>16;
		buf[4]=yc>>8;
		buf[5]=yc;
		Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);	
		i=i+4;
	}	
	if((Moto_Pflag & 0x0004))//方向设置
	{
		dir=rxBuf[para_pos+i];
		i=i+1;
	}	
	if((Moto_Pflag & 0x0008))//如果半径设置
	{
		rr=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		i=i+4;
	}	
	if((Moto_Pflag & 0x0010))//弧长小于半圆
	{
		cw=rxBuf[para_pos+i];
		i=i+1;
	}
	if((Moto_Pflag & 0x0020))//第三点
	{
		tx=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		i=i+4;
	  ty=((rxBuf[para_pos+i]<<24)|(rxBuf[para_pos+i+1]<<16)|(rxBuf[para_pos+i+2]<<8)|rxBuf[para_pos+i+3]);
		i=i+4;
	}
	if((Moto_Pflag & 0x0040))//弦切角
	{
		angle=rxBuf[para_pos+i];
		i=i+1;
	}
//	FPGA_POSH=Read_Fpga_Regist(0xd6,Fpga_No+1);
//  FPGA_POSL=Read_Fpga_Regist(0xd7,Fpga_No+1);
//  xo =(FPGA_POSH<<16)+FPGA_POSL;//x当前位置

//  FPGA_POSH=Read_Fpga_Regist(0xd8,Fpga_No+1);
//  FPGA_POSL=Read_Fpga_Regist(0xd9,Fpga_No+1);
//  yo=(FPGA_POSH<<16)+FPGA_POSL;//y当前位置
	xo = Read_Fpga_Regist(0x8F03,Fpga_No);//x当前位置
	yo = Read_Fpga_Regist(0x8F04,Fpga_No);//x当前位置
	if((Moto_Pflag & 0x0002)==0)//如果圆心坐标没设置
	{
			if((Moto_Pflag & 0x0008)==0)//如果半径座标也没设置
			{
					if((Moto_Pflag & 0x0020)==0)//如果第三点也没设置
					{
							 if((Moto_Pflag & 0x0040)==0)//如果圆心角也没设置
							 {
								 return 0;
							 }
							 else//设置了圆心角度设置了
							 { 				
								 //求出xc、yc
								 xyc=CircleCentreFromTangent((f64) xo, (f64) yo, (f64) xe, (f64) ye, (f64)angle,cw);
								 y2x(xo,yo,xe,ye,xyc,Fpga_No);
							} 
								 								

					}
					else//第三点坐标设置le
					{
							 if((tx>xo))
							 {
								 if(ty>yo)
								 {	 
										dir=(1^arc_par.rel_dir[Fpga_No/2][0])&arc_par.rel_dir[Fpga_No/2][1];//逆时针
								 }
								 else
								 {
										dir=(0^arc_par.rel_dir[Fpga_No/2][0])&arc_par.rel_dir[Fpga_No/2][1];//顺时针
								 }								 
							 }
							 else if(ty>yo)
							 {
									dir=(0^arc_par.rel_dir[Fpga_No/2][0])&arc_par.rel_dir[Fpga_No/2][1];//顺时针
							 }
							 else
							 {
									dir=(1^arc_par.rel_dir[Fpga_No/2][0])&arc_par.rel_dir[Fpga_No/2][1];//逆时针
							 }							 
            // }							 
						 xyc=CircleCentreFromThirdPoint((f64) xo, (f64) yo, (f64) xe, (f64) ye, (f64) tx, (f64) ty); 
					   y2x(xo,yo,xe,ye,xyc,Fpga_No);					
					}	
			}
			else//如果半径长度设置le 
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
                if(dir==0)//方向顺时针
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
									//方向
									buf[0]=0x06;
									buf[1]=0x01;
									buf[2]=0x00;									
									buf[3]=0x00;
									buf[4]=0x00;
									buf[5]=0x01;
									Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
									i=i+1;
								}	
								else //逆时针
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
										//方向
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
	else//设置了圆心坐标
	{
		if((xc==0)&&(yc==0))//圆心坐标不对
		{
			return 1;
		}	
			
	}	
  if((Moto_Pflag & 0x0008)==0)//如果半径座标也没设置
	{	
			if(dir==0)//方向顺时针
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
				//方向
				buf[0]=0x06;
				buf[1]=0x01;
				buf[2]=0x00;									
				buf[3]=0x00;
				buf[4]=0x00;
				buf[5]=0x01;
				Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
				i=i+1;
			}	
			else //逆时针
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
					//方向
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
	 //运行步数
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
	 //启动命令
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
//圆弧插补参数读取
//pflag:参数标记
//Fpga_No:轴组（0-3）
//****************************************************
u8 Read_Arc_Par(u8 Fpga_No,u16 pflag)
{
	u8 i=0;
	u8 j=0;
	 if(pflag & 0x0002)//速度
	 { 
		  for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=arc_par.max_speed[Fpga_No][j];
			}	
			i=i+4;			
	 }
	 if(pflag & 0x0004)//最低速度
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=arc_par.min_speed[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0008)//加速度
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=arc_par.acc[Fpga_No][j];
			}
			i=i+4;
	 }
	 if(pflag & 0x0010)//减速度
	 {
			for(j=0;j<4;j++)
			{
				Para_Buf[i+j]=arc_par.dec[Fpga_No][j];
			}
			i=i+4;
	 }
	return i;//返回长度
}	
/*************************************************************
*计算圆弧要走的步数
*xo，yo：圆心坐标
*xee，yee：终点坐标
**************************************************************/
u32 Arc_Runstep(u8 Fpga_No ,u32 xo,u32 yo,u32 xe,u32 ye,u8 dir)
 {
  u32 run_step=0;
	u32 arc_r;//半径
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
    //运行一圈
			run_step=arc_r*8;
		  xstep=arc_r*4;
		  ystep=arc_r*4;
	}	
  else
	{	
		//起点坐标象限判断 
		if((r_xo & line_qua)==0)//Y轴上
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
    else if((r_yo & line_qua)==0)//X轴上
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
		else //不在X轴也不在Y轴上
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
		
		//终点坐标象限判断 
    if((r_xe & line_qua)==0)//Y轴上
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
    else if((r_ye & line_qua)==0)//X轴上
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
		else //不在X轴也不在Y轴上
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
//轴组参数初始化
//Fpga_No:轴组号：0~3
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
	  //直线 
	   //最高速度
			Max_Speed=(line_par.max_speed[Fpga_No][0]<<24)+(line_par.max_speed[Fpga_No][1]<<16)+(line_par.max_speed[Fpga_No][2]<<8)+line_par.max_speed[Fpga_No][3];
      Speed_Data=Moto_Speed_Con(Max_Speed); 
			buf[0]=0x04;
			buf[1]=0x05;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		 //最低速
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
	  //加速度  
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
	  //减速度
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
		//减速步数
			Speed_Data=Get_Dstep(Max_Speed,Min_Speed,Dec_Speed);
			buf[0]=0x04;
			buf[1]=0x07;
			buf[2]=Speed_Data>>24;
			buf[3]=Speed_Data>>16;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);	
	  //直线插补象限 停用
//			buf[0]=0x37;
//			buf[1]=0;
//			buf[2]=line_par.quadrant[Fpga_No][0];
//			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
			i=i+1;
	 //X轴正方向
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
   //Y轴正方向
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
  //圆弧
	   //最高速度
			Max_Speed=(arc_par.max_speed[Fpga_No][0]<<24)+(arc_par.max_speed[Fpga_No][1]<<16)+(arc_par.max_speed[Fpga_No][2]<<8)+arc_par.max_speed[Fpga_No][3];
      Speed_Data=Moto_Speed_Con(Max_Speed); 
			buf[0]=0x06;
			buf[1]=0x08;
			buf[2]=0;
			buf[3]=0;	
			buf[4]=Speed_Data>>8;
			buf[5]=Speed_Data;
			Send_parameter(Fpga_No,buf,FPGA_COMMAND_LEN);
		 //最低速
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
	  //加速度  
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
	  //减速度
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
	 //减速步数
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
//求圆心坐标
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

	if(y1-y2==0)//两坐标点在同一条水平线上
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

	//然后解这个方程组：
	//yc=ya+k*(xc-xa);
	//(yc-ya)^2+(xc-xa)^2=s/4;  
	
  //(k*(xc-xa))^2+(xc-xa)^2=s/4;
	//(k^2*(xc-xa)^2)+(xc-xa)^2=s/4;
	//(k^2*(xc^2+xa^2-2xc*xa))+(xc-xa)^2=s/4;
	//(k^2*xc^2+k^2*xa^2-k^2*2xc*xa))+(xc-xa)^2=s/4;
  //k^2*xc^2 +k^2*xa^2 -k^2*2xc*xa +xc^2 +xa^2 -2xc*xa=s/4;
	
	    
	//解出来：
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
		
		if((XC1*XC1+YC1*YC1)>(XC2*XC2+YC2*YC2))//默认选离绝对坐标原点近的圆心
		{
			if(type>=1)//默认选离绝对坐标原点近的圆心
			{
				xc=XC1;
				yc=YC1;
			}	
			else//默认选离绝对坐标原点远的圆心
			{
				xc=XC2;
				yc=YC2;
			}	
		}	
		else
		{
			if(type>=1)//默认选离绝对坐标原点近的圆心
			{
				xc=XC2;
				yc=YC2;
			}	
			else//默认选离绝对坐标原点远的圆心
			{
				xc=XC1;
				yc=YC1;
			}
		}	
	}
	else
	{	
		return 0;//失败
	}	
	return 1;//成功 
}
//************************************************
//坐标转换
//
//************************************************
void y2x(u32 xo,u32 yo,u32 xe,u32 ye,_PT xyc,u8 Moto_NO)
{ 
	 u8 buf[6]={0};
	 if(arc_par.rel_dir[Moto_NO/2][0]==0)//x相反
	 {	 
			xee=(xe>=xo)? 0x80000000|xe-xo :(xo-xe);
	 }	 
	 else
	 {	 
		  xee=(xe>=xo)? xe-xo :0x80000000|(xo-xe);
	 }	
	 
	 if(arc_par.rel_dir[Moto_NO/2][1]==1)//y同向
	 {	 
		    yee=(ye>=yo)? ye-yo :0x80000000|(yo-ye);		
				
	 }	 
	 else
	 {	 
			 yee=(ye>=yo)? 0x80000000|(ye-yo) :(yo-ye);
        
	 }	 
	 //求出xc、yc
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

//	buf[0]=0x42;//圆心x高
//	buf[1]=xc>>24;
//	buf[2]=xc>>16;
//	Send_parameter(no+1,buf,FPGA_COMMAND_LEN);

//	buf[0]=0x43;//圆心x低
//	buf[1]=xc>>8;
//	buf[2]=xc;
//	Send_parameter(no+1,buf,FPGA_COMMAND_LEN);
//				
//	buf[0]=0x44;//圆心y高
//	buf[1]=yc>>24;
//	buf[2]=yc>>16;
//	Send_parameter(no+1,buf,FPGA_COMMAND_LEN);	

//	buf[0]=0x45;//圆心y低
//	buf[1]=yc>>8;
//	buf[2]=yc;
//	Send_parameter(no+1,buf,FPGA_COMMAND_LEN);
	buf[0]=0x06;//圆心x
	buf[1]=0x02;		
	buf[2]=xc>>24;
	buf[3]=xc>>16;
	buf[4]=xc>>8;
	buf[5]=xc;
	Send_parameter(Moto_NO+1,buf,FPGA_COMMAND_LEN);
	buf[0]=0x06;//圆心y
	buf[1]=0x03;		
	buf[2]=yc>>24;
	buf[3]=yc>>16;
	buf[4]=yc>>8;
	buf[5]=yc;
	Send_parameter(Moto_NO+1,buf,FPGA_COMMAND_LEN);
}	


/**
  * @brief  光栅尺命令解析
  *  
  * @param  rxBuf     接收数据  
  *  			rxLen     数据长度
  *   
  * @retval 返回值说明
            OPT_SUCCESS命令执行完成
            OPT_FAILURE命令未执行完成，比如等待未等到
  *
  * @note   							                                            字符串
  */

RET_STATUS GratingRuler_CmdParse(u8* rxBuf,u8 rxLen, CMD_ARG *arg)	
{
	u8 i;
	u8 Para_len;
	u32 buf=0;
	u32 Grating_No;//光栅尺号
	u8 Grating_Function;//光栅尺功能（设置参数，写参数，启动。。）
	u8 type;
	u32 pflag;
	Grating_No = UartToU32(&rxBuf[0],4,LITTLEEND);//rxBuf[0];
	Grating_Function = rxBuf[4];
	type = rxBuf[5];
	pflag = UartToU32(&rxBuf[6],4,LITTLEEND);//(rxBuf[3]<<8)+rxBuf[4];
	rxBuf = rxBuf+3;
	switch(Grating_Function)
	{
		case 0x30:	//光栅尺参数设置
				switch(type) 
				{
					case 0xFF:  

							break;
					case 0xFE://使用变量值
							break;
					default:
						   break;
				}
			break;
		case 0x31:	//光栅尺参数读取
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
								buf=buf+(Para_Buf[i]<<(8*(Para_len-i-1)));//buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
							}
							ValSet(rxBuf[5],buf);
							break; 					
					default:
							 break;
				}
			break;	
		case 0x32:	//光栅尺使能
				if(type == 0xFF)
				{
					GratingRuler_Ctrl(Grating_No,pflag&0x0001);
				}
				
			break;				
		case 0x33:	//光栅尺计数清零
				if(type == 0xFF)
				{
					Clear_GratingRuler(Grating_No);
				}
			break;	
		case 0x34:	//光栅尺计数读取
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
								buf=buf+(Para_Buf[i]<<(8*(Para_len-i-1)));//buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
							}
							ValSet(rxBuf[5],buf);
							break; 					
					default:
							 break;
				}
			break;
		case 0x35:	//光栅尺位置读取
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
								buf=buf+(Para_Buf[i]<<(8*(Para_len-i-1)));//buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
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
//读取光栅尺位置
//
//Grating_No:光栅尺号（0-7）
//****************************************************
u8 Read_GratingRuler_Pos(u8 Grating_No)
{
	 s32 Grating_POS;
	 u16 i=0;
	 Cs_FPGA(Grating_No/2);
	 if(Grating_No%2)	//Y轴 偶数轴
	 {
		 bmq_count = Read_Fpga_Regist(0x8F02,Grating_No>>1);  
	 }
	 else			//X轴 奇数轴
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
//清除光栅尺计数
//
//Grating_No:光栅尺号（0-7）
//****************************************************
void Clear_GratingRuler(u8 Grating_No)
{
	u8 buf[6] = {0};
	Cs_FPGA(Grating_No/2);
	if(Grating_No%2)//Y轴 偶数轴
	{
		buf[0]=0X02;
	}
	else//X轴 奇数轴
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
	Send_parameter(Grating_No>>1,buf,FPGA_COMMAND_LEN);//去除清零标志
}
//****************************************************
//使能/失能光栅尺
//
//Grating_No:光栅尺号（0-7）
//flag: 1使能 0失能
//****************************************************
void GratingRuler_Ctrl(u8 Grating_No,u16 flag)
{
	u8 buf[6] = {0};
	Cs_FPGA(Grating_No/2);
	if(Grating_No%2)//Y轴 偶数轴
	{
		buf[0]=0X02;
	}
	else//X轴 奇数轴
	{
		buf[0]=0x00;
	}	
	buf[1] = 0x0D;
	buf[2] = 0;
	buf[3] = 0;	
	buf[4] = 0;
	if(flag)//使能
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
  * @brief  电机参数初始化
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
  * @brief  单步预计运行总时间(单位毫秒)
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
				acctime = (CDV_INT64U)(TargetSpeed-MinSpeed)*1000/acc;//加速时间
				dectime = (CDV_INT64U)(TargetSpeed-MinSpeed)*1000/dec;//减速时间
				MaxSpeed = TargetSpeed;
				uniformtime = (CDV_INT32U)((CDV_INT64U)(run_step-DecStep-AccStep)*1000/(CDV_INT64U)TargetSpeed);	
				runtime = acctime+dectime+uniformtime;
			}
//			else if(AccStep>run_step)//加速步数大于总步数  减速度改成加速度
//			{
//				//DecStep = run_step/2;		
//				DecStep=run_step*acc/(dec+acc);
//				dec_Con = (u32)Moto_Acc_Con(dec,TargetSpeed,MinSpeed);	
//				MinSpeed_Con = (u32)Moto_Speed_Con(MinSpeed);
//				MaxSpeed = MaxSpeed_Real(DecStep,MinSpeed_Con,dec_Con);				
//				dectime = (MaxSpeed-MinSpeed)*1000/dec;//减速时间
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
				dectime = (CDV_INT64U)(MaxSpeed-MinSpeed)*1000/dec;//减速时间	
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
		acctime = (CDV_INT32U)((CDV_INT32U)((MinSpeed_Con-TargetSpeed_Con)/(float)acc_Con+0.5)*FPGA_SPEED_CHANGE_CYCLE*1000);//加速时间
		dectime = (CDV_INT32U)((CDV_INT32U)((MinSpeed_Con-TargetSpeed_Con)/(float)dec_Con+0.5)*FPGA_SPEED_CHANGE_CYCLE*1000);//减速时间
		TargetSpeed=(CDV_INT32U)((CDV_INT64U)FPGA_W_FRE_DIV2*(CDV_INT64U)FPGA_ACC_DIV/(CDV_INT64U)TargetSpeed);
		TargetSpeed=(CDV_INT32U)((CDV_INT64U)FPGA_W_FRE_DIV2*(CDV_INT64U)FPGA_ACC_DIV/(CDV_INT64U)TargetSpeed);
		uniformtime = (CDV_INT32U)((CDV_INT64U)(run_step-DecStep-AccStep)*1000/(CDV_INT64U)TargetSpeed);	
		runtime = acctime+dectime+uniformtime;
	}
	else if(AccStep>run_step)//加速步数大于总步数  减速度改成加速度
	{
		DecStep = run_step/2;		
		MaxSpeed = MaxSpeed_Real(DecStep,MinSpeed_Con,acc_Con);
		MaxSpeed_Con = (u32)Moto_Speed_Con(MaxSpeed);
		acctime = (CDV_INT32U)((CDV_INT32U)((MinSpeed_Con-MaxSpeed_Con)/(float)acc_Con+0.5)*FPGA_SPEED_CHANGE_CYCLE*1000);//加速时间
		runtime = acctime*2;		
	}
	else	//加速步数小于总步数 加速度改成减速度
	{
		DecStep = run_step/2;		
		MaxSpeed = MaxSpeed_Real(DecStep,MinSpeed_Con,dec_Con);
		MaxSpeed_Con = (u32)Moto_Speed_Con(MaxSpeed);
		dectime = (CDV_INT32U)((CDV_INT32U)((MinSpeed_Con-MaxSpeed_Con)/(float)dec_Con+0.5)*FPGA_SPEED_CHANGE_CYCLE*1000);//减速时间	
		runtime = dectime*2;			
	}
	#endif
	moto_sta.ela_runtime[Moto_No] = GetCdvTimeTick();
	gStageStartup_Time[Moto_No] = GetCdvTimeTick();
	return runtime;
}
/**
  * @brief  速度达不到最高速度时，真实运行最大速度计算
  * @param  DecStep：减速步数
				MinSpeed_Con：最低速度(转换后)
				acc_Con：加速度(转换后)
  * @retval 
  * @note   
  */
u32 MaxSpeed_Real(u32 DecStep,u32 MinSpeed_Con,u32 acc_Con)
{
	u32 MaxSpeedReal;
	u32 speed_imm;//即时速度
	double step_imm=0;//即时步数
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
  * @brief  电机消息设置
  * @param  Moto_No：电机号
				pos：触发位置
				mode：触发模式
				id：消息ID
  * @retval 
  * @note   
  */
void MotoMessageSet(u8 Moto_No,u32 pos,u8 mode,u16 id)
{
	u8 buf[6] = {0};
	Cs_FPGA(Moto_No/2);
	if(Moto_No%2)//Y轴 偶数轴
	{
		buf[0]=0X02;
	}
	else//X轴 奇数轴
	{
		buf[0]=0x00;
	}	
	//触发位置
	buf[1] = 0x33;
	buf[2] = pos>>24;
	buf[3] = pos>>16;	
	buf[4] = pos>>8;
	buf[5] = pos;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	
	//触发模式
	buf[1] = 0x34;
	buf[2] = 0;
	buf[3] = 0;	
	buf[4] = 0;
	buf[5] = mode;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);
	
	//消息ID
	buf[1] = 0x35;
	buf[2] = 0;
	buf[3] = 0;	
	buf[4] = id>>8;
	buf[5] = id;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
	
}
/**
  * @brief  电机消息查询
  * @param  no：电机号
  * @retval 触发的消息ID号
  * @note   
  */
u16 MotoMessageQuery(u8 Moto_No)
{
	u16 mes_id;	
	mes_id=(u16)Read_Fpga_Regist(0x8F09,Moto_No >>1);
	return mes_id;
}
/**
  * @brief  电机消息清除
  * @param  no：电机号
  * @retval 触发的消息ID号
  * @note   
  */
void MotoMessageClear(u8 Moto_No)
{
	u8 buf[6] = {0};
	Cs_FPGA(Moto_No/2);
	if(Moto_No%2)//Y轴 偶数轴
	{
		buf[0]=0X02;
	}
	else//X轴 奇数轴
	{
		buf[0]=0x00;
	}		
	//消息ID
	buf[1] = 0x35;
	buf[2] = 0;
	buf[3] = 0;	
	buf[4] = 0;
	buf[5] = 0;
	Send_parameter(Moto_No>>1,buf,FPGA_COMMAND_LEN);	
}
/**
  * @brief  将串口值src按nByte字节，转化成u32
  * @param  src：串口值
				nByte：转化字节数 小于等于4
				iBigEndian：1，大端模式；0，小端模式
  * @retval 
  * @note   
  */
// 
u32 UartToU32(u8 * src,u8 nByte, u8 iBigEndian)
{
	u32 uldst=0;
	u16 i = 0;
	if(nByte>4)nByte=4;
	if (1 == iBigEndian)// 大端模式
	{
		for (i =0; i < nByte; i++)
		{
			uldst |= src[i]<<((nByte-i-1)*8);			
		}
	}
	else if (0 == iBigEndian)// 小端模式
	{
		for (i =0; i < nByte; i++)
		{
			uldst |= src[i]<<(i*8);			
		}
	}
	
	return uldst;
}
/**
  * @brief  电机正转（向前）
  * @param  *rxBuf：串口缓存指针
				rxLen：缓存长度
				Moto_No：电机号
  * @retval 
  * @note   
  */
// 
void MotorFwd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	u8 rdir;
	u32 variate_num;//变量号
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
	if(gmoto_flag[Moto_No]==1)//如果上一次暂停之后没继续，就要从新读取FPGA当前的坐标
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
//			 case 0xfe://按变量移动
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
								if(rxLen>10)//需要同时设置其他参数
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
			case 0xfe://按变量移动
								if(rxLen>10)//需要同时设置其他参数
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
			case 0xFD://参数混合
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
								for(i=0;i*5<rxLen-10;i++)
								{
									sub_type[i] = rxBuf[i*5+5];
									if(i==0)
									{
										if(sub_type[i]==0xFF)//常数
										{
											num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										}
										else if(sub_type[i]==0xFE)//变量
										{
											variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
											num = VarGet(variate_num);
										}
									}
									else
									{
										if(sub_type[i]==0xFE)//变量
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
  * @brief  电机多段速正转（向前）
  * @param  *rxBuf：串口缓存指针
				rxLen：缓存长度
				Moto_No：电机号
  * @retval 
  * @note   
  */
// 
void MotorMultFwd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	u8 rdir;
	u32 variate_num;//变量号
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
	if(gmoto_flag[Moto_No]==1)//如果上一次暂停之后没继续，就要从新读取FPGA当前的坐标
	{
		gmoto_flag[Moto_No]=0;
		Para_len=Read_Moto_Par(Moto_No,0x1000);	
		gmoto_pos[Moto_No]=UartToU32(&Para_Buf[0],4,BIGENDIAN);
	}
	if(FpgaMotoWait(Moto_No))//运行结束
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
							if(rxLen>10)//需要同时设置其他参数
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
		case 0xfe://按变量移动
							if(rxLen>10)//需要同时设置其他参数
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
		case 0xFD://参数混合
							pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
							for(i=0;i*5<rxLen-10;i++)
							{
								sub_type[i] = rxBuf[i*5+5];
								if(i==0)
								{
									if(sub_type[i]==0xFF)//常数
									{
										num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
									}
									else if(sub_type[i]==0xFE)//变量
									{
										variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										num = VarGet(variate_num);
									}
								}
								else
								{
									if(sub_type[i]==0xFE)//变量
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
  * @brief  将u32数据转成u8缓存数组数据
  * @param  *dst：目标数组/指针
				data：u32类型原数据
				iBigEndian：存储方式 1大端 0小端
  * @retval 
  * @note   
  */
//
void U32ToBuf(u8 *dst,u32 data, u8 iBigEndian)
{
	if(iBigEndian == BIGENDIAN)	//大端
	{	
		*(dst+3) = (data&0x000000FF);
		*(dst+2) = (data&0x0000FF00)>>8;
		*(dst+1) = (data&0x00FF0000)>>16;
		*dst= (data&0xFF000000)>>24;		
	}
	else//小端
	{
		*dst = (data&0x000000FF);
		*(dst+1) = (data&0x0000FF00)>>8;
		*(dst+2) = (data&0x00FF0000)>>16;
		*(dst+3) = (data&0xFF000000)>>24;	

	}
}
/**
  * @brief  将u16数据转成u8缓存数组数据
  * @param  *dst：目标数组/指针
				data：u32类型原数据
				iBigEndian：存储方式 1大端 0小端
  * @retval 
  * @note   
  */
//
void U16ToBuf(u8 *dst,u32 data, u8 iBigEndian)
{
	if(iBigEndian == BIGENDIAN)	//大端
	{	
		*(dst+1) = data&0x00FF;
		*dst= (data&0xFF00)>>8;		
	}
	else//小端
	{
		*dst = data&0x00FF;
		*(dst+1) = (data&0xFF00)>>8;

	}
}
/**
  * @brief  电机反转（向后）
  * @param  *rxBuf：串口缓存指针
				rxLen：缓存长度
				Moto_No：电机号
  * @retval 
  * @note   
  */
// 
void MotorRev(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	//u8 rdir;
	u32 variate_num;//变量号
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
		if(gmoto_flag[Moto_No]==1)//如果上一次暂停之后没继续，就要从新读取FPGA当前的坐标
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
			 case 0xfe://按变量移动
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
		if(gmoto_flag[Moto_No]==1)//如果上一次暂停之后没继续，就要从新读取FPGA当前的坐标
		{
			gmoto_flag[Moto_No]=0;
			Para_len=Read_Moto_Par(Moto_No,0x1000);	
			gmoto_pos[Moto_No]=UartToU32(&Para_Buf[0],4,BIGENDIAN);
		}
		switch(type) 
		{
			case 0xFF: 
								if(rxLen>10)//需要同时设置其他参数
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
			case 0xfe://按变量移动
								if(rxLen>10)//需要同时设置其他参数
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
			case 0xFD://参数混合
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
								for(i=0;i*5<rxLen-10;i++)
								{
									sub_type[i] = rxBuf[i*5+5];
									if(i==0)
									{
										if(sub_type[i]==0xFF)//常数
										{
											num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										}
										else if(sub_type[i]==0xFE)//变量
										{
											variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
											num = VarGet(variate_num);
										}
									}
									else
									{
										if(sub_type[i]==0xFE)//变量
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
  * @brief  电机多段速反转（向后）
  * @param  *rxBuf：串口缓存指针
				rxLen：缓存长度
				Moto_No：电机号
  * @retval 
  * @note   
  */
// 
void MotorMultRev(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	//u8 rdir;
	u32 variate_num;//变量号
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
	if(gmoto_flag[Moto_No]==1)//如果上一次暂停之后没继续，就要从新读取FPGA当前的坐标
	{
		gmoto_flag[Moto_No]=0;
		Para_len=Read_Moto_Par(Moto_No,0x1000);	
		gmoto_pos[Moto_No]=UartToU32(&Para_Buf[0],4,BIGENDIAN);
	}
	if(FpgaMotoWait(Moto_No))//运行结束
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
							if(rxLen>10)//需要同时设置其他参数
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
		case 0xfe://按变量移动
							if(rxLen>10)//需要同时设置其他参数
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
		case 0xFD://参数混合
							pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
							for(i=0;i*5<rxLen-10;i++)
							{
								sub_type[i] = rxBuf[i*5+5];
								if(i==0)
								{
									if(sub_type[i]==0xFF)//常数
									{
										num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
									}
									else if(sub_type[i]==0xFE)//变量
									{
										variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										num = VarGet(variate_num);
									}
								}
								else
								{
									if(sub_type[i]==0xFE)//变量
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
  * @brief  电机移到
  * @param  *rxBuf：串口缓存指针
				rxLen：缓存长度
				Moto_No：电机号
  * @retval 
  * @note   
  */
// 
void MotorMoveTo(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	u8 rdir;
	u32 variate_num;//变量号
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
							if(rxLen>10)//需要同时设置其他参数
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
		case 0xFE://使用变量值
//							variate_num=rxBuf[1];
//							num = VarGet(variate_num);
							if(rxLen>10)//需要同时设置其他参数
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
			case 0xFD://参数混合
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
								for(i=0;i*5<rxLen-10;i++)
								{
									sub_type[i] = rxBuf[i*5+5];
									if(i==0)
									{
										if(sub_type[i]==0xFF)//常数
										{
											num = (CDV_INT32S)UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
										}
										else if(sub_type[i]==0xFE)//变量
										{
											variate_num=UartToU32(&rxBuf[i*5+6],4,LITTLEEND);
											num = VarGet(variate_num);
										}
									}
									else
									{
										if(sub_type[i]==0xFE)//变量
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
  * @brief  电机归零
  * @param  *rxBuf：串口缓存指针
				rxLen：缓存长度
				Moto_No：电机号
  * @retval 
  * @note   
  */
// 
void MotorRtnZero(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CDV_INT32U Moto_No) 
{
	u8 Para_len;
	u8 rdir;
	u32 variate_num;//变量号
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
								if(rxLen>10)//需要同时设置其他参数
								{
									pflag = UartToU32(&rxBuf[1],4,LITTLEEND);		
									
									Moto_Config(Moto_No,pflag,rxBuf+5,type,0);								
								}
								break;
			case 0xfe://按变量移动
								if(rxLen>10)//需要同时设置其他参数
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
			case 0xFD://参数混合
								pflag = UartToU32(&rxBuf[1],4,LITTLEEND);	
								for(i=0;i*5<rxLen-10;i++)
								{
									sub_type[i] = rxBuf[i*5+5];	
									if(sub_type[i]==0xFE)//变量
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
  * @brief  参数范围控制
  * @param  *Data：数据地址
				Max：最大值
				Min：最小值
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
  * @brief  电机资源参数读取
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
  * @brief  计算S型速度曲线
  * @param  
  * @retval speed:实时速度
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
	CDV_INT32U cnt = gAccAndDec_Cnt[Moto_No];//cnt：加减速过程中的第几个时间点，一个时间点为5ms
	CDV_INT32U speed;
	CDV_FP32 len; 
	if(Moto_RunSta[Moto_No]==ACC)	
		len = (CDV_FP32)gAcc_Times[Moto_No];	//5ms改变一次速度
	else if(Moto_RunSta[Moto_No]==DEC)	
		len = (CDV_FP32)gDec_Times[Moto_No];	//5ms改变一次速度
//	else if(Moto_RunSta[Moto_No]==UNIFORM)	
//		return speed_max*FPGA_ACC_DIV;
	if(gAccAndDec_Cnt[Moto_No]>=(CDV_INT32U)len)
	{
		if(Moto_RunSta[Moto_No]==ACC)			
			return speed_max*FPGA_ACC_DIV;
		else if(Moto_RunSta[Moto_No]==DEC)	
			return speed_min*FPGA_ACC_DIV;	
	}
//	melo = moto_par.SModel_flexible[Moto_No] * (cnt-len/2) / (len/2);//SModel_flexible代表S曲线区间（越大代表压缩的最厉害，中间（x坐标0点周围）加速度越大；越小越接近匀加速。理想的S曲线的取值为4-6
//	deno = 1.0 / (1 + expf(-melo));   
//	if(Moto_RunSta[Moto_No]==ACC)	//加速
//		speed = (delt * deno + speed_min)*FPGA_ACC_DIV;
//	else if(Moto_RunSta[Moto_No]==DEC)		//减速
//		speed = (speed_max - delt * deno)*FPGA_ACC_DIV;
	if(Moto_RunSta[Moto_No]==ACC)	//加速
		melo = moto_par.SModel_flexible[Moto_No] * (cnt-len/2) / (len/2);//SModel_flexible代表S曲线区间（越大代表压缩的最厉害，中间（x坐标0点周围）加速度越大；越小越接近匀加速。理想的S曲线的取值为4-6
	else if(Moto_RunSta[Moto_No]==DEC)		//减速
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
	CDV_FP32 len = delt/(FPGA_SPEED_CHANGE_CYCLE*acc);	//5ms改变一次速度
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
	if(State)//加速
		speed = (delt * deno + speed_min)*FPGA_ACC_DIV;
	else	//减速
		speed = (speed_max - delt * deno)*FPGA_ACC_DIV;;
	return speed;
}

/**
  * @brief  实时速度设置
* @param  Moto_No:电机号
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
	CDV_INT32U ela_runtime;//已运行时间
	CDV_INT32U pos;
	CDV_INT32U running_step;//已运行步数
	CDV_INT32U run_step;//需要运行步数
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