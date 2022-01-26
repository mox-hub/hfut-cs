//***********************************************
//File name:0205
//Author:李浩然
//Date:2019.10.20
//Student ID:2019218211
//***********************************************
#include<stdio.h>
#include<math.h>

int main(void)
{
    float x, y, z, fDistance;
    float a, b, c;
    printf("请输入x,y,z:");
    scanf("%f%f%f", &x, &y, &z);

    fDistance = sqrt(x * x + y * y + z * z);
    a = acos(x/fDistance);
    b = acos(y/fDistance);
    c = acos(z/fDistance);

    printf("到原点的距离 = %f", fDistance);
    printf("与x轴的夹角= %f", a);
    printf("与y轴的夹角= %f", b);
    printf("与z轴的夹角= %f", c);

    return 0;
}

