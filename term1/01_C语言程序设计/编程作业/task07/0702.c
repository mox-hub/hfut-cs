//****************************************
//File Name:0702
//Author:Li Haoran
//Date:2019.11.23
//Student ID:2019218211
//****************************************
#include<stdio.h>

int CommonDivisor(int a,int b);

int main(void)
{
    int a, b, result;

    printf("请输入第一个整数:\n");
    scanf("%d", &a);
    printf("请输入第二个整数:\n");
    scanf("%d", &b);

    result = CommonDivisor(a, b);

    if(result == -1)
        printf("输入有误!");
    else
        printf("%d和%d的最大公约数为%d\n", a, b, result);

    return 0;
}

int CommonDivisor(int a,int b)
{
    int c, temp;

    if(a <= 0 || b <= 0)
        return(-1);

    if(b > a)
    {
        temp = a;
        a = b;
        b = temp;
    }
    do
    {
        c = a % b;
        a = b;
        b = c;
    }while(c != 0);

    return(a);

}
