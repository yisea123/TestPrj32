#include "dac.h"
 	
CDV_INT32S* g_dacval = (CDV_INT32S*)g_modbusInReg.var + 4;
//缓存输出值
CDV_INT32S dac_val_tmp[4] = {0};

DAC_STRUCT dac_par[Dac_no] = {0,0,0,0,0,0,0,0,0,0};//dac参数结构

CDV_INT32S  transducer_speed;//变频器转速
CDV_INT32S  transducer_time;//变频器加速时间
float transducer_vol;//变频器转速对应电压值


CDV_INT32S  transducer1_speed;//变频1器转速
CDV_INT32S  transducer1_time;//变频1器加速时间
float transducer1_vol;//变频器1转速对应电压值

u16 transducer2_speed;//变频2器转速
u8 transducer2_time;//变频2器加速时间
float transducer2_vol;//变频器2转速对应电压值


u32  Get_Da_vol(u8 no)
{
  return dac_par[no].vol;
	
}


//DAC通道1输出初始化
void Dac1_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); //使能DAC时钟
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; //模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC通道1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
}
//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
//	double temp=vol;
//	temp/=1000;
//	temp=temp*4095/3.3;
//	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
	if(vol>4095)vol=4095;
	DAC_SetChannel1Data(DAC_Align_12b_R,vol);//12位右对齐数据格式设置DAC值
}

//DAC通道2输出初始化
void Dac2_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_2,&DAC_InitType);	 //初始化DAC通道2

	DAC_Cmd(DAC_Channel_2, ENABLE);  //使能DAC通道2
  
  DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
}
//设置通道2输出电压
//vol:0~3300,代表0~3.3V
void Dac2_Set_Vol(u16 vol)
{
	double temp=vol;
	if(temp > 4095 )temp = 4095;
//	temp/=1000;
//	temp=temp*4095/3.3;
	DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}

//******************************************************
//输出电压值
//dac_num:0-3 对应dac通道1-4
//vol：输出电压
//******************************************************
void Dac_set_Voltge( CDV_INT08U dac_num, float vol)
{
	double temp=vol;
	u16 *buf;
  u16 lens;
	u16 adc_val;
	u16 i;
	if(temp>4095)
		temp= 4095;
	
	dac_par[dac_num].vol = temp;
		if( dac_num == 0X00 )
		{
			 Dac1_Set_Vol(temp);
		}
		else if( dac_num == 0X01 )
		{
			 Dac2_Set_Vol(temp);
		}	
		else if( dac_num == 0X02 )
		{
			 WriteToDA2C_7512(temp);
		}	
		else if ( dac_num == 0X03 )
		{
			 WriteToDAC7512(temp);
		}
		
		else
		{
			;
		}		
	
}

//******************************************************
//输出电压值
//dac_num:0-3 对应dac通道1-4
//vol：输出电压
//******************************************************
CDV_INT32S Dac_Get_Voltge( CDV_INT08U dac_num)
{
	return dac_par[dac_num].vol*1000/385;
}

//*******************************************
//CDV dac 启动
//start_val：起始电压 mv
//end_val：目标电压 mv
//run_time :过度时间ms
//*******************************************
RET_STATUS CDV_DacRun(u32 start_val,u32 end_val,u32 run_time)
{
	u8 flag;
	u32 i;
	float val;//差值
	u32 dotimes;
	float pval;
	float run_val=0;
	if(end_val>start_val)
	{
		val=end_val-start_val;
		flag=1;
	}	
	else
	{
		val=start_val-end_val;
		flag=0;
	}	
	dotimes=run_time/100;//每100ms一次
	pval=val/dotimes;
	run_val=start_val+pval;
	for(i=0;i<dotimes;i++)
	{
		Dac1_Set_Vol(run_val);
		delay_ms(100);
		if(flag==1)
		{	
			run_val=run_val+pval;
		}
    else
    {
			run_val=run_val+pval;
		} 			
	}
	return OPT_SUCCESS;
}

//*******************************************
//CDV dac 停止
//*******************************************
RET_STATUS CDV_DacStop()
{
	return OPT_SUCCESS;
}

//*******************************************
//CDV dac 等待
//*******************************************
RET_STATUS CDV_DacWait()
{
	return OPT_SUCCESS;
}

