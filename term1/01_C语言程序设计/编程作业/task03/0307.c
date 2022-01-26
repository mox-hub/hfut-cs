//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    int iYear, iMonth, iDay;
    printf("请输入年份:");
    scanf("%d", &iYear);
    printf("请输入月份:");
    scanf("%d", &iMonth);

    if(iMonth == 2)
    {
        if(iYear%100==0&&iYear%400!=0||iYear%4==0)
            printf("该月有29天");
        else
            printf("该月有28天");
    }
    else
    {
        if(iMonth==4||iMonth==6||iMonth==9||iMonth==11)
            printf("该月有30天\n");
        else
            printf("该月有31天\n");
    }


    return 0;
}
