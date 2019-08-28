#include "ProportionalValve.h"

#ifdef ENABLE_PID
ProportionalValve Struct_Valve[PID_NUM];
PID_STRUCT Pid_par[PID_no];
u16  LastPoint;
double Present_Valve_DAC[PID_NUM] = {0};
double Present_Valve_DA = 0;
u16 Scale_Valve_DA =10;
float Present_Pressure_AD ;
u16 R_Pressure_AD = 0 ;//反向气压AD
double R_Pressure = 0 ;	//反向气压KPA

/*********ADC2**************/
u16 R_PressureAD_Zero = 1313 ;	//反向压力零位B
double R_AD_COEFFICIENT=251;    //K
u16 R_PressureAD_Zero_1 = 1313 ;	//反向压力零位 
double R_AD_COEFFICIENT_1=251;
u16 R_PressureAD_Zero_2 = 1328 ;	//反向压力零位  运行状态
double R_AD_COEFFICIENT_2=249;
/*********ADC2**************/

/*********ADC1**************/
//u16 PressureAD_Zero[PID_NUM] = {1170,1170,1170,1170};	//压力零位
u16  PressureAD_Zero = 1170;
double AD_COEFFICIENT=125.8;//比例调节阀输出1-5V电压对应0-500kP，500/4=125
u16 PressureAD_Zero_1 = 1170;	//压力零位
double AD_COEFFICIENT_1=125.8;
u16 PressureAD_Zero_2 = 1200;	//压力零位  运行状态
double AD_COEFFICIENT_2=124.9709;//系数  运行状态
/*********ADC1**************/

