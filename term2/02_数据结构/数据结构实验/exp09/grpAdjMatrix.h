//************************************************************//
//*    图的邻接矩阵存储的头文件，文件名：grpAdjMatrix.h      *//
//*                                                          *//
//************************************************************//

//#include "stdio.h"
//#include "stdlib.h"
//#include "iostream.h"
//#include "string.h"

#define INF 65535          //定义无穷大
#define MaxVerNum  100     //定义最大顶点个数
//typedef int elementType;  //定义图中顶点的数据类型
typedef char elementType;  //定义图中顶点的数据类型
typedef int cellType;      //定义邻接矩阵中元素的数据类型
                           //对无权图，1-相邻（有边），0-不相邻（无边）
                           //对有权图，为边的权值，特别是无穷大。
                           //枚举图的类型--无向图(UDG)，无向网(UDN)，有向图(DG)，有向网(DN)
typedef enum{UDG, UDN, DG, DN} GraphKind;  

bool visited[MaxVerNum+1];  //全局数组，标记顶点是否已经访问，visited[0]单元不用

//****************************************************//
//*  定义邻接矩阵表示的图结构。5个分量组成：         *//
//*      data[]数组保存图中顶点数据元素              *//
//*      AdjMatrix[][]邻接矩阵                       *//
//*      VerNum顶点个数                              *//
//*      ArcNum边（弧）条数                          *//
//*      gKind枚举图的类型                           *//
//*  考虑到名称的统一性，图类型名称定义为Graph       *//
//****************************************************//
typedef struct GraphAdjMatrix
{
	elementType Data[MaxVerNum+1];                 //顶点数组，存放顶点元素的值，Data[0]单元不用
	cellType AdjMatrix[MaxVerNum+1][MaxVerNum+1];  //邻接矩阵，数组下标为0单元不用，从AdjMatrix[1][1]单元开始
	int VerNum;       //顶点数
	int ArcNum;       //弧（边）数
	GraphKind gKind;  //图的类型:0-无向图；1-无向网；2-有向图；3-有向网
} Graph;  //图的类型名

//******************* 访问图中顶点的函数*********************//
//* 函数功能：打印图中顶点元素，并标记为已经访问            *//
//* 入口参数  Graph G，待访问的图；int verID 目标顶点编号   *//
//* 出口参数：无                                            *//
//* 返 回 值：空                                            *//
//* 函 数 名：visit(Graph &G, int verID)                    *//
//***********************************************************//
void visit(Graph &G, int verID)
{        //顶点编号从1开始，数组0单元不用
	cout<<G.Data[verID]<<"\t";
	visited[verID]=true;
}

//*******************  图中查找目标顶点 *********************//
//* 函数功能：给定顶点元素，在图中查找此顶点元素            *//
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
		if( G.Data[i]==v )
			return i;
	}
	return -1;
}
//求顶点v的第一个邻接点
int firstAdj(Graph &G,int v)
{
	int w;
	for(w=1;w<=G.VerNum;w++)
	{
		if((G.AdjMatrix[v][w]>=1)   &&
		   (G.AdjMatrix[v][w])<INF)
		   return w;    //返回第一个邻接点编号
	}
	 return 0;          //未找到邻接点，返回0
}
//求顶点v的位于邻接点w后的下一个邻接点
int nextAdj(Graph &G,int v,int w)
{
	int k;
	for(k=w+1;k<=G.VerNum;k++)
	{
		if((G.AdjMatrix[v][k]>=1)   &&
		   (G.AdjMatrix[v][k])<INF)
		   return k;    //返回v的位于w之后的下一个邻接点k
	}
	return 0;           //不存在下一个邻接点，返回0
}

//*************** 连通图或一个连通分量的DFS *****************//
//* 函数功能：深度优先遍历连通图，或一个连通分量            *//
//* 入口参数  Graph G，待访问的图；int verID 起始顶点编号   *//
//* 出口参数：无                                            *//
//* 返 回 值：无                                            *//
//* 函 数 名：DFS(Graph &G, int verID)                      *//
//***********************************************************//
void DFS(Graph &G, int verID)
{
	int w;
	visit(G, verID);
	w=firstAdj(G,verID);
	while(w!=0)
	{
		if(!visited[w])        //w尚未访问
			DFS(G,w);          //递归深度遍历
		w=nextAdj(G,verID,w);    //递归返回后，再找verID的位于w后的下一个邻接点	
	}

/*	
//以下代码亦可实现遍历DFS
	for(int w=1;w<=G.VerNum;w++)
	{
		if((G.AdjMatrix[verID][w]>=1) && 
		   (G.AdjMatrix[verID][w]<INF) &&
		   (!visited[w]))
		{
			DFS(G,w);
		}
	}
*/
}

