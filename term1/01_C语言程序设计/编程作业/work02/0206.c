//***********************************************
//File name:0206
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main(void)
{
    float a, b, c, d, fAverange;
    printf("请输入四科成绩：\n");
    scanf("%f%f%f%f", &a, &b, &c, &d);

    fAverange = (a + b + c + d)/4;

    printf("四科成绩的平均值是：%.2f", fAverange);

    return 0;
}
