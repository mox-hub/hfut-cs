//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    int a, i;
    printf("Please input an intager:");
    scanf("%d", &a);

    for(i = 10; i > 0;i--)
    {
        printf("%d", a % 10);
        a /= 10;
        if(a == 0)
            break;

    }

    return 0;
}
