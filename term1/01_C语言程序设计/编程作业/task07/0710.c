//****************************************
//File Name:0708
//Author:Li Haoran
//Date:2019.11.23
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<stdlib.h>

void statistics(char *p, int iCount[4]);

int main(void)
{
    char str[80];
    int iCount[4] = {0};

    printf("请输入字符串:\n");
    gets(str);

    statistics(str,iCount);
    printf("大写字母有%d个\n", iCount[0]);
    printf("小写字母有%d个\n", iCount[1]);
    printf("数字有%d个\n", iCount[2]);
    printf("其他字符有%d个\n", iCount[3]);

    return 0;
}

void statistics(char *p, int iCount[4])
{
    for(;*p;p++)
    {
        if(*p >= 'A' && *p <= 'Z')
            iCount[0] += 1;
        else if(*p >= 'a' && *p <= 'z')
            iCount[1] += 1;
        else if(*p >='0' && *p <= '9')
            iCount[2] += 1;
        else
            iCount[3] += 1;
    }
}
