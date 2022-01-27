//排序算法常用结构、参数定义
//部分排序算法
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;
#define MaxLen 200           //定义排序表最大长度

typedef int keyType;         //定义排序关键字类型

typedef struct elementType   //定义排序序列的元素类型
{
	keyType key;             //定义排序关键字
        //元素其它字段，可根据需要进行定义
} elementType;               //排序序列元素类型，这里只有关键字key

int listLen;                 //保存排序表长度
int n=0;

//希尔排序算法---------------------------------------------------------------------------
//希尔排序
void ShellSort(elementType A[])
{
	int dh=listLen/2;   //初始步长
	int i,j;
	while(dh>0)        //步长控制的分组循环
	{
		for(i=dh+1; i<=listLen; i++)   //从第一个元素，右边同组第一个元素开始逐个取元素，进行同组插入排序
		{
			A[0]=A[i];   //待插入元素保存到临时变量
			j=i-dh;      //j指向同组有序区最后一个元素
			while(j>0 && A[j].key>A[0].key)
			{
				A[j+dh]=A[j];   //当前元素比待插元素大，后移一个步长
				j=j-dh;         //j前移一步长，取有序区同组下一个元素			
			}
			A[j+dh]=A[0];       //待插元素存入最终位置，为什么是j+dh		
		}
		dh=dh/2;	            //修改步长为原步长的一半。
	}
}
//希尔排序算法结束-----------------------------------------------------------------------


//创建排序表算法
//打印排序表


//控制台输入创建排序表开始------------------------------------------------------------------
//控制台输入创建排序表
void cSortList(elementType A[])
{
	int i=1;      //排序表元素从数组下标1开始存储，A[0]单元留作他用
	keyType x;    //读取元素关键字值
	
	listLen=0;    //初始化排序表长度

	cout<<"控制台输入创建排序表。请输入元素关键字值（-9999退出）："<<endl;
	cin>>x;
	while(x!=-9999)
	{
		if(i>(MaxLen-1))
		{
			cout<<"排序表空间满，插入元素失败。"<<endl;
			break;		
		}
		A[i].key=x;  //插入新元素
		i++;
		listLen++;

		cin>>x;
	}	
}
//控制台输入创建排序表结束------------------------------------------------------------------

//随机数创建排序表

void srnd(elementType A[])
{

	int i;
	int n,m;
	
	//string str,str1;


	char ss[10];

	listLen=0;
	cout<<"请输入要产生的随机数个数，n=";
	cin>>n;
	
	if(n>MaxLen-1)
	{
		cout<<"您要求产生的随机数个数超出了排序表长度"<<MaxLen-1<<"，创建排序表失败。"<<endl;
		return;
	
	}	
	cout<<"请输入控制随机数大小参数，比如100以内数，请输入100，m=";
	cin>>m;
    
	
	srand((unsigned)time(NULL));	//产生随机数种子
	//srand((unsigned)time(GetTickCount()));	//产生随机数种子
	for(i=1;i<=n;i++)               //随机数写入排序表A[]
		A[i].key=rand()%m;
    listLen=n;                      //表长度为n
	cout<<endl;

//	_mkdir( "..\\sortData" );       //创建排序数据文件存放目录
//	ofstream f;
	
    //str="..\\sortData\\sort"+itoa(n)+".srt";
//	sprintf(ss,"%d",n);
//	str1=ss;
//	str="..\\sortData\\sort";//+str1+".srt";
//	f.open("..\\sortData\\sort1.srt");

//	cout<<str1<<endl;

}



//打印表元素算法开始------------------------------------------------------------------------
void printList(elementType Org[], elementType A[])
{
	int i;
	cout<<"原始表Org[]元素：";
	for(i=1;i<=listLen;i++)
		cout<<Org[i].key<<"\t";
	cout<<endl;

	cout<<"排序表 A[] 元素：";
	for(i=1;i<=listLen;i++)
		cout<<A[i].key<<"\t";
	cout<<endl;
}
//打印表元素算法结束------------------------------------------------------------------------

//快速排序

//分区算法
void partition( elementType A[], int low, int high, int *mid )
{
	A[0]=A[low];          //保存枢轴元素到a[0]，用最低端元素作为分区元素
	while(low<high)
	{                     //从高端寻找一个关键字值比枢轴关键字小的元素，移到A[low]
		while(low<high && A[high].key>=A[0].key)  //A[high].key不小于分区关键字，high下标直接左移
			high--;
		A[low]=A[high];    //高端找到一个比分区关键字小的元素，移到A[low]
		                   //A[low]的数据此前已经存入A[0]，可以把A[high]数据考入A[low]
		                   //执行后，A[high]可以存入新的元素

		                   //从低端寻找一个关键字值比枢轴关键字大的元素，移到A[high]
		while(low<high && A[low].key<=A[0].key)   //A[low].key不大于枢轴关键字，low下标右移
			low++;
		A[high]=A[low];    //低端找到一个比枢轴关键字大的元素，移到A[high]
		                   //原先的A[high]元素已经移走，可以存入A[low]
		                   //执行后，A[low]可以存入新的元素	
	}
	                       //至此，low==high，找到枢轴元素存储位置
	A[low]=A[0];   //或 A[high]=A[0]，A[0]放置到目标位置
	(*mid)=low;    //或 (*mid)=high，取得分界点下标，返回，以继续对子表进行划分	
}

