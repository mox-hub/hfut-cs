//****************************************
//File Name:0501
//Author:Li Haoran
//Date:20199.11.8
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    int iNum[10];
    int iMax = 0, i, j;

    printf("Please input 10 number:\n");
    for(i = 0;i < 10;i++)
    {
        scanf("%d", &iNum[i]);
    }
    for(i = 0;i < 10;i++)
    {
        if(iNum[i] >= iMax)
        {
            iMax = iNum[i];
            j = i;
        }
    }
    printf("最大值为:%d\n", iMax);
    printf("它是第%d个数", j);
}

