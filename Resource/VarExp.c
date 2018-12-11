/**
  ******************************************************************************
  * @file    /VarExp.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-9-1
  * @brief   a package of var expression function
  * 
@verbatim  
               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "VarExp.h"
	
#include "cdv_include.h"   

//#include <stdio.h>  //计算器
//#include <malloc.h>
#define ReadVar(no)     VarGet(no)
#define ReadI(no)       IBitRead(no)
#define ReadO(no)       OBitRead(no)
#define ReadAdc(no)     AdcRead(no)  //Get_Read_AD( no )
#define ReadDac(no)     DacRead(no)  //Dac_Get_Voltge(no)
#define ReadTime(no)    ReadClock(no)
#define ReadWorker(no)  WorkerRead(no)
#define STACK_SIZE		  20	//max size of the stack
#define STACK_INCREMENT	10	//realloc size
//float模式有效位数7位，2^23
typedef struct FStack  //stack of int
{
	float* base;
	float* top;
	int stacksize;
}FStack;


void InitF(FStack* s)
{
//	s->base = (float*)malloc(STACK_SIZE * sizeof(FStack));
//	if (!s->base)
//	{
//		//printf("overflow!\n");
//		return;
//	}
	NEWCH(s->base, STACK_SIZE * sizeof(float));
	s->top = s->base;
	s->stacksize = STACK_SIZE;
}

void DeInitF(FStack* s)
{
	DELETE(s->base);
	s->top = s->base;
	s->stacksize = 0;
}

BOOL isEmptyF(FStack* s)
{
	if (s->top == s->base)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void PushF(FStack* s, float e)
{
	if (s->top - s->base >= s->stacksize)
	{
		//printf("stack is full!\nrealloc %d\n", STACK_INCREMENT);
		//s->base = (float*)realloc(s->base, (s->stacksize + sizeof(FStack)));
		RENEWCH(s->base, (s->stacksize + sizeof(FStack)));
		if (!s->base)
		{
			//printf("overflow!\n");
			return;
		}
		s->top = s->base + s->stacksize;
		s->stacksize += STACK_INCREMENT;
	}
	*(s->top) = e;
	(s->top)++;
}

float GetTopF(FStack* s)
{
	float e;
	if (s->top == s->base)
	{
		//printf("stack is empty!\n");
		return 0;
	}
	e = *(s->top - 1);
	return e;
}

void PopF(FStack* s)
{
	if (s->top == s->base)
	{
		//printf("stack is empty!\n");
		//DELETE(s->base);
		return;
	}
	s->top--;
}
////////////////////////////////////////////////IStack

//typedef struct IStack  //stack of int
//{
//	int* base;
//	int* top;
//	int stacksize;
//}IStack;

void Init(IStack* s)
{
//	s->base = (float*)malloc(STACK_SIZE * sizeof(FStack));
//	if (!s->base)
//	{
//		//printf("overflow!\n");
//		return;
//	}
	NEWCH(s->base, STACK_SIZE * sizeof(int));
	s->top = s->base;
	s->stacksize = STACK_SIZE;
}

void Clear(IStack* s)//保持堆栈
{
	s->top = s->base;
}

void DeInit(IStack* s)
{
	DELETE(s->base);
	s->top = s->base;
	s->stacksize = 0;
}

BOOL isEmpty(IStack* s)
{
	if (s->top == s->base)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void Push(IStack* s, int e)
{
	if (s->top - s->base >= s->stacksize)
	{
		//printf("stack is full!\nrealloc %d\n", STACK_INCREMENT);
		//s->base = (int*)realloc(s->base, (s->stacksize + sizeof(IStack)));
		RENEWCH(s->base, (s->stacksize + sizeof(IStack)));
		if (!s->base)
		{
			//printf("overflow!\n");
			return;
		}
		s->top = s->base + s->stacksize;
		s->stacksize += STACK_INCREMENT;
	}
	*(s->top) = e;
	(s->top)++;
}

int GetTop(IStack* s)
{
	int e;
	if (s->top == s->base)
	{
		return 0;
	}
	e = *(s->top - 1);
	return e;
}

void Pop(IStack* s)
{
	if (s->top == s->base)
	{
		return;
	}
	s->top--;
}
///////////////////////////////////////////////CStack
//typedef struct CStack  //stack of char
//{
//	char* base;
//	char* top;
//	int stacksize;
//}CStack;

void InitC(CStack* s)
{
//	s->base = (char*)malloc(STACK_SIZE * sizeof(CStack));
//	if (!s->base)
//	{
//		//printf("overflow!\n");
//		return;
//	}
	NEWCH(s->base, STACK_SIZE * sizeof(char));
	s->top = s->base;
	s->stacksize = STACK_SIZE;
}

void ClearC(CStack* s)
{
	s->top = s->base;
}

void DeInitC(CStack* s)
{
	DELETE(s->base);
	s->top = s->base;
	s->stacksize = 0;
}

BOOL isEmptyC(CStack* s)
{
	if (s->top == s->base)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void PushC(CStack* s, int e)
{
	if (s->top - s->base >= s->stacksize)
	{
		//printf("stack is full!\nrealloc %d\n", STACK_INCREMENT);
		//s->base = (char*)realloc(s->base, (s->stacksize + sizeof(CStack)));
		RENEWCH(s->base, (s->stacksize + sizeof(CStack)));
		if (!s->base)
		{
			//printf("overflow!\n");
			return;
		}
		s->top = s->base + s->stacksize;
		s->stacksize += STACK_INCREMENT;
	}
	*(s->top) = e;
	(s->top)++;
}

char GetTopC(CStack* s)
{
	char e;
	if (s->top == s->base)
	{
		//printf("stack is empty!\n");
		return 0;
	}
	e = *(s->top - 1);
	return e;
}

void PopC(CStack* s)
{
	if (s->top == s->base)
	{
		//printf("stack is empty!\n");
		//DELETE(s->base);
		return;
	}
	s->top--;
}


BOOL isPrefixSignOperators(char ch)
{
	if (ch == '+' || ch == '-')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL isArithmeticOperators(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL IsRelationalOperators(char ch)
{
	if (ch == '>' || ch == '=' || ch == '<' || ch == '!' || ch == '{' || ch == '}')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL IsLogicalOperators(char ch)
{
	if (ch == '&' || ch == '|' || ch == '^')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL isNumeric(char ch)
{
	if ((ch >= '0' && ch <= '9') || ch == '.')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL isLBracket(char ch)
{
	if (ch == '(')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL isRBracket(char ch)
{
	if (ch == ')')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL isVar(char ch)
{
	if (ch == 'V')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL isI(char ch)
{
	if (ch == 'I')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL isO(char ch)
{
	if (ch == 'O')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL isAdc(char ch)
{
	if (ch == 'A')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL isDac(char ch)
{
	if (ch == 'D')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL isTime(char ch)
{
	if (ch == 'T')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL isWorker(char ch)
{
	if (ch == 'W')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL isIgnorable(char ch)
{
	if (ch == ' ' || ch == '	')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int Priority(char ch)
{
	int p;
	switch(ch)
	{
	case '(':
		p = 0;
		break;
	case '|':
		p = 1;
		break;
	case '&':
		p = 2;
		break;
	case '^':
		p = 3;
		break;
	case '!':
	case '=':
		p = 4;
		break;
	case '>':
	case '<':
	case '{':
	case '}':
		p = 5;
		break;
	case '+':
	case '-':
		p = 6;
		break;
	case '*':
	case '/':
	case '%':
		p = 7;
		break;
	}
	return p;
}

float CalculateF(float f1, float f2, char oper)
{
	float f3;
	switch(oper)
	{
	case '+':
		f3 = f1 + f2;
		break;
	case '-':
		f3 = f1 - f2;
		break;
	case '*':
		f3 = f1 * f2;
		break;
	case '%':
		f3 = (float)((int)f1 % (int)f2);
		break;
	case '/':
		if (f2 == 0)
		{
			//printf("\nDevided by zero!");
			f3 = 0;
		}
		else
		{
			f3 = f1 / f2;
		}
		break;
	case '{':
		f3 = f1 <= f2;
		break;
	case '}':
		f3 = f1 >= f2;
		break;
	case '=':
		f3 = f1 == f2;
		break;
	case '!':
		f3 = f1 != f2;
		break;
	case '&':
		f3 = f1 && f2;
		break;
	case '|':
		f3 = f1 || f2;
		break;
	case '^':
		f3 = (float)((short)f1 ^ (short)f2);
		break;
	case '>':
		f3 = f1 > f2;
		break;
	case '<':
		f3 = f1 < f2;
		break;
	}
	return f3;
}

int Calculate(int f1, int f2, char oper)
{
	int f3;
	switch(oper)
	{
	case '+':
		f3 = f1 + f2;
		break;
	case '-':
		f3 = f1 - f2;
		break;
	case '*':
		f3 = f1 * f2;
		break;
	case '%':
		f3 = f1 % f2;
		break;
	case '/':
		if (f2 == 0)
		{
			//printf("\nDevided by zero!");
			f3 = 0;
		}
		else
		{
			f3 = f1 / f2;
		}
		break;
	case '{':
		f3 = f1 <= f2;
		break;
	case '}':
		f3 = f1 >= f2;
		break;
	case '=':
		f3 = f1 == f2;
		break;
	case '!':
		f3 = f1 != f2;
		break;
	case '&':
		f3 = f1 && f2;
		break;
	case '|':
		f3 = f1 || f2;
		break;
	case '^':
		f3 = f1 ^ f2;
		break;
	case '>':
		f3 = f1 > f2;
		break;
	case '<':
		f3 = f1 < f2;
		break;
	}
	return f3;
}

int atoi_(const char *str, size_t len)
{
		int value_ = 0;
		int sign = 1;
		if (str[0] == '-') { // handle negative
				sign = -1;
				++str;
				--len;
		}

		switch (len) { // handle up to 10 digits, assume we're 32-bit
				case 10:    value_ += (str[len-10] - '0') * 1000000000;
				case  9:    value_ += (str[len- 9] - '0') * 100000000;
				case  8:    value_ += (str[len- 8] - '0') * 10000000;
				case  7:    value_ += (str[len- 7] - '0') * 1000000;
				case  6:    value_ += (str[len- 6] - '0') * 100000;
				case  5:    value_ += (str[len- 5] - '0') * 10000;
				case  4:    value_ += (str[len- 4] - '0') * 1000;
				case  3:    value_ += (str[len- 3] - '0') * 100;
				case  2:    value_ += (str[len- 2] - '0') * 10;
				case  1:    value_ += (str[len- 1] - '0');
						value_ *= sign;
						return value_;
				default:
						return 0;
		}
}


float StrtoFloat(char* str, int* pos)
{
	float fRes;
	int i = *pos;
	int k;
	char n[50] = {0};
	for (k = 0; k < 50 && ((str[i] >= '0' && str[i] <= '9') || str[i] == '.'); i++, k++)
	{
		n[k] = str[i];
	}
	n[k] = '\0';
	*pos = i - 1;
	fRes = atof(n);
	return fRes;
}

float Char2Float(char* str, int* pos, const short len)
{
	float fRes;
	int i = *pos;
	int k;
	char n[50] = {0};
	
	if('p'==str[i]) {
	  n[k] = '+';
		k++;
		i++;
	}
	else if('n'==str[i]) {
		n[k] = '-';
		k++;
		i++;
	}
	else
	{
	}
	
	for (k = 0; i < len && k < 50 && ((str[i] >= '0' && str[i] <= '9') || str[i] == '.'); i++, k++)
	{
		n[k] = str[i];
	}
	n[k] = '\0';
	*pos = i - 1;
	fRes = atof(n);
	return fRes;
}

int Char2Int(char* str, int* pos, const short len)
{
	int fRes;
	int i = *pos;
	char *dig = str + i;
	int l = 0;
	
	if (str[i] == '+' || str[i] <= '-')
	{
		l++;
		i++;
	}
	
	while (str[i] >= '0' && str[i] <= '9')
	{
		l++;
		i++;
	}
	*pos = i - 1;
	fRes = atoi_(dig, l);
	return fRes;
}

char StrtoSpecialRelationalOperator(const char* str, int* pos)
{
	int i = *pos;
	int k;
	char result = 0;
	for (k = 0; k < 4 && IsRelationalOperators(str[i + k]); k++)
	{
		;
	}
		
	if (1 == k) {
		if('>' == str[i] || '<' == str[i] || '=' == str[i] || '!' == str[i] || '{' == str[i] || '}' == str[i])
			result = str[i];
	}
	
	else if (2 == k) {
		if ('=' == str[i + 1]) {
			switch (str[i])
			{
			case '>':
				result = '}';
				break;
			case '<':
				result = '{';
				break;
			case '=':
				result = '=';
				break;
			case '!':
				result = '!';
				break;
			default:
				break;
			}
		}
	}

	*pos = *pos + k - 1;
	
	return result;
}

char StrtoSpecialLogicalOperator(const char* str, int* pos)
{
	int i = *pos;
	int k;
	char result = 0;
	for (k = 0; k < 4 && IsLogicalOperators(str[i + k]); k++)
	{
		;
	}

	if (1 == k) {
		if ('^' == str[i]|| '&' == str[i]||'|' == str[i])
			result = str[i];
	}
	else if (2 == k) {
		if ('&' == str[i + 1] && '&' == str[i]) {
			result =  '&';
		}
		else if ('|' == str[i + 1] && '|' == str[i]) {
			result = '|';
		}
	}

	*pos = *pos + k - 1;

	return result;
}

BOOL Check(char* str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%' && str[i] != '.' && str[i] != '(' && str[i] != ')' && (str[i] < '0' || str[i] > '9'))
		{
			return FALSE;
		}
		i++;
	}
	return TRUE;
}

/*
左结合规则：
不带空格
数字：数字、操作符、左括号、变量 起始
操作符：数字、右括号
左括号：操作符、左括号  起始
右括号：左括号、数字
变量：操作符、左括号  起始

带空格
数字：操作符、左括号  起始
操作符：数字、右括号
左括号：操作符、左括号  起始
右括号：左括号、数字
变量：操作符、左括号  起始
*/
BOOL CheckEx(char* str, short len)
{
	int i = 0;
	char elm = 0;//0:起始 1:左括 2:操作符 3:数字 4:变量 5:右括 高位1表示被空格阻隔
	while (str[i] != '\0' && i < len)
	{
		if (isLBracket(str[i]))
		{
			
			if((elm & 0x0F) > 2)
				return FALSE;
			
			elm = 1;
		}
		else if (isArithmeticOperators(str[i]))
		{
			if((elm & 0x0F) != 3 && (elm & 0x0F) != 5)
				return FALSE;
			
			elm = 2;
		}
		else if(isNumeric(str[i]))
		{
			if(elm & 0x80)
			{
				if((elm & 0x0F) > 2)
				  return FALSE;
			}
			else
			{
				if((elm & 0x0F) > 4)
				  return FALSE;
			}
			elm = 3;
		}
		else if (isVar(str[i]) || isI(str[i]) || isO(str[i]))
		{
			if((elm & 0x0F) > 2)
				return FALSE;
			
			elm = 4;
		}
		else if (isRBracket(str[i]))
		{
			if((elm & 0x0F) != 3 && (elm & 0x0F) != 1)
				return FALSE;
			
			elm = 5;
		}
		else if(isIgnorable(str[i]))
		{
			elm |= 0x80;//标记有空格
		}
		else
		{
			return FALSE;
		}
		
		i++;
	}
	return TRUE;
}
BOOL CheckExOut(const char* str , char *out, short len)
{
	int i = 0;
	int j = 0;
	int left_brackets = 0, right_brackets = 0;
	char elm = 0;//0:起始 1:左括 2:操作符 3:数字 4:变量 5:右括 高位1表示被空格阻隔
	char current_char;
	while ((current_char = str[i]) != '\0' && i < len)
	{
		if (isLBracket(str[i]))
		{

			if ((elm & 0x0F) > 2)
				return FALSE;
			left_brackets++;
			elm = 1;
		}
		else if (isArithmeticOperators(str[i]))
		{
//			if ((elm & 0x0F) != 3 && (elm & 0x0F) != 5)
//				return FALSE;
			if ((elm & 0x0F) != 3 && (elm & 0x0F) != 5){
				if (isPrefixSignOperators(str[i]) && 2 >= (elm & 0x0F) && i + 1 < len && isNumeric(str[i + 1])) {// + -作为正负号的判断 p + n -
					current_char = str[i] == '+' ? 'p' : 'n';
					elm = 3;
				}	else {
				  return FALSE;
				}
			} else {
			  elm = 2;
			}
		}
		else if (IsRelationalOperators(str[i]))
		{
			if ((elm & 0x0F) != 3 && (elm & 0x0F) != 5)
				return FALSE;

			if (!(current_char = StrtoSpecialRelationalOperator(str, &i)))
				return FALSE;

			elm = 2;
		}
		else if (IsLogicalOperators(str[i]))
		{
			if ((elm & 0x0F) != 3 && (elm & 0x0F) != 5)
				return FALSE;

			if (!(current_char = StrtoSpecialLogicalOperator(str, &i)))
				return FALSE;

			elm = 2;
		}
		else if (isNumeric(str[i]))
		{
			if (elm & 0x80)
			{
				if ((elm & 0x0F) > 2)
					return FALSE;
			}
			else
			{
				if ((elm & 0x0F) > 4)
					return FALSE;
			}
			elm = 3;
		}
		else if (isVar(str[i]) || isI(str[i]) || isO(str[i]) || isAdc(str[i]) || isDac(str[i]) || isTime(str[i]) || isWorker(str[i]))//资源的判断
		{
			if ((elm & 0x0F) > 2)
				return FALSE;

			elm = 4;
		}
		else if (isRBracket(str[i]))
		{
			if ((elm & 0x0F) != 5 && (elm & 0x0F) != 3 && (elm & 0x0F) != 1)
				return FALSE;
			right_brackets++;
			elm = 5;
		}
		else if (isIgnorable(str[i]))
		{
			elm |= 0x80;//标记有空格
			i++;
			continue;
		}
		else
		{
			return FALSE;
		}
		
		if(out)
  		out[j] = current_char;// str[i];
		
		j++;
		i++;
	}
	
	if(out)
  	out[j] = '\0';

	if(right_brackets == left_brackets)
	  return TRUE;
	
	return FALSE;
}


