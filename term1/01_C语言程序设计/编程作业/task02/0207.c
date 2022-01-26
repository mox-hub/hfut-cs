//***********************************************
//File name:0207
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main(void)
{
    int a;
    printf("请输入0~9中的任意一位数字：");
    scanf("%d", &a);
    a = a + 32;
    printf("它对应的ASCII码是：%d", a);

    return 0;
}
