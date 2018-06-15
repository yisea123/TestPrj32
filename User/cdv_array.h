
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

//LIST
struct LIST_T {
	struct LIST_T* head;  // head是个空元素，便于快速定位head
	struct LIST_T* tail;  // tail是个空元素，便于快速定位tail
	struct LIST_T* prev;  // 上一个元素
	struct LIST_T* next;  // 下一个元素
	void* data;           // 元素数据
	unsigned int size;    // 元素bytes // 对于head 是数量
	unsigned int tag;     // 用户自定义标签，可用与识别data类型，0和0xFFFFFFFF无法使用，为head和tail标签
};

typedef struct LIST_T CDV_LIST;

#include "cdv_include.h"

#define  MAX_ARRAY_LEN  1000

//ARRAY_U32 NewArrayU32(void);
void ClearArrayU32(ARRAY_U32 *array);
void ClearArrayU8(ARRAY_U8 *array) ;
void ArrayU32Add(ARRAY_U32 *array, CDV_INT32U data);



//LIST
#define TAG_HEAD 0x0
#define TAG_TAIL 0xFFFFFFFF
CDV_LIST* LIST_Cteate(void) ;
CDV_LIST* LIST_AddTail(CDV_LIST *elm, void *data ,/* size_t size,*/ uint32_t tag);
CDV_LIST* LIST_AddHead(CDV_LIST *elm, void *data , /*size_t size,*/ uint32_t tag) ;
CDV_LIST* LIST_Insert(CDV_LIST *elm, void *data , /*size_t size,*/ uint32_t tag);
CDV_LIST* LIST_Remove(CDV_LIST *elm);
void LIST_RemoveAll(CDV_LIST *elm);

#endif

