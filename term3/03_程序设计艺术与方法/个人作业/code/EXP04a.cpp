#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int minDist(string A, string B)
{
    int aLen = A.length();
    int bLen = B.length();
    int D[bLen + 1][aLen + 1];
    for (int j = 0; j <= aLen; j++) {
        //初始化第一行
        D[0][j] = j;
    }
    for (int i = 0; i <= bLen; i++) {
        //初始化第一列
        D[i][0] = i;
    }

    for (int i = 1; i <= bLen; i++) {
        for (int j = 1; j <= aLen; j++) {
            //初始化表格
            D[i][j] = min(min(D[i - 1][j] + 1, D[i][j - 1] + 1), (A[j - 1] == B[i - 1] ? D[i - 1][j - 1] : D[i - 1][j - 1] + 1));
        }
    }

    return D[bLen][aLen];
}

int main()
{
    string a, b;
    cout << "请输入字符串A :" << endl;
    cin >> a;
    cout << "\n请输入字字符串B :" << endl;
    cin >> b;
    cout << "\n最少次数："<<minDist(a, b)<<endl;
    return 0;
}