float ArithmeticF(const char* inexp, const short expLen)
{
	char *exp;
	int i;
	float f, f1, f2;
	char oper;
	FStack fstack;
	CStack cstack;
	fstack.base = 0;
	fstack.stacksize = 0;
	fstack.top = 0;
	cstack.base = 0;
	cstack.stacksize = 0;
	cstack.top = 0;
	
	ASSERT(CheckExOut(inexp, NULL, expLen));
	
	exp = (char*)inexp;
	
	InitF(&fstack);
	InitC(&cstack);
	
	for (i = 0; expLen != i && exp[i] != '\0' && exp[i] != -52/*0xCC*/; i++)
	{
		if (isArithmeticOperators(exp[i]) || IsLogicalOperators(exp[i]) || IsRelationalOperators(exp[i]))
		{
			if (!isEmptyC(&cstack))
			{
				while (!isEmptyC(&cstack) && Priority(exp[i]) <= Priority(GetTopC(&cstack)))
				{
					oper = GetTopC(&cstack);
					PopC(&cstack);
					f2 = GetTopF(&fstack);
					PopF(&fstack);
					f1 = GetTopF(&fstack);
					PopF(&fstack);
					f = CalculateF(f1, f2, oper);
					PushF(&fstack, f);
				}
				PushC(&cstack, exp[i]);
			}
			else
			{
				PushC(&cstack, exp[i]);
			}
		}
		else if (isNumeric(exp[i]))
		{
			f = Char2Float(exp, &i, expLen);//StrtoFloat(exp, &i);
			PushF(&fstack, f);
		}
		else if ('p'==exp[i] || 'n'==exp[i])
		{
			f = Char2Float(exp, &i, expLen);//StrtoFloat(exp, &i);
			PushF(&fstack, f);
		}
		else if (exp[i] == '(')
		{
			PushC(&cstack, exp[i]);
		}
		else if (exp[i] == ')')
		{
			while (GetTopC(&cstack) != '(' && !isEmptyC(&cstack))
			{
				oper = GetTopC(&cstack);
				PopC(&cstack);
				f2 = GetTopF(&fstack);
				PopF(&fstack);
				f1 = GetTopF(&fstack);
				PopF(&fstack);
				f = CalculateF(f1, f2, oper);
				PushF(&fstack, f);
			}
			PopC(&cstack);
		}
		else if (isVar(exp[i]))//变量
		{
			i++;
			f = Char2Float(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadVar(f);
			PushF(&fstack, f);
		}
		else if (isI(exp[i]))//I
		{
			i++;
			f = Char2Float(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadI(f-1);
			PushF(&fstack, f);
		}
		else if (isO(exp[i]))//O
		{
			i++;
			f = Char2Float(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadO(f-1);
			PushF(&fstack, f);
		}
		else if (isAdc(exp[i]))//ADC
		{
			i++;
			f = Char2Float(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadAdc((int)f-1);
			PushF(&fstack, f);
		}
		else if (isDac(exp[i]))//DAC
		{
			i++;
			f = Char2Float(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadDac((int)f-1);
			PushF(&fstack, f);
		}
		else if (isTime(exp[i]))//TIME
		{
			i++;
			f = Char2Float(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadTime(f);
			PushF(&fstack, f);
		}
	}
	//exp  analyse  over
	while (!isEmptyC(&cstack))
	{
		oper = GetTopC(&cstack);
		PopC(&cstack);
		f2 = GetTopF(&fstack);
		PopF(&fstack);
		f1 = GetTopF(&fstack);
		PopF(&fstack);
		f = CalculateF(f1, f2, oper);
		PushF(&fstack, f);
	}
	//printf("\nThe result is:%f\n", GetTop(&fstack));
	PopF(&fstack);
	
	DeInitF(&fstack);
	DeInitC(&cstack);
	return f;
}

int Arithmetic(const char* inexp, const short expLen)
{
	char *exp = NULL;
	int i;
	int f, f1, f2;
	char oper;
	IStack istack;
	CStack cstack;
	istack.base = 0;
	istack.stacksize = 0;
	istack.top = 0;
	cstack.base = 0;
	cstack.stacksize = 0;
	cstack.top = 0;
	
	ASSERT(CheckExOut(inexp, NULL, expLen));
	
	exp = (char*)inexp;
	
	Init(&istack);
	InitC(&cstack);
	
	for (i = 0; expLen != i && exp[i] != '\0' && exp[i] != -52/*0xCC*/; i++)
	{
		if (isArithmeticOperators(exp[i]) || IsLogicalOperators(exp[i]) || IsRelationalOperators(exp[i]))
		{
			if (!isEmptyC(&cstack))
			{
				while (!isEmptyC(&cstack) && Priority(exp[i]) <= Priority(GetTopC(&cstack)))
				{
					oper = GetTopC(&cstack);
					PopC(&cstack);
					f2 = GetTop(&istack);
					Pop(&istack);
					f1 = GetTop(&istack);
					Pop(&istack);
					f = Calculate(f1, f2, oper);
					Push(&istack, f);
				}
				PushC(&cstack, exp[i]);
			}
			else
			{
				PushC(&cstack, exp[i]);
			}
		}
		else if (isNumeric(exp[i]))
		{
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			Push(&istack, f);
		}
		else if ('p'==exp[i] || 'n'==exp[i])
		{
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			Push(&istack, f);
		}
		else if (exp[i] == '(')
		{
			PushC(&cstack, exp[i]);
		}
		else if (exp[i] == ')')
		{
			while (GetTopC(&cstack) != '(' && !isEmptyC(&cstack))
			{
				oper = GetTopC(&cstack);
				PopC(&cstack);
				f2 = GetTop(&istack);
				Pop(&istack);
				f1 = GetTop(&istack);
				Pop(&istack);
				f = Calculate(f1, f2, oper);
				Push(&istack, f);
			}
			PopC(&cstack);
		}
		else if (isVar(exp[i]))//变量
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadVar(f);
			Push(&istack, f);
		}
		else if (isI(exp[i]))//I
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadI(f-1);
			Push(&istack, f);
		}
		else if (isO(exp[i]))//O
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadO(f-1);
			Push(&istack, f);
		}
		else if (isAdc(exp[i]))//ADC
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadAdc(f-1);
			Push(&istack, f);
		}
		else if (isDac(exp[i]))//DAC
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadDac(f-1);
			Push(&istack, f);
		}
		else if (isTime(exp[i]))//TIME
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadTime(f);
			Push(&istack, f);
		}
	}
	//exp  analyse  over
	while (!isEmptyC(&cstack))
	{
		oper = GetTopC(&cstack);
		PopC(&cstack);
		f2 = GetTop(&istack);
		Pop(&istack);
		f1 = GetTop(&istack);
		Pop(&istack);
		f = Calculate(f1, f2, oper);
		Push(&istack, f);
	}
	//printf("\nThe result is:%f\n", GetTop(&istack));
	Pop(&istack);
	
	DeInit(&istack);
	DeInitC(&cstack);
	return f;
}


