#include"head.h"

// <1>尾插法创建单链表，打印创建结果。
void createList(node *&L)
{
	int x;
	node* u;
	node* r;
	L = (node*)malloc(sizeof(node));
	L->next = NULL;
	r = L;
	printf("请输入整数（输入-1结束）：\n");
	scanf("%d",&x);
	while(x != -1)
	{
		u = (node*)malloc(sizeof(node));;
		u->data = x;
		u->next = NULL;
		r->next = u;
		r = u;
		scanf("%d",&x);
	}
	printf("尾插法建表成功！\n");	
	u = L;
	printList(u);
} 

// <2>头插法创建单链表，打印创建结果。 
 void createListH(node* L)
{
	node* u;
	int x;
	L = (node*)malloc(sizeof(node));
	L->next = NULL;
	printf("请输入整形数据（输入-1退出）：\n");
	scanf("%d",&x);
	while(x != -1)
	{
		u = (node*)malloc(sizeof(node));
		u->data = x;
		u->next = L->next;
		L->next = NULL;
		scanf("%d",&x);
	}
	printf("头插法建表成功！\n");
} 

// <3>销毁单链表。
 void destroyList(node* L)
{
	node *p,*u;
	p = L;
	while(p)
	{
		u = p->next;
		p = NULL;
		p = u;	
	}	
	L = NULL;
} 

// <4>求链表长度。
 int listLength(node *L)
{
	int len = 0;//设计变量保存长度
	node* p = L->next;//接管头结点 
	while(p != NULL)
	{
		len++;
		p = p->next;	
	} 
	
	return len;//返回计数值 
}

// <5>求单链表中第i个元素（函数），若不存在，报错。
 node* getElement(node* L,int i)
{
	node* p = L->next;
	int j = 1;
	while(j!=i && p!=NULL)
	{//循环到下一个结点 
		p = p->next;
		j++;
	}
	
	return p; //当j == i时，p为目标节点
//		否则返回p== null，序号超出范围 
}
// <6>在第i个结点前插入值为x的结点。
void listInsert(node* L ,int i,int x) 
{
	
	node* p = L;
	node* s;
	int k = 0;
	while(k != i-1 && p != NULL)
	{
		p = p->next;
		k++;
	}
	if(p == NULL)
		printf("Error!\n"); 
	else
	{
		s = (node*)malloc(sizeof(node)); //创建一个新节点
		s->data = x;
		s->next = p->next;
		p->next = s; 
	}
} 
// <7>链表中查找元素值为x的结点，成功返回结点指针，失败报错。
 node* locate(node* L,int x)
{
	node*p = L->next;
	while(p != NULL)
	{
		if(p->data == x)
		{
			return p;//查找成功，返回目标节点指针 
		}
		else
		{
			p = p->next;//指针后移到下一个节点 
		}
	} 
	return NULL;// 查找失败，返回 空指针 
} 
// <8>删除单链表中第i个元素结点。
 void listDelete(node* L,int i)
{
	node* u;
	node* p = L;
	int k = 0;
	while(k != i-1 && p != NULL)
	{//搜索ai-1节点 
		p = p->next;
		k++;
	}
	if(p == NULL || p->next == NULL)
	{//删除的位置i超出范围，删除失败 
		printf("节点不存在\n"); 
	}
	else//此时p指向ai-1 
	{
		u = p->next;//u指向ai 
		p->next = u->next;//ai-1 的next指向ai+1 
		u = NULL;//删除节点ai 
        printf("删除成功！\n");	//删除成功 
	}
}
// <9>在一个递增有序的单链表L中插入一个值为x的元素，并保持其递增有序特性。
 void orderInsert(node* L, int x)
 {
 	node *p = L;
 	int i = 1;
 	while(p != NULL)
 	{
 		if(p->data < x)
		 {
		 	p = p->next;
 			i++;	
		 }	
 		else
		{
 			listInsert(L,i,x);
			break;	
		}
	}
 }
// <10>将单链表Ｌ中的奇数项和偶数项结点分解开（元素值为奇数、偶数），
//分别放入新的单链表中，然后原表和新表元素同时输出到屏幕上，以便对照求解结果。
void doubleList(node* L,node* A,node* B)
{
	node* p = L;
	while(p != NULL)
	{//进行拆分 
		if(p->data % 2 == 1)
			orderInsert(A,p->data);
		else 
			orderInsert(B,p->data);
		p = p->next;	
	} 	
//	开始输入
	printList(L);
	printList(A);
	printList(B);	

} 
// <11>求两个递增有序单链表L1和L2中的公共元素，放入新的单链表L3中
 void publicList(node* L1,node* L2,node* L3)
 {
	node* p = L1;
	node* q = L2;
	int i = 1;
	while(p != NULL)
	{
		while(q != NULL)
		{
			if(p->data == q->data)
			{
				listInsert(L3,i,p->data);
				i++;
			}
			q = q->next;
		}
		p = p->next;
	}
 }
// <12>删除递增有序单链表中的重复元素，要求时间性能最好。
 void Delete(node *L)
 {
	node* p;
	p = L;
	int i = 0;
	while(p != NULL)
	{
		if(p->data == p->next->data)
		{
			listDelete(p,i);
		}
		p = p->next;
		i++;
	}
 }
// <13>递增有序单链表L1、L2，不申请新结点，利用原表结点对2表进行合并，
// 并使得合并后成为一个集合，合并后用L1的头结点作为头结点，删除L2的头结点，要求时间性能最好。
 node* mergeList(node* L1,node* L2)
 {
 	node *p = L1;
 	node *q = L2;
 	while(p->next != NULL)
 		p = p->next;
 	p->next = q;
 	q = NULL;
 	free(L2);
 	
 	return p;
 }
 
 int main(void)
{//测试
	node *L = NULL;
	createList(L);
	printList(L);
	return 0;	
} 
 
 
 
 