//*******************    任意图的DFS    *********************//
//* 函数功能：连通或非连通的DFS遍历                         *//
//* 入口参数  Graph G，待访问的图；int verID 起始顶点编号   *//
//* 出口参数：无                                            *//
//* 返 回 值：无                                            *//
//* 函 数 名：DFSTraverse(Graph &G, int verID)              *//
//***********************************************************//
void DFSTraverse(Graph &G, int verID)
{
	int vID;
	for(vID=1;vID<=G.VerNum;vID++)  //访问标记数组初始化
		visited[vID]=false;
	
	DFS(G,verID);  //从指定的顶点，遍历指定的第一个连通分量
	for(vID=1;vID<=G.VerNum;vID++)  //再依次遍历图中其它的连通分量
	{
		if(!visited[vID])
			DFS(G,vID);
	}
}

//*************** 连通图或一个连通分量的BFS *****************//
//* 函数功能：广度优先遍历连通图，或一个连通分量            *//
//* 入口参数  Graph G，待访问的图；int verID 起始顶点编号   *//
//* 出口参数：无                                            *//
//* 返 回 值：无                                            *//
//* 函 数 名：BFS(Graph &G, int verID)                      *//
//***********************************************************//
void BFS(Graph &G, int verID)
{
	visit(G, verID);
	seqQueue Q;
	initialQueue(&Q);             //初始化循环顺序队列
	enQueue(&Q, verID);           //编号为verID的顶点入队
	int v,w,x;
	while(!queueEmpty(Q))         //队列不空循环处理顶点
	{
		getFront(Q, v);           //取队头元素到v，即顶点编号为v。
		outQueue(&Q,x);           //v出队		
		w=firstAdj(G,v);          //搜索v的第一个邻接点
		while(w!=0)
		{
			if(!visited[w])       //w尚未访问，访问w，并标记已访问
			{
				visit(G,w);         
				enQueue(&Q,w);    //顶点w入队
			}
			w=nextAdj(G,v,w);     //搜索顶点v位于w后的下一个邻接点
		}
	}


/*
//以下代码亦可实现BFS	
	while(!queueEmpty(Q))  //队列不空循环处理顶点
	{
		getFront(Q, u);  //取队头元素到u，即顶点编号为u。
		outQueue(&Q,x);      //u出队		
		for(int w=1;w<=G.VerNum;w++)
		{
			if((G.AdjMatrix[u][w]>=1) && 
			   (G.AdjMatrix[u][w]<INF) &&
			   (!visited[w]))
			{
				visit(G,w);     //访问编号w的邻接点
				enQueue(&Q,w);  //编号w的邻接点入队
			}
		}
	}
*/
}
//*******************    任意图的BFS    *********************//
//* 函数功能：连通或非连通的BFS遍历                         *//
//* 入口参数  Graph G，待访问的图；int verID 起始顶点编号   *//
//* 出口参数：无                                            *//
//* 返 回 值：无                                            *//
//* 函 数 名：BFSTraverse(Graph &G, int verID)              *//
//***********************************************************//
void BFSTraverse(Graph &G, int verID)
{
	int vID;
	for(vID=1;vID<=G.VerNum;vID++)  //访问标记数组初始化
		visited[vID]=false;
	
	BFS(G,verID);  //从指定的顶点，遍历指定的第一个连通分量
	for(vID=1;vID<=G.VerNum;vID++)  //再依次遍历图中其它的连通分量
	{
		if(!visited[vID])
			BFS(G,vID);  
	}

}

//******************** 打印图的相关信息 *********************//
//* 函数功能：打印图的相关信息                              *//
//* 入口参数：Graph G，待打印的图                           *//
//* 出口参数：无                                            *//
//* 返 回 值：空                                            *//
//* 函 数 名：GraphPrint(Graph &G)                          *//
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
	cout<<"编  号：";
	for(i=1;i<=G.VerNum;i++)
	{
		cout<<i<<"\t";
	}
	cout<<endl;
	cout<<"顶  点：";
	for(i=1;i<=G.VerNum;i++)
	{
		cout<<G.Data[i]<<"\t";
	}
	cout<<endl;

	//打印邻接矩阵
	cout<<"图的邻接矩阵："<<endl;
	for(i=1;i<=G.VerNum;i++)
	{   
		cout<<"\t";
		for(j=1;j<=G.VerNum;j++)
		{
			if((G.gKind==UDN || G.gKind==DN) && G.AdjMatrix[i][j]==INF)
				cout<<"INF"<<"\t";  //网，无穷大时，打印“INF”表示
			else
				cout<<G.AdjMatrix[i][j]<<"\t";
		}
		cout<<endl;
	}

}
