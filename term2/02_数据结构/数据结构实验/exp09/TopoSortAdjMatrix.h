
//拓扑排序算法--基于邻接矩阵

//初始化获取每个顶点的入度，存入入度数组inds[]中
void GetInDegrees(Graph &G, int inds[])
{
	int i,j;
	for(j=1;j<=G.VerNum;j++)  //邻接矩阵列循环
	{
		for(i=1;i<=G.VerNum;i++)  //邻接矩阵行循环
		{
			if(G.AdjMatrix[i][j]>=1 && G.AdjMatrix[i][j]<INF)
				inds[j]++;  //编号j的入度加1.
		}
	}

	//for(i=1;i<=G.VerNum;i++)  //输出每个顶点的初始入度
	//{
	//	cout<<i<<"--"<<inds[i]<<endl;
	//}
}
//拓扑排序算法--使用栈
int TopologicalSortS(Graph &G, int topoList[])
{
	int inds[MaxVerNum];  //定义顶点入度数组
	seqStack S;  //定义一个顺序栈，保存入度为0的顶点
	int i;
	int v;  //顶点编号
	int vCount=0; //记录顶点入度为0的顶点数
	
	initialStack(S);  //初始化栈

	for(i=1;i<=G.VerNum;i++)
		inds[i]=0;  //入度数组初始化

	//拓扑序列数组初始化
	for(i=1;i<G.VerNum;i++)		
		topoList[i]=-1;  //初始化顶点编号为-1

	//从邻接矩阵获取图中各个顶点的初始入度
	GetInDegrees(G, inds);

	//入度为0的顶点入栈
	for(i=1;i<=G.VerNum;i++)
	{
		if(inds[i]==0)
			push(S,i);
	}

	while(!stackEmpty(S))
	{
		pop(S,v);  //从栈顶弹出一个入度为0的顶点编号到v
		topoList[vCount+1]=v;  //当前入度为0顶点v，加入拓扑序列
		vCount++;  //已处理顶点（入度为0）数加1		

		//与v邻接的顶点的入度减1
		for(i=1;i<=G.VerNum;i++)
		{			
			if(G.AdjMatrix[v][i]>=1 && G.AdjMatrix[v][i]<INF && inds[i]>0)
			{
				inds[i]--;  //与v邻接的顶点i的入度减1
				if(inds[i]==0) //顶点i的入度已经为0，入栈
					push(S,i);
			}
		}
	}

	if(vCount==G.VerNum)
		return 1;  //返回无回路标记
	else	
		return 0;  //有回路，不能产生拓扑序列
}

//拓扑排序算法--使用队列
int TopologicalSortQ(Graph &G, int topoList[])
{
	int inds[MaxVerNum];  //顶点的入度数组
	seqQueue Q;  //保存入度为0顶点的队列
	int i;
	int v;  //保存当前处理的顶点编号
	int vCount=0;  //保存入度为0的顶点数

	initialQueue(&Q);

	for(i=1;i<=G.VerNum;i++)
	{
		inds[i]=0;  //inds[]数组元素初始化为0
		topoList[i]=-1;  //拓扑序列数组初始化为-1
	}
	GetInDegrees(G,inds); //从图的邻接矩阵获取图中各顶点的初始入度

	for(i=1;i<=G.VerNum;i++)
	{
		if(inds[i]==0)  //入度为0的顶点编号i入队
			enQueue(&Q,i);
	}

	while(!queueEmpty(Q))
	{
		getFront(Q,v);  //取队头元素到v
		outQueue(&Q,v);  //队头元素v出队
		topoList[vCount+1]=v;  //队头元素进入拓扑序列
		vCount++;  //入度为0顶点数加1

		for(i=1;i<=G.VerNum;i++)  //v的邻接顶点入度减1
		{
			if(G.AdjMatrix[v][i]>=1 && G.AdjMatrix[v][i]<INF && inds[i]>0)
			{
				inds[i]--;  //v的邻接顶点i的入度减1
				if(inds[i]==0)  //顶点i的入度已经为0，入队
					enQueue(&Q,i);
			}
		}
	}

	if(vCount==G.VerNum)
		return 1;  //无回路，拓扑排序成功
	else
		return 0;  //有回路，拓扑排序失败
}

//拓扑排序算法--不使用栈和队列，使用一个标记数组solved[]
int TopologicalSort(Graph &G, int topoList[])
{
	int inds[MaxVerNum];  //保存图中个顶点的入度
	int solved[MaxVerNum];  //标记图中入度为0的顶点是否已经处理过。0--未处理；1--已处理。
	int i;
	int v=-1;  //当前处理的入度为0的顶点编号
	int vCount=0;  //记录入度为0的顶点个数。

	//初始化
	for(i=1;i<=G.VerNum;i++)
	{
		inds[i]=0;       //所有顶点入度置为0
		solved[i]=0;     //所有顶点标记为未处理
		topoList[i]=-1;  //拓扑序列元素全部初始化为-1
	}

	//从邻接矩阵获取图中各个顶点的初始入度，由inds[]数组返回
	GetInDegrees(G,inds);
    
	//取得第一个入度为0的顶点编号（如果存在），保存到v
	for(i=1;i<=G.VerNum;i++)
	{
		if(inds[i]==0 && !solved[i])
		{
			v=i;
			break;
		}
	}

	while(v!=-1)
	{
		topoList[vCount+1]=v;  //顶点v加入拓扑序列
		vCount++;  //入度为0顶点数加1
		solved[v]=1;  //标记顶点v已经处理

		//与顶点v相邻的顶点入度减1
		for(i=1;i<=G.VerNum;i++)
		{
			if(G.AdjMatrix[v][i]>=1 && G.AdjMatrix[v][i]<INF && inds[i]>0)
				inds[i]--;  //顶点i入度减1
		}

		//获取下一个入度为0，且尚未处理的顶点
		v=-1;
		for(i=1;i<=G.VerNum;i++)
		{
			if(inds[i]==0 && !solved[i])
			{
				v=i;
				break;  //找到符合条件的顶点，退出循环--入度为0，且未处理
			}
		}
	}

	if(vCount==G.VerNum)
		return 1;  //无回路，拓扑排序成功
	else
		return 0;  //有回路，拓扑排序失败
}
