#include <stdio.h>

#define MAXLEN 100  //定义最大容量 

typedef int elementType; //定义数据类型

typedef struct sList
{
	elementType list[MAXLEN];
	int listLen;
} seqList;//封装线性表 并进行typedef 

//1.初始化 
void initialList(seqList L)
{
	L.listLen = 0;
}

//2.求表长度 
int length(seqList L)
{
	return L.listLen;
}

//3.按序号取元素 
int getElement(seqList L, int i)
{
	if(i < 1 || i > L.listLen)
	{
		printf("数据出错\n");
		return -1;
	}
		
	else
	{
		return L.list[i-1];
	}		
}

//4.按值查找元素 
int listLocate(seqList L, elementType x)
{
	int i;
	for(i = 0; i < L.listLen; i++)
		if(L.list[i] == x)
			return i+1;
    return 0; 
}

//5.插入元素 
int listInsert(seqList *L, elementType x, int i)
{
	int j;
	if(L->listLen == MAXLEN)
		return 0; 
	else if(i < 1 || i > L->listLen + 1)
		return 1;  
	else
	{
		for(j = L->listLen-1; j >= i-1; j--)
		{
			L->list[j+1]=L->list[j];   //循环后移表元素
		}	
		L->list[i-1] = x;   //插入元素 x
		L->listLen++;     //表长度增1  
		return 2;         //成功插入，返回值2  
	}
}

//6.删除元素 
int listDelete(seqList *L, int i)
{
	int j;
	if(L->listLen <= 0)
		return 0; //空表返回0
	else if(i < 1 || i > L->listLen)
		return 1;  //序号不在有效范围
	else
	{
		for(j = i; j < L->listLen; j++ )
			L->list[j-1]=L->list[j]; //循环前移表元素
		L->listLen--;  //修改表长度
		return 2;  //成功删除，返回值2.
	}
}
