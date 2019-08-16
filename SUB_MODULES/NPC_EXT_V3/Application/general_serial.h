#ifndef  _CDV_GENERAL_SERIAL_
#define  _CDV_GENERAL_SERIAL_

#include "cdv_include.h"

extern CDV_INT08U general_serial_buf[64];
extern u32 general_serial_count;
extern u32 general_serial_count2;
RET_STATUS General_Serial(CDV_INT08U* rxBuf, CDV_INT08U rxLen);
u8 External2Internal(void);
void general_serial_qequest(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CDV_INT08U* para, CDV_INT08U paraLen);
void general_serial_return(CDV_INT08U* rxBuf, CDV_INT08U rxLen);


#endif

