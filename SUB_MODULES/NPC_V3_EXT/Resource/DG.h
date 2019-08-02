//#include "stm32f4xx.h"

#ifndef  _CDV_DG_
#define  _CDV_DG_


#include "cdv_include.h"
extern u8 DG_Receive_Data[16];
extern u32 DG_Receive_Len;

typedef struct{
	u8 action_type[33][2];//动作方法1：绝对，0：相对
	u8 speed[33][2];//速度
	u8 coord[33][4];//位置
	u8 acc[33][2] ;//加速度
	u8 dec[33][2] ;//减速度
	
	u8 tyl[33][2];//推压力
	u8 space[33][2];//阈值（范围值）
	u8 tys[33][2];//推压速度
	u8 max_tl[33][2];//最大定位推力
	u8 out_space1[33][4] ;//区域输出端1
	u8 out_space2[33][4] ;//区域输出端2
	u8 dwf[33][4];        //定位幅
	u8 start_directly[33][2];//直接指定位置启动
	u8 Controler_Id;        //控制器ID,默认1	
} DG_PAR;

typedef struct{
	u8 DG_No;//电缸号
	u8 DG_Name_Len;//电缸名称长度
	u8 DG_Name_Buf[16];//电缸名称
	
} DG_Script;



void Enable_serial(u8 ID);//串行使能
void Hold(u8 ID,u8 state);//暂时停止
//void Rehold(u8 ID);//取消暂时停止
void SVON(u8 ID,u8 state);//伺服ON
void Verify_SVON(u8 ID);//确认伺服完成
u8 Wait_SVON(void);//等待伺服完成
void Setup(u8 ID);//归零
void Verify_Setup(u8 ID);//确认归零是否完成
void Over_Setup(u8 ID);//归零动作结束
u8 Wait_Setup(u8 data);//等待归零完成
void Reset(u8 ID);//重启
void Verify_Reset(u8 ID);//确认重启
void Over_Reset(u8 ID);//等待重启完成
void DG_Ready(u8 ID,u8 state);//电缸准备
u16 getCRC16(u8*ptr,u16 len);//校验码生成
void Send_Directly_Run_Code(u8 ID);//发送启动直接运行的指令

void Paramater_Parse(u16 addr);
void Start_directly_run(u8 ID);
u8 Verify_Run_Over(u8 ID);
RET_STATUS Wait_Run_Over(u8 ID);

void Set_Action_Type(u8 ID,u8 Step_Num,u8 dataH,u8 dataL);
void Set_Speed(u8 ID,u8 Step_Num,u8 dataH,u8 dataL);
void Set_Coord(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2);
void Set_Acc(u8 ID,u8 Step_Num,u8 dataH,u8 dataL);
void Set_Dec(u8 ID,u8 Step_Num,u8 dataH,u8 dataL);
void Set_Tyl(u8 ID,u8 Step_Num,u8 dataH,u8 dataL);
void Set_Space(u8 ID,u8 Step_Num,u8 dataH,u8 dataL);
void Set_Tys(u8 ID,u8 Step_Num,u8 dataH,u8 dataL);
void Set_Max_Tl(u8 ID,u8 Step_Num,u8 dataH,u8 dataL);
void Set_Out_Space1(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2);
void Set_Out_Space2(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2);
void Set_Dwf(u8 ID,u8 Step_Num,u8 dataH1,u8 dataH2,u8 dataL1,u8 dataL2);
	
void Process_Directly_Run_Code(u8 ID);

u8 Select_Step(u8 ID,u8 Step_Num);
u8 Activate_Point_Run(u8 ID);
u8 Over_Point_Run(u8 ID);

u8 Point_run(u8 ID,u8 Step_Num);
u8 DG_Script_Read_And_Config(u32 addr,u8 num);
RET_STATUS DG_RUN(u8* rxBuf,u8 rxLen/*, u8 debug*/,CDV_INT08U uartNo);

u8 Read_Current_Inf(u8 DG_id,u8 DG_points,u16 addr);
void Read_Point_Para(u8 ID,u8 Step_Num,u8 Para_No);
void DGRefresh(void);
#endif





