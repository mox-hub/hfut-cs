#include <stdio.h>

#define MAXLEN 100
//MAXLEN 最大容量 
typedef struct
{
	int data[MAXLEN];//数据域 
	int top;//显示栈顶元素的位置 
}seqStack;

typedef struct lsNode
{
	int data; //链栈结点数据域
	struct lsNode *next; //链栈结点指针域
} node;


//(1)初始化 
void initStack(seqStack *S)
{
	S->top = -1;
}

//(2)判栈空 
void stackEmpty(seqStack *S)
{
	if (S->top == -1)
		printf("栈空！\n");

	else
		printf("栈非空!\n");
}

//(3)判栈满 
void stackFull(seqStack S)
{
	if (S.top == MAXLEN - 1)//判断栈满的条件 
		printf("栈满！\n");
	else
		printf("栈非满！\n");
}

//(4)取栈顶元素 
int stackTop(seqStack* S)
{
	int x;
	if (S->top == -1)
		printf("栈空！\n");
	else
	{
		x = S->data[S->top];//参数x返回栈顶元素 
		return x;	
	}
}

//(5)入栈
void push(seqStack* S, int x)
{
	if (S->top == MAXLEN - 1)
		printf("栈满！\n");
	else
	{
		S->top++;//栈顶指针后移 
		S->data[S->top] = x; //元素x入栈 
	}
}

//(6)出栈 
int pop(seqStack* S)
{
	int x;
	if (S->top == -1)
		printf("栈空，不能删除\n");
	else
	{
		x = S->data[S->top];//取栈顶元素至x 
		S->top--;//栈顶减1，删除了栈顶元素 
	}
	return x;
}
//<1>利用顺序栈实现将10进制数转换为16进制数。
void numberBaseSwitch(int N)
{
	int mod;
	int x = 0;
	seqStack S;
	initStack(&S);
	while(N != 0)
	{
		mod = N % 16; 
//		printf("%d\n",mod);
		push(&S,mod);
		N = N / 16;
	}
	printf("16进制数为：");
	while(S.top != -1)
	{
		x = pop(&S);
		switch(x)
		{
			case 10: printf("A");break;
			case 11: printf("B");break;
			case 12: printf("C");break;
			case 13: printf("D");break;
			case 14: printf("E");break;
			case 15: printf("F");break;
			default: printf("%d",x);
		}

	}
}

//
char leftOfRight(char c)
{
   if (c == ')') return '(';
   if (c == ']') return '[';
    return '{';
}
void isValid(char str[])
{	   
	seqStack left;
	initStack(&left);
	char c;
	int i = 0;
	while(str[i] != '\0')
	{
		c = str[i];
		if(c == '(' || c == '[' || c == '{')
			push(&left,str[i]);
		else
		{
			if(left.top != -1 && leftOfRight(c) == (char)pop(&left))	
				pop(&left);
			else
			{
				printf("出现错误！\n");
				break;
			}
		} 			
	}	
	printf("未出现错误！\n");
}

void initialStack(node *top)
{
	top = NULL;
}

void getTop(node * top, int x)
{
	if(top == NULL)
		printf("栈空！\n");
	else
	{
		x=top->data; //取栈顶元素，用参数x返回
		printf("取值成功！\n");
	} 
}
void pushP(node * top, int x)
{
	node* s;
	s->data = x;
	s->next = top;
	top = s;
}

void popP(node * top, int x)
{ 
	node* u;
	if(top==NULL)
		printf("栈空！\n");
	else
	{
		x=top->data; //取栈顶元素，由变量x返回
		u=top; //栈顶指针保存到u
		top=top->next; //栈顶指针后移一个元素结点
		printf("；出栈成功！"); 
	}
}
//两部分的算法一致，引用的函数名一致 
