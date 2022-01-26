//****************************************
//File Name:0709
//Author:Li Haoran
//Date:2019.11.23
//Student ID:2019218211
//****************************************
#include"traingle.h"

int main(void)
{
    int a, b;
    double result, ra, rb;
    printf("请输入sin的度数:\n");
    scanf("%d", &a);
    printf("请输入cos的度数:\n");
    scanf("%d", &b);

    ra = sinX(a);
    rb = cosX(b);
    result = ra + rb;

    printf("sin%d = %f\n", a, ra);
    printf("cos%d = %f\n", b, rb);
    printf("结果为%f\n", result);

    return 0;

}
