//****************************************
//File Name:0503.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    int iNum[10], i, num = 0;

    printf("请输入十个整数：\n");
    for(i = 0;i < 10;i++)
    {
        scanf("%d", &iNum[i]);
    }
    for(i = 0;i < 10;i++)
    {
        if(iNum[i] % 2 == 0)
            num += 1;
    }
    printf("奇数有%d个\n", 10 - num);
    printf("偶数有%d个\n", num
           );
}
