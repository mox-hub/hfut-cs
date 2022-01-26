//****************************************
//File Name:06
//Author:Li Haoran
//Date:20199.11.14
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    float a[10], *p, *q = NULL,t;
    p = a;

    printf("请输入十个随机数:\n");
    for(p = a;p < a + 10;p++)
    {
        scanf("%f",p);
    }

    for(p = a;*p;p++)
    {
        for(q = p + 1;q < a + 10;q++)
        {
            if(*p > *q)
            {
                t = *p;
                *p = *q;
                *q = t;
            }
        }
    }
    printf("排序后为：\n");
    for(p = a;p < a + 10;p++)
    {
        printf("%f\n",*p);
    }

    return 0;
}
