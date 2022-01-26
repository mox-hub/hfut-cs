#include<stdio.h>

int main(void)
{
    float fScore[10], fAverage = 0.0f;
    int i;

    printf("请输入十位学生的成绩:\n");
    for(i = 0;i < 10;i++)
    {
        scanf("%f", &fScore[i]);
    }
    for(i = 0;i < 10;i++)
    {
        fAverage += fScore[i];
    }
    fAverage /= 10;
    printf("平均分为:%.1f", fAverage);

    printf("超出平均分的学号为:\n");
    for(i = 0;i < 10;i++)
    {
        if(fScore[i] > fAverage)
            printf("%d\n", i + 1);
    }

    return 0;
}
