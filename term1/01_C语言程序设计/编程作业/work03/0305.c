//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    float a, b, c;
    printf("请输入三角形三边:");
    scanf("%f%f%f", &a, &b, &c);
    if(a+b<=c||b+c<=a||c+a<=b)
        printf("无法组成三角形！");
    else if(a*a==b*b+c*c||b*b==a*a+c*c||c*c==a*a+b*b)
        printf("该三角形是直角三角形");
    else if(a==b==c)
        printf("三角形是等腰三角形");
    else if(a==b||a==c||c==a)
        printf("三角形是等边三角形");
    else
        printf("三角形是不规则三角形");

    return 0;

}

