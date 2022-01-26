//****************************************
//File Name:0707
//Author:Li Haoran
//Date:2019.11.23
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<stdlib.h>

void Transpose(int **p, int row, int col);

int main(void)
{
    int row = 0, col = 0;
    int **array = NULL;
    int i, j;

    printf("请输入行数:\n");
    scanf("%d", &row);
    printf("请输入列数:\n");
    scanf("%d", &col);

    if(row != col)
    {
        printf("行数必须等于列数！\n");
        return 0;
    }
    array = (int**)malloc(sizeof(int*)*row);
    for(i = 0;i < row;i++)
    {
        array[i] = (int*)malloc(sizeof(int)*col);
    }

    printf("请输入数组元素:\n");
    for(i = 0;i < row;i++)
    {
        for(j = 0;j < col;j++)
        {
            scanf("%d", &array[i][j]);
        }
    }
    printf("原矩阵为:\n");
    for(i = 0;i < row;i++)
    {
        for(j = 0;j < col;j++)
        {
            printf("%d\t", array[i][j]);
        }
        printf("\n");
    }
    printf("转制后为:\n");
    Transpose(array, row, col);

    return 0;
}

void Transpose(int **p, int row, int col)
{
    int i, j, temp;
    for(i = 0;i < row;i++)
    {
        for(j = 0;j < i;j++)
        {
            temp = p[i][j];
            p[i][j] = p[j][i];
            p[j][i] = temp;
        }
    }
    for(i = 0;i < row;i++)
    {
        for(j = 0;j < col;j++)
        {
            printf("%d\t", p[i][j]);
        }
        printf("\n");
    }

}
