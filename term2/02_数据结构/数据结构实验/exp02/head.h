#include<stdio.h>
#include<stdlib.h>

//节点的定义 
typedef struct slNode
{
	int data;//数据域 
	struct slNode* next;//指针域 ,自身引用 
}node;

//1.初始化单链表 
node* initialList()
{
	node* p;
	p =(node*)malloc(sizeof(node));//动态申请内存，作为头结点 
	p->next = NULL;
	return p;	//返回链表 
} 

//2.计算链表长度 
int listLength(node *L);
//3.获得指定位置的元素 
node* getElement(node* L,int i);

//4.获得指定元素的位置 
node* locate(node* L,int x);

//5.插入新节点
void listInsert(node* L ,int i,int x);

//6.删除节点
void listDelete(node* L,int i);

//7.链表的销毁
void destroyList(node* L);

//8.尾插法建表
void createList(node *L);

//9.头插法建表
void createListH(node* L); 
 
//输出
void printList(node* L)
{
	node* q;
	q = L->next;
	printf("{");
	while(q != NULL)
	{
		printf("%d ",q->data);
		q = q->next;
	}
	printf("}\n");
} 
