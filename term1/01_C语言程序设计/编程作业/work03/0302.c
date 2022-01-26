//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    int iMonth;
    do
    {
        printf("请输入月份:");
        scanf("%d", &iMonth);
        if(iMonth<1||iMonth>12)
            printf("输入错误！");
    }while(iMonth<1||iMonth>12)

    switch(iMonth)
        case 1:printf("它的英文是January");break;
        case 2:printf("它的英文是February");break;
        case 3:printf("它的英文是January");break;
        case 4:printf("它的英文是January");break;
        case 5:printf("它的英文是January");break;
        case 6:printf("它的英文是January");break;
        case 7:printf("它的英文是January");break;
        case 8:printf("它的英文是January");break;
        case 9:printf("它的英文是January");break;
        case 10:printf("它的英文是January");break;
        case 11:printf("它的英文是January");break;
        case 12:printf("它的英文是January");break;


    return 0;

}

