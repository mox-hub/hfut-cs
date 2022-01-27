#include <string.h>
#include <iostream>

#define NODENUM  100       //定义最大结点数
typedef char elementType;  //elementType 定义为字符类型
//typedef int elementType;  //elementType 定义为整数类型

typedef struct biNode
{
	elementType data;
	struct biNode* lChild, * rChild;
}btNode, * BiTree;

using namespace std;

//先序遍历
void preTraverse(btNode* T)
{
	if (T)
	{
		cout << T->data << " ";         //访问根结点。打印当前结点元素值，替代visit(T)函数
		preTraverse(T->lChild);     //先序遍历左子树
		preTraverse(T->rChild);     //先序遍历右子树
	}
}
//中序遍历
void inTraverse(btNode* T)
{
	if (T)
	{
		inTraverse(T->lChild);      //中序遍历左子树
		cout << T->data << " ";         //访问根结点。打印当前结点元素值，替代visit(T)函数
		inTraverse(T->rChild);      //中序遍历右子树
	}
}
//后序遍历
void postTraverse(btNode* T)
{
	if (T)
	{
		postTraverse(T->lChild);      //后序遍历左子树		
		postTraverse(T->rChild);      //后序遍历右子树
		cout << T->data << " ";         //访问根结点。打印当前结点元素值，替代visit(T)函数
	}
}

//按元素值搜素节点
void btSearch(btNode* T, elementType x, btNode*& pR)
{
	//pR--返回的目标结点指针
	if (T)
	{
		if (T->data == x)
			pR = T;
		else
		{
			btSearch(T->lChild, x, pR);  //递归搜索左子树
			btSearch(T->rChild, x, pR);  //递归搜索右子树
		}
	}
}

//求二叉树高度（深度）
int btHeight(btNode* T)
{
	int lh, rh;
	if (!T)
		return 0;
	else
	{
		lh = btHeight(T->lChild);   //求左子树高度
		rh = btHeight(T->rChild);   //求右子树高度
		//return (lh>rh?lh:rh)+1;  //简略写法
		if (lh > rh)
			return lh + 1;
		else
			return rh + 1;
	}
}

//求结点层次数
bool getNodeLevel(btNode* T, elementType x, int& nLevel, int nLevelTemp)
{
	//nLevel 返回目标结点层数
	//nLevelTemp  保存当前结点层数
	if (!T)
	{
		nLevel = 0;
		return false;
	}
	if (T->data == x)
	{
		nLevel = nLevelTemp;
		return true;
	}
	if (getNodeLevel(T->lChild, x, nLevel, nLevelTemp + 1))
	{
		cout << T->data << ", ";
		return true;
	}
	if (getNodeLevel(T->rChild, x, nLevel, nLevelTemp + 1))
	{
		cout << T->data << ", ";
		return true;
	}
	return false;
}

