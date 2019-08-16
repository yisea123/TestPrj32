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
#include "cdv_include.h"

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////Array void Function///////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
/*�����߸����ͷ�*/
//size ���͵��ֽ���
ARRAY_VOID* CreateArray(size_t size) {
	ARRAY_VOID* array = NULL;
	NEWCH(array,sizeof(ARRAY_VOID));
	array->size = size;
	return array;
}


void ClearArray(ARRAY_VOID *array) {
	ASSERT(array);
	DELETE(array->p);
	array->len = 0;
}
//��CteateArrayһ����
void	DeleteArray(ARRAY_VOID **array) {
	ClearArray(*array);
	DELETE(*array);
}


//void��������ע��data������
CDV_INT08S ArrayAdd(ARRAY_VOID *array, void* data) {
	void *tmp = NULL;
	ASSERT(array);
	//ASSERT(array->len < MAX_ARRAY_LEN); 
	if(array->len >= MAX_ARRAY_LEN)
		return -1;
	
	if(array->len) {
		NEWCH(tmp , (array->len+1)*array->size);
		MemCpy(tmp , array->p , (array->len)*array->size);
		DELETE(array->p);
		array->p = tmp;
	} else {
		NEWCH(array->p , (array->len+1)*array->size);
	}
	
	MemCpy((char*)array->p + (array->len)*array->size, data , array->size);
	array->len++;
	return 0;
}


CDV_INT08S ArrayDelete(ARRAY_VOID *array, CDV_INT32U no) {
	ASSERT(array);
	if(no >= array->len)
		return -1;

	MemCpyUnsafe((char*)array->p + (no)*array->size, (char*)array->p + (no+1)*array->size , (array->len-1-no)*array->size);
	array->len--;
	return 0;
}


//void��������ע��data������
CDV_INT08S ArrayModify(ARRAY_VOID *array, CDV_INT32U no, void* data) {
	ASSERT(array);
	if(no >= array->len)
		return -1;

	MemCpy((char*)array->p + (no)*array->size, data , array->size);
	return 0;
}

void* ArrayGet(ARRAY_VOID *array, CDV_INT32U no) {
	ASSERT(array);
	if(no >= array->len)
		return NULL;

	return (char*)array->p + (no)*array->size;
}

void ArrayTest(void) {
	typedef struct tagTest {
		char ch1;
		int  w1;
		char ch2;
	} test_type;
	int size = 0, i;
	ARRAY_VOID* testArray = NULL;
	test_type data = {0, 0, 0};
	size = sizeof(test_type);
	testArray = CreateArray(size);
	
	for(i = 0; i < 10 ; i++) {
		data.ch1++;
		data.ch2+=2;
		data.w1+=3;
		ArrayAdd(testArray, &data);
	}
	
	DeleteArray(&testArray);
	
	
}

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////ArrayU32 Function///////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
/*�����߸����ͷ�*/
ARRAY_U32* CteateArrayU32(void) {
	
	ARRAY_U32* array = NULL;
	NEWCH(array,sizeof(ARRAY_U32));
	return array;
	
}


void	DeleteArrayU32(ARRAY_U32 **array) {
	
	ClearArrayU32(*array);
	DELETE(*array);
}

