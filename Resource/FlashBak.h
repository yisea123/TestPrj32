
/**
  ******************************************************************************
  * @file    /FlashBak.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2018-4-10
  * @brief   a package to solve the problem of lost data in flash module
	* @design  
	           1.划分两块完整的flash扇区（4096，一块是原先的区域，一块是新增的备份区域
						 2.每次写入都要对备份区域执行同样的操作，并在两块扇区最后一个字节做上标记，此举用以防止单一扇区操作会在掉电时数据丢光
						 3.恢复时查看标记，如果标记不正确，从备份出取出，如果备份也不正确，(￣▽￣)"GG
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
//恢复
void FlashBak_VarRestore(void);

//备份
CDV_INT32S FlashBak_VarBackUp(CDV_INT32U no, CDV_INT32S num);
CDV_INT32S FlashBak_VarBackUpEx(CDV_INT08U* buf, CDV_INT32U addr, CDV_INT32S num);
#endif

#endif
