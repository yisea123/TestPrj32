
/**
  ******************************************************************************
  * @file    /_include.h 
  * @author  MMY
  * @version V1.0.0
  * @date    31-July-2019
  * @brief   project 's necessary h files 
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2019 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  __INCLUDE_
#define  __INCLUDE_

#if defined (STM32F10X_HD)
#include "stm32f10x.h"   //m3
#elif defined (STM32F40_41xxx)
#include "stm32f4xx.h"   //m4
#endif
//OTHER
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//USER
#include "_common.h" 
#include "_typedef.h"
//APP
#include "cmdParse.h"
#include "modbus.h"
//#include "UniversalSCom.h"
//#include "Cascade.h"
//CONFIG
#include "iox.h"
#include "74HC595.h"
#include "uartx.h"
#include "tim8.h"
#include "tim4.h"
#endif

