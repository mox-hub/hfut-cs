
//Floyd算法

//typedef cellType dist[MaxVerNum][MaxVerNum];
//typedef int path[MaxVerNum][MaxVerNum];

void Floyd(Graph &G, cellType dist[MaxVerNum][MaxVerNum], int path[MaxVerNum][MaxVerNum])
{
	int i,j,k;
	//初始化距离矩阵和路径矩阵
	for(i=1;i<=G.VerNum;i++)
	{
		for(j=1;j<=G.VerNum;j++)
		{
			dist[i][j]=G.AdjMatrix[i][j];   //距离矩阵初始化为邻接矩阵
			                                        //初始化路径矩阵，路径矩阵元素path[i][j]中保存编号j顶点的前驱的顶点编号
            if( i!=j && G.AdjMatrix[i][j]<INF)  //如果i,j之间存在边，则j的前驱为i。否则前驱置为-1
				path[i][j]=i;
			else
				path[i][j]=-1;													
		}
	}

	//从k=1开始，迭代到k=G.verNum。依次选择一个顶点k，作为顶点i、j之间的中转顶点，优化顶点i、j之间的距离
	//下面是Floyd算法的核心--三重for循环
	for(k=1; k<=G.VerNum; k++)
	{
		for(i=1; i<=G.VerNum; i++)
		{
			for(j=1; j<=G.VerNum;j++)
			{
				if(i!=j && dist[i][k]+dist[k][j]<dist[i][j])  //k作为中转跳点，i、j之间距离变小，接收k作为中转点，更新i、j之间的距离
				{
					dist[i][j]=dist[i][k]+dist[k][j];  //更新距离
					path[i][j]=path[k][j];  //修改前驱顶点					
				}
			}	
		}

		    //打印k顶点作为中转跳点后优化的距离矩阵
		cout<<"第"<<k<<"轮优化后的距离矩阵："<<endl;
		for(i=1;i<=G.VerNum;i++)
		{   
			cout<<"\t";
			for(j=1;j<=G.VerNum;j++)
			{
				if((G.gKind==UDN || G.gKind==DN) && dist[i][j]==INF)
					cout<<"INF"<<"\t";  //网，无穷大时，打印“INF”表示
				else
					cout<<dist[i][j]<<"\t";
			}
			cout<<endl;
		}
		    //打印k顶点作为中转跳点后优化的路径矩阵
		cout<<"第"<<k<<"轮优化后的路径矩阵："<<endl;
		for(i=1;i<=G.VerNum;i++)
		{   
			cout<<"\t";
			for(j=1;j<=G.VerNum;j++)
			{
				cout<<path[i][j]<<"\t";
			}
			cout<<endl;
		}
	}
}

//打印Floyd算法给出的最短路径
void PrintFloyd(Graph &G, cellType dist[MaxVerNum][MaxVerNum], int path[MaxVerNum][MaxVerNum])
{
	int sPath[MaxVerNum];  //定义一个类似栈操作的数组
	int pra;  //前驱结点编号
	int top=-1;  //栈顶
	int i;

	int j;
	int m;

	for(i=1; i<=G.VerNum; i++)
	{		
		for(j=1; j<=G.VerNum; j++)
		{
			cout<<G.Data[i]<<" to "<<G.Data[j];
			if(dist[i][j]==INF)
				cout<<" 无可达路径。"<<endl;
			else
			{
				cout<<" 最短距离："<<dist[i][j]<<endl;
				cout<<"           路径：";

				top++;
				sPath[top]=j;   //sPath[0]为当前顶点i
				pra=path[i][j];  //i顶点的前驱顶点
				while(pra!=i)
				{
					top++;
					sPath[top]=pra;
					pra=path[i][pra];
				}
				top++;
				sPath[top]=i;  //加进起始顶点i
				
				if(dist[i][j]!=INF)
				{
					for(m=top;m>=0;m--)  //sPath[top]为指定的起始顶点
					{
						cout<<G.Data[sPath[m]]<<"  ";
					}
				}
				
				top=-1;
				cout<<endl;				
			}	
		}
	}
}