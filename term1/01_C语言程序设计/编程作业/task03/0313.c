//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    float e, i, n, fPrecision, fItem;
    e =0;
    fPrecision = 1e-6;
    fItem = 1.0f;

    printf("请输入项数(如果使用自带精度请输入0):");
    scanf("%f", &n);

    if(n == 0)
    {
        for(i = 1;fItem >= fPrecision; i++)
        {
            fItem *= 1 / i;
            e += fItem;
        }
    }
    else
    {
        for(i = 1;i < n;i++)
        {
            fItem *= 1 / i;
            e += fItem;
        }
    }
    e += 1;
    printf("e的值为:%12f", e);

    return 0;
}
