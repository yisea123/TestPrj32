#ifndef __PID_H
#define __PID_H	
#include "cdv_include.h"  
//#include "sys.h" 
//#define BUFFX_LEN 200
//#define BUFFV_LEN 200
#define PID_no 4


u8 Read_Pid_Par(u8 *buf,u8 no,u32 pflag); //PID²ÎÊý¶ÁÈ¡
RET_STATUS PidCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);


#endif 















