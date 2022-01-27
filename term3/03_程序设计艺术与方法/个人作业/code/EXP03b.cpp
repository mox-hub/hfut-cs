#include<stdio.h>
#include<string.h>
#include<math.h>
#include<algorithm>

#define eps 10e-8
#define inf 1<<29

using namespace std;

struct point
{
    double x, y;
};

int n;
double xx[22], yy[22][5];
point p[90];
double adj[90][90];
int psize;
double ff(double x)
{
    return x * x;
}
double dis(point p1, point p2)
{
    return sqrt(ff(p1.x - p2.x) + ff(p1.y - p2.y));
}
double cross(double x1, double y1, double x2, double y2, double x3, double y3)
{
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}
bool judge(point p1, point p2)
{
    if (p1.x >= p2.x)return 0;
    int i = 0;
    while (xx[i] <= p1.x && i < n)i++;
    while (xx[i] < p2.x && i < n)
    {
        if (cross(p1.x, p1.y, p2.x, p2.y, xx[i], 0) * cross(p1.x, p1.y, p2.x, p2.y, xx[i], yy[i][1]) < 0
            || cross(p1.x, p1.y, p2.x, p2.y, xx[i], yy[i][2]) * cross(p1.x, p1.y, p2.x, p2.y, xx[i], yy[i][3]) < 0
            || cross(p1.x, p1.y, p2.x, p2.y, xx[i], yy[i][4]) * cross(p1.x, p1.y, p2.x, p2.y, xx[i], 10) < 0)
            return 0;
        i++;
    }
    return 1;
}
void floyd()
{
    int i, j, k;
    for (k = 0; k < psize; k++)
        for (i = 0; i < psize; i++)
            for (j = i + 1; j < psize; j++)
                if (i != k && j != k)
                    adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
    printf("最短距离为：%.2f\n", adj[0][psize - 1]);
}
int main()
{
    int i, j;
    printf("输入墙的个数：\n");
    scanf("%d", &n);
    printf("分别输入墙的x坐标和门的y坐标\n");
        p[0].x = 0; p[0].y = 5; psize = 1;
       for (i = 0; i < n; i++)
        {
            scanf("%lf", &xx[i]);
            for (j = 1; j <= 4; j++)
            {
                p[psize].x = xx[i];
                scanf("%lf", &p[psize].y);
                yy[i][j] = p[psize].y;
                psize++;
            }
        }
        p[psize].x = 10; p[psize].y = 5;
        psize++;
        for (i = 0; i < psize; i++)
            for (j = 0; j < psize; j++)
                adj[i][j] = inf;
        for (i = 0; i < psize; i++)
            for (j = i + 1; j < psize; j++)
                if (judge(p[i], p[j]))
                    adj[i][j] = dis(p[i], p[j]);
        floyd();  
    return 0;
}

