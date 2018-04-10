
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


/** @brief  ����������bit�ĺ�
  * @param  P	Ҫ������������ʼ��ַ
  *       	N	�ĸ�λҪ����
  * @note   ʹ��ʱ��ע����Խ��
  */
#define SET_PTR_BIT(P, N)   *((u8*)P + ((N) >> 3)) |= (0x01 << ((N) & 0x07))
#define RESET_PTR_BIT(P, N) *((u8*)P + ((N) >> 3)) &= (0xFF ^ (0x01 <<((N) & 0x07)))
#define READ_PTR_BIT(P, N)  ((*((u8*)P + ((N) >> 3)) & (0x01 << ((N) & 0x07))) ? 1 : 0)


/** @brief  ��������������bit�����1��bit
  * @param  V	Ҫ�����ı���
  * @note   ʹ��ע������ת��
	*	@example(U8)GET_LSB((U8)A)
  */
#define GET_LSB(V) ((((V) << 1)|(V)) ^ ((V) << 1))





#endif
