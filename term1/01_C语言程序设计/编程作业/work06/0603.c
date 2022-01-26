//****************************************
//File Name:06
//Author:Li Haoran
//Date:20199.11.14
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<string.h>

int main(void)
{
    char a[10][10], *p[10], *t;
    int i, j;

    printf("请输入十个英文名字：\n");
    for(i = 0;i < 10;i++)
    {
        p[i] = a[i];
    }
    for(i = 0;i < 10;i++)
    {
        gets(p[i]);
    }

    for(i = 0;i < 10;i++)
    {
        for(j = i + 1;j < 10;j++)
        {
            if(strcmp(p[i],p[j]) > 0)
            {
             t = p[i];
             p[i] = p[j];
             p[j] = t;
            }
        }
    }

    printf("排序后的结果为：\n");
    for(i = 0;i < 10;i++)
    {
        puts(p[i]);
    }

    return 0;
}
