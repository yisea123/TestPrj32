#ifndef  _CDV_FREQCY_
#define  _CDV_FREQCY_
#include "cdv_include.h" 
#define VALUENUM 0x10
#define FORWARD  1
#define BACK     0
void FrequencySet(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug);
void FrequencyRead(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug,CDV_INT08U uartNo);
void FreqcyForward(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug);
void FreqcyReverse(CDV_INT08U *buf,CDV_INT32U FlashAddr,CDV_INT08U debug);
void FreqcyStop(CDV_INT08U *buf,CDV_INT08U debug);
RET_STATUS FreqcyWait(CDV_INT08U *buf,CDV_INT08U debug);
CDV_INT32S FreValueSum(CDV_INT16U ValueNum);
CDV_INT32S FlashSum(CDV_INT16U ValueNum);
CDV_INT32S CalculateForAll(CDV_INT08U* buf,CDV_INT32S addr,CDV_INT32S lenth);
void SendToSet(CDV_INT08U,CDV_INT08U ValueNo,CDV_INT16U SetValue);
void SaveCommand(CDV_INT32U valuenum,CDV_INT08U ValueLen,CDV_INT08U* buf,CDV_INT32U FlashAddr);
CDV_INT32S ReadCommand(CDV_INT08U DeviceNo,CDV_INT08U Grade,CDV_INT16U ValueNum,CDV_INT08U ValueLen,CDV_INT32U FlashAddr,CDV_INT08U Flag);
void setting(CDV_INT08U DeviceNo,CDV_INT08U ActionNo,CDV_INT16U Addr,CDV_INT16U Data);
//CDV_INT32U CheckGrade(CDV_INT08U *buf,CDV_INT08U ValueLen);
//RET_STATUS FreqcyWait(CDV_INT08U debug);













//typedef            char        CDV_CHAR;                        /*  8-bit character                                     */
//typedef  unsigned  short       CDV_WORD;                        /* 16-bit word                              */
//typedef  unsigned  char        CDV_BOOLEAN;                     /*  8-bit boolean or logical                            */
//typedef  unsigned  char        CDV_INT08U;                      /*  8-bit unsigned integer                              */
//typedef    signed  char        CDV_INT08S;                      /*  8-bit   signed integer                              */
//typedef  unsigned  short       CDV_INT16U;                      /* 16-bit unsigned integer                              */
//typedef    signed  short       CDV_INT16S;                      /* 16-bit   signed integer                              */
//typedef  unsigned  int         CDV_INT32U;                      /* 32-bit unsigned integer                              */
//typedef    signed  int         CDV_INT32S;                      /* 32-bit   signed integer                              */
//typedef  unsigned  long  long  CDV_INT64U;                      /* 64-bit unsigned integer                              */
//typedef    signed  long  long  CDV_INT64S;                      /* 64-bit   signed integer                              */

//typedef            float       CDV_FP32;                        /* 32-bit floating point                                */
//typedef            double      CDV_FP64;                        /* 64-bit floating point                                */



#endif
