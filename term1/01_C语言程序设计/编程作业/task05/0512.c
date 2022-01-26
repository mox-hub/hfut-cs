//****************************************
//File Name:0512.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<string.h>

int main(void)
{
    char str[10][10], str1[10];
    int i;

    printf("请输入十个名字：\n");
    for(i = 0;i < 10;i++)
    {
        gets(str[i]);
    }
    printf("请输入要查找的名字：\n");
    gets(str1);

    for(i = 0;i <  10;i++)
    {
        if(strcmp(str[i],str1) == 0)
        {
            printf("存在，他的位置是第%d位", i+1);
            break;
        }
    }
    if(i == 10)
        printf("未查找到!");

    return 0;

}