int ArithmeticEx(const char* inexp, const short expLen, CMD_ARG *arg)
{
	char *exp = NULL;
	int i;
	int f, f1, f2;
	//float ff;
	char oper;
	IStack *istack = &arg->stack.istack;
	CStack *cstack = &arg->stack.cstack;
	
	ASSERT(CheckExOut(inexp, NULL, expLen));
	
	exp = (char*)inexp;
	
	Clear(istack);
	ClearC(cstack);
	
	for (i = 0; expLen != i && exp[i] != '\0' && exp[i] != -52/*0xCC*/; i++)
	{
		if (isNumeric(exp[i]))
		{
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			Push(istack, f);
		}
		else if (isVar(exp[i]))//变量
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadVar(f);
			Push(istack, f);
		}
		else if (isI(exp[i]))//I
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadI(f-1);
			Push(istack, f);
		}
		else if (exp[i] == '(')
		{
			PushC(cstack, exp[i]);
		}
		else if (exp[i] == ')')
		{
			while (GetTopC(cstack) != '(' && !isEmptyC(cstack))
			{
				oper = GetTopC(cstack);
				PopC(cstack);
				f2 = GetTop(istack);
				Pop(istack);
				f1 = GetTop(istack);
				Pop(istack);
				f = Calculate(f1, f2, oper);
				Push(istack, f);
			}
			PopC(cstack);
		}
		else if (isArithmeticOperators(exp[i]) || IsLogicalOperators(exp[i]) || IsRelationalOperators(exp[i]))
		{
			if (!isEmptyC(cstack))
			{
				while (!isEmptyC(cstack) && Priority(exp[i]) <= Priority(GetTopC(cstack)))
				{
					oper = GetTopC(cstack);
					PopC(cstack);
					f2 = GetTop(istack);
					Pop(istack);
					f1 = GetTop(istack);
					Pop(istack);
					f = Calculate(f1, f2, oper);
					Push(istack, f);
				}
				PushC(cstack, exp[i]);
			}
			else
			{
				PushC(cstack, exp[i]);
			}
		}
		else if ('p'==exp[i] || 'n'==exp[i])
		{
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			Push(istack, f);
		}
		else if (isO(exp[i]))//O
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadO(f-1);
			Push(istack, f);
		}
		else if (isAdc(exp[i]))//ADC
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadAdc(f-1);
			Push(istack, f);
		}
		else if (isDac(exp[i]))//DAC
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadDac(f-1);
			Push(istack, f);
		}
		else if (isTime(exp[i]))//TIME
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadTime(f);
			Push(istack, f);
		}
		else if (isWorker(exp[i]))//Worker
		{
			i++;
			f = Char2Int(exp, &i, expLen);//StrtoFloat(exp, &i);
			f = ReadWorker(f);
			Push(istack, f);
		}
	}
	//exp  analyse  over
	while (!isEmptyC(cstack))
	{
		oper = GetTopC(cstack);
		PopC(cstack);
		f2 = GetTop(istack);
		Pop(istack);
		f1 = GetTop(istack);
		//ff =f1;
		//f1 = ff;
		
		Pop(istack);
		f = Calculate(f1, f2, oper);
		Push(istack, f);
	}
	//printf("\nThe result is:%f\n", GetTop(&istack));
	Pop(istack);

	return f;
}