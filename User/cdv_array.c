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
	MemCpy(array->p, p, len);
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
	
	if(array->len) {
		NEW32U(tmp , array->len);
		MemCpy(tmp , array->p , array->len * sizeof(CDV_INT32U));
		NEW32U(array->p , array->len + 1);
		MemCpy(array->p , tmp , array->len * sizeof(CDV_INT32U));
	} else {
		NEW32U(array->p , array->len + 1);
	}
	
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
	MemCpy(array->p, p, len);
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
	MemCpy(tmp , array->p , array->len * sizeof(CDV_INT32U));
	NEW32U(array->p , array->len + 1);
	MemCpy(array->p , tmp , array->len * sizeof(CDV_INT32U));
	*(array->p + array->len ) = data;
	array->len++;
	DELETE(tmp);
}

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////Tree Function///////////////////////
	//////////////////////未完成/////////////////////////
	///////////////////////////////////////////////////////
//
typedef struct TREE_T {
	struct TREE_T* father;
	struct TREE_T* lchild;
	struct TREE_T* rchild;
	void* data;
	size_t size;
}CDV_TREE;

//用户负责释放
CDV_TREE* CteateTree(void) {
	
	CDV_TREE* tree = NULL;
	NEWCH(tree,sizeof(CDV_TREE));
	return tree;
	
}

CDV_TREE* InsertLNode(CDV_TREE *node, void *data , size_t size) {
	CDV_TREE *insert_tree = CteateTree();
	NEWCH(insert_tree->data , size);
	MemCpy(insert_tree->data, data, size);
	insert_tree->size = size;
	insert_tree->father = node;
	insert_tree->lchild = node->lchild;
	insert_tree->rchild = NULL;
	node->lchild->father = insert_tree;
	node->lchild = insert_tree;
	return insert_tree;
}

CDV_TREE* InsertRNode(CDV_TREE *node, void *data , size_t size) {
	CDV_TREE *insert_tree = CteateTree();
	NEWCH(insert_tree->data , size);
	MemCpy(insert_tree->data, data, size);
	insert_tree->size = size;
	insert_tree->father = node;
	insert_tree->rchild = node->rchild;
	insert_tree->lchild = NULL;
	node->rchild->father = insert_tree;
	node->rchild = insert_tree;
	return insert_tree;
}

void RemoveNode(CDV_TREE *node) {
	if (node->lchild) {
		RemoveNode(node->lchild);
		node->lchild = NULL;
	}
	
	if (node->rchild) {
		RemoveNode(node->rchild);
		node->rchild = NULL;
	}
	DELETE(node->data);
}


void	DeleteTree(CDV_TREE **tree) {
	
	RemoveNode(*tree);
	DELETE(*tree);
}


	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////LIST Function///////////////////////
	/////////////head - elm - elm - elm - tail ////////////
	///////////////////////////////////////////////////////
  //非线程安全，使用时注意

//用户负责释放
CDV_LIST* LIST_Cteate(void) {
	
	CDV_LIST* head = NULL;
	CDV_LIST* tail = NULL;
	NEWCH(head,sizeof(CDV_LIST));
	NEWCH(tail,sizeof(CDV_LIST));
	
	head->head = head;
	head->tail = tail;
	head->prev = NULL;
	head->next = tail;
	head->tag  = TAG_HEAD;
	
	tail->head = head;
	tail->tail = tail;
	tail->prev = head;
	tail->next = NULL;
	tail->tag  = TAG_TAIL;
	return head;
	
}
// tail添加
// 链表一元素
CDV_LIST* LIST_AddTail(CDV_LIST *elm, void *data/* , size_t size*/, uint32_t tag) {
  OS_ERR err;
	CDV_LIST *add = NULL; // 增加的元素
	CDV_LIST *head = NULL; // 开头
	CDV_LIST *tail = NULL; // 结尾
	CDV_LIST *last = NULL; // 末尾元素
	ASSERT(elm && data /*&& size*/);
	ASSERT(tag != TAG_HEAD && tag != TAG_TAIL);
	NEWCH(add,sizeof(CDV_LIST));
	head = elm->head;
	tail = elm->tail;
  OSSemPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
	last = tail->prev;
	/*NEWCH(add->data , size);*/
	/*MemCpy(add->data, data, size);*/
	add->data = data;
	/*add->size = size;*/
	add->head = head;
	add->tail = tail;
	add->prev = last;
	add->next = tail;
	add->tag  = tag;
	
	last->next = add;
	
	tail->prev = add;

	head->size++;
  OSSemPost (&LIST_SEM,OS_OPT_POST_1,&err);
	return add;
}

