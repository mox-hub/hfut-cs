//****************************************
//File Name:0802
//Author:Li Haoran
//Date:2019.12.11
//Student ID:2019218211
//****************************************
#include<stdio.h>
#include<stdlib.h>

void *Min(float **array, float *min,int row, int col);

int main(void)
{
    int row = 0, col = 0;
    float **array = NULL, min[3]={0};
    int i, j;

    printf("请输入行数:\n");
    scanf("%d", &row);
    printf("请输入列数:\n");
    scanf("%d", &col);

    array = (float**)malloc(sizeof(float*)*row);
    for(i = 0;i < row;i++)
    {
        array[i] = (float*)malloc(sizeof(float)*col);
    }

    for(i = 0;i < row;i++)
    {
        for(j = 0;j < col;j++)
        {
            array[i][j] = rand();
        }
    }//random
    for(i = 0;i < row;i++)
    {
        for(j = 0;j < col;j++)
        {
            printf("%f\t", array[i][j]);
        }
        printf("\n");
    }//show
    Min(array,min,row,col);
    //show the min
    printf("\n最小值为:%f\n", min[0]);
    printf("行数为:%f\n", min[1]);
    printf("列数为:%f\n", min[2]);

    return 0;
}
void *Min(float **array, float *min,int row,int col)
{
    int i = 0, j = 0;
    min[0] = array[0][0];
    for(i = 0;i < row;i++)
    {
        for(j = 0;j < col;j++)
        {
            if(array[i][j] < min[0])
            {
                min[0] = array[i][j];
                min[1] = i;//min row
                min[2] = j;//min col
            }
        }
    }
}
