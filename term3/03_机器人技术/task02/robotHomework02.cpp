/**
 * @author 
 * @date 2020_10_3
 * @version 1.0
 * @property it is the second program of robot homework
 */
//头文件引用
#include <iostream>
#include <math.h>
//使用标准命名空间
using namespace std;
//定义各种几何类
class point 
{
    public:
        double x;
        double y;
    public:
        void setXY(double in_x, double in_y) 
        {
            x = in_x;
            y = in_y;
        
        }
        point() {}
        point(double in_x, double in_y)
        {
            setXY(in_x, in_y);
        }
};

class straightLine
{
    public:
        double a;
        double b;
        double c; 
    public:
    //建立标准直线方程
    straightLine(double in_a, double in_b, double in_c)
    {
        a = in_a;
        b = in_b;
        c = in_c;
    }
    //两点式建立标准式直线a = y0 – y1, b = x1 – x0, c = x0y1 – x1y0。
    straightLine(point p1,point p2)
    {
        a = p2.y - p1.y;
        b = p2.x - p1.x;
        c = p1.x * p2.y - p2.x * p1.y;
    }

    double getValueOfY(double x)
    {
        return -(a * x + c) / b;
    }
    double getValueOfX(double y)
    {
        return -(b * y + c) / a;
    }
};

class circle
{
    public:
        point center;
        double r;
    public:
    void setCircle(point pos, double in_r)
    {
        center = pos;
        r = in_r;
    }
        circle() {}
        circle(point pos, double in_r)
        {
            setCircle(pos, in_r);
        }
};

class rectangle
{
    public:
        point a;
        point b;
    public:
        rectangle() {}
        rectangle(point in_a, point in_b)
        {
            a = in_a;
            b = in_b;
        }

};
void getCrossoverPoint(straightLine line1, straightLine line2)
{
    double D = (line1.a * line2.b) - (line2.a * line1.b);
    point result;
    if(D == 0)
    {
        cout << "两条直线没有交点" << endl;
    }
    else 
    {
        result.x = (line1.b * line2.c - line2.b * line1.c) / D;
        result.y = (line1.c * line2.a - line2.c * line1.a) / D;
        cout << "交点坐标为：( " << result.x << " , " << result.y << " )" << endl;
    }
}

void getCrossoverPoint(straightLine line1, circle c)
{
	double splot = -(line1.a / line1.b);
	double intercept = -(line1.c / line1.b);
	double x0 = c.center.x;
	double y0 = c.center.y;
    double r = c.r;
    double delta = (pow(splot, 2) + 1) * pow(r, 2) - pow(x0, 2) * pow(splot, 2)
			+ 2 * splot * x0 * (intercept + y0) - pow(y0, 2) - 2 * y0 * intercept 
			- pow(intercept, 2);
        
	if (delta > 0) 
        {
			double x1 = (-((y0 + intercept) * splot + x0) - sqrt(delta)) / (1 + pow(splot, 2));
			double y1 = line1.getValueOfY(x1);
            cout << "直线和圆的第一个交点为：(" << x1 << "," << y1 << ")" << endl;

			double x2 = (-((y0 + intercept) * splot + x0) + sqrt(delta)) / (1 + pow(splot, 2));
			double y2 = line1.getValueOfY(x2);
			cout << "直线和圆的第二个交点为：(" << x2 << "," << y2 << ")" << endl;
		}
    if (delta == 0) 
        {
            double x1 = (-((y0 + intercept) * splot + x0) - sqrt(delta)) / (1 + pow(splot, 2));
			double y1 = line1.getValueOfY(x1);
            cout << "直线和圆的交点为：(" << x1 << "," << y1 << ")" << endl;
        }
	if(delta < 0) 
    {
        cout << "直线和圆没有交点" << endl;
    }
}

void getCrossoverPoint(straightLine line1,rectangle rect)
{
    double t;
    double a = line1.a;
    double b = line1.b;
    double c = line1.c;
    /*
    p3 ----- p2
    | ------- |
    | ------- |
    p1 ----- p4
    */
    point p1 = rect.a;
    point p2 = rect.b;
    point p3(p1.x, p2.y);
    point p4(p2.x, p1.y);

    if (a == 0)
    {
        t = -(c / b);
        if (t > p1.y && t < p2.y)
        {
            cout << "直线与矩形的两个交点为：(" << p1.x << "," << t << ")和(" << p2.x << "," << t << ")" << endl;
        }
        else
        {
            cout << "直线和矩形没有交点" << endl;
        }
    }
    else if (b == 0)
    {
        t = -(c / a);
        if (t > p1.x && t < p2.x)
        {
            cout << "直线与矩形的两个交点为：(" << t << "," << p1.y << ")和(" << t << "," << p2.y << ")" << endl;
        }
        else
        {
            cout << "直线和矩形没有交点" << endl;
        }
    }
    else
    {
        cout << "直线和矩形的交点为：" << endl;
        //p1p3
        if(line1.getValueOfY(p1.x) >= p1.y && line1.getValueOfY(p1.x) <= p3.y)
        {
            straightLine line2(1, 0, -p1.x);
            getCrossoverPoint(line1, line2);
        }
        //p2p4
        if(line1.getValueOfY(p2.x) >= p4.y && line1.getValueOfY(p1.x) <= p2.y)
        {
            straightLine line2(1, 0, -p2.x);
            getCrossoverPoint(line1, line2);
        }
        //p2p3
        if(line1.getValueOfX(p2.x) >= p3.x && line1.getValueOfX(p1.x) <= p2.x)
        {
            straightLine line2(0, 1, -p2.y);
            getCrossoverPoint(line1, line2);
        }
        //p1p4
        if(line1.getValueOfX(p1.x) >= p1.x && line1.getValueOfX(p1.x) <= p4.x)
        {
            straightLine line2(0, 1,-p1.y);
            getCrossoverPoint(line1, line2);
        }
    }
    
}

int main () 
{
    //定义几何体
    double r = 1.0f;
    point center(0, 0);
    point a(2.0, 3.0), b(3.0, 4.0);
    straightLine line1(1.0,-1.0,1.0), line2(1.0f,1.0f,-1.0f);
    circle circle1(center, r);
    rectangle rect(a, b);
    //测试部分
    getCrossoverPoint(line1, line2);
    getCrossoverPoint(line1, circle1);
    getCrossoverPoint(line1, rect);
    return 0;
}