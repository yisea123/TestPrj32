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

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	///////////////////Tree Function///////////////////////
	///////////////////////////////////////////////////////
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
	memcpy(insert_tree->data, data, size);
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
	memcpy(insert_tree->data, data, size);
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