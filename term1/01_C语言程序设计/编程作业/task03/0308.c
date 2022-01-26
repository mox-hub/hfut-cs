//****************************************
//File Name:
//Author:
//Date:
//Student ID:
//****************************************
#include<stdio.h>

int main(void)
{
    float fSum, fA, fB, fC, fD;
    printf("请分别输入听,说,读,写的成绩:\n");
    scanf("%f%f%f%f", &fA, &fB, &fC, &fD);

    fSum = (fA + fB + fC + fD)/4;
    if(fA<5.00f||fB<5.00f||fC<5.00f||fD<5.00f)
        printf("您的雅思成绩未合格！");
    else
    {
        //计算总数
        if(fSum - (int)fSum >= 0.50f)
            fSum ==(int)fSum;
        else if(fSum - (int)fSum >= 0.25f)
            fSum == (int)fSum + 0.50f;

        if(fSum >=6.00f)
            printf("恭喜您，您的雅思成绩合格了！");
        else
            printf("您的雅思成绩未合格！");
    }
    return 0;
}
