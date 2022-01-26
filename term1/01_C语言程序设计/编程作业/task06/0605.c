//****************************************
//File Name:06
//Author:Li Haoran
//Date:20199.11.14
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
     int a[3][4], b[3][4], *p, *q;
     int i, j;

     printf("请输入第一个矩阵:\n");
     for(i = 0;i < 3;i++)
     {
         for(j = 0;j < 4;j++)
         {
             scanf("%d", &a[i][j]);
         }
     }
     printf("请输入第二个矩阵:\n");
     for(i = 0;i < 3;i++)
     {
         for(j = 0;j < 4;j++)
         {
             scanf("%d", &b[i][j]);
         }
     }

     for(i = 0;i < 3;i ++)
     {
         for(j = 0;j < 4;j++)
         {
             a[i][j] += b[i][j];
         }
     }

     printf("加和后的结果为:\n");

     for(i = 0;i < 3;i++)
     {
         for(j = 0;j < 4;j++)
         {
             printf("%d\t", a[i][j]);
         }
         printf("\n");
     }

}
