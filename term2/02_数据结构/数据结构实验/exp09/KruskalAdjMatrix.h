
//Kruskal算法--基于邻接矩阵
//所有数组0单元未用，使顶点编号与数组小标相同





//定义边的结构类型
typedef struct edgeType
{
	int vBegin;  //边的起始顶点编号，从1开始
	int vEnd;    //边的终止顶点编号，从1开始
	cellType eWeight;  //边的权值
}EdgeType;

//从图的邻接矩阵读取所有边的信息，存储到一个数组中
void GetEdges(Graph &G, EdgeType edges[])
{
	int i,j;
	int k=0;  //作为边的编号
	for(i=1;i<=G.VerNum;i++)  //行循环
	{
		for(j=1;j<=G.VerNum;j++)  //列循环
		{
			if((G.AdjMatrix[i][j]>=1) &&   //数组下标与顶点编号相同
		       (G.AdjMatrix[i][j]<INF)) 
			{
				edges[k].vBegin=i;  //边的第一顶点
				edges[k].vEnd=j;    //边的第二顶点 
				edges[k].eWeight=G.AdjMatrix[i][j];  //边的权值
				k++;
			}		   
		}
	}
}

//获取当前可用最小边
EdgeType GetMinEdge(Graph &G, EdgeType edges[], int edgeUsed[], int &n)  //n为返回的最小边在edges[]数组中的下标
{
	EdgeType minEdge;
	cellType wMin=INF;  //保存最小权值
	int i;
	int M;  //控制循环次数
	if(G.gKind==UDG || G.gKind==UDN)
		M=G.ArcNum*2;  //无向网，无向图，应为对称性，邻接居中有效数据是边数的2倍
	else
		M=G.ArcNum;    //有向图，有向网中，M即为图的边数

	for(i=0; i<M ;i++) //因为无向图邻接矩阵对称中，有效数据是边数的2倍，所以乘2
	{
		if(edgeUsed[i]==0 && edges[i].eWeight<wMin)
		{
			wMin=edges[i].eWeight;
			minEdge.eWeight=edges[i].eWeight;;
			minEdge.vBegin=edges[i].vBegin;
			minEdge.vEnd=edges[i].vEnd;
			n=i;
		}
	}

	return minEdge;  //返回取得的最小边
}

//Kruskal算法
void Kruskal(Graph &G)
{
	int conVerID[MaxVerNum];  //顶点的连通分量编号数组
	EdgeType edges[MaxVerNum*MaxVerNum];  //存放图的所有边信息
	EdgeType treeEdges[MaxVerNum];        //存放生成树中的边信息，n-1条

	int edgeUsed[MaxVerNum*MaxVerNum];  //辅助数组，标记图中的边是否已经尝试过。1--已用过，0--未用过
	                                    //也可以用排序算法先对图的所有边进行排序来完成这个工作

	EdgeType minEdge;  //保存最小边
	int i,j;
	int k=0;
	int conID;  //边的终止顶点的连通分量的编号
    
	int n;  //返回的最小边的序号

	//获取图所有边的信息，存入数组edges[]
	GetEdges( G, edges );

	//初始化可用边数组--可用排序算法取代
	int M;  //循环次数
	if(G.gKind==UDG ||G.gKind==UDN)
		M=G.ArcNum*2; //因为无向图、无向网邻接矩阵对称，有效数据是边数的2倍，所以乘2
	else
		M=G.ArcNum;

	for(i=0; i<M; i++)  
		edgeUsed[i]=0;

	//初始化连通分量编号。开始每个顶点作为一个连通分量，每个一个编号，从1开始，与顶点编号相同
	for(i=1;i<=G.VerNum;i++)
	{
		conVerID[i]=i;  //顶点编号与数组下标差1
	}

	for(i=1; i<G.VerNum; i++)  //取出n-1条边，构成生成树
	{
		minEdge=GetMinEdge(G, edges, edgeUsed, n );
		while(conVerID[minEdge.vBegin]==conVerID[minEdge.vEnd])
		{
			edgeUsed[n]=1;  //2个顶点属于同一个连通分量，必构成回路，此最小边不可用
			minEdge=GetMinEdge( G, edges, edgeUsed, n ); //继续取下一条最小边，直到取得可用边为止
		}
		    //到此取得了1条可用最小边，加入生成树中
		treeEdges[i]=minEdge;
		conID=conVerID[minEdge.vEnd]; //取得最小边的终点编号
		//conID=conVerID[minEdge.vBegin]; //取得最小边的起点编号

		for(j=1;j<=G.VerNum;j++)  //合并连通编号到最小编号
		{
			if(conVerID[j]==conID)  //所有连通分量编号为conID的顶点，连通分量编号都改为最小边起始顶点的连通号
			{
				conVerID[j]=conVerID[minEdge.vBegin];
			}
		}


		edgeUsed[n]=1; 

	}

	//输出结果	
	cellType wAll=0;  //总权值
	cout<<endl;  //输出结果
	cout<<"Kruskal最小生成树-->>"<<endl;
	cout<<"选择的边和权值："<<endl;
	for(i=1;i<G.VerNum;i++)  //n-1条边
	{	
		cout<<"("<<G.Data[treeEdges[i].vBegin]<<","<<G.Data[treeEdges[i].vEnd]<<")";
		cout<<" 或 ("<<treeEdges[i].vBegin<<","<<treeEdges[i].vEnd<<")。权值："<<treeEdges[i].eWeight<<endl;
		wAll+=treeEdges[i].eWeight;
	}
	cout<<"生成树总权值："<<wAll<<endl;
	cout<<endl;
	
}

