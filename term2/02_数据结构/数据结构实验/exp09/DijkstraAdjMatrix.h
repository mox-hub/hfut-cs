

//Dijkstra算法--基于邻接矩阵
//**************  Dijkstra算法--基于邻接矩阵 ****************//
//* 函数功能：给定顶点，求解此点与网中其它顶点的最短路径    *//
//* 入口参数：Graph G，待访问的网（图）                     *//
//*           int vID，指定顶点的编号                       *//
//* 出口参数：int path[]，返回最短路径信息                  *//
//*           int dist[]，返回最短距离值                    *//
//* 返 回 值：无                                            *//
//***********************************************************//
void Dijkstra(Graph &G, int path[], int dist[], int vID)
{
	int solved[MaxVerNum];  //标记顶点是否已经求出最短路径。1--已求解，0--未求解
	int i,j;
	int v;
	cellType minDist;  //最短距离，cellType为自定义的邻接矩阵中元素的数据类型
	//初始化集合S和距离向量
	for(i=1;i<=G.VerNum;i++)
	{
		solved[i]=0;  //所有顶点均待求
		dist[i]=G.AdjMatrix[vID][i];
		if(dist[i]!=INF)
			path[i]=vID;  //第i顶点的前驱为vID
		else
			path[i]=-1;   //当前顶点i无前驱
	}
	solved[vID]=1; //标记顶点vID已求解
	dist[vID]=0;   //vID到自身的距离为0
	path[vID]=-1;  //vID为起始顶点，无前驱

	//依次找出其它n-1个顶点加入已求解集合S
	for(i=1; i<G.VerNum; i++)
	{
		minDist=INF;
		for(j=1;j<=G.VerNum;j++)  //在未解顶点中寻找距vID距离最近的顶点，编号保存到v。
		{
			if(solved[j]==0 && dist[j]<minDist)  //j目前尚在V-S中，为未解顶点
			{
				v=j;
				minDist=dist[j];
			}
		}
		
		if(minDist==INF)
			return;
		cout<<"选择顶点："<<G.Data[v]<<"--距离："<<minDist<<endl;   //输出本次选择的顶点距离

		solved[v]=1;  //顶点v已找到最短距离，加入已解集合S中

		//对选中的顶点v，修改未解顶点集V-S中，v的邻接顶点（直接后继）到顶点vID的距离
		for(j=1; j<=G.VerNum; j++)
		{
			if(solved[j]==0 && (minDist+G.AdjMatrix[v][j])<dist[j])
			{
				dist[j]=minDist+G.AdjMatrix[v][j];   //更新顶点j到顶点vID的最短距离。
				path[j]=v;   //更新顶点j的直接前驱为顶点v
			}		
		}
	}

/*	cout<<"*******************"<<endl;
	for(i=1; i<=G.VerNum;i++)
	{
		cout<<i<<"--"<<path[i]<<endl;
	}

	cout<<"*******************"<<endl;
*/
}

void PrintDijkstra(Graph &G, int path[], int dist[], int vID )
{
	int sPath[MaxVerNum];  //定义一个类似栈操作的数组
	int vPre;  //前驱结点编号
	int top=-1;  //栈顶
	int i;

	int j;

	for(i=1; i<=G.VerNum; i++)
	{
		cout<<G.Data[vID]<<" to "<<G.Data[i];
		if(dist[i]==INF)
			cout<<" 无可达路径。"<<endl;
		else
		{
			cout<<" 最短距离："<<dist[i]<<endl;
			cout<<"           路径：";
		}
		//printf("\nDistanced: %7d, path:", dist[i]);
		
		top++;
		sPath[top]=i;   //sPath[0]为当前顶点i
		vPre=path[i];  //i顶点的前驱顶点
		while(vPre!=-1)  //从第i个顶点，通过前驱顶点往前搜索到根结点，给出最短路径
		{
			top++;
			sPath[top]=vPre;
			vPre=path[vPre];
		}

/*		while(top>0)
		{
			top--;
			cout<<G.Data[sPath[top]]<<"  ";
			//printf("%2d", sPath[top]);			
		}
*/
		
		if(dist[i]!=INF)
		{
			for(j=top;j>=0;j--)  //sPath[top]为指定的起始顶点
			{
				cout<<G.Data[sPath[j]]<<"  ";
			}
		}

		top=-1;
		cout<<endl;
	}
}