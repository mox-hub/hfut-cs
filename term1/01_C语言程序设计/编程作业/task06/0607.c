//****************************************
//File Name:06
//Author:Li Haoran
//Date:20199.11.14
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    char str[80], a, *p;

    printf("请输入字符串:\n");
    gets(str);
    printf("请输入要删去的字符:\n");
    scanf("%c", &a);

    for(p = str;*p;p++)
    {
        if(*p != a)
        {
            printf("%c", *p);
        }
    }

    return 0;
}
