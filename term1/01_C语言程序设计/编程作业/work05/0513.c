//****************************************
//File Name:0513.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<string.h>

int main(void)
{
    char str[80] = {0}, str2[80] = {0}, x;
    int i, iLocation;

    printf("请输入字符串:");
    gets(str);
    printf("请输入插入字符:");
    scanf("%c", &x);
    printf("请输入下标:");
    scanf("%d", &iLocation);

    for(i = 0;i < iLocation;i++)
    {
        str2[i] = str[i];
    }
    str2[iLocation] = x;
    for(i = iLocation + 1;i < strlen(str) + 1;i++)
    {
        str2[i] = str[i - 1];
    }

    puts(str2);

    return 0;
}

