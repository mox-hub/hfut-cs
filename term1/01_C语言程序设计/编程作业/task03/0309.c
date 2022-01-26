//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    int i, iNum = 0;
    printf("请按回车开始");
    getchar();
    //第一种for循环
    for(i = 0;i < 10000;i++)
    {
        if(i % 3 == 0)
            iNum += i;
    }
    printf("for循环的结果是:%d\n", iNum);

    //第二种while循环
    i = 0;
    iNum = 0;
    while(i <
           10000)
    {
        if(i % 3 == 0)
            iNum += i;
        i++;
    }
    printf("while循环的结果是:%d\n", iNum);

    //第三种do while循环
    i = 0;
    iNum = 0;
    do{
        if(i % 3 == 0)
            iNum += i;
        i++;
    }while(i < 10000);
    printf("do while循环的结果是:%d\n", iNum);

    return 0;
}
