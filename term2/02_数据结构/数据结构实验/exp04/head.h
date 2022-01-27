#include<stdio.h>
#define MAXLEN 100
typedef struct
{
 	int data[MAXLEN]; // 存放元素
 	int front; //队头指针
 	int rear; //队尾指针
}seqQueue;

//1.初始化队列 
void initQueue(seqQueue Q)
{
	//初始首尾都指向0，便于循环处理 
	Q.front = 0;
	Q.rear = 0;
}
//2.判队空
void queueEmpty(seqQueue Q)
{
	if(Q.front == Q.rear)
		printf("队空！\n");
	else
		printf("队非空！\n"); 
} 

//3.判队满
void queueFull(seqQueue Q)
{
	if((Q.rear+1)%MAXLEN == Q.front)
		printf("队满！\n");
	else
		printf("队非满！\n");	
} 

//4.取队头
void queueFront(seqQueue Q,int x)
{
	if(Q.front == Q.rear)
		printf("队空！\n");
	else
		x = Q.data[(Q.front+1)%MAXLEN];	
} 

//5.入队
void enQueue(seqQueue Q,int x)
{
	if((Q.rear+1)%MAXLEN == Q.front)
		printf("队满！\n");	
	else
	{
		Q.rear = (Q.rear+1)%MAXLEN;
		Q.data[Q.rear];	
	}	
} 
 
//6.出队 
void outQueue(seqQueue Q,int x)
{
	if(Q.front == Q.rear)
		printf("队空！\n");
	else
	{
		Q.front = (Q.front+1)%MAXLEN;
		x = Q.data[Q.front];
	}
}
//7.统计队中数据的个数
int numberOfQueue(seqQueue Q)
{
	int x;
	if(Q.front > Q.rear)
		x = Q.front - Q.rear;
	else
		x = Q.rear - Q.front;
	return x;
} 
//8.自定义
void myQueue()
{
	seqQueue Q;
	initQueue(Q);
	int x;
	while(1)
	{
		printf("请输入数字，0退出：");
		scanf("%d",&x);
		if(x == 0)
			break;
		else if(x%2 == 1)
			enQueue(Q,x);
		else
		{
			outQueue(Q,x);
			printf("队头为%d\n",&x);			
		}
	}
	printf("输入完成！\n");	
}  

