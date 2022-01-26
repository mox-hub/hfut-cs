//****************************************
//File Name:0606
//Author:Li Haoran
//Date:20199.11.14
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    int a[4][4], b[4][4], *p[4];
    int i, j;

    printf("请输入第一个数组:\n");
    for(i = 0;i < 4;i++)
    {
        for(j = 0;j < 4;j++)
        {
            scanf("%d", &a[i][j]);
        }
        printf("\n");
    }


    for(i = 0;i < 4;i++)
    {
        for(j = 0;j < 4;j++)
        {
            b[i][j] = a[j][i];
        }
    }

    for(i = 0;i < 4;i++)
    {
        for(j = 0;j < 4;j++)
        {
            printf("%d\t",b[i][j]);
        }
        printf("\n");
    }
    return 0;
}
