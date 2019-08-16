
/**
  ******************************************************************************
  * @file    /bit_operation.h 
  * @author  MMY
  * @version V1.0.0
  * @date    27-Feb-2018
  * @brief   
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */



#ifndef  _BIT_OPERATION_
#define  _BIT_OPERATION_


/** @brief  操作数组中bit的宏
  * @param  P	要操作的数组起始地址
  *       	N	哪个位要操作
  * @note   使用时，注意检测越界
  */
#define SET_PTR_BIT(P, N)   *((u8*)P + ((N) >> 3)) |= (0x01 << ((N) & 0x07))
#define RESET_PTR_BIT(P, N) *((u8*)P + ((N) >> 3)) &= (0xFF ^ (0x01 <<((N) & 0x07)))
#define READ_PTR_BIT(P, N)  ((*((u8*)P + ((N) >> 3)) & (0x01 << ((N) & 0x07))) ? 1 : 0)


/** @brief  保留变量中连续bit的最后1个bit
  * @param  V	要操作的变量
  * @note   使用注意类型转换
	*	@example(U8)GET_LSB((U8)A)
  */
#define GET_LSB(V) ((((V) << 1)|(V)) ^ ((V) << 1))





#endif
