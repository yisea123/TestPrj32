#ifndef  _MOTO_H
#define  _MOTO_H
#include "cdv_include.h"  

#define FPGA_W_FRE 100000000 //FPGA����Ƶ��
#define FPGA_W_FRE_DIV2 (FPGA_W_FRE/2) //FPGA����Ƶ�ʶ���Ƶ
#define FPGA_SPEED_CHANGE_CYCLE 0.005  //����ٶȱ仯����Ϊ0.005s ��5ms
#define FPGA_PROTOCOL_48	//FPGAЭ���ʽΪ16λ��ַ 32λ����
#define FPGA_ACC_DIV 256

//#define FPGA_SMODE_SPEED_CHANGE_CYCLE 0.005  //����ٶȱ仯����Ϊ0.005s ��5ms
#define MULT_SPEEND_MODE 2//�����
#define S_MODE 1//s�ͼӼ���
#define TRAP_MODE 0//���μӼ���
#define AXLE_NUM 8

#define BIGENDIAN 1
#define LITTLEEND 0
//#define STORAGEPATTERN LITTLEEND

#define FPGA_COMMAND_LEN 6
extern CDV_INT32S gmoto_pos[8];
extern u8 gmoto_flag2;
extern u32 bufmax_speed;
extern u8 gmoto_flag[AXLE_NUM] ;

////	CString drvtype[AxisNum] ;//�������� 
////	CString max_speed[AxisNum] ;//����ٶ�
////	CString min_speed[AxisNum] ;//����ٶ�
////	CString up_limit[AxisNum] ;//����λ
////	CString down_limit[AxisNum] ;//����λ
////	CString zero_dir[AxisNum];//���㷽��	
////	CString pstv_dir[AxisNum];//������
////	CString div_num[AxisNum];//ϸ����
////	CString pul_mode[AxisNum] ;//����ģʽ
////	CString max_acc[AxisNum];//��߼Ӽ��ٶ�
////	CString min_acc[AxisNum];//��߼Ӽ��ٶ�
////	CString zero_mode[AxisNum] ;//����ģʽ
////	CString run_mode[AxisNum];//�˶�ģʽ
////	CString encoder_type[AxisNum];//����������
////	CString backlash_comp[AxisNum] ;//��϶����

//����������
typedef struct{
	u8 drvtype[AXLE_NUM] ;//�������� 
	u32 max_speed[AXLE_NUM] ;//����ٶ�
	u32 min_speed[AXLE_NUM] ;//����ٶ�
	u32 up_limit[AXLE_NUM] ;//����λ
	u32 down_limit[AXLE_NUM] ;//����λ
	u8 zero_dir[AXLE_NUM];//���㷽��
	u8 pstv_dir[AXLE_NUM];//������
	u16 div_num[AXLE_NUM];//ϸ����
	u8 pul_mode[AXLE_NUM] ;//CW��CCwģʽ�л�
	u32 max_acc[AXLE_NUM] ;//��߼Ӽ��ٶ�
	u32 min_acc[AXLE_NUM] ;//��߼Ӽ��ٶ�
	u8 zero_mode[AXLE_NUM] ;//����ģʽ
	u8 run_mode[AXLE_NUM] ;//�˶�ģʽ
	u8 encoder_type[AXLE_NUM] ;//����������
	u8 backlash_comp[AXLE_NUM] ;//��϶����	
	u32 encoder_pre[AXLE_NUM] ;//����������
	u16 diameter[AXLE_NUM];//�ֶ�Բֱ��
	u16 lead[AXLE_NUM] ;//˿�ܵ���		
	
	u8 speed_type[AXLE_NUM] ;//�ٶ���������
	u32 acc[AXLE_NUM];//���ٶ�
	u32 dec[AXLE_NUM] ;//���ٶ�	
	u32 zero_speed[AXLE_NUM] ;//�����ٶ�	
	u32 jog_run_speed[AXLE_NUM] ;//�㶯�ٶ�	
	u32 mult_a_d_speed[AXLE_NUM][10] ;//��μӼ��ٶ�
	u32 mult_speed[AXLE_NUM][10] ;//���Ŀ���ٶ�
	u32 mult_step[AXLE_NUM][10] ;//��β���
	u8 mult_speed_num[AXLE_NUM];//��ζ���
	//Pcf added in April 29
	u32 target_speed[AXLE_NUM] ;//Ŀ�������ٶ�
	u16 SModel_flexible[AXLE_NUM] ;//S���ٶ���������
	u32 DecStep[AXLE_NUM] ;//���ٲ���
} MOTOR_INFO;
extern MOTOR_INFO moto_par;
//������״̬
typedef struct{
u32 pos[AXLE_NUM];//��ǰλ��
u32 encoder_cnt[AXLE_NUM];//����������
u32 speed[AXLE_NUM];	//��ǰ�ٶ�
u8 p_limit[AXLE_NUM];	//Ӳ����λ״̬
u8 n_limit[AXLE_NUM]; //Ӳ����λ״̬
u8 zero[AXLE_NUM]; 	//Ӳ�����ź�
u32 runtime[AXLE_NUM]; //����Ԥ��������ʱ��
u32 ela_runtime[AXLE_NUM];	//������ʱ��
u32 target_pos[AXLE_NUM] ;//Ŀ��λ��
u8 enable[AXLE_NUM];//ʹ��	
}MOTOR_STATE;
extern MOTOR_STATE moto_sta;
//ֱ�߲岹�������
typedef struct{
	u8 drvtype[4][1] ;//�������� 
	u8 max_speed[4][4] ;//����ٶ�
	u8 min_speed[4][4] ;//����ٶ�
	u8 acc[4][4] ;//���ٶ�
	u8 dec[4][4] ;//���ٶ�
	u8 up_limit[4][4] ;//����λ
	u8 down_limit[4][4] ;//����λ
  u8 quadrant[4][1];//�岹����
	u8 x_pos_dir[4][1];//x��������
	u8 y_pos_dir[4][1];//y��������
} LINE_INFO;
extern LINE_INFO line_par;
//Բ���岹�������
typedef struct{
	u8 drvtype[8][1] ;//�������� 
	u8 max_speed[8][4] ;//����ٶ�
	u8 min_speed[8][4] ;//����ٶ�
	u8 acc[8][4] ;//���ٶ�
	u8 dec[8][4] ;//���ٶ�
	u8 up_limit[8][4] ;//����λ
	u8 down_limit[8][4] ;//����λ
	u8 dir[4][1];//˳��ʱ��
	u8 rel_dir[4][2];//����x.y���򣬺�Բ���귽��
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
RET_STATUS MotorChk(CDV_INT08U no , CDV_INT08U opt);//������˶��Ƿ����
//RET_STATUS MotorChk(CDV_INT08U no , CDV_INT08U opt);//�����ѯ
//RET_STATUS MotorStop(CDV_INT08U no );//�����ͣ
RET_STATUS MotorCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg); 
void AllMotorCTL(CDV_INT08U ctl) ;
u8 Line_Arc_Config(u8 no);//���������ʼ��
u32 UartToU32(u8 * src,u8 nByte, u8 iBigEndian);
void MotoResParRead(void);
CDV_INT32U CalculateSModelLine(CDV_INT08U Moto_No);
RET_STATUS RealTimeSpeedSet(CDV_INT08U Moto_No);
#endif



