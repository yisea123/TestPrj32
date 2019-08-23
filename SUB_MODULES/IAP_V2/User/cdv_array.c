/**
  ******************************************************************************
  * @file    /cdv_array.c 
  * @author  MMY
  * @version V2.0.0
  * @date    18-Mar-2016
  * @brief   about a point that can become longer self 
	*
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
#include "cdv_array.h"

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////ArrayU32 Function///////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
/*调用者负责释放*/
ARRAY_U32* CteateArrayU32(void) {
	
	ARRAY_U32* array = NULL;
	NEWCH(array,sizeof(ARRAY_U32));
	return array;
	
}


void	DeleteArrayU32(ARRAY_U32 **array) {
	
	ClearArrayU32(*array);
	DELETE(*array);
}

void WriteArrayU32(ARRAY_U32 *array, unsigned int *p, unsigned int len) {
	NEW32U(array->p , len);
	memcpy(array->p, p, len);
	array->len = len;
}

void ClearArrayU32(ARRAY_U32 *array) {
	DELETE(array->p);
	array->len = 0;
}

void ArrayU32Add(ARRAY_U32 *array, CDV_INT32U data) {
	CDV_INT32U *tmp = NULL;
	if(array->len >= MAX_ARRAY_LEN)
		return;
	NEW32U(tmp , array->len);
	memcpy(tmp , array->p , array->len * sizeof(CDV_INT32U));
	NEW32U(array->p , array->len + 1);
	memcpy(array->p , tmp , array->len * sizeof(CDV_INT32U));
	*(array->p + array->len ) = data;
	array->len++;
	DELETE(tmp);
}


	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////ArrayU8 Function///////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
/*调用者负责释放*/
ARRAY_U8* CteateArrayU8(void) {
	
	ARRAY_U8* array = NULL;
	NEWCH(array,sizeof(ARRAY_U8));
	return array;
	
}


void	DeleteArrayU8(ARRAY_U8 **array) {
	
	ClearArrayU8(*array);
	DELETE(*array);
}

void WriteArrayU8(ARRAY_U8 *array, unsigned int *p, unsigned int len) {
	NEW32U(array->p , len);
	memcpy(array->p, p, len);
	array->len = len;
}

void ClearArrayU8(ARRAY_U8 *array) {
	DELETE(array->p);
	array->len = 0;
}

void ArrayU8Add(ARRAY_U8 *array, CDV_INT32U data) {
	CDV_INT32U *tmp = NULL;
	if(array->len >= MAX_ARRAY_LEN)
		return;
	NEW32U(tmp , array->len);
	memcpy(tmp , array->p , array->len * sizeof(CDV_INT32U));
	NEW32U(array->p , array->len + 1);
	memcpy(array->p , tmp , array->len * sizeof(CDV_INT32U));
	*(array->p + array->len ) = data;
	array->len++;
	DELETE(tmp);
}
