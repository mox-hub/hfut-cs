//****************************************
//File Name:0502.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    float fScore[10];
    int i, iNum[5] = {0};

    printf("请输入十位学生的成绩:\n");
    for(i = 0;i < 10;i++)
    {
        scanf("%f", &fScore[i]);
    }
    for(i = 0;i < 10;i++)
    {
        if(fScore[i] > 100)
            printf("第%d位成绩输入错误！\n", i);
        else if(fScore[i] >= 90)
            iNum[0] += 1;
        else if(fScore[i] >= 80)
            iNum[1] += 1;
        else if(fScore[i] >= 70)
            iNum[2] += 1;
        else if(fScore[i] >= 60)
            iNum[3] += 1;
        else
            iNum[4] += 1;
    }

    for(i = 0;i < 5;i++)
    {
        printf("%d到%d的人数为%d\n",100 - 10 * i, 10 * (10 - i - 1), iNum[i]);
    }

    return 0;
}