void  exp02(btNode* T,int& nLevel, int nLevelTemp)
{
	if (T)
	{
		exp02(T->lChild,nLevel,nLevelTemp);      //中序遍历左子树
		cout << T->data << " ";         //访问根结点。打印当前结点元素值，替代visit(T)函数
		getNodeLevel(T,T->data,nLevel,nLevelTemp);
		cout << "层次值：" << nLevel << endl; 
		exp02(T->rChild,nLevel,nLevelTemp);      //中序遍历右子树
	}
}
//求双亲结点，返回双亲结点指针
void getParent(btNode* T, elementType x, btNode*& pParent)
{
	if (T->data == x)
	{
		cout << "目标结点为根结点，无父结点。" << endl;;
		return;
	}
	if ((T->lChild && T->lChild->data == x) || (T->rChild && T->rChild->data == x))
	{
		pParent = T;  //取得父节点指针，返回
		return;
	}
	if (T->lChild)
		getParent(T->lChild, x, pParent);  //递归搜索左子树
	if (T->rChild)
		getParent(T->rChild, x, pParent);  //递归搜索右子树
}
//求孩子结点
int getChildren(btNode* T, elementType x, btNode*& pL, btNode*& pR)
{
	//pL,pR分别返回左右孩子结点指针
	btNode* p;
	pL = NULL;
	pR = NULL;
	if (T == NULL)
		return 0;       //空树，返回
	btSearch(T, x, p);      //搜索结点x
	if (p == NULL)
		return 1;       //x不在树上
	if (p->lChild)
	{
		pL = p->lChild;   //返回左孩子指针
	}
	if (p->rChild)
		pR = p->rChild;   //返回右孩子指针
	return 2;
}
//求兄弟结点
int getSibling(btNode* T, elementType x, btNode*& pL, btNode*& pR)
{
	//pL返回左兄弟指针，pR返回右兄弟指针
	btNode* p = NULL;
	pL = NULL;
	pR = NULL;
	if (T == NULL)
		return 0;   //空树
	getParent(T, x, p);
	if (p == NULL)
	{
		if (T->data == x)
			return 1;   //x为根结点
		else
			return 2;   //x不在树上
	}

	if (p->lChild && p->lChild->data == x)
	{
		pR = p->rChild;
		return 3;    //右兄弟可能存在
	}

	if (p->rChild && p->rChild->data == x)
	{
		pL = p->lChild;
		return 4;   //左兄弟可能存在
	}
}

//求树上总结点数
void getNodeNumber(btNode* T, int& num)
{                  //改造先序遍历计数结点数
	if (T)
	{
		num++;    //计数根结点
		getNodeNumber(T->lChild, num);    //累计左子树结点
		getNodeNumber(T->rChild, num);    //累计右子树结点
	}

}

int getNodeNumber1(btNode* T)
{
	if (T == NULL)
		return 0;
	else
		return (1 + getNodeNumber1(T->lChild) + getNodeNumber1(T->rChild));
}
//求叶子结点数
void getLeafNumber(btNode* T, int& num)
{
	if (T)
	{
		if (T->lChild == NULL && T->rChild == NULL)
			num++;
		getLeafNumber(T->lChild, num);
		getLeafNumber(T->rChild, num);
	}
}
//求1度结点数
void get1DegreeNumber(btNode* T, int& num)
{
	if (T)
	{
		if ((T->lChild && T->rChild == NULL) || (T->lChild == NULL && T->rChild))
			num++;
		get1DegreeNumber(T->lChild, num);
		get1DegreeNumber(T->rChild, num);
	}
}
//求2度结点数
void get2DegreeNumber(btNode* T, int& num)
{
	if (T)
	{
		if (T->lChild && T->rChild)
			num++;
		get2DegreeNumber(T->lChild, num);
		get2DegreeNumber(T->rChild, num);
	}
}
//交换左右子树
void swapTree(btNode* T) 
{
	btNode* temp;
	if (T)
	{
		//递归 
		swapTree(T->lChild);
		swapTree(T->rChild);
		//交换部分 
		temp = T->lChild;
		T->lChild = T->rChild;
		T->rChild = temp; 
	}
} 
//判定空树
bool isEmpty(btNode* T)
{
	if (T != NULL)
		return false;
	else
		return true;
}

//销毁二叉树
void destroyBt(btNode*& T)
{
	if (T)
	{
		destroyBt(T->lChild);
		destroyBt(T->rChild);
		delete T;
	}
	T = NULL;
}
//数据文件创建二叉树开始-------------------------------------------------------------------------
//********************** 从数据文件创建二叉树 ***********************//
//* 分两步完成：第一步将数据从文本文件读入数组                      *//
//* 第二步从数组递归创建二叉树                                      *//
//* 两步由2个函数完成                                               *//
//*******************************************************************//

//删除字符串、字符数组左边空格
void strLTrim(char* str)
{
	int i, j;
	int n = 0;
	n = strlen(str) + 1;
	for (i = 0; i < n; i++)
	{
		if (str[i] != ' ')  //找到左起第一个非空格位置
			break;
	}
	//以第一个非空格字符为手字符移动字符串
	for (j = 0; j < n; j++)
	{
		str[j] = str[i];
		i++;
	}
}

