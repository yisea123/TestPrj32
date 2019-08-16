#include "dac.h"
 	
// DAC_STRUCT dac_par[Dac_no];//dac参数结构

CDV_INT32S  transducer_speed;//变频器转速
CDV_INT32S  transducer_time;//变频器加速时间
float transducer_vol;//变频器转速对应电压值


CDV_INT32S  transducer1_speed;//变频1器转速
CDV_INT32S  transducer1_time;//变频1器加速时间
float transducer1_vol;//变频器1转速对应电压值

u16 transducer2_speed;//变频2器转速
u8 transducer2_time;//变频2器加速时间
float transducer2_vol;//变频器2转速对应电压值
//DAC通道1输出初始化

void Dac1_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //使能PORTA通道时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				  // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 输出高
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值

}
//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
//	double temp=vol;
//	temp/=1000;
//	temp=temp*4095/3.3;
//	DAC_SetChannel1Data(DAC_Align_12b_R,temp); //12位右对齐数据格式设置DAC值
	if(vol>4095) 
		vol=4095;
	DAC_SetChannel1Data(DAC_Align_12b_R,vol);	 //12位右对齐数据格式设置DAC值
			
}


void Dac2_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	 //使能PORTA通道时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);  //PA.5 输出高
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC2输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_2,&DAC_InitType);	 //初始化DAC通道2

	DAC_Cmd(DAC_Channel_2, ENABLE);  //使能DAC2
  
  DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值

}

//设置通道2输出电压
//vol:0~3300,代表0~3.3V
void Dac2_Set_Vol(u16 vol)
{
//	double temp=vol;
//	temp /=1000;
//	temp = temp*4095/3.3;
//	DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
	if(vol > 4095 )
		vol = 4095;
	DAC_SetChannel2Data(DAC_Align_12b_R,vol);//12位右对齐数据格式设置DAC值
	
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


void	DAC_SETOUT(void)
	{
			u8 i;
		 for(i=0;i<6;i++)
		 ;
//	Dac_set_Voltge(i,500);
	}


