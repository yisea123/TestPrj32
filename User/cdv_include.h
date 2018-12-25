
/**
  ******************************************************************************
  * @file    /cdv_include.h 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   project 's h files
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CDV_INCLUDE_
#define  _CDV_INCLUDE_


#include "includes.h"    //ucosiii
#include "stm32f4xx.h"   //m4
/*user*/
#include "cdv_define.h" 
#include "cdv_config.h"
#include "basic_resource.h"
#include "log.h"
#include "cdv_array.h"
#include "modbus.h"
#include "offline.h"
#include "FPGA.h"
#include "FPGA_spi.h"
//#include "cdv_debug.h"
//#include "esp8266.h"
#include "mainUart.h"
#include "Cascade.h"
//#include "rs485_uart3.h"	
//#include "rs485.h"	
//#include "formula.h"
#include "newScript.h"
#include "DAC7512.h"
#include "cmdParse.h"
#include "tcp_server.h"
#include "udpmain.h"
#include "CentralizedControl.h"

#if USE_WORKER_DEBUG == 1u
  #include "workerDebug.h"
#endif
#include "worker.h"

/*resource*/
#include "IO.h"
#include "Var.h"
#include "Msg.h"
#include "Time.h"
#include "Logic.h"
#include "Moto.h"
#include "PID.h"
//#include "Tray.h"
//#include "DG.h"
//#include "FreChange.h"
//#include "Weight_Sensor.h"
//#include "Prompt.h"
//#include "general_serial.h"
#include "MotoParmTrans.h"
#include "adc.h"
#include "dac.h"
#include "ProportionalValve.h"
#include "SysMng.h"
#include "PH.h"
#include "CSBCtrl.h"
#include "UniversalSCom.h"
#include "VarExp.h"
#include "memmng.h"
#include "FlashBak.h"
#include "Counter.h"
/*Config*/
#include "dmax.h"
#include "uartx.h"
#include "spix.h"
#include "spi2.h"
#include "spi3.h"
#include "exti.h"
#include "uart3.h"
//#include "sram.h"
#include "pvd.h"
#include "lowPower.h"
#include "extiPowerOff.h"
/*thirdpart*/
#include "delay.h"
#include "sysdef.h"
#include "usart.h"
#include "flash.h"
#include "rtc.h"
#include "malloc.h"
#include "Alarm.h"
#include "led.h" 
#include "bit_operation.h"
/*lwip*/
#include "lwip_comm.h" 
#include "httpd.h"
#endif

