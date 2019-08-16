
/**
  ******************************************************************************
  * @file    /pvd.h 
  * @author  MMY
  * @version V0.0.1
  * @date    2018-2-1
  * @brief   a package of pvd
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */



#ifndef  _PVD_

#define  _PVD_

#include "cdv_include.h"

#if USE_PVD == 1u
CDV_INT08S PVD_Restore(void);
void PVD_Config(void);
void PVD_Erase(void);
void PVD_FlagClear(void);
u32 PVD_GetFlag(void);
void PVD_Save(void);
u8 PVD_Flash_Read(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite) ;
u8 PVD_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite) ;
#endif

#endif
