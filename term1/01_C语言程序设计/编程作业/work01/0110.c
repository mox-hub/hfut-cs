/* example1-10 */
#include<stdio.h>
#include<math.h>
#define PI 3.1415

int main(void)
{
    float r, fArea;
    printf("请输入半径：");
    scanf("%f", &r);

    fArea = PI * r * r;

    printf("圆的面积为：%f", fArea);

    return 0;
}





