#ifndef  _MODBUS_
#define  _MODBUS_
//#include "f4_includes.h"
#include "cdv_include.h"  

#define MULTIDEVICE_TEST  	0//���豸����  1ʱ��Ӧ������ַ+4�ı���
/*****************************�����붨��******************************************/
#define COIL_CHN   30
#define INCOIL_CHN 30
#define REG_N    600
#define INREG_N  100

/*
*MODBUS���ƶ���
*����modbus�ض���Դ�ļĴ�������Ȧ������������Դ�򽻵�ʱ����
*/

/*ͨ�ò���*/
#define COIL_P               g_modbusInCoil.coilCh/*incoil�������ָ��*/
#define COIL_CH(CHSF)          g_modbusInCoil.coilCh[CHSF]/**/
#define COIL_SET(CHSF, BITSF)  SET_COIL_ADDR(((CHSF)<<3) + (BITSF))/*ָ��i����Ȧ��1*/
#define COIL_RESET(CHSF, BITSF)    RESET_COIL_ADDR(((CHSF)<<3) + (BITSF))/*ָ��i����Ȧ��0*/
#define COIL_READ(CHSF, BITSF)    (READ_COIL_ADDR(((CHSF)<<3) + (BITSF))? 1 :0)/*��ȡָ��i����Ȧֵ*/

#define os1    ((MODBUS_REG_ADDR(REG_DA1)>30)? 0 :1)  /*��ȡָ��DA����Ȧֵ*/
#define os2    ((MODBUS_REG_ADDR(REG_DA2)>30)? 0 :1)  /*��ȡָ��DA����Ȧֵ*/
#define os3    ((MODBUS_REG_ADDR(REG_DA3)>30)? 0 :1)  /*��ȡָ��DA����Ȧֵ*/
#define os4    ((MODBUS_REG_ADDR(REG_DA4)>30)? 0 :1)  /*��ȡָ��DA����Ȧֵ*/
#define os5    ((MODBUS_REG_ADDR(REG_DA5)>30)? 0 :1)  /*��ȡָ��DA����Ȧֵ*/
#define os6    ((MODBUS_REG_ADDR(REG_DA6)>30)? 0 :1)  /*��ȡָ��DA����Ȧֵ*/

#define is1    ((MODBUS_INREG_ADDR(INREG_AD1)>30)? 0 :1)  /*��ȡָ��AD����Ȧֵ*/
#define is2    ((MODBUS_INREG_ADDR(INREG_AD2)>30)? 0 :1)  /*��ȡָ��AD����Ȧֵ*/
#define is3    ((MODBUS_INREG_ADDR(INREG_AD3)>30)? 0 :1)  /*��ȡָ��AD����Ȧֵ*/
#define is4    ((MODBUS_INREG_ADDR(INREG_AD4)>30)? 0 :1)  /*��ȡָ��AD����Ȧֵ*/
#define is5    ((MODBUS_INREG_ADDR(INREG_AD5)>30)? 0 :1)  /*��ȡָ��AD����Ȧֵ*/
#define is6    ((MODBUS_INREG_ADDR(INREG_AD6)>30)? 0 :1)  /*��ȡָ��AD����Ȧֵ*/

/*��Ȧ*/
#define COIL_O_CHNUM         20/*20���ֽ�*/
#define COIL_MOTO_CHNUM      4
#define COIL_SAVE_CHNUM      1
#define COIL_MSG_CHNUM       4
#define COIL_O_CHSF          0x00
#define COIL_MOTO_CHSF       (COIL_O_CHSF+COIL_O_CHNUM)
#define COIL_SAVE_CHSF       (COIL_MOTO_CHSF+COIL_MOTO_CHNUM)
#define COIL_MSG_CHSF        (COIL_SAVE_CHSF+COIL_SAVE_CHNUM)




#define COIL_RES_O           0x00
#define COIL_RES_EXO         (COIL_RES_O + CSB_IO_NUM)
#define COIL_RES_MOTOR_EN    0xa0
#define COIL_RES_RUN_DIR     0xa8
#define COIL_RES_ZERO_DIR    0xb0
#define COIL_RES_MSG         (COIL_MSG_CHSF<<3 )
#define COIL_RES_SECTION_SENSOR    0xf0

