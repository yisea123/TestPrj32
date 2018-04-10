#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sysdef.h>	  
//////////////////////////////////////////////////////////////////////////////////  
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//使用SysTick的普通计数模式对延迟进行管理(支持ucosii和ucosiii)
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/5/2
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//V1.1修改说明
//增加了对UCOSIII的支持
////////////////////////////////////////////////////////////////////////////////// 	 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void TaskSched(void);
void TaskSched1(void);
#define CDV_TIME_NUM 20

extern u32 cdvTimeTick;
#define GET_TICK cdvTimeTick
#ifdef CDV_TIME_NUM
//u32 GetCdvTimeTick(void);
//void SetCdvTimeTick(u32 timeTick);
u32 CalcTimeMS(u32 endTick , u32 startTick);
void StartCdvTime(u8 no);
u32 GetCdvTimeTick(void);
u32 EndCdvTime(u8 no);
void DelayMS(u32 num);
//extern u32 Sys_time_1ms;	//系统计时1ms自增
//extern u32 Sys_time_50ms;	//系统计时1s自增
extern u8 ValveAdjust_Flag;
#endif
#endif





























