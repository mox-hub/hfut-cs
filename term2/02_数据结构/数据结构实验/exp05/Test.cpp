//测试的主函数
#include <iostream>
#include "BiTree.h"

int main(int argc, char* argv[])
{	//BiTree BT;
	btNode* T = NULL; //* p;
	btNode* pL, * pR;
	btNode* pParent;
	elementType x;
	int nChoice;
	//elementType InArr[NODENUM];  //InArr[0]不用

	char strLine[NODENUM][3]; //文件数据读入到数组		
	int  nArrLen = 0;           //数组实际长度，即结点数
	int  nR = 0;
	int i = 0;

	do
	{
		//创建菜单
		cout << "****** 二叉树二叉链表结构测试程序 ******" << endl;
		cout << "*  0--退出                             *" << endl;
		cout << "*  1--文件创建二叉树                   *" << endl;
		cout << "*  2--打印二叉树的三种遍历             *" << endl;
		cout << "*  3--中序输出各节点值及层次数         *" << endl;
		cout << "*  4--求二叉树高度（深度）             *" << endl;
		cout << "*  5--求结点数量                       *" << endl;
		cout << "*  6--求叶子节点数量                   *" << endl;
		cout << "*  7--求度为2的节点数量                *" << endl;
		cout << "*  8--求元素x的相关节点值              *" << endl;
		cout << "*  9--求元素x的层次                    *" << endl;
		cout << "*  10--顺序二叉树转二叉树链表          *" << endl;
		cout << "*  11--交换左右子树                    *" << endl;
		cout << "*  12--销毁二叉树                      *" << endl;
		cout << "****************************************" << endl;
		cout << "请选择操作(0-12)：" << endl;
		cin >> nChoice;
		switch (nChoice)
		{
		case 0:  //退出程序
			system("cls");  //清除屏幕
			cout << "<-- 程序退出 -->" << endl;  //选择退出
			break;

		case 1:  //从数据文件创建二叉树
			system("cls");  //清除屏幕
			if (T != NULL)  //防止重复创建，造成内存泄露
			{
				cout << "二叉树已经存在，若需重新创建，请先销毁再创建。" << endl;
				break;
			}
			char strFileName[100];
			cout << "请输入二叉树数据文件名(i.e. bt3.btr)：" << endl;
			cin >> strFileName;  //控制台输入数据文件名			

			nArrLen = 0;  //数组长度
			nR = 0;
			if (!ReadFileToArray(strFileName, strLine, nArrLen))
			{
				cout << "<--从文件创建二叉树失败。-->" << endl;
				break;  //文件打开错误，返回
			}

			if (nArrLen == 0)
			{
				cout << ("<--出错：文件中没有结点数据。-->") << endl; 
				break;
			}

			if (CreateBiTreeFromFile(T, strLine, nArrLen, nR)) //从第1行开始
				cout << "<--二叉树创建成功！-->" << endl;
			else
				cout << "<--从文件创建二叉树失败。-->" << endl;
			break;

		case 2:  //遍历二叉树
			system("cls");  //清除屏幕
			cout << "二叉树遍历结果：" << endl;
			if (isEmpty(T))
			{
				cout << "<--二叉树为空树。-->" << endl;
				break;
			}
			cout << "先序次序：";
			preTraverse(T);   //递归先序遍历
			cout << endl;
			cout << "中序次序：";
			inTraverse(T);    //递归中序遍历
			cout << endl;
			cout << "后序次序：";
			postTraverse(T);  //递归后序遍历
			cout << endl;
			break;

		case 3:	//中序输出各节点值及层次数
			system("cls");  //清除屏幕
			cout << "中序输出各节点值及层次数结果：" << endl;
			nR = 0;
			i = 0;
			exp02(T,nR,i);
			break;
			
		case 4:  //求二叉树高度（深度）
			system("cls");  //清除屏幕
			cout << "二叉树高度（深度）：" << btHeight(T) << endl;//求二叉树深度			
			break;

		case 5:  //求结点数
			system("cls");  //清除屏幕
			nR = 0;
			getNodeNumber(T, nR);     //求总结点数
			cout << "二叉树结点数：" << nR << endl;
			break; 

		case 6: //求叶子结点数
			system("cls");
			nR = 0;
			getLeafNumber(T, nR);
			cout << "二叉树叶子结点数：" << nR << endl;
			break;

		case 7: //求度为2的节点数量
			system("cls");
			nR = 0;
			get2DegreeNumber(T, nR);
			cout << "二叉树度为2结点数：" << nR << endl;
			break;

		case 8: //求元素x的相关节点值
			system("cls");
			cout << "请输入元素x的值：" << endl;
			cin >> x;//输入元素x
			getParent(T, x, pParent);
			if (pParent != NULL)
			{
				cout << "二叉树的父结点：" << pParent->data << endl;
			}
			else
			{
				cout << "不存在父节点！" << endl;
			}
			getSibling(T, x, pL, pR);
			if (pL != NULL)
			{
				cout << "二叉树的左兄弟结点：" << pL->data << endl;
			}
			else
			{
				cout << "不存在左兄弟节点！" << endl;
			}
			if (pR != NULL)
			{
				cout << "二叉树的右兄弟结点：" << pR->data << endl;
			}
			else
			{
				cout << "不存在右兄弟节点！" << endl;
			}
			getChildren(T, x, pL, pR);
			if (pL != NULL)
			{
				cout << "二叉树的左孩子结点：" << pL->data << endl;
			}
			else
			{
				cout << "不存在左孩子节点！" << endl;
			}
			if (pR != NULL)
			{
				cout << "二叉树的右孩子结点：" << pR->data << endl;
			}
			else
			{
				cout << "不存在右孩子节点！" << endl;
			}
			break;

		case 9: //求元素x的层次
			system("cls");
			cout << "请输入元素x的值：" << endl;
			cin >> x;//输入元素x
			nR = 0;
			getNodeLevel(T, x, nR, i);
			if (nR == 0) 
			{
				cout<<"x="<<x<<" 结点不在树上。"<<endl;
			} 
			else
			{
				cout << "元素的层次为：" << nR << endl;	
		    }
			break;

		case 10: //顺序二叉树转二叉树链表
			system("cls");  //清除屏幕
			if (T != NULL)  //防止重复创建，造成内存泄露
			{
				cout << "二叉树已经存在，若需重新创建，请先销毁再创建。" << endl;
				break;
			}
			cout << "请输入二叉树数据文件名(i.e. bt3.btr)：" << endl;
			cin >> strFileName;  //控制台输入数据文件名			

			nArrLen = 0;  //数组长度
			nR = 0;
			if (!ReadFileToArray(strFileName, strLine, nArrLen))
			{
				cout << "<--从文件创建二叉树失败。-->" << endl;
				break;  //文件打开错误，返回
			}

			if (nArrLen == 0)
			{
				cout << ("<--出错：文件中没有结点数据。-->") << endl; 
				break;
			}

			if (CreateBiTreeFromFile(T, strLine, nArrLen, nR)) //从第1行开始
				cout << "<--二叉树创建成功！-->" << endl;
			else
				cout << "<--从文件创建二叉树失败。-->" << endl;
			break;
			
		case 11: //交换左右子树
			system("cls");
			if (isEmpty(T))
			{
				cout << "<--二叉树为空树。-->" << endl;
				break;
			}
			swapTree(T);
			cout << "左右子树交换成功" << endl;
			break;

		case 12:  //销毁二叉树
			system("cls");  //清除屏幕
			if (T != NULL)
				destroyBt(T);  //销毁二叉树			
			cout << "<-- 二叉树已经销毁！-->" << endl;
			break;

		}
		system("pause");
		system("cls");
	} while (nChoice != 0);  //循环显示菜单，nMenu==0退出

	if (T != NULL)
		destroyBt(T);    //销毁二叉树

	return 0;
}
