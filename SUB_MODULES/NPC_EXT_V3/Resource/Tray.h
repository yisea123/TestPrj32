
#ifndef  _CDV_TRAY_
#define  _CDV_TRAY_

#include "cdv_include.h" 
#define TrayNameMax 256
#define TRAYNUM 1000
#define ERROR 0
#define ONETRAY TRAYNUM*26
RET_STATUS TrayScript(CDV_INT08U* Rbuf,CDV_INT32U Addr, CDV_INT08U debug,CDV_INT08U uartNo);
void GetScript(CDV_INT32U no);
CDV_INT32S AxisHandle(CDV_INT08U TrayNo,CDV_INT08U Axis);
CDV_INT32S ValueSum(CDV_INT16U ValueNum);
void Tray_Wirte(CDV_INT08U *buf,CDV_INT32U Addr);
void Tray_Read(CDV_INT08U *buf,CDV_INT08U uartNo);
void HeadInfo_Wirte(CDV_INT08U *buf);
void HeadInfo_Read(CDV_INT08U *buf,CDV_INT08U uartNo);

RET_STATUS Tray_Init(void);
void TrayHandle(CDV_INT08U TrayNo, CDV_INT32U condition,CDV_INT32U Addr, CDV_INT08U debug);
void ValueHandle(CDV_INT08U TrayNo,CDV_INT16U ValueNo,CDV_INT32U Addr, CDV_INT08U debug);
//CDV_INT32S CalculateForAll(CDV_INT08U* buf,CDV_INT32S addr,CDV_INT32S lenth);
void TrayGoToPoint(CDV_INT08U TrayNo,CDV_INT32U GongGeNo,CDV_INT32U Addr, CDV_INT08U debug);
void TrayGoToPointV(CDV_INT08U TrayNo,CDV_INT16U ValueNo,CDV_INT32U Addr, CDV_INT08U debug);
void TrayGoCurrPoint(CDV_INT08U TrayNo,CDV_INT08U debug);
void TrayGoNextPoint(CDV_INT08U TrayNo,CDV_INT32U Addr, CDV_INT08U debug);
void TrayGoLastPoint(CDV_INT08U TrayNo,CDV_INT32U Addr, CDV_INT08U debug);
void TrayGoStartPoint(CDV_INT08U TrayNo,CDV_INT32U Addr,CDV_INT08U debug);
void MotorToRun(CDV_INT08U TrayNo,CDV_INT08U AxisNum,CDV_INT08U debug);
RET_STATUS TrayWait(CDV_INT08U TrayNo,CDV_INT08U debug ) ;

typedef struct TrayData1
{
	CDV_INT08U TrayNo[1];
	CDV_INT08U LatticeNo[2];
	CDV_INT08U ValueNum[2];
	CDV_INT08U Axis[1];
	CDV_INT08U ConditionMin[4];
	CDV_INT08U ConditionMax[4];
	CDV_INT08U X[4];
	CDV_INT08U Y[4];
	CDV_INT08U Z[4];

}TrayData;


#endif

