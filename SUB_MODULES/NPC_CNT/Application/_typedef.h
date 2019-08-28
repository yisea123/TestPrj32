
/**
  ******************************************************************************
  * @file    /user_define.h 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   define struct union variable and so on
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _TYPEDEF_
#define  _TYPEDEF_

typedef enum
{ 
  FALSE  = 0,
  TRUE,
}BOOL;


typedef enum {
	BUF_NONE = 0,
	BUF_NEW,
	
}BUF_OPT;

typedef enum  {
	RC_NONE= 0,
	RC_CRC,
	
}REQUEST_CTRL;

typedef enum
{ 
  CDV_ONLINE  = 0, //����ģʽ����������ģʽ
  CDV_SCRIPT_SEND  = 1,//���ű�״̬
	//CDV_SCRIPT_RECV,//�սű�״̬
	CDV_FPGA,//FPGA����ģʽ
	/*�ѻ�ģʽ*/
	CDV_START, //�ѻ�ģʽ������״̬
	CDV_STPAUSE,//�ѻ�ģʽ��������ͣ
	CDV_RUN,//�ѻ�ģʽ������״̬
	CDV_PAUSE, //�ѻ�ģʽ����ͣ
	CDV_STOP, //�ѻ�ģʽ��ֹͣ
	CDV_WAIT,//�ѻ�ģʽ���������õȴ�	
	/*����ģʽ*/
//	CDV_DEBUG,//����ģʽ
	CDV_GET_ALL_WORKER,//��ȡ���й����б�
	CDV_GET_WORKER_SCRIPT,//��ȡ���˽ű��б�
	CDV_RUN_WORKER_SCRIPT,//ִ�й��˽ű�
	CDV_RECV,//���սű�
	CDV_WIFI,//wifi����ģʽ
	CDV_FPGA_INIT,//fpga��ʼ��
	CDV_HARD_FAULT,//Ӳ������
	CDV_NEW_ERROR,//�ڴ��������
	CDV_DEBUG_JUMP,//�ѻ����ԡ���ת
	CDV_WHILE_ERROR,//��ѭ��
	CDV_LOG_SEND,//log������
	CDV_GET_VERSION,//�ѻ�����ȡ�汾��
	CDV_FORMULA,//�ѻ�����ȡ�䷽����
	CDV_ASSERT,
	CDV_NULL,//�������
}CDV_STATUS;

typedef enum
{ 
	OPT_SUCCESS= 0,
	OPT_FAILURE,
  
}RET_STATUS;

typedef struct 
{
	unsigned char hostid;//������
	unsigned char uart;//ռ�ô���
	void *arg;     // ����������Եĸ�������
	unsigned short len;//�����
	unsigned char* buf;//����
	void* ptrWorker; //ָ���˽ṹ�壬�����߼��ȹ��˿���
	/*��Դ�ظ����*/
	unsigned short reqlen;
	unsigned char* reqbuf;
	/*���������¼���������ñ���*/
	unsigned short speclen;
	unsigned char* specbuf;
	//ArithmeticStack stack;
}CMD_ARG;


/*
 * ���������
 */
 #define HAVE_ONLINE_CMD g_olCache.mark
typedef struct{

	unsigned char* buf;         /*���Ͷ���*/
	unsigned short len;          /*���鴦��λ��*/	
	unsigned char uart;         /*��Ǵ��ĺŴ��ڷ�����*/
	unsigned char mark;         /*�ظ����*/
	void *arg;               /*������*/
} OLCMD_CACHE;
extern OLCMD_CACHE g_olCache;
extern OLCMD_CACHE g_portCmdCache;

// define �Ķ�������Ҫ��ǰ��������ʹ��ǰ��c����������

//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��

#if defined (STM32F10X_HD)
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 #elif defined (STM32F40_41xxx)
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
#endif
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����
// bit operation

#define  BIT_SET(val , bit)      ((val) |= (0x01 << (bit)))
#define  BIT_RESET(val , bit)    ((val) &= ((~0x0) ^(0x01 << (bit))))
#define  BIT_READ(val , bit)     (((val) & (0x01 <<(bit)))? 1 :0)

#define SET_COIL(P, N) do{\
	*((u8*)P + ((N) >> 3)) |= (0x01 <<((N) & 0x07));\
}while(0);
#define RESET_COIL(P, N) do{\
	*((u8*)P + ((N) >> 3)) &= (0xFF ^(0x01 <<((N) & 0x07)));\
}while(0);
#define READ_COIL(P, N) ((*((u8*)P + ((N) >> 3)) & (0x01 <<((N) & 0x07)))? 1 :0)

//////

#define PORT_A ((GPIO_TypeDef *) GPIOA_BASE)
#define PORT_B ((GPIO_TypeDef *) GPIOB_BASE)
#define PORT_C ((GPIO_TypeDef *) GPIOC_BASE)
#define PORT_D ((GPIO_TypeDef *) GPIOD_BASE)
#define PORT_E ((GPIO_TypeDef *) GPIOE_BASE)
#define PORT_F ((GPIO_TypeDef *) GPIOF_BASE)
#define PORT_G ((GPIO_TypeDef *) GPIOG_BASE)
#define PORT_H ((GPIO_TypeDef *) GPIOH_BASE)
#define PORT_I ((GPIO_TypeDef *) GPIOI_BASE)

#define PIN_0  ((uint16_t)0x0001)
#define PIN_1  ((uint16_t)0x0002)
#define PIN_2  ((uint16_t)0x0004)
#define PIN_3  ((uint16_t)0x0008)
#define PIN_4  ((uint16_t)0x0010)
#define PIN_5  ((uint16_t)0x0020)
#define PIN_6  ((uint16_t)0x0040)
#define PIN_7  ((uint16_t)0x0080)
#define PIN_8  ((uint16_t)0x0100)
#define PIN_9  ((uint16_t)0x0200)
#define PIN_10 ((uint16_t)0x0400)
#define PIN_11 ((uint16_t)0x0800)
#define PIN_12 ((uint16_t)0x1000)
#define PIN_13 ((uint16_t)0x2000)
#define PIN_14 ((uint16_t)0x4000)
#define PIN_15 ((uint16_t)0x8000)


/////////////////////////////////////////////////

#define HIGH16U(A) (((u16)(A) & 0xff00) >> 8)
#define LOW16U(A)  ((u16)(A) & 0x00ff)
#define ENDIAN_TF16U(A)    ((((u16)(A) & 0xff00) >> 8) | \
                              (((u16)(A) & 0x00ff) << 8))
#define ENDIAN_TF(A,N)   

#endif

