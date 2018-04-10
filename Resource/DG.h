//#include "stm32f4xx.h"

#ifndef  _CDV_DG_
#define  _CDV_DG_


#include "cdv_include.h"
extern u8 DG_Receive_Data[16];
extern u32 DG_Receive_Len;

typedef struct{
	u8 action_type[33][2];//��������1�����ԣ�0�����
	u8 speed[33][2];//�ٶ�
	u8 coord[33][4];//λ��
	u8 acc[33][2] ;//���ٶ�
	u8 dec[33][2] ;//���ٶ�
	
	u8 tyl[33][2];//��ѹ��
	u8 space[33][2];//��ֵ����Χֵ��
	u8 tys[33][2];//��ѹ�ٶ�
	u8 max_tl[33][2];//���λ����
	u8 out_space1[33][4] ;//���������1
	u8 out_space2[33][4] ;//���������2
	u8 dwf[33][4];        //��λ��
	u8 start_directly[33][2];//ֱ��ָ��λ������
	u8 Controler_Id;        //������ID,Ĭ��1	
} DG_PAR;

typedef struct{
	u8 DG_No;//��׺�
	u8 DG_Name_Len;//������Ƴ���
	u8 DG_Name_Buf[16];//�������
	
} DG_Script;



void Enable_serial(u8 ID);//����ʹ��
void Hold(u8 ID,u8 state);//��ʱֹͣ
//void Rehold(u8 ID);//ȡ����ʱֹͣ
void SVON(u8 ID,u8 state);//�ŷ�ON
void Verify_SVON(u8 ID);//ȷ���ŷ����
u8 Wait_SVON(void);//�ȴ��ŷ����
void Setup(u8 ID);//����
void Verify_Setup(u8 ID);//ȷ�Ϲ����Ƿ����
void Over_Setup(u8 ID);//���㶯������
u8 Wait_Setup(u8 data);//�ȴ��������
void Reset(u8 ID);//����
void Verify_Reset(u8 ID);//ȷ������
void Over_Reset(u8 ID);//�ȴ��������
void DG_Ready(u8 ID,u8 state);//���׼��
u16 getCRC16(u8*ptr,u16 len);//У��������
void Send_Directly_Run_Code(u8 ID);//��������ֱ�����е�ָ��

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





