//#include "stm32f4xx.h"

#ifndef  _CDV_Weight_Sensor_
#define  _CDV_Weight_Sensor_


#include "cdv_include.h"
extern u8 DG_Receive_Data[16];
extern u32 DG_Receive_Len;


void Set_Weight_Sensor_resolution(u8 ID,u8 dataH,u8 dataL);
void Set_Weight_Sensor_Baudrate(u8 ID,u8 dataH,u8 dataL);
void Set_Weight_Sensor_Smoothinglens(u8 ID,u8 dataH,u8 dataL);
void Set_Weight_Sensor_Speed(u8 ID,u8 dataH,u8 dataL);
void Set_Weight_Sensor_Standard_Zreo(u8 ID);
void Set_Weight_Sensor_Standard(u8 ID,u8 dataH,u8 dataL);
void Read_Weight_Sensor_Para(u8 ID,u8 Para_No);
u8 Read_Weight_Sensor_Inf(u8 DG_id,u16 addr);
u8 Set_Weight_Sensor_Par(u8 DG_id,u8 DG_pflag,u8 para_pos,u8* rxBuf );



#endif