//*******************************************
//CDV dac
//
//
//
//
//*******************************************
RET_STATUS DacCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CMD_ARG *arg)
{
	u8 i=0 , j=0;
	RET_STATUS  ret =OPT_SUCCESS;
	CDV_INT08U  opt , type ;  //功能码 ，功能类型
	CDV_INT32U  no ;    //资源号
	CDV_INT32U  pflag;   //参数标记
	u8  Para_len;   //读取参数长度
	//
  CDV_INT32U  var_num;  //存变量号
	u32  var_val;   //存变量值
	u8  da_num;   //da号
	u16  transducer_vol,vol;  //变频器转速对应电压值
	u8 adc_num;
	no  = *(CDV_INT32U*)(rxBuf);    
	opt = rxBuf[4];
	type = rxBuf[5];
  da_num = rxBuf[0];
	
	if(type!=0xed)	
	  pflag = (rxBuf[9]<<24)+(rxBuf[8]<<16)+(rxBuf[7]<<8)+rxBuf[6];
//	pflag = *(CDV_INT32U*)&rxBuf[6];
	
	
//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];
//	pflag= (rxBuf[3]<<8)+rxBuf[4];
//  da_num = rxBuf[0];
	switch(opt) 
	{
		//启动	     
		case 0x02:    //输出
			           if(type==0xed)	
								 {
//									 transducer_speed = Arithmetic((char*)rxBuf + 6, rxLen - 6);
//									 transducer_vol=(float)transducer_speed/1000*385;   //1000对应1V  10000对应10V模拟量输出
									 dac_val_tmp[no] = ArithmeticEx((char*)rxBuf + 6, rxLen - 6, arg);//transducer_vol;//暂存，在update中输出
									 //Dac_set_Voltge( no,transducer_vol);
								 }
								 else	if(type==0xff)
								 {	
									if(pflag & 0x00000001) //输出转速
									{	
//										transducer1_speed=(rxBuf[7]<<8)+rxBuf[8];
//										transducer1_time=rxBuf[12];
										transducer_speed=(rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10];
										transducer_time=(rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14];
										if(transducer_speed>TRA_SPEED_LIMIT)transducer1_speed = TRA_SPEED_LIMIT;
										
									}	
										
								 }
								 else if(type==0xfe)	
								 {
										if(pflag & 0x00000001) //输出值  气压值转换成电压值输出
										{	
											transducer_speed=VarGet((rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10]);
											transducer_time=VarGet((rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14]);
											if(transducer_speed>TRA_SPEED_LIMIT)transducer_speed = TRA_SPEED_LIMIT; 
										}	
										
								 }
								 else {
									 return ret;
								 }								 
//										transducer_vol=(float)transducer_speed-dac_par[no].Zero;  // 
//										transducer_vol=transducer_vol/125;
//										transducer_vol=transducer_vol*((float)dac_par[no].Slope/1000); //*上斜率
	                  
									   
//						 }	
//			        if(no==0) //比例阀控制
//							{	
//									if(type==0xff)
//									{	
//										if(pflag & 0x00000001)//输出值
//										{	
//											Struct_Valve[no].Ptarget=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]; //获取压力值
//											if(Struct_Valve[no].Ptarget>280)Struct_Valve[no].Ptarget = 280;
//											i=i+4;
//										}	
//										if(pflag & 0x00000002)//过渡时间
//										{	
//											Struct_Valve[no].Inflating_Times=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]; //获取过渡时间，单位S
//											i=i+4;
//										}
//										ProportionalValve_Init();
//								 }
//								 else if(type==0xfe)	
//								 {
//										if(pflag & 0x00000001)//输出值
//										{	
//											Struct_Valve[no].Ptarget=VarGet((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
//											//Struct_Valve.Ptarget_Rel = (Dish_Weight-Struct_Valve.Ptarget)*1.2522;
//											Struct_Valve[no].Ptarget_Rel = (242-Struct_Valve[no].Ptarget)*1.2522;
//											i=i+4;
//										}	
//										if(pflag & 0x00000002)//过渡时间
//										{	
//											Struct_Valve[no].Inflating_Times=VarGet((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
//											i=i+4;
//										}
//										ProportionalValve_Init();
//								 }
//								 else
//								 {
//									 return ret;
//								 }	
//						 }
//             else  if(no==1)//DAC2变频器1控制
//						 {
//								if(type==0xff)
//								{	
//									if(pflag & 0x00000001) //输出转速
//									{	
////										transducer1_speed=(rxBuf[7]<<8)+rxBuf[8];
////										transducer1_time=rxBuf[12];
//										transducer1_speed=(rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10];
//										transducer1_time=(rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14];
//										if(transducer1_speed>TRA1_SPEED_LIMIT)transducer1_speed = TRA1_SPEED_LIMIT;
//										
//									}	
//										
//								 }
//								 else if(type==0xfe)	
//								 {
//										if(pflag & 0x00000001) //输出值
//										{	
////											transducer1_speed=VarGet(rxBuf[5]);
////											transducer1_time=VarGet(rxBuf[6]);
//											transducer1_speed=VarGet((rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10]);
//											transducer1_time=VarGet((rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14]);
//											if(transducer1_speed>TRA1_SPEED_LIMIT)transducer1_speed = TRA1_SPEED_LIMIT; 
//										}	
//										
//								 }
//								 else
//								 {
//									 return ret;
//								 }
//								 void Dac_Set_Vol (DAC_STRUCT dac_par);
//								 transducer1_vol=(float)transducer1_speed/33*1000;//10转/V
//								 transducer1_vol=transducer1_vol-(float)dac_par.Zero[no];
//								 transducer1_vol=transducer1_vol*((float)dac_par.Slope[no]/10000);//*上斜率

//								 Dac2_Set_Vol(transducer1_vol);
//						 }	
//             else if(no==2)//DAC3变频器2控制
//						 {
//								if(type==0xff)
//								{	
//									if(pflag & 0x00000001)//输出转速
//									{	
////										transducer2_speed=(rxBuf[7]<<8)+rxBuf[8];
////										transducer2_time=rxBuf[12];
//										transducer2_speed=(rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10];
//										transducer2_time=(rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14];
//										if(transducer2_speed>TRA2_SPEED_LIMIT) transducer2_speed = TRA2_SPEED_LIMIT;
//										
//									}	
//										
//								 }
//								 else if(type==0xfe)	
//								 {
//										if(pflag & 0x00000001)//输出值
//										{	
////											transducer2_speed=VarGet(rxBuf[5]);
////											transducer2_time=VarGet(rxBuf[6]);
//											transducer2_speed=VarGet((rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10]);
//											transducer2_time=VarGet((rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14]);
//											if(transducer2_speed>TRA2_SPEED_LIMIT) transducer2_speed = TRA2_SPEED_LIMIT; 
//										}	
//										
//								 }
//								 else
//								 {
//									 return ret;
//								 }
//								 //transducer_vol=transducer_speed/33*1000; //10转/V
//								 transducer2_vol=(float)transducer2_speed/33*1000; //10转/V，DA输出放大3.3倍，3.3*10=33，电压单位mv
//								 transducer2_vol=transducer2_vol-(float)dac_par.Zero[no];
//								 transducer2_vol=transducer2_vol*((float)dac_par.Slope[no]/10000); //*上斜率
//								 
//								 WriteToDA1C_7512(transducer2_vol);
//								 // Dac2_Set_Vol(transducer_vol);
//						 }														

		      break;

#ifdef ENABLE_PID
		//参数设置
		case 0x00:		
                if((type==0xFF)||(type==0x2F))
								{	
										if(pflag & 0x00000001) //斜率值
										{	
											dac_par[no].Slope=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}	
										if(pflag & 0x00000002) //零位值
										{	
											dac_par[no].Zero=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}	
										if(pflag & 0x00000004) //输出最小值
										{	
											dac_par[no].Out_Min=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}	
										if(pflag & 0x00000008) //输出最大值
										{	
											dac_par[no].Out_Max=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x00000010) //DA通道值
										{	
											dac_par[no].Dac_num=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];											
											i=i+4;
										}
										if(pflag & 0x00000020) //系数1
										{	
											dac_par[no].Da_Coeff1=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x00000040) //系数2
										{	
											dac_par[no].Da_Coeff2=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										
										PIDInit_Valve();
								 }		
						 else if((type==0xFE)||(type==0x2E)) //按变量设参数
							   { 
								    var_num=(rxBuf[13+j]<<24 )+(rxBuf[12+j]<<16)+(rxBuf[11+j]<<8)+rxBuf[10+j]; //取变量号
										var_val = VarGet(var_num); //取变量值
								    if(pflag & 0x00000001) //斜率值
										{	
											dac_par[no].Slope=var_val;
										}	
										if(pflag & 0x00000002) //零位值
										{	
											dac_par[no].Zero=var_val;
										}	
										if(pflag & 0x00000004) //输出最小值
										{	
											dac_par[no].Out_Min=var_val;
										}	
										if(pflag & 0x00000008) //输出最大值
										{	
											dac_par[no].Out_Max=var_val;
										}
										if(pflag & 0x00000010)//DA通道
										{	
											dac_par[no].Dac_num=var_val;
										}
										if(pflag & 0x00000020)//系数1
										{	
											dac_par[no].Da_Coeff1=var_val;
										}
										if(pflag & 0x00000040)//系数2
										{	
											dac_par[no].Da_Coeff2=var_val;
										}
										
										PIDInit_Valve();
							 } 	 
							if((type==0x2F) || (type==0x2E)) //不是流程中修改参数，保存参数值到flash
	            {								 
								SPI_Flash_Write((CDV_INT08U *)&dac_par, DAC_PAR_ADDR, sizeof(dac_par)); //写入flash
							}								
							break; 			
//			        if(da_num==0)
//							{	
//								PressureAD_Zero	=	(rxBuf[5+i]<<24)+(rxBuf[6+i]<<16)+(rxBuf[7+i]<<8)+rxBuf[8+i];
//							}
//              else if(da_num==1)
//              {
//								AD_COEFFICIENT	=	((rxBuf[5+i]<<24)+(rxBuf[6+i]<<16)+(rxBuf[7+i]<<8)+rxBuf[8+i])/10000;
//							}			
//		          break;
		//参数读取
		case 0x01:
					if((type==0xFF)||(type==0x2F))
						{	
							u8  Read_Par_Buf[48];   // 读取参数缓存
							Para_len=Read_Dac_Par(Read_Par_Buf,no,pflag);
							ResRequest(arg->buf, arg->len, Read_Par_Buf,Para_len,arg);
						}	
		        else if((type==0xFE)||(type==0x2E))//参数读到变量
						{	
							u8  Read_Par_Buf[48];   // 读取参数缓存
							var_val = 0;
							Para_len=Read_Dac_Par(Read_Par_Buf,no,pflag);
							for(i=0;i<Para_len;i++)
							{
								var_val=var_val+(Read_Par_Buf[i]<<(8*(Para_len-i-1)));//buf是4Byte，一次只能一个变量，每个变量最大存取4个字节数值
							}
							ValSet(rxBuf[10],var_val);
							
						}
		        break;
//		case 0x05:
//			      Present_Pressure_AD = Get_Adc_Voltge(adc_num);
//		         break;
//		case 0x06:
//			       RunProcess(&Struct_Valve[no]);
//						break;
//		case 0x07:
//			     Dac_set_Voltge( da_num, (u16)Present_Valve_DA); 
//		
//		        break;
		
		//停止
		case 0x03:
			   Process_Start =0; 				
//				 PressureAD_Zero = PressureAD_Zero_1;	//压力零位
				 AD_COEFFICIENT=AD_COEFFICIENT_1;
				 R_PressureAD_Zero = R_PressureAD_Zero_1;
				 R_AD_COEFFICIENT = R_AD_COEFFICIENT_1;
		     break;
		//等待
		case 0x04:
			        if(Run_Stat==1)
							{
								ret=OPT_SUCCESS;
							}	
							else
							{
								ret=OPT_FAILURE;
								//delay_ms(1);
							}	
		     break;
#endif
		case 0xFF:/*查询*/
			var_val = DacRead(no);//Dac_Get_Voltge(no);
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&var_val), 4, arg, RC_CRC);
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
u8 Read_Dac_Par(u8 *buf,u8 no,u32 pflag)
{
	u8 i,j;
	u32 FPGA_POS;
	u16 FPGA_POSH;
	u16 FPGA_POSL;
	i=0;
	 if(pflag & 0x00000001)//斜率值
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par[no].Slope>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000002)//零位值
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par[no].Zero>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000004)//输出最小值
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par[no].Out_Min>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000008)//输出最大值
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par[no].Out_Max>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000010) //DA通道
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par[no].Dac_num >>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000020) //系数1   
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par[no].Da_Coeff1 >>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000040) //系数2     
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par[no].Da_Coeff2 >>(24-8*j);
			}
			i=i+4;
	 }
	return i;//返回长度
	 
}	
//RET_STATUS DacCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,u8 uartNo)
//{
//	u8 i=0;
//	RET_STATUS ret =OPT_SUCCESS;
//	CDV_INT08U no , opt , type; //资源号，功能码，功能类型
//	u16 pflag;//参数标记
//	u32 end_val=0;//输入的数字量
//	u32 run_time=0;//运行时间
//	float current_val;
//	float pre_val;//压力值Kp
//	float get_pre_val;//获取的压力值Kp
//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];
//	pflag= (rxBuf[3]<<8)+rxBuf[4];
//  
//	switch(opt) 
//	{
//		//参数设置
//		case 0x00:
//			    
//		       break;
//		//参数读取
//		case 0x01:
//			 
//		       break;
//		//启动	     
//		case 0x02:
//			        if(type==0xff)
//							{	
//								if(pflag & 0x0001)//输出值
//								{	
//									pre_val=(rxBuf[5+i]<<24)+(rxBuf[6+i]<<16)+(rxBuf[7+i]<<8)+rxBuf[8+i];
//									end_val=pre_val/50*1000;//mv
//									i=i+4;
//								}	
//								if(pflag & 0x0002)//过渡时间
//								{	
//									run_time=(rxBuf[5+i]<<24)+(rxBuf[6+i]<<16)+(rxBuf[7+i]<<8)+rxBuf[8+i];
//									i=i+4;
//								}
//						 }
//						 else if(type==0xfe)	
//						 {
//								if(pflag & 0x0001)//输出值
//								{	
//									end_val=DW_CDV_VAR(rxBuf[5+i]);
//									i=i+1;
//								}	
//								if(pflag & 0x0002)//过渡时间
//								{	
//									run_time=DW_CDV_VAR(rxBuf[5+i]);
//									i=i+1;
//								}
//						 }
//             else
//						 {
//							 return ret;
//						 }	
//             current_val=Get_Adc_Average(ADC_Channel_0,20);
//						 current_val=current_val/4095*3.3;
//						 if(current_val<1)
//						 {
//								current_val=0;
//						 }	
//             else
//             {							 
//							  current_val=(current_val-1);
//						 }	 
//						 get_pre_val=current_val*125;
//						 current_val=get_pre_val/50*1000;//mv
//						 //current_val=current_val*(3.3/4095)*1000;
//						 CDV_DacRun((u32)current_val,end_val,run_time);						 
//		      break;
//		//停止
//		case 0x03:
//			
//		     break;
//		//等待
//		case 0x04:
//			
//		     break;
//		default:
//			    break;
//		
//	}	
//	return ret;
//	
//}

