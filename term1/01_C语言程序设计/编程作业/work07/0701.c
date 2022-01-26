//****************************************
//File Name:0701
//Author:Li Haoran
//Date:2019.11.23
//Student ID:2019218211
//****************************************
#include<stdio.h>

long long int factorial(int n);

int main(void)
{
    int n;
    long long int result;

    printf("请输入整数:\n");
    scanf("%d", &n);

    result = factorial(n);
    if(result == -1)
        printf("输入的参数有误！\n");
    else
        printf("%d的阶乘是:%lld\n", n, result);

    return 0;
}

long long int factorial(int n)
{
    int i;
    long long int result = 1;

    if(n < 0)
        return(-1);
    else if(n == 0)
        return 1;
    else
    {
        for(i = 1;i <= n;i++)
        {
            result *= i;
        }
        return(result);
    }

}
