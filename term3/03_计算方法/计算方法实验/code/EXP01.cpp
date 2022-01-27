#include <iostream>

using namespace std;

double getY(double x)
{
	return (1 / (1 + x * x));
}
double Lagrange(int n,double x0,double length)
{
	int i, j, k;
	double t = 0;
	double y0 = 0;
	double x[n+1], y[n+1];
	
	for (i = 0; i <= n; i++)
	{
		x[i] = -5.0 + i * (length / 10.0);
		y[i] = getY(x[i]);
		//cout << "("<<x[i]<<","<<y[i]<<")"<<endl;
	}	
	for (k = 0; k <= n; k++)
	{
		t = 1.0;
		for (j = 0; j <= n; j++)
		{
			if(j == k) continue; 
			t = t * ((x0 - x[j]) / (x[k] - x[j]));
		//	cout <<"t = "<< t<<endl;
		}
		
		y0 = y0 + t * y[k];
	}
	return y0;
}

double Piecewise(int n,double x0,double length)
{
	int i, k;
	double t = 0;
	double y0 = 0;
	double x[n+1], y[n+1];
	
	for (i = 0; i <= n; i++)
	{
		x[i] = -5.0 + i * (length / 10.0);
		y[i] = getY(x[i]);
		//cout << "("<<x[i]<<","<<y[i]<<")"<<endl;
	}	
	for(i = 0;i < n; i++)
	{
		if(x[i+1] >= x0)
		{
			k = i;
			break;	
		}
	}
//	cout << x[i] << endl;
	y0 = y[k]*(x0-x[k+1])/(x[k]-x[k+1])	+y[k+1]*(x0-x[k])/(x[k+1]-x[k]);
	 
	return y0;
}

void Newton()
{
	int i;
	double x0,y0 = 0;
	double x[3],y[3];
	double f0;
	double f1[2];
	double f2;
	cout << "请输入3组点:" <<endl;
	for(i = 0; i < 3;i++)
	{
		cout <<"x:";
		cin >> x[i];
		cout <<"y:";
		cin >> y[i];	
	} 
	cout << "请输入预测点x:" <<endl;
	cin >> x0; 
//	for(i = 0; i < 3;i++)
//	{
//		cout << x[i]<<","<< y[i]<<endl;
//	} 
	f0 = y[0];
	f1[0] = (y[1]-y[0]) / (x[1]-x[0]);	
 	f1[1] = (y[2]-y[1]) / (x[2]-x[1]);	
	f2 = (f1[0] - f1[1])/ (x[0]-x[2]);
	y0 = f0 + f1[0]*(x0-x[0]) +f2 *(x0-x[0])*(x0-x[1]);
	
	cout << "第二题Newton插值：" << endl;
	cout <<"根号下"<<x0<<"的估计值为："<<y0<<endl; 
}
int main(void)
{
	double x0[2],y0[2];
	double L[2],P[2];
	double length;
	double t = 0;
	int n, i;
	
	cout << "please input n:" << endl;
	cin >> n;
	cout << "please input first x0:" << endl;
	cin >> x0[0];
	cout << "please input second x0:" << endl;
	cin >> x0[1];
	cout << "please input length:" << endl;
	cin >> length;	
	
	for(i = 0;i < 2; i++)
	{
		L[i] = Lagrange(n,x0[i],length);	
	} 
	for(i = 0;i < 2; i++)
	{
		P[i] = Piecewise(n,x0[i],length);	
	} 
	
	cout << "第一题插值：" << endl;
	cout << "X\tY(精确)\ty(拉格朗日)\ty(分段线性)\t误差(拉)\t误差(分)" << endl;
	for(i = 0;i < 2; i++)
	{
		y0[i] = getY(x0[i]);
		cout <<x0[i]<<"\t"<<y0[i]<<"\t"<<L[i]<<"\t"<<P[i]<<"\t"<<y0[i]-L[i]<<"\t"<<y0[i]-P[i]<<endl;	
	} 
	
	system("pause");
	Newton(); 
	return 0;
}
