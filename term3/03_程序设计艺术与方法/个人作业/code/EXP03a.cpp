#include<stdio.h>
#include<string.h>
#include<vector>
#include<iostream>
#include<math.h>
#include<algorithm>

using namespace std;

const int maxn = 1100;
const double esp = 1e-10;

// define struct point (x, y)
struct point {
    double x, y;
} myPoint[maxn];

//define myline of two point
struct line {
    point head, tail;
}myline[maxn];

double dmin(double a, double b) {
    return a < b ? a : b;
}
double dmax(double a, double b) {
    return a > b ? a : b;
}

//check is cross
bool cross(const line& line1, const line& line2) 
{
    // save point
    point a = line1.head;
    point b = line1.tail;
    point c = line2.head;
    point d = line2.tail;

    if (dmin(a.x, b.x) > dmax(c.x, d.x) ||
        dmin(a.y, b.y) > dmax(c.y, d.y) ||
        dmin(c.x, d.x) > dmax(a.x, b.x) ||
        dmin(c.y, d.y) > dmax(a.y, b.y))
            return 0;

    double h, i, j, k;
    h = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    i = (b.x - a.x) * (d.y - a.y) - (b.y - a.y) * (d.x - a.x);
    j = (d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x);
    k = (d.x - c.x) * (b.y - c.y) - (d.y - c.y) * (b.x - c.x);
    return (h * i <= esp && j * k <= esp);
}

int main() {
    int n = 0;
    int cas = 1;
    while (scanf("%d", &n) != EOF) 
    {
        if (n == 0) break;
        if (n < 3) 
        {
            printf("Figure %d: Impossible\n", cas++);
            continue;
        }

        if (cas != 1)
            puts("");
        // get input point x, y
        for (int i = 0; i < n; i++) 
        {
            cin >> myPoint[i].x >> myPoint[i].y;
        }
        
        // get the line of every side two point
        for (int i = 0; i < n; i++) 
        {
            point u, v;
            if (i == (n - 1)) 
            {
                u.x = myPoint[i].x;
                u.y = myPoint[i].y;
                v.x = myPoint[0].x;
                v.y = myPoint[0].y;
            }
            else 
            {
                u.x = myPoint[i].x;
                u.y = myPoint[i].y;
                v.x = myPoint[i + 1].x;
                v.y = myPoint[i + 1].y;
            }
            myline[i].head = u;
            myline[i].tail = v;
        }
        bool flag = false;
        bool tmp = false;

        for (int i = 0; i < n; i++) 
        {
            for (int j = i + 2; j < n; j++) {
                if (i == 0 && (j == (n - 1)))
                    continue;
                tmp = cross(myline[i], myline[j]);
                if (tmp) {
                    flag = true;
                    break;
                }
            }
            if (flag)
                break;
        }
        if (flag) 
        {
            printf("Figure %d: Impossible\n", cas++);
        }
        // get the area
        else 
        {
            double area = 0;
            double x1, x2, y1, y2, x0, y0;
            x0 = x1 = myPoint[0].x;
            y0 = y1 = myPoint[0].y;
            for (int i = 1; i <= n; i++)
            {
                if (i < n) 
                {
                    x2 = myPoint[i].x;
                    y2 = myPoint[i].y;
                }
                else
                {
                    x2 = x0;
                    y2 = y0;
                }
                area += (y1 + y2) * (x2 - x1) * 0.5;
                x1 = x2;
                y1 = y2;
            }
            if (area < 0)
                area *= -1;
            printf("Figure %d: %.2f\n", cas++, area);
        }
    }
    return 0;
}

