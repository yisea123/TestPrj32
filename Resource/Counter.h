
/**
  ******************************************************************************
  * @file    /Counter.h 
  * @author  MMY
  * @version V0.0.1
  * @date    10-16-2018
  * @brief   a package of counter
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _COUNTER_
#define  _COUNTER_

#include "cdv_include.h"    

typedef struct
{	
	GPIO_TypeDef * port;//输出端口
	CDV_INT16U pin;//输出端口号
	CDV_INT32S* cnt_var;//绑定变量
	CDV_INT32S* dir_var;//绑定变量
	
}COUNTER;

#define COUNTER_NUM 5

void Counter_init(void);


#endif

