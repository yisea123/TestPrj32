
/**
  ******************************************************************************
  * @file    /user_define.h 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   define struct union variable and so on
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CDV_ARRAY_
#define  _CDV_ARRAY_
typedef struct {
	unsigned int len;//长度
	unsigned char * p;//保存数据的指针
} ARRAY_U8;

typedef struct {
	unsigned int len;//长度
	unsigned int * p;//保存数据的指针
} ARRAY_U32;

#include "cdv_include.h"

#define  MAX_ARRAY_LEN  1000

//ARRAY_U32 NewArrayU32(void);
void ClearArrayU32(ARRAY_U32 *array);
void ClearArrayU8(ARRAY_U8 *array) ;
void ArrayU32Add(ARRAY_U32 *array, CDV_INT32U data);

#endif

