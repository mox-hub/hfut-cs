//***********************************************
//File name:02
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>

int main(void)
{
    int a;
    printf("请输入年份:");
    scanf("%d", &a);

    if(a % 100 == 0)
    {
        if(a % 400 == 0)
            printf("%d是闰年", a);
        else
            printf("%d是平年", a);
    }
    else
    {
        if(a % 4 == 0)
            printf("%d是闰年", a);
        else
            printf("%d是平年", a);
    }
    return 0;

}
