//****************************************
//File Name:06
//Author:Li Haoran
//Date:20199.11.14
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    float a[3][4], s[3][2] = {0}, *p[3];
    int i, j;

    for(i = 0;i < 3;i++)
    {
        p[i] = a[i];
    }
    printf("请输入3行4列的数组:\n");
    for(i = 0;i < 3;i++)
    {
        for(j = 0;j < 4;j++)
        {
            scanf("%f", &a[i][j]);
        }
    }
    for(i = 0;i < 3;i ++)
    {
        for(j = 0;*(p[i]+j);j++)
        {
            s[i][0] += *(p[i]+j);
        }
        s[i][0] /= 4;
    }
    for(i = 0;i < 3;i ++)
    {
        for(j = 0;*(p[i]+j);j++)
        {
            s[i][1] += (*(p[i]+j) -s[i][0])*(*(p[i]+j) -s[i][0]);
        }
        s[i][1] /= 4;
    }

    for(i = 0;i < 3;i++)
    {
        for(j = 0;j < 2;j++)
        {
            printf("%f\t", s[i][j]);
        }
        printf("\n");
    }

    return 0;
}