//�Ĵ�����ַ���� ��д
#define REG_CONSTANTPWR 		0x00/*�㹦�ʼĴ�����ַ*/ 
//#define REG_PWRGEARS 			0x01/*���ʵ�λ�Ĵ�����ַ*/ 
#define REG_TARGETTEMP			0x01/*Ŀ���¶ȼĴ�����ַ*/ 
#define REG_TARGETTEMP_LOWLIMIT	0x02/*Ŀ���¶�(����)�Ĵ�����ַ*/   
#define REG_DATE				 	0x03/*�������ڼĴ�����ʼ��ַ*/ //2byte
#define REG_TIME				 	0x05/*����ʱ��Ĵ�����ʼ��ַ*/ //2byte
#define REG_OSCILLATORMODEL   0x07/*�����ͺżĴ�����ַ*/	
#define REG_FREQUENCYCEN      0x08/*����Ƶ�����üĴ�����ַ*/	
#define REG_FREQUENCYRANGE    0x09/*Ƶ�ʷ�Χ���üĴ�����ַ*/	
#define REG_SCANPERIODFRE     0x0A/*ɨƵ�������üĴ�����ַ*/
#define REG_POWER      		   0x0B/*�������üĴ�����ַ*/	
#define REG_POWERAMP  		   0x0C/*����������üĴ�����ַ*/	
#define REG_SCANPERIODPWR     0x0D/*�����������üĴ�����ַ*/
#define REG_POWEREDGE     		0x0E/*�������β������üĴ�����ַ*/
#define REG_UPPERLIMIT_PREADJ 0x0F/*���ʵ����������üĴ�����ַ*/
#define REG_LOWLIMIT_PREADJ   0x10/*���ʵ����������üĴ�����ַ*/
#define REG_OVERLOADCUR       0x11/*���ص������üĴ�����ַ*/
#define REG_RATEDCUR 		   0x12/*������Ĵ�����ַ*/
#define REG_TMPCOEFF_CONST	   0x13/*�¶�ϵ���Ĵ�����ַ*/
#define REG_HEATPROTECT_DELAYTIME	0x14/*��Һλʱ���ȹ��ܹر���ʱʱ��Ĵ�����ַ*/
#define REG_FRE_ADJUST_RANGE	0x15/*Ƶ�ʵ��ڷ�Χ�Ĵ�����ַ*/
#define REG_PID_PWR_P 			0x16/*����PID���ڱ����������üĴ�����ַ*/
#define REG_PID_PWR_I			0x17/*����PID���ڻ��ֲ������üĴ�����ַ*/
#define REG_PID_PWR_D			0x18/*����PID����΢�ֲ������üĴ�����ַ*/
#define REG_PID_PWR_CTRL_T		0x19/*����PID���ڿ����������üĴ�����ַ*/
#define REG_PID_HEAT_P 			0x1A/*����PID���ڱ����������üĴ�����ַ*/
#define REG_PID_HEAT_I			0x1B/*����PID���ڻ��ֲ������üĴ�����ַ*/
#define REG_PID_HEAT_D			0x1C/*����PID����΢�ֲ������üĴ�����ַ*/
#define REG_PID_HEAT_CTRL_T	0x1D/*����PID���ڿ����������üĴ�����ַ*/

#define REG_DA1        0x0000  //DA1�Ĵ�����ַ
#define REG_DA2        0x0001  //DA2�Ĵ�����ַ
#define REG_DA3        0x0002  //DA3�Ĵ�����ַ
#define REG_DA4        0x0003  //DA4�Ĵ�����ַ
#define REG_DA5        0x0004  //DA5�Ĵ�����ַ
#define REG_DA6        0x0005  //DA6�Ĵ�����ַ


//����Ĵ���	
#define INREG_AD1        0x0000  //AD1�Ĵ�����ַ
#define INREG_AD2        0x0001  //AD2�Ĵ�����ַ
#define INREG_AD3        0x0002  //AD3�Ĵ�����ַ
#define INREG_AD4        0x0003  //AD4�Ĵ�����ַ
#define INREG_AD5        0x0004  //AD5�Ĵ�����ַ
#define INREG_AD6        0x0005  //AD6�Ĵ�����ַ



