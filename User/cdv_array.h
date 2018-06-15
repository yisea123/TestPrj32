
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
	unsigned int len;//����
	unsigned char * p;//�������ݵ�ָ��
} ARRAY_U8;

typedef struct {
	unsigned int len;//����
	unsigned int * p;//�������ݵ�ָ��
} ARRAY_U32;

//LIST
struct LIST_T {
	struct LIST_T* head;  // head�Ǹ���Ԫ�أ����ڿ��ٶ�λhead
	struct LIST_T* tail;  // tail�Ǹ���Ԫ�أ����ڿ��ٶ�λtail
	struct LIST_T* prev;  // ��һ��Ԫ��
	struct LIST_T* next;  // ��һ��Ԫ��
	void* data;           // Ԫ������
	unsigned int size;    // Ԫ��bytes // ����head ������
	unsigned int tag;     // �û��Զ����ǩ��������ʶ��data���ͣ�0��0xFFFFFFFF�޷�ʹ�ã�Ϊhead��tail��ǩ
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

