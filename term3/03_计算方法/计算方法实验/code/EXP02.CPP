/**
 * @author: Li Haporan 
 * @date: 2020.10.20
 * @details: 实验二,数值积分.{1.包括复化梯形公式2.复化辛普森公式3.龙贝格算法}
*/
#include <iostream>
#include <math.h>


using namespace std;

double fx(double x)
{
	if(x == 0)
		return 1;
    double y;
    y = sin(x) / x;
    return y;
}
//1.包括复化梯形公式
double Trapezoid(double a, double b,double epsilon)
{
    double h = b - a;   //复化梯形公式的系数
    double T1 = ( h / 2 ) * (fx(a) + fx(b)), T2;
    double T;
    double s;
    double x;
    // cout << T1 << endl;
    while (fabs(T2 - T) < epsilon)
    {   
    	// cout << "yes" << endl;
        s = 0;
        x = a + ( h / 2.0 );
        if (x <= b)
        {
            s = s + fx(x);
            x = x + h;
        }
        // cout << s << endl;
        T2 = ( T1 / 2.0) + ( h / 2.0 ) * s;

        T = T1;
        T1 = T2;
    }
    return T2;
}
double Trapezoid(double a, double b,int k)
{
    double h = b - a;   //复化梯形公式的系数
    double T1 = ( h / 2 ) * (fx(a) + fx(b)), T2;
    double T;
    double s;
    double x;
    // cout << T1 << endl;
    for(int i = 0;i < k; i++)
    {   
    	// cout << "yes" << endl;
        s = 0;
        x = a + ( h / 2.0 );
        if (x <= b)
        {
            s = s + fx(x);
            x = x + h;
        }
        // cout << s << endl;
        T2 = ( T1 / 2.0) + ( h / 2.0 ) * s;

        T = T1;
        T1 = T2;
    }
    return T2;
}
//2.复化辛普森公式
double Simpson(double a, double b,double epsilon)
{
    //Page 65 2-1
    double h = b - a;   //复化梯形公式的系数
    double T1 = ( h / 6.0 ) * (fx(a) + fx(b));
    double T;
    double T2;
    double s;
    double x;
    while (fabs(T2 - T) < epsilon)
    {   
        // cout << "yes" << endl;
        s = 0;
        x = a + ( h / 2.0 );
        if (x <= b)
        {
            s = s + 2 * fx(x);
            x = x + (h / 2);
            s = s + 4 * fx(x);
            x = x + (h / 2);
        }

        T2 = ( T1 / 6.0 ) + ( h / 6.0 ) * s;
        T = T1;
        T1 = T2;
    }
    return T2;

}
//3.龙贝格算法
double Romberg(double a, double b,double epsilon)
{
    double h = b - a;
    double T1 = ( h / 2 ) * (fx(a) + fx(b)),T2;
    double S1, S2;
    double C1, C2;
    double R1, R2;
    int k = 1;
    cout << "0" << "\t" << T1 << "\t";
    while (fabs(R2 - R1) < epsilon)
    {   
        T2 = ( h / 2 ) * (fx(a) + fx(b));
        cout << endl;
        cout << k << "\t";
        C1 = C2; 
        R1 = R2;
        T2 = Trapezoid(a, b, k);
        cout << T2 << "\t";
        S2 = (4 * T2 - T1) / 3;
        cout << S2 << "\t";
        if (k == 1)
        {
            k = k + 1;
            h = h / 2;
            T1 = T2;
            S1 = S2;
            continue;
        }
        C2 = (16 * S2 - S1) / 15;
        cout << C2 << "\t";
        if (k == 2)
        {
            k = k + 1;
            h = h / 2;
            T1 = T2;
            S1 = S2;
            C1 = C2;
            continue;
        }
        R2 = (64 * C2 - C1) / 63;
        cout << R2 << "\t";
        if (k == 3)
        {
            k = k + 1;
            h = h / 2;
            T1 = T2;
            S1 = S2;
            C1 = C2; 
            R1 = R2;
            continue;
        }
    }
    cout << endl;

    return R2;
}

int main (void)
{
    // cout << "start" << endl;
    cout << "Trapezoid = "<<Trapezoid(0.0,1.0,1e-6) << endl;
    cout << "Simpson = "<<Simpson(0.0,1.0,1e-6) << endl;
    cout << "Romberg = "<<Romberg(0.0,1.0,1e-6) << endl;
    // cout << "Trapezoid = "<<Trapezoid(0.0,1.0,16) << endl;   
    return 0;

}