#define INREG_CURTEMP			0x31/*��ǰ�¶ȼĴ�����ַ*/ 
#define INREG_CURRENT			0x32/*��ǰ�����Ĵ�����ʼ��ַ*/ 
#define INREG_PHVALUE			0x33/*PHֵ�Ĵ�����ʼ��ַ*/ 
#define INREG_WATERLEVEL		0x34/*Һλ״̬�Ĵ�����ַ*/ 
#define INREG_RRATE 				0x35/*�����ʼĴ�����ʼ��ַ*/ 
#define INREG_CURPWR			   0x36/*��ǰ���ʼĴ�����ַ*/ 
#define INREG_FRE_IN			   0x37/*��ǰ����Ƶ�ʼĴ�����ַ*/ 
#define INREG_FRE_OUT		   0x38/*��ǰ����Ƶ�ʼĴ�����ʼ��ַ*/ 
#define INREG_POWER_FACTOR		0x3A/*�������ؼĴ�����ʼ��ַ*/ 
#define INREG_V_IN				0x3B/*��ǰ������ѹ�Ĵ�����ַ*/ 
 

#define Modbus_max_send_buf 100//130   //���֡��
#define Modbus_max_recevie_buf 100//130 
//#define MODBUSCMDBUF_NUM 5	//���������������С
/*****************************************************************************/
/******************************************************************************/
//                            MODBUS��ز�������
/********************************************************************************/
/*******************************************************************************/


/******************************************************************************/
/*
 *MODBUS���ݽṹ�Ͳ�������
 */ 
#define LITTLE_ENDIAN
/*����ͨѶ*/


#define HIGH16U(A) (((CDV_INT16U)(A) & 0xff00) >> 8)
#define LOW16U(A)  ((CDV_INT16U)(A) & 0x00ff)
#define HIGH32U(A) (((CSB_INT32U)(A) & 0xffff0000) >> 16)
#define LOW32U(A)  ((CSB_INT32U)(A) & 0x0000ffff)
//#include "f4_includes.h"

/*modbus����Ĵ����ṹ��*/
typedef union
{                                  /*!< Structure used for bit  access                  */
  CDV_WORD reg[INREG_N];                          /*!< Type      used for word access                  */
} MODBUS_Input_Register;

extern MODBUS_Input_Register g_modbusInReg;

#define MODBUS_INREG_ADDR(A) g_modbusInReg.reg[(A)]

/*modbus�Ĵ����ṹ��*/
typedef struct 
{
  CDV_INT08U NumUltra;                         /**/
  CDV_WORD reg[100];                           /*!< �Ĵ��� */
} MODBUS_Register;

extern MODBUS_Register g_modbusReg;

#define MODBUS_REG_ADDR(A) g_modbusReg.reg[(A)]


/*modbus��Ȧ�ṹ��*/
typedef struct
{
  CDV_CHAR coilCh[50];                     /*!< Type      used for word access                  */
} MODBUS_Coil;

extern MODBUS_Coil g_modbusCoil;
#define CH_CDV_O(A)          g_o[A]

#define MODBUS_COIL_CH(A) g_modbusOCoil.coilCh[(A)]
#define SET_COIL_ADDR(A) do{\
	(MODBUS_COIL_CH((A) >> 3)) |= (0x01 <<((A) & 0x07));\
}while(0);
/*modbus������Ȧ�ṹ��*/
typedef union
{
  struct
  {
		struct
		{
			CDV_CHAR I0:1;                        
			CDV_CHAR I1:1;                       
			CDV_CHAR I2:1;                       
			CDV_CHAR I3:1;                       
			CDV_CHAR I4:1;                       
			CDV_CHAR I5:1;                       
			CDV_CHAR I6:1;                       
			CDV_CHAR I7:1;                       
			CDV_CHAR I8:1;                      
			CDV_CHAR I9:1;
			CDV_CHAR I10:1;
			CDV_CHAR I11:1;
			CDV_CHAR I12:1;
			CDV_CHAR I13:1;
			CDV_CHAR I14:1;
			CDV_CHAR I15:1;
			
			CDV_CHAR I16:1;                        
			CDV_CHAR I17:1;                       
			CDV_CHAR I18:1;                       
			CDV_CHAR I19:1;                       
			CDV_CHAR I20:1;                       
			CDV_CHAR I21:1;                       
			CDV_CHAR I22:1;                       
			CDV_CHAR I23:1;                       
			CDV_CHAR I24:1;                      
			CDV_CHAR I25:1;
			CDV_CHAR I26:1;
			CDV_CHAR I27:1;
			CDV_CHAR I28:1;
			CDV_CHAR I29:1;
			CDV_CHAR I30:1;
			CDV_CHAR I31:1;
			        
		} I;                                    /*!< bit:     0 ~ 9f  speed          */
  } TYPE;                                   /*!< Structure used for bit  access                  */
  CDV_CHAR coilCh[INCOIL_CHN];                            /*!< Type      used for word access                  */
} MODBUS_Input_Coil;

