#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include "string.h"
#include "time.h"
//=========自定义头文件=================
#include "seqQueue.h"
#include "SeqStack.h"
#include "grpAdjMatrix.h"
#include "createGrpAdjMatrix.h"
#include "DijkstraAdjMatrix.h"
#include "Floyd.h"
#include "KruskalAdjMatrix.h"
#include "PrimAdjMatrix.h"
#include "TopoSortAdjMatrix.h"

using namespace std;

int main(int argc, char* argv[])
{
	Graph G;
	G.VerNum=-1;    //标记图图尚未创建
	int vID;        //顶点的编号，从1开始。 
	elementType x;  //选择遍历的起始顶点

	int i;
	int topoList[MaxVerNum+1];  //保存拓扑序列的数组


	//控制主菜单
    int nChoice=-1;
	do
	{
		//显示主菜单
		cout<<"---图的邻接矩阵表示测试程序---"<<endl;
		cout<<"* 0-退出                     *"<<endl;
		cout<<"* 1-数据文件创建图           *"<<endl;
		cout<<"* 2-打印图                   *"<<endl;
		cout<<"* 3-连通图DFS                *"<<endl;
		cout<<"* 4-任意图DFS                *"<<endl;
		cout<<"* 5-连通图BFS                *"<<endl;
		cout<<"* 6-任意图BFS                *"<<endl;
		cout<<"* 7-Prim算法                 *"<<endl;
		cout<<"* 8-Kruskal算法              *"<<endl;
		cout<<"* 9-Dijkstra算法             *"<<endl;
		cout<<"* 10-Floyd算法               *"<<endl;
		cout<<"* 11-拓扑排序                *"<<endl;
		cout<<"* 12-关键路径                *"<<endl;
		cout<<"------------------------------"<<endl;
		cout<<"请输入数字选择操作（0-12）：";
		cin>>nChoice;

		if(cin.fail())
		{
			
			cin.clear();
			cin.ignore();
			cin.sync();
			
			system("cls");  //清除屏幕
			cout<<"错误：数据读入类型错误。"<<endl;
			nChoice=-1;
			continue;
		}

		switch(nChoice)
		{
		case 0:        //0-退出
			cout<<endl;
			cout<<"<--  程序退出  -->"<<endl<<endl;
			break;

		case 1:   //1-数据文件创建图
            system("cls");  //清除屏幕

			char strFileName[100];
			cout<<"请输入图数据文件名(i.e. Graph.grp)：";
			cin>>strFileName;  //控制台输入数据文件名			

			if(CreateGrpFromFile(strFileName,G))  //从文件输入数据元素，创建图			    
				printGraph(G);			
			break;

		case 2:  //2-打印图的相关信息
			system("cls");  //清除屏幕
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误： 图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}
			printGraph(G);  //打印图
			cout<<endl;
			break;

		case 3:  //3-连通图（分量）DFS
			system("cls");  //清除屏幕
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误： 图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}

			for(vID=1;vID<=MaxVerNum;vID++)  //访问标记数组初始化
				visited[vID]=false;
			
			cout<<"请输入DFS的起始顶点：";
			cin>>x;

			vID=LocateVertex(G,x);
			if(vID==-1)
			{
				cout<<endl<<"错误：选择遍历的起始顶点不在图上，遍历失败。"<<endl<<endl;
				break;
			}
			cout<<endl<<"深度优先遍历顶点序列："<<endl;
			DFS(G,vID);  //递归调用深度优先遍历算法，遍历连通图。或其中一个连通分量

			cout<<endl<<endl;
			break;

		case 4:     //4-任意图的（通用）深度优先遍历
			system("cls");  //清除屏幕
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误： 图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}
			cout<<"请输入DFS的起始顶点：";
			cin>>x;
			vID=LocateVertex(G,x);
			if(vID==-1)
			{
				cout<<endl<<"错误：选择遍历的起始顶点不在图上，遍历失败。"<<endl<<endl;
				break;
			}
			cout<<endl<<"深度优先遍历顶点序列："<<endl;
			DFSTraverse(G,vID);  //深度优先遍历图的各个连通分量
			cout<<endl<<endl;
			break;

		case 5:  //5-广度优先遍历连通图，或一个连通分量
			system("cls");  //清除屏幕
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误： 图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}			
			for(vID=1;vID<=MaxVerNum;vID++)  //访问标记数组初始化
				visited[vID]=false;
			
			cout<<"请输入BFS的起始顶点：";
			cin>>x;

			vID=LocateVertex(G,x);
			if(vID==-1)
			{
				cout<<endl<<"错误：选择遍历的起始顶点不在图上，遍历失败。"<<endl<<endl;
				break;
			}
			cout<<endl<<"广度优先遍历顶点序列："<<endl;
			BFS(G,vID);  //广度优先遍历连通图，或一个连通分量
			cout<<endl<<endl;
			break;

		case 6:  //6-广度优先遍历任意图--连通或非连通
			system("cls");  //清除屏幕
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误： 图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}
			cout<<"请输入BFS的起始顶点：";
			cin>>x;
			vID=LocateVertex(G,x);
			if(vID==-1)
			{
				cout<<endl<<"错误：选择遍历的起始顶点不在图上，遍历失败。"<<endl<<endl;
				break;
			}
			cout<<endl<<"广度优先遍历顶点序列："<<endl;
			BFSTraverse(G,vID);  //广度优先遍历图的各个连通分量
			cout<<endl<<endl;

			break;

		case 7:  //7-Prim算法
			system("cls");
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误：图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}

			cout<<"请输入Prim算法的起始顶点：";
			cin>>x;

			vID=LocateVertex(G, x);
			if(vID==-1)
			{
				cout<<endl<<"错误：选择遍历的起始顶点不在图上，算法失败。"<<endl<<endl;
				break;
			}

			//初始化已经选定顶点数组
			for(i=1;i<=G.VerNum;i++)
				inTree[i]=0;  //表示全部没有选定
			
			Prim(G,vID); //执行Prim算法，产生minEdges[]数组

			//PrimPrint(G, minEdges, vID);  //打印结果

			break;

		case 8:  //8-Kruskal算法
			system("cls");
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误：图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}
			Kruskal(G);
			disjointKruskal(G);   //使用并查集处理的Kruskal算法
			break;

		case 9:  //9-Dijkstra算法
			system("cls");  //清除屏幕
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误： 图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}
			cout<<"请输入 Dijkstra 算法的起始顶点：";
			cin>>x;
			vID=LocateVertex(G,x);
			if(vID==-1)
			{
				cout<<endl<<"错误：选择的起始顶点不在图上，搜索失败。"<<endl<<endl;
				break;
			}

			int path[MaxVerNum];
			int dist[MaxVerNum];
			Dijkstra(G, path, dist, vID);
			PrintDijkstra(G, path, dist, vID);  //打印最短路径
			break;

		case 10:  //10-Floyd算法
			system("cls");  //清除屏幕
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误： 图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}
			
			cellType dist1[MaxVerNum][MaxVerNum];  //二维距离矩阵
			int path1[MaxVerNum][MaxVerNum];       //二维路径矩阵  

			Floyd(G, dist1, path1);  //执行Floyd算法
			PrintFloyd(G, dist1, path1);
			break;

		case 11:  //11-拓扑排序
			system("cls");  //清除屏幕
			if(G.VerNum==-1)
			{
				cout<<endl<<"错误： 图尚未创建，请先创建图，再执行此操作。"<<endl<<endl;
				break;
			}

			//if(TopologicalSortS(G, topoList)) //调用拓扑排序算法--使用栈
			//if(TopologicalSortQ(G, topoList)) //调用拓扑排序算法--使用队列
			if(TopologicalSort(G, topoList)) //调用拓扑排序算法--不使用栈、队列，借助一个solved[]数组
			{
				cout<<endl;
				cout<<"本图拓扑排序成功。拓扑序列："<<endl;
				for(i=1;i<=G.VerNum;i++)  //以顶点编号输出拓扑序列
					cout<<topoList[i]<<"\t";
				cout<<endl<<"或："<<endl;
				for(i=1;i<=G.VerNum;i++)  //以顶点元素输出拓扑序列
					cout<<G.Data[topoList[i]]<<"\t";
				cout<<endl<<endl;
			}
			else
			{
				cout<<endl<<"本图具有回路，拓扑排序失败。"<<endl<<endl;
			}			  

			break;	

		default:
			cout<<"错误：操作选择错误，请重新选择！"<<endl;;
			break;
		}

	}while(nChoice!=0);

	return 0;
}
