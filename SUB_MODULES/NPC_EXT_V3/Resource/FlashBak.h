
/**
  ******************************************************************************
  * @file    /FlashBak.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2018-4-10
  * @brief   a package to solve the problem of lost data in flash module
	* @design  
	           1.��������������flash������4096��һ����ԭ�ȵ�����һ���������ı�������
						 2.ÿ��д�붼Ҫ�Ա�������ִ��ͬ���Ĳ��������������������һ���ֽ����ϱ�ǣ��˾����Է�ֹ��һ�����������ڵ���ʱ���ݶ���
						 3.�ָ�ʱ�鿴��ǣ������ǲ���ȷ���ӱ��ݳ�ȡ�����������Ҳ����ȷ��(������)"GG
						 4.
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _FLASH_BAK_
#define  _FLASH_BAK_


#include "cdv_include.h"    

#if USE_FLASH_BAK
//�ָ�
void FlashBak_Restore(void);

//����
CDV_INT32S FlashBak_BackUp(void);
#endif

#endif
