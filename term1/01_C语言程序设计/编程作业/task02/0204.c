//***********************************************
//File name:0204
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>
#define PI 3.14159

int main(void)
{
    float fAngle, r, fArea;
    printf("请输入度数和半径：");
    scanf("%f%f", &fAngle, &r);

    fArea = (fAngle / 360) * PI * r * r;

    printf("扇形的面积是:%f", fArea);

    return 0;
}
