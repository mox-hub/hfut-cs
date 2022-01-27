#include <iostream>
#include <cstring>
#include <vector>

using namespace std;
typedef struct pNode
{
	int level;    		//tech level
	int time;     		//requare time 
	int super;       //superior
} PTNode;

typedef struct pTree
{
	PTNode node[100];   //node array
	int n;              //node count
}pTree; //tree

//find first child of node of v
int firstChild(pTree &T,int v)    
{
	int w;
	if(v == -1)
		return -1;

	for(w = 0;w < T.n; w++)
	{
		if(T.node[w].super == v)
			return w;
	}
	return -1;
}

//find second child of node of v
int nextSibling(pTree &T,int v,int w)
{
	int i;
	for(i = w + 1; i < T.n; i++)
		if(T.node[i].super == v)
			return i;
	return -1;
}

//traverse all child, and add low level of time into sum
void Traverse(pTree T,int v,int v2,int *sum)
{
	int w = firstChild(T, v);

	while(w != -1)    
    {
        if(T.node[w].level < T.node[v2].level)
        {
            *sum += T.node[w].time;
        }
		Traverse(T, w, v2, sum);
		w = nextSibling(T, v, w); //搜索下一子结点
	}
}

int main()
{
	//define variate
    int n;
    pTree T;
	int mi, ri, ti;

	// get input
	T.n = 0;
    cin >> n;
    for(int i = 0; i < n; i++) 
    {
        cin >> mi >> ri >> ti;
        T.n++;
        T.node[i].time = ti;
        T.node[i].level = ri;
        T.node[i].super = mi - 1;
    }
	//get time result
    for(int i = 0; i < n; i++)  
    {
        int sum = 0;
		Traverse(T, i, i, &sum);
		cout << sum << endl;
	}
	
	return 0;
}
