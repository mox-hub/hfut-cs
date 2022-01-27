#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

//set const 
const int maxN = 10005;
const int maxM = 50005;
const int inf = 0x3fffffff;
// the number of edge
int m;
// the number of node
int n;

struct edge 
{
	// define next and to
	int amap;
	int heromap;
	int next;
	int to;	
};

struct new_edge
{
	int to;
	int dis;
}; 

struct node
{
	int dis;
	int pos;
	// overload operator
	bool operator < (const node &x) const
	{
		return x.dis < dis;
	}
};

edge e[maxM]; //edge
vector<new_edge> v[maxN]; //new edge
priority_queue<node>q;


int cnt;  //count
int head[maxN], d1[maxN], d2[maxN], d3[maxN]; //distance
bool vis[maxN];// isVisited

// inline func to improve speed 
inline void add_edge(int u,int v,int w,int x)
{
	// add new adge
	cnt++;
	e[cnt].to = v;
	e[cnt].heromap = w;
	e[cnt].amap = x;
	e[cnt].next = head[u];
	head[u] = cnt;
} 
// heromap
void dijkstra1(int s)
{
	// initialized
	fill(d1, d1 + maxN, inf);
	// set beginning node
	d1[s] = 0;
	q.push((node){0, s});
	// start dijkstra
	while(!q.empty())
	{
		node tmp = q.top();
		q.pop();

		// set node is visited
		int x = tmp.pos;
		vis[x] = true;

		//record heromap's shortest path
		for(int i=head[x];i;i=e[i].next)
		{
			int y = e[i].to;
			if(d1[y] > d1[x]+e[i].heromap)
			{
				d1[y] = d1[x]+e[i].heromap;
				if( !vis[y] )
				{
					q.push((node){d1[y], y});
				}		
			}
		}		
	}
}
// amap
void dijkstra2(int s)
{
	// initialized
	fill(vis, vis + maxN, false);
	fill(d2, d2 + maxN, inf);
	// set beginning node
	d2[s] = 0;
	q.push((node){0, s});
	// start dijkstra
	while(!q.empty())
	{
		node tmp = q.top();
		q.pop();

		int x = tmp.pos;
		vis[x] = true;

		for(int i = head[x]; i; i = e[i].next)
		{
			int y = e[i].to;
			if(d2[y] > d2[x]+e[i].amap)
			{
				d2[y] = d2[x]+e[i].amap;
				if( !vis[y] )
				{
					q.push((node){d2[y], y});
				}
			}
		}
	}		
}
// compare
void dijkstra3(int s)
{
	// initialized
	fill(vis, vis + maxN, false);
	fill(d3, d3 + maxN, inf);
	// set beginning node
	d3[s] = 0;
	q.push((node){0, s});
	// start dijkstra
	while(!q.empty())
	{
		node tmp = q.top();
		q.pop();
		// node pos
		int x = tmp.pos;
		vis[x] = true;

		for(int i = 0; i < (int)v[x].size(); i++)
		{ 
			new_edge tmp2 = v[x][i];
			int y = tmp2.to;	// node to
			if(d3[y] > d3[x] + tmp2.dis)
			{
				d3[y] = d3[x] + tmp2.dis;
				if(!vis[y])
				{
					q.push((node){d3[y], y});
				}
			}
		}
	}		
}
//main start
int main()
{
	// get input
	cin >> n >> m;
	for(int i = 1;i <= m; i++)
	{
		int u, v, w, x;
		cin >> u >> v >> w >> x;
		add_edge(v, u, w, x);
	}

	//get shortest path of amap and heromap
	dijkstra1(n);
	dijkstra2(n);
	// traverse node
    for(int i = 1; i <= n; i++)
	{
		// traverse edge
    	for(int j = head[i]; j; j = e[j].next)
		{
    	    new_edge tmp;
			//set distance
			tmp.dis = 0;
			if(d1[i]+e[j].heromap > d1[e[j].to]) tmp.dis++;
			if(d2[i]+e[j].amap > d2[e[j].to]) tmp.dis++;
			// set to
			tmp.to = i;
			// add new edge of node x 
			v[e[j].to].push_back(tmp);
		} 
	}

    dijkstra3(1);
	cout << d3[n] << endl;

	return 0;
}