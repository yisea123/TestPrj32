
/**
  ******************************************************************************
  * @file    /Var.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-9-1
  * @brief   a package of var expression function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _VAR_EXP_
#define  _VAR_EXP_

typedef struct IStack  //stack of int
{
	int* base;
	int* top;
	int stacksize;
}IStack;

typedef struct CStack  //stack of char
{
	char* base;
	char* top;
	int stacksize;
}CStack;

typedef struct ArithmeticStack  //stack of char
{
	IStack istack;
	CStack cstack;
}ArithmeticStack;
 


float ArithmeticF(const char* inexp, const short expLen);
int Arithmetic(const char* inexp, const short expLen);
void Init(IStack* s);
void DeInit(IStack* s);
void InitC(CStack* s);
void DeInitC(CStack* s);
#endif

