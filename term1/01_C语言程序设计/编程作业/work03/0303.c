//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>
int main(void)
{
    int a, b, c, max, mid, min;
    printf("ÇëÊäÈëa,b,c:");
    scanf("%f%f%f", &a, &b, &c);
    max = a;
    if(b>a)
        max = b;
    if(c>a)
        max = c;
    min = c;
    if(a<c)
        min = a;
    if(b<c)
        min = b;
    printf("max = %d\n min = %d", max, min);


    return 0;

}

