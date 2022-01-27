#include <iostream>
#include <cstring>
using namespace std;
int main()
{
    int a, b;
    cout << "请输入矩阵的行数和列数:" << endl;
    cin >> a >> b;
    int d[a][b];
    cout << "请输入矩阵:" << endl;
    for(int i = 0; i < a; i++){
        for(int j = 0; j < b; j++){
            cin >> d[i][j];
        }
    }
    int sum, tmp;
    int c[b];
    sum = -999;
    for(int i = 0; i < a; i++){
        memset(c, 0 ,sizeof(c));
        for(int j = i; j < a; j++){
            tmp = 0;
            for(int k = 0; k < b; k++){
                c[k] += d[j][k];
                if(tmp >= 0)
                    tmp += c[k];
                else
                    tmp = c[k];
                if(sum < tmp)
                    sum = tmp;
            }
        }
    }
    cout << "矩阵最大的子矩阵和为:" << sum;
}

