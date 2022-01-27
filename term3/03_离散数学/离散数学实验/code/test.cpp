#include <iostream>

using namespace std;

#define MAX 100
#define MAXWEIGHT 100000

double graph[MAX][MAX];

double prim(double graph[][MAX], int n)
{
    double shortestPath[MAX], SpanningTree[MAX];
    int i, j, minID;
    double min, sum = 0.f;

    for(i = 2; i <= n; i++)
    {
        shortestPath[i] = graph[1][i];
        SpanningTree[i] = 1;
    }

    SpanningTree[1] = 0;

    for(i = 2; i <= n; i++)
    {
        minID = 0;
        min = MAXWEIGHT;
        // get min id
        for(j = 2; j <= n; j++)
        {
            if(shortestPath[j] < min && shortestPath[j] != 0)
            {
                min = shortestPath[j];
                minID = j;
            }
        }

        cout << "V" << SpanningTree[minID] << " --> V" << minID << " = " << min << endl;

        sum += min;
        shortestPath[minID] = 0;

        for(j = 2; j <= n; j++)
        {
            if(graph[minID][j] < shortestPath[j])
            {
                shortestPath[j] = graph[minID][j];
                SpanningTree[j] = minID;
            }
        }
    }
    return sum;
}

// main start
// TODO : here is start
int main( void ) 
{
    int i, j, m, n;
    double weight;
    double result;

    // get input
    cout<< "请输入节点数以及边数：";
    cin>> m >> n;

    // init with max weight == 100000
    for(i = 1; i <= m; i++) 
    {
        for (j = 1; j <= m; j++) 
        {
            graph[i][j] = MAXWEIGHT;
        }
    }
// TODO :
    cout<< "请输入两节点及之间的权值：" <<endl;

    for(int k = 1; k <= n; k++)
    {
        cin>> i >> j >> weight;
        graph[i][j] = weight;
        graph[j][i] = weight;
    }
    //start prim func
    result = prim(graph, m);
    cout << "最短路径的长度为：" << result << " km" << endl;

    return 0;
}

