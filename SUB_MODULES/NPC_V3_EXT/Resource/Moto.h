#ifndef  _MOTO_H
#define  _MOTO_H
#include "cdv_include.h"  

#define FPGA_W_FRE 100000000 //FPGA工作频率
#define FPGA_W_FRE_DIV2 (FPGA_W_FRE/2) //FPGA工作频率二分频
#define FPGA_SPEED_CHANGE_CYCLE 0.005  //电机速度变化周期为0.005s 即5ms
#define FPGA_PROTOCOL_48	//FPGA协议格式为16位地址 32位数据
#define FPGA_ACC_DIV 256

//#define FPGA_SMODE_SPEED_CHANGE_CYCLE 0.005  //电机速度变化周期为0.005s 即5ms
#define MULT_SPEEND_MODE 2//多段速
#define S_MODE 1//s型加减速
#define TRAP_MODE 0//梯形加减速
#define AXLE_NUM 8

#define BIGENDIAN 1
#define LITTLEEND 0
//#define STORAGEPATTERN LITTLEEND

#define FPGA_COMMAND_LEN 6
extern CDV_INT32S gmoto_pos[8];
extern u8 gmoto_flag2;
extern u32 bufmax_speed;
extern u8 gmoto_flag[AXLE_NUM] ;

////	CString drvtype[AxisNum] ;//驱动类型 
////	CString max_speed[AxisNum] ;//最高速度
////	CString min_speed[AxisNum] ;//最低速度
////	CString up_limit[AxisNum] ;//上限位
////	CString down_limit[AxisNum] ;//下限位
////	CString zero_dir[AxisNum];//回零方向	
////	CString pstv_dir[AxisNum];//正方向
////	CString div_num[AxisNum];//细分数
////	CString pul_mode[AxisNum] ;//脉冲模式
////	CString max_acc[AxisNum];//最高加减速度
////	CString min_acc[AxisNum];//最高加减速度
////	CString zero_mode[AxisNum] ;//回零模式
////	CString run_mode[AxisNum];//运动模式
////	CString encoder_type[AxisNum];//编码器类型
////	CString backlash_comp[AxisNum] ;//背隙补偿

//单轴电机参数
typedef struct{
	u8 drvtype[AXLE_NUM] ;//驱动类型 
	u32 max_speed[AXLE_NUM] ;//最高速度
	u32 min_speed[AXLE_NUM] ;//最低速度
	u32 up_limit[AXLE_NUM] ;//上限位
	u32 down_limit[AXLE_NUM] ;//下限位
	u8 zero_dir[AXLE_NUM];//回零方向
	u8 pstv_dir[AXLE_NUM];//正方向
	u16 div_num[AXLE_NUM];//细分数
	u8 pul_mode[AXLE_NUM] ;//CW、CCw模式切换
	u32 max_acc[AXLE_NUM] ;//最高加减速度
	u32 min_acc[AXLE_NUM] ;//最高加减速度
	u8 zero_mode[AXLE_NUM] ;//回零模式
	u8 run_mode[AXLE_NUM] ;//运动模式
	u8 encoder_type[AXLE_NUM] ;//编码器类型
	u8 backlash_comp[AXLE_NUM] ;//背隙补偿	
	u32 encoder_pre[AXLE_NUM] ;//编码器精度
	u16 diameter[AXLE_NUM];//分度圆直径
	u16 lead[AXLE_NUM] ;//丝杠导程		
	
	u8 speed_type[AXLE_NUM] ;//速度曲线类型
	u32 acc[AXLE_NUM];//加速度
	u32 dec[AXLE_NUM] ;//减速度	
	u32 zero_speed[AXLE_NUM] ;//回零速度	
	u32 jog_run_speed[AXLE_NUM] ;//点动速度	
	u32 mult_a_d_speed[AXLE_NUM][10] ;//多段加减速度
	u32 mult_speed[AXLE_NUM][10] ;//多段目标速度
	u32 mult_step[AXLE_NUM][10] ;//多段步长
	u8 mult_speed_num[AXLE_NUM];//多段段数
	//Pcf added in April 29
	u32 target_speed[AXLE_NUM] ;//目标运行速度
	u16 SModel_flexible[AXLE_NUM] ;//S型速度曲线区间
	u32 DecStep[AXLE_NUM] ;//减速步数
} MOTOR_INFO;
extern MOTOR_INFO moto_par;
//单轴电机状态
typedef struct{
u32 pos[AXLE_NUM];//当前位置
u32 encoder_cnt[AXLE_NUM];//编码器计数
u32 speed[AXLE_NUM];	//当前速度
u8 p_limit[AXLE_NUM];	//硬正限位状态
u8 n_limit[AXLE_NUM]; //硬负限位状态
u8 zero[AXLE_NUM]; 	//硬回零信号
u32 runtime[AXLE_NUM]; //单步预计运行总时间
u32 ela_runtime[AXLE_NUM];	//已运行时间
u32 target_pos[AXLE_NUM] ;//目标位置
u8 enable[AXLE_NUM];//使能	
}MOTOR_STATE;
extern MOTOR_STATE moto_sta;
//直线插补电机参数
typedef struct{
	u8 drvtype[4][1] ;//驱动类型 
	u8 max_speed[4][4] ;//最高速度
	u8 min_speed[4][4] ;//最低速度
	u8 acc[4][4] ;//加速度
	u8 dec[4][4] ;//减速度
	u8 up_limit[4][4] ;//上限位
	u8 down_limit[4][4] ;//下限位
  u8 quadrant[4][1];//插补象限
	u8 x_pos_dir[4][1];//x轴正方向
	u8 y_pos_dir[4][1];//y轴正方向
} LINE_INFO;
extern LINE_INFO line_par;
//圆弧插补电机参数
typedef struct{
	u8 drvtype[8][1] ;//驱动类型 
	u8 max_speed[8][4] ;//最高速度
	u8 min_speed[8][4] ;//最低速度
	u8 acc[8][4] ;//加速度
	u8 dec[8][4] ;//减速度
	u8 up_limit[8][4] ;//上限位
	u8 down_limit[8][4] ;//下限位
	u8 dir[4][1];//顺逆时针
	u8 rel_dir[4][2];//物理x.y方向，和圆坐标方向
} ARC_INFO;
extern ARC_INFO arc_par;

typedef enum{
	READY,
	ACC,
	DEC,
	UNIFORM
}MOTOR_RUNSTA;
extern MOTOR_RUNSTA Moto_RunSta[AXLE_NUM];

void Moto_Init(u8 no,u32 Moto_Pflag);
RET_STATUS MotorChk(CDV_INT08U no , CDV_INT08U opt);//检测电机运动是否完成
//RET_STATUS MotorChk(CDV_INT08U no , CDV_INT08U opt);//电机查询
//RET_STATUS MotorStop(CDV_INT08U no );//电机急停
RET_STATUS MotorCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg); 
void AllMotorCTL(CDV_INT08U ctl) ;
u8 Line_Arc_Config(u8 no);//轴组参数初始化
u32 UartToU32(u8 * src,u8 nByte, u8 iBigEndian);
void MotoResParRead(void);
CDV_INT32U CalculateSModelLine(CDV_INT08U Moto_No);
RET_STATUS RealTimeSpeedSet(CDV_INT08U Moto_No);
#endif



