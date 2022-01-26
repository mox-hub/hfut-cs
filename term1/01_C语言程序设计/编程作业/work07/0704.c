//****************************************
//File Name:0704
//Author:Li Haoran
//Date:2019.11.23
//Student ID:2019218211
//****************************************
#include<stdio.h>

void sort(int num[],int n);

int main(void)
{
    int num[80], i, n;

    printf("请输入要输入的个数\n");
    scanf("%d", &n);
    printf("请输入数组:\n");
    for(i = 0;i < n;i++)
    {
        scanf("%d", &num[i]);
    }

    sort(num, n);

    printf("排序后的结果为:\n");
    for(i = 0;i < n;i++)
    {
        printf("%d\n", num[i]);
    }

    return 0;

}

void sort(int num[],int n)
{
    int i, j, temp;
    for(i = 0;i < n;i++)
    {
        for(j = i + 1;j < n;j++)
        {
            if(num[i] < num[j])
            {
                temp = num[i];
                num[i] = num[j];
                num[j] = temp;
            }
        }
    }
}