void  Dac_Set_Vol (DAC_STRUCT *dac_par,u8 no)
{
    transducer_vol=(float)transducer_speed/33*1000; //10转/V，DA输出放大3.3倍，3.3*10=33，电压单位mv
	  transducer_vol=transducer_vol-(float)dac_par[no].Zero;
	  transducer_vol=transducer_vol*((float)dac_par[no].Slope/10000); //*上斜率
	
	  Dac_set_Voltge ( dac_par[no].Dac_num ,transducer_vol);
}

u16  Get_Da_value(u8 no)
{
  float value = (dac_par[no].vol * 10000 / (float)dac_par[no].Slope + (float)dac_par[no].Zero) * 33 / 1000;
	return value;
}


//刷新dac的值到g_adcval
void UpdateDacVal(void) {
	int i = 0;
	
	
	for (i = 0; i < DA_NUM; i++) {
	  Dac_set_Voltge(i, (float)dac_val_tmp[i]/1000*385);
		g_dacval[i] = Dac_Get_Voltge(i);
	}
}

CDV_INT32S DacRead(CDV_INT08U no) {
	CDV_INT16U localaddr;
//	CDV_INT16U remoteaddr;
//	CDV_INT08U host;
	
	if (no < DA_NUM)
		return g_dacval[no];
	
	CascadeModbus_MapFindInExp(2, no, &localaddr, NULL, NULL);
	
	return g_modbusReg.reg[localaddr];
}