#include"seqList.h"
#include<String.h> 
//<1>求顺序表中第i个元素（函数），若不存在，报错。
//在seqList.h中有具体实现 
//int getElement(seqList *L, int i, elementType &x); 

//<2>在第i个元素位置插入值为x的元素。
//在seqList.h中有具体实现 
//int listInsert( seqList *L, int i, elementType x); 

//<3>删除顺序表中第i个元素结点。
//在seqList.h中有具体实现
//int listDelete(seqList *L, int i);

//<4>在一个递增有序的顺序表L中插入一个值为x的元素，并保持其递增有序特性。
int orderlistInsert(seqList *L, int x) 
{
	int i = L->listLen - 1;
	
	if (i >= MAXLEN - 1) 
	{
		printf("顺序表已满");
		return 0;	
	} 
	else
	{
		while(i >= 0 && L->list[i] > x)
		{	 
			L->list[i+1]=L->list[i];
			 i--; 
		}
		L->list[i+1]=x;
		L->listLen++;
		return 1;
	}
} 

//<5>将存储整数的顺序表Ｌ，按奇数、偶数拆分为2个新表。
//然后将原表和新表元素同时输出到屏幕上，以便对照求解结果

int DoubleList(seqList *L,seqList *A,seqList *B)
{
	int i;
	if(L->listLen <= 0) 
	{
		printf("不合法的表值"); 
	} 
	for (i = 0;i < L->listLen; i++)
	{
		if(L->list[i] % 2 == 1){
			A->list[A->listLen] = L->list[i];
			A->listLen++;
		} 	
		else
		{
			B->list[B->listLen] = L->list[i];
			B->listLen++;
		}
	}
	return 1;	
}

//<6>求两个递增有序顺序表L1和L2中的公共元素
//，放入新的顺序表L3中

int publicList(seqList *A,seqList *B,seqList *C)
{
	int ai = 0,bi = 0;
	int temp;
	
	for (ai = 0;ai < A->listLen; ai++)
	{
		temp = 0;
		for (bi = temp;bi < B->listLen;bi++)
		{
			if (A->list[ai] == B->list[bi])
			{
				C->list[C->listLen - 1] = A->list[ai];
				C->listLen++; 
				temp = bi; 
			}
			else if(A->list[ai] < B->list[bi])
			{
				temp = bi;
				break;
			}
		}	
	}
	return 1;
}

//<7>删除递增有序顺序表中的重复元素，
//并统计移动元素次数，要求时间性能最好

int deleteList(seqList *L) 
{
	int i = 0;
	int time = 0;

	if (L->listLen <= 0 || L == NULL)
	{
		return 0;
	}
	for (i = L->listLen;i >= 0;i--)
	{
		if(L->list[i] == L->list[i-1])
		{
			listDelete(L,i);
			time += L->listLen - i;
		}
	}

	return time; 
}

//自定义函数 
//插入初始化顺序表
void Initial(seqList *A,int *A1,int n) 
{
	int i = 0;
	A->listLen = n;
	if(A->listLen <= 0 || A->listLen > MAXLEN)
		printf("数据出错！");
	for(i = 0;i < A->listLen; i++)
	{
		A->list[i] = A1[i];
	} 	
} 

//删除链表
void Delete(seqList *A) 
{
	int i = 0;
	while(A->listLen != 0) 
	{
		listDelete(A,i);
	}
	
}
//循环输出顺序表
void printList(seqList *L) 
{
	int i;
	printf("{");
	for (i = 0;i < L->listLen;i++) 
	{
		printf("%d  ",L->list[i]);
	}
	printf("}\n"); 
	
}  


