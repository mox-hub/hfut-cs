//平衡二叉树定义和运算实现
#include <iostream> 

using namespace std;

typedef int elementType;


typedef struct avlNode
{
	elementType data;
	int bf;
	struct avlNode *lChild,*rChild;
}btNode, *AvlTree;


//LL型调整
btNode* LL(btNode* A, btNode* B)  //返回调整后的子树根结点B
{
	A->lChild=B->rChild;   //B的右子树调整为A的左子树
	B->rChild=A;           //A为B的右孩子 
	A->bf=0;               //A的平衡因子为0
	B->bf=0;               //B的平衡因子为0

	return B;              //B为调整后的根结点
}

//LR型调整
btNode* LR(btNode* A, btNode* B)  //返回调整后的子树根结点指针
{
	btNode* C=B->rChild;   //C作为B的右子树
	A->lChild=C->rChild;   //C的右子树调为A的左子树
	B->rChild=C->lChild;   //C的左子树调为B的右子树
	C->lChild=B;           //B调为C的左子树
	C->rChild=A;           //A调为C的右子树
	   //更新平衡因子
	   //根据新结点插入到C的不同位置，对调整后A、B、C的平衡因子进行调整
	switch(C->bf)
	{
	case 1:  //新结点插入到了C的左子树上
		A->bf=-1;
		B->bf=0;
		break;
	case 0:  //最简单的LR型，即B的左子树空，新插入结点为C本身。
		A->bf=0;
		B->bf=0;
		break;
	case -1:  //新结点插入到了C的右子树上
		A->bf=0;
		B->bf=1;
		break;
	}
	C->bf=0;  //调整C的平衡因子

	return C;  //C为调整后的根结点
}


//RR型调整
btNode* RR(btNode* A, btNode* B)  //返回调整后的子树根结点指针
{
	A->rChild=B->lChild;   //B的左子树调整为A的右子树
	B->lChild=A;           //A为B的左孩子
	A->bf=0;
	B->bf=0;

	return B;              //B为调整后的根结点
}
//RL型调整
btNode* RL(btNode* A, btNode* B)
{
	btNode* C=B->lChild;  //C作为B的左子树
	A->rChild=C->lChild;  //C的左子树调为A的右子树
	B->lChild=C->rChild;  //C的右子树调为B的左子树
	C->lChild=A;          //A调为C的左孩子
	C->rChild=B;          //B调为C的右孩子
	    //更新平衡因子
	    //根据新结点插入到C的不同位置，对调整后A、B、C的平衡因子进行调整
	switch(C->bf)
	{
	case 1:  //新结点插入到了C的左子树
		A->bf=0;
		B->bf=-1;
		break;
	case 0:  //最简单的RL型，即B的右子树为空，新插入结点为C本身
		A->bf=0;
		B->bf=0;
		break;
	case -1:  //新结点插入到了C的右子树上
		A->bf=1;
		B->bf=0;
		break;
	}
	C->bf=0;  //调整C的平衡因子

	return C;  //C为调整后的根结点
}

int InsertNode(btNode *&T, elementType x)
{
	btNode *p,*s;
	btNode *A,*Af,*B;
	int d;
	if(T==NULL)   //空树，插入为根结点
	{
		T=new btNode;
		T->data=x;
		T->lChild=NULL;
		T->rChild=NULL;
		T->bf=0;
		return 1;
	}	
	
	Af=NULL;
	p=NULL;    //插入结点的父结点
	s=T;
	A=T;
	while(s)
	{
		if(s->data==x)  //关键字已经存在，插入失败，退出
			return 0;
		if(s->bf!=0)    //搜索插入点p
		{
			Af=p;
			A=s;
		}
		p=s;           //p为插入结点的父结点
		if(s->data>x)
			s=s->lChild;
		else
			s=s->rChild;	
	}

	s=new btNode;
	s->data=x;
	s->lChild=NULL;
	s->rChild=NULL;
	s->bf=0;

	if(p->data>x)
		p->lChild=s;   //新结点插入为p的左孩子
	else
		p->rChild=s;   //新结点插入为p的右孩子

	//新结点已经插入，A为最低不平衡子树根结点
	//d为标记，d=1：新结点插入到A的左子树；d=-1：新结点插入到A的右子树
	if(A->data>x)      //x插入到A的左子树上
	{
		B=A->lChild;   //B为A的左孩子
		p=B;
		d=1;
	}
	else               //x插入到A的右子树上
	{
		B=A->rChild;   //B为A的右孩子
		p=B;
		d=-1;
	}

	//修改结点B到新插入结点s路径上各结点的bf值
	while(p!=s)
	{
		if(p->data>x)   //p的左子树增高
		{
			p->bf=1;
			p=p->lChild;		
		}
		else             //p的右子树增高
		{
			p->bf=-1;
			p=p->rChild;	
		}	
	}

	if(A->bf==0)   //A原来bf为0，插入后不会失去平衡
	{
		A->bf=d;
		return 1;	
	}
	if(A->bf==-d)  //新结点插在较低子树上，A的平衡因子变为0
	{
		A->bf=0;
		return 1;
	}

	//新结点插入在较高子树上，失衡，需要调整
	if(d==1)       //新结点插入在A的左子树上
	{
		if(B->bf==1)
			B=LL(A,B);  //LL型调整
		else
			B=LR(A,B);  //LR型调整
	}
	else          //新结点插入在A的右子树上
	{
		if(B->bf==-1)
			B=RR(A,B);  //RR型调整
		else
			B=RL(A,B);  //RL型调整	
	}

	if(Af==NULL)
		T=B;   //原来的A为树根
	else
	{
		if(Af->lChild==A)
			Af->lChild=B;
		else
			Af->rChild=B;	
	}
}