u16 PressureValveDA_Zero[PID_NUM] ={ 0 };	//比例阀输出实际压力（非气压）为零位时DA值
double Present_Pressure_Rel = 0 ;	//真实压力
u8  ADSigPre_Flag = 0;
u8 ValveDAadjust_Flag = 0; //50ms
u8 Lcd_Refresh_Flag = 0;
u8 Sequence = 0;//工序号
double Error_Rel= 0;//当前真实误差
double Error_Sum = 0;//总误差 用作积分
//double Integral_Val = 0.01;	//积分系数
double Change_DA[PID_NUM] = {0};	//输出DA改变化
double Pressure_Coefficient=1;	//气压对应实际重量系数
u8 Run_Stat =0;//运行模式  0加压 1稳定
u8 Process_Start = 0; //流程开启标志 
u8 Process_Exit = 0; //退出流程  0关 1启动
u16 Ptarget_Num = 1;//目标压力分段  序号
u8 PInc_Flag = 0;//目标压力分段序号增加标志
double PTarget_Sys= 0;//系统当前目标压力
double LastPressure_Sys= 0;//切换目标压力前系统压力
CDV_INT16U ADC_ConvertedValue[RX_ADC1_DMA];
CDV_INT08U Inc_time_Flag = 0;
CDV_INT32U Run_time = 0;
u8 run_finish;//加压完成标志
u16 DA_Debug = 0;
PID Valve_Pid[PID_NUM];
//u16 Dish_Weight=0;
void PIDInit(PID *PP)
{
  memset(PP,0,sizeof(PID));
} 
void PIDInit_Valve(void)
{
	#if 0
	Valve_Pid.Proportion=0.05;  /*设置PID比例系数*/ 
	//Valve_Pid.Integral=0.001;  /*设定PID积分系数*/
	Valve_Pid.Integral=1000;  		/*设定PID积分系数*/
	Valve_Pid.Derivative=0.05;  	/*设定PID微分系数*/
	Valve_Pid.Integral_Limit = 100;/*设定积分上限*/
	Valve_Pid.SetPoint=0;  /*设定目标值*/
	#else
	//	Modification in 2016-7-25 by pengcongfeng		
	
	Pid_par[0].Proportion = (CDV_FP64)Valve_Pid[0].Proportion/10000;
	Pid_par[0].Integral = (CDV_FP64)Valve_Pid[0].Integral/10000;
	Pid_par[0].Derivative = (CDV_FP64)Valve_Pid[0].Derivative/10000;
	Pid_par[0].Integral_Limit = (CDV_FP64)Valve_Pid[0].Integral_Limit/10000;
	Pid_par[0].Ctrl_Time = (CDV_FP64)Valve_Pid[0].Ctrl_Time/10000;
	Pid_par[0].AD_no = (CDV_FP64)Valve_Pid[0].AD_no;
	Pid_par[0].DA_no = (CDV_FP64)Valve_Pid[0].DA_no;
	
	Pid_par[1].SetPoint = Struct_Valve[1].Ptarget;
	Pid_par[1].Proportion = (CDV_FP64)Valve_Pid[1].Proportion/10000;
	Pid_par[1].Integral = (CDV_FP64)Valve_Pid[1].Integral/10000;
	Pid_par[1].Derivative = (CDV_FP64)Valve_Pid[1].Derivative/10000;
	Pid_par[1].Integral_Limit = (CDV_FP64)Valve_Pid[1].Integral_Limit/10000;
	Pid_par[1].Ctrl_Time = (CDV_FP64)Valve_Pid[1].Ctrl_Time/10000;
	Pid_par[1].AD_no = (CDV_FP64)Valve_Pid[1].AD_no;
	Pid_par[1].DA_no = (CDV_FP64)Valve_Pid[1].DA_no;
	
	Pid_par[2].SetPoint = Struct_Valve[2].Ptarget;
	Pid_par[2].Proportion = (CDV_FP64)Valve_Pid[2].Proportion/10000;
	Pid_par[2].Integral = (CDV_FP64)Valve_Pid[2].Integral/10000;
	Pid_par[2].Derivative = (CDV_FP64)Valve_Pid[2].Derivative/10000;
	Pid_par[2].Integral_Limit = (CDV_FP64)Valve_Pid[2].Integral_Limit/10000;
	Pid_par[2].Ctrl_Time = (CDV_FP64)Valve_Pid[2].Ctrl_Time/10000;
	Pid_par[2].AD_no = (CDV_FP64)Valve_Pid[2].AD_no;
	Pid_par[2].DA_no = (CDV_FP64)Valve_Pid[2].DA_no;
	
	Pid_par[3].SetPoint = Struct_Valve[3].Ptarget;
	Pid_par[3].Proportion = (CDV_FP64)Valve_Pid[3].Proportion/10000;
	Pid_par[3].Integral = (CDV_FP64)Valve_Pid[3].Integral/10000;
	Pid_par[3].Derivative = (CDV_FP64)Valve_Pid[3].Derivative/10000;
	Pid_par[3].Integral_Limit = (CDV_FP64)Valve_Pid[3].Integral_Limit/10000;
	Pid_par[3].Ctrl_Time = (CDV_FP64)Valve_Pid[3].Ctrl_Time/10000;
	Pid_par[3].AD_no = (CDV_FP64)Valve_Pid[3].AD_no;
	Pid_par[3].DA_no = (CDV_FP64)Valve_Pid[3].DA_no;
	#endif
}
void PIDClear_Valve(void)
{
	Valve_Pid[0].LastError = 0;
	Valve_Pid[0].SumError = 0;
	Valve_Pid[0].PrevError = 0;
}
CDV_FP64 PIDcal(PID *pp,CDV_FP64 PresPoint,CDV_FP64 SetPoint)
{
	CDV_FP64  dError,Error; 
	CDV_FP64 tmp;
	Error = SetPoint -  PresPoint;    // 当前偏差        
	pp->SumError += Error;                // 积分
	
	//	add in 2016-7-25 by pengcongfeng	
	if(pp->SumError>pp->Integral_Limit)
		pp->SumError = pp->Integral_Limit;
	else if(pp->SumError<(0-(double)pp->Integral_Limit))
		pp->SumError = 0-(double)pp->Integral_Limit;
	
	dError = pp->LastError - pp->PrevError;     // 当前微分         
	pp->PrevError = pp->LastError;         
	pp->LastError = Error;  
	
	//	Modification in 2016-7-14 by pengcongfeng
//	pp->SumError=pp->SumError>Valve_Pid.Integral_Limit ? pp->Integral_Limit:pp->SumError;
	pp->SumError=pp->SumError>pp->Integral_Limit ? pp->Integral_Limit:pp->SumError;
	//if(pp->SumError>pp->Integral_Limit)pp->SumError=Valve_Pid.Integral_Limit;
	//tmp =pp->Proportion * Error +pp->Integral * pp->SumError +pp->Derivative * dError ;
	
	tmp =pp->Proportion * Error + pp->SumError/pp->Integral +pp->Derivative * dError ;
	return tmp;
}

