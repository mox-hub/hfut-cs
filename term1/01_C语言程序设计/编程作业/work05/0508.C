//****************************************
//File Name:0508.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    int iNum[10], i,num;

    printf("请随机输入10个整形数据:\n");
    for(i = 0;i < 10;i++)
    {
        scanf("%d", &iNum[i]);
    }
    printf("请输入要查找的数据:\n");
    scanf("%d", &num);

    for(i = 0;i < 10;i++)
    {
        if(iNum[i] == num)
        {
            printf("找到!是第%d位数", i+1);
            break;
        }
    }
    if(i == 10)
    {
        printf("未找到！");
    }

    return 0;

}
