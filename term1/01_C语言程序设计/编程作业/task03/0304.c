//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    float a[3];
    int i, j,temp;
    printf("请输入三个数:");
    for(i=0;i<3;i++)
        scanf("%f",&a[i]);
    for(i=0;i<3;i++)
        for(j=0;j<3-i-1;j++)
            if(a[i]<a[i+1])
            {
                temp = a[i];
                a[i] = a[i+1];
                a[i+1] = temp;
            }
    printf("max = %f,min = %f",a[0],a[2]);

    return 0;
}

