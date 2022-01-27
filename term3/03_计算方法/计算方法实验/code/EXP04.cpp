/**
 * @author: Li Haporan 
 * @date: 2020.10.20
 * @details: 实验四,非线性方程求根迭代
 */

#include <iostream>
#include <math.h>

using namespace std;

//默认函数的求值函数6
double f(double x)
{
    return (x * x * x) - x - 1;
}

//默认函数的一次导数函数
double df(double x) 
{
    return 3 * (x * x) - 1;
}

//牛顿下山法
void NewtonDownhill(double x0, double e, int N, int M)
{
    //定义参数
    int k = 0;
    double x1;
    double lamda = 1.0;
    bool flag = false;

// 循环开始牛顿下山法
    while ( true )
    {
        // 输出下山因子
        if(lamda != 1)
        {
            cout << "第" << k << "步下山因子：" << lamda << endl;
        }
        // 下山因子初始化为1.0
        lamda = 1.0;

        // 因为导数值为0结束为奇异标志
        if ( df(x0) == 0 )
        {   
            cout << "奇异标志" << endl;
        }
        // 否则进行下山迭代
        else
        {
            while ( true )
            {
                if(flag == true)
                {
                    flag = false;
                    break;
                }
                x1 = x0 - lamda * (f(x0) / df(x0));

                // 如果满足飞条件,说明下山未结束
                if ( fabs( f(x1) ) > fabs( f(x0) ) )
                {
                    cout << "第" << k << "步下山因子为：" << lamda << endl;

                    k ++;
                    lamda = lamda * 0.5;
                    // 下山次数过多的情况
                    if ( k >= M)
                    {
                        cout << "下山次数过多，请重新输入x0" << endl;
                        exit(0);
                    }
                    else continue;
                 
                }
                // 下山结束后,进行输出
                else
                {
                    if (fabs(x1 -x0) < e)
                    {
                        cout << "近似根为：" << x1 << endl;
                        exit(0);
                    }
                    else
                    {
                        k = k + 1;
                        x0 = x1;
                        if (k >= N)
                        {
                            cout<<"迭代失败！"<<endl;
                            exit(0);
                        }
                        else
                        {
                            flag = true;
                            continue;
                        }    
                    }
                }           
            }
        }
    }
}

int main (void) 
{
    /**
     * @param x0 初值
     * @param e 误差限
     * @param N 迭代最大次数
     * @param M 下山最大次数
     */
    double x0;
    double e;
    int N;
    int M;

    //输入四项参数
    cout << "请输入初值:" << endl;
    cin >> x0;
    cout<<"请输入误差限:"<<endl;
    cin >> e;
    cout << "请输入下山最大次数:" << endl;
    cin >> M;
    cout << "请输入迭代最大次数:" << endl;
    cin >> N;

    //进行牛顿下山法
    NewtonDownhill(x0, e, N, M);
    
    return 0;
}
