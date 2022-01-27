#include <stdio.h>
#include "AVLTree.h"

int main (void) 
{
	int i;
	int nMenu;

	elementType x;


	btNode* T=NULL;
	btNode* p;
	
    do
	{
		//创建菜单
		cout<<"*********平衡二叉树测试程序*********"<<endl;
		cout<<"*       0--退出                    *"<<endl;
		cout<<"*       1--创建平衡二叉树          *"<<endl;
		cout<<"*       2--销毁平衡二叉树          *"<<endl;
		cout<<"*       3--打印平衡二叉树          *"<<endl;
		cout<<"*----------------------------------*"<<endl;
		cout<<"*       4--二叉排序树递归查找      *"<<endl;			
		cout<<"*       5--插入结点                *"<<endl;
		cout<<"*       6--删除结点                *"<<endl;
		cout<<"************************************"<<endl;
		cout<<"请选择操作(0-8)：";
		cin>>nMenu;
		switch(nMenu)
		{
		case 0:  //退出程序
		
			cout<<"当前选择操作：退出。"<<endl;
			cout<<"<-- 程序退出 -->"<<endl;  //选择退出
			break;
			
		case 1:  //创建平衡二叉树--交互
		
			system("cls");  //清除屏幕
			if(T!=NULL)  //防止重复创建，造成内存泄露
			{
				cout<<"二叉树已经存在，若需重新创建，请先销毁再创建。"<<endl;
				break;
			}
			CreateAVLTree(T);   //交互创建二叉排序
			cout<<"成功创建二叉树。"<<endl;
			break;
			
		case 2:    //销毁平衡二叉树
		
			system("cls");  //清除屏幕
			if (T!=NULL)
				DestroyBt(T);
			cout<<"成功销毁二叉树。"<<endl;
			break;
			
		case 3:    //打印平衡二叉树
		
			system("cls");  //清除屏幕
			if(T==NULL)
			{
				cout<<"<--二叉树为空树。-->"<<endl;
				break;
			}
			cout<<"先序次序：";
			preTraverse(T);   //递归先序遍历
			cout<<endl;
			cout<<"中序次序：";
			inTraverse(T);    //递归中序遍历
			cout<<endl;
			cout<<"后序次序：";
			postTraverse(T);  //递归后序遍历
			cout<<endl;
			cout<<endl;
			break;
			
    	case 4:  //递归查找
			system("cls");  //清除屏幕
			cout<<"请输入待查元素值(整数):"<<endl;
			cout<<"x=";
			cin>>x;

			cout<<"二叉排序树递归查找：";
			p=Search(T,x);
			if(p)
				cout<<"成功，目标结点值为："<<p->data<<"。"<<endl;
			else
				cout<<"失败。"<<endl;
			break;

		case 5:  //二叉排序树递归插入结点
		
			system("cls");  //清除屏幕
			cout<<"请输入插入结点数据(整数，-9999退出):"<<endl;
			cout<<"x=";
			cin>>x;			
			while(x!=-9999 )
			{
				p=new btNode;
				p->data=x;
				p->lChild=NULL;
				p->rChild=NULL;
				
				InsertNode(T,x);

				cout<<"x=";
				cin>>x;
			}
			inTraverse(T);
			cout<<endl<<endl;
			break;
			
		case 6:  //平衡二叉树删除结点
			system("cls");  //清除屏幕
			
			cout<<"请输入待删除结点元素值(整数):"<<endl; 
			cout<<"x=";
			cin>>x;
			if(delNode(T,x))
				cout<<"删除结点成功！"<<endl;
			else
				cout<<"删除结点失败。"<<endl;			
			cout<<endl;

			setBf(T,x);
			break;
			
		default:
			cout<<"功能选择错误，请在0到6之间选择，==>"<<endl;
			break;
		}
		
		system("pause");
		system("cls");
	}while(nMenu != 0);  //循环显示菜单，nMenu==0 退出

    if (T!=NULL)
        DestroyBt(T);    //销毁二叉树
        
	return 0;
}
