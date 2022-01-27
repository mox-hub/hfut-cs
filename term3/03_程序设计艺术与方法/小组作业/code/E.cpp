#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>

using namespace std;
//set const MAX = 1000
const int MAX = 1000;
//point (x, y)
struct Point
{
	double x, y;
} point[MAX];

// set result record the solpe of line, the size of set is result
set<float> result; 
//get cross of three point
double cross(Point a, Point b, Point c)
{
	return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}
//get distance of two point
double dis(Point a, Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
//the cmp of sort
bool cmp(Point a, Point b)
{

	if (cross(point[0], a, b) == 0)
		return dis(a, point[0]) < dis(b, point[0]);
	else
		return cross(point[0], a, b) > 0;
}
// get slope of line
double slope(Point a, Point b)
{ 
    // slope is infinity
	if (a.x == b.x)
		return double(1e9);
    // slope is 0
	else if (a.y == b.y)
		return 0;
	else
		return (b.y - a.y) / (b.x - a.x);
}
// main start
int main()
{
    //get n ,the count of point
	int n;
	cin >> n;
    // set average point
	Point ave; 
	ave.x = 0;
	ave.y = 0;

    //get input point
	for (int i = 0; i < n; ++i)
	{
		cin >> point[i].x >> point[i].y;
		ave.x += point[i].x;
		ave.y += point[i].y;
	}
    //set average point
	ave.x /= n;
	ave.y /= n;
	
    //solve convex hill
	for (int i = 1; i < n; ++i)
	{
		//set point[0] is first point
		if ((point[i].y < point[0].y))
			swap(point[0], point[i]);
		else if (point[i].y == point[0].y && point[i].x < point[0].x)
			swap(point[i], point[0]);
	}
	
	sort(point + 1, point + n, cmp);

	//get edge point
	int tot = 1;
	for (int i = 2; i < n; ++i)
	{ //手写栈
		while (tot > 0 && cross(point[tot - 1], point[tot], point[i]) <= 0)
			tot--;
		tot++;
		point[tot] = point[i];
	}
	// convex hill's line average point
	Point temp; 
	
	bool flag = 0;
	int ans = 0;
	result.clear();
	//get all lines' average point
	for (int i = 0; i <= tot; ++i)
	{
		for (int j = i + 1; j <= tot; ++j)
		{
			temp.x = (point[i].x + point[j].x) / 2;
			temp.y = (point[i].y + point[j].y) / 2;
			//clear flag
			flag = 0;
			// is perpendicular pass through center point
			if (float(temp.x - ave.x) * (point[i].x - point[j].x) +
							float(temp.y - ave.y) * (point[i].y - point[j].y) == 0)
			{ 
				flag = 1;
			}

			if (flag)
			{
				// is coincide
				if (temp.x == ave.x && temp.y == ave.y)
				{
					result.insert(slope(point[i], point[j]));
				}
				else
				{
					result.insert(slope(temp, ave));
				}
			}
		}
	}
	//  set result is ans 
	ans = result.size();
	// set the other point in
	if (tot == 1)
		ans++;
	cout << ans << endl;
	return 0;
}