//----并查集处理部分--------------------------------------

//1.初始化并查集
//--int anc[]存储每个集合的代表值，此处anc[0]单元未用；n为元素个数
//--anc[]可设为全局数组，在递归中更节约空间
void unionIni(int anc[], int n)
{
	for(int i=1;i<=n; i++)
		anc[i]=i;
}

//2.查找函数
int find(int anc[], int x) 
{
	if(x!=anc[x])
		anc[x]=find(anc,anc[x]);
	return anc[x];
}

//3.合并函数
void unionSet(int anc[], int x, int y)
{
	int fx,fy;
	fx=find(anc,x);
	fy=find(anc,y);
	if(fx!=fy)
		anc[x]=fy;
}

//----并查集处理结束---------------------------------------

//--使用并查集的Kruskal算法
void disjointKruskal(Graph &G)
{
	
	EdgeType edges[MaxVerNum*MaxVerNum];  //存放图的所有边信息
	EdgeType treeEdges[MaxVerNum];        //存放生成树中的边信息，n-1条

	int edgeUsed[MaxVerNum*MaxVerNum];  //辅助数组，标记图中的边是否已经尝试过。1--已用过，0--未用过
	                                    //也可以用排序算法先对图的所有边进行排序来完成这个工作

	int anc[MaxVerNum];  //---只在并查集处理时使用
	unionIni(anc,MaxVerNum);

	EdgeType minEdge;  //保存最小边
	int i,j;
	
    
	int n;  //返回的最小边的序号

	//获取图所有边的信息，存入数组edges[]
	GetEdges( G, edges );

	//初始化可用边数组--可用排序算法取代
	int M;  //循环次数
	if(G.gKind==UDG ||G.gKind==UDN)
		M=G.ArcNum*2; //因为无向图、无向网邻接矩阵对称，有效数据是边数的2倍，所以乘2
	else
		M=G.ArcNum;

	for(i=0; i<M; i++)  
		edgeUsed[i]=0;


	for(i=1; i<G.VerNum; i++)  //取出n-1条边，构成生成树
	{
		minEdge=GetMinEdge(G, edges, edgeUsed, n );
		if(find(anc,minEdge.vBegin)==find(anc,minEdge.vEnd))  //并查集查询当前边的2个顶点是否在同一个连通分量上
		{
			edgeUsed[n]=1;  //2个顶点属于同一个连通分量，必构成回路，此最小边不可用
			minEdge=GetMinEdge( G, edges, edgeUsed, n ); //继续取下一条最小边，直到取得可用边为止
		}
		    //到此取得了1条可用最小边，加入生成树中
		treeEdges[i]=minEdge;

		//并查集的合并处理，边的2个顶点合并到一个集合（同一个连通分量）
		unionSet(anc,minEdge.vBegin,minEdge.vEnd);

		edgeUsed[n]=1; 

	}

	//输出结果	
	cellType wAll=0;  //总权值
	cout<<endl;  //输出结果
	cout<<"Kruskal（并查集）最小生成树-->>"<<endl;
	cout<<"选择的边和权值："<<endl;
	for(i=1;i<G.VerNum;i++)  //n-1条边
	{	
		cout<<"("<<G.Data[treeEdges[i].vBegin]<<","<<G.Data[treeEdges[i].vEnd]<<")";
		cout<<" 或 ("<<treeEdges[i].vBegin<<","<<treeEdges[i].vEnd<<")。权值："<<treeEdges[i].eWeight<<endl;
		wAll+=treeEdges[i].eWeight;
	}
	cout<<"生成树总权值："<<wAll<<endl;

	cout<<endl;	
}