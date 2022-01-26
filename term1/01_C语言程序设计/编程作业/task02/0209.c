//***********************************************
//File name:0209
//Author:2019.10.20
//Date:李浩然
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main(void)
{
   float a, b, c, d = 0, fAmount;
   printf("请输入存款本金:");
   scanf("%f", &a);
   printf("请输入存款年利率:");
   scanf("%f", &b);
   printf("请输入存款年限:");
   scanf("%f", &c);

   fAmount = a * (b + 1) * c * d;

    printf("本金和利息的总和为%f", fAmount);

    return 0;

}
