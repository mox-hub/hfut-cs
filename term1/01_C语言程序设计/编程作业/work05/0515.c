//****************************************
//File Name:0515.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    int iNum[5][5] = {17, 24, 1, 8, 15, 23, 5, 7, 14, 16, 4, 6, 13, 20, 22, 10, 12, 19, 21, 3, 11, 18, 25, 2, 9};
    int i, j, a = 0, b = 0, c = 0;
    int iFlag = 1;

    for(i = 0;i < 5;i++)
    {
        a += iNum[0][i];
    }
    for(i = 1;i < 5;i++)
    {
        for(j = 0;j < 5;j++)
        {
            b += iNum[i][j];
        }
        if(a != b)
        {
            printf("该矩阵不是魔方阵！");
            iFlag = 0;
            break;
        }
        b = 0;
    }
    b = 0;
    for(j = 0;j < 5;j++)
    {
        for(i = 0;i < 5;i++)
        {
            b += iNum[i][j];
        }
        if(a != b)
        {
            printf("该矩阵不是魔方阵！");
            iFlag = 0;
            break;
        }
        b = 0;
    }
    for(i = 0;i < 5;i++)
    {
        b += iNum[i][i];
        c += iNum[i][4-i];
    }
    if(b != a||c != a)
    {
        printf("该矩阵不是魔方阵！");
        iFlag = 0;
    }
    if(iFlag == 1)
    {
        printf("该矩阵是魔方阵！");
    }

    return 0;

}