CDV_FP64 PIDcal1(PID_STRUCT *pp,CDV_FP64 SetPoint)
{
	CDV_FP64  dError,Error; 
	CDV_FP64 tmp;
	CDV_FP64 PresPoint;
	PresPoint = Get_Ad_value(pp->AD_no);
	Error = SetPoint -  PresPoint;   			//当前偏差        
	pp->SumError += Error;                //积分
	
	//	add in 2016-7-25 by pengcongfeng	
	if(pp->SumError>pp->Integral_Limit)
		pp->SumError = pp->Integral_Limit;
	else if(pp->SumError<(0-(double)pp->Integral_Limit))
		pp->SumError = 0-(double)pp->Integral_Limit;
	
	dError = pp->LastError - pp->PrevError;     //当前微分         
	pp->PrevError = pp->LastError;         
	pp->LastError = Error;  
	
	//	Modification in 2016-7-14 by pengcongfeng
//	pp->SumError=pp->SumError>Valve_Pid.Integral_Limit ? pp->Integral_Limit:pp->SumError;
	pp->SumError=pp->SumError>pp->Integral_Limit ? pp->Integral_Limit:pp->SumError;
	//if(pp->SumError>pp->Integral_Limit)pp->SumError=Valve_Pid.Integral_Limit;
	//tmp =pp->Proportion * Error +pp->Integral * pp->SumError +pp->Derivative * dError ;
	
	tmp =pp->Proportion * Error + pp->SumError/pp->Integral +pp->Derivative * dError ;
	return tmp;
}
//比例阀、压力传感器系数上电初始化
//	add in 2016-7-25 by pengcongfeng	
void Valve_PSensor_Coe_Init(void)
{
		PressureAD_Zero_1 = (CDV_FP64)adc_par[0].Zero*4096/3300;;    //比例阀
	  AD_COEFFICIENT_1 = (CDV_FP64)adc_par[0].Slope/10000;;
	  PressureAD_Zero_2 = (CDV_FP64)adc_par[0].Zero2*4096/3300;;
	  AD_COEFFICIENT_2 = (CDV_FP64)adc_par[0].Slope2/10000;;
	
     R_PressureAD_Zero_1 = (CDV_FP64)adc_par[2].Zero*4096/3300;;  //压力传感器
	  R_AD_COEFFICIENT_1 = (CDV_FP64)adc_par[2].Slope/10000;;
     R_PressureAD_Zero_2 = (CDV_FP64)adc_par[2].Zero2*4096/3300;;
	  R_AD_COEFFICIENT_2 = (CDV_FP64)adc_par[2].Slope2/10000;;

}
//比例阀、压力传感器系数切换
//	add in 2016-7-25 by pengcongfeng	
void Valve_PSensor_Coe_Switchover(void)
{
	if(Process_Start)
	{
		PressureAD_Zero = PressureAD_Zero_2;
		AD_COEFFICIENT = AD_COEFFICIENT_2;
		R_PressureAD_Zero = R_PressureAD_Zero_2;
		R_AD_COEFFICIENT = R_AD_COEFFICIENT_2;		
	}
	else
	{
		PressureAD_Zero = PressureAD_Zero_1;
		AD_COEFFICIENT = AD_COEFFICIENT_1;	
		R_PressureAD_Zero = R_PressureAD_Zero_1;
		R_AD_COEFFICIENT = R_AD_COEFFICIENT_1;			
	}			
}
//比例阀启动初始化
void ProportionalValve_Init(void)
{
		PIDClear_Valve();
		//PressureAD_Zero = 1188;
	LastPressure_Sys = Get_Ad_value(0);
		
//		Present_Pressure_AD = Grubbs(ADC_ConvertedValue,RX_ADC1_DMA);//获取ADC值，取自DMA的
//	  Present_Pressure_AD =Present_Pressure_AD*fen_ya_val;//如果接了分压电阻，乘上分压系数，没接的时候默认是系数是1
//		LastPressure_Sys = (Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient;//计算当前的压力值，PressureAD_Zero为零压力时ADC对应的值
		Process_Start =1;
		Run_Stat = 0;
		Run_time=0;
		Ptarget_Num = 1;		
		ValveDAadjust_Flag = 0;
//		PressureAD_Zero = PressureAD_Zero_2;	 //压力零位
		AD_COEFFICIENT=AD_COEFFICIENT_2;
	  R_PressureAD_Zero = R_PressureAD_Zero_2;
	  R_AD_COEFFICIENT = R_AD_COEFFICIENT_2;		
}
//运行流程
//参数：流程参数结构体
double RunProcess(u16 no)
{
	
	Change_DA[no] =	PIDcal1(&Pid_par[no],(double)Ptarget_Offset(Struct_Valve[no].Ptarget));  
	if((Present_Valve_DAC[no]+Change_DA[no])>0)
		Present_Valve_DAC[no] +=Change_DA[no];
	else
		Present_Valve_DAC[no] = 0;	
	
	if(Present_Valve_DAC[no]>4095) 
		Present_Valve_DAC[no] = 4095;
	
	Dac_set_Voltge(Valve_Pid[no].DA_no,Present_Valve_DAC[no]);
	delay_ms(30);		
//		Present_Pressure_AD = Grubbs(ADC_ConvertedValue,RX_ADC1_DMA);//获取ADC值，取自DMA的
//	  Present_Pressure_AD =Present_Pressure_AD*fen_ya_val;//如果接了分压电阻，乘上分压系数，没接的时候默认是系数是1
//		Present_Pressure_Rel = (Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient;
////		R_Pressure = (R_Pressure_AD-R_PressureAD_Zero)*3.3/4095*R_AD_COEFFICIENT;
////		if(R_Pressure<0)R_Pressure==0;
////		if(Process_Start)R_Pressure -=6;
//	  Valve_PSensor_Coe_Switchover();
//		R_Pressure_Measure();
////	if(Present_Pressure_AD>1000) //应为比例调节阀的输出电压范围1-5V所以1/3.3*4.95=1240.9，当AD值小于1000时说明有问题
////		{
//			if(Process_Start)
//			{					
//				if(Run_Stat==0)//加压
//				 {
//					if(ValveDAadjust_Flag)//50ms标记，在定时器3中置位
//					{
//						ValveDAadjust_Flag = 0;
////						Sys_time_1ms = 0;
//						if(ProportionalValve_Struct->Ptarget>=LastPressure_Sys)//总目标压力>=当前压力
//							//得到下一次要执行的目标压力=（总目标压力-当前压力）/加压时间+当前的压力值
//							PTarget_Sys = (((double)Ptarget_Offset(ProportionalValve_Struct->Ptarget)-LastPressure_Sys)/ProportionalValve_Struct->Inflating_Times)*Ptarget_Num+LastPressure_Sys;
//						else
//							PTarget_Sys = LastPressure_Sys-((LastPressure_Sys-(double)Ptarget_Offset(ProportionalValve_Struct->Ptarget))/ProportionalValve_Struct->Inflating_Times)*Ptarget_Num;
//						//下一次要执行的目标压力-当前的压力值
//						Error_Rel = PTarget_Sys-(Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient;
//						if(Run_time<ProportionalValve_Struct->Inflating_Times*10)//把加压时间(单位s)分成10等分
//						{		
//              //计算要输出电压对应的数字量/							
//							Change_DA = (Error_Rel/ITV2050/3.3*4095)/((10-Run_time%10)*5);//比例调节阀输入0-10V对应压力值5K-500K，（500-5)/3.3=150
//							if((Present_Valve_DA+Change_DA)>0)
//							{	
//								Present_Valve_DA +=Change_DA;
//							}	
//							else
//							{	
//								Present_Valve_DA = 0;	
//							}	
//							return Present_Valve_DA;
//							Dac1_Set_Vol((u16)Present_Valve_DA);	
//						}
//						else
//						{
//							Run_Stat = 1;
//							Run_time=0;
//							Ptarget_Num = 1;
//							//run_finish=1;
//						}			
//						if(Inc_time_Flag)	
//            {							
//							Run_time++;
//					  }		
//						Inc_time_Flag++;
//						Inc_time_Flag%=2;
//						if(Run_time%10==0&&Run_time>0)
//						{
//							if(PInc_Flag)
//							{
//								Ptarget_Num++;			
//								PInc_Flag = 0;
//							}
//						}
//						else
//						{
//							PInc_Flag = 1;
//						}
//						//delay_ms(50);
//						//delay_ms(43);
//						delay_ms(1);
//					}
//					else
//					{
//						delay_ms(1);
//					}
//				}
//				else
//				{								
//					Change_DA =  PIDcal(&Valve_Pid[0],(Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient,(double)Ptarget_Offset(ProportionalValve_Struct->Ptarget))/ITV2050*4095/3.3;			 
//					if((Present_Valve_DA+Change_DA)>0)
//						Present_Valve_DA +=Change_DA;
//					else
//						Present_Valve_DA = 0;	
//					return Present_Valve_DA;
//					Dac1_Set_Vol((u16)Present_Valve_DA);
//					delay_ms(30);		
//				}		
//			}		
//			else
//			{
//				delay_ms(50);	
////				
//				
//			}
////	}
////	else
////	{
////		delay_ms(1);	
////	}
//			
}
//运行流程
//参数：流程参数结构体
//double RunProcessIndex(u8 u8PidIndex)
//{
//	ProportionalValve* ProportionalValve_Struct = &Struct_Valve[u8PidIndex];
//	u8 u8AdNo = 0;
//	 if (u8PidIndex >= PID_NUM)
//		return 0;
//		u8AdNo = Valve_Pid[u8PidIndex].AD_no;
//		Present_Pressure_AD[u8AdNo] = Get_Ad_value(u8AdNo);//获取ADC值，取自DMA的
//	  Present_Pressure_AD[u8AdNo] *= fen_ya_val;//如果接了分压电阻，乘上分压系数，没接的时候默认是系数是1
//	
//	 
//	 
//		Present_Pressure_Rel = (Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient;
////		R_Pressure = (R_Pressure_AD-R_PressureAD_Zero)*3.3/4095*R_AD_COEFFICIENT;
////		if(R_Pressure<0)R_Pressure==0;
////		if(Process_Start)R_Pressure -=6;
//	  Valve_PSensor_Coe_Switchover();
//		R_Pressure_Measure();
////	if(Present_Pressure_AD>1000)//应为比例调节阀的输出电压范围1-5V所以1/3.3*4.95=1240.9，当AD值小于1000时说明有问题
////		{
//			if(Process_Start)
//			{					
//				if(Run_Stat==0)//加压
//				 {
//					if(ValveDAadjust_Flag)//50ms标记，在定时器3中置位
//					{
//						ValveDAadjust_Flag = 0;
////						Sys_time_1ms = 0;
//						if(ProportionalValve_Struct->Ptarget>=LastPressure_Sys)//总目标压力>=当前压力
//							//得到下一次要执行的目标压力=（总目标压力-当前压力）/加压时间+当前的压力值
//							PTarget_Sys = (((double)Ptarget_Offset(ProportionalValve_Struct->Ptarget)-LastPressure_Sys)/ProportionalValve_Struct->Inflating_Times)*Ptarget_Num+LastPressure_Sys;
//						else
//							PTarget_Sys = LastPressure_Sys-((LastPressure_Sys-(double)Ptarget_Offset(ProportionalValve_Struct->Ptarget))/ProportionalValve_Struct->Inflating_Times)*Ptarget_Num;
//						//下一次要执行的目标压力-当前的压力值
//						Error_Rel = PTarget_Sys-(Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient;
//						if(Run_time<ProportionalValve_Struct->Inflating_Times*10)//把加压时间(单位s)分成10等分
//						{		
//              //计算要输出电压对应的数字量/							
//							Change_DA = (Error_Rel/ITV2050/3.3*4095)/((10-Run_time%10)*5);//比例调节阀输入0-10V对应压力值5K-500K，（500-5)/3.3=150
//							if((Present_Valve_DA+Change_DA)>0)
//							{	
//								Present_Valve_DA +=Change_DA;
//							}	
//							else
//							{	
//								Present_Valve_DA = 0;	
//							}	
//							return Present_Valve_DA;
//							Dac1_Set_Vol((u16)Present_Valve_DA);	
//						}
//						else
//						{
//							Run_Stat = 1;
//							Run_time=0;
//							Ptarget_Num = 1;
//							//run_finish=1;
//						}			
//						if(Inc_time_Flag)	
//            {							
//							Run_time++;
//					  }		
//						Inc_time_Flag++;
//						Inc_time_Flag%=2;
//						if(Run_time%10==0&&Run_time>0)
//						{
//							if(PInc_Flag)
//							{
//								Ptarget_Num++;			
//								PInc_Flag = 0;
//							}
//						}
//						else
//						{
//							PInc_Flag = 1;
//						}
//						//delay_ms(50);
//						//delay_ms(43);
//						delay_ms(1);
//					}
//					else
//					{
//						delay_ms(1);
//					}
//				}
//				else
//				{								
//					Change_DA =  PIDcal(&Valve_Pid[0],(Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient,(double)Ptarget_Offset(ProportionalValve_Struct->Ptarget))/ITV2050*4095/3.3;			 
//					if((Present_Valve_DA+Change_DA)>0)
//						Present_Valve_DA +=Change_DA;
//					else
//						Present_Valve_DA = 0;	
//					return Present_Valve_DA;
//					Dac1_Set_Vol((u16)Present_Valve_DA);
//					delay_ms(30);		
//				}		
//			}		
//			else
//			{
//				delay_ms(50);	
////				LastPoint  =  Get_Ad_value(adc_par, 1);
//				Change_DA =  PIDcal(&Valve_Pid[1], Get_Ad_value( 1),(double)Ptarget_Offset(ProportionalValve_Struct->Ptarget))/ITV2050*4095/3.3;
//				
//			}
////	}
////	else
////	{
////		delay_ms(1);	
////	}
//}
 ////////////////////////////////////////////////////////// 
 //  LastPoint  =  Get_Ad_value(adc_par, 1);

 //////////////////////////////////////////////////////////
