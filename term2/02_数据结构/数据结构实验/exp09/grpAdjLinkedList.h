//************************************************************//
//*    图的邻接链表表示的头文件，文件名：GraphAdjLinkList.h  *//
//*                                                          *//
//************************************************************//
#define INF 65535          //定义无穷大
#define MaxVerNum  100     //定义最大顶点个数
using namespace std; 

typedef char elementType;  //定义图中顶点的数据类型
typedef int eInfoType;     //边链表中关于边的信息的数据类型，比如，带权图中可以表示边的权值  
typedef int cellType;      //定义邻接矩阵中元素的数据类型
                           //对无权图，1-相邻（有边），0-不相邻（无边）
                           //对有权图，为边的权值，特别是无穷大。
typedef enum{UDG, UDN, DG, DN} GraphKind;  //枚举图的类型--无向图，无向网，有向图，有向网

typedef struct eNode       //边链表结点结构
{
	int adjVer;            //邻接顶点地址，此处为顶点在顶点表中序号，从1开始
	eInfoType eInfo;       //边链表中表示边的相关信息，比如表的权值
	struct eNode* next;    //指向边链表中的下一个结点
}EdgeNode;                 //边链表结点类型

typedef struct vNode       //顶点表中元素结构
{
	elementType data;      //存放图中顶点的数据
	EdgeNode* firstEdge;   //指向此顶点关联的第一条边的指针，即边链表的头指针
}VerNode;

typedef struct GraphAdjLinkList
{
	VerNode VerList[MaxVerNum+1];  //存放顶点的顺序表，数组0单元不用
	int VerNum;                    //顶点数
	int ArcNum;                    //弧（边）数
	GraphKind gKind;               //图的类型:0-无向图；1-无向网；2-有向图；3-有向网
}Graph;                            //图的类型名


bool visited[MaxVerNum+1];  //全局数组，标记顶点是否已经被访问。0--未访问；1--已访问。数组0单元不用

//******************* 访问图中顶点的函数*********************//
//* 函数功能：打印图中顶点元素，并标记为已经访问            *//
//* 入口参数  Graph G，待访问的图；int verID 目标顶点编号   *//
//* 出口参数：无                                            *//
//* 返 回 值：空                                            *//
//* 函 数 名：visit(Graph &G, int verID)                    *//
//***********************************************************//
void visit(Graph &G, int verID)
{         //顶点编号从1开始，数组0单元不用
	cout<<G.VerList[verID].data<<"\t";
	visited[verID]=true;
}

//*******************  图中查找目标顶点 *********************//
//* 函数功能：给定顶点元素，在图中查找此顶点元素的编号      *//
//* 入口参数  Graph G，待访问的图；elementType v 目标顶点   *//
//* 出口参数：无                                            *//
//* 返 回 值：int。如果目标顶点存在，返回顶点编号，         *//
//*                顶点编号从1开始；否则返回-1              *//
//* 函 数 名：visit(Graph &G, int verID)                    *//
//***********************************************************//
int LocateVertex(Graph &G, elementType v)
{
	for(int i=1;i<=G.VerNum;i++)
	{
		if( G.VerList[i].data==v )
			return i;
	}
	return -1;
}

//搜索顶点v的第一个邻接顶点
int firstAdj(Graph &G, int v)
{
	EdgeNode *p;
	p=G.VerList[v].firstEdge;
	if(p)
		return p->adjVer;
	else
		return 0;
}

//搜索顶点v位于邻接点w之后的下一个邻接点
int nextAdj(Graph &G, int v, int w)
{
	EdgeNode *p;
	p=G.VerList[v].firstEdge;         //取顶点v的边链表头指针
	while(p->next)
	{
		if(p->adjVer==w)
			return p->next->adjVer;  //返回w之后下一个邻接点编号
		p=p->next;	
	}
	return 0;                        //未找到下一个邻接点，返回0

}


//******************** 打印图的相关信息 *********************//
//* 函数功能：打印图的相关信息                              *//
//* 入口参数  Graph G，待打印的图                           *//
//* 出口参数：无                                            *//
//* 返 回 值：空                                            *//
//* 函 数 名：printGraph(Graph &G)                          *//
//***********************************************************//
void printGraph(Graph &G)
{
	int i=0,j=0;
	//打印图的类型
	switch(G.gKind)
	{
	case UDG:
		cout<<"图类型：无向图"<<endl;
		break;
	case UDN:
		cout<<"图类型：无向网"<<endl;
		break;
	case DG:
  	    cout<<"图类型：有向图"<<endl;
		break;
	case DN:
		cout<<"图类型：有向网"<<endl;
		break;
	default:
		cout<<"图类型错误。"<<endl;
		break;
	}
	//打印图的顶点数
	cout<<"顶点数："<<G.VerNum<<endl;
	//打印图的边数
	cout<<"边  数："<<G.ArcNum<<endl;
	//打印顶点及其编号
	cout<<"编号\t顶点\t边链表"<<endl;
    EdgeNode* p;    
	for(i=1;i<=G.VerNum;i++)
	{
		cout<<i<<"\t"<<G.VerList[i].data<<"\t";
		p=G.VerList[i].firstEdge;
		while(p!=NULL)
		{
			cout<<p->adjVer<<"\t";
			p=p->next;
		}
		cout<<endl;
	}
	cout<<endl;
	//打印邻接矩阵
	cout<<"邻接矩阵："<<endl;
	for(i=1;i<=G.VerNum;i++)
	{   
		cout<<"\t";
		p=G.VerList[i].firstEdge;
		j=1;
		while(p!=NULL || j<=G.VerNum)
		{
			if((j<=G.VerNum) && (p!=NULL) && j==p->adjVer)  //有边
			{
				cout<<p->eInfo<<"\t";
				j++;
				p=p->next;
			}
			else   //无边
			{
				if(G.gKind==UDN || G.gKind==DN)
					cout<<"INF"<<"\t";  //网，无边时打印权值为无穷大，用“INF”表示
				else
					cout<<"0"<<"\t";    //图，无边时用0表示
				j++;			
			}
			
		}
		cout<<endl;
	}
}
