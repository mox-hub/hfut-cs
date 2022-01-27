
//Prim算法基于邻接矩阵的实现




int inTree[MaxVerNum+1]={0}; //标记顶点已经在Prim生成树中，或已经访问过。1或true--已访问，0或false--未访问
                             //inTree[0]单元未用
                             //或者为标记已经在集合U中的顶点
//保存候选边的信息
typedef struct minEdgeType
{
	int v;              //V-U中当前选中的顶点编号，从1开始。即刚从V-U中选出放到U中的顶点
	cellType eWeight;  //U中某个顶点到V-U中当前顶点v的最小距离
} MinEdgeType;

//保存已经选择的边的信息
typedef struct selectedEdge
{
	int vBegin;  //选中边的起始顶点编号，从1开始
	int vEnd;    //选中边的结束顶点编号，从1开始
	cellType eWeight;
} SelectedEdgeType;

//检查图G中编号为vBegin和vEnd之间是否有边
int HasEdge(Graph &G, int vBegin, int vEnd)
{
	if(G.gKind==UDN || G.gKind==DN)  //网情况
	{
		if(G.AdjMatrix[vBegin][vEnd]!=INF)  //权值非无穷大即有边
			return 1;  //有边，返回1
		else
			return 0;  //无边，返回0
	}
	else  //图情况
	{
		if(G.AdjMatrix[vBegin][vEnd]==1)  //顶点编号与数组下标差1
			return 1;  //有边，返回1
		else
			return 0;  //无边，返回0
	}
}


//初始化候选边数组，和已经选择边数组
void InitMinEdges(Graph &G, MinEdgeType minEdges[],int vID)
{
	int i;
	for(i=1;i<=G.VerNum;i++)
	{
		//初始化候选边数组
		if(HasEdge(G, vID, i))
		{
			minEdges[i].v=vID;
			minEdges[i].eWeight=G.AdjMatrix[vID][i];  //权值初始化为无穷大
		}
		else
			minEdges[i].eWeight=INF;
	}
}

//从候选边集合中选出最小边，返回在V-U中的关联顶点编号
int GetMinEdge(Graph &G, MinEdgeType minEdges[])
{
	cellType eMin=INF;  //保存最小的权值
	int i,j;
	for(i=1;i<=G.VerNum;i++)
	{
		if(inTree[i]==0 && minEdges[i].eWeight<eMin)
		{
			j=i;  //如果当前编号为i的顶点在集合V-U中，且权值比eMin小，暂选为最小边
			eMin=minEdges[i].eWeight;
		}
	}
	return j;  //j即为V-U中，最小边关联顶点的编号
}


void ChangeMinEdgesWeight(Graph &G, MinEdgeType minEdges[], int vID)
{
	//对新选出的编号为vID的顶点（新加入集合U中），调整候选边集合
	int i,j; 
	for(i=1;i<=G.VerNum;i++)
	{
		if(inTree[i]==0)  //编号i顶点在V-U中，不在U中
		{
			//检查vID与i之间是否相邻（有边）
			//检查U中顶点vID与i之间的边权值是否更小，若更小则更新(vID,i)权值
			if(HasEdge(G,vID,i) && G.AdjMatrix[vID][i]<minEdges[i].eWeight)
			{
				minEdges[i].v=vID;
				minEdges[i].eWeight=G.AdjMatrix[vID][i];
			}
		}
	}
}

void Prim(Graph &G, int vID)
{
	
	MinEdgeType minEdges[MaxVerNum];  //minEdges[i]的下标加1，即i+1为选定边的起始点
	                                  //minEdges[i].v为选定边的终点  

	int i;
	int	curID;  //当前选择顶点编号
	cellType wAll=0;  //权值总和

	InitMinEdges(G, minEdges, vID);  //初始化候选边数组

	inTree[vID]=true;  //标记vID已在生成树上，即集合U中

	for(i=1;i<G.VerNum;i++)  //选择n-1条边，形成生成树
	{
		curID=GetMinEdge(G, minEdges);  //选择V-U中最小边关联的顶点		
		inTree[curID]=true;  //标记curID已选进U中
		ChangeMinEdgesWeight(G, minEdges, curID);  //修改权值

		
	}
	
	cout<<endl;  //输出结果
	cout<<"Prim生成树起始顶点："<<G.Data[vID]<<"，编号："<<vID<<endl;
	cout<<"选择的边和权值："<<endl;
	for(i=1;i<=G.VerNum;i++)
	{
		//cout<<i<<"--"<<minEdges[i].v<<",  "<<minEdges[i].eWeight<<endl;
		if(i!=vID)
		{
			cout<<"("<<G.Data[minEdges[i].v]<<","<<G.Data[i]<<")";
			cout<<" 或 ("<<minEdges[i].v<<","<<i<<")。权值："<<minEdges[i].eWeight<<endl;
			//cout<<i<<"--"<<minEdges[i].v<<",  "<<minEdges[i].eWeight<<endl;
			//cout<<minEdges[i].eWeight<<"\t";
			wAll+=minEdges[i].eWeight;
		}
	}
	cout<<"生成树总权值："<<wAll<<endl;
	cout<<endl;

}

void PrimPrint(Graph &G, MinEdgeType minEdges[], int vID)
{
	int i;
	cellType wAll=0;
	cout<<"Prim生成树起始顶点："<<vID<<endl;
	cout<<"选择的边和权值："<<endl;
	for(i=1;i<=G.VerNum;i++)
	{
		if(i!=vID)
		{
			cout<<i<<"--"<<minEdges[i].v<<",  "<<minEdges[i].eWeight<<endl;
			wAll+=minEdges[i].eWeight;
		}
	}
	cout<<"生成树总权值："<<wAll<<endl;
	cout<<endl;

}