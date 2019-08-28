#ifndef __PROPORTIONALVALVE_H	
#define __PROPORTIONALVALVE_H	 
#include "cdv_include.h" 
#define RX_ADC1_DMA 200 //本来是1000
#define ITV2050 150     //比例阀型号2050对应270，比例阀型号2030对应150；
#define fen_ya_val 1.606 //比例阀的分压电阻系数 5.3kΩ/3.3KΩ=1.606，如果没接分压电阻设置成1
#define PID_no 4
#define PID_NUM  4
typedef struct {
	CDV_FP64  SetPoint;          
	//  设定目标 Desired Value      
	CDV_FP64  LastSetPoint; 
	//  上一次调节设定的Value  
	CDV_INT32U  Proportion;         
	//  比例常数 Proportional Const   
	CDV_INT32U  Integral;         
  //  积分常数 Integral Const       
  CDV_INT32U  Derivative;        
	//  微分常数 Derivative Const  
	CDV_FP64  LastError;          //  Error[-1]  
	CDV_FP64  PrevError;          //  Error[-2]  
	CDV_FP64  SumError;           //  Sums of Errors
	CDV_INT32U  Integral_Limit;     // 积分上限
	
 	CDV_INT32U  Ctrl_Time;     		// 控制间隔单位ms
	CDV_INT32U  AD_no;   			  //  AD通道
	CDV_INT32U  DA_no;          //  DA通道
	float Present_Pressure_AD[PID_NUM];
 }PID;

typedef struct {
	CDV_FP64  SetPoint;          
	//  设定目标 Desired Value      
	CDV_FP64  LastSetPoint; 
	//  上一次调节设定的Value  
	CDV_FP64  Proportion;         
	//  比例常数 Proportional Const   
	CDV_FP64  Integral;         
  //  积分常数 Integral Const       
  CDV_FP64  Derivative;        
	//  微分常数 Derivative Const  
	CDV_FP64  LastError;          //  Error[-1]  
	CDV_FP64  PrevError;          //  Error[-2]  
	CDV_FP64  SumError;           //  Sums of Errors
	CDV_FP64  Integral_Limit;     //  积分上限
	CDV_INT32U  AD_no;   			  //  AD通道
	CDV_INT32U  DA_no;          //  DA通道
 	CDV_FP64  Ctrl_Time;     // 控制间隔 单位ms
 }PID_STRUCT;

typedef struct 
{
	u8 u8Enable;
	CDV_INT16U Inflating_Times;	//加压时间
	CDV_INT16U Ptarget;//目标气压	Kpa
	CDV_INT16U Ptarget_Rel;//目标压力 KG	
}ProportionalValve;
extern PID_STRUCT Pid_par[PID_no];
extern PID Valve_Pid[PID_NUM];
void LcdSet_UI(ProportionalValve* ProportionalValve_Struct);
void LcdRun_UI(void);
void ProportionalValve_Init(void);
//double RunProcess(ProportionalValve* ProportionalValve_Struct, PID* Valve_Pid);
double RunProcess(u16 no);
double RunProcessIndex(u8 u8PidIndex);
//void RunProcess(ProportionalValve* ProportionalValve_Struct);
//void Down_Slow(void);
//void Up_Slow(void);
u16 Ptarget_Offset(u16 Target);


extern ProportionalValve Struct_Valve[PID_NUM];
extern double Present_Valve_DA ;
extern float Present_Pressure_AD ;
extern u16 R_Pressure_AD;
extern double R_Pressure;	
extern u8  ADSigPre_Flag,Lcd_Refresh_Flag;
extern u16 PressureValveDA_Zero[PID_NUM];
extern u8 Process_Start,Process_Exit;
extern u8 Sequence;
extern double LastPressure_Sys;
extern double Pressure_Coefficient;
extern u8 Run_Stat;
extern CDV_INT32U Run_time;
extern u8 ValveDAadjust_Flag; //50ms
extern u16 Ptarget_Num;
extern CDV_INT16U ADC_ConvertedValue[RX_ADC1_DMA];
extern u16 Dish_Weight;
void PIDInit_Valve(void);
void PIDClear_Valve(void);
//extern double Present_Pressure_Rel;
extern u16 PressureAD_Zero,PressureAD_Zero_1,PressureAD_Zero_2;
extern double AD_COEFFICIENT,AD_COEFFICIENT_1,AD_COEFFICIENT_2;
extern u16 R_PressureAD_Zero,R_PressureAD_Zero_1,R_PressureAD_Zero_2;
extern double R_AD_COEFFICIENT,R_AD_COEFFICIENT_1,R_AD_COEFFICIENT_2;
CDV_FP64 PIDcal(PID *pp,CDV_FP64 PresPoint,CDV_FP64 SetPoint);
CDV_FP64 PIDcal1(PID_STRUCT *pp, CDV_FP64 SetPoint);
void R_Pressure_Measure(void);
void Valve_Debug(void);
void Valve_PSensor_Coe_Init(void);
void Valve_PSensor_Coe_Switchover(void);
#endif