//head添加
CDV_LIST* LIST_AddHead(CDV_LIST *elm, void *data ,/* size_t size, */uint32_t tag) {
	OS_ERR err;
	CDV_LIST *add = NULL; // 增加的元素
	CDV_LIST *head = NULL; // 开头
	CDV_LIST *tail = NULL; // 结尾
	CDV_LIST *first = NULL; // 首尾元素
	ASSERT(elm && data /*&& size*/);
	ASSERT(tag != TAG_HEAD && tag != TAG_TAIL);
	NEWCH(add,sizeof(CDV_LIST));
	head = elm->head;
	tail = elm->tail;
	OSSemPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
	first = head->next;
	/*NEWCH(add->data , size);
	MemCpy(add->data, data, size);*/
	//add->size = size;
	add->data = data;
	add->head = head;
	add->tail = tail;
	add->prev = head;
	add->next = first;
	add->tag  = tag;
	
	first->prev = add;
	
	head->next = add;
	
	head->size++;
	OSSemPost (&LIST_SEM,OS_OPT_POST_1,&err);
	return add;
}

// 前插入
// head之前不能插入
CDV_LIST* LIST_Insert(CDV_LIST *elm, void *data ,/* size_t size,*/ uint32_t tag) {
	OS_ERR err;
	CDV_LIST *insert = NULL; // 增加的元素
	CDV_LIST *head = NULL; // 开头
	CDV_LIST *tail = NULL; // 结尾
	CDV_LIST *prev = NULL; // 
	ASSERT(elm && data /*&& size*/);
	ASSERT(tag != TAG_HEAD);
	NEWCH(insert,sizeof(CDV_LIST));
	head = elm->head;
	tail = elm->tail;
	OSSemPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
	prev = elm->prev;
	/*NEWCH(insert->data , size);
	MemCpy(insert->data, data, size);
	insert->size = size;*/
	insert->data = data;
	insert->head = head;
	insert->tail = tail;
	insert->prev = prev;
	insert->next = elm;
	insert->tag  = tag;
	elm->prev = insert;
	
	prev->next = insert;
	
	head->size++;
	OSSemPost (&LIST_SEM,OS_OPT_POST_1,&err);
	return insert;
}

CDV_LIST* LIST_Remove(CDV_LIST *elm) {
	OS_ERR err;
	CDV_LIST *prev = NULL; // 开头
	CDV_LIST *next = NULL; // 结尾
	ASSERT(elm);
	prev = elm->prev;
	next = elm->next;
	
	if (elm->tag == TAG_HEAD || elm->tag == TAG_TAIL) {
		LIST_RemoveAll(elm);
		return NULL;
	}
	OSSemPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
	//DELETE(elm->data);
	DELETE(elm);
	prev->next = next;
	next->prev = prev;
	prev->head->size--;
	OSSemPost (&LIST_SEM,OS_OPT_POST_1,&err);
	return next;
}


void LIST_RemoveAll(CDV_LIST *elm) {
	OS_ERR err;
	CDV_LIST *tail = NULL; // 结尾
	CDV_LIST *deling = NULL;
	uint32_t num = elm->head->size + 2;
	
	ASSERT(elm);
	deling = elm->head;
	tail = elm->tail;
	OSSemPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
	while (--num) { 
		//DELETE(deling->data);
		deling = deling->next;
		DELETE(deling->prev);
		
		if(deling == tail) {
		  //DELETE(deling->data);
		  DELETE(deling);
			break;
		}
	}
	OSSemPost (&LIST_SEM,OS_OPT_POST_1,&err);
	ASSERT(1 == num);
	
}

/** @brief  LIST
  * @param  
  * @retval 
  * @note   
  */
void LIST_Check(CDV_LIST *elm)  
{  
	CDV_LIST *head = NULL; // 结尾
	CDV_LIST *tail = NULL;
	CDV_LIST *deal = NULL;
	uint32_t num ;
	
	ASSERT(elm && elm->head && elm->tail);
	head = elm->head;
	tail = elm->tail;
	ASSERT(head->data == NULL && head->tag == TAG_HEAD);
	ASSERT(tail->data == NULL && tail->size == 0 && tail->tag == TAG_TAIL);
	deal = head;
	num = head->size + 2;
	
	while (--num) { 
		ASSERT(deal->next->prev == deal);
		// 非head
		deal = deal->next;
		ASSERT(deal->prev->next == deal);
		ASSERT(deal->head == head);
		ASSERT(deal->tail == tail);
		
		if(deal == tail) {
			break;
		}
		// 非头尾
		ASSERT(deal->data && deal->size);
	  ASSERT(deal->tag != TAG_HEAD && deal->tag != TAG_TAIL);
	}
	
	ASSERT(1 == num);
}

/** @brief  LIST
  * @param  
  * @retval 
  * @note   
  */
void LIST_Test(void)  
{  
	CDV_LIST *list = NULL;
	static uint32_t list_test_on = 0;
	uint32_t num = list_test_on;
	if(list_test_on) {
		list = LIST_Cteate();
		
		while(num--) {
			list = LIST_AddTail(list, &num, /*sizeof(num),*/ 4);
			list = LIST_AddHead(list, &num, /*sizeof(num*/ 4);
			list = LIST_Insert(list, &num, /*sizeof(num),*/ 4);
			list = LIST_Remove(list);
		}
		
		LIST_Check(list);
		
		LIST_RemoveAll(list);
	}
}