//测试函数 
int main (void) 
{
	int i = 0;
	int element;
	int result;	
	seqList A, B, C;
	
	initialList(A);
	initialList(B);
	initialList(C);


//<1>求顺序表中第i个元素（函数），若不存在，报错。
//	实验测试数据基本要求：
//	第一组数据：顺序表长度n≥10，i分别为5，n，0，n+1，n+2
//	第二组数据：顺序表长度n=0，i分别为0，2

	int T1[] = {0,1,2,3,4,5,6,7,8,9};
	int T2[] = {0};
	int n = sizeof(T1) / 4;
	Initial(&A,T1,n);
	
	printList(&A);
	
	printf("测试1内容：查找第i位元素\n"); 
	element = getElement(A, 5);
	printf("element = %d\n",element);
	
	element = getElement(A, n);
	printf("element = %d\n",element);
	
	element = getElement(A, 0);
	
	element = getElement(A, n+1);
		 
	element = getElement(A, n+2);
	
	A.listLen = 0;
	element = getElement(A, 0);
	
	element = getElement(A, 2);

//<2>在第i个元素位置插入值为x的元素。
//实验测试数据基本要求：
//第一组数据：顺序表长度n≥10，x=100,  i分别为5,n,n+1,0,1,n+2
//第二组数据：顺序表长度n=0，x=100，i=5
	int x = 100;
	Initial(&A,T1,n);
	//使用测试1的顺序表A实现测试2 
	listInsert(&A,x,5); 
	
	printf("\n\n");
	printf("测试2内容：第i个元素位置插入值为x的元素\n");
	 
	printList(&A);
	
	Initial(&A,T1,n);
	listInsert(&A,x,n); 
	printList(&A);
	
	Initial(&A,T1,n);
	listInsert(&A,x,n+1); 
	printList(&A);
	
	Initial(&A,T1,n);
	listInsert(&A,x,0); 
	printList(&A);
	
	Initial(&A,T1,n);
	listInsert(&A,x,1); 
	printList(&A);	
	
	Initial(&A,T1,n);
	listInsert(&A,x,n+2); 
	printList(&A);
	
	
//<3>删除顺序表中第i个元素结点
//实验测试数据基本要求
//第一组数据：顺序表长度n≥10，i分别为5,n,1,n+1,0 
//第二组数据：顺序表长度n=0， i=5

	printf("\n\n");
	printf("测试3内容：删除顺序表中第i个元素结点\n");
//i = 5
	Initial(&A,T1,n);
	printf("删除前：");
	printList(&A);
	listDelete(&A,5);
	printf("删除后：");
	printList(&A); 
	printf("\n");
//i = n
	Initial(&A,T1,n);
	printf("删除前：");
	printList(&A);
	listDelete(&A,n);
	printf("删除后：");
	printList(&A);
	printf("\n");
//i = 1
	Initial(&A,T1,n);
	printf("删除前：");
	printList(&A);
	listDelete(&A,1);
	printf("删除后：");
	printList(&A);
	printf("\n");
//i = n + 1
	Initial(&A,T1,n);
	printf("删除前：");
	printList(&A);
	listDelete(&A,n+1);
	printf("删除后：");
	printList(&A);
	printf("\n");
//i = 0
	Initial(&A,T1,n);
	printf("删除前：");
	printList(&A);
	listDelete(&A,0);
	printf("删除后：");
	printList(&A);
	printf("\n");
//n = 0, i = 5
	Initial(&A,T2,1);
	printf("删除前：");
	printList(&A);
	listDelete(&A,5);
	printf("删除后：");
	printList(&A);


//<4>在一个递增有序的顺序表L中插入一个值为x的元素，并保持其递增有序特性。
//实验测试数据基本要求：
//顺序表元素为 （10,20,30,40,50,60,70,80,90,100）,
//x分别为25，85，110和8

	printf("\n\n");
	printf("测试4内容：顺序表L中插入一个值为x的元素\n");
	
	int T3[] = {10,20,30,40,50,60,70,80,90,100};
	Initial(&A,T3,n);
	printList(&A); 
	
	orderlistInsert(&A,25);
	printList(&A);
	
	orderlistInsert(&A,85);
	printList(&A);
	
	orderlistInsert(&A,110);
	printList(&A);
	
	orderlistInsert(&A,8); 
	printList(&A);

//<5>将存储整数的顺序表Ｌ，按奇数、偶数拆分为2个新表。然后将原表和新表元素同时输出到屏幕上，以便对照求解结果。
//实验测试数据基本要求：
//第一组数据：顺序表元素为 （1,2,3,4,5,6,7,8,9,10,20,30,40,50,60）
//第二组数据：顺序表元素为 （10,20,30,40,50,60,70,80,90,100）

	printf("\n\n");
	printf("测试5内容：奇数、偶数拆分为2个新表\n");
	int T4[] = {10,20,30,40,50,60,70,80,90,100};
	Initial(&A,T4,10);
	Initial(&B,T2,1);
	Initial(&C,T2,1);	
	printList(&A);
	DoubleList(&A,&B,&C);
	printf("拆分后：\n");
	printList(&B);
	printList(&C); 
	printf("\n"); 
	
	int T5[] = {1,2,3,4,5,6,7,8,9,10,20,30,40,50,60};
	Initial(&A,T5,15);
	Initial(&B,T2,1);
	Initial(&C,T2,1);
	printList(&A);
	DoubleList(&A,&B,&C);
	printf("拆分后：\n");
	printList(&B);
	printList(&C);

//<6>求两个递增有序顺序表L1和L2中的公共元素，放入新的顺序表L3中。
//实验测试数据基本要求： 
//第一组
//第一个顺序表元素为 （1，3，6，10，15，16，17，18，19，20）
//第二个顺序表元素为 （1，2，3，4，5，6，7，8，9，10，18，20，30）
//第二组
//第一个顺序表元素为 （1，3，6，10，15，16，17，18，19，20）
//第二个顺序表元素为 （2，4，5，7，8，9，12，22）
//第三组
//第一个顺序表元素为 （）
//第二个顺序表元素为 （1，2，3，4，5，6，7，8，9，10）
	printf("\n\n");
	printf("测试6内容：顺序表L1和L2中的公共元素\n");
	int T6[] = {1,3,6,10,15,16,17,18,19,20};//10
	int T7[] = {1,2,3,4,5,6,7,8,9,10,18,20,30};//13
	int T8[] = {1,3,6,10,15,16,17,18,19,20};//10
	int T9[] = {2,4,5,7,8,9,12,22};//8
	int T10[] = {1,2,3,4,5,6,7,8,9,10};//10
	
	Initial(&A,T6,10);
	Initial(&B,T7,13);
	Initial(&C,T2,1);
	printList(&A);
	printList(&B); 
	publicList(&A,&B,&C);
	printList(&C);
	printf("\n");	
	
	Initial(&A,T8,10);
	Initial(&B,T9,8);
	Initial(&C,T2,1);
	printList(&A);
	printList(&B); 
	publicList(&A,&B,&C);
	printList(&C);
	printf("\n");
	
	Initial(&A,T10,10);
	Initial(&B,T2,1);
	Initial(&C,T2,1);
	printList(&A);
	printList(&B); 
	publicList(&A,&B,&C);
	printList(&C);

//<7>删除递增有序顺序表中的重复元素，并统计移动元素次数，要求时间性能最好。
//实验测试数据基本要求：
//第一组数据：顺序表元素为 （1,2,3,4,5,6,7,8,9）
//第二组数据：顺序表元素为 （1,1,2,2,2,3,4,5,5,5,6,6,7,7,8,8,9）
//第三组数据：顺序表元素为 （1,2,3,4,5,5,6,7,8,8,9,9,9,9,9） 
	printf("\n\n");
	printf("测试7内容：\n");
	
	int time = 0;
	int T11[] = {1,2,3,4,5,6,7,8,9};//9
	int T12[] = {1,1,2,2,2,3,4,5,5,5,6,6,7,7,8,8,9};//17
	int T13[] = {1,2,3,4,5,5,6,7,8,8,9,9,9,9,9};//15

	Initial(&A,T11,9);
	printList(&A);
	time = deleteList(&A);
	printList(&A);
	printf("time = %d", time);
	printf("\n");
	
	Initial(&A,T12,17);
	printList(&A);
	time = deleteList(&A);
	printList(&A);
	printf("time = %d", time);
	printf("\n");
	
	Initial(&A,T13,15);
	printList(&A);
	time = deleteList(&A);
	printList(&A);
	printf("time = %d", time);
	printf("\n");
	
	return 0;
}