//快速排序算法
void QuickSort( elementType A[], int low, int high )
{
	int mid;
	if(low<high)
	{
		partition( A, low, high, &mid );    //划分
		QuickSort( A, low, mid-1 );         //递归调用对左子表继续快速排序
		QuickSort( A, mid+1, high );        //递归调用对右子表继续快速排序	
	}
}

//堆排序

//假设数组A[]中保存的就是关键字key

//调整以k为根的子树序列使成为堆。
//其中k为子树根，m为子树的结点数，也是最大结点（元素）编号，或编号最大结点的下标
//假设k的左右子树均为堆，即以2k和2k+1为根的左右子树均是堆

void Sift(elementType A[], int k, int m)
{
	int i,j;
	bool finished=false;    //设置未结束标志
	i=k;                    //i为当前子树根结点编号
	j=2*i;                  //j为当前根结点i左孩子结点的编号
	A[0]=A[i];              //A[0]保存当前根结点i的元素值，空出位置存储调整过来的根
	while(j<=m && !finished)
	{
		if(j<m && A[j].key <A[j+1].key )  //j指向当前根结点i的左右孩子中值较大者
			j++;
		if(A[0].key >=A[j].key)             //A[0]值（i结点的值）大于左右孩子的值，已经为大根堆，无需调整，置finished=true
			finished=true;
		else                    //否则，x<A[j]，需将A[j]调整为当前子树的根(堆顶)
		{
			A[i]=A[j];
			i=j;                //x调到j后，可能破坏j为根结点的子树的堆，需要继续调整（往下筛），更新子树根结点i为j
			                    //本来此处应是A[j]=A[0]，i=j，A[0]=A[i]，A[0]值还是原来的值，省略A[0]值的交换操作，提高效率
			                    //只要为x寻找合适的位置即可
			j=2*j;              //i仍为当前子树根结点，j指向i的左孩子。回去循环继续把i子树调整为堆。 
		}
	}
	A[i]=A[0];  //循环结束，i即为A[0]的最终位置，以k为根结点的完全二叉树为堆。
}

//建初堆
//从原始待排序序列创建初堆
void CreateHeap(elementType A[], int n)
{
	//n为结点个数（元素个数）
	int i;
	        //初建堆--由初始序列产生堆（此处为大根堆）
	        //从第n/2结点开始往上筛，直到1号结点（根、堆顶）
	for(i=n/2; i>=1; i--)  
	{
		Sift(A, i, n);  //每次调用此函数，都将以i为根结点的子树调整为堆。
	}
}

//堆排序
//排序结果仍放在数组A[]中
void HeapSort(elementType A[], int n)
{
	int i;


	CreateHeap(A,n);   //初建堆

/*	        //初建堆--由初始序列产生堆（此处为大根堆）
	        //从第n/2结点开始往上筛，直到1号结点（根、堆顶）
	for(i=n/2; i>=1; i--)  
	{
		Sift(A, i, n);  //每次调用此函数，都将以i为根结点的子树调整为堆。
	}

	     //打印初建堆
	cout<<"初建堆后的堆序列："<<endl;
	for(i=1; i<=n;i++)
		cout<<A[i]<<"  ";
	cout<<endl;
*/
	       //由堆序列产生排序序列，此时整棵树（完全二叉树）为堆（此处为大根堆）
	for(i=n;i>=2;i--)
	{
		A[0]=A[i];  //完全二叉树最后一个结点保存到A[0]，空出位置输出根A[1]，即当前子树的根（堆顶） 
		A[i]=A[1];  //输出根，即A[1]保存到排序后的最终位置i。
		A[1]=A[0];  //原第i元素暂作为“根”。
		           
		            
		            //又A[1]=A[0]后可能破坏了当前树的堆属性，需要从根结点1开始重新调整为堆
		            //因为输出根，此时树的结点数为i-1。
		Sift(A,1,i-1); 
	}
} 

//模3排序
void swap (elementType A[],int a,int b)
{
	elementType temp;
	temp = A[a];
	A[a] = A[b];
	A[b] = temp;
}

void SortBy3 (elementType A[],int n)
{
	int a = 0;   
    int b = n - 1;   
    
    while (a < b){   
        while (a < b && A[a].key % 3 == 0)   
            a++;   
        while (a < b && A[b].key % 3 != 0)   
            b--;   
        if (a != b){   
            swap(A,a,b);   
            a++;   
            b--;   
        }   
    }   
    b = n - 1;   
    while (a < b){   
        while (a < b && A[a].key % 3 == 1)   
            a++;   
        while (a < b && A[b].key % 3 != 1)   
            b--;   
        if (a != b){   
            swap(A, a, b);   
            a++;   
            b--;   
        }   
    } 	
} 

