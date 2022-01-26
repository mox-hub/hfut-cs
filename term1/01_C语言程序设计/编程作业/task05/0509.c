//****************************************
//File Name:0509.c
//Author:Li Haoran
//Date:2019.11.08
//Student ID:2019218211
//****************************************
#include<stdio.h>

int main(void)
{
    int iNum[10], i,num;
    int high, low, mid;
    high = 0;
    low = 9;

    printf("请输入10个有序递增整形数据:\n");
    for(i = 0;i < 10;i++)
    {
        scanf("%d", &iNum[i]);
    }
    printf("请输入要查找的数据:\n");
    scanf("%d", &num);

    while(low <= high)
    {
        mid = (low + high) / 2;
        if(iNum[mid] == num)
        {
             printf("找到!是第%d位数字", i+1);
             break;
        }
        else if(num < iNum[mid])
            low = mid - 1;
        else
            high = mid + 1;
    }
    if(low > high)
        printf("没有找到！");

    return 0;

}