//从文本文件数据读入到数组中，同时返回实际结点数量
bool ReadFileToArray(char fileName[], char strLine[NODENUM][3], int& nArrLen)
{
	//读文本文件数据到数组，返回数组及其长度
	FILE* pFile;      //定义二叉树的文件指针
	char str[1000];   //存放读出一行文本的字符串
	char strTemp[10]; //判断是否注释行

	pFile = fopen(fileName, "r");
	if (!pFile)
	{
		cout << "错误：文件" << fileName << "打开失败。" << endl;
		return false;
	}

	while (fgets(str, 1000, pFile) != NULL)  //跳过空行和注释行
	{
		//删除字符串左边空格
		strLTrim(str);
		if (str[0] == '\n')               //空行，继续读取下一行
			continue;

		strncpy(strTemp, str, 2);
		if (strstr(strTemp, "//") != NULL)  //跳过注释行
			continue;
		else                            //非注释行、非空行，跳出循环
			break;
	}
	//循环结束，str中应该已经是二叉树数据标识"BinaryTree"，判断文件格式
	if (strstr(str, "BinaryTree") == NULL)
	{
		printf("错误：打开的文件格式错误！\n");
		fclose(pFile);           //关闭文件
		return false;
	}

	nArrLen = 0;     //数组行号初始化为0
	while (fgets(str, 1000, pFile) != NULL)
	{
		//删除字符串左边空格
		strLTrim(str);
		if (str[0] == '\n')  //空行，继续读取下一行
			continue;

		strncpy(strTemp, str, 2);
		if (strstr(strTemp, "//") != NULL)  //注释行，跳过，继续读取下一行
			continue;

		char* token = strtok(str, " ");   //以空格为分隔符，分割一行数据
		if (token == NULL)  //分割为空串，失败退出
		{
			printf("错误：读取二叉树结点数据失败！\n");
			fclose(pFile); //关闭文件
			return false;
		}

		strLine[nArrLen][0] = *token;  //获取结点数据
		token = strtok(NULL, " ");  //读取下一个子串，即此结点的左子树信息
		if (token == NULL)  //分割为空串，失败退出
		{
			printf("错误：读取二叉树结点数据失败！\n");
			fclose(pFile); //关闭文件
			return false;
		}
		strLine[nArrLen][1] = *token;  //获取此结点的左子树信息数据
		token = strtok(NULL, " ");  //读取下一个子串，即此结点的右子树信息
		if (token == NULL)  //分割为空串，失败退出
		{
			printf("错误：读取二叉树结点数据失败！\n");
			fclose(pFile); //关闭文件
			return false;
		}
		strLine[nArrLen][2] = *token;  //获取此结点的右子树信息数据

		nArrLen++;  //数组行号加1	
	}

	fclose(pFile); //关闭文件
	return true;
}


//从数组创建二叉树--数组中保存的是二叉树的先序序列，及每个结点的子树信息
bool CreateBiTreeFromFile(btNode*& pBT, char strLine[NODENUM][3], int nLen, int& nRow)
{
	//strLine[NODENUM][3]--保存节点数据的二维数组
	//nLen--数组长度，即：节点个数
	//nRow--数组当前行号

	if ((nRow >= nLen) || (nLen == 0))
		return false;  //数据已经处理完毕，或者没有数据，退出

	//根据数组数据递归创建子树
	pBT = new btNode;
	pBT->data = strLine[nRow][0];
	pBT->lChild = NULL;
	pBT->rChild = NULL;

	int nRowNext = nRow;  //保留本次递归的行号	

	if (strLine[nRowNext][1] == '1')  //当前结点有左子树，递归创建左子树，读下一行数据
	{
		nRow++;
		CreateBiTreeFromFile(pBT->lChild, strLine, nLen, nRow);
	}

	if (strLine[nRowNext][2] == '1')  //当前结点有右子树，递归创建右子树，读下一行数组
	{
		nRow++;
		CreateBiTreeFromFile(pBT->rChild, strLine, nLen, nRow);
	}

	return true;
}