//交互创建平衡二叉树算法开始------------------------------------------------------------------
void CreateAVLTree(btNode *&T)
{
	elementType x;	
	T=NULL;      //初始化二叉排序树为空树
	cout<<"请输入结点数据（整数，-9999退出）："<<endl;
	cout<<"x=";
	cin>>x;
	    //循环读入数据，创建结点，插入到平衡二叉树中，-9999退出
	while(x!=-9999)
	{		
		InsertNode(T,x);   //平衡二叉树插入结点

		cout<<"x=";
		cin>>x;	
	}
}
//交互创建平衡二叉树算法结束------------------------------------------------------------------


//递归先序遍历
void preTraverse(btNode *T)
{
	if(T)
	{
		cout<<T->data<<" ";         //访问根结点。打印当前结点元素值，替代visit(T)函数
		preTraverse(T->lChild);     //先序遍历左子树
		preTraverse(T->rChild);     //先序遍历右子树
	}
}
//中序遍历--递归
void inTraverse(btNode *T)
{
	if(T)
	{		
		inTraverse(T->lChild);      //中序遍历左子树
		cout<<T->data<<" ";         //访问根结点。打印当前结点元素值，替代visit(T)函数
		inTraverse(T->rChild);      //中序遍历右子树
	}
}
//后序遍历--递归
void postTraverse(btNode *T)
{
	if(T)
	{		
		postTraverse(T->lChild);      //后序遍历左子树		
		postTraverse(T->rChild);      //后序遍历右子树
		cout<<T->data<<" ";         //访问根结点。打印当前结点元素值，替代visit(T)函数
	}
}

//二叉排序树删除结点算法开始----------------------------------------------------------------
int delNode(btNode *&T, elementType x)
{
	btNode *p,*s,*pf;   //p指删除结点，pf指p的父结点
	
	if(T==NULL)
		return 0;       //空树删除失败
	  //搜索待删除结点p，及其父结点指针pf
	p=T;
	pf=T;
	while(p)
	{		
		if(p->data==x)   //找到目标结点，退出循环
			break;
		else if(p->data>x)  //搜索左子树
		{
			pf=p;
			p=p->lChild;
		}
		else                //搜索右子树
		{
			pf=p;
			p=p->rChild;		
		}	
	}
	if(p==NULL)   //目标结点不存在，删除失败
		return 0;
	    //下面开始删除结点p，其父结点为pf
	if(p->lChild==NULL)   //p没有左子树，或叶子结点，用右子树的根结点替代p
	{
		if(pf==p)   //删除的是根结点
			T=p->rChild;
		else
		{
			if(pf->lChild==p)
				pf->lChild=p->rChild;
			else
				pf->rChild=p->rChild;		
		}
		delete p;	
	}
	else if(p->rChild==NULL)  //p没有右子树
	{
		if(pf==p)   //删除的是根结点
			T=p->lChild;
		else
		{
			if(pf->lChild==p)
				pf->lChild=p->lChild;
			else
				pf->rChild=p->lChild;		
		}
		delete p;		
	}
	else    
	{       //顶替法1
		    //p的左、右子树皆不空，p的中序直接前驱替代，即p左子树最大值结点替代
		pf=p;		
		s=p->lChild;
		while(s->rChild)
		{
			pf=s;			
			s=s->rChild;
		}
		p->data=s->data;
		if(pf==p)  //s为p（pf）左子树的根结点，且没有右子树，s是p（pf）的直接前驱。将s的左子树接续为p的左子树。
			pf->lChild=s->lChild;
		else
			pf->rChild=s->lChild;
		delete s;	
	}	
	return 1;
}

//二叉排序树删除结点算法开始----------------------------------------------------------------

//销毁二叉树
void DestroyBt(btNode *&T)
{
	if(T)
	{
		DestroyBt(T->lChild);   //递归销毁左子树
		DestroyBt(T->rChild);   //递归销毁右子树
		delete T;               //释放根结点
		T=NULL;
	}
}

//求二叉树高度（深度）
int btHeight(btNode *T)
{
	int lh,rh;
	if(!T)
		return 0;
	else
	{
		lh=btHeight(T->lChild);   //求左子树高度
		rh=btHeight(T->rChild);   //求右子树高度
		//return (lh>rh?lh:rh)+1;  //简略写法
		if(lh>rh)
			return lh+1;
		else
			return rh+1;
	}
}

//设置平衡因子bf--测试删除算法
void setBf(btNode *&T,elementType x)
{
	btNode *p;
	
	if(T==NULL)
		return;
	p=T;
	while(p)
	{
		p->bf=btHeight(p->lChild)-btHeight(p->rChild);
		if(p->data>x)
			p=p->lChild;
		else
			p=p->rChild;	
	}
}

//二叉排序树查找结点算法开始----------------------------------------------------------------
btNode* Search(btNode *&T, elementType x)
{
	btNode *p,*s,*pf;   //p指删除结点，pf指p的父结点
	
	if(T==NULL)
		return NULL;       //空树查找失败
	  //搜索待删除结点p，及其父结点指针pf
	p=T;
	pf=T;
	while(p)
	{		
		if(p->data==x)   //找到目标结点，退出循环
			break;
		else if(p->data>x)  //搜索左子树
		{
			pf=p;
			p=p->lChild;
		}
		else                //搜索右子树
		{
			pf=p;
			p=p->rChild;		
		}	
	}
	
	if(p==NULL)   //目标结点不存在，查找失败
		return NULL;
			
	return p;
}
