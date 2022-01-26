//***********************************************
//File name:0212
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main (void)
{
    float a;
    printf("请输入a的值:");
    scanf("%f", &a);

    if(a < 0)
        printf("%.2f为负数", a);
    else if(a > 0)
        printf("%.2f为正数", a);
    else
        printf("%.2f为0", a);

    return 0;
}
