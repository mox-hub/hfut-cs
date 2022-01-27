# include <iostream>
# include "createTree.h"

using namespace std;

//销毁孩子兄弟链表表示的树（森林）
void destroyTree(csNode *&T)
{
	if(T)
	{
		destroyTree(T->firstChild);
		destroyTree(T->nextSibling);
		delete T;
	}
	T=NULL;
}
//求叶子结点数
int leafNum(csNode *T)
{
	if(T==NULL)
		return 0;
	else if(T->firstChild==NULL)  //T为叶子结点
		return 1+leafNum(T->nextSibling);
	else
		return leafNum(T->firstChild)+leafNum(T->nextSibling);
}

//求结点总数
int nodeNum(csNode *T)
{
	if(T==NULL)
		return 0;
	else
		return 1+nodeNum(T->firstChild)+nodeNum(T->nextSibling);
}

//求树（森林）高度（深度）
int height(csNode *T)
{
	int h,h1;
	csNode *p;
	if(T==NULL)
		return 0;
	else
	{		
		p=T->firstChild;
		h=height(p);
		if(p)
			p=p->nextSibling;
		while(p)
		{
			h1=height(p);
			if(h<h1)
				h=h1;
			p=p->nextSibling;		
		}
		return 1+h;	
	}
}

//广义表形式输出
void outGList(csNode *T)
{
	if(T)
	{
		cout<<T->data;   //访问根结点
		if(T->firstChild)
		{
			cout<<"(";
			outGList(T->firstChild);
		}
		if(T->nextSibling)     
		{
			cout<<",";
			outGList(T->nextSibling);	
		}
		else
			cout<<")";			    
	}
}

//先序输出结点及层次号
void outPreOrder(csNode *T, int level)
{
	if(T)
	{
		cout<<"("<<T->data<<","<<level<<") ";   //访问根结点
		outPreOrder(T->firstChild,level+1);     //T的孩子层次为T的层次加1
		outPreOrder(T->nextSibling,level);	    //T的兄弟层次与T相同
	}
}


int main(void)
{
    //准备的代码
    int nChoice;	//选择判断的控制
    pTree T1;	//双亲表示的树或森林
	csNode *T;	//孩子兄弟链表表示的树
	int i = 0;
	T1.n = 0;
	T = NULL;
	int nR = 0;

    do {
        	//创建菜单
		cout << "******      树与森林测试程序      ******" << endl;
		cout << "*  0--退出                             *" << endl;
		cout << "*  1--文件创建树或森林                 *" << endl;
		cout << "*  2--打印森林的三种遍历               *" << endl;
		cout << "*  3--求森林的高度                     *" << endl;
		cout << "*  4--求森林的节点总数                 *" << endl;
		cout << "*  5--求森林叶子节点数                 *" << endl;
		cout << "*  6--求森林的度                       *" << endl;
		cout << "*  7--先序输出节点值及其层次号         *" << endl;
		cout << "*  8--输出广义表表示的树               *" << endl;
		cout << "*  9--销毁树（森林）                   *" << endl;
		cout << "****************************************" << endl;
		cout << "请选择操作(0-8)：" << endl;
		cin >> nChoice;
        switch (nChoice)
		{
		case 0:  //退出程序
			system("cls");  //清除屏幕
			cout << "<-- 程序退出 -->" << endl;  //选择退出
			break;
        
        case 1:  //文件创建树或森林
        	system("cls");  //清除屏幕
			if (T != NULL)  //防止重复创建，造成内存泄露
			{
				cout << "森林已经存在，若需重新创建，请先销毁再创建。" << endl;
				break;
			}
			char strFileName[100];
			cout << "请输入森林数据文件名(F14.tre)：" << endl;
			cin >> strFileName;  //控制台输入数据文件名			 

			if (CreateTreeFromFile(strFileName,T1))
			{
				createCsTree(T,T1);
				cout << "<-- 树或森林创建成功 -->" << endl;	
			}
			else
				cout << "<-- 树或森林创建失败 -->" << endl;
        	break; 

        case 2:  //打印森林的三种遍历
        	system("cls");  //清除屏幕
			cout<<"先序次序：";
			preTraverse(T1);
			cout << endl;
			cout<<"后序次序：";
			postTraverse(T1);
			cout << endl;
			cout<<"层次次序：";
//			hieOrder(T);
			cout<<endl;
			break;

		case 3:  //求森林的高度
			system("cls");  //清除屏幕
			nR = height(T);
			cout << "树或森林的高度：" << nR << endl;
			break;

		case 4:  //求森林的节点总数
			system("cls");  //清除屏幕
			nR = nodeNum(T);
			cout << "树或森林的结点数：" << nR << endl;
			break;

		case 5:  //求森林叶子节点数
			system("cls");  //清除屏幕
			nR = leafNum(T);
			cout << "树或森林的叶子结点数：" << nR << endl;
			break;

		case 6:  //求森林的度
			system("cls");  //清除屏幕
			nR = 0;
			nR = height(T);
			cout << "树（森林）的度：" << nR << endl;
			break;

		case 7:  //先序输出节点值及其层次号
			system("cls");  //清除屏幕
			if(T==NULL)
			{
				cout<<"<--树（森林）为空。-->"<<endl;
				break;
			}
			cout<<"<--树（森林）先序序列及层次："<<endl;
			outPreOrder(T,1);    //先序输出结点值及层次
			cout<<endl;
			outGList(T);
			cout<<endl;
			break;

		case 8:  //输出广义表表示的树
			system("cls");  //清除屏幕
			if(T==NULL)
			{
				cout<<"<--树（森林）为空。-->"<<endl;
				break;
			}
			cout<<"树（森林）广义表形式输出："<<endl;
			cout<<"(";       //为与最后一个括号配对
			outGList(T);     //广义表形式输出
			cout<<endl;
			break;

		case 9:  //销毁树（森林）
			system("cls");  //清除屏幕
			if (T!=NULL)
				destroyTree(T);  //销毁二叉树			
			cout<<"<-- 树（森林）已经销毁 -->"<< endl;
			break;
        }

		system("pause");
		system("cls");
    } while (nChoice != 0);  //循环显示菜单，nMenu==0退出

    return 0;
}

int search(int k,int arr[])
{
	int x = k % 11;
	int i;
	for(i = x; (i-1)%x == 0; i++)
	{
		if(k == arr[i])
			return i;
	}
	 return -1;
}
