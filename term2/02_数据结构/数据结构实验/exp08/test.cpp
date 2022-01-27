#include <iostream>
#include "head.h"

using namespace std;

int main(int argc, char* argv[])
{
	int nMenu;  //菜单控制
	int dh;
	int i;

    elementType Org[MaxLen];  //存放未排序的原始序列
	elementType A[MaxLen];    //用以排序 
	elementType T[MaxLen];    //存放二路归并后的序列
	elementType x;
	int arrLen;   //保存数组A[]中元素个数

	do
	{
		//创建菜单
		cout<<"--------排序算法主菜单-----------"<<endl;
		cout<<"|  0--退出                      |"<<endl;
		cout<<"|  1--控制台创建排序表          |"<<endl;
		cout<<"|  2--随机数创建排序表          |"<<endl;		
		cout<<"|  3--打印排序表                |"<<endl;
		cout<<"*-------------------------------*"<<endl;
		cout<<"*  4--希尔排序算法              *"<<endl;
		cout<<"*  5--快速排序                  *"<<endl;
		cout<<"*  6--堆排序                    *"<<endl;
		cout<<"*  7--mod3排序                  *"<<endl;
		cout<<"*-------------------------------*"<<endl;
		cout<<"请选择操作(0-7)：";
		cin>>nMenu;
		switch(nMenu)
		{
		case 0:  //退出程序
			cout<<"当前选择操作：退出。"<<endl;
			cout<<"<-- 程序退出 -->"<<endl;  //选择退出
			break;
		case 1:  //控制台创建排序表
			cout<<"当前选择操作：控制台创建排序表。"<<endl;
			
			cSortList(Org);  //创建序列，保存到Org[]数组

			memcpy(A,Org, sizeof(Org));

			printList(Org,A);
			
			if(listLen==0)
				cout<<"<-- 排序表创建失败！ -->"<<endl;
			else
				cout<<"<-- 排序表创建成功！ -->"<<endl;
			
			break;
		case 2:    //随机数创建排序表
			cout<<"当前选择操作：随机数创建排序表。"<<endl;
			
			srnd(Org);  //创建序列，保存到Org[]数组

			memcpy(A,Org, sizeof(Org));

			printList(Org,A);
			
			if(listLen==0)
				cout<<"<-- 排序表创建失败！ -->"<<endl;
			else
				cout<<"<-- 排序表创建成功！ -->"<<endl;			
			break;
		case 3:
			if(listLen==0)
			{
				cout<<endl<<"错误：排序表尚未创建，不能打印。"<<endl<<endl;
				break;
			}

			printList(Org,A);  //打印原始表和排序后表

			break;
					
		case 4:  //希尔排序
			if(listLen==0)
			{
				cout<<endl<<"错误：排序表尚未创建。"<<endl<<endl;
				break;
			}
			memcpy(A,Org, sizeof(Org));   //原始表数据复制排序表

			ShellSort(A);                 //希尔排序算法

			printList(Org,A);             //打印原始表和排序后表
			cout<<endl<<"希尔排序完成。"<<endl<<endl;

			break;					
							
		case 5:  //快速排序
			if(listLen==0)
			{
				cout<<endl<<"错误：排序表尚未创建。"<<endl<<endl;
				break;
			}
			memcpy(A,Org, sizeof(Org));   //原始表数据复制排序表

			QuickSort (A, 1, listLen );   //快速排序算法

			printList(Org,A);             //打印原始表和排序后表
			cout<<endl<<"快速排序完成。"<<endl<<endl;

			break;	
							
		case 6:  //堆排序
			if(listLen==0)
			{
				cout<<endl<<"错误：排序表尚未创建。"<<endl<<endl;
				break;
			}
			memcpy(A,Org, sizeof(Org));   //原始表数据复制排序表

			HeapSort(A,listLen);          //堆排序算法

			printList(Org,A);             //打印原始表和排序后表
			cout<<endl<<"堆排序完成。"<<endl<<endl;

			break;			
	
		case 7:  //mod3排序
			if(listLen==0)
			{
				cout<<endl<<"错误：排序表尚未创建。"<<endl<<endl;
				break;
			}
			
			SortBy3(A,listLen);
			printList(Org,A);              //打印原始表和排序后表
			cout<<endl<<"计数排序完成。"<<endl<<endl;

			break;							
		default:
			cout<<"功能选择错误，请在0到14之间选择，==>"<<endl;
			break;
		}
	
		system("pause");
		system("cls"); 
	}while(nMenu!=0);  //循环显示菜单，nMenu==0 退出
	
	return 0;
}
