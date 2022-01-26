//***********************************************
//File name:0211
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main(void)
{
    int a, b, c, d, e;
    printf("请输入一个四位数:");
    scanf("%d", &a);

    b = a % 10;
    c = (a / 10) % 10;
    d = (a / 100) % 10;
    e = a / 1000;

    printf("个位为:%d\n", b);
    printf("十位为:%d\n", c);
    printf("百位为:%d\n", d);
    printf("千位为:%d\n", e);

    return 0;

}