extern MODBUS_Input_Coil g_modbusInCoil;

#define MODBUS_INCOIL_CH(A) g_modbusInCoil.coilCh[(A)]
#define RESET_COIL_ADDR(A) do{\
	(MODBUS_COIL_CH((A) >> 3)) &= (0xFF ^(0x01 <<((A) & 0x07)));\
}while(0);

/*modbus��Ȧ�ṹ��*/
typedef union
{
  struct
  {
		struct
		{
			CDV_CHAR O0:1;                        
			CDV_CHAR O1:1;                       
			CDV_CHAR O2:1;                       
			CDV_CHAR O3:1;                       
			CDV_CHAR O4:1;                       
			CDV_CHAR O5:1;                       
			CDV_CHAR O6:1;                       
			CDV_CHAR O7:1;                       
			CDV_CHAR O8:1;                      
			CDV_CHAR O9:1;
			CDV_CHAR O10:1;
			CDV_CHAR O11:1;
			CDV_CHAR O12:1;
			CDV_CHAR O13:1;
			CDV_CHAR O14:1;
			CDV_CHAR O15:1;
			
			CDV_CHAR O16:1;                        
			CDV_CHAR O17:1;                       
			CDV_CHAR O18:1;                       
			CDV_CHAR O19:1;                       
			CDV_CHAR O20:1;                       
			CDV_CHAR O21:1;                       
			CDV_CHAR O22:1;                       
			CDV_CHAR O23:1;                       
			CDV_CHAR O24:1;                      
			CDV_CHAR O25:1;
			CDV_CHAR O26:1;
			CDV_CHAR O27:1;
			CDV_CHAR O28:1;
			CDV_CHAR O29:1;
			CDV_CHAR O30:1;
			CDV_CHAR O31:1;
			
		} O;                                     /*!< bit:     0~9f  speed          */
		
  } TYPE;                                  /*!< Structure used for bit  access                  */
  CDV_CHAR coilCh[COIL_CHN];                     /*!< Type      used for word access                  */
} MODBUS_OCoil;

extern MODBUS_OCoil g_modbusOCoil;

//#define MODBUS_COIL_CH(A) g_modbusCoil.coilCh[(A)]
#define COIL_REFRESH(A,B) if(B)SET_COIL_ADDR(A)else RESET_COIL_ADDR(A);//��Ȧ���� BΪ0����Ȧ���� BΪ���㣺��Ȧ��λ

#define READ_O(A) (((CH_CDV_O((A) >> 3)) & (0x01 <<((A) & 0x07)))? 1 :0)

#define READ_COIL_ADDR(A) (MODBUS_COIL_CH((A) >> 3)) & (0x01 <<((A) & 0x07))

extern CDV_INT08U Debug_mode;
extern CDV_INT08U Modbus_slave_addr;    //Ŀ��ӻ���ַ
extern CDV_INT08U Function_Code;		//����ģʽ������
extern CDV_INT16U Modbus_slave_reg_addr;	//�ӻ��Ĵ�����ַ
extern CDV_INT16U Modbus_slave_reg_num;	//�ӻ��Ĵ�����
extern CDV_INT16U Modbus_reg_val1;//д��ӻ��Ĵ�����ֵ����Modbus_slave_reg_num=1����д�����16λ���ݣ�Modbus_reg_val2��Ч
extern CDV_INT16U Modbus_reg_val2;//��Modbus_slave_reg_num=2����д�����32λ���ݣ�Modbus_reg_val1Ϊ��16��Modbus_reg_val2λ��16
extern CDV_REG08 Modbus_recevie_flag;
extern CDV_REG32 Modbus_timeout_cnt;
//extern CDV_INT08U Modbus_RX_buf[Modbus_max_recevie_buf];
extern CDV_INT08U Modbus_RX_buf[Modbus_max_recevie_buf];
extern CDV_REG08 ModbusCmdBuf_Cnt,ModbusCmdBuf_CntMax,ModbusCmdExecution_State;
extern CDV_INT08U Modbus_TX_buf[Modbus_max_send_buf];
//extern CDV_INT08U Modbus_RX_len;
extern CDV_INT08U Modbus_RX_len;
extern CDV_REG08 Modbus_cmd_flag,Modbus_exe_flag,Modbus_Process_Flag;
extern CDV_INT08U Modbus_SendWait_flag;
extern CDV_REG16 Modbus_SendWait_Cnt;
extern CDV_INT08U USART_TX_BUF_ADDR[Modbus_max_send_buf];
extern CDV_INT08U USART_TX_BUF[Modbus_max_send_buf];
extern CDV_REG08 Tx_flag;
extern CDV_INT08U Modbus_ReadRegister_num;//?????????
extern CDV_INT16U Modbus_ReadRegister_value[100];//?????????
extern CDV_INT08U Modbus_ReadRegister_ture,AlarmB_Flag_last;
extern CDV_INT08U Modbus_local_UI;
extern float TEMP_last;
extern CDV_INT08U Command_Number;//MODBUS???????
extern CDV_INT08U Interface_Number,Interface_Number0,Interface_Number_last;//MODBUS??????????
extern CDV_REG08 All_Parameters_flag;

