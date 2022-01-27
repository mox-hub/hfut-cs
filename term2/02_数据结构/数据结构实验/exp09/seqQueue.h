//seqQueue.h
//顺序循环队列的定义和运算实现

#include "iostream"

//----------------------------------------------------------------------
//定义相关符号
#define MAXLEN 100
//typedef int elementType;
typedef int eleType;
using namespace std;
//----------------------------------------------------------------------
//顺序队列存储结构定义
typedef struct sQueue
{
	eleType data[MAXLEN];
	int front,rear;
}seqQueue;


//----------------------------------------------------------------------
//1. 初始化顺序队列
void initialQueue(seqQueue* Q) //参数号可用传值、引用
{
	Q->front=0;  //空栈
	Q->rear=0;
}
//----------------------------------------------------------------------
//2. 判队空
bool queueEmpty(seqQueue &Q)
{
	if(Q.front==Q.rear)
		return true;  //队空，返回true
	else
		return false;  //队不空
}
//----------------------------------------------------------------------
//3. 判队满
bool queueFull(seqQueue &Q)
{
	if(((Q.rear+1) % MAXLEN)==Q.front)
		return true;  //队满，返回true
	else
		return false;  //不满，返回false
}
//----------------------------------------------------------------------
//4. 取队头元素
bool getFront(seqQueue &Q, eleType &x)
{
	if(queueEmpty(Q))
		return false; //队空
	else
	{
		x=Q.data[(Q.front+1) % MAXLEN];  //front指示的下一个单元才是队头元素
	               //为何要取模呢？
	    return true;
	}
}
//----------------------------------------------------------------------
//5. 入队（插入）
bool enQueue(seqQueue* Q, eleType x)
{
	if(queueFull(*Q))
		return false;    //队列已满
	else
	{
		Q->rear=((Q->rear)+1) % MAXLEN;  //后移rear
		Q->data[Q->rear]=x;  //填入数据x
		                     //2行语句可否并未1行呢？
		return  true;
	}
}
//----------------------------------------------------------------------
//6. 出队（删除）
bool outQueue(seqQueue* Q,eleType &x)
{
	if(queueEmpty(*Q))
		return false;  //空队列，没有元素可供出队！
	else
	{
		Q->front=(Q->front+1) % MAXLEN;
		x=Q->data[Q->front];
		return true;
	}
}

//打印队元素
void printQueue(seqQueue &Q)
{
	int i;
	if(Q.front==Q.rear)
		cout<<"空队列。";
	else
	{
		cout<<"队列元素数："<<(Q.rear-Q.front+MAXLEN)%MAXLEN<<"。"<<endl;
		cout<<"队列元素：";
		i=(Q.front+1)%MAXLEN;   //i指第一个元素
		while(i!=Q.rear)
		{
			cout<<Q.data[i]<<"\t";
			i=(i+1)%MAXLEN;		
		}
		cout<<Q.data[Q.rear]<<"\t";  //打印队尾元素				
	}
	cout<<endl<<endl;
}

//交互循环压入元素
void createSeqQueue(seqQueue &Q)
{		
	eleType x;
	
	cout<<"请输入数据元素(整数，-9999退出):"<<endl;
	cout<<"x=";
	cin>>x;

	while(x!=-9999 && (Q.rear+1)%MAXLEN!=Q.front)   //非结束符，且队未满，循环入队
	{
		Q.rear=(Q.rear+1)%MAXLEN;   //队尾循环后移
		Q.data[Q.rear]=x;

		cout<<"x=";
		cin>>x;
	}
	if((Q.rear+1)%MAXLEN!=Q.front)
		cout<<"队满，不能再进行元素入队。"<<endl;
}

//随机数循环入队开始------------------------------------------------------------------
//随机数创建循环顺序队列
void rndCQueue(seqQueue &Q)
{
	int i;
	int n,m;
	Q.front=0;  //置为空队
	Q.rear=0;

	cout<<"请输入要产生的随机数个数，n=";
	cin>>n;
	
	if(n>=MAXLEN)
	{
		cout<<"您要求产生的随机数个数超出了顺序队列长度"<<MAXLEN-1<<"，创建顺序队列失败。"<<endl;
		return;	
	}	
	cout<<"请输入控制随机数大小参数，比如100以内数，请输入100，m=";
	cin>>m;    
	
	srand((unsigned)time(NULL));	//产生随机数种子
	//srand((unsigned)GetTickCount());	//产生随机数种子
	for(i=0;i<n;i++)                //随机数写入队列
	{
		Q.rear=(Q.rear+1)%MAXLEN;
		Q.data[Q.rear]=rand()%m;
	}
	cout<<endl;
}
//随机数循环入队结束------------------------------------------------------------------
