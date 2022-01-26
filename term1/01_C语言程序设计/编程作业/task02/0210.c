//***********************************************
//File name:0210
//Author:李浩然
//Date:2019.10.20
//Student ID:李浩然
//***********************************************
#include<stdio.h>

int main(void)
{
    int a, b, temp;
    printf("请输入a,b:");
    scanf("%d%d", &a, &b);

    temp = a;
    a = b;
    b = temp;

    printf("a = %d\nb = %d\n", a, b);;

    return 0;

}
