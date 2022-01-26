//****************************************
//File Name:0510.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<string.h>

int main(void)
{
    char str[80];
    int i, iFlag = 0;

    printf("请输入字符串:");
    gets(str);

    for(i = 0;i < (strlen(str) / 2) - 1;i++)
    {
        if(str[i] != str[strlen(str)-i-1])
        {
            printf("该字符串不是回文\n");
            iFlag = 1;
            break;
        }
    }
    if(iFlag == 0)
        printf("该字符串是回文\n");

    return 0;
}
