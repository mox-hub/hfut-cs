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
    char str1[80], str2[80];
    char *p, *q;
    p = str1;
    q = str2;

    printf("请输入第一个字符串:\n");
    gets(str1);
    printf("请输入第二个字符串:\n");
    gets(str2);

    while(*p)
    {
        p++;
    }
    while(*p++ = *q++);

    printf("%s",str1);

    return 0;
}