//#define Modbus_addr  0x01  //������ַ
extern CDV_INT08U Modbus_addr;//������ַ
extern CDV_INT08U Modbus_mode;//0:�ӻ�ģʽ 1:����ģʽ
extern CDV_INT16U Ctrl_commond_addr;
extern CDV_INT16U Ctrl_num;
extern CDV_INT08U slave_number[10],HeartBeat_slave_request_flag,slave_number_dis[10];
extern CDV_REG08 HeartBeat_flag,Interface_refresh_flag;//���������ͱ�־
extern CDV_REG16 HeartBeat_timeout;//���������ͼ������
extern CDV_REG08	slave_display;//�ӻ�����ʾ��־
extern CDV_REG16	slave_count;//�ӻ�����ʾ��ʱ
extern CDV_REG08	slave_display2;	

//CDV_INT16U getCRC16(CDV_INT08U *ptr,CDV_INT16U len) ;
//CDV_INT08U ModbusParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen);
void RequestAdd(CDV_INT08U* txBuf,CDV_INT08U txLen);
void ReadCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void ReadInCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void ReadRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void ReadInRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void WriteCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void WriteRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void WriteMultiCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void WriteMultiRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen);
void AddTx(CDV_INT08U* txBuf, CDV_INT08U txLen);
void RequestAdd(CDV_INT08U* rxBuf,CDV_INT08U txLen);
void modbus(void);
void Modbus_cmd(void);
void Modbus_exe(void);
CDV_INT16U getCRC16(CDV_INT08U *ptr,CDV_INT16U len);
void Modbus_EleParm_Measure(CDV_INT16U addr);
void Host_Mode(void);
void Hostmode_datahandle(CDV_INT08U Modbus_slave_addr0);
void Modbus_EleParm_Set(CDV_INT16U addr);
void WriteMultiRegister_hostmode(CDV_INT08U Modbus_slave_addr0,CDV_INT16U addr,CDV_INT16U num,CDV_INT16U reg_val1,CDV_INT16U reg_val2);
void WriteMultiRegister_handle(CDV_INT08U* rxBuf,CDV_INT08U rxLen,CDV_INT16U addr,CDV_INT16U num);
void WriteMultiRegister_hostmode2(CDV_INT08U Modbus_slave_addr0,CDV_INT16U addr,CDV_INT16U num);
void Modbus_EleParm_Set2(CDV_INT16U addr);
void HeartBeat(void);
void HeartBeat_slave(void);
void HeartBeat_host_send(void);
void HeartBeat_host_handle(void);
void ReadRegister_hostmode(CDV_INT08U Modbus_slave_addr0,CDV_INT16U addr,CDV_INT16U num);
void WriteCoilCmdParse(CDV_INT16U addr);
void Modbus_Cmd_Receive(void);
void WriteMultiRegisterCmdParse(CDV_INT16U addr);
void ReadRegisterCmdParse(CDV_INT16U addr,CDV_INT16U num);
void ReadInRegisterCmdParse(CDV_INT16U addr,CDV_INT16U num);
void Register_Init(void);
void Debug_DataMeasure(void);
#endif