void WriteArrayU32(ARRAY_U32 *array, CDV_INT32U *p, CDV_INT32U len) {
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
	
/*�����߸����ͷ�*/
ARRAY_U8* CteateArrayU8(void) {
	
	ARRAY_U8* array = NULL;
	NEWCH(array,sizeof(ARRAY_U8));
	return array;
	
}


void	DeleteArrayU8(ARRAY_U8 **array) {
	
	ClearArrayU8(*array);
	DELETE(*array);
}

void WriteArrayU8(ARRAY_U8 *array, CDV_INT32U *p, CDV_INT32U len) {
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
	//////////////////////δ���/////////////////////////
	///////////////////////////////////////////////////////
//
typedef struct TREE_T {
	struct TREE_T* father;
	struct TREE_T* lchild;
	struct TREE_T* rchild;
	void* data;
	size_t size;
}CDV_TREE;

//�û������ͷ�
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
  //���̰߳�ȫ��ʹ��ʱע��

//�û������ͷ�
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
// tail���
// ����һԪ��
CDV_LIST* LIST_AddTail(CDV_LIST *elm, void *data/* , size_t size*/, CDV_INT32U tag) {
  OS_ERR err;
	CDV_LIST *add = NULL; // ���ӵ�Ԫ��
	CDV_LIST *head = NULL; // ��ͷ
	CDV_LIST *tail = NULL; // ��β
	CDV_LIST *last = NULL; // ĩβԪ��
	ASSERT(elm && data /*&& size*/);
	ASSERT(tag != TAG_HEAD && tag != TAG_TAIL);
	NEWCH(add,sizeof(CDV_LIST));
	head = elm->head;
	tail = elm->tail;
  OSMutexPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
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
	add->time = GetCdvTimeTick();
	
	last->next = add;
	
	tail->prev = add;

	head->size++;
  OSMutexPost (&LIST_SEM,OS_OPT_POST_NO_SCHED,&err);
	return add;
}

//head���
CDV_LIST* LIST_AddHead(CDV_LIST *elm, void *data ,/* size_t size, */CDV_INT32U tag) {
	OS_ERR err;
	CDV_LIST *add = NULL; // ���ӵ�Ԫ��
	CDV_LIST *head = NULL; // ��ͷ
	CDV_LIST *tail = NULL; // ��β
	CDV_LIST *first = NULL; // ��βԪ��
	ASSERT(elm && data /*&& size*/);
	ASSERT(tag != TAG_HEAD && tag != TAG_TAIL);
	NEWCH(add,sizeof(CDV_LIST));
	head = elm->head;
	tail = elm->tail;
	OSMutexPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
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
	OSMutexPost (&LIST_SEM,OS_OPT_POST_NO_SCHED,&err);
	return add;
}

// ǰ����
// head֮ǰ���ܲ���
CDV_LIST* LIST_Insert(CDV_LIST *elm, void *data ,/* size_t size,*/ CDV_INT32U tag) {
	OS_ERR err;
	CDV_LIST *insert = NULL; // ���ӵ�Ԫ��
	CDV_LIST *head = NULL; // ��ͷ
	CDV_LIST *tail = NULL; // ��β
	CDV_LIST *prev = NULL; // 
	ASSERT(elm && data /*&& size*/);
	ASSERT(tag != TAG_HEAD);
	NEWCH(insert,sizeof(CDV_LIST));
	head = elm->head;
	tail = elm->tail;
	OSMutexPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
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
	OSMutexPost (&LIST_SEM,OS_OPT_POST_NO_SCHED,&err);
	return insert;
}

CDV_LIST* LIST_Remove(CDV_LIST *elm) {
	OS_ERR err;
	CDV_LIST *prev = NULL; // ��ͷ
	CDV_LIST *next = NULL; // ��β
	ASSERT(elm);
	prev = elm->prev;
	next = elm->next;
	
	if (elm->tag == TAG_HEAD || elm->tag == TAG_TAIL) {
		LIST_RemoveAll(elm);
		return NULL;
	}
	OSMutexPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
	//DELETE(elm->data);
	DELETE(elm);
	prev->next = next;
	next->prev = prev;
	prev->head->size--;
	OSMutexPost (&LIST_SEM,OS_OPT_POST_NO_SCHED,&err);
	return next;
}


void LIST_RemoveAll(CDV_LIST *elm) {
	OS_ERR err;
	CDV_LIST *tail = NULL; // ��β
	CDV_LIST *deling = NULL;
	CDV_INT32U num = elm->head->size + 2;
	
	ASSERT(elm);
	deling = elm->head;
	tail = elm->tail;
	OSMutexPend(&LIST_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
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
	OSMutexPost (&LIST_SEM,OS_OPT_POST_NO_SCHED,&err);
	ASSERT(1 == num);
	
}

/** @brief  LIST
  * @param  
  * @retval 
  * @note   
  */
void LIST_Check(CDV_LIST *elm)  
{  
	CDV_LIST *head = NULL; // ��β
	CDV_LIST *tail = NULL;
	CDV_LIST *deal = NULL;
	CDV_INT32U num ;
	
	ASSERT(elm && elm->head && elm->tail);
	head = elm->head;
	tail = elm->tail;
	ASSERT(head->data == NULL && head->tag == TAG_HEAD);
	ASSERT(tail->data == NULL && tail->size == 0 && tail->tag == TAG_TAIL);
	deal = head;
	num = head->size + 2;
	
	while (--num) { 
		ASSERT(deal->next->prev == deal);
		// ��head
		deal = deal->next;
		ASSERT(deal->prev->next == deal);
		ASSERT(deal->head == head);
		ASSERT(deal->tail == tail);
		
		if(deal == tail) {
			break;
		}
		// ��ͷβ
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
	static CDV_INT32U list_test_on = 0;
	CDV_INT32U num = list_test_on;
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