//void Down_Slow(void)
//{
//	Present_Valve_DA = 120*4095/(151*3.3);	//120kpa
//	Dac1_Set_Vol((u16)Present_Valve_DA);	
//}
//void Up_Slow(void)
//{
//	Present_Valve_DA = 60*4095/(151*3.3);		//60kpa
//	Dac1_Set_Vol((u16)Present_Valve_DA);	
//}	
void Valve_Debug(void)
{
	Present_Valve_DA = DA_Debug*4096/(ITV2050*3.3);		//60kpa
	Dac1_Set_Vol((u16)Present_Valve_DA);	
	Present_Pressure_AD = Grubbs(ADC_ConvertedValue,RX_ADC1_DMA);
	Present_Pressure_AD =Present_Pressure_AD*fen_ya_val;//如果接了分压电阻，乘上分压系数，没接的时候默认是系数是1
}
//目标偏置处理
//比例阀检测有3kpa正向误差
u16 Ptarget_Offset(u16 Target)
{
	u16 tmp = Target;
//	if(tmp>6)
//		tmp -=3;
//	else
	if(tmp<6)
		tmp = 3;
//	if(tmp>200)
//		tmp -= 2;	
	return tmp;
}
void R_Pressure_Measure(void)
{
		R_Pressure_AD = Grubbs(adc2_value,adc2_value_lens);
		R_Pressure = (R_Pressure_AD-R_PressureAD_Zero)*3.3/4095*R_AD_COEFFICIENT;
		if(R_Pressure<0)R_Pressure=0;
//		if(Process_Start)R_Pressure -=6;	
	
	if(EndCdvTime(0) > 50 && RPressureCnt <100) {
		StartCdvTime(0);
		RPressureData[RPressureCnt++] = R_Pressure;
		
	}
	
}

#endif

