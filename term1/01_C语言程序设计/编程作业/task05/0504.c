//****************************************
//File Name:0504.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<string.h>
int main(void)
{
    char str[80];
    int i;

    printf("ÇëÊäÈë×Ö·û´®:");
    gets(str);

    for(i = 0;i < strlen(str);i++)
    {
        if(str[i] >= 'a' &&str[i] <= 'z')
            str[i] -= 32;
         else if(str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
    }

    puts(str);

    return 0;
}
