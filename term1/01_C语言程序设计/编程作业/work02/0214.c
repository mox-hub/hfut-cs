//***********************************************
//File name:0214
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main(void)
{
    char a;
    printf("请输入字符:\n");
    scanf("%c", &a);

    if(a >= 'a' && a <= 'z')
        printf("%c是小写字母", a);
    else if(a >= 'A' && a < 'a')
        printf("%c是大写字母", a);
    else if(a >= '0' && a <= '9')
        printf("%c是数字", a);
    else
        printf("%c是其他字符", a);
}
