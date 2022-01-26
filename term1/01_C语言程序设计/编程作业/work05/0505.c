//****************************************
//File Name:0505.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    int iNum[10], i, j;
    int temp;

    printf("请输入十个整形数据:\n");
    for(i = 0;i < 10;i++)
    {
        scanf("%d", &iNum[i]);
    }
    for(i = 0;i < 10;i++)
    {
        for(j = 0;j < 10 - i - 1;j++)
        {
            if(iNum[j] > iNum[j+1])
            {
                temp = iNum[j+1];
                iNum[j+1] = iNum[j];
                iNum[j] = temp;
            }
        }
    }

    printf("排序后的结果为：\n");
    for(i = 0;i < 10;i++)
    {
        printf("%d\n", iNum[i]);
    }

}
