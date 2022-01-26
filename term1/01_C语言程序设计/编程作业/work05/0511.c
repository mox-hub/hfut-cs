//****************************************
//File Name:0211.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    char str[80], x;
    int i, iNum = 0;

    printf("请输入字符串:\n");
    gets(str);
    printf("请输入查找字符:\n");
    scanf("%c", &x);

    for(i = 0;str[i] != '\0';i++)
    {
       if(str[i] == x)
            iNum += 1;
    }
    if(iNum == 0)
        printf("没有找到!\n");
    else
        printf("%c在字符串中出现了%d次", x, iNum);

    return 0;

}
