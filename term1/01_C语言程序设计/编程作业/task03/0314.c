//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{

    int i, j, iSum;
    iSum = 0;

    for(i = 1;i < 1000;i++)
    {
        for(j = 1; j < i;j++)
        {
            if((i % j) == 0)
                iSum += j;
        }
        if(iSum == i)
            printf("%d\n", i);
        iSum = 0;

    }
    return 0;
}
