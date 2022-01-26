//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    int i, j;
    for(i = 1;i <= 5;i++)
    {
        for(j = 4;j >= i;j--)
            printf("  ");
        for(j = 1;j < 2*i;j++)
            printf(" *");
        printf("\n");
    }
     for(i = 6;i < 10;i++)
    {
        for(j = 1;j <= i-5;j++)
            printf("  ");
        for(j = 1;j <= 9-2*(i-5);j++)
            printf(" *");
        printf("\n");
    }
    return 0;
}
