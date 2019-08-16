#include "pid.h"
#ifdef ENABLE_PID
CDV_INT32S  Pid_Value; //变频器转速
CDV_INT32S  Pid_Time;  //变频器加速时间
//*******************************************
//CDV PID解析
//
//
//
//
//*******************************************
RET_STATUS PidCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg)
{
	u8 i=0 ,j=0;
	RET_STATUS  ret =OPT_SUCCESS;
	CDV_INT08U  opt , type; //功能码，功能类型
	CDV_INT32U  no ;     //资源号
	CDV_INT32U  pflag;   //参数标记
	u8 Para_len;         //读取参数长度
  u8 Read_Par_Buf[48];  // 读取参数缓存
	CDV_INT32U  var_num;  //存变量号
	u32 var_val;   //存变量值
	u8 ad_num;     //ad号
	u16 VFD_Speed;
	u16 AD_value;
	float current_val;
	double transducer_vol;
	s32 tmp;
	u8 Para_Buf[4];
	
	no  = *(CDV_INT32U*)(rxBuf);    //rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	ad_num = rxBuf[0];
//	pflag = (rxBuf[9]<<24)+(rxBuf[8]<<16)+(rxBuf[7]<<8)+rxBuf[6];
	pflag = *(CDV_INT32U*)&rxBuf[6];
	


	//u8 Para_len;
	switch(opt) 
	{
		//参数设置
		case 0x00:

							if((type==0xFF)||(type==0x2F))
							 {	 
										
										if(pflag & 0x00000001) //PID比例参数P
										{	
											Valve_Pid[no].Proportion =(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x0000002) //PID积分参数I
										{	
											Valve_Pid[no].Integral = (rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x00000004) //PID微分参数D
										{	
											Valve_Pid[no].Derivative = (rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x00000008) //PID控制间隔
										{
											Valve_Pid[no].Ctrl_Time = (rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}										
										if(pflag & 0x00000010) //PID积分上限
										{
											Valve_Pid[no].Integral_Limit = (rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x00000020) //AD通道
										{
										 Valve_Pid[no].AD_no = (rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x00000040) //DA通道
										{
											Valve_Pid[no].DA_no = (rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
										}
										
										PIDInit_Valve();
										Valve_PSensor_Coe_Switchover();
							 }	
							 else if ((type==0xFE)||(type==0x2E))  //按变量设参数
							 {
								  var_num = (rxBuf[13+j]<<24)+(rxBuf[12+j]<<16)+(rxBuf[11+j]<<8)+rxBuf[10+j]; //取变量号
									var_val = VarGet(var_num);      	//取变量值
						   
										if(pflag & 0x00000001) //PID比例参数P
										{	
											Valve_Pid[no].Proportion = var_val;								
										}
										if(pflag & 0x00000002) //PID积分参数I
										{	
											Valve_Pid[no].Integral = var_val;
										}
										if(pflag & 0x00000004) //PID微分参数D
										{	
											Valve_Pid[no].Derivative = var_val;		
										}
										if(pflag & 0x00000008) //PID控制间隔
										{
											Valve_Pid[no].Ctrl_Time = var_val;
										}											
										if(pflag & 0x00000010) //PID积分上限
										{
											Valve_Pid[no].Integral_Limit = var_val;	
										}
										if(pflag & 0x00000020) //AD通道值
										{	
											Valve_Pid[no].DA_no = var_val;
										}	
										if(pflag & 0x00000040) //DA通道值
										{
											Valve_Pid[no].DA_no = var_val;	
										}
									Valve_PSensor_Coe_Switchover();
							 }	 
							 if((type==0xFF) || (type==0xFE))//不是流程中修改参数，保存参数值到flash
	             {							 
								 SPI_Flash_Write((CDV_INT08U *)&Valve_Pid, PID_PAR_ADDR, sizeof(Valve_Pid)); //写入flash
							 }
		       break;
		//参数读取
		case 0x01:
							if((type==0xFF)||(type==0x2F))
							{	
								Para_len=Read_Pid_Par(Read_Par_Buf,no,pflag);
								ResRequest(arg->buf, arg->len, Read_Par_Buf,Para_len,arg);
							}	
							else if((type==0xFE)||(type==0x2E)) //参数读到变量
							{	
								var_val = 0;
								Para_len=Read_Adc_Par(Read_Par_Buf,no,pflag);
								for(i=0;i<Para_len;i++)
								{
									var_val=var_val+(Read_Par_Buf[i]<<(8*(Para_len-i-1))); //buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
								}
								ValSet(rxBuf[10],var_val);
							}
		          break;
							
							//启动	     
		case 0x02:
//			        if(no==0) //PID启动
//							{	
									if(type==0xff)
									{	
										if(pflag & 0x00000001) //输出值
										{	
											Struct_Valve[no].Ptarget=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];  //获取压力值
											if(Struct_Valve[no].Ptarget>280)Struct_Valve[no].Ptarget = 280;
											i=i+4;
										}	
										if(pflag & 0x00000002) //过渡时间
										{	
											Struct_Valve[no].Inflating_Times=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]; //获取过渡时间，单位S
											i=i+4;
										}
										ProportionalValve_Init();
								 }
								 else if(type==0xfe)	
								 {
										if(pflag & 0x00000001) //输出值
										{	
											Struct_Valve[no].Ptarget=VarGet((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
											Struct_Valve[no].Ptarget_Rel = (242-Struct_Valve[no].Ptarget)*1.2522;
											i=i+4;
										}	
										if(pflag & 0x00000002) //过渡时间
										{	
											Struct_Valve[no].Inflating_Times=VarGet((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
											i=i+4;
										}
										ProportionalValve_Init();
								 }
								 else
								 {
									 return ret;
								 }	
//						 }
		//读到变量	     
		case 0x03:
			        		          
							AD_value = Get_Ad_value(Valve_Pid[no].AD_no);
		          ValSet(rxBuf[6],AD_value);
//               if(ad_num==0) //获取比例调节阀反馈的压力值
//              {
////								if(Process_Start)
////								{
////									PressureAD_Zero = PressureAD_Zero_2;
////									AD_COEFFICIENT = AD_COEFFICIENT_2;
////								}
////								else
////								{
////									PressureAD_Zero = PressureAD_Zero_1;
////									AD_COEFFICIENT = AD_COEFFICIENT_1;									
////								}
//								Valve_PSensor_Coe_Switchover();
//								Present_Pressure_Rel = (Present_Pressure_AD-PressureAD_Zero)*3.3/4095*AD_COEFFICIENT*Pressure_Coefficient;

//								ValSet(rxBuf[6],(CDV_INT32S)Present_Pressure_Rel);
////						 }
//							}	
//              //ResParaRequest(rxBuf,rxLen , Para_Buf,Para_len,1);							
		      break;
		//等待等于
		case 0x04:
			        if(type==0xFF) //等待等于常量
							{
							
							}
 
              else if(type==0xFE) //等待等于变量
							{	

              }  							
			   
		     break;
		case 0x05	: //读取AD电压
			 
//			         if(ad_num==0) //比例调节阀反馈电压
//							 {
//									Present_Pressure_AD = Grubbs(ADC_ConvertedValue,RX_ADC1_DMA);
//								  Present_Pressure_AD =Present_Pressure_AD*fen_ya_val;//如果接了分压电阻，乘上分压系数，没接的时候默认是系数是1
//									tmp = Present_Pressure_AD*3300/4096;
//							 }
//							 else if(ad_num==2) //压力传感器反馈电压
//							 {
//									R_Pressure_AD = Grubbs(adc2_value,adc2_value_lens);
//									tmp = R_Pressure_AD*3300/4096;
//							 }
//							 
//							 else if(ad_num==1) //变频器反馈电压
//							 {
//                    tmp = Grubbs(adc3_value,100)*3300/4096;
//							 }
							  tmp = Get_Read_AD(Valve_Pid[no].AD_no);
               if(type==0xFE)//读到变量
							 {	
                 								 
								 ValSet(rxBuf[6],tmp);
							 }
							 
               else//读到开发层
							 {								 
								  Para_Buf[0]=tmp>>24;
								  Para_Buf[1]=tmp>>16;
								  Para_Buf[2]=tmp>>8;
								  Para_Buf[3]=tmp;
								  ResRequest(arg->buf, arg->len, Para_Buf,4,arg);
							 }							 
		     break;	
		case 0x06	: //PID输出
			        if(type==0xff)
								{	
									if(pflag & 0x00000001) //输出转速
									{	
										Pid_Value=(rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10];
										Pid_Time=(rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14];
										if(Pid_Value>TRA_SPEED_LIMIT)Pid_Value = TRA_SPEED_LIMIT;
										
									}	
										
								 }
								 else if(type==0xfe)	
								 {
										if(pflag & 0x00000001) //输出值  气压值转换成电压值输出
										{	
											Pid_Value=VarGet((rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10]);
											Pid_Time=VarGet((rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14]);
											if(Pid_Value>TRA_SPEED_LIMIT)Pid_Value = TRA_SPEED_LIMIT; 
										}	
										
								 }
								 else
								 {
									 return ret;
								 }
//						Valve_Pid.Proportion = (CDV_FP64)pid_par[no].Proportion/10000;
//						Valve_Pid.Integral = (CDV_FP64)pid_par[no].Integral/10000;
//						Valve_Pid.Derivative = (CDV_FP64)pid_par[no].Derivative/10000;
//						Valve_Pid.Integral_Limit = (CDV_FP64)pid_par[no].Integral_Limit/10000;
//						Valve_Pid.Ctrl_Time = (CDV_FP64)pid_par[no].Ctrl_Time/10000;
							Dac_set_Voltge( Valve_Pid[no].DA_no,Pid_Value); 

		break;
		default:
			    break;
		
	}	
	return ret;
}
//****************************************************
//读取参数
//no:设备号
//pflag:参数标记
//buf:参数缓存
//****************************************************
u8 Read_Pid_Par(u8 *buf,u8 no,u32 pflag)
{
	u8 i,j;
	u32 FPGA_POS;
	u16 FPGA_POSH;
	u16 FPGA_POSL;
	
	u8 buf_no;
	
	i=0;
	if(no==0)
	{
		buf_no=0;
	}
	else if(no==1)
	{
		buf_no=2;
	}
	else if(no==2)
	{
		buf_no=1;
	}
  
				if(pflag & 0x00000001) //PID比例参数P
				{	for(j=0;j<4;j++)
				*(buf+i+j)=Valve_Pid[no].Proportion>>(24-8*j);
					i=i+4;
				}
				if(pflag & 0x00000002) //PID积分参数I
				{	for(j=0;j<4;j++)
				*(buf+i+j)=Valve_Pid[no].Integral>>(24-8*j);
					i=i+4;
				}
				if(pflag & 0x00000004) //PID微分参数D
				{	for(j=0;j<4;j++)
				*(buf+i+j)=Valve_Pid[no].Derivative>>(24-8*j);
					i=i+4;
				}
				if(pflag & 0x00000008) //PID控制间隔
				{for(j=0;j<4;j++)
				*(buf+i+j)=Valve_Pid[no].Ctrl_Time>>(24-8*j);
					i=i+4;
				}										
				if(pflag & 0x00000010) //PID积分上限
				{for(j=0;j<4;j++)
				*(buf+i+j)=Valve_Pid[no].Integral_Limit>>(24-8*j);
					i=i+4;
				}
				if(pflag & 0x00000020)  //AD通道
				{
					for(j=0;j<4;j++)
					*(buf+i+j)=Valve_Pid[no].AD_no>>(24-8*j);	
					i=i+4;
				}
				if(pflag & 0x00000040) //DA通道
				{for(j=0;j<4;j++)
				*(buf+i+j)=Valve_Pid[no].DA_no>>(24-8*j);
				i=i+4;
				}
	
	return i;	//返回长度
	 
}	
//******************************************************
//变频器转速
//
//
//******************************************************
//u16 Get_VFD_Speed(u16 *buf,u16 lens)
//{
//	u16 i;
//	u16 vfd_speed;
//	double sum=0;
//	double vol;
//	for(i=0;i<lens;i++)
//	{
//		sum=sum+buf[i];
//	}
//	vol=sum/lens/4095*3.3;
//	vol=vol-(float)adc_par.Zero[1];
//  vol=vol*((float)adc_par.Slope[1]/1000);//*斜率
//	//vfd_speed=vol*4*5;
//	vfd_speed=vol*10*4;//变频器电压1v=10转
//	//vfd_speed=vfd_speed*(float)adc_par.Slope[1]/1000-(float)adc_par.Zero[1]/1000;//*斜率-零位值
//	return vfd_speed;
//}


#endif
