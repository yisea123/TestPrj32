#ifndef _CDV_LOGIC_
#define _CDV_LOGIC_
#include "cdv_include.h" 
#define LogAddOff 16384; //һ����Դ�Ĵ�С

typedef struct LogicInfo{

	CDV_INT08U HeadLen[1];     //��Դͷ��Ϣ�ĳ���
	CDV_INT32U BranshByte[10]; //һ����ִ֧�����ݵĴ�С
	CDV_INT08U BranshNum[1];   //һ����Դ�ķ�֧��
	CDV_INT08U CmdNum[1];      //һ����֧��ִ������
	CDV_INT08U FlagBit[1];     //�������λ
	
}LOGIC;

RET_STATUS LogicScript(CDV_INT08U* Rbuf,CDV_INT32U Addr, CDV_INT08U debug);
void Log_JudgeByNo(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug);
void Log_JudgeByIf(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug);
void Log_LoopByIf(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug);
void Log_LoopByNo(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug);

LOGIC ReadLogHead(CDV_INT08U ResNo,CDV_INT32U Addr);
CDV_INT32U ReadLogBranshNo(CDV_INT32U FlashAddr,CDV_INT08U BranshNo,CDV_INT32U Count,CDV_INT08U BranshNum,CDV_INT32U* 	BranshByte);
CDV_INT32U ReadLogBranshCdt(CDV_INT32U FlashAddr,CDV_INT32U BranshCdt,CDV_INT32U Count,CDV_INT08U BranshNum,CDV_INT32U* 	BranshByte);
void Script_ToRun(CDV_INT32U Addr,CDV_INT08U CmdNum,CDV_INT08U debug);

//typedef struct LogicInfo{

//	CDV_INT08U HeadLen[1];     //��Դͷ��Ϣ�ĳ���
//	CDV_INT32U BranshByte[10]; //һ����ִ֧�����ݵĴ�С
//	CDV_INT08U BranshNum[1];   //һ����Դ�ķ�֧��
//	CDV_INT08U CmdNum[1];      //һ����֧��ִ������
//	CDV_INT08U FlagBit[1];     //�������λ
//	
//}LOGIC;

#endif

