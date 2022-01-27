//seqStack.h
//--顺序栈的定义和运算实现
//--此处为了兼容，有些定义做了修改

#include "iostream"

//-----------------------------------------------------------------------
//定义符号
#define MAXLEN 100
typedef int SeleType;
//-----------------------------------------------------------------------
//顺序栈存储定义
typedef struct sStack
{
	SeleType data[MAXLEN];
	int top;
}seqStack;
//-----------------------------------------------------------------------
//1. 栈初始化
void initialStack(seqStack &S)
{
	S.top=-1;
}
//-----------------------------------------------------------------------
//2. 判断栈空
bool stackEmpty(seqStack &S) //传值、指针、引用
{
	if(S.top==-1)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------
//3. 取栈顶元素
bool getTop(seqStack &S, SeleType &x)
{
	if(stackEmpty(S))
		return false; //空栈，返回false
	else
	{
		x=S.data[S.top];
		return true;  //取得栈顶，返回true；取得的值有 x 传递。
	}
}
//-----------------------------------------------------------------------
//4. 判断栈满
bool stackFull(seqStack &S)
{
	if(S.top==MAXLEN-1)
		return true; //栈满，返回true
	else
		return false; //栈未满，返回false
}

//-----------------------------------------------------------------------
//5. 入栈
bool push(seqStack &S, SeleType x)
{
	if(stackFull(S))
		return false; //栈满，不能入栈，返回false
	else
	{
		S.top++; //增加栈顶
		S.data[S.top]=x; //数据入栈
		return true;
	}
} 
//-----------------------------------------------------------------------
//6. 出栈
bool pop(seqStack &S, SeleType &r) //r带回弹出的元素
{
	if(stackEmpty(S)) //空栈，没元素出栈，返回false
		return false;
	else
	{
		r=S.data[S.top];
		S.top--;
		return true;
	}
}

//打印栈元素
void printStack(seqStack &S)
{
	int i;
	if(S.top==-1)
		cout<<"空栈。";
	else
	{
		cout<<"栈内元素数："<<S.top+1<<"。"<<endl;
		cout<<"栈元素：";
		for(i=0;i<=S.top;i++)
		{
			cout<<S.data[i]<<"\t";			
		}		
	}
	cout<<endl<<endl;
}

//交互循环压入元素
void createSeqStack(seqStack &S)
{		
	SeleType x;
	
	cout<<"请输入数据元素(整数，-9999退出):"<<endl;
	cout<<"x=";
	cin>>x;

	while(x!=-9999 && S.top!=MAXLEN-1)   //非结束符，且栈未满，循环入栈
	{
		S.top++;         //增加栈顶
		S.data[S.top]=x; //数据入栈

		cout<<"x=";
		cin>>x;
	}
	if(S.top==MAXLEN-1)
		cout<<"栈满，不能再压入元素。"<<endl;
}

//随机数循环压入元素开始------------------------------------------------------------------
//随机数创建顺序栈
void rndCStack(seqStack &S)
{
	int i;
	int n,m;
	S.top=-1;  //置为空栈
	cout<<"请输入要产生的随机数个数，n=";
	cin>>n;
	
	if(n>MAXLEN)
	{
		cout<<"您要求产生的随机数个数超出了顺序栈长度"<<MAXLEN<<"，创建顺序栈失败。"<<endl;
		return;	
	}	
	cout<<"请输入控制随机数大小参数，比如100以内数，请输入100，m=";
	cin>>m;    
	
	srand((unsigned)time(NULL));	//产生随机数种子
	//srand((unsigned)GetTickCount());	//产生随机数种子
	for(i=0;i<n;i++)                //随机数写入排序表A[]
	{
		S.top++;
		S.data[i]=rand()%m;
	}
	cout<<endl;
}
//随机数循环压入元素结束------------------------------------------------------------------
